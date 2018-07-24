/* The BeebEm GUI pages:
 *
 * A simple GUI for the GP2x.
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


#define KEY_COUNT 43


typedef struct {
	EG_Window	*win;

	EG_Widget	*text;
	EG_Widget	*box;

	EG_Widget       *but_key[KEY_COUNT];
} SaveAsKeyboard;


static SaveAsKeyboard kbd, *kbd_p = NULL;

static int done, key;

static int shift;


//#define MAX_FILENAME_LEN 55
//#define MAX_FILENAME_LEN 50
#define MAX_FILENAME_LEN 54

static char filename[MAX_FILENAME_LEN+1], old_char;
static int cursor_i, cursor_s=0, name_i;


//static SDL_Rect save_as_calc_loc(int xo, int yo, int x, int y, int w, int h)
//{
//	return (SDL_Rect) {(x*7)+xo, (y*12)+yo, w*(7)-1, h*(12)-1};
//}

//static SDL_Rect save_as_calc_loc(int x, int y, int w, int h)
//{
//	return (SDL_R
//}

static struct {
	int x, y, w, h;
	int l,r,u,d;
	char name[255+1];
	int key, alt_key;
} save_as_keyboard_mapping[KEY_COUNT] = {

	/* 0=F0  1=F1  2=F2  3=F3  4=F4  5=F5  6=F6  7=F7  8=F8  9=F9
 	*/
//#define	R1_OFFSET	10

				//  l   r   u   d
	{ 68,  24,   80-68,  34-24, 41,  1, -1, 11,   "1", '1', '1'}, /* 00 1 */
	{ 82,  24,   94-82,  34-24,  0,  2, -1, 12,   "2", '2', '2'}, /* 01 2 */
	{ 96,  24,  108-96,  34-24,  1,  3, -1, 13,   "3", '3', '3'}, /* 02 3 */ 
	{110,  24, 122-110,  34-24,  2,  4, -1, 14,   "4", '4', '4'}, /* 03 4 */
	{124,  24, 136-124,  34-24,  3,  5, -1, 15,   "5", '5', '5'}, /* 04 5 */
	{138,  24, 150-138,  34-24,  4,  6, -1, 16,   "6", '6', '6'}, /* 05 6 */
	{152,  24, 164-152,  34-24,  5,  7, -1, 17,   "7", '7', '7'}, /* 06 7 */
	{166,  24, 178-166,  34-24,  6,  8, -1, 18,   "8", '8', '8'}, /* 07 8 */
	{180,  24, 192-180,  34-24,  7,  9, -1, 19,   "9", '9', '9'}, /* 08 9 */
	{194,  24, 206-194,  34-24,  8, 10, -1, 20,   "0", '0', '0'}, /* 09 0 */
	{208,  24, 220-208,  34-24, 9, 42, -1, 20,   "\34" ,   8, -1}, /* 10 Delete */

	{ 75,  36,   87-75,  46-36, -1, 12,  0, 21,   "q", 'q', 'Q'}, /* 11 Q */
	{ 89,  36,  101-89,  46-36, 11, 13,  1, 22,   "w", 'w', 'W'}, /* 12 W */
	{103,  36, 115-103,  46-36, 12, 14,  2, 23,   "e", 'e', 'E'}, /* 13 E */
	{117,  36, 129-117,  46-36, 13, 15,  3, 24,   "r", 'r', 'R'}, /* 14 R */
	{131,  36, 143-131,  46-36, 14, 16,  4, 25,   "t", 't', 'T'}, /* 15 T */
	{145,  36, 157-145,  46-36, 15, 17,  5, 26,   "y", 'y', 'Y'}, /* 16 Y */
	{159,  36, 171-159,  46-36, 16, 18,  6, 27,   "u", 'u', 'U'}, /* 17 U */
	{173,  36, 185-173,  46-36, 17, 19,  7, 28,   "i", 'i', 'I'}, /* 18 I */
	{187,  36, 199-187,  46-36, 18, 20,  8, 29,   "o", 'o', 'O'}, /* 19 O */
	{201,  36, 212-201,  46-36, 19, -1,  9, 29,   "p", 'p', 'P'}, /* 20 P */

	{ 82,  48,   94-82,  58-48, -1, 22, 11, 31,   "a", 'a', 'A'}, /* 21 A */
	{ 96,  48,  108-96,  58-48, 21, 23, 12, 32,   "s", 's', 'S'}, /* 22 S */
	{110,  48, 122-110,  58-48, 22, 24, 13, 33,   "d", 'd', 'D'}, /* 23 D */
	{124,  48, 136-124,  58-48, 23, 25, 14, 34,   "f", 'f', 'F'}, /* 24 F */
	{138,  48, 150-138,  58-48, 24, 26, 15, 35,   "g", 'g', 'G'}, /* 25 G */
	{152,  48, 164-152,  58-48, 25, 27, 16, 36,   "h", 'h', 'H'}, /* 26 H */
	{166,  48, 178-166,  58-48, 26, 28, 17, 37,   "j", 'j', 'J'}, /* 27 J */
	{180,  48, 192-180,  58-48, 27, 29, 18, 38,   "k", 'k', 'K'}, /* 28 K */
	{194,  48, 206-194,  58-48, 28, -1, 19, 39,   "l", 'l', 'L'}, /* 29 L */

	{ 75,  60,   87-75,  70-60, -1, 31, 21, -1, "\36",   1, -1}, /* 30 Shift-Lock */
	{ 89,  60,  101-89,  70-60, 30, 32, 21, 40,   "z", 'z', 'Z'}, /* 31 Z */
	{103,  60, 115-103,  70-60, 31, 33, 22, 40,   "x", 'x', 'X'}, /* 32 X */
	{117,  60, 129-117,  70-60, 32, 34, 23, 40,   "c", 'c', 'C'}, /* 33 C */
	{131,  60, 143-131,  70-60, 33, 35, 24, 40,   "v", 'v', 'V'}, /* 34 V */
	{145,  60, 157-145,  70-60, 34, 36, 25, 40,   "b", 'b', 'B'}, /* 35 B */
	{159,  60, 171-159,  70-60, 35, 37, 26, 40,   "n", 'n', 'N'}, /* 36 N */
	{173,  60, 185-173,  70-60, 36, 38, 27, 40,   "m", 'm', 'M'}, /* 37 M */
	{187,  60, 199-187,  70-60, 37, 39, 28, 40, "\04",  10, -1}, /* 38 Left */
	{201,  60, 213-201,  70-60, 38, -1, 29, -1, "\03",  11, -1}, /* 39 Right */

	{ 89,  72,  199-89,  82-72, -1, -1, 35, -1,"_",'_', ' '}, /* 40 Space */

	{  4+4, 24, 68-8-4, 34-24, -1, -1, -1, -1,"Cancel", -10, -1},	 /* 41 */
	{  208+4+(220-208), 24, 68-8-4, 34-24, 10, -1, -1, -1,"Accept", -20, -1} /* 42 */


};


/* Default GUI size (fixed and none scalable for GP2x, unlike UNIX BeebEm):
 */
#define GUI_WIDTH       320
#define GUI_HEIGHT      240


static EG_BOOL	Make(SDL_Surface *frame_buffer_p /*, int current_key*/);
static void Destroy(void);


#define EMULATE_BUTTON_PRESS(n) SaveAs_Keyboard_Callback_Key(kbd.but_key[n], (void*) n);

static void SaveAs_Keyboard_Callback_Key(EG_Widget*, void* user_p)
{
	char caption[2]={" "};
        int l, i = ((int) (intptr_t) user_p);

	if (i>=KEY_COUNT)
		return;

	if (save_as_keyboard_mapping[i].key == 10) {
		filename[cursor_i]=old_char;
		if (cursor_i>0) cursor_i-=1;
		old_char = filename[cursor_i];
		return;
	} else if (save_as_keyboard_mapping[i].key == 11) {
		filename[cursor_i]=old_char;
		if (cursor_i<=name_i) cursor_i+=1;
		old_char = filename[cursor_i];
		return;
	} else if (save_as_keyboard_mapping[i].key == 1) {
		shift=(shift^1);		

		for(l=0; l<KEY_COUNT; l++) {
			if (save_as_keyboard_mapping[l].alt_key > 0 && save_as_keyboard_mapping[l].key != ' ') {
				if (shift)
					caption[0] = (char) save_as_keyboard_mapping[l].alt_key;
				else 
					caption[0] = (char) save_as_keyboard_mapping[l].key;

				EG_Button_SetCaption(kbd.but_key[l], caption);
				EG_Button_RepaintLot(kbd.but_key[l]);
			}
		}
		return;
	} else if (save_as_keyboard_mapping[i].key == 8) {
		char new_filename[MAX_FILENAME_LEN+1];

		if (cursor_i > 0) {
			filename[cursor_i]=old_char;
			memcpy(new_filename, filename, cursor_i-1);
			new_filename[cursor_i-1]=0;

			strcat(new_filename, filename+cursor_i);
			strcat(new_filename, " ");

			strcpy(filename, new_filename);
			cursor_i--;
			name_i--;

			old_char = filename[cursor_i];

		}

		return;
	} else if (save_as_keyboard_mapping[i].key == -10) {
		// cancel
//		strcpy(filename, "");
//		old_char = filename[cursor_i];
		done=2;

		return;
	} else if (save_as_keyboard_mapping[i].key == -20) {
		// except

//		filename[name_i+1]='\0';
//		old_char = filename[cursor_i];
		done=1;
		return;
	}

	// add the char
	if (name_i < MAX_FILENAME_LEN-2) {
		char new_filename[MAX_FILENAME_LEN+1+1]; // one extra as we add a char
		char new_char[]=" ";

		filename[cursor_i]=old_char;

		memcpy(new_filename, filename, cursor_i);
                new_filename[cursor_i]=0;

		if (shift)
			new_char[0]=save_as_keyboard_mapping[i].alt_key;
		else
			new_char[0]=save_as_keyboard_mapping[i].key;

		strcat(new_filename, new_char);

		strcat(new_filename, filename+cursor_i);
		new_filename[strlen(new_filename)-1]=0;	// remove last char.	

		strcpy(filename, new_filename);
		name_i++; cursor_i++;
		old_char = filename[cursor_i];
	}
}





const char* FileSelector_SaveAs_Show(SDL_Surface *frame_buffer_p, const char* default_filename_p)
{
	EG_Window *dialog_window_p;
	SDL_Event event;
	int old_delay;

	static char returned_filename[MAX_FILENAME_LEN+1];

	/* Create BBC Keyboard page:
	 */
	if (Make(frame_buffer_p /*, current_key*/) != EG_TRUE) {
		EG_Log(EG_LOG_FATAL
		 , dL"Unable to create Save As Keyboard page!", dR);
		return NULL;
	}
	kbd_p = &kbd;

	/* Set default value for text box
 	 */
	memset(filename, ' ', MAX_FILENAME_LEN-1);
	filename[MAX_FILENAME_LEN-1]='\0';

	strncpy(filename, default_filename_p, MAX_FILENAME_LEN-1);
	cursor_i = strlen(filename);
	name_i = cursor_i -1;

	// Add blanks to end.
	int i;
	for(i=strlen(filename); i<MAX_FILENAME_LEN; i++)
		filename[i]=' ';
	filename[MAX_FILENAME_LEN-1]='\0';


//	if (default_filename_p != NULL) {
//
//		printf("*******************%s*********************\n", default_filename_p);
//
//		cursor_i = strlen(default_filename_p);
//		if (cursor_i>MAX_FILENAME_LEN) cursor_i=MAX_FILENAME_LEN;
//
//		memcpy(filename, default_filename_p, cursor_i);
//		filename[MAX_FILENAME_LEN-1]=0;
//		name_i=cursor_i-1;
//	}
	EG_Label_SetCaption(kbd.text, filename);

	/* Show the window:
	 */

	// Accept has focus
	EG_Button_GetFocus(kbd.but_key[42]);

	dialog_window_p = kbd_p->win;
	EG_Window_Show(dialog_window_p);

	Button_Reset();
	old_delay = Button_GetAutoRepeatDelay();
	Button_SetAutoRepeatDelay(5);

	shift = 0;
	key = -1;
	done = 0;
	do {
		while ( SDL_PollEvent(&event) ) {

			if (event.type != SDL_USEREVENT) {
				if (event.type == SDL_KEYDOWN) {
					switch (event.key.keysym.sym) {
					case SDLK_BACKSPACE:
						EMULATE_BUTTON_PRESS(10);
						break;	

					case SDLK_LSHIFT:
					case SDLK_RSHIFT:
					case SDLK_CAPSLOCK:
						EMULATE_BUTTON_PRESS(30);
						break;

					case SDLK_PAGEUP:
						EMULATE_BUTTON_PRESS(39);
						break;
					case SDLK_PAGEDOWN:
						EMULATE_BUTTON_PRESS(38);
						break;
					case SDLK_SPACE:
						EMULATE_BUTTON_PRESS(40);
						break;
					case SDLK_1:
						EMULATE_BUTTON_PRESS(0);
						break;
					case SDLK_2:
						EMULATE_BUTTON_PRESS(1);
						break;
					case SDLK_3:
						EMULATE_BUTTON_PRESS(2);
						break;
					case SDLK_4:
						EMULATE_BUTTON_PRESS(3);
						break;
					case SDLK_5:
						EMULATE_BUTTON_PRESS(4);
						break;
					case SDLK_6:
						EMULATE_BUTTON_PRESS(5);
						break;
					case SDLK_7:
						EMULATE_BUTTON_PRESS(6);
						break;
					case SDLK_8:
						EMULATE_BUTTON_PRESS(7);
						break;
					case SDLK_9:
						EMULATE_BUTTON_PRESS(8);
						break;
					case SDLK_0:
						EMULATE_BUTTON_PRESS(9);
						break;

					case SDLK_q:
						EMULATE_BUTTON_PRESS(11);
						break;
					case SDLK_w:
						EMULATE_BUTTON_PRESS(12);
						break;
					case SDLK_e:
						EMULATE_BUTTON_PRESS(13);
						break;
					case SDLK_r:
						EMULATE_BUTTON_PRESS(14);
						break;
					case SDLK_t:
						EMULATE_BUTTON_PRESS(15);
						break;
					case SDLK_y:
						EMULATE_BUTTON_PRESS(16);
						break;
					case SDLK_u:
						EMULATE_BUTTON_PRESS(17);
						break;
					case SDLK_i:
						EMULATE_BUTTON_PRESS(18);
						break;
					case SDLK_o:
						EMULATE_BUTTON_PRESS(19);
						break;
					case SDLK_p:
						EMULATE_BUTTON_PRESS(20);
						break;

					case SDLK_a:
						EMULATE_BUTTON_PRESS(21);
						break;
					case SDLK_s:
						EMULATE_BUTTON_PRESS(22);
						break;
					case SDLK_d:
						EMULATE_BUTTON_PRESS(23);
						break;
					case SDLK_f:
						EMULATE_BUTTON_PRESS(24);
						break;
					case SDLK_g:
						EMULATE_BUTTON_PRESS(25);
						break;
					case SDLK_h:
						EMULATE_BUTTON_PRESS(26);
						break;
					case SDLK_j:
						EMULATE_BUTTON_PRESS(27);
						break;
					case SDLK_k:
						EMULATE_BUTTON_PRESS(28);
						break;
					case SDLK_l:
						EMULATE_BUTTON_PRESS(29);
						break;

					case SDLK_z:
						EMULATE_BUTTON_PRESS(31);
						break;
					case SDLK_x:
						EMULATE_BUTTON_PRESS(32);
						break;
					case SDLK_c:
						EMULATE_BUTTON_PRESS(33);
						break;
					case SDLK_v:
						EMULATE_BUTTON_PRESS(34);
						break;
					case SDLK_b:
						EMULATE_BUTTON_PRESS(35);
						break;
					case SDLK_n:
						EMULATE_BUTTON_PRESS(36);
						break;
					case SDLK_m:
						EMULATE_BUTTON_PRESS(37);
						break;

					default:
						break;

					}
				}

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


		/* Toggle cursor
		 */
		cursor_s^=1;
		if (cursor_s) {
			old_char =  filename[cursor_i];
			filename[cursor_i]='\37';
		} else {
			filename[cursor_i]=old_char;
		}
		EG_Label_SetCaption(kbd.text, filename);

	} while (! done );
	filename[cursor_i]=old_char;

	Button_Reset();
	Button_SetAutoRepeatDelay(old_delay);

	EG_Window_Hide(dialog_window_p);

	Destroy();

	if (done==1) {
		memset(returned_filename, 0, MAX_FILENAME_LEN+1);
		memcpy(returned_filename, filename, name_i+1);

		return returned_filename;
	}


	return NULL;
	
}

static EG_BOOL Make(SDL_Surface *frame_buffer_p /*, int current_key*/)
{
	EG_Widget *w_p;
	SDL_Color c;
	SDL_Rect r, r2, r3, r4;
	int i /*, focused_index=-1*/;

	/* Clear the keyboard struct:
	 */
	memset(&kbd, 0, sizeof(kbd));


        /* Default colors:
         */
	c = VERY_DARK_MENU_COLORS;

//	c = (SDL_Color) {96, 64,255-64, 0};
//	c2 = (SDL_Color) {96+16, 64+32,255-64, 0};

//	r = (SDL_Rect) { 16, GUI_HEIGHT/2 - 64, GUI_WIDTH - 32, 128 };
//	r = (SDL_Rect) { 16, GUI_HEIGHT/2 - (64-(48/2)), GUI_WIDTH - 32, 128-48 };

	r = (SDL_Rect) { (GUI_WIDTH-288)/2, (GUI_HEIGHT-90)/2, 288, 90 };

	kbd.win = EG_Window_Create("save_as_keyboard_win", frame_buffer_p, c, r);

	r2 = EG_Window_GetDimension(kbd.win); r2.x+=5; r2.w -=10;

//	kbd.lbl_title = EG_Label_Create("bbc_keyboard_title", c, EG_LABEL_ALIGN_CENTER, "BBC Keyboard:", C_1_1(r2, 1, 2));
//	(void) EG_Window_AddWidget(kbd.win, kbd.lbl_title);

	r3 = C_1_1(r2, 1, 16);
//	kbd.box = EG_Box_Create("bbc_keyboard_box", EG_BOX_BORDER_SUNK, c, r3);
//	(void) EG_Window_AddWidget(kbd.win, kbd.box);
//	r3.x+=4; r3.w-=8; r3.y+=4; r3.h -=8;

//	r4 = CALC_LOC(r3.x, r3.y, 0, 0, 4, 1);

	r4 = r3; r4.h=14;
	w_p = EG_Box_Create("border", EG_BOX_BORDER_SUNK, c, r4);
	(void) EG_Window_AddWidget(kbd.win, w_p);

	r4.x+=2; r4.y+=2; r4.w-=4; r4.h-=4;
	kbd.text = EG_Label_Create("text", c, EG_LABEL_ALIGN_LEFT, "123456789012345678901234567890123456789012345678901234567890", r4);

	(void) EG_Window_AddWidget(kbd.win, kbd.text);


	
	for(i=0; i<KEY_COUNT; i++) {
//		kbd.but_key[i] = EG_Button_Create("save_as_keyboard", c, EG_BUTTON_ALIGN_CENTER, save_as_keyboard_mapping[i].name, save_as_calc_loc(r3.x, r3.y
//		 , save_as_keyboard_mapping[i].x, save_as_keyboard_mapping[i].y, save_as_keyboard_mapping[i].w, save_as_keyboard_mapping[i].h) );

//		kbd.but_key[i] = EG_Button_Create("save_as_keyboard", c, EG_BUTTON_ALIGN_CENTER
//		 , save_as_keyboard_mapping[i].name
//		 , save_as_calc_loc(0, 0, save_as_keyboard_mapping[i].x, save_as_keyboard_mapping[i].y
//		   , save_as_keyboard_mapping[i].w, save_as_keyboard_mapping[i].h)
//		 );
		  

		kbd.but_key[i] = EG_Button_Create("save_as_keyboard", c, EG_BUTTON_ALIGN_CENTER
		 , save_as_keyboard_mapping[i].name
		 , (SDL_Rect) {save_as_keyboard_mapping[i].x + EG_Window_GetXDisplacement(kbd.win)-1, save_as_keyboard_mapping[i].y + EG_Window_GetYDisplacement(kbd.win)-1
		   , save_as_keyboard_mapping[i].w+2, save_as_keyboard_mapping[i].h+2}
		 );



//		EG_Button_DrawWithoutBorder(kbd.but_key[i]);
		(void) EG_Button_SetMyCallback_OnClick(kbd.but_key[i], SaveAs_Keyboard_Callback_Key, (void*) i);
		(void) EG_Window_AddWidget(kbd.win, kbd.but_key[i]);

//		/* Determine which key has focus:
//	 	 */
//		if (bbc_keyboard_mapping[i].key == current_key)
//			focused_index = i;

	}

	for(i=0; i<KEY_COUNT; i++) {
		if (save_as_keyboard_mapping[i].l >= 0)
			EG_Widget_SetSmartFocusPeerLeft(kbd.but_key[i], kbd.but_key[save_as_keyboard_mapping[i].l]);
		if (save_as_keyboard_mapping[i].r >= 0)
			EG_Widget_SetSmartFocusPeerRight(kbd.but_key[i], kbd.but_key[save_as_keyboard_mapping[i].r]);
		if (save_as_keyboard_mapping[i].u >= 0)
			EG_Widget_SetSmartFocusPeerUp(kbd.but_key[i], kbd.but_key[save_as_keyboard_mapping[i].u]);
		if (save_as_keyboard_mapping[i].d >= 0)
			 EG_Widget_SetSmartFocusPeerDown(kbd.but_key[i], kbd.but_key[save_as_keyboard_mapping[i].d]);
	}

	/* Change focus to focused key:
	 */
//	if (focused_index >= 0)
//		EG_Window_SetFocusToThisWidget(kbd.but_key[focused_index]);
//
        return EG_TRUE;
}

static void Destroy(void)
{
	if (kbd_p != NULL) {
	        EG_Window_DestroyAllChildWidgets(kbd.win);
	        EG_Window_Destroy(kbd.win);

	//	/* Clear out struct just to be sure:
	//	 */
	//	memset(&kbd, 0, sizeof(kbd));
	}	
}
