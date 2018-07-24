#ifndef _SKIN_HEADER
#define _SKIN_HEADER

#if HAVE_CONFIG_H
#       include <config.h>
#endif

#include <sdl.h>
#include <types.h>

#include <SDL/SDL.h>

#define EG_SKIN_BUTTON_MAXCOUNT 20

/* Box structure:
 */

typedef struct {
	SDL_Surface *top_p, *bottom_p, *left_p, *right_p;
	SDL_Surface *top_flipped_p, *bottom_flipped_p, *left_flipped_p, *right_flipped_p;

	SDL_Surface *frame_buffer_p;
	SDL_Color fill_color;
} EG_Skin_Box;

typedef struct {
	SDL_Surface *buttons_p, *buttons_flipped_p;

	unsigned int button_count;

	SDL_Surface *frame_buffer_p;
} EG_Skin_Button;

#ifdef __cplusplus
	extern "C" {
#endif

void 		SkinTest(SDL_Surface*);

EG_Skin_Box* 	EG_Skin_Box_CreateNew(const char *top_p, const char *bottom_p, const char *left_p, const char *right_p);
void 		EG_Skin_Box_Free(EG_Skin_Box *box_p);
EG_BOOL		EG_Skin_Box_Paint(EG_Skin_Box *box_p, SDL_Surface *s_p, int x, int y, unsigned int w, unsigned int h);

EG_BOOL		EG_Skin_Box_SetFillColor(EG_Skin_Box *box_p, unsigned char red, unsigned char green, unsigned char blue);
SDL_Color 	EG_Skin_Box_GetFillColor(EG_Skin_Box *box_p);

Uint8		EG_Skin_Box_GetColorR(EG_Skin_Box *box_p);
Uint8 		EG_Skin_Box_GetColorG(EG_Skin_Box *box_p);
Uint8 		EG_Skin_Box_GetColorB(EG_Skin_Box *box_p);

EG_Skin_Button* EG_Skin_Button_CreateNew(const char *buttons_file_p, unsigned int button_count);
void 		EG_Skin_Button_Free(EG_Skin_Button *button_p);
int 		EG_Skin_Button_Paint(EG_Skin_Button *button_p, SDL_Surface *s_p, unsigned int index, int x, int y, unsigned int w, unsigned int h);

void		EG_Skin_Button_SetColorKey(EG_Skin_Button *button_p, unsigned char r, unsigned char g, unsigned char b);

#ifdef __cplusplus
	}
#endif



#endif
