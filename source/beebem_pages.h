#ifndef BEEBEM_PAGES_HEADER
#define BEEBEM_PAGES_HEADER

#include <gui/gui.h>

EG_BOOL 	BeebEmPages_Create(SDL_Surface *frame_buffer_p);
void 		BeebEmPages_Destroy(void);

void		BeebEmPages_ShowMenu(void);

void 		BeebEmPages_SetPalette(SDL_Surface *surface_p);

#endif
