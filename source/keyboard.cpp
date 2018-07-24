#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#include "keyboard.h"
#include "beebconfig.h"
#include "beebconfig_data.h"
#include "sysvia.h"
#include "main.h"
#include "disc8271.h"


// [TODO] For Set options below (move SET to its own file).
#include "sdl.h"
#include "video.h"
///////////


// This is not used everywhere yet:
//#define BUTTON_COUNT	28
#define BUTTON_COUNT	32

int gp2x_key_lookup[BUTTON_COUNT];

int gp2x_mapping[BUTTON_COUNT][2] =
 {

         {WIIGC1_BUTTON_LEFT, SDLK_z},
         {WIIGC1_BUTTON_RIGHT,SDLK_x},
         {WIIGC1_BUTTON_UP, SDLK_QUOTE},
         {WIIGC1_BUTTON_DOWN,SDLK_SLASH},

         {WIIGC2_BUTTON_LEFT, SDLK_z},
         {WIIGC2_BUTTON_RIGHT,SDLK_x},
         {WIIGC2_BUTTON_UP, SDLK_QUOTE},
         {WIIGC2_BUTTON_DOWN,SDLK_SLASH},



         {WIIGC1_BUTTON_A,SDLK_SPACE},
         {WIIGC1_BUTTON_B,SDLK_RETURN},
         {WIIGC1_BUTTON_X,SDLK_n},
         {WIIGC1_BUTTON_Y,SDLK_y},
         {WIIGC1_BUTTON_MINUS, SDLK_z},
         {WIIGC1_BUTTON_TRIGGER_Z,SDLK_x},

         {CLASSIC1_BUTTON_A, SDLK_SPACE},
	 {CLASSIC1_BUTTON_B, SDLK_RETURN},
	 {CLASSIC1_BUTTON_X, SDLK_n},
	 {CLASSIC1_BUTTON_Y, SDLK_y},
         {CLASSIC1_BUTTON_L, SDLK_QUOTE},
         {CLASSIC1_BUTTON_R,SDLK_SLASH},


         {WIIGC2_BUTTON_A,SDLK_SPACE},
         {WIIGC2_BUTTON_B,SDLK_RETURN},
         {WIIGC2_BUTTON_X,SDLK_n},
         {WIIGC2_BUTTON_Y,SDLK_y},
         {WIIGC2_BUTTON_MINUS, SDLK_z},
         {WIIGC2_BUTTON_TRIGGER_Z,SDLK_x},

         {CLASSIC2_BUTTON_A, SDLK_SPACE},
	 {CLASSIC2_BUTTON_B, SDLK_RETURN},
	 {CLASSIC2_BUTTON_X, SDLK_n},
	 {CLASSIC2_BUTTON_Y, SDLK_y},
         {CLASSIC2_BUTTON_L, SDLK_QUOTE},
         {CLASSIC2_BUTTON_R,SDLK_SLASH}




/*         {GP2X_BUTTON_LEFT, SDLK_z},
         {GP2X_BUTTON_RIGHT,SDLK_x},
         {GP2X_BUTTON_UP, SDLK_QUOTE},
         {GP2X_BUTTON_DOWN,SDLK_SLASH},

         {GP2X_R_BUTTON_LEFT, SDLK_z},
         {GP2X_R_BUTTON_RIGHT,SDLK_x},
         {GP2X_R_BUTTON_UP, SDLK_QUOTE},
         {GP2X_R_BUTTON_DOWN,SDLK_SLASH},

         {GP2X_BUTTON_A,SDLK_SPACE},
         {GP2X_BUTTON_B,SDLK_RETURN},
         {GP2X_BUTTON_X,SDLK_n},
         {GP2X_BUTTON_Y,SDLK_y},
         {GP2X_BUTTON_SELECT, SDLK_F10},
	 {GP2X_BUTTON_CLICK, SDLK_1},
	 {GP2X_BUTTON_L, SDLK_2},
	 {GP2X_BUTTON_R, SDLK_3},
	 {GP2X_BUTTON_VOLUP, KEY_USEASVOLUME},
	 {GP2X_BUTTON_VOLDOWN, KEY_USEASVOLUME},

         {GP2X_R_BUTTON_A,SDLK_SPACE},
         {GP2X_R_BUTTON_B,SDLK_RETURN},
         {GP2X_R_BUTTON_X,SDLK_n},
         {GP2X_R_BUTTON_Y,SDLK_y},
         {GP2X_R_BUTTON_SELECT, SDLK_F10},
	 {GP2X_R_BUTTON_CLICK, SDLK_1},
	 {GP2X_R_BUTTON_L, SDLK_2},
	 {GP2X_R_BUTTON_R, SDLK_3},
	 {GP2X_R_BUTTON_VOLUP, KEY_USEASVOLUME},
	 {GP2X_R_BUTTON_VOLDOWN, KEY_USEASVOLUME}*/
};

static struct BeebKeyTrans SDLtoBeebEmKeymap[]={

// New BBC keys:

{BBC_BREAK		-2,-2},

{BBC_F0,		2,0},
{BBC_F1,		7,1},
{BBC_F2,		7,2},
{BBC_F3,		7,3},
{BBC_F4,		1,4},
{BBC_F5,		7,4},
{BBC_F6,		7,5},
{BBC_F7,		1,6},
{BBC_F8,		7,6},
{BBC_F9,		7,7},
{BBC_ESCAPE,		7,0},
{BBC_1,			3,0},
{BBC_2,			3,1},
{BBC_3,			1,1},
{BBC_4,			1,2},
{BBC_5,			1,3},
{BBC_6,			3,4},
{BBC_7,			2,4},
{BBC_8,			1,5},
{BBC_9,			2,6},
{BBC_0,			2,7},
{BBC_EQUALS,		1,7},
{BBC_CARET,		1,8},
{BBC_BACKSLASH,		7,8},
{BBC_LEFT,		1,9},
{BBC_RIGHT,		7,9},
{BBC_TAB,		6,0},
{BBC_Q,			1,0},
{BBC_W,			2,1},
{BBC_E,			2,2},
{BBC_R,			3,3},
{BBC_T,			2,3},
{BBC_Y,			4,4},
{BBC_U,			3,5},
{BBC_I,			2,5},
{BBC_O,			3,6},
{BBC_P,			3,7},
{BBC_AT,		4,7},
{BBC_LEFTSQUAREBRACKET,	3,8},
{BBC_UNDERSCORE,	2,8},
{BBC_UP,		3,9},
{BBC_DOWN,		2,9},
{BBC_CAPSLOCK,		4,0},
{BBC_CTRL,		0,1},
{BBC_A,			4,1},
{BBC_S,			5,1},
{BBC_D,			3,2},
{BBC_F,			4,3},
{BBC_G,			5,3},
{BBC_H,			5,4},
{BBC_J,			4,5},
{BBC_K,			4,6},
{BBC_L,			5,6},
{BBC_PLUS,		5,7},
{BBC_ASTERIX,		4,8},
{BBC_RIGHTSQUAREBRACKET,	5,8},
{BBC_RETURN,		4,9},
{BBC_SHIFTLOCK,		5,0},
{BBC_SHIFT,		0,0},
{BBC_Z,			6,1},
{BBC_X,			4,2},
{BBC_C,			5,2},
{BBC_V,			6,3},
{BBC_B,			6,4},
{BBC_N,			5,5},
{BBC_M,			6,5},
{BBC_COMMA,		6,6},
{BBC_PERIOD,		6,7},
{BBC_FORWARDSLASH,	6,8},
{BBC_DELETE,		5,9},
{BBC_COPY,		6,9},
{BBC_SPACE,		6,2},


// Critical Beeb SDL keys:

{SDLK_TAB,              6,0},    // TAB
{SDLK_RETURN,           4,9},    // RETURN

{SDLK_LCTRL,            0,1},    // CONTROL
{SDLK_RCTRL,            0,1},    // CONTROL

{SDLK_LSHIFT,           0,0},   // SHIFT
{SDLK_RSHIFT,           0,0},   // SHIFT

{SDLK_CAPSLOCK,         4,0},    // CAPS LOCK (Totally fucked up in SDL..)
{SDLK_LSUPER,           4,0},   // CAPS LOCK (so Alt Gr is also CAPS-LOCK..)

{SDLK_ESCAPE,           7,0},    // ESCAPE
{SDLK_SPACE,            6,2},    // SPACE

{SDLK_LEFT,             1,9},    // LEFT
{SDLK_UP,               3,9},    // UP
{SDLK_RIGHT,            7,9},    // RIGHT
{SDLK_DOWN,             2,9},    // DOWN

{SDLK_DELETE,           5,9},    // DELETE
{SDLK_BACKSPACE,        5,9},    // DELETE

{SDLK_INSERT,           6,9},   // COPY

{SDLK_0,                2,7},    // 0
{SDLK_1,                3,0},    // 1
{SDLK_2,                3,1},    // 2
{SDLK_3,                1,1},    // 3
{SDLK_4,                1,2},    // 4
{SDLK_5,                1,3},    // 5
{SDLK_6,                3,4},    // 6
{SDLK_7,                2,4},    // 7
{SDLK_8,                1,5},    // 8
{SDLK_9,                2,6},    // 9
{SDLK_a,                4,1},    // A
{SDLK_b,                6,4},    // B
{SDLK_c,                5,2},    // C
{SDLK_d,                3,2},    // D
{SDLK_e,                2,2},    // E
{SDLK_f,                4,3},    // F
{SDLK_g,                5,3},    // G
{SDLK_h,                5,4},    // H
{SDLK_i,                2,5},    // I
{SDLK_j,                4,5},    // J
{SDLK_k,                4,6},    // K
{SDLK_l,                5,6},    // L
{SDLK_m,                6,5},    // M
{SDLK_n,                5,5},    // N
{SDLK_o,                3,6},    // O
{SDLK_p,                3,7},    // P
{SDLK_q,                1,0},    // Q
{SDLK_r,                3,3},    // R
{SDLK_s,                5,1},    // S
{SDLK_t,                2,3},    // T
{SDLK_u,                3,5},    // U
{SDLK_v,                6,3},    // V
{SDLK_w,                2,1},    // W
{SDLK_x,                4,2},    // X
{SDLK_y,                4,4},    // Y
{SDLK_z,                6,1},    // Z
{SDLK_F10,              2,0},    // f0
{SDLK_F1,               7,1},    // f1
{SDLK_F2,               7,2},    // f2
{SDLK_F3,               7,3},    // f3
{SDLK_F4,               1,4},    // f4
{SDLK_F5,               7,4},    // f5
{SDLK_F6,               7,5},    // f6
{SDLK_F7,               1,6},    // f7
{SDLK_F8,               7,6},    // f8
{SDLK_F9,               7,7},    // f9

{SDLK_MINUS,            5,7},   // "+" / ";"
{SDLK_COMMA,            6,6},   // "<" / ","
{SDLK_EQUALS,           1,7},   // "=" / "-"
{SDLK_PERIOD,           6,7},   // ">" / "."
{SDLK_BACKQUOTE,        2,8},   // "-" / "£"

{SDLK_SEMICOLON,        4,7},   // "@"
{SDLK_QUOTE,            4,8},   // "*" / ":"

{SDLK_SLASH,            6,8},   // "/" / "?"

{SDLK_HASH,             1,8},   // circumflex / tilde

{SDLK_PAUSE,            -2,-2}, // BREAK

{-1, -1, -1}              // ** END OF LIST **
};

int ConvertSDLKeyToBBCKey(int sym, int *col, int *row)
{
        struct BeebKeyTrans *p = SDLtoBeebEmKeymap;

        for(;((p->row != -1) && (p->sym != sym));p++) {}

        *(row) = p->row;
        *(col) = p->col;

        return *row<0 ? 0 : 1;
}

void apply_mapping(int gp2xButton, int sdlk)
{
        for (int i=0; i < (32); i++)
        {
                //printf("ZZZZZZZ %d\n", gp2x_mapping[i][0]);
                if (gp2x_mapping[i][0] == gp2xButton)
                {
                        printf("apply_mapping gp2xButton = %d   [%d][1] = %d\n", gp2xButton, i, sdlk);
                        gp2x_mapping[i][1] = sdlk;
                }
        }
}

/* Public wrapper for above:
 */
void Keyboard_SetKey(int button, int sdlk)
{
	apply_mapping(button, sdlk);
}


int gp2xButtonToSDLK(int button)
{
	int ret = -1;

#ifndef GEKKO
	// If upside down then use fake 'upside down' buttons instead:
	// mainWin check is just for now, mainWin is going...
	if (mainWin!=NULL)
		if (mainWin->ScreenIsReversed())
			button+=GP2X_REVERSED_BUTTONS_OFFSET;
#endif

	// The offset for Player #2 buttons is calculated in main.cpp
        for (int i = 0; i < 32; i++)
        {
                if (gp2x_mapping[i][0] == button)
                {
//                        printf ("XXX button = %d, sdlk = %d\n", button, gp2x_mapping[i][1]);
                        ret = gp2x_mapping[i][1];
                }
/*                else
                {
                        printf ("not found: %d tried against %d\n", button, gp2x_mapping[i][0]);
                }*/
        }

// This needs to be in main.
//	/* Handle volume buttons:
//	 * (If they are mapped to KEY_USEASVOLUME, then return -1 but handle volume change.)
//	 */
//	if (button == GP2X_BUTTON_VOLUP && ret == KEY_USEASVOLUME) {
//		if (OSS_Mixer_GetVolume()<100)
//        		OSS_Mixer_SetVolume(OSS_Mixer_GetVolume()+10);
//	}
//
//	if (button == GP2X_BUTTON_VOLDOWN && ret == KEY_USEASVOLUME) {
//		if (OSS_Mixer_GetVolume()>0)
//			OSS_Mixer_SetVolume(OSS_Mixer_GetVolume()-10);
//	}

	return ret;
}

/* Public version of above for mapping, offset is never added so mapper can always map correct button:
 */
int Keyboard_GetButtonMapping(int button)
{
	int ret = -1;

	for (int i = 0; i < 32; i++)
		if (gp2x_mapping[i][0] == button)
			ret = gp2x_mapping[i][1];

	return ret;
}


int buttonnameToGP2XButton(char* name)
{
	if (!strcasecmp (name, "P1_LEFT")) return WIIGC1_BUTTON_LEFT;
        if (!strcasecmp (name, "P1_RIGHT")) return WIIGC1_BUTTON_RIGHT;
        if (!strcasecmp (name, "P1_UP")) return WIIGC1_BUTTON_UP;
        if (!strcasecmp (name, "P1_DOWN")) return WIIGC1_BUTTON_DOWN;
        if (!strcasecmp (name, "P1_A")) return WIIGC1_BUTTON_A;
        if (!strcasecmp (name, "P1_B")) return WIIGC1_BUTTON_B;
        if (!strcasecmp (name, "P1_X")) return WIIGC1_BUTTON_X;
        if (!strcasecmp (name, "P1_Y")) return WIIGC1_BUTTON_Y;
        if (!strcasecmp (name, "P1_MINUS")) return WIIGC1_BUTTON_MINUS;
        if (!strcasecmp (name, "P1_CLASSIC_A")) return CLASSIC1_BUTTON_A;
	if (!strcasecmp (name, "P1_CLASSIC_B")) return CLASSIC1_BUTTON_B;
	if (!strcasecmp (name, "P1_CLASSIC_X")) return CLASSIC1_BUTTON_X;
	if (!strcasecmp (name, "P1_CLASSIC_Y")) return CLASSIC1_BUTTON_Y;
	if (!strcasecmp (name, "P1_CLASSIC_L")) return CLASSIC1_BUTTON_L;
	if (!strcasecmp (name, "P1_CLASSIC_R")) return CLASSIC1_BUTTON_R;

        if (!strcasecmp (name, "P2_LEFT")) return WIIGC2_BUTTON_LEFT;
        if (!strcasecmp (name, "P2_RIGHT")) return WIIGC2_BUTTON_RIGHT;
        if (!strcasecmp (name, "P2_UP")) return WIIGC2_BUTTON_UP;
        if (!strcasecmp (name, "P2_DOWN")) return WIIGC2_BUTTON_DOWN;
        if (!strcasecmp (name, "P2_A")) return WIIGC2_BUTTON_A;
        if (!strcasecmp (name, "P2_B")) return WIIGC2_BUTTON_B;
        if (!strcasecmp (name, "P2_X")) return WIIGC2_BUTTON_X;
        if (!strcasecmp (name, "P2_Y")) return WIIGC2_BUTTON_Y;
        if (!strcasecmp (name, "P2_MINUS")) return WIIGC2_BUTTON_MINUS;
        if (!strcasecmp (name, "P2_CLASSIC_A")) return CLASSIC2_BUTTON_A;
	if (!strcasecmp (name, "P2_CLASSIC_B")) return CLASSIC2_BUTTON_B;
	if (!strcasecmp (name, "P2_CLASSIC_X")) return CLASSIC2_BUTTON_X;
	if (!strcasecmp (name, "P2_CLASSIC_Y")) return CLASSIC2_BUTTON_Y;
	if (!strcasecmp (name, "P2_CLASSIC_L")) return CLASSIC2_BUTTON_L;
	if (!strcasecmp (name, "P2_CLASSIC_R")) return CLASSIC2_BUTTON_R;

        return -1;
}

char* SDLKToKeyname(int sdlk)
{
	// New BBC keys:

	if ( sdlk == BBC_F0) return "BBC_F0";
	if ( sdlk == BBC_F1) return "BBC_F1";
	if ( sdlk == BBC_F2) return "BBC_F2";
	if ( sdlk == BBC_F3) return "BBC_F3";
	if ( sdlk == BBC_F4) return "BBC_F4";
	if ( sdlk == BBC_F5) return "BBC_F5";
	if ( sdlk == BBC_F6) return "BBC_F6";
	if ( sdlk == BBC_F7) return "BBC_F7";
	if ( sdlk == BBC_F8) return "BBC_F8";
	if ( sdlk == BBC_F9) return "BBC_F9";
	if ( sdlk == BBC_ESCAPE) return "BBC_ESCAPE";
	if ( sdlk == BBC_1) return "BBC_1";
	if ( sdlk == BBC_2) return "BBC_2";
	if ( sdlk == BBC_3) return "BBC_3";
	if ( sdlk == BBC_4) return "BBC_4";
	if ( sdlk == BBC_5) return "BBC_5";
	if ( sdlk == BBC_6) return "BBC_6";
	if ( sdlk == BBC_7) return "BBC_7";
	if ( sdlk == BBC_8) return "BBC_8";
	if ( sdlk == BBC_9) return "BBC_9";
	if ( sdlk == BBC_0) return "BBC_0";
	if ( sdlk == BBC_EQUALS) return "BBC_EQUALS";
	if ( sdlk == BBC_CARET) return "BBC_CARET";
	if ( sdlk == BBC_BACKSLASH) return "BBC_BACKSLASH";
	if ( sdlk == BBC_LEFT) return "BBC_LEFT";
	if ( sdlk == BBC_RIGHT) return "BBC_RIGHT";
	if ( sdlk == BBC_TAB) return "BBC_TAB";
	if ( sdlk == BBC_Q) return "BBC_Q";
	if ( sdlk == BBC_W) return "BBC_W";
	if ( sdlk == BBC_E) return "BBC_E";
	if ( sdlk == BBC_R) return "BBC_R";
	if ( sdlk == BBC_T) return "BBC_T";
	if ( sdlk == BBC_Y) return "BBC_Y";
	if ( sdlk == BBC_U) return "BBC_U";
	if ( sdlk == BBC_I) return "BBC_I";
	if ( sdlk == BBC_O) return "BBC_O";
	if ( sdlk == BBC_P) return "BBC_P";
	if ( sdlk == BBC_AT) return "BBC_AT";

//	if ( sdlk == BBC_LEFTSQUAREBRACKET) return "BBC_LEFTSQUAREBRACKET";
	if ( sdlk == BBC_LEFTSQUAREBRACKET) return "BBC_LEFTSBRACKET";

	if ( sdlk == BBC_UNDERSCORE) return "BBC_UNDERSCORE";
	if ( sdlk == BBC_UP) return "BBC_UP";	
	if ( sdlk == BBC_DOWN) return "BBC_DOWN";
	if ( sdlk == BBC_CAPSLOCK) return "BBC_CAPSLOCK";
	if ( sdlk == BBC_CTRL) return "BBC_CTRL";
	if ( sdlk == BBC_A) return "BBC_A";
	if ( sdlk == BBC_S) return "BBC_S";
	if ( sdlk == BBC_D) return "BBC_D";
	if ( sdlk == BBC_F) return "BBC_F";
	if ( sdlk == BBC_G) return "BBC_G";
	if ( sdlk == BBC_H) return "BBC_H";
	if ( sdlk == BBC_J) return "BBC_J";
	if ( sdlk == BBC_K) return "BBC_K";
	if ( sdlk == BBC_L) return "BBC_L";
	if ( sdlk == BBC_PLUS) return "BBC_PLUS";
	if ( sdlk == BBC_ASTERIX) return "BBC_ASTERIX";

//	if ( sdlk == BBC_RIGHTSQUAREBRACKET) return "BBC_RIGHTSQUAREBRACKET";
	if ( sdlk == BBC_RIGHTSQUAREBRACKET) return "BBC_RIGHTSBRACKET";

	if ( sdlk == BBC_RETURN) return "BBC_RETURN";
	if ( sdlk == BBC_SHIFTLOCK) return "BBC_SHIFTLOCK";
	if ( sdlk == BBC_SHIFT) return "BBC_SHIFT";
	if ( sdlk == BBC_Z) return "BBC_Z";
	if ( sdlk == BBC_X) return "BBC_X";
	if ( sdlk == BBC_C) return "BBC_C";
	if ( sdlk == BBC_V) return "BBC_V";
	if ( sdlk == BBC_B) return "BBC_B";
	if ( sdlk == BBC_N) return "BBC_N";
	if ( sdlk == BBC_M) return "BBC_M";
	if ( sdlk == BBC_COMMA) return "BBC_COMMA";
	if ( sdlk == BBC_PERIOD) return "BBC_PERIOD";
	if ( sdlk == BBC_FORWARDSLASH) return "BBC_FORWARDSLASH";
	if ( sdlk == BBC_DELETE) return "BBC_DELETE";
	if ( sdlk == BBC_COPY) return "BBC_COPY";
	if ( sdlk == BBC_SPACE) return "BBC_SPACE";

	// Special keys:
	
	if ( sdlk == KEY_USEASVOLUME) return "VOLUME";
	if ( sdlk == KEY_NOTHING) return "NOTHING";

	// Critical Beeb keys:

        if (sdlk == SDLK_BACKSPACE) return "BACKSPACE";
        if (sdlk == SDLK_TAB) return "TAB";
        if (sdlk == SDLK_CLEAR) return "CLEAR";
        if (sdlk == SDLK_ESCAPE) return "ESCAPE";
        if (sdlk == SDLK_SPACE) return "SPACE";
        if (sdlk == SDLK_EXCLAIM) return "EXCLAIM";
        if (sdlk == SDLK_QUOTEDBL) return "QUOTEDBL";
        if (sdlk == SDLK_HASH) return "HASH";
        if (sdlk == SDLK_PAUSE) return "PAUSE";
        if (sdlk == SDLK_DOLLAR) return "DOLLAR";
        if (sdlk == SDLK_AMPERSAND) return "AMPERSAND";
        if (sdlk == SDLK_QUOTE) return "QUOTE";
        if (sdlk == SDLK_LEFTPAREN) return "LEFTPAREN";
        if (sdlk == SDLK_RIGHTPAREN) return "RIGHTPAREN";
        if (sdlk == SDLK_ASTERISK) return "ASTERISK";
        if (sdlk == SDLK_COMMA) return "COMMA";
        if (sdlk == SDLK_MINUS) return "MINUS";
        if (sdlk == SDLK_PERIOD) return "PERIOD";
        if (sdlk == SDLK_SLASH) return "SLASH";
        if (sdlk == SDLK_0) return "0";
        if (sdlk == SDLK_1) return "1";
        if (sdlk == SDLK_2) return "2";
        if (sdlk == SDLK_3) return "3";
        if (sdlk == SDLK_4) return "4";
        if (sdlk == SDLK_5) return "5";
        if (sdlk == SDLK_6) return "6";
        if (sdlk == SDLK_7) return "7";
        if (sdlk == SDLK_8) return "8";
        if (sdlk == SDLK_9) return "9";
        if (sdlk == SDLK_QUOTE) return "COLON";
        if (sdlk == SDLK_SEMICOLON) return "SEMICOLON";
        if (sdlk == SDLK_LESS) return "LESS";
        if (sdlk == SDLK_EQUALS) return "EQUALS";
        if (sdlk == SDLK_GREATER) return "GREATER";
        if (sdlk == SDLK_QUESTION) return "QUESTION";
        if (sdlk == SDLK_AT) return "AT";
        if (sdlk == SDLK_LEFTBRACKET) return "LEFTBRACKET";
        if (sdlk == SDLK_BACKSLASH) return "BACKSLASH";
        if (sdlk == SDLK_RIGHTBRACKET) return "RIGHTBRACKET";
        if (sdlk == SDLK_CARET) return "CARET";
        if (sdlk == SDLK_UNDERSCORE) return "UNDERSCORE";
        if (sdlk == SDLK_BACKQUOTE) return "BACKQUOTE";
        if (sdlk == SDLK_a) return "a";
        if (sdlk == SDLK_b) return "b";
        if (sdlk == SDLK_c) return "c";
        if (sdlk == SDLK_d) return "d";
        if (sdlk == SDLK_e) return "e";
        if (sdlk == SDLK_f) return "f";
        if (sdlk == SDLK_g) return "g";
        if (sdlk == SDLK_h) return "h";
        if (sdlk == SDLK_i) return "i";
        if (sdlk == SDLK_j) return "j";
        if (sdlk == SDLK_k) return "k";
        if (sdlk == SDLK_l) return "l";
        if (sdlk == SDLK_m) return "m";
        if (sdlk == SDLK_n) return "n";
        if (sdlk == SDLK_o) return "o";
        if (sdlk == SDLK_p) return "p";
        if (sdlk == SDLK_q) return "q";
        if (sdlk == SDLK_r) return "r";
        if (sdlk == SDLK_s) return "s";
        if (sdlk == SDLK_t) return "t";
        if (sdlk == SDLK_u) return "u";
        if (sdlk == SDLK_v) return "v";
        if (sdlk == SDLK_w) return "w";
        if (sdlk == SDLK_x) return "x";
        if (sdlk == SDLK_y) return "y";
        if (sdlk == SDLK_z) return "z";
        if (sdlk == SDLK_DELETE) return "DELETE";
        if (sdlk == SDLK_KP0) return "KP0";
        if (sdlk == SDLK_KP1) return "KP1";
        if (sdlk == SDLK_KP2) return "KP2";
        if (sdlk == SDLK_KP3) return "KP3";
        if (sdlk == SDLK_KP4) return "KP4";
        if (sdlk == SDLK_KP5) return "KP5";
        if (sdlk == SDLK_KP6) return "KP6";
        if (sdlk == SDLK_KP7) return "KP7";
        if (sdlk == SDLK_KP8) return "KP8";
        if (sdlk == SDLK_KP9) return "KP9";
        if (sdlk == SDLK_KP_PERIOD) return "KP_PERIOD";
        if (sdlk == SDLK_KP_DIVIDE) return "KP_DIVIDE";
        if (sdlk == SDLK_KP_MULTIPLY) return "KP_MULTIPLY";
        if (sdlk == SDLK_KP_MINUS) return "KP_MINUS";
        if (sdlk == SDLK_KP_PLUS) return "KP_PLUS";
        if (sdlk == SDLK_KP_ENTER) return "KP_ENTER";
        if (sdlk == SDLK_KP_EQUALS) return "KP_EQUALS";
        if (sdlk == SDLK_UP) return "UP";
        if (sdlk == SDLK_DOWN) return "DOWN";
        if (sdlk == SDLK_RIGHT) return "RIGHT";
        if (sdlk == SDLK_LEFT) return "LEFT";
        if (sdlk == SDLK_INSERT) return "INSERT";
        if (sdlk == SDLK_HOME) return "HOME";
        if (sdlk == SDLK_END) return "END";
        if (sdlk == SDLK_PAGEUP) return "PAGEUP";
        if (sdlk == SDLK_PAGEDOWN) return "PAGEDOWN";
        if (sdlk == SDLK_F10) return "F0";  // NB
        if (sdlk == SDLK_g) return "g";
        if (sdlk == SDLK_g) return "g";
        if (sdlk == SDLK_g) return "g";
        if (sdlk == SDLK_g) return "g";
        if (sdlk == SDLK_g) return "g";
        if (sdlk == SDLK_g) return "g";
        if (sdlk == SDLK_F1) return "F1";
        if (sdlk == SDLK_F2) return "F2";
        if (sdlk == SDLK_F3) return "F3";
        if (sdlk == SDLK_F4) return "F4";
        if (sdlk == SDLK_F5) return "F5";
        if (sdlk == SDLK_F6) return "F6";
        if (sdlk == SDLK_F7) return "F7";
        if (sdlk == SDLK_F8) return "F8";
        if (sdlk == SDLK_F9) return "F9";
        if (sdlk == SDLK_NUMLOCK) return "NUMLOCK";
        if (sdlk == SDLK_CAPSLOCK) return "CAPSLOCK";
        if (sdlk == SDLK_RSHIFT) return "RSHIFT";
        if (sdlk == SDLK_LSHIFT) return "LSHIFT";
        if (sdlk == SDLK_RCTRL) return "RCTRL";
        if (sdlk == SDLK_LCTRL) return "LCTRL";
        if (sdlk == SDLK_RALT) return "RALT";
        if (sdlk == SDLK_LALT) return "LALT";
        if (sdlk == SDLK_RMETA) return "RMETA";
        if (sdlk == SDLK_LMETA) return "LMETA";
        if (sdlk == SDLK_LSUPER) return "LSUPER";
        if (sdlk == SDLK_RSUPER) return "RSUPER";
        if (sdlk == SDLK_MODE) return "MODE";
        if (sdlk == SDLK_HELP) return "HELP";
        if (sdlk == SDLK_PRINT) return "PRINT";
        if (sdlk == SDLK_SYSREQ) return "SYSREQ";
        if (sdlk == SDLK_BREAK) return "BREAK";
        if (sdlk == SDLK_MENU) return "MENU";
        if (sdlk == SDLK_POWER) return "POWER";
        if (sdlk == SDLK_EURO) return "EURO";

        return "UNKNOWN";
}

/* Public wrapper of above function - keyboardmapper.cc uses this.
 */
const char* Keyboard_GetKeyName(int sdlk)
{
	return (const char*) SDLKToKeyname(sdlk);
}



int keynameToSDLK( char* name)
{
	// New BBC keys:

	if ( name[0] == 0) return KEY_NOTHING;

	if ( !strcasecmp (name, "BBC_F0")) return BBC_F0;
	if ( !strcasecmp (name, "BBC_F1")) return BBC_F1;
	if ( !strcasecmp (name, "BBC_F2")) return BBC_F2;
	if ( !strcasecmp (name, "BBC_F3")) return BBC_F3;
	if ( !strcasecmp (name, "BBC_F4")) return BBC_F4;
	if ( !strcasecmp (name, "BBC_F5")) return BBC_F5;
	if ( !strcasecmp (name, "BBC_F6")) return BBC_F6;
	if ( !strcasecmp (name, "BBC_F7")) return BBC_F7;
	if ( !strcasecmp (name, "BBC_F8")) return BBC_F8;
	if ( !strcasecmp (name, "BBC_F9")) return BBC_F9;
	if ( !strcasecmp (name, "BBC_ESCAPE")) return BBC_ESCAPE;
	if ( !strcasecmp (name, "BBC_1")) return BBC_1;
	if ( !strcasecmp (name, "BBC_EXCLAIMATION")) return BBC_EXCLAIMATION;
	if ( !strcasecmp (name, "BBC_!")) return BBC_EXCLAIMATION;
	if ( !strcasecmp (name, "BBC_2")) return BBC_2;
	if ( !strcasecmp (name, "BBC_DOUBLEQUOTE")) return BBC_DOUBLEQUOTE;
	if ( !strcasecmp (name, "BBC_\"")) return BBC_DOUBLEQUOTE;
	if ( !strcasecmp (name, "BBC_3")) return BBC_3;
	if ( !strcasecmp (name, "BBC_HASH")) return BBC_HASH;
	if ( !strcasecmp (name, "BBC_#")) return BBC_HASH;
	if ( !strcasecmp (name, "BBC_4")) return BBC_4;
	if ( !strcasecmp (name, "BBC_DOLLAR")) return BBC_DOLLAR;
	if ( !strcasecmp (name, "BBC_$")) return BBC_DOLLAR;
	if ( !strcasecmp (name, "BBC_5")) return BBC_5;
	if ( !strcasecmp (name, "BBC_PERCENT")) return BBC_PERCENT;
	if ( !strcasecmp (name, "BBC_%")) return BBC_PERCENT;
	if ( !strcasecmp (name, "BBC_6")) return BBC_6;
	if ( !strcasecmp (name, "BBC_AMPERSAND")) return BBC_AMPERSAND;
	if ( !strcasecmp (name, "BBC_&")) return BBC_AMPERSAND;
	if ( !strcasecmp (name, "BBC_7")) return BBC_7;
	if ( !strcasecmp (name, "BBC_QUOTE")) return BBC_QUOTE;
	if ( !strcasecmp (name, "BBC_'")) return BBC_QUOTE;
	if ( !strcasecmp (name, "BBC_`")) return BBC_QUOTE;
	if ( !strcasecmp (name, "BBC_8")) return BBC_8;
	if ( !strcasecmp (name, "BBC_LEFTBRACKET")) return BBC_LEFTBRACKET;
	if ( !strcasecmp (name, "BBC_(")) return BBC_LEFTBRACKET;
	if ( !strcasecmp (name, "BBC_9")) return BBC_9;
	if ( !strcasecmp (name, "BBC_RIGHTBRACKET")) return BBC_RIGHTBRACKET;
	if ( !strcasecmp (name, "BBC_)")) return BBC_RIGHTBRACKET;
	if ( !strcasecmp (name, "BBC_0")) return BBC_0;
	if ( !strcasecmp (name, "BBC_EQUALS")) return BBC_EQUALS;
	if ( !strcasecmp (name, "BBC_=")) return BBC_EQUALS;
	if ( !strcasecmp (name, "BBC_MINUS")) return BBC_MINUS;
	if ( !strcasecmp (name, "BBC_-")) return BBC_MINUS;
	if ( !strcasecmp (name, "BBC_DASH")) return BBC_DASH;
	if ( !strcasecmp (name, "BBC_HYPHEN")) return BBC_HYPHEN;
	if ( !strcasecmp (name, "BBC_CARET")) return BBC_CARET;
	if ( !strcasecmp (name, "BBC_^")) return BBC_CARET;
	if ( !strcasecmp (name, "BBC_TILDE")) return BBC_TILDE;
	if ( !strcasecmp (name, "BBC_~")) return BBC_TILDE;
	if ( !strcasecmp (name, "BBC_BACKSLASH")) return BBC_BACKSLASH;
	if ( !strcasecmp (name, "BBC_\\")) return BBC_BACKSLASH;
	if ( !strcasecmp (name, "BBC_BAR")) return BBC_BAR;
	if ( !strcasecmp (name, "BBC_|")) return BBC_BAR;
	if ( !strcasecmp (name, "BBC_VERTICALBAR")) return BBC_VERTICALBAR;
	if ( !strcasecmp (name, "BBC_LEFT")) return BBC_LEFT;
	if ( !strcasecmp (name, "BBC_RIGHT")) return BBC_RIGHT;
	if ( !strcasecmp (name, "BBC_TAB")) return BBC_TAB;
	if ( !strcasecmp (name, "BBC_Q")) return BBC_Q;
	if ( !strcasecmp (name, "BBC_W")) return BBC_W;
	if ( !strcasecmp (name, "BBC_E")) return BBC_E;
	if ( !strcasecmp (name, "BBC_R")) return BBC_R;
	if ( !strcasecmp (name, "BBC_T")) return BBC_T;
	if ( !strcasecmp (name, "BBC_Y")) return BBC_Y;
	if ( !strcasecmp (name, "BBC_U")) return BBC_U;
	if ( !strcasecmp (name, "BBC_I")) return BBC_I;
	if ( !strcasecmp (name, "BBC_O")) return BBC_O;
	if ( !strcasecmp (name, "BBC_P")) return BBC_P;
	if ( !strcasecmp (name, "BBC_AT")) return BBC_AT;
	if ( !strcasecmp (name, "BBC_@")) return BBC_AT;
	if ( !strcasecmp (name, "BBC_ATSIGN")) return BBC_ATSIGN;
	if ( !strcasecmp (name, "BBC_LEFTSBRACKET")) return BBC_LEFTSQUAREBRACKET;
	if ( !strcasecmp (name, "BBC_LEFTSQUAREBRACKET")) return BBC_LEFTSQUAREBRACKET;
	if ( !strcasecmp (name, "BBC_[")) return BBC_LEFTSQUAREBRACKET;
	if ( !strcasecmp (name, "BBC_LEFTCBRACKET")) return BBC_LEFTCURLYBRACKET;
	if ( !strcasecmp (name, "BBC_LEFTCURLYBRACKET")) return BBC_LEFTCURLYBRACKET;
	if ( !strcasecmp (name, "BBC_{")) return BBC_LEFTCURLYBRACKET;
	if ( !strcasecmp (name, "BBC_UNDERSCORE")) return BBC_UNDERSCORE;
	if ( !strcasecmp (name, "BBC__")) return BBC_UNDERSCORE;
	if ( !strcasecmp (name, "BBC_POUND")) return BBC_POUND;
	if ( !strcasecmp (name, "BBC_UP")) return BBC_UP;	
	if ( !strcasecmp (name, "BBC_DOWN")) return BBC_DOWN;
	if ( !strcasecmp (name, "BBC_CAPSLOCK")) return BBC_CAPSLOCK;
	if ( !strcasecmp (name, "BBC_CTRL")) return BBC_CTRL;
	if ( !strcasecmp (name, "BBC_CONTROL")) return BBC_CONTROL;
	if ( !strcasecmp (name, "BBC_A")) return BBC_A;
	if ( !strcasecmp (name, "BBC_S")) return BBC_S;
	if ( !strcasecmp (name, "BBC_D")) return BBC_D;
	if ( !strcasecmp (name, "BBC_F")) return BBC_F;
	if ( !strcasecmp (name, "BBC_G")) return BBC_G;
	if ( !strcasecmp (name, "BBC_H")) return BBC_H;
	if ( !strcasecmp (name, "BBC_J")) return BBC_J;
	if ( !strcasecmp (name, "BBC_K")) return BBC_K;
	if ( !strcasecmp (name, "BBC_L")) return BBC_L;
	if ( !strcasecmp (name, "BBC_PLUS")) return BBC_PLUS;
	if ( !strcasecmp (name, "BBC_+")) return BBC_PLUS;
	if ( !strcasecmp (name, "BBC_SEMICOLON")) return BBC_SEMICOLON;
	if ( !strcasecmp (name, "BBC_;")) return BBC_SEMICOLON;
	if ( !strcasecmp (name, "BBC_ASTERIX")) return BBC_ASTERIX;
	if ( !strcasecmp (name, "BBC_*")) return BBC_ASTERIX;
	if ( !strcasecmp (name, "BBC_COLON")) return BBC_COLON;
	if ( !strcasecmp (name, "BBC_:")) return BBC_COLON;
	if ( !strcasecmp (name, "BBC_RIGHTSBRACKET")) return BBC_RIGHTSQUAREBRACKET;
	if ( !strcasecmp (name, "BBC_RIGHTSQUAREBRACKET")) return BBC_RIGHTSQUAREBRACKET;
	if ( !strcasecmp (name, "BBC_]")) return BBC_RIGHTSQUAREBRACKET;
	if ( !strcasecmp (name, "BBC_RIGHTCBRACKET")) return BBC_RIGHTCURLYBRACKET;
	if ( !strcasecmp (name, "BBC_RIGHTCURLYBRACKET")) return BBC_RIGHTCURLYBRACKET;
	if ( !strcasecmp (name, "BBC_}")) return BBC_RIGHTCURLYBRACKET;
	if ( !strcasecmp (name, "BBC_RETURN")) return BBC_RETURN;
	if ( !strcasecmp (name, "BBC_ENTER")) return BBC_ENTER;
	if ( !strcasecmp (name, "BBC_SHIFTLOCK")) return BBC_SHIFTLOCK;
	if ( !strcasecmp (name, "BBC_SHIFT")) return BBC_SHIFT;
	if ( !strcasecmp (name, "BBC_LEFTSHIFT")) return BBC_LEFTSHIFT;
	if ( !strcasecmp (name, "BBC_LEFT_SHIFT")) return BBC_LEFTSHIFT;
	if ( !strcasecmp (name, "BBC_LSHIFT")) return BBC_LSHIFT;
	if ( !strcasecmp (name, "BBC_L_SHIFT")) return BBC_LEFTSHIFT;
	if ( !strcasecmp (name, "BBC_RIGHTSHIFT")) return BBC_RIGHTSHIFT;
	if ( !strcasecmp (name, "BBC_RIGHT_SHIFT")) return BBC_RIGHTSHIFT;
	if ( !strcasecmp (name, "BBC_RSHIFT")) return BBC_RSHIFT;
	if ( !strcasecmp (name, "BBC_R_SHIFT")) return BBC_RSHIFT;
	if ( !strcasecmp (name, "BBC_Z")) return BBC_Z;
	if ( !strcasecmp (name, "BBC_X")) return BBC_X;
	if ( !strcasecmp (name, "BBC_C")) return BBC_C;
	if ( !strcasecmp (name, "BBC_V")) return BBC_V;
	if ( !strcasecmp (name, "BBC_B")) return BBC_B;
	if ( !strcasecmp (name, "BBC_N")) return BBC_N;
	if ( !strcasecmp (name, "BBC_M")) return BBC_M;
	if ( !strcasecmp (name, "BBC_COMMA")) return BBC_COMMA;
	if ( !strcasecmp (name, "BBC_,")) return BBC_COMMA;
	if ( !strcasecmp (name, "BBC_LEFTABRACKET")) return BBC_LEFTANGLEBRACKET;
	if ( !strcasecmp (name, "BBC_LEFTANGLEBRACKET")) return BBC_LEFTANGLEBRACKET;
	if ( !strcasecmp (name, "BBC_<")) return BBC_LEFTANGLEBRACKET;
	if ( !strcasecmp (name, "BBC_PERIOD")) return BBC_PERIOD;
	if ( !strcasecmp (name, "BBC_.")) return BBC_PERIOD;
	if ( !strcasecmp (name, "BBC_RIGHTABRACKET")) return BBC_RIGHTANGLEBRACKET;
	if ( !strcasecmp (name, "BBC_RIGHTANGLEBRACKET")) return BBC_RIGHTANGLEBRACKET;
	if ( !strcasecmp (name, "BBC_>")) return BBC_RIGHTANGLEBRACKET;
	if ( !strcasecmp (name, "BBC_FORWARDSLASH")) return BBC_FORWARDSLASH;
	if ( !strcasecmp (name, "BBC_/")) return BBC_FORWARDSLASH;
	if ( !strcasecmp (name, "BBC_QUESTION")) return BBC_QUESTION;
	if ( !strcasecmp (name, "BBC_?")) return BBC_QUESTION;
	if ( !strcasecmp (name, "BBC_QUESTIONMARK")) return BBC_QUESTIONMARK;
	if ( !strcasecmp (name, "BBC_DELETE")) return BBC_DELETE;
	if ( !strcasecmp (name, "BBC_DEL")) return BBC_DEL;
	if ( !strcasecmp (name, "BBC_BACKSPACE")) return BBC_BACKSPACE;
	if ( !strcasecmp (name, "BBC_COPY")) return BBC_COPY;
	if ( !strcasecmp (name, "BBC_SPACE")) return BBC_SPACE;

	// Special keys:

	if ( !strcasecmp (name, "VOLUME")) return KEY_USEASVOLUME;
	if ( !strcasecmp (name, "NOTHING")) return KEY_NOTHING;

	// Critical Beeb keys:

        if( !strcasecmp (name, "BACKSPACE")) return SDLK_BACKSPACE;
        if( !strcasecmp (name, "TAB")) return SDLK_TAB;
        if( !strcasecmp (name, "CLEAR")) return SDLK_CLEAR;
        if( !strcasecmp (name, "RETURN")) return SDLK_RETURN;
        if( !strcasecmp (name, "ESCAPE")) return SDLK_ESCAPE;
        if( !strcasecmp (name, "SPACE")) return SDLK_SPACE;
        if( !strcasecmp (name, "EXCLAIM")) return SDLK_EXCLAIM;
        if( !strcasecmp (name, "QUOTEDBL")) return SDLK_QUOTEDBL;
        if( !strcasecmp (name, "HASH")) return SDLK_HASH;
        if( !strcasecmp (name, "PAUSE")) return SDLK_PAUSE;
        if( !strcasecmp (name, "DOLLAR")) return SDLK_DOLLAR;
        if( !strcasecmp (name, "AMPERSAND")) return SDLK_AMPERSAND;
        if( !strcasecmp (name, "QUOTE")) return SDLK_QUOTE;
        if( !strcasecmp (name, "LEFTPAREN")) return SDLK_LEFTPAREN;
        if( !strcasecmp (name, "RIGHTPAREN")) return SDLK_RIGHTPAREN;
        if( !strcasecmp (name, "ASTERISK")) return SDLK_ASTERISK;
        if( !strcasecmp (name, "PLUS")) return SDLK_PLUS;
        if( !strcasecmp (name, "COMMA")) return SDLK_COMMA;
        if( !strcasecmp (name, "MINUS")) return SDLK_MINUS;
        if( !strcasecmp (name, "PERIOD")) return SDLK_PERIOD;
        if( !strcasecmp (name, "SLASH")) return SDLK_SLASH;
        if( !strcasecmp (name, "0")) return SDLK_0;
        if( !strcasecmp (name, "1")) return SDLK_1;
        if( !strcasecmp (name, "2")) return SDLK_2;
        if( !strcasecmp (name, "3")) return SDLK_3;
        if( !strcasecmp (name, "4")) return SDLK_4;
        if( !strcasecmp (name, "5")) return SDLK_5;
        if( !strcasecmp (name, "6")) return SDLK_6;
        if( !strcasecmp (name, "7")) return SDLK_7;
        if( !strcasecmp (name, "8")) return SDLK_8;
        if( !strcasecmp (name, "9")) return SDLK_9;
        if( !strcasecmp (name, "COLON")) return SDLK_QUOTE;  // dodgy mapping
        if( !strcasecmp (name, "SEMICOLON")) return SDLK_SEMICOLON;
        if( !strcasecmp (name, "LESS")) return SDLK_LESS;
        if( !strcasecmp (name, "EQUALS")) return SDLK_EQUALS;
        if( !strcasecmp (name, "GREATER")) return SDLK_GREATER;
        if( !strcasecmp (name, "QUESTION")) return SDLK_QUESTION;
        if( !strcasecmp (name, "AT")) return SDLK_AT;
        if( !strcasecmp (name, "LEFTBRACKET")) return SDLK_LEFTBRACKET;
        if( !strcasecmp (name, "BACKSLASH")) return SDLK_BACKSLASH;
        if( !strcasecmp (name, "RIGHTBRACKET")) return SDLK_RIGHTBRACKET;
        if( !strcasecmp (name, "CARET")) return SDLK_CARET;
        if( !strcasecmp (name, "UNDERSCORE")) return SDLK_UNDERSCORE;
        if( !strcasecmp (name, "BACKQUOTE")) return SDLK_BACKQUOTE;
        if( !strcasecmp (name, "a")) return SDLK_a;
        if( !strcasecmp (name, "b")) return SDLK_b;
        if( !strcasecmp (name, "c")) return SDLK_c;
        if( !strcasecmp (name, "d")) return SDLK_d;
        if( !strcasecmp (name, "e")) return SDLK_e;
        if( !strcasecmp (name, "f")) return SDLK_f;
        if( !strcasecmp (name, "g")) return SDLK_g;
        if( !strcasecmp (name, "h")) return SDLK_h;
        if( !strcasecmp (name, "i")) return SDLK_i;
        if( !strcasecmp (name, "j")) return SDLK_j;
        if( !strcasecmp (name, "k")) return SDLK_k;
        if( !strcasecmp (name, "l")) return SDLK_l;
        if( !strcasecmp (name, "m")) return SDLK_m;
        if( !strcasecmp (name, "n")) return SDLK_n;
        if( !strcasecmp (name, "o")) return SDLK_o;
        if( !strcasecmp (name, "p")) return SDLK_p;
        if( !strcasecmp (name, "q")) return SDLK_q;
        if( !strcasecmp (name, "r")) return SDLK_r;
        if( !strcasecmp (name, "s")) return SDLK_s;
        if( !strcasecmp (name, "t")) return SDLK_t;
        if( !strcasecmp (name, "u")) return SDLK_u;
        if( !strcasecmp (name, "v")) return SDLK_v;
        if( !strcasecmp (name, "w")) return SDLK_w;
        if( !strcasecmp (name, "x")) return SDLK_x;
        if( !strcasecmp (name, "y")) return SDLK_y;
        if( !strcasecmp (name, "z")) return SDLK_z;
        if( !strcasecmp (name, "DELETE")) return SDLK_DELETE;
        if( !strcasecmp (name, "KP0")) return SDLK_KP0;
        if( !strcasecmp (name, "KP1")) return SDLK_KP1;
        if( !strcasecmp (name, "KP2")) return SDLK_KP2;
        if( !strcasecmp (name, "KP3")) return SDLK_KP3;
        if( !strcasecmp (name, "KP4")) return SDLK_KP4;
        if( !strcasecmp (name, "KP5")) return SDLK_KP5;
        if( !strcasecmp (name, "KP6")) return SDLK_KP6;
        if( !strcasecmp (name, "KP7")) return SDLK_KP7;
        if( !strcasecmp (name, "KP8")) return SDLK_KP8;
        if( !strcasecmp (name, "KP9")) return SDLK_KP9;
        if( !strcasecmp (name, "KP_PERIOD")) return SDLK_KP_PERIOD;
        if( !strcasecmp (name, "KP_DIVIDE")) return SDLK_KP_DIVIDE;
        if( !strcasecmp (name, "KP_MULTIPLY")) return SDLK_KP_MULTIPLY;
        if( !strcasecmp (name, "KP_MINUS")) return SDLK_KP_MINUS;
        if( !strcasecmp (name, "KP_PLUS")) return SDLK_KP_PLUS;
        if( !strcasecmp (name, "KP_ENTER")) return SDLK_KP_ENTER;
        if( !strcasecmp (name, "KP_EQUALS")) return SDLK_KP_EQUALS;
        if( !strcasecmp (name, "UP")) return SDLK_UP;
        if( !strcasecmp (name, "DOWN")) return SDLK_DOWN;
        if( !strcasecmp (name, "RIGHT")) return SDLK_RIGHT;
        if( !strcasecmp (name, "LEFT")) return SDLK_LEFT;
        if( !strcasecmp (name, "INSERT")) return SDLK_INSERT;
        if( !strcasecmp (name, "HOME")) return SDLK_HOME;
        if( !strcasecmp (name, "END")) return SDLK_END;
        if( !strcasecmp (name, "PAGEUP")) return SDLK_PAGEUP;
        if( !strcasecmp (name, "PAGEDOWN")) return SDLK_PAGEDOWN;
        if( !strcasecmp (name, "F0")) return SDLK_F10;
        if( !strcasecmp (name, "F1")) return SDLK_F1;
        if( !strcasecmp (name, "F2")) return SDLK_F2;
        if( !strcasecmp (name, "F3")) return SDLK_F3;
        if( !strcasecmp (name, "F4")) return SDLK_F4;
        if( !strcasecmp (name, "F5")) return SDLK_F5;
        if( !strcasecmp (name, "F6")) return SDLK_F6;
        if( !strcasecmp (name, "F7")) return SDLK_F7;
        if( !strcasecmp (name, "F8")) return SDLK_F8;
        if( !strcasecmp (name, "F9")) return SDLK_F9;
        if( !strcasecmp (name, "F10")) return SDLK_F10;
        if( !strcasecmp (name, "F11")) return SDLK_F11;
        if( !strcasecmp (name, "F12")) return SDLK_F12;
        if( !strcasecmp (name, "F13")) return SDLK_F13;
        if( !strcasecmp (name, "F14")) return SDLK_F14;
        if( !strcasecmp (name, "F15")) return SDLK_F15;
        if( !strcasecmp (name, "NUMLOCK")) return SDLK_NUMLOCK;
        if( !strcasecmp (name, "CAPSLOCK")) return SDLK_CAPSLOCK;
        if( !strcasecmp (name, "SCROLLOCK")) return SDLK_SCROLLOCK;
        if( !strcasecmp (name, "RSHIFT")) return SDLK_RSHIFT;
        if( !strcasecmp (name, "LSHIFT")) return SDLK_LSHIFT;
        if( !strcasecmp (name, "SHIFT")) return SDLK_LSHIFT; // for convenience
        if( !strcasecmp (name, "RCTRL")) return SDLK_RCTRL;
        if( !strcasecmp (name, "LCTRL")) return SDLK_LCTRL;
        if( !strcasecmp (name, "CTRL")) return SDLK_LCTRL; // // for convenience
        if( !strcasecmp (name, "RALT")) return SDLK_RALT;
        if( !strcasecmp (name, "LALT")) return SDLK_LALT;
        if( !strcasecmp (name, "RMETA")) return SDLK_RMETA;
        if( !strcasecmp (name, "LMETA")) return SDLK_LMETA;
        if( !strcasecmp (name, "LSUPER")) return SDLK_LSUPER;
        if( !strcasecmp (name, "RSUPER")) return SDLK_RSUPER;
        if( !strcasecmp (name, "MODE")) return SDLK_MODE;
        if( !strcasecmp (name, "HELP")) return SDLK_HELP;
        if( !strcasecmp (name, "PRINT")) return SDLK_PRINT;
        if( !strcasecmp (name, "SYSREQ")) return SDLK_SYSREQ;
        if( !strcasecmp (name, "BREAK")) return SDLK_BREAK;
        if( !strcasecmp (name, "MENU")) return SDLK_MENU;
        if( !strcasecmp (name, "POWER")) return SDLK_POWER;
        if( !strcasecmp (name, "EURO")) return SDLK_EURO;
        return -1;
}

/* Public wrapper of above function - keyboardmapper.cc uses this.
 */
int Keyboard_GetKeyNumeric(const char *key_string)
{
	return keynameToSDLK( (char*) key_string);
}


int kbd_load_mapping_file(FILE *KbdFile)
{
        char     Buffer[512];
        char    *Scan;
        char    *sdlkName;

        while (fgets(Buffer,512,KbdFile) != (char *)0)
        {

                Scan = strchr(Buffer,'\n');
                if (Scan) *Scan = '\0';
                /* For this #@$% of windows ! */
                Scan = strchr(Buffer,'\r');
                if (Scan) *Scan = '\0';

		RemoveLeadingWhiteSpace(Buffer);

                if (Buffer[0] == '#') continue;

		/* If this line is a SET option, process it externally instead:
	 	 */
		if (ProcessSetOption(Buffer)) continue;

                Scan = strchr(Buffer,'=');
                if (! Scan) continue;

                *Scan = '\0';
                sdlkName = Scan + 1;

                int gp2xButtonNum = buttonnameToGP2XButton(Buffer);
                printf ("gp2x = %s, sdlk = %s\n", Buffer, sdlkName);
                int sdlk = keynameToSDLK(sdlkName);
                apply_mapping(gp2xButtonNum, sdlk);
        }

        // now populate the lookup table
	gp2x_key_lookup[JOY_UP] = gp2xButtonToSDLK(WIIGC1_BUTTON_UP);
        gp2x_key_lookup[JOY_DOWN] = gp2xButtonToSDLK(WIIGC1_BUTTON_DOWN);
        gp2x_key_lookup[JOY_LEFT] = gp2xButtonToSDLK(WIIGC1_BUTTON_LEFT);
        gp2x_key_lookup[JOY_RIGHT] = gp2xButtonToSDLK(WIIGC1_BUTTON_RIGHT);
        gp2x_key_lookup[JOY_A] = gp2xButtonToSDLK(WIIGC1_BUTTON_A);
        gp2x_key_lookup[JOY_B] = gp2xButtonToSDLK(WIIGC1_BUTTON_B);
        gp2x_key_lookup[JOY_X] = gp2xButtonToSDLK(WIIGC1_BUTTON_X);
        gp2x_key_lookup[JOY_Y] = gp2xButtonToSDLK(WIIGC1_BUTTON_Y);
        gp2x_key_lookup[JOY_MINUS] = gp2xButtonToSDLK(WIIGC1_BUTTON_MINUS);
	gp2x_key_lookup[JOY_Z] = gp2xButtonToSDLK(WIIGC1_BUTTON_TRIGGER_Z);
	gp2x_key_lookup[JOY_CLASSIC_A] = gp2xButtonToSDLK(CLASSIC1_BUTTON_A);
	gp2x_key_lookup[JOY_CLASSIC_B] = gp2xButtonToSDLK(CLASSIC1_BUTTON_B);
	gp2x_key_lookup[JOY_CLASSIC_X] = gp2xButtonToSDLK(CLASSIC1_BUTTON_X);
	gp2x_key_lookup[JOY_CLASSIC_Y] = gp2xButtonToSDLK(CLASSIC1_BUTTON_Y);
	gp2x_key_lookup[JOY_L] = gp2xButtonToSDLK(CLASSIC1_BUTTON_L);
	gp2x_key_lookup[JOY_R] = gp2xButtonToSDLK(CLASSIC1_BUTTON_R);

	gp2x_key_lookup[R_JOY_UP] = gp2xButtonToSDLK(WIIGC2_BUTTON_UP);
        gp2x_key_lookup[R_JOY_DOWN] = gp2xButtonToSDLK(WIIGC2_BUTTON_DOWN);
        gp2x_key_lookup[R_JOY_LEFT] = gp2xButtonToSDLK(WIIGC2_BUTTON_LEFT);
        gp2x_key_lookup[R_JOY_RIGHT] = gp2xButtonToSDLK(WIIGC2_BUTTON_RIGHT);
        gp2x_key_lookup[R_JOY_A] = gp2xButtonToSDLK(WIIGC2_BUTTON_A);
        gp2x_key_lookup[R_JOY_B] = gp2xButtonToSDLK(WIIGC2_BUTTON_B);
        gp2x_key_lookup[R_JOY_X] = gp2xButtonToSDLK(WIIGC2_BUTTON_X);
        gp2x_key_lookup[R_JOY_Y] = gp2xButtonToSDLK(WIIGC2_BUTTON_Y);
        gp2x_key_lookup[R_JOY_MINUS] = gp2xButtonToSDLK(WIIGC2_BUTTON_MINUS);
	gp2x_key_lookup[R_JOY_Z] = gp2xButtonToSDLK(WIIGC2_BUTTON_TRIGGER_Z);
	gp2x_key_lookup[R_JOY_CLASSIC_A] = gp2xButtonToSDLK(CLASSIC2_BUTTON_A);
	gp2x_key_lookup[R_JOY_CLASSIC_A] = gp2xButtonToSDLK(CLASSIC2_BUTTON_B);
	gp2x_key_lookup[R_JOY_CLASSIC_A] = gp2xButtonToSDLK(CLASSIC2_BUTTON_X);
	gp2x_key_lookup[R_JOY_CLASSIC_A] = gp2xButtonToSDLK(CLASSIC2_BUTTON_Y);
	gp2x_key_lookup[R_JOY_L] = gp2xButtonToSDLK(CLASSIC2_BUTTON_L);
	gp2x_key_lookup[R_JOY_R] = gp2xButtonToSDLK(CLASSIC2_BUTTON_R);

        return 0;
}


int LoadKeyboardMapping(char *filename)
{
	int ret;

	ret = kbd_load_mapping(filename);

	if (ret == 0)
		return 1;
	else
		return 0;
}

int kbd_load_mapping(char *kbd_filename)
{
        FILE    *KbdFile;
        int      error = 0;

        KbdFile = fopen(kbd_filename, "r");
        error   = 1;

        if (KbdFile != (FILE*)0)
        {
                kbd_load_mapping_file(KbdFile);
                error = 0;
                fclose(KbdFile);
        }

        return error;
}

void HandleButtonPress(int pressed, int joy)
{
        int col, row;

        int keysym = gp2x_key_lookup[joy];

        if (ConvertSDLKeyToBBCKey(keysym, &col, &row))
        {
                if (pressed)
                        BeebKeyDown(row, col);
                else
                        BeebKeyUp(row, col);
        }
}

void AllDirectionButtonsUp()
{
        int col, row;

//	return;

	// L,R,UP, DOWN normal + L,R, Up, DOWN reversed.
        for (int i = 0; i < (4*2); i++)
        {
                ConvertSDLKeyToBBCKey(gp2x_key_lookup[i], &col, &row);
                BeebKeyUp(row, col);
        }
}

void AllButtonsUp(void)
{
	int col, row;

	for (int i = 0; i < BUTTON_COUNT; i++) {
		ConvertSDLKeyToBBCKey(gp2x_key_lookup[i], &col, &row);
		BeebKeyUp(row, col);
	}
}

void HandleButtonPressForKeysym(int pressed, int keysym)
{
        int col, row;

	// Will do nothing when keysym = -1 (which it will sometimes).

        if (ConvertSDLKeyToBBCKey(keysym, &col, &row))
        {
                if (pressed)
                        BeebKeyDown(row, col);
                else
                        BeebKeyUp(row, col);
        }


//	printf("col = %d, row = %d\n", col, row);
}
