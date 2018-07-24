/****************************************************************************/
/*              Beebem - (c) David Alan Gilbert 1994                        */
/*              ------------------------------------                        */
/* This program may be distributed freely within the following restrictions:*/
/*                                                                          */
/* 1) You may not charge for this program or for any part of it.            */
/* 2) This copyright message must be distributed with all copies.           */
/* 3) This program must be distributed complete with source code.  Binary   */
/*    only distribution is not permitted.                                   */
/* 4) The author offers no warrenties, or guarentees etc. - you use it at   */
/*    your own risk.  If it messes something up or destroys your computer   */
/*    thats YOUR problem.                                                   */
/* 5) You may use small sections of code from this program in your own      */
/*    applications - but you must acknowledge its use.  If you plan to use  */
/*    large sections then please ask the author.                            */
/*                                                                          */
/* If you do not agree with any of the above then please do not use this    */
/* program.                                                                 */
/* Please report any problems to the author at beebem@treblig.org           */
/****************************************************************************/
/* Video handling -          David Alan Gilbert */

/* Version 2 - 24/12/94 - Designed to emulate start of frame interrupt
   correctly */

/* Mike Wyatt 7/6/97 - Added cursor display and Win32 port */

/* Richard Gellman 4/2/2001 AAAARGH SHADOW RAM! HELP! */




#define CHAR_WIDTH 8 //12
#define CHAR_HEIGHT 9 //20





//--#if HAVE_CONFIG_H
//--#	include <config.h>
//--#endif

//--#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <stdio.h>

//--#include "6502core.h"
//--#include "beebmem.h"
//--#include "beebwin.h"
//--#include "main.h"
//--#include "sysvia.h"
//--#include "video.h"
//--#include "uefstate.h"
//--#include "beebsound.h"
//--#include "debug.h"
//--#include "teletext.h"


//++
#include <iostream>
#include <stdlib.h>
#include <SDL.h>

#include "6502core.h"
#include "beebmem.h"
#include "beebwin.h"
#include "main.h"
#include "mode7font.h"
#include "sysvia.h"
#include "video.h"

#include "beebconfig.h"
#include "beebconfig_data.h"

#include "uefstate.h"

#include "virtualkeyboard.h"

#ifdef BEEB_DOTIME
#include <sys/times.h>
#ifdef SUNOS
#include <sys/param.h>
#endif
#ifdef HPUX
#include <unistd.h>
#endif
#endif


/* Handle SDL surface locking:
 */
extern SDL_Surface *frame_buffer_p;
#define LOCK(s)   {if(SDL_MUSTLOCK(s))(void)SDL_LockSurface(s);}
#define UNLOCK(s) {if(SDL_MUSTLOCK(s))(void)SDL_UnlockSurface(s);}
//<----


//++
unsigned int FrameCount=0;
//<-


//++
int IsTeletext=0;
//<-



#ifdef BEEB_DOTIME
#include <sys/times.h>
#ifdef SUNOS
#include <sys/param.h>
#endif
#ifdef HPUX
#include <unistd.h>
#endif
#endif

//++
#include "sdl.h"
//#include "user_config.h"
//<+

using namespace std;

/* Bit assignments in control reg:
   0 - Flash colour (0=first colour, 1=second)
   1 - Teletext select (0=on chip serialiser, 1=teletext)
 2,3 - Bytes per line (2,3=1,1 is 80, 1,0=40, 0,1=20, 0,0=10)
   4 - CRTC Clock chip select (0 = low frequency, 1= high frequency)
 5,6 - Cursor width in bytes (0,0 = 1 byte, 0,1=not defined, 1,0=2, 1,1=4)
   7 - Master cursor width (if set causes large cursor)
*/
EightUChars FastTable[256];
SixteenUChars FastTableDWidth[256]; /* For mode 4,5,6 */
int FastTable_Valid=0;

NarrowResolution FastTableSDL[256];
WideResolution FastTableDWidthSDL[256]; /* For modes 4, 5 and 6 */


typedef void (*LineRoutinePtr)(void);
LineRoutinePtr LineRoutine;

/* Translates middle bits of VideoULA_ControlReg to number of colours */
static int NColsLookup[]={16, 4, 2, 0 /* Not supported 16? */, 0, 16, 4, 2}; /* Based on AUG 379 */

unsigned char VideoULA_ControlReg=0x9c;
unsigned char VideoULA_Palette[16];

unsigned char CRTCControlReg=0;
unsigned char CRTC_HorizontalTotal=127;     /* R0 */
unsigned char CRTC_HorizontalDisplayed=80;  /* R1 */
unsigned char CRTC_HorizontalSyncPos=98;    /* R2 */
unsigned char CRTC_SyncWidth=0x28;          /* R3 - top 4 bits are Vertical (in scan lines) and bottom 4 are horizontal in characters */
unsigned char CRTC_VerticalTotal=38;        /* R4 */
unsigned char CRTC_VerticalTotalAdjust=0;   /* R5 */
unsigned char CRTC_VerticalDisplayed=32;    /* R6 */
unsigned char CRTC_VerticalSyncPos=34;      /* R7 */
unsigned char CRTC_InterlaceAndDelay=0;     /* R8 - 0,1 are interlace modes, 4,5 display blanking delay, 6,7 cursor blanking delay */
unsigned char CRTC_ScanLinesPerChar=7;      /* R9 */
unsigned char CRTC_CursorStart=0;           /* R10 */
unsigned char CRTC_CursorEnd=0;             /* R11 */
unsigned char CRTC_ScreenStartHigh=6;       /* R12 */
unsigned char CRTC_ScreenStartLow=0;        /* R13 */
unsigned char CRTC_CursorPosHigh=0;         /* R14 */
unsigned char CRTC_CursorPosLow=0;          /* R15 */
unsigned char CRTC_LightPenHigh=0;          /* R16 */
unsigned char CRTC_LightPenLow=0;           /* R17 */


/* New hardware cursor variables:
 */
int CursorX = 0;
int CursorY = 0;
int CursorH = 0;
int CursorW = 0;
int CursorO = 0;





#define AdjustVideo NewAdjustVideo
void NewAdjustVideo();


unsigned int ActualScreenWidth=640;
int InitialOffset=0;
long ScreenAdjust=0; // Mode 7 Defaults.
long VScreenAdjust=0;
int VStart,HStart;
unsigned char HSyncModifier=9;
unsigned char TeletextEnabled;
char TeletextStyle=1; // Defines wether teletext will skip intermediate lines in order to speed up
int THalfMode=0; // 1 if to use half-mode (TeletextStyle=1 all the time)
int CurY=-1;
FILE *crtclog;

int ova,ovn; // mem ptr buffers

/* CharLine counts from the 'reference point' - i.e. the point at which we reset the address pointer - NOT
the point of the sync. If it is -ve its actually in the adjust time */
typedef struct {
  int Addr;       /* Address of start of next visible character line in beeb memory  - raw */
  int StartAddr;  /* Address of start of first character line in beeb memory  - raw */
  int PixmapLine; /* Current line in the pixmap */
  int FirstPixmapLine; /* The first pixmap line where something is visible.  Used to eliminate the 
                          blank virtical retrace lines at the top of the screen.  */
  int PreviousFirstPixmapLine; /* The first pixmap line on the previous frame */
  int LastPixmapLine; /* The last pixmap line where something is visible.  Used to eliminate the 
                          blank virtical retrace lines at the bottom of the screen.  */
  int PreviousLastPixmapLine; /* The last pixmap line on the previous frame */
  int IsTeletext; /* This frame is a teletext frame - do things differently */
  char *DataPtr;  /* Pointer into host memory of video data */

  int CharLine;   /* 6845 counts in characters vertically - 0 at the top , incs by 1 - -1 means we are in the bit before the actual display starts */
  int InCharLineUp; /* Scanline within a character line - counts up*/
  int VSyncState; // Cannot =0 in MSVC $NRM; /* When >0 VSync is high */
} VideoStateT;

static  VideoStateT VideoState;

int VideoTriggerCount=9999; /* Number of cycles before next scanline service */

/* first subscript is graphics flag (1 for graphics,2 for separated graphics), next is character, then scanline */
/* character is (valu &127)-32 */
static unsigned int EM7Font[3][96][20]; // 20 rows to account for "half pixels"

static int Mode7FlashOn=1; /* True if a flashing character in mode 7 is on */
static int Mode7DoubleHeightFlags[80]; /* Pessimistic size for this flags - if 1 then corresponding character on NEXT line is top half */
static int CurrentLineBottom=0;
static int NextLineBottom=0; // 1 if the next line of double height should be bottoms only

/* Flash every half second(?) i.e. 25 x 50Hz fields */
// No. On time is longer than off time. - according to my datasheet, its 0.75Hz with 3:1 ON:OFF ratio. - Richard Gellman
// cant see that myself.. i think it means on for 0.75 secs, off for 0.25 secs
#define MODE7FLASHFREQUENCY 25
#define MODE7ONFIELDS 37
#define MODE7OFFFIELDS 13
unsigned char CursorOnFields,CursorOffFields;
int CursorFieldCount=32;
unsigned char CursorOnState=1;
int Mode7FlashTrigger=MODE7ONFIELDS;

/* If 1 then refresh on every display, else refresh every n'th display */
int Video_RefreshFrequency=1;
/* The number of the current frame - starts at Video_RefreshFrequency - at 0 actually refresh */
static int FrameNum=0;



//++
int time_slept=0;
//<-



WideResolution* GetWideScanlineSDL(int y, int *BytesToGo);
NarrowResolution* GetNarrowScanlineSDL(int y, int *BytesToGo);


static void LowLevelDoScanLineNarrow();
static void LowLevelDoScanLineWide();
static void LowLevelDoScanLineNarrowNot4Bytes();
static void LowLevelDoScanLineWideNot4Bytes();

static void LowLevelDoScanLineNarrowSDL();
static void LowLevelDoScanLineNarrowNot4BytesSDL();
static void LowLevelDoScanLineWideSDL();
static void LowLevelDoScanLineWideNot4BytesSDL();



static void DoFastTable16SDL(void);
static void DoFastTable16XStep8SDL(void);
static void DoFastTable4SDL(void);
static void DoFastTable4XStep4SDL(void);
static void DoFastTable2SDL(void);
static void DoFastTable2XStep2SDL(void);

static int IsFlippedMode(void);




static void VideoAddCursor(void);
//$$ void AdjustVideo();
void VideoAddLEDs(void);


//+>
//## Just for now.. Hack to fudge scanline width in graphic/teletext code:
//## Dave Eggleston - will go when video support is rewritten.


//--static int screen_width;
//--static void set_screen_width(void)
//--{
//--//	screen_width = fullscreen;
//--
//--//	switch (fullscreen?cfg_Fullscreen_Resolution:cfg_Windowed_Resolution) {
//--	switch (mainWin->IsFullScreen()?cfg_Fullscreen_Resolution:cfg_Windowed_Resolution) {
//--	case RESOLUTION_640X512:
//--	case RESOLUTION_640X480_S:
//--	case RESOLUTION_640X480_V:
//--		screen_width=640;
//--		break;
//--	default:
//--		screen_width=320;
//--	}
//--}
//<+


// A 'Chunky Mode' get pixel routine:
static inline int GetPixel_8(SDL_Surface *surface_p, int x, int y)
{
	Uint8 *p;

	if (surface_p == NULL) return 0;
	if (x<0 || x>surface_p->w) return 0;
	if (y<0 || y>surface_p->h) return 0;
	if (surface_p->format->BytesPerPixel != 1) return 0;

	p = (Uint8*) surface_p->pixels+y*surface_p->pitch+x;
	return (int) *p;
}

// This does the same thing as the Windows Mode7 font loader
// but does it using .bmp files for easier editing.
static void BuldMode7Font_8x9(void) {

	SDL_Surface *f_p;
	unsigned int m7cc, m7cy, m7cx, m7cr, p;

	f_p = SDL_LoadBMP(config.files.teletext_font_loc);
	if (f_p == NULL) {
		fprintf(stderr, "Couldn't open teletext font [%s].\n", config.files.teletext_font_loc);
		exit(10);
	}

	for(m7cc=0; m7cc<96; m7cc++) {
		for(m7cy=0; m7cy<9; m7cy++) {
			m7cr=0;
			for(m7cx=0; m7cx<8; m7cx++) {
				if ( (p=GetPixel_8(f_p, m7cx, m7cy+(9*m7cc) /* *(m7cc*9) */))==1) {
					printf("*");

#ifdef GEKKO
					// No reversed text please.
					m7cr|=(1<<(8-m7cx));
#else

					if (SDL_BYTEORDER == SDL_LIL_ENDIAN)
						m7cr|=(1<<(8-m7cx));
					else
						m7cr|=(1<<(m7cx));
#endif

				} else {
					printf("_");
				}
			}

			printf("\t%X\n", m7cr);

			EM7Font[0][m7cc][m7cy]=m7cr;
			EM7Font[1][m7cc][m7cy]=m7cr;
			EM7Font[2][m7cc][m7cy]=m7cr;
		}
		
		printf("\n");
	}


 unsigned int row1,row2,row3; // row builders for mode 7 graphics


  // Now fill in the graphics - this is built from an algorithm, but has certain lines/columns
  // blanked for separated graphics.
  for (m7cc=0;m7cc<96;m7cc++) {
	  // here's how it works: top two blocks: 1 & 2
	  // middle two blocks: 4 & 8
	  // bottom two blocks: 16 & 64
	  // its only a grpahics character if bit 5 (32) is clear.
	  if (!(m7cc & 32)) {
		  row1=0; row2=0; row3=0;
		  // left block has a value of 4032, right 63 and both 4095
//		  if (m7cc & 1) row1|=4032;
//		  if (m7cc & 2) row1|=63;
//		  if (m7cc & 4) row2|=4032;
//		  if (m7cc & 8) row2|=63;
//		  if (m7cc & 16) row3|=4032;
//		  if (m7cc & 64) row3|=63;

		if (m7cc & 1)  row1|=0xf0;
		if (m7cc & 2)  row1|=0x0f;
		if (m7cc & 4)  row2|=0xf0;
		if (m7cc & 8)  row2|=0x0f;
		if (m7cc & 16) row3|=0xf0;
		if (m7cc & 64) row3|=0x0f;


		  // now input these values into the array
		  // top row of blocks - continuous
//		  EM7Font[1][m7cc][0]=EM7Font[1][m7cc][1]=EM7Font[1][m7cc][2]=row1;
//		  EM7Font[1][m7cc][3]=EM7Font[1][m7cc][4]=EM7Font[1][m7cc][5]=row1;
		EM7Font[1][m7cc][0]=EM7Font[1][m7cc][1]=EM7Font[1][m7cc][2]=row1;
		  // Separated
		//row1&=975; // insert gaps
		row1&=0xEE;
		EM7Font[2][m7cc][0]=EM7Font[2][m7cc][1]=row1;
		EM7Font[2][m7cc][2]=0;
//		  EM7Font[2][m7cc][0]=EM7Font[2][m7cc][1]=EM7Font[2][m7cc][2]=row1;
//		  EM7Font[2][m7cc][3]=row1; EM7Font[2][m7cc][4]=EM7Font[2][m7cc][5]=0;


		  // middle row of blocks - continuous
//		  EM7Font[1][m7cc][6]=EM7Font[1][m7cc][7]=EM7Font[1][m7cc][8]=row2;
//		  EM7Font[1][m7cc][9]=EM7Font[1][m7cc][10]=EM7Font[1][m7cc][11]=row2;
//		  EM7Font[1][m7cc][12]=EM7Font[1][m7cc][13]=row2;
		EM7Font[1][m7cc][3]=EM7Font[1][m7cc][4]=EM7Font[1][m7cc][5]=row2;
		  // Separated
		//row2&=975; // insert gaps
		row2&=0xEE;
		EM7Font[2][m7cc][3]=EM7Font[2][m7cc][4]=row2;
		EM7Font[2][m7cc][5]=0;
//		  EM7Font[2][m7cc][6]=EM7Font[2][m7cc][7]=EM7Font[2][m7cc][8]=row2;
//		  EM7Font[2][m7cc][9]=EM7Font[2][m7cc][10]=EM7Font[2][m7cc][11]=row2;
//		  EM7Font[2][m7cc][12]=EM7Font[2][m7cc][13]=0;


		  // Bottom row - continuous
//		  EM7Font[1][m7cc][14]=EM7Font[1][m7cc][15]=EM7Font[1][m7cc][16]=row3;
//		  EM7Font[1][m7cc][17]=EM7Font[1][m7cc][18]=EM7Font[1][m7cc][19]=row3;
		EM7Font[1][m7cc][6]=EM7Font[1][m7cc][7]=EM7Font[1][m7cc][8]=row3;
		  // Separated
		//row3&=975; // insert gaps
		row3&=0xEE;
		EM7Font[2][m7cc][6]=EM7Font[2][m7cc][7]=row3;
		EM7Font[2][m7cc][8]=0;
//		  EM7Font[2][m7cc][14]=EM7Font[2][m7cc][15]=EM7Font[2][m7cc][16]=row3;
//		  EM7Font[2][m7cc][17]=row3; EM7Font[2][m7cc][18]=EM7Font[2][m7cc][19]=0;
	  } // check for valid char to modify
  } // character loop.





}

//static void BuildMode7Font_12x20(void) {
//}


/*-------------------------------------------------------------------------------------------------------------*/
//--static void BuildMode7Font(void) {
//--  FILE *m7File;
//--  unsigned char m7cc,m7cy;
//--  unsigned int m7cb;
//--  unsigned int row1,row2,row3; // row builders for mode 7 graphics
//--  char TxtFnt[256];
//--  /* cout <<"Building mode 7 font data structures\n"; */
//-- // Build enhanced mode 7 font
//--
//--	BuldMode7Font_8x9();
//--	return;
//--
//--
//--//////////////////////////////
//--  unsigned char *p = (unsigned char*) frame_buffer_p->pixels;
//--//////////////////////////////
//--
//--
//--//->
//--//--  strcpy(TxtFnt,RomPath);
//--//--  strcat(TxtFnt,"teletext.fnt");
//--//++
//--//	strcpy(TxtFnt, DATA_DIR);
//--//	strcat(TxtFnt, "/resources/teletext.fnt");
//--
//--	GetLocation_teletext(TxtFnt, 256);
//--	pINFO(dL"Loading teletext font: '%s'", dR, TxtFnt);
//--//<-
//--
//--  m7File=fopen(TxtFnt,"rb");
//--  if (m7File == NULL)
//--  {
//--    char errstr[200];
//--    sprintf(errstr, "Cannot open Teletext font file teletext.fnt");
//--    MessageBox(GETHWND,errstr,"BBC Emulator",MB_OK|MB_ICONERROR);
//--    exit(1);
//--  }
//--  for (m7cc=32;m7cc<=127;m7cc++) {
//--	  for (m7cy=0;m7cy<=17;m7cy++) {
//--		  m7cb=fgetc(m7File);
//--		  m7cb|=fgetc(m7File)<<8;
//--		  EM7Font[0][m7cc-32][m7cy+2]=m7cb<<2;
//--		  EM7Font[1][m7cc-32][m7cy+2]=m7cb<<2;
//--		  EM7Font[2][m7cc-32][m7cy+2]=m7cb<<2;
//--
//--	  		for(int i=0; i<16; i++) {
//--				int c = (m7cb >> (15-i)) & 1;
//--				printf("%c", c == 1 ? '*' : '_');
//--  			}
//--		  printf("\n");
//--	  }
//--	  EM7Font[0][m7cc-32][0]=EM7Font[1][m7cc-32][0]=EM7Font[2][m7cc-32][0]=0;
//--	  EM7Font[0][m7cc-32][1]=EM7Font[1][m7cc-32][1]=EM7Font[2][m7cc-32][1]=0;
//--
//--	EM7Font[0][m7cc-32][0]=EM7Font[0][m7cc-32][1];
//--	EM7Font[0][m7cc-32][1]=EM7Font[0][m7cc-32][3];
//--	EM7Font[0][m7cc-32][2]=EM7Font[0][m7cc-32][5];
//--	EM7Font[0][m7cc-32][3]=EM7Font[0][m7cc-32][7];
//--	EM7Font[0][m7cc-32][4]=EM7Font[0][m7cc-32][9];
//--	EM7Font[0][m7cc-32][5]=EM7Font[0][m7cc-32][11];
//--	EM7Font[0][m7cc-32][6]=EM7Font[0][m7cc-32][13];
//--	EM7Font[0][m7cc-32][7]=EM7Font[0][m7cc-32][15];
//--
//--  }
//--
//--  fclose(m7File);
//--  // Now fill in the graphics - this is built from an algorithm, but has certain lines/columns
//--  // blanked for separated graphics.
//--  for (m7cc=0;m7cc<96;m7cc++) {
//--	  // here's how it works: top two blocks: 1 & 2
//--	  // middle two blocks: 4 & 8
//--	  // bottom two blocks: 16 & 64
//--	  // its only a grpahics character if bit 5 (32) is clear.
//--	  if (!(m7cc & 32)) {
//--		  row1=0; row2=0; row3=0;
//--		  // left block has a value of 4032, right 63 and both 4095
//--		  if (m7cc & 1) row1|=4032;
//--		  if (m7cc & 2) row1|=63;
//--		  if (m7cc & 4) row2|=4032;
//--		  if (m7cc & 8) row2|=63;
//--		  if (m7cc & 16) row3|=4032;
//--		  if (m7cc & 64) row3|=63;
//--		  // now input these values into the array
//--		  // top row of blocks - continuous
//--		  EM7Font[1][m7cc][0]=EM7Font[1][m7cc][1]=EM7Font[1][m7cc][2]=row1;
//--		  EM7Font[1][m7cc][3]=EM7Font[1][m7cc][4]=EM7Font[1][m7cc][5]=row1;
//--		  // Separated
//--		  row1&=975; // insert gaps
//--		  EM7Font[2][m7cc][0]=EM7Font[2][m7cc][1]=EM7Font[2][m7cc][2]=row1;
//--		  EM7Font[2][m7cc][3]=row1; EM7Font[2][m7cc][4]=EM7Font[2][m7cc][5]=0;
//--		  // middle row of blocks - continuous
//--		  EM7Font[1][m7cc][6]=EM7Font[1][m7cc][7]=EM7Font[1][m7cc][8]=row2;
//--		  EM7Font[1][m7cc][9]=EM7Font[1][m7cc][10]=EM7Font[1][m7cc][11]=row2;
//--		  EM7Font[1][m7cc][12]=EM7Font[1][m7cc][13]=row2;
//--		  // Separated
//--		  row2&=975; // insert gaps
//--		  EM7Font[2][m7cc][6]=EM7Font[2][m7cc][7]=EM7Font[2][m7cc][8]=row2;
//--		  EM7Font[2][m7cc][9]=EM7Font[2][m7cc][10]=EM7Font[2][m7cc][11]=row2;
//--		  EM7Font[2][m7cc][12]=EM7Font[2][m7cc][13]=0;
//--		  // Bottom row - continuous
//--		  EM7Font[1][m7cc][14]=EM7Font[1][m7cc][15]=EM7Font[1][m7cc][16]=row3;
//--		  EM7Font[1][m7cc][17]=EM7Font[1][m7cc][18]=EM7Font[1][m7cc][19]=row3;
//--		  // Separated
//--		  row3&=975; // insert gaps
//--		  EM7Font[2][m7cc][14]=EM7Font[2][m7cc][15]=EM7Font[2][m7cc][16]=row3;
//--		  EM7Font[2][m7cc][17]=row3; EM7Font[2][m7cc][18]=EM7Font[2][m7cc][19]=0;
//--	  } // check for valid char to modify
//--  } // character loop.
//--}; /* BuildMode7Font */
/*-------------------------------------------------------------------------------------------------------------*/

























static void DoFastTable16(void) {
  unsigned int beebpixvl,beebpixvr;
  unsigned int bplvopen,bplvtotal;
  unsigned char tmp;

  for(beebpixvl=0;beebpixvl<16;beebpixvl++) {
    bplvopen=((beebpixvl & 8)?128:0) |
             ((beebpixvl & 4)?32:0) |
             ((beebpixvl & 2)?8:0) |
             ((beebpixvl & 1)?2:0);
    for(beebpixvr=0;beebpixvr<16;beebpixvr++) {
      bplvtotal=bplvopen |
             ((beebpixvr & 8)?64:0) |
             ((beebpixvr & 4)?16:0) |
             ((beebpixvr & 2)?4:0) |
             ((beebpixvr & 1)?1:0);
      tmp=VideoULA_Palette[beebpixvl];
      if (tmp>7) {
        tmp&=7;
        if (VideoULA_ControlReg & 1) tmp^=7;
      };
// [HERE]
      FastTable[bplvtotal].data[0]=FastTable[bplvtotal].data[1]=
        FastTable[bplvtotal].data[2]=FastTable[bplvtotal].data[3]=mainWin->cols[tmp];
//	FastTable[bplvtotal].data[0] = mainWin->cols[tmp];
//	FastTable[bplvtotal].data[1]=0;
//	FastTable[bplvtotal].data[2]=0;
//	FastTable[bplvtotal].data[3]=0;


      tmp=VideoULA_Palette[beebpixvr];
      if (tmp>7) {
        tmp&=7;
        if (VideoULA_ControlReg & 1) tmp^=7;
      };
// [HERE]
      FastTable[bplvtotal].data[4]=FastTable[bplvtotal].data[5]=
        FastTable[bplvtotal].data[6]=FastTable[bplvtotal].data[7]=mainWin->cols[tmp];
//	FastTable[bplvtotal].data[4]=mainWin->cols[tmp];
//	FastTable[bplvtotal].data[5]=0;
//	FastTable[bplvtotal].data[6]=0;
//	FastTable[bplvtotal].data[7]=0;



    }; /* beebpixr */
  }; /* beebpixl */

}; /* DoFastTable16 */

// [HERE] Mode 2
static void DoFastTable16SDL(void) {
  unsigned int beebpixvl,beebpixvr;
  unsigned int bplvopen,bplvtotal;
  unsigned char tmp;

  for(beebpixvl=0;beebpixvl<16;beebpixvl++) {
    bplvopen=((beebpixvl & 8)?128:0) |
             ((beebpixvl & 4)?32:0) |
             ((beebpixvl & 2)?8:0) |
             ((beebpixvl & 1)?2:0);
    for(beebpixvr=0;beebpixvr<16;beebpixvr++) {
      bplvtotal=bplvopen |
             ((beebpixvr & 8)?64:0) |
             ((beebpixvr & 4)?16:0) |
             ((beebpixvr & 2)?4:0) |
             ((beebpixvr & 1)?1:0);
      tmp=VideoULA_Palette[beebpixvl];
      if (tmp>7) {
        tmp&=7;
        if (VideoULA_ControlReg & 1) tmp^=7;
      };
	if ( IsFlippedMode() )
		FastTableSDL[bplvtotal].data[3]=FastTableSDL[bplvtotal].data[2]=mainWin->cols[tmp];
	else
		FastTableSDL[bplvtotal].data[0]=FastTableSDL[bplvtotal].data[1]=mainWin->cols[tmp];

      tmp=VideoULA_Palette[beebpixvr];
      if (tmp>7) {
        tmp&=7;
        if (VideoULA_ControlReg & 1) tmp^=7;
      };
	if ( IsFlippedMode() )
		FastTableSDL[bplvtotal].data[1]=FastTableSDL[bplvtotal].data[0]=mainWin->cols[tmp];
	else
		FastTableSDL[bplvtotal].data[2]=FastTableSDL[bplvtotal].data[3]=mainWin->cols[tmp];

    }; /* beebpixr */
  }; /* beebpixl */
}; /* DoFastTable16 */


/*-------------------------------------------------------------------------------------------------------------*/
static void DoFastTable16XStep8(void) {
  unsigned int beebpixvl,beebpixvr;
  unsigned int bplvopen,bplvtotal;
  unsigned char tmp;

  for(beebpixvl=0;beebpixvl<16;beebpixvl++) {
    bplvopen=((beebpixvl & 8)?128:0) |
             ((beebpixvl & 4)?32:0) |
             ((beebpixvl & 2)?8:0) |
             ((beebpixvl & 1)?2:0);
    for(beebpixvr=0;beebpixvr<16;beebpixvr++) {
      bplvtotal=bplvopen |
             ((beebpixvr & 8)?64:0) |
             ((beebpixvr & 4)?16:0) |
             ((beebpixvr & 2)?4:0) |
             ((beebpixvr & 1)?1:0);
      tmp=VideoULA_Palette[beebpixvl];
      if (tmp>7) {
        tmp&=7;
        if (VideoULA_ControlReg & 1) tmp^=7;
      };
// [HERE]
      FastTableDWidth[bplvtotal].data[0]=FastTableDWidth[bplvtotal].data[1]=
        FastTableDWidth[bplvtotal].data[2]=FastTableDWidth[bplvtotal].data[3]=
      FastTableDWidth[bplvtotal].data[4]=FastTableDWidth[bplvtotal].data[5]=
        FastTableDWidth[bplvtotal].data[6]=FastTableDWidth[bplvtotal].data[7]=mainWin->cols[tmp];
//	FastTableDWidth[bplvtotal].data[0]=mainWin->cols[tmp];
//	FastTableDWidth[bplvtotal].data[1]=0;
//	FastTableDWidth[bplvtotal].data[2]=0;
//	FastTableDWidth[bplvtotal].data[3]=0;
//	FastTableDWidth[bplvtotal].data[4]=0;
//	FastTableDWidth[bplvtotal].data[5]=0;
//	FastTableDWidth[bplvtotal].data[6]=0;
//	FastTableDWidth[bplvtotal].data[7]=0;
//
      tmp=VideoULA_Palette[beebpixvr];
      if (tmp>7) {
        tmp&=7;
        if (VideoULA_ControlReg & 1) tmp^=7;
      };
// [HERE]
      FastTableDWidth[bplvtotal].data[8]=FastTableDWidth[bplvtotal].data[9]=
        FastTableDWidth[bplvtotal].data[10]=FastTableDWidth[bplvtotal].data[11]=
      FastTableDWidth[bplvtotal].data[12]=FastTableDWidth[bplvtotal].data[13]=
        FastTableDWidth[bplvtotal].data[14]=FastTableDWidth[bplvtotal].data[15]=mainWin->cols[tmp];
//	FastTableDWidth[bplvtotal].data[8]=mainWin->cols[tmp];
//	FastTableDWidth[bplvtotal].data[9]=0;
//	FastTableDWidth[bplvtotal].data[10]=0;
//	FastTableDWidth[bplvtotal].data[11]=0;
//	FastTableDWidth[bplvtotal].data[12]=0;
//	FastTableDWidth[bplvtotal].data[13]=0;
//	FastTableDWidth[bplvtotal].data[14]=0;
//	FastTableDWidth[bplvtotal].data[15]=0;
//

    }; /* beebpixr */
  }; /* beebpixl */
}; /* DoFastTable16XStep8 */

static void DoFastTable16XStep8SDL(void) {
  unsigned int beebpixvl,beebpixvr;
  unsigned int bplvopen,bplvtotal;
  unsigned char tmp;

  for(beebpixvl=0;beebpixvl<16;beebpixvl++) {
    bplvopen=((beebpixvl & 8)?128:0) |
             ((beebpixvl & 4)?32:0) |
             ((beebpixvl & 2)?8:0) |
             ((beebpixvl & 1)?2:0);
    for(beebpixvr=0;beebpixvr<16;beebpixvr++) {
      bplvtotal=bplvopen |
             ((beebpixvr & 8)?64:0) |
             ((beebpixvr & 4)?16:0) |
             ((beebpixvr & 2)?4:0) |
             ((beebpixvr & 1)?1:0);
      tmp=VideoULA_Palette[beebpixvl];
      if (tmp>7) {
        tmp&=7;
        if (VideoULA_ControlReg & 1) tmp^=7;
      };
	if ( IsFlippedMode() )
		FastTableDWidthSDL[bplvtotal].data[4]=FastTableDWidthSDL[bplvtotal].data[5]=FastTableDWidthSDL[bplvtotal].data[6]=FastTableDWidthSDL[bplvtotal].data[7]=mainWin->cols[tmp];
	else
        FastTableDWidthSDL[bplvtotal].data[0]=FastTableDWidthSDL[bplvtotal].data[1]=
         FastTableDWidthSDL[bplvtotal].data[2]=FastTableDWidthSDL[bplvtotal].data[3]=mainWin->cols[tmp];

      tmp=VideoULA_Palette[beebpixvr];
      if (tmp>7) {
        tmp&=7;
        if (VideoULA_ControlReg & 1) tmp^=7;
      };
	if ( IsFlippedMode() )
		FastTableDWidthSDL[bplvtotal].data[0]=FastTableDWidthSDL[bplvtotal].data[1]=FastTableDWidthSDL[bplvtotal].data[2]=FastTableDWidthSDL[bplvtotal].data[3]=mainWin->cols[tmp];
	else
        FastTableDWidthSDL[bplvtotal].data[4]=FastTableDWidthSDL[bplvtotal].data[5]=
         FastTableDWidthSDL[bplvtotal].data[6]=FastTableDWidthSDL[bplvtotal].data[7]=mainWin->cols[tmp];

    }; /* beebpixr */
  }; /* beebpixl */
}; /* DoFastTable16XStep8 */




/*-------------------------------------------------------------------------------------------------------------*/
/* Some guess work and experimentation has determined that the left most pixel uses bits 7,5,3,1 for the       */
/* palette address, the next uses 6,4,2,0, the next uses 5,3,1,H (H=High), then 5,2,0,H                        */
static void DoFastTable4(void) {
  unsigned char tmp;
  unsigned long beebpixv,pentry;

  for(beebpixv=0;beebpixv<256;beebpixv++) {
    pentry=((beebpixv & 128)?8:0)
           | ((beebpixv & 32)?4:0)
           | ((beebpixv & 8)?2:0)
           | ((beebpixv & 2)?1:0);
    tmp=VideoULA_Palette[pentry];
    if (tmp>7) {
      tmp&=7;
      if (VideoULA_ControlReg & 1) tmp^=7;
    };
// [HERE]
    FastTable[beebpixv].data[0]=FastTable[beebpixv].data[1]=mainWin->cols[tmp];
//	FastTable[beebpixv].data[0]=mainWin->cols[tmp];
//	FastTable[beebpixv].data[1]=0;
//
    pentry=((beebpixv & 64)?8:0)
           | ((beebpixv & 16)?4:0)
           | ((beebpixv & 4)?2:0)
           | ((beebpixv & 1)?1:0);
    tmp=VideoULA_Palette[pentry];
    if (tmp>7) {
      tmp&=7;
      if (VideoULA_ControlReg & 1) tmp^=7;
    };
// [HERE]
    FastTable[beebpixv].data[2]=FastTable[beebpixv].data[3]=mainWin->cols[tmp];
//	FastTable[beebpixv].data[2]=mainWin->cols[tmp];
//	FastTable[beebpixv].data[3]=0;
//
    pentry=((beebpixv & 32)?8:0)
           | ((beebpixv & 8)?4:0)
           | ((beebpixv & 2)?2:0)
           | 1;
    tmp=VideoULA_Palette[pentry];
    if (tmp>7) {
      tmp&=7;
      if (VideoULA_ControlReg & 1) tmp^=7;
    };
// [HERE]
    FastTable[beebpixv].data[4]=FastTable[beebpixv].data[5]=mainWin->cols[tmp];
//	FastTable[beebpixv].data[4]=mainWin->cols[tmp];
//	FastTable[beebpixv].data[5]=0;
//
    pentry=((beebpixv & 16)?8:0)
           | ((beebpixv & 4)?4:0)
           | ((beebpixv & 1)?2:0)
           | 1;
    tmp=VideoULA_Palette[pentry];
    if (tmp>7) {
      tmp&=7;
      if (VideoULA_ControlReg & 1) tmp^=7;
    };
// [HERE]
    FastTable[beebpixv].data[6]=FastTable[beebpixv].data[7]=mainWin->cols[tmp];
//	FastTable[beebpixv].data[6]=mainWin->cols[tmp];
//	FastTable[beebpixv].data[7]=0;
//
  }; /* beebpixv */
}; /* DoFastTable4 */

static void DoFastTable4SDL(void) {
  unsigned char tmp;
  unsigned long beebpixv,pentry;

  for(beebpixv=0;beebpixv<256;beebpixv++) {
    pentry=((beebpixv & 128)?8:0)
           | ((beebpixv & 32)?4:0)
           | ((beebpixv & 8)?2:0)
           | ((beebpixv & 2)?1:0);
    tmp=VideoULA_Palette[pentry];
    if (tmp>7) {
      tmp&=7;
      if (VideoULA_ControlReg & 1) tmp^=7;
    };
	if ( IsFlippedMode() )
		FastTableSDL[beebpixv].data[3]=mainWin->cols[tmp];
	else
        FastTableSDL[beebpixv].data[0]=mainWin->cols[tmp];

    pentry=((beebpixv & 64)?8:0)
           | ((beebpixv & 16)?4:0)
           | ((beebpixv & 4)?2:0)
           | ((beebpixv & 1)?1:0);
    tmp=VideoULA_Palette[pentry];
    if (tmp>7) {
      tmp&=7;
      if (VideoULA_ControlReg & 1) tmp^=7;
    };
	if ( IsFlippedMode() )
		FastTableSDL[beebpixv].data[2]=mainWin->cols[tmp];
	else
        FastTableSDL[beebpixv].data[1]=mainWin->cols[tmp];

    pentry=((beebpixv & 32)?8:0)
           | ((beebpixv & 8)?4:0)
           | ((beebpixv & 2)?2:0)
           | 1;
    tmp=VideoULA_Palette[pentry];
    if (tmp>7) {
      tmp&=7;
      if (VideoULA_ControlReg & 1) tmp^=7;
    };
	if ( IsFlippedMode() )
		FastTableSDL[beebpixv].data[1]=mainWin->cols[tmp];
	else
        FastTableSDL[beebpixv].data[2]=mainWin->cols[tmp];
    pentry=((beebpixv & 16)?8:0)
           | ((beebpixv & 4)?4:0)
           | ((beebpixv & 1)?2:0)
           | 1;
    tmp=VideoULA_Palette[pentry];
    if (tmp>7) {
      tmp&=7;
      if (VideoULA_ControlReg & 1) tmp^=7;
    };
	if ( IsFlippedMode() )
		FastTableSDL[beebpixv].data[0]=mainWin->cols[tmp];
	else
        FastTableSDL[beebpixv].data[3]=mainWin->cols[tmp];
  }; /* beebpixv */
}; /* DoFastTable4 */



/*-------------------------------------------------------------------------------------------------------------*/
/* Some guess work and experimentation has determined that the left most pixel uses bits 7,5,3,1 for the       */
/* palette address, the next uses 6,4,2,0, the next uses 5,3,1,H (H=High), then 5,2,0,H                        */
static void DoFastTable4XStep4(void) {
  unsigned char tmp;
  unsigned long beebpixv,pentry;

  for(beebpixv=0;beebpixv<256;beebpixv++) {
    pentry=((beebpixv & 128)?8:0)
           | ((beebpixv & 32)?4:0)
           | ((beebpixv & 8)?2:0)
           | ((beebpixv & 2)?1:0);
    tmp=VideoULA_Palette[pentry];
    if (tmp>7) {
      tmp&=7;
      if (VideoULA_ControlReg & 1) tmp^=7;
    };
// [HERE]
    FastTableDWidth[beebpixv].data[0]=FastTableDWidth[beebpixv].data[1]=
    FastTableDWidth[beebpixv].data[2]=FastTableDWidth[beebpixv].data[3]=mainWin->cols[tmp];
//	FastTableDWidth[beebpixv].data[0]=mainWin->cols[tmp];
//	FastTableDWidth[beebpixv].data[1]=0;
//	FastTableDWidth[beebpixv].data[2]=0;
//	FastTableDWidth[beebpixv].data[3]=0;
//

    pentry=((beebpixv & 64)?8:0)
           | ((beebpixv & 16)?4:0)
           | ((beebpixv & 4)?2:0)
           | ((beebpixv & 1)?1:0);
    tmp=VideoULA_Palette[pentry];
    if (tmp>7) {
      tmp&=7;
      if (VideoULA_ControlReg & 1) tmp^=7;
    };
// [HERE]
    FastTableDWidth[beebpixv].data[4]=FastTableDWidth[beebpixv].data[5]=
    FastTableDWidth[beebpixv].data[6]=FastTableDWidth[beebpixv].data[7]=mainWin->cols[tmp];
//	FastTableDWidth[beebpixv].data[4]=mainWin->cols[tmp];
//	FastTableDWidth[beebpixv].data[5]=0;
//	FastTableDWidth[beebpixv].data[6]=0;
//	FastTableDWidth[beebpixv].data[7]=0;
//
    pentry=((beebpixv & 32)?8:0)
           | ((beebpixv & 8)?4:0)
           | ((beebpixv & 2)?2:0)
           | 1;
    tmp=VideoULA_Palette[pentry];
    if (tmp>7) {
      tmp&=7;
      if (VideoULA_ControlReg & 1) tmp^=7;
    };
// [HERE]
    FastTableDWidth[beebpixv].data[8]=FastTableDWidth[beebpixv].data[9]=
    FastTableDWidth[beebpixv].data[10]=FastTableDWidth[beebpixv].data[11]=mainWin->cols[tmp];
//	FastTableDWidth[beebpixv].data[8]=mainWin->cols[tmp];
//	FastTableDWidth[beebpixv].data[9]=0;
//	FastTableDWidth[beebpixv].data[10]=0;
//	FastTableDWidth[beebpixv].data[11]=0;
//
    pentry=((beebpixv & 16)?8:0)
           | ((beebpixv & 4)?4:0)
           | ((beebpixv & 1)?2:0)
           | 1;
    tmp=VideoULA_Palette[pentry];
    if (tmp>7) {
      tmp&=7;
      if (VideoULA_ControlReg & 1) tmp^=7;
    };
// [HERE]
    FastTableDWidth[beebpixv].data[12]=FastTableDWidth[beebpixv].data[13]=
    FastTableDWidth[beebpixv].data[14]=FastTableDWidth[beebpixv].data[15]=mainWin->cols[tmp];
//	FastTableDWidth[beebpixv].data[12]=mainWin->cols[tmp];
//	FastTableDWidth[beebpixv].data[13]=0;
//	FastTableDWidth[beebpixv].data[14]=0;
//	FastTableDWidth[beebpixv].data[15]=0;
//
  }; /* beebpixv */
}; /* DoFastTable4XStep4 */

static void DoFastTable4XStep4SDL(void) {
  unsigned char tmp;
  unsigned long beebpixv,pentry;

  for(beebpixv=0;beebpixv<256;beebpixv++) {
    pentry=((beebpixv & 128)?8:0)
           | ((beebpixv & 32)?4:0)
           | ((beebpixv & 8)?2:0)
           | ((beebpixv & 2)?1:0);
    tmp=VideoULA_Palette[pentry];
    if (tmp>7) {
      tmp&=7;
      if (VideoULA_ControlReg & 1) tmp^=7;
    };
	if ( IsFlippedMode() )
	FastTableDWidthSDL[beebpixv].data[6]=FastTableDWidthSDL[beebpixv].data[7]=mainWin->cols[tmp];
	else
        FastTableDWidthSDL[beebpixv].data[0]=FastTableDWidthSDL[beebpixv].data[1]=mainWin->cols[tmp];

    pentry=((beebpixv & 64)?8:0)
           | ((beebpixv & 16)?4:0)
           | ((beebpixv & 4)?2:0)
           | ((beebpixv & 1)?1:0);
    tmp=VideoULA_Palette[pentry];
    if (tmp>7) {
      tmp&=7;
      if (VideoULA_ControlReg & 1) tmp^=7;
    };
	if ( IsFlippedMode() )
			FastTableDWidthSDL[beebpixv].data[4]=FastTableDWidthSDL[beebpixv].data[5]=mainWin->cols[tmp];
	else
        FastTableDWidthSDL[beebpixv].data[2]=FastTableDWidthSDL[beebpixv].data[3]=mainWin->cols[tmp];

    pentry=((beebpixv & 32)?8:0)
           | ((beebpixv & 8)?4:0)
           | ((beebpixv & 2)?2:0)
           | 1;
    tmp=VideoULA_Palette[pentry];
    if (tmp>7) {
      tmp&=7;
      if (VideoULA_ControlReg & 1) tmp^=7;
    };
	if ( IsFlippedMode() )
		FastTableDWidthSDL[beebpixv].data[2]=FastTableDWidthSDL[beebpixv].data[3]=mainWin->cols[tmp];
	else
        FastTableDWidthSDL[beebpixv].data[4]=FastTableDWidthSDL[beebpixv].data[5]=mainWin->cols[tmp];
    pentry=((beebpixv & 16)?8:0)
           | ((beebpixv & 4)?4:0)
           | ((beebpixv & 1)?2:0)
           | 1;
    tmp=VideoULA_Palette[pentry];
    if (tmp>7) {
      tmp&=7;
      if (VideoULA_ControlReg & 1) tmp^=7;
    };
	if ( IsFlippedMode() )
		FastTableDWidthSDL[beebpixv].data[0]=FastTableDWidthSDL[beebpixv].data[1]=mainWin->cols[tmp];
	else
        FastTableDWidthSDL[beebpixv].data[6]=FastTableDWidthSDL[beebpixv].data[7]=mainWin->cols[tmp];

  }; /* beebpixv */
}; /* DoFastTable4XStep4 */


/*-------------------------------------------------------------------------------------------------------------*/
/* Some guess work and experimentation has determined that the left most pixel uses the same pattern as mode 1 */
/* all the way upto the 5th pixel which uses 31hh then 20hh and hten 1hhh then 0hhhh                           */
static void DoFastTable2(void) {
  unsigned char tmp;
  unsigned long beebpixv,beebpixvt,pentry;
  int pix;

  for(beebpixv=0;beebpixv<256;beebpixv++) {
    beebpixvt=beebpixv;
    for(pix=0;pix<8;pix++) {
      pentry=((beebpixvt & 128)?8:0)
             | ((beebpixvt & 32)?4:0)
             | ((beebpixvt & 8)?2:0)
             | ((beebpixvt & 2)?1:0);
      beebpixvt<<=1;
      beebpixvt|=1;
      tmp=VideoULA_Palette[pentry];
      if (tmp>7) {
        tmp&=7;
        if (VideoULA_ControlReg & 1) tmp^=7;
      };
      FastTable[beebpixv].data[pix]=mainWin->cols[tmp];
    }; /* pix */
  }; /* beebpixv */
}; /* DoFastTable2 */

static void DoFastTable2SDL(void) {
  unsigned char tmp;
  unsigned long beebpixv,beebpixvt,pentry;
  int pix;

  for(beebpixv=0;beebpixv<256;beebpixv++) {
    beebpixvt=beebpixv;
    for(pix=0;pix<8;pix++) {
      pentry=((beebpixvt & 128)?8:0)
             | ((beebpixvt & 32)?4:0)
             | ((beebpixvt & 8)?2:0)
             | ((beebpixvt & 2)?1:0);
      beebpixvt<<=1;
      beebpixvt|=1;
      tmp=VideoULA_Palette[pentry];
      if (tmp>7) {
        tmp&=7;
        if (VideoULA_ControlReg & 1) tmp^=7;
      };
	if ( IsFlippedMode() )
		FastTableSDL[beebpixv].data[3-(pix/2)]=mainWin->cols[tmp];
	else
        FastTableSDL[beebpixv].data[pix/2]=mainWin->cols[tmp];
    }; /* pix */
  }; /* beebpixv */
}; /* DoFastTable2 */


/*-------------------------------------------------------------------------------------------------------------*/
/* Some guess work and experimentation has determined that the left most pixel uses the same pattern as mode 1 */
/* all the way upto the 5th pixel which uses 31hh then 20hh and hten 1hhh then 0hhhh                           */
static void DoFastTable2XStep2(void) {
  unsigned char tmp;
  unsigned long beebpixv,beebpixvt,pentry;
  int pix;

  for(beebpixv=0;beebpixv<256;beebpixv++) {
    beebpixvt=beebpixv;
    for(pix=0;pix<8;pix++) {
      pentry=((beebpixvt & 128)?8:0)
             | ((beebpixvt & 32)?4:0)
             | ((beebpixvt & 8)?2:0)
             | ((beebpixvt & 2)?1:0);
      beebpixvt<<=1;
      beebpixvt|=1;
      tmp=VideoULA_Palette[pentry];
      if (tmp>7) {
        tmp&=7;
        if (VideoULA_ControlReg & 1) tmp^=7;
      };
      FastTableDWidth[beebpixv].data[pix*2]=FastTableDWidth[beebpixv].data[pix*2+1]=mainWin->cols[tmp];
    }; /* pix */
  }; /* beebpixv */
}; /* DoFastTable2XStep2 */

static void DoFastTable2XStep2SDL(void) {
  unsigned char tmp;
  unsigned long beebpixv,beebpixvt,pentry;
  int pix;

  for(beebpixv=0;beebpixv<256;beebpixv++) {
    beebpixvt=beebpixv;
    for(pix=0;pix<8;pix++) {
      pentry=((beebpixvt & 128)?8:0)
             | ((beebpixvt & 32)?4:0)
             | ((beebpixvt & 8)?2:0)
             | ((beebpixvt & 2)?1:0);
      beebpixvt<<=1;
      beebpixvt|=1;
      tmp=VideoULA_Palette[pentry];
      if (tmp>7) {
        tmp&=7;
        if (VideoULA_ControlReg & 1) tmp^=7;
      };
	if ( IsFlippedMode() )
		FastTableDWidthSDL[beebpixv].data[7-pix]=mainWin->cols[tmp];
	else
        FastTableDWidthSDL[beebpixv].data[pix]=mainWin->cols[tmp];
    }; /* pix */
  }; /* beebpixv */
}; /* DoFastTable2XStep2 */


/*-------------------------------------------------------------------------------------------------------------*/
/* Check validity of fast table, and if invalid rebuild.
   The fast table accelerates the translation of beeb video memory
   values into X pixel values */
static void DoFastTable(void) {
  /* if it's already OK then quit */
  if (FastTable_Valid) return;

  if (!(CRTC_HorizontalDisplayed & 3)) {
//    LineRoutine=(VideoULA_ControlReg & 0x10)?LowLevelDoScanLineNarrow:LowLevelDoScanLineWide;
      LineRoutine=(VideoULA_ControlReg & 0x10)?LowLevelDoScanLineNarrowSDL:LowLevelDoScanLineWideSDL;
  } else {
    //LineRoutine=(VideoULA_ControlReg & 0x10)?LowLevelDoScanLineNarrowNot4Bytes:LowLevelDoScanLineWideNot4Bytes;
    LineRoutine=(VideoULA_ControlReg & 0x10)?LowLevelDoScanLineNarrowNot4BytesSDL:LowLevelDoScanLineWideNot4BytesSDL;
  };

  /* What happens next dpends on the number of colours */
  switch (NColsLookup[(VideoULA_ControlReg & 0x1c) >> 2]) {
    case 2:
      if (VideoULA_ControlReg & 0x10) {
        //DoFastTable2();
        DoFastTable2SDL();
      } else {
        //DoFastTable2XStep2();
        DoFastTable2XStep2SDL();
      };
      FastTable_Valid=1;     
      break;

    case 4:
      if (VideoULA_ControlReg & 0x10) {
        //DoFastTable4();
        DoFastTable4SDL();
      } else {
        //DoFastTable4XStep4();
        DoFastTable4XStep4SDL();
      };
      FastTable_Valid=1;     
      break;

    case 16:
      if (VideoULA_ControlReg & 0x10) {
        //DoFastTable16();
        DoFastTable16SDL();
      } else {
        //DoFastTable16XStep8();
        DoFastTable16XStep8SDL();
      };
      FastTable_Valid=1;
      break;

    default:
      break;
  }; /* Colours/pixel switch */
}; /* DoFastTable */

/*-------------------------------------------------------------------------------------------------------------*/

































//--#define BEEB_DOTIME_SAMPLESIZE 50
//--
//--static void VideoStartOfFrame(void) {
//--  static int InterlaceFrame=0;
//--  int CurStart;
//--  int IL_Multiplier;
//--//--#ifdef BEEB_DOTIME
//--//--  static int Have_GotTime=0;
//--//--  static struct tms previous,now;
//--//--  static int Time_FrameCount=0;
//--//--
//--//--  double frametime;
//--//--  static CycleCountT OldCycles=0;
//--//--  
//--//--  int CurStart; 
//--//--
//--//--
//--//--  if (!Have_GotTime) {
//--//--    times(&previous);
//--//--    Time_FrameCount=-1;
//--//--    Have_GotTime=1;
//--//--  };
//--//--
//--//--  if (Time_FrameCount==(BEEB_DOTIME_SAMPLESIZE-1)) {
//--//--    times(&now);
//--//--    frametime=now.tms_utime-previous.tms_utime;
//--//--#ifndef SUNOS
//--//--#ifndef HPUX
//--//--                frametime/=(double)CLOCKS_PER_SEC;
//--//--#else
//--//--                frametime/=(double)sysconf(_SC_CLK_TCK);
//--//--#endif
//--//--#else 
//--//--                frametime/=(double)HZ;
//--//--#endif
//--//--    frametime/=(double)BEEB_DOTIME_SAMPLESIZE;
//--//--    cerr << "Frametime: " << frametime << "s fps=" << (1/frametime) << "Total cycles=" << TotalCycles << "Cycles in last unit=" << (TotalCycles-OldCycles) << "\n";
//--//--    OldCycles=TotalCycles;
//--//--    previous=now;
//--//--    Time_FrameCount=0;
//--//--  } else Time_FrameCount++;
//--//--
//--//--#endif
//--
//--//--#ifdef WIN32
//--  /* FrameNum is determined by the window handler */
//--  if (mainWin)
//--    FrameNum = mainWin->StartOfFrame();
//--//--#else
//--//--  /* If FrameNum hits 0 we actually refresh */
//--//--  if (FrameNum--==0) {
//--//--    FrameNum=Video_RefreshFrequency-1;
//--//--  };
//--//--#endif
//--	// Cursor update for blink. I thought I'd put it here, as this is where the mode 7 flash field thingy is too
//--	// - Richard Gellman
//--	CursorFieldCount--;
//--	if (CursorFieldCount<0) {
//--		CurStart = CRTC_CursorStart & 0x60;
//--		// 0 is cursor displays, but does not blink
//--		// 32 is no cursor
//--		// 64 is 1/16 fast blink
//--		// 96 is 1/32 slow blink
//--		if (CurStart==0) { CursorFieldCount=CursorOnFields; CursorOnState=1; }
//--		if (CurStart==32) { CursorFieldCount=CursorOffFields; CursorOnState=0; }
//--		if (CurStart==64) { CursorFieldCount=8; CursorOnState^=1; }
//--		if (CurStart==96) { CursorFieldCount=16; CursorOnState^=1; }
//--	}
//--
//--  if (CRTC_VerticalTotalAdjust==0) {
//--    VideoState.CharLine=0;
//--    VideoState.InCharLineUp=0;
//--  } else {
//--    VideoState.CharLine=-1;
//--    VideoState.InCharLineUp=0;
//--  };
//--
//--  
//--  VideoState.IsTeletext=(VideoULA_ControlReg &2)>0;
//--  if (!VideoState.IsTeletext) {
//--    VideoState.Addr=VideoState.StartAddr=CRTC_ScreenStartLow+(CRTC_ScreenStartHigh<<8);
//--  } else {
//--    int tmphigh=CRTC_ScreenStartHigh;
//--    /* undo wrangling of start address - I don't understand why this should be - see p.372 of AUG for this info */
//--    tmphigh^=0x20;
//--    tmphigh+=0x74;
//--    tmphigh&=255;
//--    VideoState.Addr=VideoState.StartAddr=CRTC_ScreenStartLow+(tmphigh<<8);
//--
//--	// O aye. this is the mode 7 flash section is it? Modified for corrected flash settings - Richard Gellman
//--    Mode7FlashTrigger--;
//--    if (Mode7FlashTrigger<0) {
//--		Mode7FlashTrigger=(Mode7FlashOn)?MODE7OFFFIELDS:MODE7ONFIELDS;
//--      Mode7FlashOn^=1; /* toggle flash state */
//--    };
//--  };
//--
//--
//--  InterlaceFrame^=1;
//--  IL_Multiplier=(CRTC_InterlaceAndDelay&1)?2:1;
//--  if (InterlaceFrame) {
//--    IncTrigger((IL_Multiplier*(CRTC_HorizontalTotal+1)*((VideoULA_ControlReg & 16)?1:2)),VideoTriggerCount); /* Number of 2MHz cycles until another scanline needs doing */
//--  } else {
//--    IncTrigger(((CRTC_HorizontalTotal+1)*((VideoULA_ControlReg & 16)?1:2)),VideoTriggerCount); /* Number of 2MHz cycles until another scanline needs doing */
//--  };
//--
//--  TeleTextPoll();
//--
//--}; /* VideoStartOfFrame */






//--//++
//--// The old GP2X one
//--#define BEEB_DOTIME_SAMPLESIZE 50
//--
//--static void VideoStartOfFrame(void) {
//--  int tmp;
//--  static int InterlaceFrame=0;
//--
//--	static unsigned int last_time=0;
//--	unsigned int current_time;
//-- 	int adjusted_time;
//--
//--//==  static int Have_GotTime=0;
//--//==  static struct tms previous,now;
//--//==  static int Time_FrameCount=0;
//--
//--//==  double frametime;
//--//==  static CycleCountT OldCycles=0;
//--
//--
//--//==  if (!Have_GotTime) {
//--//==    times(&previous);
//--//==    Time_FrameCount=-1;
//--//==    Have_GotTime=1;
//--//==  };
//--
//--//==  if (Time_FrameCount==(BEEB_DOTIME_SAMPLESIZE-1)) {
//--//==    times(&now);
//--//==    frametime=now.tms_utime-previous.tms_utime;
//--//==    frametime/=(double)CLOCKS_PER_SEC;
//--//==
//--//==    OldCycles=TotalCycles;
//--//==    previous=now;
//--//==    Time_FrameCount=0;
//--//==  } else Time_FrameCount++;
//--
//--
//--
//--	/* Inc frame count and every odd frame synchronize simulated Beeb time with the
//--	 * Host's actual time. 
//--	 *
//--	 * Timing:
//--	 *
//--	 * 1000ms / 25fps = 40ms of real time should pass between video repaints.
//--	 */
//--
//--#define SYNC 40
//--
//--	if ( (++FrameCount&1) == 1 && mainWin != NULL) {
//--		unsigned int current_time = mainWin->GetTicks();
//--		adjusted_time = SYNC - ( (int) ( current_time - last_time) );
//--		if (adjusted_time > SYNC) adjusted_time = SYNC;
//--
//--		if (adjusted_time > 0 && adjusted_time <= SYNC) {
//--			time_slept += adjusted_time;
//--			mainWin->Sleep(adjusted_time);
//--		}
//--
//--		last_time = mainWin->GetTicks();
//--	}
//--
//--
//--  /* If FrameNum hits 0 we actually refresh */
//--  if (FrameNum--==0) {
//--    FrameNum=Video_RefreshFrequency-1;
//--  };
//--
//--  if (CRTC_VerticalTotalAdjust==0) {
//--    VideoState.CharLine=0;
//--    VideoState.InCharLine=CRTC_ScanLinesPerChar;
//--    VideoState.InCharLineUp=0;
//--  } else {
//--    VideoState.CharLine=-1;
//--    VideoState.InCharLine=CRTC_VerticalTotalAdjust;
//--    VideoState.InCharLineUp=0;
//--  };
//--  VideoState.IsTeletext=(VideoULA_ControlReg &2)>0;
//--  if (!VideoState.IsTeletext) {
//--    VideoState.Addr=CRTC_ScreenStartLow+(CRTC_ScreenStartHigh<<8);
//--  } else {
//--    int tmphigh=CRTC_ScreenStartHigh;
//--    /* undo wrangling of start address - I don't understand why this should be - see p.372 of AUG for this info */
//--    tmphigh^=0x20;
//--    tmphigh+=0x74;
//--    tmphigh&=255;
//--    VideoState.Addr=CRTC_ScreenStartLow+(tmphigh<<8);
//--
//--    Mode7FlashTrigger--;
//--    if (Mode7FlashTrigger<0) {
//--      Mode7FlashTrigger=MODE7FLASHFREQUENCY;
//--      Mode7FlashOn^=1; /* toggle flash state */
//--    };
//--    for(tmp=0;tmp<80;tmp++) {
//--      Mode7DoubleHeightFlags[tmp]=1; /* corresponding character on NEXT line is top half */
//--    };
//--  };
//--
//--  InterlaceFrame^=1;
//--  if (InterlaceFrame) {
//--    IncTrigger((2*(CRTC_HorizontalTotal+1)*((VideoULA_ControlReg & 16)?1:2)),VideoTriggerCount); /* Number of 2MHz cycles until another scanline needs doing */
//--  } else {
//--    IncTrigger(((CRTC_HorizontalTotal+1)*((VideoULA_ControlReg & 16)?1:2)),VideoTriggerCount); /* Number of 2MHz cycles until another scanline needs doing */
//--  };
//--
//--//	if (mainWin->OnScreenKeyboardShown())
//--//		VirtualKeyboard_UpdateLEDs();
//--
//--//++
//--	//TeleTextPoll();
//--//<-
//--
//--}; /* VideoStartOfFrame */

//<-






// New combined one.
#define BEEB_DOTIME_SAMPLESIZE 50

static void VideoStartOfFrame(void) {
  static int InterlaceFrame=0;
  int CurStart;
  int IL_Multiplier;

  int tmp;
  static unsigned int last_time=0;
  unsigned int current_time;
  int adjusted_time;

//--  /* FrameNum is determined by the window handler */
//--  if (mainWin)
//--    FrameNum = mainWin->StartOfFrame();



// GP2X sync code @ 25HZ

//#ifdef GP2X
//  static unsigned long gp2x_time=0;

//	unsigned long gp2x_frame_end = gp2x_time + (7372800/25);
//	unsigned long gp2x_current_time = GP2X_timer();
//
//
//	if (gp2x_current_time > gp2x_frame_end) {
//		printf("Going too slow (went over by %ld)\n", gp2x_current_time - gp2x_frame_end);
//	} else {
//		printf("Going too fast (had %ld left)\n", gp2x_frame_end - gp2x_current_time);
//	}
//
//
//	// End of frame.
//	gp2x_time = GP2X_timer();

//#else
#	define SYNC 40
	if ( (++FrameCount&1) == 1 && mainWin != NULL) {
		unsigned int current_time = mainWin->GetTicks();
		adjusted_time = SYNC - ( (int) ( current_time - last_time) );
		if (adjusted_time > SYNC) adjusted_time = SYNC;

		if (adjusted_time > 0 && adjusted_time <= SYNC) {
			time_slept += adjusted_time;
			mainWin->Sleep(adjusted_time);
		}

		last_time = mainWin->GetTicks();
	}
//#endif





	// Cursor update for blink. I thought I'd put it here, as this is where the mode 7 flash field thingy is too
	// - Richard Gellman
	CursorFieldCount--;
	if (CursorFieldCount<0) {
		CurStart = CRTC_CursorStart & 0x60;
		// 0 is cursor displays, but does not blink
		// 32 is no cursor
		// 64 is 1/16 fast blink
		// 96 is 1/32 slow blink
		if (CurStart==0) { CursorFieldCount=CursorOnFields; CursorOnState=1; }
		if (CurStart==32) { CursorFieldCount=CursorOffFields; CursorOnState=0; }
		if (CurStart==64) { CursorFieldCount=8; CursorOnState^=1; }
		if (CurStart==96) { CursorFieldCount=16; CursorOnState^=1; }
	}

  if (CRTC_VerticalTotalAdjust==0) {
    VideoState.CharLine=0;
    VideoState.InCharLineUp=0;
  } else {
    VideoState.CharLine=-1;
    VideoState.InCharLineUp=0;
  };

  VideoState.IsTeletext=(VideoULA_ControlReg &2)>0;
  if (!VideoState.IsTeletext) {
    VideoState.Addr=VideoState.StartAddr=CRTC_ScreenStartLow+(CRTC_ScreenStartHigh<<8);
  } else {
    int tmphigh=CRTC_ScreenStartHigh;
    /* undo wrangling of start address - I don't understand why this should be - see p.372 of AUG for this info */
    tmphigh^=0x20;
    tmphigh+=0x74;
    tmphigh&=255;
    VideoState.Addr=VideoState.StartAddr=CRTC_ScreenStartLow+(tmphigh<<8);

	// O aye. this is the mode 7 flash section is it? Modified for corrected flash settings - Richard Gellman
    Mode7FlashTrigger--;
    if (Mode7FlashTrigger<0) {
		Mode7FlashTrigger=(Mode7FlashOn)?MODE7OFFFIELDS:MODE7ONFIELDS;
      Mode7FlashOn^=1; /* toggle flash state */
    };
  };

  InterlaceFrame^=1;
  IL_Multiplier=(CRTC_InterlaceAndDelay&1)?2:1;
  if (InterlaceFrame) {
    IncTrigger((IL_Multiplier*(CRTC_HorizontalTotal+1)*((VideoULA_ControlReg & 16)?1:2)),VideoTriggerCount); /* Number of 2MHz cycles until another scanline needs doing */
  } else {
    IncTrigger(((CRTC_HorizontalTotal+1)*((VideoULA_ControlReg & 16)?1:2)),VideoTriggerCount); /* Number of 2MHz cycles until another scanline needs doing */
  };

//  TeleTextPoll();

}; /* VideoStartOfFrame */

















/* Scale BBC frame buffer to GP2X height:
 */

unsigned char scanlines_scaled[256] = {
        0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0xff,
        0x0f, 0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1a, 0x1b, 0x1c, 0x1d, 0xff,
        0x1e, 0x1f, 0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28, 0x29, 0x2a, 0x2b, 0x2c, 0xff,
        0x2d, 0x2e, 0x2f, 0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x3a, 0x3b, 0xff,
        0x3c, 0x3d, 0x3e, 0x3f, 0x40, 0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48, 0x49, 0x4a, 0xff,
        0x4b, 0x4c, 0x4d, 0x4e, 0x4f, 0x50, 0x51, 0x52, 0x53, 0x54, 0x55, 0x56, 0x57, 0x58, 0x59, 0xff,
        0x5a, 0x5b, 0x5c, 0x5d, 0x5e, 0x5f, 0x60, 0x61, 0x62, 0x63, 0x64, 0x65, 0x66, 0x67, 0x68, 0xff,
        0x69, 0x6a, 0x6b, 0x6c, 0x6d, 0x6e, 0x6f, 0x70, 0x71, 0x72, 0x73, 0x74, 0x75, 0x76, 0x77, 0xff,
        0x78, 0x79, 0x7a, 0x7b, 0x7c, 0x7d, 0x7e, 0x7f, 0x80, 0x81, 0x82, 0x83, 0x84, 0x85, 0x86, 0xff,
        0x87, 0x88, 0x89, 0x8a, 0x8b, 0x8c, 0x8d, 0x8e, 0x8f, 0x90, 0x91, 0x92, 0x93, 0x94, 0x95, 0xff,
        0x96, 0x97, 0x98, 0x99, 0x9a, 0x9b, 0x9c, 0x9d, 0x9e, 0x9f, 0xa0, 0xa1, 0xa2, 0xa3, 0xa4, 0xff,
        0xa5, 0xa6, 0xa7, 0xa8, 0xa9, 0xaa, 0xab, 0xac, 0xad, 0xae, 0xaf, 0xb0, 0xb1, 0xb2, 0xb3, 0xff,
        0xb4, 0xb5, 0xb6, 0xb7, 0xb8, 0xb9, 0xba, 0xbb, 0xbc, 0xbd, 0xbe, 0xbf, 0xc0, 0xc1, 0xc2, 0xff,
        0xc3, 0xc4, 0xc5, 0xc6, 0xc7, 0xc8, 0xc9, 0xca, 0xcb, 0xcc, 0xcd, 0xce, 0xcf, 0xd0, 0xd1, 0xff,
        0xd2, 0xd3, 0xd4, 0xd5, 0xd6, 0xd7, 0xd8, 0xd9, 0xda, 0xdb, 0xdc, 0xdd, 0xde, 0xdf, 0xe0, 0xff,
        0xe1, 0xe2, 0xe3, 0xe4, 0xe5, 0xe6, 0xe7, 0xe8, 0xe9, 0xea, 0xeb, 0xec, 0xed, 0xee, 0xef, 0xff
};


/*unsigned char scanlines_scaled[256] = {
        0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0xff, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
        0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f, 0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17,
        0x18, 0x19, 0x1a, 0x1b, 0x1c, 0x1d, 0x1e, 0x1f, 0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27,
        0x28, 0x29, 0x2a, 0x2b, 0x2c, 0x2d, 0x2e, 0x2f, 0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37,
        0x38, 0x39, 0x3a, 0x3b, 0x3c, 0x3d, 0x3e, 0x3f, 0x40, 0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47,
        0x48, 0x49, 0x4a, 0x4b, 0x4c, 0x4d, 0x4e, 0x4f, 0x50, 0x51, 0x52, 0x53, 0x54, 0x55, 0x56, 0x57,
        0x58, 0x59, 0x5a, 0x5b, 0x5c, 0x5d, 0x5e, 0x5f, 0x60, 0x61, 0x62, 0x63, 0x64, 0x65, 0x66, 0x67,
        0x68, 0x69, 0x6a, 0x6b, 0x6c, 0x6d, 0x6e, 0x6f, 0x70, 0x71, 0x72, 0x73, 0x74, 0x75, 0x76, 0x77,
        0x78, 0x79, 0x7a, 0x7b, 0x7c, 0x7d, 0x7e, 0x7f, 0x80, 0x81, 0x82, 0x83, 0x84, 0x85, 0x86, 0x87,
        0x88, 0x89, 0x8a, 0x8b, 0x8c, 0x8d, 0x8e, 0x8f, 0x90, 0x91, 0x92, 0x93, 0x94, 0x95, 0x96, 0x97,
        0x98, 0x99, 0x9a, 0x9b, 0x9c, 0x9d, 0x9e, 0x9f, 0xa0, 0xa1, 0xa2, 0xa3, 0xa4, 0xa5, 0xa6, 0xa7,
        0xa8, 0xa9, 0xaa, 0xab, 0xac, 0xad, 0xae, 0xaf, 0xb0, 0xb1, 0xb2, 0xb3, 0xb4, 0xb5, 0xb6, 0xb7,
        0xb8, 0xb9, 0xba, 0xbb, 0xbc, 0xbd, 0xbe, 0xbf, 0xc0, 0xc1, 0xc2, 0xc3, 0xc4, 0xc5, 0xc6, 0xc7,
        0xc8, 0xc9, 0xca, 0xcb, 0xcc, 0xcd, 0xce, 0xcf, 0xd0, 0xd1, 0xd2, 0xd3, 0xd4, 0xd5, 0xd6, 0xd7,
        0xd8, 0xd9, 0xda, 0xdb, 0xdc, 0xdd, 0xde, 0xdf, 0xe0, 0xe1, 0xe2, 0xe3, 0xff, 0xe4, 0xff, 0xe5, 0xff, 0xe6, 0xe7,
        0xe8, 0xe9, 0xea, 0xeb, 0xec, 0xed, 0xff, 0xee, 0xff, 0xef, 0xff, 0xff,    
};*/


unsigned char scanlines_top[256] = {
        0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f,
        0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1a, 0x1b, 0x1c, 0x1d, 0x1e, 0x1f,
        0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28, 0x29, 0x2a, 0x2b, 0x2c, 0x2d, 0x2e, 0x2f,
        0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x3a, 0x3b, 0x3c, 0x3d, 0x3e, 0x3f,
        0x40, 0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48, 0x49, 0x4a, 0x4b, 0x4c, 0x4d, 0x4e, 0x4f,
        0x50, 0x51, 0x52, 0x53, 0x54, 0x55, 0x56, 0x57, 0x58, 0x59, 0x5a, 0x5b, 0x5c, 0x5d, 0x5e, 0x5f,
        0x60, 0x61, 0x62, 0x63, 0x64, 0x65, 0x66, 0x67, 0x68, 0x69, 0x6a, 0x6b, 0x6c, 0x6d, 0x6e, 0x6f,
        0x70, 0x71, 0x72, 0x73, 0x74, 0x75, 0x76, 0x77, 0x78, 0x79, 0x7a, 0x7b, 0x7c, 0x7d, 0x7e, 0x7f,
        0x80, 0x81, 0x82, 0x83, 0x84, 0x85, 0x86, 0x87, 0x88, 0x89, 0x8a, 0x8b, 0x8c, 0x8d, 0x8e, 0x8f,
        0x90, 0x91, 0x92, 0x93, 0x94, 0x95, 0x96, 0x97, 0x98, 0x99, 0x9a, 0x9b, 0x9c, 0x9d, 0x9e, 0x9f,
        0xa0, 0xa1, 0xa2, 0xa3, 0xa4, 0xa5, 0xa6, 0xa7, 0xa8, 0xa9, 0xaa, 0xab, 0xac, 0xad, 0xae, 0xaf,
        0xb0, 0xb1, 0xb2, 0xb3, 0xb4, 0xb5, 0xb6, 0xb7, 0xb8, 0xb9, 0xba, 0xbb, 0xbc, 0xbd, 0xbe, 0xbf,
        0xc0, 0xc1, 0xc2, 0xc3, 0xc4, 0xc5, 0xc6, 0xc7, 0xc8, 0xc9, 0xca, 0xcb, 0xcc, 0xcd, 0xce, 0xcf,
        0xd0, 0xd1, 0xd2, 0xd3, 0xd4, 0xd5, 0xd6, 0xd7, 0xd8, 0xd9, 0xda, 0xdb, 0xdc, 0xdd, 0xde, 0xdf,
        0xe0, 0xe1, 0xe2, 0xe3, 0xe4, 0xe5, 0xe6, 0xe7, 0xe8, 0xe9, 0xea, 0xeb, 0xec, 0xed, 0xee, 0xef,
        0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff
};

unsigned char scanlines_bottom[256] = {
        0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
        0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f,
        0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1a, 0x1b, 0x1c, 0x1d, 0x1e, 0x1f,
        0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28, 0x29, 0x2a, 0x2b, 0x2c, 0x2d, 0x2e, 0x2f,
        0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x3a, 0x3b, 0x3c, 0x3d, 0x3e, 0x3f,
        0x40, 0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48, 0x49, 0x4a, 0x4b, 0x4c, 0x4d, 0x4e, 0x4f,
        0x50, 0x51, 0x52, 0x53, 0x54, 0x55, 0x56, 0x57, 0x58, 0x59, 0x5a, 0x5b, 0x5c, 0x5d, 0x5e, 0x5f,
        0x60, 0x61, 0x62, 0x63, 0x64, 0x65, 0x66, 0x67, 0x68, 0x69, 0x6a, 0x6b, 0x6c, 0x6d, 0x6e, 0x6f,
        0x70, 0x71, 0x72, 0x73, 0x74, 0x75, 0x76, 0x77, 0x78, 0x79, 0x7a, 0x7b, 0x7c, 0x7d, 0x7e, 0x7f,
        0x80, 0x81, 0x82, 0x83, 0x84, 0x85, 0x86, 0x87, 0x88, 0x89, 0x8a, 0x8b, 0x8c, 0x8d, 0x8e, 0x8f,
        0x90, 0x91, 0x92, 0x93, 0x94, 0x95, 0x96, 0x97, 0x98, 0x99, 0x9a, 0x9b, 0x9c, 0x9d, 0x9e, 0x9f,
        0xa0, 0xa1, 0xa2, 0xa3, 0xa4, 0xa5, 0xa6, 0xa7, 0xa8, 0xa9, 0xaa, 0xab, 0xac, 0xad, 0xae, 0xaf,
        0xb0, 0xb1, 0xb2, 0xb3, 0xb4, 0xb5, 0xb6, 0xb7, 0xb8, 0xb9, 0xba, 0xbb, 0xbc, 0xbd, 0xbe, 0xbf,
        0xc0, 0xc1, 0xc2, 0xc3, 0xc4, 0xc5, 0xc6, 0xc7, 0xc8, 0xc9, 0xca, 0xcb, 0xcc, 0xcd, 0xce, 0xcf,
        0xd0, 0xd1, 0xd2, 0xd3, 0xd4, 0xd5, 0xd6, 0xd7, 0xd8, 0xd9, 0xda, 0xdb, 0xdc, 0xdd, 0xde, 0xdf,
        0xe0, 0xe1, 0xe2, 0xe3, 0xe4, 0xe5, 0xe6, 0xe7, 0xe8, 0xe9, 0xea, 0xeb, 0xec, 0xed, 0xee, 0xef,
};

unsigned char scanlines_center[256] = {
        0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
        0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f, 0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17,
        0x18, 0x19, 0x1a, 0x1b, 0x1c, 0x1d, 0x1e, 0x1f, 0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27,
        0x28, 0x29, 0x2a, 0x2b, 0x2c, 0x2d, 0x2e, 0x2f, 0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37,
        0x38, 0x39, 0x3a, 0x3b, 0x3c, 0x3d, 0x3e, 0x3f, 0x40, 0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47,
        0x48, 0x49, 0x4a, 0x4b, 0x4c, 0x4d, 0x4e, 0x4f, 0x50, 0x51, 0x52, 0x53, 0x54, 0x55, 0x56, 0x57,
        0x58, 0x59, 0x5a, 0x5b, 0x5c, 0x5d, 0x5e, 0x5f, 0x60, 0x61, 0x62, 0x63, 0x64, 0x65, 0x66, 0x67,
        0x68, 0x69, 0x6a, 0x6b, 0x6c, 0x6d, 0x6e, 0x6f, 0x70, 0x71, 0x72, 0x73, 0x74, 0x75, 0x76, 0x77,
        0x78, 0x79, 0x7a, 0x7b, 0x7c, 0x7d, 0x7e, 0x7f, 0x80, 0x81, 0x82, 0x83, 0x84, 0x85, 0x86, 0x87,
        0x88, 0x89, 0x8a, 0x8b, 0x8c, 0x8d, 0x8e, 0x8f, 0x90, 0x91, 0x92, 0x93, 0x94, 0x95, 0x96, 0x97,
        0x98, 0x99, 0x9a, 0x9b, 0x9c, 0x9d, 0x9e, 0x9f, 0xa0, 0xa1, 0xa2, 0xa3, 0xa4, 0xa5, 0xa6, 0xa7,
        0xa8, 0xa9, 0xaa, 0xab, 0xac, 0xad, 0xae, 0xaf, 0xb0, 0xb1, 0xb2, 0xb3, 0xb4, 0xb5, 0xb6, 0xb7,
        0xb8, 0xb9, 0xba, 0xbb, 0xbc, 0xbd, 0xbe, 0xbf, 0xc0, 0xc1, 0xc2, 0xc3, 0xc4, 0xc5, 0xc6, 0xc7,
        0xc8, 0xc9, 0xca, 0xcb, 0xcc, 0xcd, 0xce, 0xcf, 0xd0, 0xd1, 0xd2, 0xd3, 0xd4, 0xd5, 0xd6, 0xd7,
        0xd8, 0xd9, 0xda, 0xdb, 0xdc, 0xdd, 0xde, 0xdf, 0xe0, 0xe1, 0xe2, 0xe3, 0xe4, 0xe5, 0xe6, 0xe7,
        0xe8, 0xe9, 0xea, 0xeb, 0xec, 0xed, 0xee, 0xef, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
};

unsigned char *scanline_mapping_p = scanlines_scaled;

//++
/* 0 = off, 1 = bottom, 2 = top:
 */
unsigned int virtual_keyboard = 0;
//<-


void SetVideoCentered(void)
{
	config.settings.vscale = CENTER;
	scanline_mapping_p = scanlines_center;
}

//++
void SetVideoTop(void)
{
	config.settings.vscale = TOP;
	scanline_mapping_p = scanlines_top;
}

void SetVideoBottom(void)
{
	config.settings.vscale = BOTTOM;
	scanline_mapping_p = scanlines_bottom;
}

void SetVideoScaled(void)
{
	config.settings.vscale = SCALED;
	scanline_mapping_p = scanlines_scaled;
}
//<-






void NewAdjustVideo() {
	InitialOffset=0-(((CRTC_HorizontalTotal+1)/2)-((HSyncModifier==8)?40:20));

	HStart=InitialOffset+((CRTC_HorizontalTotal+1)-(CRTC_HorizontalSyncPos+(CRTC_SyncWidth&15)));
	HStart+=(HSyncModifier==8)?2:1;

	if (TeletextEnabled) HStart+=2;
	if (HStart<0) HStart=0;

	ScreenAdjust=(HStart*HSyncModifier);
}

static inline int IsFlippedMode(void)
{

//	printf ("%d\n", mainWin->ScreenIsReversed());
	return mainWin->ScreenIsReversed();
//	return 1;
}




//static inline unsigned char* GetTeletextScanlinePtr(int scanline)
//{
//	unsigned char *p;
//
//	if (scanline<0 || scanline>240) return NULL;
//
//	if ( IsFlippedMode() ) {
//		scanline = abs( 239 - ( (long) scanline) );
//	}
//
//	p = ( (unsigned char*) (video_output->pixels) ) + (scanline + 32) * video_output->pitch + 32;
//
//	if (IsFlippedMode() ) {
//		p+= 320;
//	}
//}



//	if (CursorO && scanline>=CursorY && scanline<=CursorY+CursorH) {
//		p = GetScanlinePtr(scanline);
//
//		if (p != NULL) {
//			if (CursorX < 0 || CursorX + CursorW >= 320) return;
//
//      		if ( IsFlippedMode() ) {
//				p-=CursorX;
//				for(int x=0; x<CursorW; x++)
//					*(p--) ^= mainWin->cols[7];
//			} else {
//				p+=CursorX;
//				for(int x=0; x<CursorW; x++)
//					*(p++) ^= mainWin->cols[7];
//			}
//		}
//	}

static inline unsigned char* GetTeletextScanlinePtr(int line)
{
		// Flipped mode
		if ( IsFlippedMode() ) line = 239 - line;
		return  (unsigned char*) (frame_buffer_p->pixels) + (line) * frame_buffer_p->pitch;
}


static void AddTeletextHardwareCursor(int scanline)
{
	unsigned char *p;

//	printf("***%d\n", scanline);

	if (config.settings.disable_cursor) return;

	if (mainWin->OnScreenKeyboardShown() && (CursorY+14) >= 105) return;

	if (CursorO) {
		if (CursorX < 0 || CursorX + CursorW >= 320) return;
			
		for(int y=0; y<CursorH; y++) {
			p = GetTeletextScanlinePtr(CursorY+ /*32+*/ y+14);

			if ( IsFlippedMode() ) {
				p+= 319 - CursorX;
				for(int x=0; x<CursorW; x++) {
					//*(p--) ^= mainWin->cols[7];
					*(p--)= mainWin->cols[7];
				}

			} else {
				p+=CursorX /*+32*/ ;
				for(int x=0; x<CursorW; x++) {
					//*(p++) ^= mainWin->cols[7];
					*(p++)= mainWin->cols[7];
				}
			}
		}
	}
}



// Keep a local copy of the virtual keyboard state, we query this once.
static int on_screen_keyboard = 0;
#define SET_KEYBOARD_SHOWN() on_screen_keyboard = mainWin->OnScreenKeyboardShown();

static inline void DrawLine(long Col, int y, int sx, int width)
{
	static int old_y = -10000;
	static unsigned char *old_p = NULL;

	unsigned char *p;

	if ( y<0 || y>= 240) return;
	if (sx<0 || sx>=320) return;
	//if (mainWin->OnScreenKeyboardShown() && y >= 105) return;
	if (on_screen_keyboard && y>= 105) return;


	if ( old_y == y) {
		p = old_p;
	} else {
		old_y = y;

		p = GetTeletextScanlinePtr(y /*+32*/ );
		old_p = p;

		if (p == NULL) {
			old_y = -10000;
			return;
		}
	}

	if (sx+width >=320) width = 320-sx;

	// Flipped mode
	if ( IsFlippedMode() ) sx = 320 - sx - width;


	p+=sx /*+32*/;

	if (width == 1)
		*p++ = Col;
	else
		memset(p, Col, width);
}



#define GAPSIZE config.settings.vsyncoffset

//static int GAPSIZE = 32;
//static int GAPSIZE = 32+8;




void SetVHoldOffset(unsigned int vhold)
{
	GAPSIZE = vhold;
}

void Video_SetVHoldNormal(void)
{
	SetVHoldOffset(32);
}

void Video_SetVHoldDown(void)
{
	SetVHoldOffset(32+8);
}


//#	define GAPSIZE (32)
static inline unsigned char* GetScanlinePtr(int line)
{
	unsigned char *p;
	unsigned int scanline;

	/* Do not render outside of normal BBC scanlines.
	 * [TODO] Make GAPSIZE (blanking gap size) variable.
  	 */
	if (line<GAPSIZE || line>=256+GAPSIZE) return NULL;

	/* Adjust to 0-255 (remove blanking gap):
 	 */
	line-=GAPSIZE;

	/* Determine if scanline is scaled:
 	 */
	scanline = scanline_mapping_p[line & 0xff];
	if (scanline==0xff || ( mainWin->OnScreenKeyboardShown() && scanline >= 105) ) return NULL;

//	scanline = line;

	/* Flip it for flipped mode:
 	 */
	if ( IsFlippedMode() ) scanline = abs( 239 - ((long)scanline) );

	/* Calc start of scanline rendering area:
 	 */
	p = ( (unsigned char *) (frame_buffer_p->pixels) ) + (scanline /*+ 32*/ ) * frame_buffer_p->pitch /*+ 32*/ ;

	/* Write backwards for flipped mode.
 	 */
//	if ( IsFlippedMode() ) p+= 319;
	if ( IsFlippedMode() ) p+= 320;

	return p;
}
EightUChars* GetNarrowScanline(int y, int *BytesToGo) {
	static EightUChars b={{0,0,0,0,0,0,0,0}};

	EightUChars *pp;
	int i;
	unsigned char *p = NULL;

                if (y > MAX_VIDEO_SCAN_LINES){
                        y = MAX_VIDEO_SCAN_LINES;
                }

		p = GetScanlinePtr(y);
		if (p == NULL) return NULL;

	// Clip output:
	if (HStart>=80)
		*BytesToGo = HStart = 0;
	
	if (HStart + CRTC_HorizontalDisplayed > 80)
		*BytesToGo = 80 - HStart;
	else
		*BytesToGo = CRTC_HorizontalDisplayed;
		
	// Left border:
	pp = (EightUChars*) p;
	for (i = 0; i < HStart; i++)
		*pp++ = b;

	// Right border:
	pp+= *BytesToGo;
	for (i = HStart + CRTC_HorizontalDisplayed; i<80; i++)
		*pp++ = b;

	// Scanline:
	p+= HStart * HSyncModifier;
	return (EightUChars*) p;
}
NarrowResolution* GetNarrowScanlineSDL(int y, int *BytesToGo) {
	static NarrowResolution b={{0,0,0,0}};

	NarrowResolution *pp;
	int i, w;
	unsigned char *p;

	/* Get scanline start:
 	 */
	if ( (p=GetScanlinePtr(y)) == NULL) return NULL;

	/* Clip output:
	 */
	if (HStart>=80)
		w = HStart = 0;
	
	if (HStart + CRTC_HorizontalDisplayed > 80)
		w = 80 - HStart;
	else
		w = CRTC_HorizontalDisplayed;
	
	/* Draw boarders
 	 */
	if ( IsFlippedMode() ) {
		// Left border:
		pp = (NarrowResolution*) p; //pp--;
		for (i = 0; i < HStart; i++)
			*pp-- = b;
	
		// Right border:
		pp-= w;
		for (i = HStart + CRTC_HorizontalDisplayed; i<80; i++)
			*pp-- = b;
	
		// Scanline:
		p-= HStart * HSyncModifier / 2;
	} else {
		// Left border:
		pp = (NarrowResolution*) p;
		for (i = 0; i < HStart; i++)
			*pp++ = b;
	
		// Right border:
		pp+= w;
		for (i = HStart + CRTC_HorizontalDisplayed; i<80; i++)
			*pp++ = b;
	
		// Scanline:
		p+= HStart * HSyncModifier / 2;
	}

	/* Return scanline width:
 	 */
	*BytesToGo = w; return (NarrowResolution*) p;
}

SixteenUChars* GetWideScanline(int y, int *BytesToGo) {
		static SixteenUChars b={{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}};
		SixteenUChars *pp;
		int i;
                unsigned char *p = NULL;

                if (y > MAX_VIDEO_SCAN_LINES){
                        y = MAX_VIDEO_SCAN_LINES;
                }

                //p = GetSDLScreenLinePtr(y);
		p = GetScanlinePtr(y);
		if (p == NULL) return NULL;

		// Dev offset
                //p+=(800*32);
                //p+=32;



	// Clip output:
	if (HStart>=40)
		*BytesToGo = HStart = 0;
	
	if (HStart + CRTC_HorizontalDisplayed > 40)
		*BytesToGo = 40 - HStart;
	else
		*BytesToGo = CRTC_HorizontalDisplayed;

		// Left border:
		pp = (SixteenUChars*) p;
		for (i = 0; i < HStart; i++)
			*pp++ = b;
	
		// Right border:
		pp+= *BytesToGo;
		for (i = HStart + CRTC_HorizontalDisplayed; i<40; i++)
			*pp++ = b;
	
		// Scanline:
		p+= HStart * HSyncModifier;

	return (SixteenUChars*) p;
}
WideResolution* GetWideScanlineSDL(int y, int *BytesToGo) {
	static WideResolution b={{0,0,0,0,0,0,0,0}};
	WideResolution *pp;
	int i, w;
	unsigned char *p;

	/* Get scanline start:
 	 */
	if ( (p=GetScanlinePtr(y)) == NULL) return NULL;

	/* Clip output:
	 */
	if (HStart>=40)
		w = HStart = 0;
	
	if (HStart + CRTC_HorizontalDisplayed > 40)
		w = 40 - HStart;
	else
		w = CRTC_HorizontalDisplayed;
	
	/* Draw boarders:
	 */
	if ( IsFlippedMode() ) {
		/* Left border:
		 */
		pp = (WideResolution*) p; //pp--;
		for (i = 0; i < HStart; i++)
			*pp-- = b;

		/* Right border:
		 */
		pp-= w;
		for (i = HStart + CRTC_HorizontalDisplayed; i<40; i++)
			*pp-- = b;
	
		/* Return scanline width:
		 */
		p-= HStart * HSyncModifier / 2;
	} else {
		/* Left border:
		 */
		pp = (WideResolution*) p;
		for (i = 0; i < HStart; i++)
			*pp++ = b;

		/* Right border:
		 */
		pp+= w;
		for (i = HStart + CRTC_HorizontalDisplayed; i<40; i++)
			*pp++ = b;
	
		/* Return scanline width:
		 */
		p+= HStart * HSyncModifier / 2;
	}

	*BytesToGo = w; return (WideResolution*) p;
}


static void ClearScanline(int scanline)
{
	unsigned char *p = GetScanlinePtr(VideoState.PixmapLine);

	if (p != NULL) {
		if ( IsFlippedMode() ) p-=320;

		memset(p, 0,320);
	}
}



static void AddMC6845HardwareCursor(int scanline)
{
	unsigned char *p;

	if (config.settings.disable_cursor) return;

	if (mainWin->OnScreenKeyboardShown() && (scanline-GAPSIZE) >= 105) return;

	if (CursorO && scanline>=CursorY && scanline<=CursorY+CursorH) {
		p = GetScanlinePtr(scanline);

		if (p != NULL) {
			if (CursorX < 0 || CursorX + CursorW > 320) return;

        		if ( IsFlippedMode() ) {
				p-=CursorX;
				for(int x=0; x<CursorW; x++)
					*(p--) ^= mainWin->cols[7];
			} else {
				p+=CursorX;
				for(int x=0; x<CursorW; x++)
					*(p++) ^= mainWin->cols[7];
			}
		}
	}
}






/* Scanline processing for modes with fast 6845 clock - i.e. narrow pixels  */
static void LowLevelDoScanLineNarrow() {
  unsigned char *CurrentPtr;
  int BytesToGo;
  EightUChars *vidPtr=GetNarrowScanline(VideoState.PixmapLine, &BytesToGo);
  if (vidPtr == NULL) return;

  /* If the step is 4 then each byte corresponds to one entry in the fasttable
     and thus we can copy it really easily (and fast!) */
  CurrentPtr=(unsigned char *)VideoState.DataPtr+VideoState.InCharLineUp;

  /* This should help the compiler - it doesn't need to test for end of loop
     except every 4 entries */
  BytesToGo/=4;
  for(;BytesToGo;CurrentPtr+=32,BytesToGo--) {
    *(vidPtr++)=FastTable[*CurrentPtr];
    *(vidPtr++)=FastTable[*(CurrentPtr+8)];
    *(vidPtr++)=FastTable[*(CurrentPtr+16)];
    *(vidPtr++)=FastTable[*(CurrentPtr+24)];
  };

}; /* LowLevelDoScanLineNarrow() */
static void LowLevelDoScanLineNarrowSDL() {
  unsigned char *CurrentPtr;
  int BytesToGo;
  NarrowResolution *vidPtr = GetNarrowScanlineSDL(VideoState.PixmapLine, &BytesToGo);

  if (vidPtr == NULL) return;
  CurrentPtr=(unsigned char *)VideoState.DataPtr+VideoState.InCharLineUp;
 
  if ( IsFlippedMode() ) {
//	  unsigned char* vidPtr2 = (unsigned char*) vidPtr;
//	  for(;BytesToGo;CurrentPtr+=8,BytesToGo--) {
//                *(vidPtr2--)=FastTableSDL[*CurrentPtr].data[0];
//                *(vidPtr2--)=FastTableSDL[*CurrentPtr].data[1];
//                *(vidPtr2--)=FastTableSDL[*CurrentPtr].data[2];
//                *(vidPtr2--)=FastTableSDL[*CurrentPtr].data[3];

	BytesToGo/=2; vidPtr--;
	for(;BytesToGo;CurrentPtr+=16,BytesToGo--) {
		*(vidPtr--)=FastTableSDL[*CurrentPtr];
		*(vidPtr--)=FastTableSDL[*(CurrentPtr+8)];
          }
  } else {
  	BytesToGo/=2;
	for(;BytesToGo;CurrentPtr+=16,BytesToGo--) {
		*(vidPtr++)=FastTableSDL[*CurrentPtr];
		*(vidPtr++)=FastTableSDL[*(CurrentPtr+8)];
	  }
  }
}



/*--------------------------------------------------------------------------*/
/* Scanline processing for modes with fast 6845 clock - i.e. narrow pixels  */
/* This version handles screen modes where there is not a multiple of 4     */
/* bytes per scanline.                                                      */
static void LowLevelDoScanLineNarrowNot4Bytes() {
  unsigned char *CurrentPtr;
  int BytesToGo;
  EightUChars *vidPtr=GetNarrowScanline(VideoState.PixmapLine, &BytesToGo);
  if (vidPtr == NULL) return;

  /* If the step is 4 then each byte corresponds to one entry in the fasttable
     and thus we can copy it really easily (and fast!) */
  CurrentPtr=(unsigned char *)VideoState.DataPtr+VideoState.InCharLineUp;

  for(;BytesToGo;CurrentPtr+=8,BytesToGo--)
    (vidPtr++)->eightbyte=FastTable[*CurrentPtr].eightbyte;

}; /* LowLevelDoScanLineNarrowNot4Bytes() */
static void LowLevelDoScanLineNarrowNot4BytesSDL() {
  unsigned char *CurrentPtr;
  int BytesToGo;
  NarrowResolution *vidPtr = GetNarrowScanlineSDL(VideoState.PixmapLine, &BytesToGo);

  if (vidPtr == NULL) return;

  CurrentPtr=(unsigned char *)VideoState.DataPtr+VideoState.InCharLineUp;

  if ( IsFlippedMode() ) {
//        unsigned char* vidPtr2 = (unsigned char*) vidPtr;
//	for(;BytesToGo;CurrentPtr+=8,BytesToGo--) {
//	        *(vidPtr2--)=FastTableDWidthSDL[*CurrentPtr].data[0];
//      	*(vidPtr2--)=FastTableDWidthSDL[*CurrentPtr].data[1];
// 	   	*(vidPtr2--)=FastTableDWidthSDL[*CurrentPtr].data[2];
//        	*(vidPtr2--)=FastTableDWidthSDL[*CurrentPtr].data[3];

	vidPtr--;
	for(;BytesToGo;CurrentPtr+=8,BytesToGo--) {
		*(vidPtr--)=FastTableSDL[*CurrentPtr];
    	}
  } else {
    for(;BytesToGo;CurrentPtr+=8,BytesToGo--) {
      *(vidPtr++)=FastTableSDL[*CurrentPtr];
    }
  }
}




/*-----------------------------------------------------------------------------*/
/* Scanline processing for the low clock rate modes                            */
static void LowLevelDoScanLineWide() {
  unsigned char *CurrentPtr;
  int BytesToGo;
//  SixteenUChars *vidPtr=mainWin->GetLinePtr16(VideoState.PixmapLine);
  SixteenUChars *vidPtr=GetWideScanline(VideoState.PixmapLine, &BytesToGo);
  if (vidPtr == NULL) return;

  /* If the step is 4 then each byte corresponds to one entry in the fasttable
     and thus we can copy it really easily (and fast!) */
  CurrentPtr=(unsigned char *)VideoState.DataPtr+VideoState.InCharLineUp;

  /* This should help the compiler - it doesn't need to test for end of loop
     except every 4 entries */
  BytesToGo/=4;
  for(;BytesToGo;CurrentPtr+=32,BytesToGo--) {
    *(vidPtr++)=FastTableDWidth[*CurrentPtr];
    *(vidPtr++)=FastTableDWidth[*(CurrentPtr+8)];
    *(vidPtr++)=FastTableDWidth[*(CurrentPtr+16)];
    *(vidPtr++)=FastTableDWidth[*(CurrentPtr+24)];
  };
}; /* LowLevelDoScanLineWide */
static void LowLevelDoScanLineWideSDL() {
  unsigned char *CurrentPtr;
  int BytesToGo;
  WideResolution *vidPtr = GetWideScanlineSDL(VideoState.PixmapLine, &BytesToGo);

  if (vidPtr == NULL) return;

  CurrentPtr=(unsigned char *)VideoState.DataPtr+VideoState.InCharLineUp;

  if ( IsFlippedMode() ) {
//        unsigned char* vidPtr2 = (unsigned char*) vidPtr;
//
//	for(;BytesToGo;CurrentPtr+=8,BytesToGo--) {
//        *(vidPtr2--)=FastTableDWidthSDL[*CurrentPtr].data[0];
//        *(vidPtr2--)=FastTableDWidthSDL[*CurrentPtr].data[1];
//        *(vidPtr2--)=FastTableDWidthSDL[*CurrentPtr].data[2];
//        *(vidPtr2--)=FastTableDWidthSDL[*CurrentPtr].data[3];
//        *(vidPtr2--)=FastTableDWidthSDL[*CurrentPtr].data[4];
//        *(vidPtr2--)=FastTableDWidthSDL[*CurrentPtr].data[5];
//        *(vidPtr2--)=FastTableDWidthSDL[*CurrentPtr].data[6];
//        *(vidPtr2--)=FastTableDWidthSDL[*CurrentPtr].data[7];
//    }

        BytesToGo/=2; vidPtr--;
        for(;BytesToGo;CurrentPtr+=16,BytesToGo--) {
                *(vidPtr--)=FastTableDWidthSDL[*CurrentPtr];
                *(vidPtr--)=FastTableDWidthSDL[*(CurrentPtr+8)];
	}


  } else {
	  BytesToGo/=2;
	  for(;BytesToGo;CurrentPtr+=16,BytesToGo--) {
	    *(vidPtr++)=FastTableDWidthSDL[*CurrentPtr];
	    *(vidPtr++)=FastTableDWidthSDL[*(CurrentPtr+8)];
	  }
  }
}

/*-----------------------------------------------------------------------------*/
/* Scanline processing for the low clock rate modes                            */
/* This version handles cases where the screen width is not divisible by 4     */
static void LowLevelDoScanLineWideNot4Bytes() {
  unsigned char *CurrentPtr;
  int BytesToGo;
//  SixteenUChars *vidPtr=mainWin->GetLinePtr16(VideoState.PixmapLine);
  SixteenUChars *vidPtr=GetWideScanline(VideoState.PixmapLine, &BytesToGo);
  if (vidPtr == NULL) return;

  CurrentPtr=(unsigned char *)VideoState.DataPtr+VideoState.InCharLineUp;

//  BytesToGo = CRTC_HorizontalDisplayed;
//  if (BytesToGo>40) BytesToGo=40;

  for(;BytesToGo;CurrentPtr+=8,BytesToGo--)
    *(vidPtr++)=FastTableDWidth[*CurrentPtr];

}; /* LowLevelDoScanLineWideNot4Bytes */
static void LowLevelDoScanLineWideNot4BytesSDL() {
  unsigned char *CurrentPtr;
  int BytesToGo;
  WideResolution *vidPtr=GetWideScanlineSDL(VideoState.PixmapLine, &BytesToGo);
  if (vidPtr == NULL) return;

  CurrentPtr=(unsigned char *)VideoState.DataPtr+VideoState.InCharLineUp;

  if ( IsFlippedMode() ) {
//        unsigned char* vidPtr2 = (unsigned char*) vidPtr;
//	for(;BytesToGo;CurrentPtr+=8,BytesToGo--) {
//        *(vidPtr2--)=FastTableDWidthSDL[*CurrentPtr].data[0];
//        *(vidPtr2--)=FastTableDWidthSDL[*CurrentPtr].data[1];
//        *(vidPtr2--)=FastTableDWidthSDL[*CurrentPtr].data[2];
//        *(vidPtr2--)=FastTableDWidthSDL[*CurrentPtr].data[3];
//        *(vidPtr2--)=FastTableDWidthSDL[*CurrentPtr].data[4];
//        *(vidPtr2--)=FastTableDWidthSDL[*CurrentPtr].data[5];
//        *(vidPtr2--)=FastTableDWidthSDL[*CurrentPtr].data[6];
//        *(vidPtr2--)=FastTableDWidthSDL[*CurrentPtr].data[7];
//	    }

	vidPtr--;
        for(;BytesToGo;CurrentPtr+=8,BytesToGo--) {
                *(vidPtr--)=FastTableDWidthSDL[*CurrentPtr];
        }



  } else {
	  for(;BytesToGo;CurrentPtr+=8,BytesToGo--) {
	    *(vidPtr++)=FastTableDWidthSDL[*CurrentPtr];
	  }
  }
}
































/*-------------------------------------------------------------------------------------------------------------*/
/* Do all the pixel rows for one row of teletext characters                                                    */
#define TELETEXT_Y_OFFSET 7
static void DoMode7Row(void) {
  char *CurrentPtr=VideoState.DataPtr;
  int CurrentChar;
  int XStep;
  unsigned char byte;
  unsigned int tmp;

  unsigned char *p;

  unsigned int Foreground=mainWin->cols[7];
  unsigned int ActualForeground;
  unsigned int Background=mainWin->cols[0];
  int Flash=0; /* i.e. steady */
  int DoubleHeight=0; /* Normal */
  int Graphics=0; /* I.e. alpha */
  int Separated=0; /* i.e. continuous graphics */
  int HoldGraph=0; /* I.e. don't hold graphics - I don't know what hold graphics is anyway! */
  // That's ok. Nobody else does either, and nor do I. - Richard Gellman.
  int HoldGraphChar=32; // AHA! we know what it is now, this is the character to "hold" during control codes
  unsigned int CurrentCol[20]={0xffffff,0xffffff,0xffffff,0xffffff,0xffffff,0xffffff,0xffffff,0xffffff,0xffffff
  ,0xffffff,0xffffff,0xffffff,0xffffff,0xffffff,0xffffff,0xffffff,0xffffff,0xffffff,0xffffff,0xffffff};
  int CurrentLen[20]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
  int CurrentStartX[20]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
  int CurrentScanLine;
  int CurrentX=0;
  int CurrentPixel;
  unsigned int col;
  int FontTypeIndex=0; /* 0=alpha, 1=contiguous graphics, 2=separated graphics */


  if (CRTC_HorizontalDisplayed>80) return; /* Not possible on beeb - and would break the double height lookup array */

  SET_KEYBOARD_SHOWN();
  XStep=1;

  for(CurrentChar=0;CurrentChar<CRTC_HorizontalDisplayed;CurrentChar++) {
    byte=CurrentPtr[CurrentChar]; 
    if (byte<32) byte+=128; // fix for naughty programs that use 7-bit control codes - Richard Gellman
    if ((byte & 32) && (Graphics)) HoldGraphChar=byte;
    if ((byte>=128) && (byte<=159)) {
		switch (byte) {
	    case 129:
        case 130:
        case 131:
        case 132:
        case 133:
        case 134:
        case 135:
          Foreground=mainWin->cols[byte-128];
          Graphics=0;
          break;

        case 136:
          Flash=1;
          break;

        case 137:
          Flash=0;
          break;

        case 140:
          DoubleHeight=0;
          break;

        case 141:
		  if (!CurrentLineBottom) NextLineBottom=1;
          DoubleHeight=1;
          break;

        case 145:
        case 146:
        case 147:
        case 148:
        case 149:
        case 150:
        case 151:
          Foreground=mainWin->cols[byte-144];
          Graphics=1;
          break;

        case 152: /* Conceal display - not sure about this */
          Foreground=Background;
          break;

        case 153:
          Separated=0;
          break;

        case 154:
          Separated=1;
          break;

        case 156:
          Background=mainWin->cols[0];
          break;

        case 157:
          Background=Foreground;
          break;

        case 158:
          HoldGraph=1;
          break;

        case 159:
          HoldGraph=0;
          break;
      }; /* Special character switch */
	  // This next line hides any non double height characters on the bottom line
      /* Fudge so that the special character is just displayed in background */
      if ((HoldGraph==1) && (Graphics==1)) byte=HoldGraphChar; else byte=32;
      FontTypeIndex=Graphics?(Separated?2:1):0;
    }; /* test for special character */
	if ((CurrentLineBottom) && ((byte&127)>31) && (!DoubleHeight)) byte=32;
	if ((CRTC_ScanLinesPerChar<=9) || (THalfMode)) TeletextStyle=2; else TeletextStyle=1;
    /* Top bit never reaches character generator */
    byte&=127;
    /* Our font table goes from character 32 up */
    if (byte<32) byte=0; else byte-=32; 

    /* Conceal flashed text if necessary */
    ActualForeground=(Flash && !Mode7FlashOn)?Background:Foreground;
    if (!DoubleHeight) {


//      for(CurrentScanLine=0+(TeletextStyle-1);CurrentScanLine<20;CurrentScanLine+=TeletextStyle) {
//	for(CurrentScanLine=0+(TeletextStyle-1);CurrentScanLine<8;CurrentScanLine+=TeletextStyle) {
	for(CurrentScanLine=0+(TeletextStyle-1);CurrentScanLine<CHAR_HEIGHT;CurrentScanLine+=TeletextStyle) {

        tmp=EM7Font[FontTypeIndex][byte][CurrentScanLine];
		//tmp=1365;
        if ((tmp==0) || (tmp==255)) {
          col=(tmp==0)?Background:ActualForeground;
          if (col==CurrentCol[CurrentScanLine]) CurrentLen[CurrentScanLine]+=CHAR_WIDTH*XStep; else {
            if (CurrentLen[CurrentScanLine])


//              mainWin->doHorizLine(CurrentCol[CurrentScanLine],VideoState.PixmapLine+CurrentScanLine,CurrentStartX[CurrentScanLine],CurrentLen[CurrentScanLine]);
//		DrawLine(unsigned char Col, int y, int sx, int width)

		DrawLine(CurrentCol[CurrentScanLine], VideoState.PixmapLine+CurrentScanLine+TELETEXT_Y_OFFSET, CurrentStartX[CurrentScanLine], CurrentLen[CurrentScanLine]);


            CurrentCol[CurrentScanLine]=col;
            CurrentStartX[CurrentScanLine]=CurrentX;
            CurrentLen[CurrentScanLine]=CHAR_WIDTH*XStep;
          }; /* same colour */
        } else {
//          for(CurrentPixel=0x800;CurrentPixel;CurrentPixel=CurrentPixel>>1) {
//	for(CurrentPixel=0x80;CurrentPixel;CurrentPixel=CurrentPixel>>1) {
	for(CurrentPixel=1<<(CHAR_WIDTH-1);CurrentPixel;CurrentPixel=CurrentPixel>>1) {

            /* Background or foreground ? */
            col=(tmp & CurrentPixel)?ActualForeground:Background;

            /* Do we need to draw ? */
            if (col==CurrentCol[CurrentScanLine]) CurrentLen[CurrentScanLine]+=XStep; else {
              if (CurrentLen[CurrentScanLine]) 

//                mainWin->doHorizLine(CurrentCol[CurrentScanLine],VideoState.PixmapLine+CurrentScanLine,CurrentStartX[CurrentScanLine],CurrentLen[CurrentScanLine]);
		DrawLine(CurrentCol[CurrentScanLine],VideoState.PixmapLine+CurrentScanLine+TELETEXT_Y_OFFSET,CurrentStartX[CurrentScanLine],CurrentLen[CurrentScanLine]);


              CurrentCol[CurrentScanLine]=col;
              CurrentStartX[CurrentScanLine]=CurrentX;
              CurrentLen[CurrentScanLine]=XStep;
            }; /* Fore/back ground */
            CurrentX+=XStep;
          }; /* Pixel within byte */
          CurrentX-=CHAR_WIDTH*XStep;
        }; /* tmp!=0 */
      }; /* Scanline for */
      CurrentX+=CHAR_WIDTH*XStep;
      Mode7DoubleHeightFlags[CurrentChar]=1; /* Not double height - so if the next line is double height it will be top half */








    } else {






      int ActualScanLine;
      /* Double height! */
//      for(CurrentPixel=0x800;CurrentPixel;CurrentPixel=CurrentPixel>>1) {
	for(CurrentPixel=1<<(CHAR_WIDTH-1);CurrentPixel;CurrentPixel=CurrentPixel>>1) {

//        for(CurrentScanLine=0+(TeletextStyle-1);CurrentScanLine<20;CurrentScanLine+=TeletextStyle) {
//	for(CurrentScanLine=0+(TeletextStyle-1);CurrentScanLine<8;CurrentScanLine+=TeletextStyle) {
	for(CurrentScanLine=0+(TeletextStyle-1);CurrentScanLine<CHAR_HEIGHT;CurrentScanLine+=TeletextStyle) {

//          if (!CurrentLineBottom) ActualScanLine=CurrentScanLine >> 1; else ActualScanLine=10+(CurrentScanLine>>1);
//	  if (!CurrentLineBottom) ActualScanLine=CurrentScanLine >> 1; else ActualScanLine=((CHAR_HEIGHT+1)/2)+(CurrentScanLine>>1);
		if (!CurrentLineBottom) ActualScanLine=CurrentScanLine >> 1; else ActualScanLine=((CHAR_HEIGHT>>1))+(CurrentScanLine>>1);
          /* Background or foreground ? */
          col=(EM7Font[FontTypeIndex][byte][ActualScanLine] & CurrentPixel)?ActualForeground:Background;

          /* Do we need to draw ? */
          if (col==CurrentCol[CurrentScanLine]) CurrentLen[CurrentScanLine]+=XStep; else {
            if (CurrentLen[CurrentScanLine])  {


//              mainWin->doHorizLine(CurrentCol[CurrentScanLine],VideoState.PixmapLine+CurrentScanLine,CurrentStartX[CurrentScanLine],CurrentLen[CurrentScanLine]);
		DrawLine(CurrentCol[CurrentScanLine],VideoState.PixmapLine+CurrentScanLine+TELETEXT_Y_OFFSET,CurrentStartX[CurrentScanLine],CurrentLen[CurrentScanLine]);

            };
            CurrentCol[CurrentScanLine]=col;
            CurrentStartX[CurrentScanLine]=CurrentX;
            CurrentLen[CurrentScanLine]=XStep;
          }; /* Fore/back ground */
        }; /* Scanline for */
        CurrentX+=XStep;
      }; /* Pixel within byte */
      Mode7DoubleHeightFlags[CurrentChar]^=1; /* Not double height - so if the next line is double height it will be top half */
	};
  }; /* character loop */






  /* Finish off right bits of scan line */
//  for(CurrentScanLine=0+(TeletextStyle-1);CurrentScanLine<20;CurrentScanLine+=TeletextStyle) {
    for(CurrentScanLine=0+(TeletextStyle-1);CurrentScanLine<CHAR_HEIGHT;CurrentScanLine+=TeletextStyle) {

    if (CurrentLen[CurrentScanLine])
//      mainWin->doHorizLine(CurrentCol[CurrentScanLine],VideoState.PixmapLine+CurrentScanLine,CurrentStartX[CurrentScanLine],CurrentLen[CurrentScanLine]);
	DrawLine(CurrentCol[CurrentScanLine],VideoState.PixmapLine+CurrentScanLine+TELETEXT_Y_OFFSET,CurrentStartX[CurrentScanLine],CurrentLen[CurrentScanLine]);

  };
  CurrentLineBottom=NextLineBottom;
  NextLineBottom=0;

  // Update the lines.
//--  mainWin->updateLines(VideoState.PixmapLine,9);

}; /* DoMode7Row */
/*-------------------------------------------------------------------------------------------------------------*/
/* Actually does the work of decoding beeb memory and plotting the line to X */
static void LowLevelDoScanLine() {
  /* Update acceleration tables */
  DoFastTable();
  if (FastTable_Valid) LineRoutine();

  AddMC6845HardwareCursor(VideoState.PixmapLine);
}; /* LowLevelDoScanLine */

void RedoMPTR(void) {
	if (VideoState.IsTeletext) VideoState.DataPtr=BeebMemPtrWithWrapMo7(ova,ovn);
	if (!VideoState.IsTeletext) VideoState.DataPtr=BeebMemPtrWithWrap(ova,ovn);
	//FastTable_Valid=0;
}

/*-------------------------------------------------------------------------------------------------------------*/
void VideoDoScanLine(void) {
  int l;

	FrameNum=0;

//++
	LOCK(frame_buffer_p);

	IsTeletext =VideoState.IsTeletext;
//<-




//+>
///	set_screen_width();
//<-


  /* cerr << "CharLine=" << VideoState.CharLine << " InCharLineUp=" << VideoState.InCharLineUp << "\n"; */
  if (VideoState.IsTeletext) {

    static int DoCA1Int=0;
    if (DoCA1Int) {
      SysVIATriggerCA1Int(0);
      DoCA1Int=0;
    }; 

  /* Clear the next 8 scan lines */
    if (!FrameNum)

	      if (/* VScreenAdjust>0 && */ VideoState.PixmapLine==0) {

			for (l=0; l<7; l++) {
				DrawLine(0, l, 0, 320);
				if (! mainWin->OnScreenKeyboardShown()) DrawLine(0, 239-l, 0, 320);
		}
		// 7 lines at top, 8 lines at bottom. This is the extra one for the bottom (above loop 0-6)
		if (! mainWin->OnScreenKeyboardShown()) DrawLine(0, 239-7, 0, 320);
	}

    if ((VideoState.CharLine!=-1) && (VideoState.CharLine<CRTC_VerticalDisplayed)) {
      ova=VideoState.Addr; ovn=CRTC_HorizontalDisplayed;
      VideoState.DataPtr=BeebMemPtrWithWrapMo7(VideoState.Addr,CRTC_HorizontalDisplayed);
      VideoState.Addr+=CRTC_HorizontalDisplayed;
      if (!FrameNum) DoMode7Row();
  //      VideoState.PixmapLine+=20;
	VideoState.PixmapLine+=CHAR_HEIGHT;
    };

    /* Move onto next physical scanline as far as timing is concerned */
    VideoState.InCharLineUp+=1;
    if ((VideoState.CharLine==-1 && VideoState.InCharLineUp>=CRTC_VerticalTotalAdjust) ||
        (VideoState.CharLine!=-1)) {
      VideoState.CharLine++;
      VideoState.InCharLineUp=0;
    }

    if (VideoState.CharLine>CRTC_VerticalTotal) {
      // Changed so that whole screen is still visible after *TV255
      VScreenAdjust=-100+(((CRTC_VerticalTotal+1)-(CRTC_VerticalSyncPos-1))*(20/TeletextStyle));
      AdjustVideo();
	      if (!FrameNum) {
	        VideoAddCursor();
//--        VideoAddLEDs();
//	        // Clear rest of screen below virtical total
//	        for (l=VideoState.PixmapLine; l<500/TeletextStyle; ++l)
//	//--          mainWin->doHorizLine(0, l, -36, 800);
//	//++
//			mainWin->doHorizLine(0, l, -72, 800);
//	//<+
//	        mainWin->updateLines(0,(500/TeletextStyle));
	      }
      VideoStartOfFrame();
      VideoState.PreviousLastPixmapLine=VideoState.PixmapLine;
      VideoState.PixmapLine=0;
      SysVIATriggerCA1Int(1);
      DoCA1Int=1;
    }else {
      if (VideoState.CharLine!=-1)  {
        IncTrigger((CRTC_HorizontalTotal+1)*((VideoULA_ControlReg & 16)?1:2)*((VideoState.CharLine==1)?9:10),VideoTriggerCount);
      } else {
        IncTrigger((CRTC_HorizontalTotal+1)*((VideoULA_ControlReg & 16)?1:2),VideoTriggerCount);
      };
    };

	AddTeletextHardwareCursor(VideoState.PixmapLine);








	if (VideoState.CharLine>=CRTC_VerticalTotal)
		SDL_UpdateRect(frame_buffer_p, 0, 0, 320, 240);



	



  } else {
    /* Non teletext. */

    /* Clear the scan line */
//$$    if (!FrameNum)
//$$	    memset(mainWin->GetLinePtr(VideoState.PixmapLine),200,800);

	//	memset(GetSDLScreenLinePtr(VideoState.PixmapLine+32)+32,239,640);

    if (VideoState.CharLine!=-1) {
      if (VideoState.CharLine<CRTC_VerticalDisplayed) {
        // Visible char line, record first line
        if (VideoState.FirstPixmapLine==-1)
          VideoState.FirstPixmapLine=VideoState.PixmapLine;
        if (VideoState.LastPixmapLine<VideoState.PixmapLine)
          VideoState.LastPixmapLine=VideoState.PixmapLine;

        /* If first row of character then get the data pointer from memory */
        if (VideoState.InCharLineUp==0) {
          ova=VideoState.Addr*8; ovn=CRTC_HorizontalDisplayed*8;
          VideoState.DataPtr=BeebMemPtrWithWrap(VideoState.Addr*8,CRTC_HorizontalDisplayed*8);
          VideoState.Addr+=CRTC_HorizontalDisplayed;
        };
  
        if ((VideoState.InCharLineUp<8) && ((CRTC_InterlaceAndDelay & 0x30)!=48)) {
          if (!FrameNum)
            LowLevelDoScanLine();
	} else {
		//unsigned char *p = GetScanlinePtr(VideoState.PixmapLine);
		//if (p!=NULL) memset(p, 0,320);
		ClearScanline(VideoState.PixmapLine);
		//memset(GetSDLScreenLinePtr(VideoState.PixmapLine+32)+32,0,640);
		}
        VideoState.PixmapLine++;
      }
      else {
        // Line not visible.  These are normally the virtical retrace lines:
        //  - lines between the last displayed line and the vsync (at the bottom of the screen) 
        //  - lines between the vsync and the last virtical total line (at the top of the screen)
	
//	unsigned char *p = GetScanlinePtr(VideoState.PixmapLine);
//	if (p!=NULL) memset(p, 0,320);
	ClearScanline(VideoState.PixmapLine);
	//memset(GetSDLScreenLinePtr(VideoState.PixmapLine+32)+32,0,640);
        VideoState.PixmapLine++;
      }
    }
    else {
	//unsigned char *p = GetScanlinePtr(VideoState.PixmapLine);
	//if (p!=NULL) memset(p, 0,320);
	ClearScanline(VideoState.PixmapLine);
	  //memset(GetSDLScreenLinePtr(VideoState.PixmapLine+32)+32,0,640);
	  VideoState.PixmapLine++;
    }

    // Sanity check
    if (VideoState.PixmapLine > MAX_VIDEO_SCAN_LINES)
      VideoState.PixmapLine = MAX_VIDEO_SCAN_LINES;

	// See if we are at the cursor line
	if (CurY == -1 && VideoState.Addr >= (CRTC_CursorPosLow+(CRTC_CursorPosHigh<<8)))
		CurY = VideoState.PixmapLine;

    /* Move onto next physical scanline as far as timing is concerned */
    VideoState.InCharLineUp+=1;
    if (VideoState.VSyncState) {
      if (!(--VideoState.VSyncState)) {
        SysVIATriggerCA1Int(0);
      };
    };

    if ((VideoState.CharLine!=-1 && VideoState.InCharLineUp>CRTC_ScanLinesPerChar) ||
        (VideoState.CharLine==-1 && VideoState.InCharLineUp>=CRTC_VerticalTotalAdjust)) {
      VideoState.CharLine++;
      if ((VideoState.VSyncState==0) && (VideoState.CharLine==CRTC_VerticalSyncPos)) {
        // Nothing displayed?
        if (VideoState.FirstPixmapLine<0)
          VideoState.FirstPixmapLine=0;

        // Limit the number of virtical retrace lines we hide otherwise 
        // screen can get very stretched.
        if (VideoState.FirstPixmapLine>32)
          VideoState.FirstPixmapLine=32;
        if ((VideoState.LastPixmapLine-VideoState.FirstPixmapLine)<255)
          VideoState.LastPixmapLine=255+VideoState.FirstPixmapLine;

        VideoState.PreviousFirstPixmapLine=VideoState.FirstPixmapLine;
        VideoState.FirstPixmapLine=-1;
        VideoState.PreviousLastPixmapLine=VideoState.LastPixmapLine;
        VideoState.LastPixmapLine=0;
        VideoState.PixmapLine=0;

        SysVIATriggerCA1Int(1);
        VideoState.VSyncState=3;
      };

      VideoState.InCharLineUp=0;
    };






    if (VideoState.CharLine>CRTC_VerticalTotal) {

	SDL_UpdateRect(frame_buffer_p, 0, 0, 320, 240);

      VScreenAdjust=0;
      if (!FrameNum) {
        VideoAddCursor();
		CurY=-1;

//        int n = VideoState.PreviousLastPixmapLine-VideoState.PreviousFirstPixmapLine+1;
  //      if (n > MAX_VIDEO_SCAN_LINES) n = MAX_VIDEO_SCAN_LINES;
//	SDL_UpdateRect(frame_buffer_p, 0, 0, 320, 240);



        //mainWin->updateLines(VideoState.PreviousFirstPixmapLine, n);
      }
      VideoStartOfFrame();
      AdjustVideo();
    } else {
      IncTrigger((CRTC_HorizontalTotal+1)*((VideoULA_ControlReg & 16)?1:2),VideoTriggerCount);
    }

  }; /* Teletext if */








	UNLOCK(frame_buffer_p);

}; /* VideoDoScanLine */











/*-------------------------------------------------------------------------------------------------------------*/
/*
void AdjustVideo() {
	ActualScreenWidth=CRTC_HorizontalDisplayed*HSyncModifier;

	if (ActualScreenWidth>800) ActualScreenWidth=800;
	if (ActualScreenWidth<640) ActualScreenWidth=640;

	ScreenAdjust=0;
	return;




	// InitialOffset
	// HStart
	// ScreenAdjust

	// CRTC_HorizontalTotal
	// CRTC_HorizontalSyncPos
	// CRTC_SyncWidth

	// HSyncModifier
	


	InitialOffset=0-(((CRTC_HorizontalTotal+1)/2)-((HSyncModifier==8)?40:20));
	HStart=InitialOffset+((CRTC_HorizontalTotal+1)-(CRTC_HorizontalSyncPos+(CRTC_SyncWidth&15)));
	HStart+=(HSyncModifier==8)?2:1;
	if (TeletextEnabled) HStart+=2;
	if (HStart<0) HStart=0;

//	ScreenAdjust=(HStart*HSyncModifier)+((VScreenAdjust>0)?(VScreenAdjust*800):0);
	ScreenAdjust=(HStart*HSyncModifier);

	


//	ScreenAdjust=0;


//+>
	// Adjust to scale:
	if (screen_width == 320)
		ScreenAdjust>>=1;
//<+
}
*/








/*-------------------------------------------------------------------------------------------------------------*/
void VideoInit(void) {
//  char *environptr;
 
printf("************* VideoInit\n");


 VideoStartOfFrame();
  ova=0x3000; ovn=640;
  VideoState.DataPtr=BeebMemPtrWithWrap(0x3000,640);
  SetTrigger(99,VideoTriggerCount); /* Give time for OS to set mode up before doing anything silly */
  FastTable_Valid=0;
  //--BuildMode7Font();
  BuldMode7Font_8x9();

//--#ifndef WIN32
//--  environptr=getenv("BeebVideoRefreshFreq");
//--  if (environptr!=NULL) Video_RefreshFrequency=atoi(environptr);
//--  if (Video_RefreshFrequency<1) Video_RefreshFrequency=1;
//--#endif

  FrameNum=Video_RefreshFrequency;
  VideoState.PixmapLine=0;
  VideoState.FirstPixmapLine=-1;
  VideoState.PreviousFirstPixmapLine=0;
  VideoState.LastPixmapLine=0;
  VideoState.PreviousLastPixmapLine=256;
  CurY=-1;
  //AdjustVideo();
//  crtclog=fopen("/crtc.log","wb");
}; /* VideoInit */


/*-------------------------------------------------------------------------------------------------------------*/
void CRTCWrite(int Address, int Value) {
  Value&=0xff;
  if (Address & 1) {
//	if (CRTCControlReg<14) { fputc(CRTCControlReg,crtclog); fputc(Value,crtclog); }
//	if (CRTCControlReg<14) {
//		fprintf(crtclog,"%d (%02X) Written to register %d from %04X\n",Value,Value,CRTCControlReg,ProgramCounter);
//	}

//--	if (DebugEnabled && CRTCControlReg<14) {
//--		char info[200];
//--		sprintf(info, "CRTC: Write register %X value %02X", (int)CRTCControlReg, Value);
//--		DebugDisplayTrace(DEBUG_VIDEO, true, info);
//--	}

	switch (CRTCControlReg) {
      case 0:
        CRTC_HorizontalTotal=Value;
		InitialOffset=0-(((CRTC_HorizontalTotal+1)/2)-((HSyncModifier==8)?40:20));
		AdjustVideo();
        break;

      case 1:
        CRTC_HorizontalDisplayed=Value;
		if (CRTC_HorizontalDisplayed > 127)
			CRTC_HorizontalDisplayed = 127;
        FastTable_Valid=0;
		AdjustVideo();
		break;

      case 2:
		CRTC_HorizontalSyncPos=Value;
		AdjustVideo();
        break;

      case 3:
        CRTC_SyncWidth=Value;
		AdjustVideo();
//		fprintf(crtclog,"V Sync width: %d\n",(Value>>4)&15);
        break;

      case 4:
        CRTC_VerticalTotal=Value;
		AdjustVideo();
        break;

      case 5:
        CRTC_VerticalTotalAdjust=Value & 0x1f;  // 5 bit register
//		fprintf(crtclog,"Vertical Total Adjust: %d\n",Value);
		AdjustVideo();
        break;

      case 6:
        CRTC_VerticalDisplayed=Value;
		AdjustVideo();
		break;

      case 7:
        CRTC_VerticalSyncPos=(Value & 0x7f);
        AdjustVideo();
		break;

      case 8:
        CRTC_InterlaceAndDelay=Value;
        break;

      case 9:
        CRTC_ScanLinesPerChar=Value;
        break;

      case 10:
        CRTC_CursorStart=Value;
        break;

      case 11:
        CRTC_CursorEnd=Value;
        break;

      case 12:
        CRTC_ScreenStartHigh=Value;
//		fprintf(crtclog,"Screen now at &%02x%02x\n",Value,CRTC_ScreenStartLow);
        break;

      case 13:
        CRTC_ScreenStartLow=Value;
//		fprintf(crtclog,"Screen now at &%02x%02x\n",CRTC_ScreenStartHigh,Value);
        break;

      case 14:
        CRTC_CursorPosHigh=Value & 0x3f; /* Cursor high only has 6 bits */
        break;

      case 15:
        CRTC_CursorPosLow=Value & 0xff;
        break;

      default: /* In case the user wrote a duff control register value */
        break;
    }; /* CRTCWrite switch */
    /* cerr << "CRTCWrite RegNum=" << int(CRTCControlReg) << " Value=" << Value << "\n"; */
  } else {
    CRTCControlReg=Value & 0x1f;
  };
}; /* CRTCWrite */

/*-------------------------------------------------------------------------------------------------------------*/
int CRTCRead(int Address) {
  if (Address & 1) {
    switch (CRTCControlReg) {
      case 14:
        return(CRTC_CursorPosHigh);
      case 15:
        return(CRTC_CursorPosLow);
      case 16:
        return(CRTC_LightPenHigh); /* Perhaps tie to mouse pointer ? */
      case 17:
        return(CRTC_LightPenLow);
      default:
        break;
    }; /* CRTC Read switch */
  } else {
    return(0); /* Rockwell part has bits 5,6,7 used - bit 6 is set when LPEN is received, bit 5 when in vertical retrace */
  }
return(0);	// Keeep MSVC happy $NRM
}; /* CRTCRead */

/*-------------------------------------------------------------------------------------------------------------*/
void VideoULAWrite(int Address, int Value) {
  int oldValue;
  if (Address & 1) {
    VideoULA_Palette[(Value & 0xf0)>>4]=(Value & 0xf) ^ 7;
    FastTable_Valid=0;
    /* cerr << "Palette reg " << ((Value & 0xf0)>>4) << " now has value " << ((Value & 0xf) ^ 7) << "\n"; */
	//fprintf(crtclog,"Pallette written to at line %d\n",VideoState.PixmapLine);
  } else {
	oldValue=VideoULA_ControlReg;
    VideoULA_ControlReg=Value;
    FastTable_Valid=0; /* Could be more selective and only do it if no.of.cols bit changes */
    /* cerr << "VidULA Ctrl reg write " << hex << Value << "\n"; */
	// Adjust HSyncModifier
	if (VideoULA_ControlReg & 16) HSyncModifier=8; else HSyncModifier=16;
	if (VideoULA_ControlReg & 2) HSyncModifier=12;
	// number of pixels per CRTC character (on our screen)
	if (Value & 2) TeletextEnabled=1; else TeletextEnabled=0;
	if ((Value&2)^(oldValue&2)) { ScreenAdjust=0; }
	AdjustVideo();
  };
}; /* VidULAWrite */

/*-------------------------------------------------------------------------------------------------------------*/
int VideoULARead(int Address) {
  return(Address); /* Read not defined from Video ULA */
}; /* VidULARead */

/*-------------------------------------------------------------------------------------------------------------*/

static void VideoAddCursor(void) {
	static int CurSizes[] = { 2,1,0,0,4,2,0,4 };
	int ScrAddr,CurAddr,RelAddr;
	int CurX;
	int CurSize;
	int CurStart, CurEnd;

	CursorO=0;

	/* Check if cursor has been hidden */
	if ((VideoULA_ControlReg & 0xe0) == 0 || (CRTC_CursorStart & 0x40) == 0)
		return;

	/* Use clock bit and cursor bits to work out size */
	if (VideoULA_ControlReg & 0x80)
		CurSize = CurSizes[(VideoULA_ControlReg & 0x70)>>4] * 8;
	else
		CurSize = 2 * 8; /* Mode 7 */

	if (VideoState.IsTeletext)
	{
		ScrAddr=CRTC_ScreenStartLow+(((CRTC_ScreenStartHigh ^ 0x20) + 0x74 & 0xff)<<8);
		CurAddr=CRTC_CursorPosLow+(((CRTC_CursorPosHigh ^ 0x20) + 0x74 & 0xff)<<8);

		CurStart = (CRTC_CursorStart & 0x1f) / 2;
		CurEnd = CRTC_CursorEnd ;
		CurSize-=4;
	}
	else
	{
		ScrAddr=CRTC_ScreenStartLow+(CRTC_ScreenStartHigh<<8);
		CurAddr=CRTC_CursorPosLow+(CRTC_CursorPosHigh<<8);

		CurStart = CRTC_CursorStart & 0x1f;
		CurEnd = CRTC_CursorEnd;
	}
		
	RelAddr=CurAddr-ScrAddr;
	if (RelAddr < 0 || CRTC_HorizontalDisplayed == 0)
		return;

	/* Work out char positions */
	CurX = RelAddr % CRTC_HorizontalDisplayed;

	/* Convert to pixel positions */
	if (VideoState.IsTeletext) {
	//	CurX = CurX * 12;
		CurX*=8;	

	//	CurY = (RelAddr / CRTC_HorizontalDisplayed) * 20 + 9;
		CurY = (RelAddr / CRTC_HorizontalDisplayed) * 9;
		//printf("%d\n", CurY);
	}
	else {
		//CurX = CurX * HSyncModifier;
		CurX = CurX * HSyncModifier;
	}

	/* Limit cursor size */ // This should be 11, not 9 - Richard Gellman
	if (CurEnd > 11)
		CurEnd = 11;

	if (CurX + CurSize >= 640)
		CurSize = 640 - CurX;

	// Cursor delay
	CurX+=((CRTC_InterlaceAndDelay&192)>>6)*HSyncModifier;
	if (VideoState.IsTeletext) CurX-=2*HSyncModifier;
//	if (CurSize > 0)
//	{
//		for (int y = CurStart; y <= CurEnd && CurY + y < 500; ++y)
//		{
//			if (CurY + y >= 0) {
//				if (CursorOnState) {
//
//cursor: 7 158 12 12
//cursor: 7 159 12 12
//cursor: 7 160 12 12
//
//
//				 //printf("cursor: %d %d %d %d\n", mainWin->cols[7], CurY + y, CurX, CurSize);
//				 //mainWin->doInvHorizLine(mainWin->cols[7], CurY + y, CurX, CurSize);
//				}
//			}
//		}
//	}

	//-------------------------------------------------------------------------------
	if (VideoState.IsTeletext) {
		CursorX=CurX;
		CursorY=CurY; //+CurStart-2;
		CursorH=CurEnd-CurStart;
		CursorW=CurSize/2;
		CursorO=CursorOnState;
	} else {
		CursorX=CurX/2;
		CursorY=CurY+CurStart-2; /* -2 for cursor to always be on screen. (make this optional) */
		CursorH=CurEnd-CurStart;
		CursorW=CurSize/2;
		CursorO=CursorOnState;
	}
	//-------------------------------------------------------------------------------

}

//--void VideoAddLEDs(void) {
//--	// now add some keyboard leds
//--	if (LEDs.ShowKB) {
//--		if (MachineType==3) mainWin->doLED(4,TRUE); else mainWin->doLED(4,LEDs.Motor);
//--		mainWin->doLED(14,LEDs.CapsLock);
//--		mainWin->doLED(24,LEDs.ShiftLock);
//--	}
//--	if (LEDs.ShowDisc)  {
//--//->		mainWin->doLED((TeletextEnabled)?532:618,LEDs.Disc0);
//--//--		mainWin->doLED((TeletextEnabled)?542:628,LEDs.Disc1);
//--//++
//--		mainWin->doLED(618,LEDs.Disc0);
//--		mainWin->doLED(628,LEDs.Disc1);
//--//<-
//--	}
//--}

/*-------------------------------------------------------------------------*/
void SaveVideoUEF(FILE *SUEF) {
	fput16(0x0468,SUEF);
	fput32(47,SUEF);
	// save the registers now
	fputc(CRTC_HorizontalTotal,SUEF);
	fputc(CRTC_HorizontalDisplayed,SUEF);
	fputc(CRTC_HorizontalSyncPos,SUEF);
	fputc(CRTC_SyncWidth,SUEF);
	fputc(CRTC_VerticalTotal,SUEF);
	fputc(CRTC_VerticalTotalAdjust,SUEF);
	fputc(CRTC_VerticalDisplayed,SUEF);
	fputc(CRTC_VerticalSyncPos,SUEF);
	fputc(CRTC_InterlaceAndDelay,SUEF);
	fputc(CRTC_ScanLinesPerChar,SUEF);
	fputc(CRTC_CursorStart,SUEF);
	fputc(CRTC_CursorEnd,SUEF);
	fputc(CRTC_ScreenStartHigh,SUEF);
	fputc(CRTC_ScreenStartLow,SUEF);
	fputc(CRTC_CursorPosHigh,SUEF);
	fputc(CRTC_CursorPosLow,SUEF);
	fputc(CRTC_LightPenHigh,SUEF);
	fputc(CRTC_LightPenLow,SUEF);
	// VIDPROC
	fputc(VideoULA_ControlReg,SUEF);
	for (int col = 0; col < 16; ++col)
		fputc(VideoULA_Palette[col] ^ 7,SUEF); /* Use real ULA values */
	fput16(ActualScreenWidth,SUEF);
	fput32(ScreenAdjust,SUEF);
	fputc(CRTCControlReg,SUEF);
	fputc(TeletextStyle,SUEF);
	fput32(0,SUEF); // Pad out
}

void LoadVideoUEF(FILE *SUEF) {
	CRTC_HorizontalTotal=fgetc(SUEF);
	CRTC_HorizontalDisplayed=fgetc(SUEF);
	CRTC_HorizontalSyncPos=fgetc(SUEF);
	CRTC_SyncWidth=fgetc(SUEF);
	CRTC_VerticalTotal=fgetc(SUEF);
	CRTC_VerticalTotalAdjust=fgetc(SUEF);
	CRTC_VerticalDisplayed=fgetc(SUEF);
	CRTC_VerticalSyncPos=fgetc(SUEF);
	CRTC_InterlaceAndDelay=fgetc(SUEF);
	CRTC_ScanLinesPerChar=fgetc(SUEF);
	CRTC_CursorStart=fgetc(SUEF);
	CRTC_CursorEnd=fgetc(SUEF);
	CRTC_ScreenStartHigh=fgetc(SUEF);
	CRTC_ScreenStartLow=fgetc(SUEF);
	CRTC_CursorPosHigh=fgetc(SUEF);
	CRTC_CursorPosLow=fgetc(SUEF);
	CRTC_LightPenHigh=fgetc(SUEF);
	CRTC_LightPenLow=fgetc(SUEF);
	// VIDPROC
	VideoULA_ControlReg=fgetc(SUEF);
	for (int col = 0; col < 16; ++col)
		VideoULA_Palette[col]=fgetc(SUEF)^7; /* Use real ULA values */
	ActualScreenWidth=fget16(SUEF);
	ScreenAdjust=fget32(SUEF);
	CRTCControlReg=fgetc(SUEF);
	TeletextStyle=fgetc(SUEF);
	VideoInit();
	if (VideoULA_ControlReg & 2) TeletextEnabled=1; else TeletextEnabled=0;
	if (VideoULA_ControlReg & 16) HSyncModifier=8; else HSyncModifier=16;
	if (VideoULA_ControlReg & 2) HSyncModifier=12;
	//SetTrigger(99,VideoTriggerCount);
}

/*-------------------------------------------------------------------------*/
void video_dumpstate(void) {
  int tmp;
  cerr << "video:\n";
  cerr << "  VideoULA_ControlReg=" << int(VideoULA_ControlReg) << "\n";
  cerr << "  VideoULA_Palette=";
  for(tmp=0;tmp<16;tmp++)
    cerr << int(VideoULA_Palette[tmp]) << " ";
  cerr << "\n  CRTC Control=" << int(CRTCControlReg) << "\n";
  cerr << "  CRTC_HorizontalTotal=" << int(CRTC_HorizontalTotal) << "\n";
  cerr << "  CRTC_HorizontalDisplayed=" << int(CRTC_HorizontalDisplayed)<< "\n";
  cerr << "  CRTC_HorizontalSyncPos=" << int(CRTC_HorizontalSyncPos)<< "\n";
  cerr << "  CRTC_SyncWidth=" << int(CRTC_SyncWidth)<< "\n";
  cerr << "  CRTC_VerticalTotal=" << int(CRTC_VerticalTotal)<< "\n";
  cerr << "  CRTC_VerticalTotalAdjust=" << int(CRTC_VerticalTotalAdjust)<< "\n";
  cerr << "  CRTC_VerticalDisplayed=" << int(CRTC_VerticalDisplayed)<< "\n";
  cerr << "  CRTC_VerticalSyncPos=" << int(CRTC_VerticalSyncPos)<< "\n";
  cerr << "  CRTC_InterlaceAndDelay=" << int(CRTC_InterlaceAndDelay)<< "\n";
  cerr << "  CRTC_ScanLinesPerChar=" << int(CRTC_ScanLinesPerChar)<< "\n";
  cerr << "  CRTC_CursorStart=" << int(CRTC_CursorStart)<< "\n";
  cerr << "  CRTC_CursorEnd=" << int(CRTC_CursorEnd)<< "\n";
  cerr << "  CRTC_ScreenStartHigh=" << int(CRTC_ScreenStartHigh)<< "\n";
  cerr << "  CRTC_ScreenStartLow=" << int(CRTC_ScreenStartLow)<< "\n";
  cerr << "  CRTC_CursorPosHigh=" << int(CRTC_CursorPosHigh)<< "\n";
  cerr << "  CRTC_CursorPosLow=" << int(CRTC_CursorPosLow)<< "\n";
  cerr << "  CRTC_LightPenHigh=" << int(CRTC_LightPenHigh)<< "\n";
  cerr << "  CRTC_LightPenLow=" << int(CRTC_LightPenLow)<< "\n";
}; /* video_dumpstate */

//--void DebugVideoState()
//--{
//--	char info[200];
//--
//--	DebugDisplayInfo("");
//--
//--	sprintf(info, "CRTC: HTot=%02X HDis=%02X HSyn=%02X SWid=%02X VTot=%02X VAdj=%02X VDis=%02X VSyn=%02X",
//--		(int)CRTC_HorizontalTotal,
//--		(int)CRTC_HorizontalDisplayed,
//--		(int)CRTC_HorizontalSyncPos,
//--		(int)CRTC_SyncWidth,
//--		(int)CRTC_VerticalTotal,
//--		(int)CRTC_VerticalTotalAdjust,
//--		(int)CRTC_VerticalDisplayed,
//--		(int)CRTC_VerticalSyncPos);
//--	DebugDisplayInfo(info);
//--
//--	sprintf(info, "CRTC: IntD=%02X SLCh=%02X CurS=%02X CurE=%02X ScrS=%02X%02X CurP=%02X%02X VidULA=%02X",
//--		(int)CRTC_InterlaceAndDelay,
//--		(int)CRTC_ScanLinesPerChar,
//--		(int)CRTC_CursorStart,
//--		(int)CRTC_CursorEnd,
//--		(int)CRTC_ScreenStartHigh,
//--		(int)CRTC_ScreenStartLow,
//--		(int)CRTC_CursorPosHigh,
//--		(int)CRTC_CursorPosLow,
//--		(int)VideoULA_ControlReg);
//--	DebugDisplayInfo(info);
//--}

/*-------------------------------------------------------------------------*/
void VideoGetText(char *text, int line)
{
	unsigned char c;
	int x;

	text[0] = 0;
	text[1] = 0;

	if (!VideoState.IsTeletext || line >= CRTC_VerticalDisplayed)
		return;

	char *dataPtr = BeebMemPtrWithWrapMo7(
		VideoState.StartAddr + line * CRTC_HorizontalDisplayed,
		CRTC_HorizontalDisplayed);

	for (x = 0; x < CRTC_HorizontalDisplayed; ++x)
	{
		c = dataPtr[x];
		if (isprint(c))
			text[x] = c;
		else
			text[x] = ' ';
	}
	text[x] = '\0';
}

