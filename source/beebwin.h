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
/* The window which the beeb emulator displays stuff in */
#ifndef BEEBWIN_HEADER
#define BEEBWIN_HEADER

#include <string.h>
#include <stdlib.h>
#include <SDL.h>

#include "port.h"
#include "sdl.h"

extern SDL_Surface *frame_buffer_p;

char* getScreenBufRow_p(unsigned int);


typedef union {
	unsigned char data[8];
  EightByteType eightbyte;
} EightUChars;

typedef union {
	unsigned char data[16];
  EightByteType eightbytes[2];
} SixteenUChars;



typedef union {
	unsigned char	data[4];
	Uint32		fourbyte;
} NarrowResolution;

typedef union {
	unsigned char	data[8];
	Uint32		fourbytes[2];
} WideResolution;




class BeebWin {
	unsigned int CurrentScanLine;
	unsigned int Frame;

	int onscreen_keyboard;
	int screen_orientation;

	int tape_led_state, shift_led_state, caps_led_state;

	int OldAutoRepeat; /* -1 means we don't know, 0 and 1 are as returned from XGetKeyboardControl */
	int DataSize;
public:
	unsigned char cols[8]; /* Beeb colour lookup */
	int ShiftBooted;
  BeebWin();
	~BeebWin();

	void SetCapsLED(int state);
	void SetShiftLED(int state);
	void SetTapeLED(int state);

	int GetCapsLED(void);
	int GetShiftLED(void);
	int GetTapeLED(void);

//	void RepaintLEDs(void);
//
//	void RepaintCapsLED(void);
//	void RepaintShiftLED(void);
//	void RepaintTapeLED(void);

	void Reset(void);

	void FindPaletteColors(void);
	char *imageData();
	int bytesPerLine();
	void updateLines(int starty, int nlines);

	int OnScreenKeyboardShown(void);
	void HideOnScreenKeyboard(void);
	void ShowOnScreenKeyboard(void);

	int ScreenIsReversed(void);
	void ReverseScreenOrientation(void);
	void NormaliseScreenOrientation(void);



	unsigned int currentscanline(void);
	unsigned int currentframe(void);

inline void Sleep(unsigned int delay)
{
	SDL_Sleep(delay);
}

inline unsigned int GetTicks(void)
{
	return (unsigned int) SDL_GetTicks();
}


inline void doHorizLine(unsigned long Col, int y, int sx, int width)
{
        char *p;

        /* Quit if rendering outside frame buffer:
         */
        if (y<0 || y>=240 || sx<0 || sx>=320 || width<=0) return;


        if (sx+width>=320) {
                width = 320-sx;
                if (width<=0) return;
        }

	if (onscreen_keyboard && y >= 105)
		return;

	if (screen_orientation) {
		y=239-y;
		sx=(320-sx)-width;
	}

	if (y<0 || y>=240 || sx<0 || sx>=320 || width<=0) {printf("TELEXT PAINTING OUT OF BOUNDS y=%d sx=%d width=%d\n", y, sx, width); return;}

	p = getScreenBufRow_p(y);
        memset(p + sx, Col, width);


}

inline void doHorizLine(unsigned long Col, int offset, int width) {
//		if ((offset+width)<DataSize) return;
//		memset(si.imageData()+offset*2+offset,Col,width);
	}; /* BeebWin::doHorizLine */



	EightUChars *GetLinePtr(int y);
	EightUChars *GetLine2Ptr(int y);
	SixteenUChars *GetLinePtr16(int y);
	SixteenUChars *GetLine2Ptr16(int y);
}; /* BeebWin */

#endif
