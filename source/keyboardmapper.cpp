/* The BeebEm GUI pages:
 *
 * A simple GUI keyboard mapper for the GP2X.
 */

#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>
#include <stdio.h>

#include <gui/log.h>
#include <SDL.h>

#include "sysvia.h"
#include "main.h"
#include "disc8271.h"
#include "video.h"

#include "beebconfig.h"
#include "beebconfig_data.h"
#include "beebem_pages.h"
#include "keyboard.h"
#include "button.h"

#include "6502core.h"

#include "sdl.h"

#include "screensaver.h"
#include "stick.h"

typedef struct {
	EG_Window	*win;

	EG_Widget	*lbl_title;
	EG_Widget	*box;

	EG_Widget       *but_key[11+16+16+15+15+2];
} BBCKeyboard;


static BBCKeyboard kbd, *kbd_p = NULL;

static int done, key;

//#define CALC_LOC(xo, yo, x, y, w, h) ( (SDL_Rect) {(x*7)+xo, (y*9)+yo, w*(7)-2, h*(9)-1} )
//#define CALC_LOC(xo, yo, x, y, w, h) ( (SDL_Rect) {(x*7)+xo, (y*12)+yo, w*(7)-1, h*(12)-1} )

SDL_Rect CALC_LOC(int xo, int yo, int x, int y, int w, int h)
{
	return (SDL_Rect) {(x*7)+xo, (y*12)+yo, w*(7)-1, h*(12)-1};
}


#define KEY_COUNT 74
static struct {
	int x, y, w, h;
	int l,r,u,d;
	char name[15+1];
	int key;
} bbc_keyboard_mapping[KEY_COUNT] = {

	/* 0=F0  1=F1  2=F2  3=F3  4=F4  5=F5  6=F6  7=F7  8=F8  9=F9
 	*/
#define	R1_OFFSET	10

	{0+R1_OFFSET,0,2,1,	-1,  1, -1, 13,		"f0"	, BBC_F0}, /*  0 */
	{2+R1_OFFSET,0,2,1,	 0,  2, -1, 14, 	"f1"	, BBC_F1}, /*  1 */
	{4+R1_OFFSET,0,2,1,	 1,  3, -1, 15, 	"f2"	, BBC_F2}, /*  2 */
	{6+R1_OFFSET,0,2,1,	 2,  4, -1, 16, 	"f3"	, BBC_F3}, /*  3 */
	{8+R1_OFFSET,0,2,1,	 3,  5, -1, 17, 	"f4"	, BBC_F4}, /*  4 */
	{10+R1_OFFSET,0,2,1,	 4,  6, -1, 18,		"f5"	, BBC_F5}, /*  5 */
	{12+R1_OFFSET,0,2,1,	 5,  7, -1, 19,		"f6"	, BBC_F6}, /*  6 */
	{14+R1_OFFSET,0,2,1,	 6,  8, -1, 20, 	"f7"	, BBC_F7}, /*  7 */
	{16+R1_OFFSET,0,2,1,	 7,  9, -1, 21, 	"f8"	, BBC_F8}, /*  8 */
	{18+R1_OFFSET,0,2,1,	 8, -1, -1, 22,		"f9"	, BBC_F9}, /*  9 */

	/* 10=ESCAPE  11=1!  12=2"  13=3#  14=4$  15=5%  16=6&  17=7'  18=8(  19=9)  20=0  21==- 22=~^ 23=|\ 24=LEFT 25=RIGHT
	 */
	{0,1,7,1,	-1, 11, -1, 26,		"ESC"	, BBC_ESCAPE}, /* 10 */
	{7,1,2,1,	10, 12, -1, 27,		"1!"	, BBC_1}, /* 11 */
	{9,1,2,1,	11, 13, -1, 28,		"2\""	, BBC_2}, /* 12 */
	{11,1,2,1,	12, 14, 0, 29,		"3#"	, BBC_3}, /* 13 */
	{13,1,2,1,	13, 15, 1, 30,		"4$"	, BBC_4}, /* 14 */
	{15,1,2,1,	14, 16, 2, 31,		"5%"	, BBC_5}, /* 15 */
	{17,1,2,1,	15, 17, 3, 32,		"6&"	, BBC_6}, /* 16 */
	{19,1,2,1,	16, 18, 4, 33,		"7'"	, BBC_7}, /* 17 */
	{21,1,2,1,	17, 19, 5, 34,		"8("	, BBC_8}, /* 18 */
	{23,1,2,1,	18, 20, 6, 35,		"9)"	, BBC_9}, /* 19 */
	{25,1,2,1,	19, 21, 7, 36,		"0"	, BBC_0}, /* 20 */
	{27,1,2,1,	20, 22, 8, 37,		"=-"	, BBC_EQUALS}, /* 21 */
	{29,1,2,1,	21, 23, 9, 38,		"~^"	, BBC_TILDE}, /* 22 */
	{31,1,2,1,	22, 24, -1, 39,		"|\\"	, BBC_BAR}, /* 23 */
	{33,1,3,1,	23, 25, -1, 40,		"\004"	, BBC_LEFT}, /* 24 */
	{36,1,3,1,	24, -1, -1, 41,		"\003"	, BBC_RIGHT}, /* 25 */

	/* 26=TAB 27=Q   28=W   29=E   30=R   31=T   32=Y   33=U   34=I   35=O   36=P  37=@  38={[ 39=<POUND>_ 40=UP 41=DOWN
	 */
	{0,2,8,1,	-1, 27, 10, 43,		"TAB"	, BBC_TAB}, /* 26 */
	{8,2,2,1,	26, 28, 11, 44,		"Q"	, BBC_Q}, /* 27 */
	{10,2,2,1,	27, 29, 12, 45,		"W"	, BBC_W}, /* 28 */
	{12,2,2,1,	28, 30, 13, 46,		"E"	, BBC_E}, /* 29 */
	{14,2,2,1,	29, 31, 14, 47,		"R"	, BBC_R}, /* 30 */
	{16,2,2,1,	30, 32, 15, 48,		"T"	, BBC_T}, /* 31 */
	{18,2,2,1,	31, 33, 16, 49,		"Y"	, BBC_Y}, /* 32 */
	{20,2,2,1,	32, 34, 17, 50,		"U"	, BBC_U}, /* 33 */
	{22,2,2,1,	33, 35, 18, 51,		"I"	, BBC_I}, /* 34 */
	{24,2,2,1,	34, 36, 19, 52,		"O"	, BBC_O}, /* 35 */
	{26,2,2,1,	35, 37, 20, 53,		"P"	, BBC_P}, /* 36 */
	{28,2,2,1,	36, 38, 21, 54,		"@"	, BBC_AT}, /* 37 */
	{30,2,2,1,	37, 39, 22, 55,		"{["	, BBC_LEFTCURLYBRACKET}, /* 38 */
	{32,2,2,1,	38, 40, 23, 56,		" _"	, BBC_POUND}, /* 39 */
	{34,2,3,1,	39, 41, 24, 56,		"\001"	, BBC_UP}, /* 40 */
	{37,2,3,1,	40, -1, 25, 56,		"\002"	, BBC_DOWN}, /* 41 */

	/* CAPS-LOCK CTRL A S   D   F   G   H   J   K   L   +; +; *: }] RETURN
 	 */
	{0,3,5,1,	-1, 43, 26, 57,		"C-LOCK"	, BBC_CAPSLOCK}, /* 42 */
	{5,3,4,1,	42, 44, 26, 58,		"CTRL"	, BBC_CTRL}, /* 43 */
	{9,3,2,1,	43, 45, 27, 59,		"A"	, BBC_A}, /* 44 */
	{11,3,2,1,	44, 46, 28, 60,		"S"	, BBC_S}, /* 45 */
	{13,3,2,1,	45, 47, 29, 61,		"D"	, BBC_D}, /* 46 */
	{15,3,2,1,	46, 48, 30, 62,		"F"	, BBC_F}, /* 47 */
	{17,3,2,1,	47, 49, 31, 63,		"G"	, BBC_G}, /* 48 */
	{19,3,2,1,	48, 50, 32, 64,		"H"	, BBC_H}, /* 49 */
	{21,3,2,1,	49, 51, 33, 65,		"J"	, BBC_J}, /* 50 */
	{23,3,2,1,	50, 52, 34, 66,		"K"	, BBC_K}, /* 51 */
	{25,3,2,1,	51, 53, 35, 67,		"L"	, BBC_L}, /* 52 */
	{27,3,2,1,	52, 54, 36, 68,		"+;"	, BBC_PLUS}, /* 53 */
	{29,3,2,1,	53, 55, 37, 69,		"*:"	, BBC_ASTERIX}, /* 54 */
	{31,3,2,1,	54, 56, 38, 69,		"}]"	, BBC_RIGHTCURLYBRACKET}, /* 55 */
	{33,3,5,1,	55, -1, 40, 70,		"RETURN"	, BBC_RETURN}, /* 56 */

	/* SHIFT-LOCK SHIFT	Z   X   C   V   B   N   M   <,  >.  ?/ <, >. ?/ SHIFT DELETE COPY
 	 */
	{0,4,5,1,	-1, 58, 42, -1,		"S-LOCK", BBC_SHIFTLOCK}, /* 57 */
	{5,4,5,1,	57, 59, 43, -1,		"SHIFT"	, BBC_SHIFT}, /* 58 */
	{10,4,2,1,	58, 60, 44, 71,		"Z"	, BBC_Z}, /* 59 */
	{12,4,2,1,	59, 61, 45, 71,		"X"	, BBC_X}, /* 60 */
	{14,4,2,1,	60, 62, 46, 71,		"C"	, BBC_C}, /* 61 */
	{16,4,2,1,	61, 63, 47, 71,		"V"	, BBC_V}, /* 62 */
	{18,4,2,1,	62, 64, 48, 72,		"B"	, BBC_B}, /* 63 */
	{20,4,2,1,	63, 65, 49, 72,		"N"	, BBC_N}, /* 64 */
	{22,4,2,1,	64, 66, 50, 72,		"M"	, BBC_M}, /* 65 */
	{24,4,2,1,	65, 67, 51, 73,		"<,"	, BBC_LEFTANGLEBRACKET}, /* 66 */
	{26,4,2,1,	66, 68, 52, 73,		">."	, BBC_RIGHTANGLEBRACKET}, /* 67 */
	{28,4,2,1,	67, 69, 53, 73,		"?/"	, BBC_QUESTIONMARK}, /* 68 */
	{30,4,4,1,	68, 70, 54, -1,		"DEL", BBC_DELETE}, /* 69 */
	{34,4,4,1,	69, -1, 56, -1,		"COPY"	, BBC_COPY}, /* 70 */

	/*	SPACE			NOTHING VOLUME
 	 */
	{10,5,8,1,	-1, 72, 61, -1,		"SPACE", BBC_SPACE}, /* 71 */
	{18,5,6,1,	71, 73, 64, -1,		"NOTHING", KEY_NOTHING}, /* 72 */
	{24,5,6,1,	72, -1, 67, -1,		"VOLUME", KEY_USEASVOLUME}, /* 73 */

};

/* Configuration state:
 */
//extern BeebConfig config;


/* Default GUI size (fixed and none scalable for GP2x, unlike UNIX BeebEm):
 */
#define GUI_WIDTH       320
#define GUI_HEIGHT      240


static EG_BOOL	BBCKeyboard_Make(SDL_Surface *frame_buffer_p /*, int current_key*/);


int BBCKeyboard_Show(SDL_Surface *frame_buffer_p, int current_key)
{
	EG_Window *dialog_window_p;
	SDL_Event event;
	int i, old_delay;

	/* Create BBC Keyboard page:
	 */
	if (kbd_p == NULL) {
		if (BBCKeyboard_Make(frame_buffer_p /*, current_key*/) != EG_TRUE) {
			EG_Log(EG_LOG_FATAL
			 , dL"Unable to create BBC Keyboard page - exiting!", dR);
			return -1;
		}
		kbd_p = &kbd;
	}

	/* Set focus to mapped key:
	 */
	for(i=0; i<KEY_COUNT; i++)
		if (bbc_keyboard_mapping[i].key == current_key)
			EG_Window_SetFocusToThisWidget(kbd.but_key[i]);
	


	/* Show the window:
	 */

	dialog_window_p = kbd.win;
	EG_Window_Show(dialog_window_p);

	Button_Reset();
	old_delay = Button_GetAutoRepeatDelay();
	Button_SetAutoRepeatDelay(5);

	key = -1;
	done = 0;
	do {
		while ( SDL_PollEvent(&event) ) {

			if (event.type != SDL_USEREVENT) {
				Button_TranslateEvent(&event, mainWin->ScreenIsReversed());
			} else {
				switch ( (intptr_t) event.user.data1) {
				case SDL_USEREVENT_BUTTON_QUIT:
					done = 1;
					break;
				case SDL_USEREVENT_BUTTON_LEFT:
					EG_Window_MoveSmartFocusLeft(dialog_window_p);
					break;

				case SDL_USEREVENT_BUTTON_RIGHT:
					EG_Window_MoveSmartFocusRight(dialog_window_p);
					break;

				case SDL_USEREVENT_BUTTON_UP:
					EG_Window_MoveSmartFocusUp(dialog_window_p);
					break;

				case SDL_USEREVENT_BUTTON_DOWN:
					EG_Window_MoveSmartFocusDown(dialog_window_p);
					break;

				case SDL_USEREVENT_BUTTON_CLICK_DOWN:
					EG_Window_Trigger_Select_Down(dialog_window_p);
					break;

				case SDL_USEREVENT_BUTTON_CLICK_UP:
					EG_Window_Trigger_Select_Up(dialog_window_p);
					break;
				}
			}

			/* Send this event to the window:
		 	 */
			EG_Window_ProcessEvent(dialog_window_p, &event, 0, 0);
		}

		/* Slight delay:
		 */
		SDL_Delay(40);	// 25fps
		Button_RaiseAutoRepeatEvents();
		SDL_UpdateRect(frame_buffer_p, 0,0,0,0);

		/* Make sure EG house keeping is triggered every 40ms:
		 */
		EG_Window_ProcessEvent(dialog_window_p, NULL, 0, 0);

	} while (! done );

	Button_Reset();
	Button_SetAutoRepeatDelay(old_delay);

	EG_Window_Hide(dialog_window_p);
//	BBCKeyboard_Destroy();

	return key;
}


static void BBCKeyboard_Callback_Key(EG_Widget*, void* user_p)
{
        int i = ((int) (intptr_t) user_p);

	if (i >= 0 && i < KEY_COUNT)
		key = bbc_keyboard_mapping[i].key;
	else
		key = -1; 

	/* Terminate processing loop:
	 */
	done = 1;


	printf("key = %d %d\n", i, key);
}

static EG_BOOL BBCKeyboard_Make(SDL_Surface *frame_buffer_p /*, int current_key*/)
{
	SDL_Color c, c2;
	SDL_Rect r, r2, r3, r4;
	int i /*, focused_index=-1*/;

	/* Clear the keyboard struct:
	 */
	memset(&kbd, 0, sizeof(kbd));

	// Map pound sign
	bbc_keyboard_mapping[39].name[0] = 163;


        /* Default colors:
         */
//	c = (SDL_Color) {96, 64,255-64, 0};
//	c2 = (SDL_Color) {96+16, 64+32,255-64, 0};

	c = DARKER_MENU_COLORS;
	c2 = DARKER_MENU_COLORS;


//	r = (SDL_Rect) { 16, GUI_HEIGHT/2 - 64, GUI_WIDTH - 32, 128 };
	r = (SDL_Rect) { 12, GUI_HEIGHT/2 - (64-(48/2)), GUI_WIDTH - 24, 128-48 + 4 };
	kbd.win = EG_Window_Create("bbc_keyboard_win", frame_buffer_p, c, r);
	r2 = EG_Window_GetDimension(kbd.win); r2.x+=2; r2.w -=4;


//	kbd.lbl_title = EG_Label_Create("bbc_keyboard_title", c, EG_LABEL_ALIGN_CENTER, "BBC Keyboard:", C_1_1(r2, 1, 2));
//	(void) EG_Window_AddWidget(kbd.win, kbd.lbl_title);

	r3 = C_1_1(r2, 1, 16);
	r3.x+=4; r3.w-=8;

//	kbd.box = EG_Box_Create("bbc_keyboard_box", EG_BOX_BORDER_SUNK, c, r3);
//	(void) EG_Window_AddWidget(kbd.win, kbd.box);
//	r3.x+=4; r3.w-=8; r3.y+=4; r3.h -=8;

//	r4 = CALC_LOC(r3.x, r3.y, 0, 0, 4, 1);
	
	for(i=0; i<KEY_COUNT; i++) {

		r4 = CALC_LOC(r3.x, r3.y
                 , bbc_keyboard_mapping[i].x, bbc_keyboard_mapping[i].y, bbc_keyboard_mapping[i].w, bbc_keyboard_mapping[i].h);

		r4.w+=1; r4.h+=1;

		kbd.but_key[i] = EG_Button_Create("bbc_keyboard", c, EG_BUTTON_ALIGN_CENTER, bbc_keyboard_mapping[i].name, r4 /* CALC_LOC(r3.x, r3.y
		 , bbc_keyboard_mapping[i].x, bbc_keyboard_mapping[i].y, bbc_keyboard_mapping[i].w, bbc_keyboard_mapping[i].h) */ );


//		EG_Button_DrawWithoutBorder(kbd.but_key[i]);
		(void) EG_Button_SetMyCallback_OnClick(kbd.but_key[i], BBCKeyboard_Callback_Key, (void*) i);
		(void) EG_Window_AddWidget(kbd.win, kbd.but_key[i]);

//		/* Determine which key has focus:
//	 	 */
//		if (bbc_keyboard_mapping[i].key == current_key)
//			focused_index = i;

	}

	for(i=0; i<KEY_COUNT; i++) {
		if (bbc_keyboard_mapping[i].l >= 0)
			EG_Widget_SetSmartFocusPeerLeft(kbd.but_key[i], kbd.but_key[bbc_keyboard_mapping[i].l]);
		if (bbc_keyboard_mapping[i].r >= 0)
			EG_Widget_SetSmartFocusPeerRight(kbd.but_key[i], kbd.but_key[bbc_keyboard_mapping[i].r]);
		if (bbc_keyboard_mapping[i].u >= 0)
			EG_Widget_SetSmartFocusPeerUp(kbd.but_key[i], kbd.but_key[bbc_keyboard_mapping[i].u]);
		if (bbc_keyboard_mapping[i].d >= 0)
			 EG_Widget_SetSmartFocusPeerDown(kbd.but_key[i], kbd.but_key[bbc_keyboard_mapping[i].d]);
	}

	/* Change focus to focused key:
	 */
//	if (focused_index >= 0)
//		EG_Window_SetFocusToThisWidget(kbd.but_key[focused_index]);
//
        return EG_TRUE;
}

void BBCKeyboard_Destroy()
{
	if (kbd_p != NULL) {
	        EG_Window_DestroyAllChildWidgets(kbd.win);
	        EG_Window_Destroy(kbd.win);

	//	/* Clear out struct just to be sure:
	//	 */
	//	memset(&kbd, 0, sizeof(kbd));
	}	
}

// 0=failed, 1=success, 2=cancelled.
int BBC_Keyboard_SaveConfig(void)
{
	FILE *output = stdout;

	char buf[MAX_DIR_LEN], kbdpath[MAX_DIR_LEN], *ext_p, *filename_p;

	/* Do nothing if no disc in drive 0:
	 */
	if (strlen(config.drive_0_path) == 0)
		return 0;


	if ( (ext_p=strrchr(config.drive_0_path, (int) '.')) == NULL) {
		printf("Illegal disc is loaded in drive 0! This shouldn't happen.\n");
		return EG_FALSE;
	}

	/* Calc. filename on its own (remove path etc):
	 */
	if ( (filename_p=strrchr(config.drive_0_path, (int) '/')) == NULL)
		filename_p=config.drive_0_path;
	else
		filename_p++;

	/* Calc. output filename:
	 */
	strncpy(buf, filename_p, strlen(filename_p));
	buf[strlen(filename_p)-strlen(ext_p)]='\0';
	sprintf(kbdpath, "%s%s.kbd", config.files.kbd_files_dir, buf);

//	printf("[%s] [%s]\n", config.drive_0_path, buf);

	/* Open output file for writing:
	 */
	if( (output=fopen(kbdpath, "w")) == NULL) {
		printf("Unable to open/create '%s'\n", kbdpath);
		return 0;
	}

	/* Write kbd file header comments.
	 */
#ifdef GEKKO
	fprintf(output, "# !BeebEm-Wii-kbd-config-file (v="VERSION")\n# \n");
	fprintf(output, "# For: %s\n\n", filename_p);
#else
	fprintf(output, "# !BeebEm-2x-kbd-config-file (v="VERSION")\n# \n");
	fprintf(output, "# For: %s\n\n", filename_p);
#endif


	/* General config:
	 */
	fprintf(output, "# General configuration options:\n");

	/* GP2X CPU Clock speed:
	 */
#ifdef GP2X
	if (config.settings.speed != 0)
		fprintf(output, "SET SPEED=%d\n", config.settings.speed);
#endif

	/* Audio volume & quality:
	 */
	fprintf(output, "SET VOLUME=%d\n", config.settings.volume);
	fprintf(output, "SET LATENCY=%d\n", config.settings.latency);

	/* Video scaling:
	 */
	switch (config.settings.vscale) {
	case CENTER:
		fprintf(output, "SET VSCALE=CENTERED\n");
		break;
	case TOP:
		fprintf(output, "SET VSCALE=TOP\n");
		break;
	case BOTTOM:
		fprintf(output, "SET VSCALE=BOTTOM\n");
		break;
	case SCALED:
		fprintf(output, "SET VSCALE=SCALED\n");
		break;
	default:
		break;
	}


	/* 240p mode:
	 */
	fprintf(output, "SET 240p=%s\n", ( config.settings.doublestrike ==  0 ? "NO" : "YES") );

	/* Bilinear filter:
  	 */
	fprintf(output, "SET ENABLE_FILTER=%s\n", config.settings.enable_filter == 0 ? "NO" : "YES");

	/* Flipped mode:
	 */
	//fprintf(output, "SET ORIENTATION=%s\n", ( config.settings.orientation == NORMAL ? "NORMAL" : "FLIPPED") );

	/* Cursor:
  	 */
	//fprintf(output, "SET DISABLE_CURSOR=%s\n", config.settings.disable_cursor == 0 ? "NO" : "YES");

	/* VSYNC offset:
	 */
	fprintf(output, "SET VSYNCOFFSET=%s\n", config.settings.vsyncoffset == 32 ? "NORMAL" : "NUDGE");

	/* Stick:
  	 */
	switch (StickManager_GetMode()) {
	case STICKMODE_HORIZONTAL:
		fprintf(output, "SET STICK=HORIZONTAL\n");
		break;
	case STICKMODE_VERTICAL:
		fprintf(output, "SET STICK=VERTICAL\n");
		break;
	case STICKMODE_MULTI:
		fprintf(output, "SET STICK=MULTI\n");
		break;

	default:
	case STICKMODE_CRITICAL:
		fprintf(output, "SET STICK=SMART\n");
		break;
	} 

	/* Floppies:
	 */
	fprintf(output, "SET FD0_TAB=%s\n", ( config.settings.protection_8271_0 == RO_8271 ? "RO" : "RW") );
	fprintf(output, "SET FD1_TAB=%s\n", ( config.settings.protection_8271_1 == RO_8271 ? "RO" : "RW") );

	fprintf(output, "\n");


	/* First player buttons:
	 */
	fprintf(output, "# Player 1:\n");

	fprintf(output, "P1_LEFT=%s\n", Keyboard_GetKeyName(Keyboard_GetButtonMapping(WIIGC1_BUTTON_LEFT)) );
	fprintf(output, "P1_RIGHT=%s\n", Keyboard_GetKeyName(Keyboard_GetButtonMapping(WIIGC1_BUTTON_RIGHT)) );
	fprintf(output, "P1_UP=%s\n", Keyboard_GetKeyName(Keyboard_GetButtonMapping(WIIGC1_BUTTON_UP)) );
	fprintf(output, "P1_DOWN=%s\n", Keyboard_GetKeyName(Keyboard_GetButtonMapping(WIIGC1_BUTTON_DOWN)) );
	fprintf(output, "P1_A=%s\n", Keyboard_GetKeyName(Keyboard_GetButtonMapping(WIIGC1_BUTTON_A)) );
	fprintf(output, "P1_B=%s\n", Keyboard_GetKeyName(Keyboard_GetButtonMapping(WIIGC1_BUTTON_B)) );
	fprintf(output, "P1_X=%s\n", Keyboard_GetKeyName(Keyboard_GetButtonMapping(WIIGC1_BUTTON_X)) );
	fprintf(output, "P1_Y=%s\n", Keyboard_GetKeyName(Keyboard_GetButtonMapping(WIIGC1_BUTTON_Y)) );
	fprintf(output, "P1_MINUS=%s\n", Keyboard_GetKeyName(Keyboard_GetButtonMapping(WIIGC1_BUTTON_MINUS)) );
	fprintf(output, "P1_CLASSIC_A=%s\n", Keyboard_GetKeyName(Keyboard_GetButtonMapping(CLASSIC1_BUTTON_A)) );
	fprintf(output, "P1_CLASSIC_B=%s\n", Keyboard_GetKeyName(Keyboard_GetButtonMapping(CLASSIC1_BUTTON_B)) );
	fprintf(output, "P1_CLASSIC_X=%s\n", Keyboard_GetKeyName(Keyboard_GetButtonMapping(CLASSIC1_BUTTON_X)) );
	fprintf(output, "P1_CLASSIC_Y=%s\n", Keyboard_GetKeyName(Keyboard_GetButtonMapping(CLASSIC1_BUTTON_Y)) );
	fprintf(output, "P1_CLASSIC_L=%s\n", Keyboard_GetKeyName(Keyboard_GetButtonMapping(CLASSIC1_BUTTON_L)) );
	fprintf(output, "P1_CLASSIC_R=%s\n", Keyboard_GetKeyName(Keyboard_GetButtonMapping(CLASSIC1_BUTTON_R)) );


	/*fprintf(output, "VOLUP=%s\n", Keyboard_GetKeyName(Keyboard_GetButtonMapping(GP2X_BUTTON_VOLUP)) );
	fprintf(output, "VOLDOWN=%s\n", Keyboard_GetKeyName(Keyboard_GetButtonMapping(GP2X_BUTTON_VOLDOWN)) );*/
	fprintf(output, "\n");

	/* Second player buttons:
	 */
	fprintf(output, "# Player 2:\n");
	fprintf(output, "P2_LEFT=%s\n", Keyboard_GetKeyName(Keyboard_GetButtonMapping(WIIGC2_BUTTON_LEFT)) );
	fprintf(output, "P2_RIGHT=%s\n", Keyboard_GetKeyName(Keyboard_GetButtonMapping(WIIGC2_BUTTON_RIGHT)) );
	fprintf(output, "P2_UP=%s\n", Keyboard_GetKeyName(Keyboard_GetButtonMapping(WIIGC2_BUTTON_UP)) );
	fprintf(output, "P2_DOWN=%s\n", Keyboard_GetKeyName(Keyboard_GetButtonMapping(WIIGC2_BUTTON_DOWN)) );
	fprintf(output, "P2_A=%s\n", Keyboard_GetKeyName(Keyboard_GetButtonMapping(WIIGC2_BUTTON_A)) );
	fprintf(output, "P2_B=%s\n", Keyboard_GetKeyName(Keyboard_GetButtonMapping(WIIGC2_BUTTON_B)) );
	fprintf(output, "P2_X=%s\n", Keyboard_GetKeyName(Keyboard_GetButtonMapping(WIIGC2_BUTTON_X)) );
	fprintf(output, "P2_Y=%s\n", Keyboard_GetKeyName(Keyboard_GetButtonMapping(WIIGC2_BUTTON_Y)) );
	fprintf(output, "P2_MINUS=%s\n", Keyboard_GetKeyName(Keyboard_GetButtonMapping(WIIGC2_BUTTON_MINUS)) );
	fprintf(output, "P2_CLASSIC_A=%s\n", Keyboard_GetKeyName(Keyboard_GetButtonMapping(CLASSIC2_BUTTON_A)) );
	fprintf(output, "P2_CLASSIC_B=%s\n", Keyboard_GetKeyName(Keyboard_GetButtonMapping(CLASSIC2_BUTTON_B)) );
	fprintf(output, "P2_CLASSIC_X=%s\n", Keyboard_GetKeyName(Keyboard_GetButtonMapping(CLASSIC2_BUTTON_X)) );
	fprintf(output, "P2_CLASSIC_Y=%s\n", Keyboard_GetKeyName(Keyboard_GetButtonMapping(CLASSIC2_BUTTON_Y)) );
	fprintf(output, "P2_CLASSIC_L=%s\n", Keyboard_GetKeyName(Keyboard_GetButtonMapping(CLASSIC2_BUTTON_L)) );
	fprintf(output, "P2_CLASSIC_R=%s\n", Keyboard_GetKeyName(Keyboard_GetButtonMapping(CLASSIC2_BUTTON_R)) );
	fprintf(output, "\n");


	/* Close file:
	 */
	fclose(output);
#ifdef GP2X
	sync();
#endif

	/* Return success:
	 */
	return 1;
}
