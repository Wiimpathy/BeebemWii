#ifndef _EG_FONT_PRIVATE_H_
#define _EG_FONT_PRIVATE_H_

#include <SDL/SDL.h>

#include "graphics_font.h"
#include "graphics_fontmap.h"


/* Font styles:
 */
#define EG_FONT_NORMAL                  0

#define EG_FONT_UNDERLINE               1
#define EG_FONT_NORMAL_UNDERLINE        1

#define EG_FONT_BOLD                    2
#define EG_FONT_BOLD_UNDERLINE          3

#define EG_FONT_ITALIC                  4
#define EG_FONT_ITALIC_UNDERLINE        5
#define EG_FONT_BOLD_ITALIC             6
#define EG_FONT_BOLD_ITALIC_UNDERLINE   7


/* Null Font:
 */
#define EG_Font_NULL            0x00000000

/* The font structs (classes):
 */

typedef struct {
        unsigned int  width;
} EG_FontChar;

typedef struct {
        unsigned int	hash;
        char		*name_p;
        unsigned int	width;
        unsigned int	pitch;
        unsigned int	height;

        EG_FontChar     ch[256];

        /* Bitmap font data, points to an array of type:
         * [nchars][height][ ((width-1>>3)&~0+3)+4 ]
         */
        Uint32          *data_p;

} EG_Font;



/* 
 */

#ifdef __cplusplus
extern "C" {
#endif


EG_Font* 	EG_Font_New(void);
void    	EG_Font_Free(EG_Font *f_p);

unsigned int	EG_Font_GetHash(EG_Font *f_p);
char*           EG_Font_GetName(EG_Font *f_p);
unsigned int	EG_Font_GetWidth(EG_Font *f_p);
unsigned int	EG_Font_GetHeight(EG_Font *f_p);

EG_Font*        EG_Font_Import(char *name_p, EG_FontMap *mapping_ap, SDL_Surface *surface_p, SDL_Color col);
void            EG_Font_Destroy(EG_Font *font_p);

void 		EG_Font_PutChar(EG_Font *f_p, SDL_Surface *surface_p, char c, int x, int y, unsigned int flags);
void 		EG_Font_PutString(EG_Font *f_p, SDL_Surface *surface_p, char *string_p, int x, int y, unsigned int flags);

#ifdef __cplusplus
}
#endif



#endif
