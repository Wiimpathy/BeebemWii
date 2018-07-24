/* Support for Rooster's cool GP2x virtual keyboard:
 */

#include "beebconfig.h"
#include "beebconfig_data.h"
#include "sysvia.h"
#include "main.h"
#include "disc8271.h"
#include "6502core.h"

#include "virtualkeyboard.h"
#include "keyboard.h"
#include "beebwin.h"
#include <SDL.h>

#define LOCK(s)   {if(SDL_MUSTLOCK(s))(void)SDL_LockSurface(s);}
#define UNLOCK(s) {if(SDL_MUSTLOCK(s))(void)SDL_UnlockSurface(s);}


/* Private stuff:
 */
 
static void		SetFocusTo(unsigned int key_index);
static void 		RenderKey(unsigned int key_index);
static SDL_Surface* 	CreateNewFlippedSurface(SDL_Surface *source_p);

/* Keyboard mapping:
 * =================
 */

#define KEYBOARD_HEIGHT 136
#define KEYBOARD_WIDTH  320

#define SELECTED	0
#define UNSELECTED	1
#define HELDANDSELECTED	2
#define	PRESSED		3
#define HELD		4
typedef int KeyState;

typedef struct {
	char		name[15+1];
	unsigned int 	x, y, width, height;
	int		left, right, up, down;
	int 		special, row, column;

	KeyState	state;
} VirtualKey;

#define KEYCOUNT 87

static VirtualKey keyboard[KEYCOUNT] = {
/*
*Name, x,   y,    w,   h,   L,  R,  U,  D,    S,  R, C, State
 */
{"f0",  70,  30,  16,  16, -1,  1, -1, 13,   -1, 2, 0, UNSELECTED}, /*   0 */
{"f1",  87,  30,  16,  16,  0,  2, -1, 14,   -1, 7, 1, UNSELECTED}, /*   1 */
{"f2", 104,  30,  16,  16,  1,  3, -1, 15,   -1, 7, 2, UNSELECTED}, /*   2 */
{"f3", 121,  30,  16,  16,  2,  4, -1, 16,   -1, 7, 3, UNSELECTED}, /*   3 */
{"f4", 138,  30,  16,  16,  3,  5, -1, 17,   -1, 1, 4, UNSELECTED}, /*   4 */
{"f5", 155,  30,  16,  16,  4,  6, -1, 18,   -1, 7, 4, UNSELECTED}, /*   5 */
{"f6", 172,  30,  16,  16,  5,  7, -1, 19,   -1, 7, 5, UNSELECTED}, /*   6 */
{"f7", 189,  30,  16,  16,  6,  8, -1, 20,   -1, 1, 6, UNSELECTED}, /*   7 */
{"f8", 206,  30,  16,  16,  7,  9, -1, 21,   -1, 7, 6, UNSELECTED}, /*   8 */
{"f9", 223,  30,  16,  16,  8, 10, -1, 22,   -1, 7, 7, UNSELECTED}, /*   9 */

{"Break",
       240,  30,  16,  16,  9, -1, -1, 23,   -1,-2,-2, UNSELECTED}, /*  10 */ 

/////////////////////////////////////////
/*
*Name, x,   y,    w,   h,   L,  R,  U,  D,    S,  R, C, State
 */
{"Escape",
       33,  46,  16,  16,   -1, 12, -1, 27,   -1, 7, 0, UNSELECTED}, /*  11 */

{"1",  50,  46,  16,  16,   11, 13, -1, 28,   -1, 3, 0, UNSELECTED}, /*  12 */
{"2",  67,  46,  16,  16,   12, 14,  0, 29,   -1, 3, 1, UNSELECTED}, /*  13 */
{"3",  84,  46,  16,  16,   13, 15,  1, 30,   -1, 1, 1, UNSELECTED}, /*  14 */
{"4", 101,  46,  16,  16,   14, 16,  2, 31,   -1, 1, 2, UNSELECTED}, /*  15 */
{"5", 118,  46,  16,  16,   15, 17,  3, 32,   -1, 1, 3, UNSELECTED}, /*  16 */
{"6", 135,  46,  16,  16,   16, 18,  4, 33,   -1, 3, 4, UNSELECTED}, /*  17 */
{"7", 152,  46,  16,  16,   17, 19,  5, 34,   -1, 2, 4, UNSELECTED}, /*  18 */
{"8", 169,  46,  16,  16,   18, 20,  6, 35,   -1, 1, 5, UNSELECTED}, /*  19 */
{"9", 186,  46,  16,  16,   19, 21,  7, 36,   -1, 2, 6, UNSELECTED}, /*  20 */
{"0", 203,  46,  16,  16,   20, 22,  8, 37,   -1, 2, 7, UNSELECTED}, /*  21 */
{"=", 220,  46,  16,  16,   21, 23,  9, 38,   -1, 1, 7, UNSELECTED}, /*  22 */
{"~", 237,  46,  16,  16,   22, 24, 10, 39,   -1, 1, 8, UNSELECTED}, /*  23 */
{"|", 254,  46,  16,  16,   23, 25, 10, 40,   -1, 7, 8, UNSELECTED}, /*  24 */

{"Left",
      271,  46,  16,  16,   24, 26, -1, 41,   -1, 1, 9, UNSELECTED}, /*  25 */

{"Right",
      288,  46,  16,  16,   25, -1, -1, 42,   -1, 7, 9, UNSELECTED}, /*  26 */

/////////////////////////////////////////
/*
*Name, x,   y,    w,   h,   L,  R,  U,  D,    S,  R, C, State
 */
{"Tab",
       33,  62,   24,  16,   -1, 28, 11, 43,   -1, 6, 0, UNSELECTED}, /*  27 */

{"Q",  58,  62,   16,  16,   27, 29, 12, 45,   -1, 1, 0, UNSELECTED}, /*  28 */
{"W",  75,  62,   16,  16,   28, 30, 13, 46,   -1, 2, 1, UNSELECTED}, /*  29 */ 
{"E",  92,  62,   16,  16,   29, 31, 14, 47,   -1, 2, 2, UNSELECTED}, /*  30 */
{"R", 109,  62,   16,  16,   30, 32, 15, 48,   -1, 3, 3, UNSELECTED}, /*  31 */
{"T", 126,  62,   16,  16,   31, 33, 16, 49,   -1, 2, 3, UNSELECTED}, /*  32 */
{"Y", 143,  62,   16,  16,   32, 34, 17, 50,   -1, 4, 4, UNSELECTED}, /*  33 */
{"U", 160,  62,   16,  16,   33, 35, 18, 51,   -1, 3, 5, UNSELECTED}, /*  34 */
{"I", 177,  62,   16,  16,   34, 36, 19, 52,   -1, 2, 5, UNSELECTED}, /*  35 */
{"O", 194,  62,   16,  16,   35, 37, 20, 53,   -1, 3, 6, UNSELECTED}, /*  36 */
{"P", 211,  62,   16,  16,   36, 38, 21, 54,   -1, 3, 7, UNSELECTED}, /*  37 */
{"@", 228,  62,   16,  16,   37, 39, 22, 55,   -1, 4, 7, UNSELECTED}, /*  38 */
{"{", 245,  62,   16,  16,   38, 40, 23, 56,   -1, 3, 8, UNSELECTED}, /*  39 */
{"_", 262,  62,   16,  16,   39, 41, 24, 57,   -1, 2, 8, UNSELECTED}, /*  40 */

{"Up",
      279,  62,   16,  16,   40, 42, 25, 57,   -1, 3, 9, UNSELECTED}, /*  41 */

{"Down",
      296,  62,   16,  16,   41, -1, 26, -1,   -1, 2, 9, UNSELECTED}, /*  42 */

/////////////////////////////////////////
/*
*Name, x,   y,    w,   h,   L,  R,  U,  D,    S,  R, C, State
 */
{"Caps-Lock",
        28,  78,  16,  16,  -1, 44, 27, 58,    -1, 4, 0, UNSELECTED}, /*  43 */

{"Ctrl",
        45,  78,  16,  16,  43, 45, 27, 59,    -1, 0, 1, UNSELECTED}, /*  44 */

{"A",   62,  78,  16,  16,  44, 46, 28, 60,    -1, 4, 1, UNSELECTED}, /*  45 */
{"S",   79,  78,  16,  16,  45, 47, 29, 61,    -1, 5, 1, UNSELECTED}, /*  46 */
{"D",   96,  78,  16,  16,  46, 48, 30, 62,    -1, 3, 2, UNSELECTED}, /*  47 */
{"F",  113,  78,  16,  16,  47, 49, 31, 63,    -1, 4, 3, UNSELECTED}, /*  48 */
{"G",  130,  78,  16,  16,  48, 50, 32, 64,    -1, 5, 3, UNSELECTED}, /*  49 */
{"H",  147,  78,  16,  16,  49, 51, 33, 65,    -1, 5, 4, UNSELECTED}, /*  50 */
{"J",  164,  78,  16,  16,  50, 52, 34, 66,    -1, 4, 5, UNSELECTED}, /*  51 */
{"K",  181,  78,  16,  16,  51, 53, 35, 67,    -1, 4, 6, UNSELECTED}, /*  52 */
{"L",  198,  78,  16,  16,  52, 54, 36, 68,    -1, 5, 6, UNSELECTED}, /*  53 */
{"+",  215,  78,  16,  16,  53, 55, 37, 69,    -1, 5, 7, UNSELECTED}, /*  54 */
{"*",  232,  78,  16,  16,  54, 56, 38, 70,    -1, 4, 8, UNSELECTED}, /*  55 */
{"}",  249,  78,  16,  16,  55, 57, 39, 70,    -1, 5, 8, UNSELECTED}, /*  56 */
{"Return",
       266,  78,  33,  16,  56, -1, 40, 71,    -1, 4, 9, UNSELECTED}, /*  57 */

/////////////////////////////////////////
/*
*Name, x,   y,    w,   h,   L,  R,  U,  D,    S,  R, C, State
 */
{"Shift-Lock",
       28,  94,  16,  16,  -1, 59, 43, -1,    -1, 5, 0, UNSELECTED}, /*  58 */

{"Shift1",
       45,  94,  24,  16,  58, 60, 44, -1,    -1, 0, 0, UNSELECTED}, /*  59 */

{"Z",  70,  94,  16,  16,  59, 61, 45, -1,    -1, 6, 1, UNSELECTED}, /*  60 */
{"X",  87,  94,  16,  16,  60, 62, 46, 73,    -1, 4, 2, UNSELECTED}, /*  61 */
{"C", 104,  94,  16,  16,  61, 63, 47, 73,    -1, 5, 2, UNSELECTED}, /*  62 */
{"V", 121,  94,  16,  16,  62, 64, 48, 73,    -1, 6, 3, UNSELECTED}, /*  63 */
{"B", 138,  94,  16,  16,  63, 65, 49, 73,    -1, 6, 4, UNSELECTED}, /*  64 */
{"N", 155,  94,  16,  16,  64, 66, 50, 73,    -1, 5, 5, UNSELECTED}, /*  65 */
{"M", 172,  94,  16,  16,  65, 67, 51, 73,    -1, 6, 5, UNSELECTED}, /*  66 */
{"<", 189,  94,  16,  16,  66, 68, 52, 73,    -1, 6, 6, UNSELECTED}, /*  67 */
{">", 206,  94,  16,  16,  67, 69, 53, 73,    -1, 6, 7, UNSELECTED}, /*  68 */
{"?", 223,  94,  16,  16,  68, 70, 54, -1,    -1, 6, 8, UNSELECTED}, /*  69 */

{"Shift2",
      240,  94,  25,  16,  69, 71, 55, -1,    -1, 0, 0, UNSELECTED}, /*  70 */

{"Delete",
      266,  94,  16,  16,  70, 72, 57, -1,    -1, 5, 9, UNSELECTED}, /*  71 */

{"Copy",
      283,  94,  16,  16,  71, -1, 57, -1,    -1, 6, 9, UNSELECTED}, /*  72 */

/////////////////////////////////////////
/*
*Name, x,   y,    w,   h,   L,  R,  U,  D,    S,  R, C, State
 */
{"Space",
       87, 111, 135,   16, -1, -1, 65, -1,     -1, 6, 2, UNSELECTED}, /* 73 */

};

/* The focused button on the virtual keyboard.
 * (Use SetFocusTo to set this value)
 *
 * If a key is in the process of being pressed, then lock the keyboard.
 */
static unsigned int focused_key_index = 0;
static unsigned int keyboard_locked = 0;

static int shift_down = 0, control_down = 0;

/* The virtual keyboard graphics:
 */
static SDL_Surface *keyboard_p=NULL, *flipped_keyboard_p=NULL;
static SDL_Surface *keyboard_selected_p=NULL, *flipped_keyboard_selected_p=NULL;
static SDL_Surface *keyboard_pressed_p=NULL, *flipped_keyboard_pressed_p=NULL;
static SDL_Surface *keyboard_heldandselected_p=NULL, *flipped_keyboard_heldandselected_p=NULL;
static SDL_Surface *keyboard_held_p=NULL, *flipped_keyboard_held_p=NULL;

static SDL_Surface *tape_on_p=NULL, *tape_off_p=NULL, *flipped_tape_on_p=NULL, *flipped_tape_off_p=NULL;
static SDL_Surface *caps_on_p=NULL, *caps_off_p=NULL, *flipped_caps_on_p=NULL, *flipped_caps_off_p=NULL;
static SDL_Surface *shift_on_p=NULL, *shift_off_p=NULL, *flipped_shift_on_p=NULL, *flipped_shift_off_p=NULL;


/* External resources, the surface to render to is set via 
 *
 * VirtualKeyboard_Initialize (so in future this can change during execution).
 *
 * mainWin is hardwired here like it is everywhere else in BeebEm anyway.
 */
static SDL_Surface *surface_p=NULL;
extern BeebWin *mainWin;


inline static void PutPixel(SDL_Surface *surface_p, int x, int y, Uint8 pixel)
{
        SDL_Rect clipping;
        Uint8 *buf_p;

        if (surface_p == NULL) return;

        /* Clip to clipping area:
         */
        SDL_GetClipRect(surface_p, &clipping);
        if (x < clipping.x || x >= clipping.x + clipping.w
            || y < clipping.y || y >= clipping.y + clipping.h)
                return;

        /* Do nothing if surface isn't 8 bit:
         */
        if (surface_p->format->BytesPerPixel != 1) {
                fprintf(stderr, "SDL Surface is not 8 bit, can't draw.\n");
                return;
        }

        /* Draw the pixel:
         */
        buf_p = (Uint8*) surface_p->pixels + y * surface_p->pitch + x;
        *buf_p = pixel;
}

inline static Uint32 GetPixel(SDL_Surface *surface_p, int x, int y)
{
        if (surface_p == NULL) return 0;

        /* Clip to bitmap:
         */
        if (x<0 || x>=surface_p->w || y<0 || y>=surface_p->h)
                return 0;

        switch (surface_p->format->BytesPerPixel) {

        case 1:
                {
                        Uint8 *bufp;
                        bufp = (Uint8*) surface_p->pixels + y * surface_p->pitch + x;
                        return (unsigned int) *bufp;
                }
        case 2:
                {
                        Uint16 *bufp;
                        bufp = (Uint16*) surface_p->pixels + y * surface_p->pitch + x;
                        return (unsigned int) *bufp;
                }

        case 3:
                printf("EG_Graphics_GetPixel: 24bit is not supported...\n");
                return 0;

        case 4:
                {
                        Uint32 *bufp;
                        bufp = (Uint32*) surface_p->pixels + y * surface_p->pitch + x;
                        return (unsigned int) *bufp;
                }
        default:
                return 0;
        }
}



/* Create a new SDL_Surface from a source surface, then flip that
 * new surface upside down:
 *
 * (The GetPixel/PutPixel routines are in gui/sdl.cc)
 */
static SDL_Surface* CreateNewFlippedSurface(SDL_Surface *source_p)
{
        SDL_Surface *flipped_p;
        int x, y;
        Uint32 color;

        flipped_p = SDL_DisplayFormat(source_p);
        if (flipped_p == NULL) return NULL;

	LOCK(flipped_p);
	LOCK(source_p);

        for(y=0; y<source_p->h; y++)
                for(x=0; x<source_p->w; x++) {
                        color = GetPixel(source_p, source_p->w-x-1, source_p->h-y-1);
                        PutPixel(flipped_p, x, y, color);
                }

	UNLOCK(source_p);
	UNLOCK(flipped_p);

        return flipped_p;
}

/* Sets focus to a specific key, if the key is illegal then focus is set to the first key:
 */
static void SetFocusTo(unsigned int key_index)
{
	if (key_index > KEYCOUNT)
		focused_key_index = 0;
	else
		focused_key_index = key_index;
}

void VirtualKeyboard_RepaintTapeLED(int state)
{
	SDL_Surface *source_p;
	SDL_Rect d;

	/* If keyboard is not shown, do nothing.
	 */
	if ( ! mainWin->OnScreenKeyboardShown() ) return;	

	if ( ! mainWin->ScreenIsReversed() ) {
		source_p = ! state ? tape_off_p : tape_on_p;
		d.x = 30;
		d.y = 217;
	} else {
		source_p = ! state ? flipped_tape_off_p : flipped_tape_on_p;
		d.x = 273;
		d.y = 7;
	}
	d.w = d.h = 0;

	SDL_BlitSurface(source_p, NULL, surface_p, &d);
	SDL_UpdateRect(surface_p, d.x, d.y, surface_p->w, surface_p->h);
}

void VirtualKeyboard_RepaintCapsLED(int state)
{
	SDL_Surface *source_p;
	SDL_Rect d;

	/* If keyboard is not shown, do nothing.
	 */
	if ( ! mainWin->OnScreenKeyboardShown() ) return;	

	if ( ! mainWin->ScreenIsReversed() ) {
		source_p = ! state ? caps_off_p : caps_on_p;
		d.x = 30+18;
		d.y = 217;
	} else {
		source_p = ! state ? flipped_caps_off_p : flipped_caps_on_p;
		d.x = 273-18;
		d.y = 7;
	}
	d.w = d.h = 0;

	SDL_BlitSurface(source_p, NULL, surface_p, &d);
	SDL_UpdateRect(surface_p, d.x, d.y, surface_p->w, surface_p->h);
}

void VirtualKeyboard_RepaintShiftLED(int state)
{
	SDL_Surface *source_p;
	SDL_Rect d;

	/* If keyboard is not shown, do nothing.
	 */
	if ( ! mainWin->OnScreenKeyboardShown() ) return;	

	if ( ! mainWin->ScreenIsReversed() ) {
		source_p = ! state ? shift_off_p : shift_on_p;
		d.x = 30+36;
		d.y = 217;
	} else {
		source_p = ! state ? flipped_shift_off_p : flipped_shift_on_p;
		d.x = 273-36;
		d.y = 7;
	}
	d.w = d.h = 0;

	SDL_BlitSurface(source_p, NULL, surface_p, &d);
	SDL_UpdateRect(surface_p, d.x, d.y, surface_p->w, surface_p->h);
}



/* Render a key on the virtual keyboard by key index, the physical
 * state of the virtual keyboard is taken into consideration:
 */
static void RenderKey(unsigned int key_index)
{
	SDL_Surface *source_p;
	SDL_Rect s, d;

	if (key_index >= KEYCOUNT) return;

	/* If the virtual keyboard is not shown, then do nothing:
	 */
	if ( ! mainWin->OnScreenKeyboardShown() ) return;

	printf("********************* STATE = %d\n", keyboard[key_index].state);

	/* Source rectangle to blit from:
	 */
	s.w = keyboard[key_index].width;
	s.h = keyboard[key_index].height;

	/* Dest x, y to blit to:
	 */
	if ( ! mainWin->ScreenIsReversed() ) {
		s.x = keyboard[key_index].x;
		s.y = keyboard[key_index].y;

		d.x = s.x;
		d.y = surface_p->h - keyboard_p->h + s.y;

		switch (keyboard[key_index].state) {

			case SELECTED:
				source_p = keyboard_selected_p;
				break;

			case HELD:
				source_p = keyboard_held_p;
				break;

			case PRESSED:
				source_p = keyboard_pressed_p;
				break;

			case HELDANDSELECTED:
				source_p = keyboard_heldandselected_p;
				break;

			default:
				source_p = keyboard_p;
				break;
		}

	} else {
		s.x = flipped_keyboard_selected_p->w - keyboard[key_index].x - s.w;
		s.y = flipped_keyboard_selected_p->h - keyboard[key_index].y - s.h;

		d.x = s.x;
		d.y = 0 + s.y;

		switch (keyboard[key_index].state) {

			case SELECTED:
				source_p = flipped_keyboard_selected_p;
				break;

			case HELD:
				source_p = flipped_keyboard_held_p;
				break;

			case PRESSED:
				source_p = flipped_keyboard_pressed_p;
				break;

			case HELDANDSELECTED:
				source_p = flipped_keyboard_heldandselected_p;
				break;

			default:
				source_p = flipped_keyboard_p;
				break;
		}

	}

	SDL_BlitSurface(source_p, &s, surface_p, &d);
	SDL_UpdateRect(surface_p, d.x, d.y, s.w, s.h);
}

/* Will change the state of a key to selected:
 */
static void SelectKey(unsigned int key_index)
{
	if (keyboard[key_index].state == HELD )
		keyboard[key_index].state = HELDANDSELECTED;
	else
		keyboard[key_index].state = SELECTED;
}

/* Will change the state of a key to unselected:
 */
static void UnselectKey(unsigned int key_index)
{
	if (keyboard[key_index].state == HELDANDSELECTED )
		keyboard[key_index].state = HELD;
	else
		keyboard[key_index].state = UNSELECTED;
}

/* Press key:
 */
static void PressKey(unsigned int key_index)
{
	keyboard[key_index].state = PRESSED;
}

static void HoldKey(unsigned int key_index)
{
	keyboard[key_index].state = HELD;
}

static void HoldAndSelectKey(unsigned int key_index)
{
	keyboard[key_index].state = HELDANDSELECTED;
}

static void UnholdKey(unsigned int key_index)
{
	keyboard[key_index].state = UNSELECTED;
}


static SDL_Surface* CreateNewVirtualKeyboardSurface(SDL_Surface *surface_p, unsigned int index)
{
	SDL_Surface *new_surface_p = NULL, *new_surface2_p = NULL;
	SDL_Rect s;

	/* Create an 8 bit surface at virtual keyboard size. 1 is subtracted from the height
	 * to remove the palette colors line:
	 */
        if ( (new_surface_p=SDL_CreateRGBSurface(
	   SDL_HWSURFACE, KEYBOARD_WIDTH, KEYBOARD_HEIGHT-1, 8, 0
	   , 0, 0, 0) ) == NULL) {
                fprintf(stderr, "Unable to create SDL Surface for XPM data"
                 ": %s\n", SDL_GetError());
                return 0;
        }

	/* Convert blank surface to display format (which will set our palette to the frame buffers):
	 */
	new_surface2_p = SDL_DisplayFormat(new_surface_p);
	SDL_FreeSurface(new_surface_p);

	if (new_surface2_p == NULL) return NULL;

	/* Copy the keyboard image we want over (we remove the palette line):
	 */
	s.x = 0; s.y = (KEYBOARD_HEIGHT * index)+1; s.w = KEYBOARD_WIDTH; s.h = KEYBOARD_HEIGHT - 1;
	SDL_BlitSurface(surface_p, &s, new_surface2_p, NULL);

	return new_surface2_p;
}

static SDL_Surface* CreateNewLEDSurface(SDL_Surface *surface_p, unsigned int x, unsigned int y, unsigned int w, unsigned int h)
{
	SDL_Surface *new_surface_p = NULL, *new_surface2_p = NULL;
	SDL_Rect s;

	/* Create an 8 bit surface at virtual keyboard size. 1 is subtracted from the height
	 * to remove the palette colors line:
	 */
        if ( (new_surface_p=SDL_CreateRGBSurface(
	   SDL_HWSURFACE, w, h, 8, 0, 0, 0, 0) ) == NULL) {
                fprintf(stderr, "Unable to create SDL Surface for XPM data"
                 ": %s\n", SDL_GetError());
                return 0;
        }

	/* Convert blank surface to display format (which will set our palette to the frame buffers):
	 */
	new_surface2_p = SDL_DisplayFormat(new_surface_p);
	SDL_FreeSurface(new_surface_p);

	if (new_surface2_p == NULL) return NULL;

	/* Copy the keyboard image we want over (we remove the palette line):
	 */
//	s.x = 0; s.y = (KEYBOARD_HEIGHT * index)+1; s.w = KEYBOARD_WIDTH; s.h = KEYBOARD_HEIGHT - 1;
	s.x = x; s.y = y; s.w = w; s.h = h;

	SDL_BlitSurface(surface_p, &s, new_surface2_p, NULL);

	return new_surface2_p;
}




static int LoadKeyboardImageAndSetPalette(SDL_Surface *surface_p)
{
	SDL_Surface *tmp_p = NULL, *tmp2_p = NULL;

	/* Load the virtual keyboard graphics file:
	 */
	printf("Loading keyboard image [%s].\n", config.files.virtual_keyboard_loc);

//	for(int i=0; i<1024*5; i++)
//		printf("[%2X:%c]", config.files.xx[i], config.files.xx[i]>=32 ? config.files.xx[i] : 32);
//	exit(1);

//	tmp2_p = SDL_LoadBMP("/mnt/sd/beebem2/resources/newvirtualkeyboard.bmp");
	tmp2_p = SDL_LoadBMP(config.files.virtual_keyboard_loc);
	if (tmp2_p == NULL) return 0;

	/* Copy the virtual keyboard's palette to the frame buffer, if the source
	 * bmp image is a 24 bit one rather then an 8bit one (no palette), then
	 * quit:
	 */
	printf("-> Setting palette.\n");
	if (tmp2_p->format->palette != NULL) {
		SDL_SetPalette(surface_p, SDL_LOGPAL|SDL_PHYSPAL, tmp2_p->format->palette->colors, 0, tmp2_p->format->palette->ncolors);
	} else {
		printf("Virtual keyboard graphics are not 8bit."
		 " Please resave as an 8bit bmp.\n");
		SDL_FreeSurface(tmp2_p);
		return 0;
	}
	printf("<-\n\n");

	/* Make a new copy of the virtual keyboard in screen format and dump the original:
	 */
	printf("-> Converting keyboard image to display format.\n");
	tmp_p = SDL_DisplayFormat(tmp2_p);
	SDL_FreeSurface(tmp2_p);
	printf("<-\n\n");


	/* Create keyboard_p and flipped_keyboard_p:
	 */
	printf("-> Generating unselected.\n");
	keyboard_p = CreateNewVirtualKeyboardSurface(tmp_p, UNSELECTED);
	if (keyboard_p == NULL) {
		printf("Could not create keyboard surface.\n");
		SDL_FreeSurface(tmp_p);
		VirtualKeyboard_FreeResources();
		return 0;
	}
	flipped_keyboard_p = CreateNewFlippedSurface(keyboard_p);
	if (flipped_keyboard_p == NULL) {
		printf("Could not create flipped keyboard surface.\n");
		SDL_FreeSurface(tmp_p);
		VirtualKeyboard_FreeResources();
		return 0;
	}
	printf("<-\n\n");

	/* Create keyboard_selected_p and flipped_keyboard_selected_p:
	 */
	printf("-> Generating selected.\n");
	keyboard_selected_p = CreateNewVirtualKeyboardSurface(tmp_p, SELECTED);
	if (keyboard_selected_p == NULL) {
		printf("Could not create 'keyboard selected' surface.\n");
		SDL_FreeSurface(tmp_p);
		VirtualKeyboard_FreeResources();
		return 0;
	}
	flipped_keyboard_selected_p = CreateNewFlippedSurface(keyboard_selected_p);
	if (flipped_keyboard_selected_p == NULL) {
		printf("Could not create flipped 'keyboard selected' surface.\n");
		SDL_FreeSurface(tmp_p);
		VirtualKeyboard_FreeResources();
		return 0;
	}
	printf("<-\n\n");

	/* Create keyboard_pressed_p and flipped_keyboard_pressed_p:
	 */
	printf("-> Generating pressed.\n");
	keyboard_pressed_p = CreateNewVirtualKeyboardSurface(tmp_p, PRESSED);
	if (keyboard_pressed_p == NULL) {
		printf("Could not create 'keyboard pressed' surface.\n");
		SDL_FreeSurface(tmp_p);
		VirtualKeyboard_FreeResources();
		return 0;
	}
	flipped_keyboard_pressed_p = CreateNewFlippedSurface(keyboard_pressed_p);
	if (flipped_keyboard_pressed_p == NULL) {
		printf("Could not create flipped 'keyboard pressed' surface.\n");
		SDL_FreeSurface(tmp_p);
		VirtualKeyboard_FreeResources();
		return 0;
	}
	printf("<-\n\n");


	/* keyboard_heldandselected_p and flipped_keyboard_heldandselected_p:
	 */
	printf("-> Generating held and selected.\n");
	keyboard_heldandselected_p = CreateNewVirtualKeyboardSurface(tmp_p, HELDANDSELECTED);
	if (keyboard_heldandselected_p == NULL) {
		printf("Could not create 'keyboard held and selected' surface.\n");
		SDL_FreeSurface(tmp_p);
		VirtualKeyboard_FreeResources();
		return 0;
	}
	flipped_keyboard_heldandselected_p = CreateNewFlippedSurface(keyboard_heldandselected_p);
	if (flipped_keyboard_heldandselected_p == NULL) {
		printf("Could not create flipped 'keyboard held and selected' surface.\n");
		SDL_FreeSurface(tmp_p);
		VirtualKeyboard_FreeResources();
		return 0;
	}
	printf("<-\n\n");


	/* keyboard_held_p and flipped_keyboard_held_p:
	 */
	printf("-> Generating held.\n");
	keyboard_held_p = CreateNewVirtualKeyboardSurface(tmp_p, HELD);
	if (keyboard_held_p == NULL) {
		printf("Could not create 'keyboard held' surface.\n");
		SDL_FreeSurface(tmp_p);
		VirtualKeyboard_FreeResources();
		return 0;
	}
	flipped_keyboard_held_p = CreateNewFlippedSurface(keyboard_held_p);
	if (flipped_keyboard_held_p == NULL) {
		printf("Could not create flipped 'keyboard held' surface.\n");
		SDL_FreeSurface(tmp_p);
		VirtualKeyboard_FreeResources();
		return 0;
	}
	printf("<-\n\n");


	/* LEDs:
	 */
	printf("-> Generating LEDs.\n");
	tape_on_p = CreateNewLEDSurface(tmp_p, 30, 817, 17, 16);
	if (tape_on_p == NULL) {
		printf("Could not create 'tape on LED' surface.\n");
		SDL_FreeSurface(tmp_p);
		VirtualKeyboard_FreeResources();
		return 0;
	}
	flipped_tape_on_p = CreateNewFlippedSurface(tape_on_p);
	if (flipped_tape_on_p == NULL) {
		printf("Could not create flipped 'tape on LED' surface.\n");
		SDL_FreeSurface(tmp_p);
		VirtualKeyboard_FreeResources();
		return 0;
	}

	tape_off_p = CreateNewLEDSurface(tmp_p, 30, 793, 17, 16);
	if (tape_off_p == NULL) {
		printf("Could not create 'tape off LED' surface.\n");
		SDL_FreeSurface(tmp_p);
		VirtualKeyboard_FreeResources();
		return 0;
	}
	flipped_tape_off_p = CreateNewFlippedSurface(tape_off_p);
	if (flipped_tape_off_p == NULL) {
		printf("Could not create flipped 'tape off LED' surface.\n");
		SDL_FreeSurface(tmp_p);
		VirtualKeyboard_FreeResources();
		return 0;
	}

	caps_on_p = CreateNewLEDSurface(tmp_p, 48, 817, 17, 16);
	if (caps_on_p == NULL) {
		printf("Could not create 'caps on LED' surface.\n");
		SDL_FreeSurface(tmp_p);
		VirtualKeyboard_FreeResources();
		return 0;
	}
	flipped_caps_on_p = CreateNewFlippedSurface(caps_on_p);
	if (flipped_caps_on_p == NULL) {
		printf("Could not create flipped 'caps on LED' surface.\n");
		SDL_FreeSurface(tmp_p);
		VirtualKeyboard_FreeResources();
		return 0;
	}

	caps_off_p = CreateNewLEDSurface(tmp_p, 48, 793, 17, 16);
	if (caps_off_p == NULL) {
		printf("Could not create 'caps off LED' surface.\n");
		SDL_FreeSurface(tmp_p);
		VirtualKeyboard_FreeResources();
		return 0;
	}
	flipped_caps_off_p = CreateNewFlippedSurface(caps_off_p);
	if (flipped_caps_off_p == NULL) {
		printf("Could not create flipped 'caps off LED' surface.\n");
		SDL_FreeSurface(tmp_p);
		VirtualKeyboard_FreeResources();
		return 0;
	}

	shift_on_p = CreateNewLEDSurface(tmp_p, 66, 817, 17, 16);
	if (shift_on_p == NULL) {
		printf("Could not create 'shift on LED' surface.\n");
		SDL_FreeSurface(tmp_p);
		VirtualKeyboard_FreeResources();
		return 0;
	}
	flipped_shift_on_p = CreateNewFlippedSurface(shift_on_p);
	if (flipped_shift_on_p == NULL) {
		printf("Could not create flipped 'shift on LED' surface.\n");
		SDL_FreeSurface(tmp_p);
		VirtualKeyboard_FreeResources();
		return 0;
	}

	shift_off_p = CreateNewLEDSurface(tmp_p, 66, 793, 17, 16);
	if (shift_off_p == NULL) {
		printf("Could not create 'shift off LED' surface.\n");
		SDL_FreeSurface(tmp_p);
		VirtualKeyboard_FreeResources();
		return 0;
	}
	flipped_shift_off_p = CreateNewFlippedSurface(shift_off_p);
	if (flipped_shift_off_p == NULL) {
		printf("Could not create flipped 'shift off LED' surface.\n");
		SDL_FreeSurface(tmp_p);
		VirtualKeyboard_FreeResources();
		return 0;
	}
	printf("<-\n\n");

	/* I clear the surfaces first otherwise they look fucking awful when you quit SDL on the GP2X...
  	 */
	SDL_FillRect(tmp_p, NULL, 0); SDL_UpdateRect(tmp_p,0,0,0,0);
	SDL_FreeSurface(tmp_p);
	return 1;
}


/* Convert the XPM data into SDL Surfaces + copy the new images palette to the frame buffer.
 * On failure nothing is created, although the frame buffer palette may change.
 */
int VirtualKeyboard_Initialize(SDL_Surface *frame_buffer_p)
{
	/* Remember rendering surface and beebem instance:
	 */
	surface_p = frame_buffer_p;

	/* Keyboard is not locked:
	 */
	keyboard_locked = 0;

	/* The initial focused button on the virtual keyboard
	 * (Should be a menu or otherwise the space bar):
	 */
	SetFocusTo(73);

	/* Make sure focused key is rendered selected:
	 */
	keyboard[focused_key_index].state = SELECTED;

	/* Load the graphics and set the frame buffers palette:
	 */
	if (! LoadKeyboardImageAndSetPalette(surface_p))
		return 0;

        /* Success:
         */
        return 1;
}

/* Renders the whole keyboard:
 */
void VirtualKeyboard_Render()
{
        SDL_Rect d = {0,0,0,0};

	int i;

	/* Render the whole virtual keyboard:
	 */
        if (! mainWin->ScreenIsReversed() ) {
                d.y = surface_p->h - keyboard_p->h;
                SDL_BlitSurface(keyboard_p, NULL, surface_p, &d);
                SDL_UpdateRect(surface_p, 0, d.y, surface_p->w, keyboard_p->h);
	} else {
                SDL_BlitSurface(flipped_keyboard_p, NULL, surface_p, &d);
                SDL_UpdateRect(surface_p, 0, 0, surface_p->w, flipped_keyboard_p->h);
        }

	/* Re-draw all keys that are not UNSELECTED:
	 */
	for (i = 0; i < KEYCOUNT; i++)
		if (keyboard[i].state != UNSELECTED) RenderKey(i);

	/* Paint the LEDs:
	 */
	//mainWin->RepaintLEDs();
	VirtualKeyboard_RepaintTapeLED(mainWin->GetTapeLED());
	VirtualKeyboard_RepaintCapsLED(mainWin->GetCapsLED());
	VirtualKeyboard_RepaintShiftLED(mainWin->GetShiftLED());
}

void VirtualKeyboard_UpdateLEDs()
{
	static int last_frame = 0, previous_tape = 0, previous_caps = 0, previous_shift = 0;

	// Don't bother to update more than once per frame.
	if (mainWin->currentframe() == last_frame) 
		return;
	last_frame = mainWin->currentframe();

	// Update the leds.
	if (mainWin->GetTapeLED() != previous_tape) {
		VirtualKeyboard_RepaintTapeLED(mainWin->GetTapeLED());
		previous_tape = mainWin->GetTapeLED();
	}

	if (mainWin->GetShiftLED() != previous_shift) {
		VirtualKeyboard_RepaintShiftLED(mainWin->GetShiftLED());
		previous_shift = mainWin->GetShiftLED();
	}

	if (mainWin->GetCapsLED() != previous_caps) {
		VirtualKeyboard_RepaintCapsLED(mainWin->GetCapsLED());
		previous_caps = mainWin->GetCapsLED();
	}

}

/* Frees all resources associated with the keyboard:
 */
void VirtualKeyboard_FreeResources()
{

	if (keyboard_p != NULL) { SDL_FillRect(keyboard_p, NULL, 0); SDL_UpdateRect(keyboard_p,0,0,0,0); SDL_FreeSurface(keyboard_p); }
	if (flipped_keyboard_p != NULL) { SDL_FillRect(flipped_keyboard_p, NULL, 0); SDL_UpdateRect(flipped_keyboard_p,0,0,0,0); SDL_FreeSurface(flipped_keyboard_p); }

	if (keyboard_selected_p != NULL) { SDL_FillRect(keyboard_selected_p, NULL, 0); SDL_UpdateRect(keyboard_selected_p,0,0,0,0); SDL_FreeSurface(keyboard_selected_p); }
	if (flipped_keyboard_selected_p != NULL) { SDL_FillRect(flipped_keyboard_selected_p, NULL, 0); SDL_UpdateRect(flipped_keyboard_selected_p,0,0,0,0); SDL_FreeSurface(flipped_keyboard_selected_p); }

	if (keyboard_pressed_p != NULL) {SDL_FillRect(keyboard_pressed_p, NULL, 0); SDL_UpdateRect(keyboard_pressed_p,0,0,0,0); SDL_FreeSurface(keyboard_pressed_p); }
	if (flipped_keyboard_pressed_p != NULL) {SDL_FillRect(flipped_keyboard_pressed_p, NULL, 0); SDL_UpdateRect(flipped_keyboard_pressed_p,0,0,0,0); SDL_FreeSurface(flipped_keyboard_pressed_p); }

	if (keyboard_held_p != NULL) {SDL_FillRect(keyboard_held_p, NULL, 0); SDL_UpdateRect(keyboard_held_p,0,0,0,0); SDL_FreeSurface(keyboard_held_p); }
	if (flipped_keyboard_held_p != NULL) {SDL_FillRect(flipped_keyboard_held_p, NULL, 0); SDL_UpdateRect(flipped_keyboard_held_p,0,0,0,0); SDL_FreeSurface(flipped_keyboard_held_p); }

	if (tape_on_p != NULL) {SDL_FillRect(tape_on_p, NULL, 0); SDL_FreeSurface(tape_on_p); }
	if (flipped_tape_on_p != NULL) {SDL_FillRect(flipped_tape_on_p, NULL, 0); SDL_FreeSurface(flipped_tape_on_p); }

	if (tape_off_p != NULL) {SDL_FillRect(tape_off_p, NULL, 0); SDL_FreeSurface(tape_off_p); }
	if (flipped_tape_off_p != NULL) {SDL_FillRect(flipped_tape_off_p, NULL, 0); SDL_FreeSurface(flipped_tape_off_p); }

	if (caps_on_p != NULL) {SDL_FillRect(caps_on_p, NULL, 0); SDL_FreeSurface(caps_on_p); }
	if (flipped_caps_on_p != NULL) {SDL_FillRect(flipped_caps_on_p, NULL, 0); SDL_FreeSurface(flipped_caps_on_p); }

	if (caps_off_p != NULL) {SDL_FillRect(caps_off_p, NULL, 0); SDL_FreeSurface(caps_off_p); }
	if (flipped_caps_off_p != NULL) {SDL_FillRect(flipped_caps_off_p, NULL, 0); SDL_FreeSurface(flipped_caps_off_p); }

	if (shift_on_p != NULL) {SDL_FillRect(shift_on_p, NULL, 0); SDL_FreeSurface(shift_on_p); }
	if (flipped_shift_on_p != NULL) {SDL_FillRect(flipped_shift_on_p, NULL, 0); SDL_FreeSurface(flipped_shift_on_p); }

	if (shift_off_p != NULL) {SDL_FillRect(shift_off_p, NULL, 0); SDL_FreeSurface(shift_off_p); }
	if (flipped_shift_off_p != NULL) {SDL_FillRect(flipped_shift_off_p, NULL, 0); SDL_FreeSurface(flipped_shift_off_p); }

}


/* Movement of focus within the virtual keyboard:
 * ----------------------------------------------
 */

/* These macros make accessing a neighboring key index a bit safer:
 */
#define NEIGHBOR_LEFT(i) 	( i >= KEYCOUNT || i < 0 ? 0 : keyboard[ i ].left )
#define NEIGHBOR_RIGHT(i) 	( i >= KEYCOUNT || i < 0 ? 0 : keyboard[ i ].right )
#define NEIGHBOR_UP(i) 		( i >= KEYCOUNT || i < 0 ? 0 : keyboard[ i ].up )
#define NEIGHBOR_DOWN(i) 	( i >= KEYCOUNT || i < 0 ? 0 : keyboard[ i ].down )

/* These macros make translating the focused key to the beeb a bit safer:
 */
#define GET_BEEB_ROW(i)		( i >= KEYCOUNT || i < 0 ? -1 : keyboard[ i ].row )
#define GET_BEEB_COLUMN(i)	( i >= KEYCOUNT || i < 0 ? -1 : keyboard[ i ].column )


/* Lock the keyboard:
 *
 * When locked, the virtual keyboard is inactive.
 */
void VirtualKeyboard_Lock()
{
	keyboard_locked = 1;
}

/* When unlocked the virtual keyboard works.
 */
void VirtualKeyboard_Unlock()
{
	keyboard_locked = 0;
}

int VirtualKeyboard_IsLocked()
{
	if (keyboard_locked) return 1;
	else return 0;
}

/* Move focus left:
 */
void VirtualKeyboard_MoveLeft()
{
	unsigned int old_focused_key_index = focused_key_index;

	/* If the keyboard is locked, disalllow movement:
	 */
	if (keyboard_locked) return;

	/* If user can move focus left, then move focus to the
	 * focused key's left neighbor, otherwise do nothing:
	 */
	if ( keyboard[focused_key_index].left != -1 ) {

		/* Select the focused key's left neighbor:
		 */
		SelectKey( NEIGHBOR_LEFT(focused_key_index) );

		/* Unselect the focused key and re-render it:
		 */
		UnselectKey(focused_key_index);
		RenderKey(focused_key_index);

		/* Move focus to the left neighbor and re-render the now
		 * focused neighbor:
	 	 */
		SetFocusTo( NEIGHBOR_LEFT(focused_key_index) );
		RenderKey(focused_key_index);
		keyboard[focused_key_index].right = old_focused_key_index;

		/* The above may look contorted, but we don't want two
		 * keys to be visibly selected at the same time on-screen.
	 	 */
	}
}

void VirtualKeyboard_MoveRight()
{
	unsigned int old_focused_key_index = focused_key_index;

	/* If the keyboard is locked, disalllow movement:
	 */
	if (keyboard_locked) return;

	/* If user can move focus right, then move focus to the
	 * focused key's right neighbor, otherwise do nothing:
	 */
	if ( keyboard[focused_key_index].right != -1 ) {

		/* Select the focused key's right neighbor:
		 */
		SelectKey( NEIGHBOR_RIGHT(focused_key_index) );

		/* Unselect the focused key and re-render it:
		 */
		UnselectKey(focused_key_index);
		RenderKey(focused_key_index);

		/* Move focus to the right neighbor and re-render the now
		 * focused neighbor:
	 	 */
		SetFocusTo( NEIGHBOR_RIGHT(focused_key_index) );
		RenderKey(focused_key_index);
		keyboard[focused_key_index].left = old_focused_key_index;

		/* The above may look contorted, but we don't want two
		 * keys to be visibly selected at the same time on-screen.
	 	 */
	}
}

void VirtualKeyboard_MoveUp()
{
	unsigned int old_focused_key_index = focused_key_index;

	/* If the keyboard is locked, disalllow movement:
	 */
	if (keyboard_locked) return;

	/* If user can move focus up, then move focus to the
	 * focused key's upwards neighbor, otherwise do nothing:
	 */
	if ( keyboard[focused_key_index].up != -1 ) {

		/* Select the focused key's up neighbor:
		 */
		SelectKey( NEIGHBOR_UP(focused_key_index) );

		/* Unselect the focused key and re-render it:
		 */
		UnselectKey(focused_key_index);
		RenderKey(focused_key_index);

		/* Move focus to the upwards neighbor and re-render the now
		 * focused neighbor:
	 	 */
		SetFocusTo( NEIGHBOR_UP(focused_key_index) );
		RenderKey(focused_key_index);
		keyboard[focused_key_index].down = old_focused_key_index;

		/* The above may look contorted, but we don't want two
		 * keys to be visibly selected at the same time on-screen.
	 	 */
	}
}

void VirtualKeyboard_MoveDown()
{
	unsigned int old_focused_key_index = focused_key_index;

	/* If the keyboard is locked, disalllow movement:
	 */
	if (keyboard_locked) return;

	/* If user can move focus down, then move focus to the
	 * focused key's downwards neighbor, otherwise do nothing:
	 */
	if ( keyboard[focused_key_index].down != -1 ) {

		/* Select the focused key's left neighbor:
		 */
		SelectKey( NEIGHBOR_DOWN(focused_key_index) );

		/* Unselect the focused key and re-render it:
		 */
		UnselectKey(focused_key_index);
		RenderKey(focused_key_index);

		/* Move focus to the downwards neighbor and re-render the now
		 * focused neighbor:
	 	 */
		SetFocusTo( NEIGHBOR_DOWN(focused_key_index) );
		RenderKey(focused_key_index);
		keyboard[focused_key_index].up = old_focused_key_index;

		/* The above may look contorted, but we don't want two
		 * keys to be visibly selected at the same time on-screen.
	 	 */
	}
}

void VirtualKeyboard_KeyDown()
{
	int beeb_key_column, beeb_key_row;

	/* If the keyboard is locked, then disallow until current key that is down is released.:
	 */
	if (keyboard_locked) return;

	/* Handle shift booting:
	 */
	if (mainWin->ShiftBooted) {
		mainWin->ShiftBooted = false;
		BeebKeyUp(0, 0);
	}

	/* Handle control:
	 */

	/* If selected key is the control key:
	 */
	if (focused_key_index == 44) {

		/* If control is already held down and it's pressed again, then handle as a normal key:a
		 * If control was not held down, then hold it down:
		 */
		if (control_down == 1) {
			/* Control already down and pressed again, so handle as a normal key.
			 */
			PressKey(focused_key_index);
			RenderKey(focused_key_index);

			/* Press the key on the BBC:
			 */
			beeb_key_column = GET_BEEB_COLUMN(focused_key_index);
			beeb_key_row = GET_BEEB_ROW(focused_key_index);
			BeebKeyDown(beeb_key_row, beeb_key_column);
	
			/* clear control_down:
			 */
			control_down = 0;
	
			/* Lock the keyboard, this forces VirtualKeyboard_KeyUp to release the key:
			 */
			VirtualKeyboard_Lock();
		} else {
			HoldAndSelectKey(focused_key_index);
			RenderKey(focused_key_index);
	
			control_down = 1;
	
			/* Press the key on the BBC:
			 */
			beeb_key_column = GET_BEEB_COLUMN(focused_key_index);
			beeb_key_row = GET_BEEB_ROW(focused_key_index);
			BeebKeyDown(beeb_key_row, beeb_key_column);
	
			/* We don't lock the keyboard as we don't want VirtualKeyboard_KeyUp to release this key
			 */
		}

		return;
	}


	/* Handle shift keys:
	 */
	if (focused_key_index == 59 || focused_key_index == 70) {

		/* If shift is already held down and it's pressed again, then handle as a normal key
		 * If shift was not held down, then hold it down:
		 */
		if (shift_down == 1) {
			/* Shift already down and pressed again, so handle as a normal key.
			 */
			UnholdKey(59); UnholdKey(70);

			PressKey(focused_key_index);
			RenderKey(59); RenderKey(70);
			RenderKey(focused_key_index);

			/* Press the key on the BBC:
			 */
			beeb_key_column = GET_BEEB_COLUMN(focused_key_index);
			beeb_key_row = GET_BEEB_ROW(focused_key_index);
			BeebKeyDown(beeb_key_row, beeb_key_column);
	
			/* clear shift_down:
			 */
			shift_down = 0;
	
			/* Lock the keyboard, this forces VirtualKeyboard_KeyUp to release the key:
			 */
			VirtualKeyboard_Lock();
		} else {
			HoldKey(59); HoldKey(70);

			HoldAndSelectKey(focused_key_index);
			RenderKey(59); RenderKey(70);
			RenderKey(focused_key_index);	

			shift_down = 1;
	
			/* Press the key on the BBC:
			 */
			beeb_key_column = GET_BEEB_COLUMN(focused_key_index);
			beeb_key_row = GET_BEEB_ROW(focused_key_index);
			BeebKeyDown(beeb_key_row, beeb_key_column);
	
			/* We don't lock the keyboard as we don't want VirtualKeyboard_KeyUp to release this key
			 */
		}

		return;
	}



	/* Set the key to pressed and re-render it:
	 */
	PressKey(focused_key_index);
	RenderKey(focused_key_index);

	/* Press the key on the BBC:
 	 */
	beeb_key_column = GET_BEEB_COLUMN(focused_key_index);
	beeb_key_row = GET_BEEB_ROW(focused_key_index);

	if ( ! (beeb_key_column == -1 && beeb_key_row == -1) ) {
		BeebKeyDown(beeb_key_row, beeb_key_column);
	}

	if (beeb_key_row == -2) {
		/* Handle reset */
		printf("RESET\n");
		Init6502core();
		Disc8271_reset();
	}

	/* Lock the keyboard until VirtualKeyboard_KeyUp is called:
 	 */
	VirtualKeyboard_Lock();
}

void VirtualKeyboard_KeyUp()
{
	int beeb_key_column, beeb_key_row;

	/* If the keyboard is not locked (i.e.: a key is down), then don't release a key:
	 */
	if ( ! keyboard_locked ) return;

	/* Release all special keys:
	 */
	if (control_down == 1) {
			/* Unhold control key on virtual keyboard:
			 */
                        UnselectKey(44);
                        RenderKey(44);
			control_down = 0;

			/* Release control key on BBC:
			 */
			beeb_key_column = GET_BEEB_COLUMN(44);
			beeb_key_row = GET_BEEB_ROW(44);
			BeebKeyUp(beeb_key_row, beeb_key_column);
	}

	if (shift_down == 1) {
		/* Unhold all shift keys:
		 */
		UnholdKey(59); UnholdKey(70);
		RenderKey(59); RenderKey(70);
		shift_down = 0;

		/* Release shift key on BBC:
		 */
		beeb_key_column = GET_BEEB_COLUMN(59);
		beeb_key_row = GET_BEEB_ROW(59);
		BeebKeyUp(beeb_key_row, beeb_key_column);
	}


	/* Set the key to selected and re-render it:
	 */
	SelectKey(focused_key_index);
	RenderKey(focused_key_index);

	/* Get key:
	 */
	beeb_key_column = GET_BEEB_COLUMN(focused_key_index);
	beeb_key_row = GET_BEEB_ROW(focused_key_index);

	BeebKeyUp(beeb_key_row, beeb_key_column);

	/* Unlock the keyboard:
	 */
	VirtualKeyboard_Unlock();
}
