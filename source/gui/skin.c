/* Simple bitmap skinning of EG widgets.
 *
 * Create the box/button from images and then link to a widget.
 */

#if HAVE_CONFIG_H
#       include <config.h>
#endif

#include <log.h>
#include <functions.h>
#include <skin.h>

#define LOCK(s)   {if(SDL_MUSTLOCK(s))(void)SDL_LockSurface(s);}
#define UNLOCK(s) {if(SDL_MUSTLOCK(s))(void)SDL_UnlockSurface(s);}


/* The usual suspects to support 'flipped' mode on the GP2X:
 */

inline static void Skin_PutPixel(SDL_Surface *surface_p, int x, int y, Uint8 pixel)
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

inline static Uint32 Skin_GetPixel(SDL_Surface *surface_p, int x, int y)
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

/* These only work for 8bit .BMP files.
 */

static SDL_Surface* LoadBMP(const char *filename_p)
{
	SDL_Surface *s_p, *r_p;

	if ( (s_p = SDL_LoadBMP(filename_p) ) == NULL) return NULL;

	r_p = SDL_DisplayFormat(s_p); // or NULL
	SDL_FreeSurface(s_p);
	
	return r_p;
}


static SDL_Surface* LoadFlippedBMP(const char *filename_p)
{
	SDL_Surface *s_p, *d_p, *r_p;
	int x, y;
	Uint32 p;

	if ( (s_p = SDL_LoadBMP(filename_p) ) == NULL) return NULL;
	if ( (d_p = SDL_LoadBMP(filename_p) ) == NULL) {
		SDL_FreeSurface(s_p);
		return NULL;
	}

	LOCK(s_p);
	LOCK(d_p);
        for(y=0; y<s_p->h; y++)
                for(x=0; x<s_p->w; x++) {
                        p = Skin_GetPixel(s_p, s_p->w-x-1, s_p->h-y-1);
                        Skin_PutPixel(d_p, x, y, p);
                }
	UNLOCK(s_p);
	UNLOCK(d_p);

	SDL_FreeSurface(s_p);

	r_p = SDL_DisplayFormat(d_p); // or NULL
	SDL_FreeSurface(d_p);
	return r_p;	
}

/*
void SkinTest(SDL_Surface *dest_p)
{
	EG_Skin_Box *t_p;
	EG_Skin_Button *b_p;




	t_p = EG_Skin_Box_CreateNew(
	 "../resources/window_top_titled.bmp",
	 "../resources/window_bottom.bmp",
	 "../resources/window_left.bmp",
	 "../resources/window_right.bmp");
	if (t_p == NULL) {
		printf("NULL\n");
		exit(1);
	}
	EG_Skin_Box_SetFillColor(t_p, 50, 60, 50);



	b_p = EG_Skin_Button_CreateNew("../resources/button1.bmp", 3);
	if (b_p == NULL) {
		printf("BUTTON NULL\n");
		exit(1);
	}
	EG_Skin_Button_SetColorKey(b_p, 255, 0, 0);

	EG_Skin_Box_Paint(t_p, dest_p, 40, 40, 320-44, 240-150);

	EG_Skin_Button_Paint(b_p, dest_p, 0, 50, 80, 80, 10);
	SDL_Delay(1000);
	EG_Skin_Button_Paint(b_p, dest_p, 1, 50, 80, 80, 10);
	SDL_Delay(1000);
	EG_Skin_Button_Paint(b_p, dest_p, 2, 50, 80, 80, 10);
	SDL_Delay(1000);
	EG_Skin_Button_Paint(b_p, dest_p, 1, 50, 80, 80, 10);
	SDL_Delay(1000);

	EG_Skin_Box_Free(t_p);
	EG_Skin_Button_Free(b_p);

	exit(1);
}
*/

EG_Skin_Box* EG_Skin_Box_CreateNew(const char *top_p, const char *bottom_p, const char *left_p, const char *right_p)
{
	EG_Skin_Box *box_p;

	/* Create and initialize box structure:
 	 */
	if ( (box_p = (EG_Skin_Box*) EG_Malloc( sizeof(EG_Skin_Box) )) == NULL ) {
		return NULL;
	}

	/* Initialize EG_Skin_Box:
	 */
	box_p->top_p = box_p->bottom_p = box_p->left_p = box_p->right_p
	 = box_p->top_flipped_p = box_p->bottom_flipped_p = box_p->left_flipped_p
	 = box_p->right_flipped_p = NULL;
	(void) EG_Skin_Box_SetFillColor(box_p, 0, 0, 0);

	/* Create top:
	 */
	if ( (box_p->top_p = LoadBMP(top_p)) == NULL) goto FAIL;
	if ( (box_p->top_flipped_p = LoadFlippedBMP(top_p)) == NULL) goto FAIL;

	/* Create bottom:
	 */
	if ( (box_p->bottom_p = LoadBMP(bottom_p)) == NULL) goto FAIL;
	if ( (box_p->bottom_flipped_p = LoadFlippedBMP(bottom_p)) == NULL) goto FAIL;

	/* Create left:
	 */
	if ( (box_p->left_p = LoadBMP(left_p)) == NULL) goto FAIL;
	if ( (box_p->left_flipped_p = LoadFlippedBMP(left_p)) == NULL) goto FAIL;

	/* Create right:
	 */
	if ( (box_p->right_p = LoadBMP(right_p)) == NULL) goto FAIL;
	if ( (box_p->right_flipped_p = LoadFlippedBMP(right_p)) == NULL) goto FAIL;

	return box_p;

FAIL:
	if (box_p->top_p != NULL) SDL_FreeSurface(box_p->top_p);
	if (box_p->top_flipped_p != NULL) SDL_FreeSurface(box_p->top_flipped_p);

	if (box_p->bottom_p != NULL) SDL_FreeSurface(box_p->bottom_p);
	if (box_p->bottom_flipped_p != NULL) SDL_FreeSurface(box_p->bottom_flipped_p);

	if (box_p->left_p != NULL) SDL_FreeSurface(box_p->left_p);
	if (box_p->left_flipped_p != NULL) SDL_FreeSurface(box_p->left_flipped_p);

	if (box_p->right_p != NULL) SDL_FreeSurface(box_p->right_p);
	if (box_p->right_flipped_p != NULL) SDL_FreeSurface(box_p->right_flipped_p);

	return NULL;
}

void EG_Skin_Box_Free(EG_Skin_Box *box_p)
{
	if (box_p->top_p != NULL) SDL_FreeSurface(box_p->top_p);
	if (box_p->bottom_p != NULL) SDL_FreeSurface(box_p->bottom_p);
	if (box_p->left_p != NULL) SDL_FreeSurface(box_p->left_p);
	if (box_p->right_p != NULL) SDL_FreeSurface(box_p->right_p);

	if (box_p->top_flipped_p != NULL) SDL_FreeSurface(box_p->top_flipped_p);
	if (box_p->bottom_flipped_p != NULL) SDL_FreeSurface(box_p->bottom_flipped_p);
	if (box_p->left_flipped_p != NULL) SDL_FreeSurface(box_p->left_flipped_p);
	if (box_p->right_flipped_p != NULL) SDL_FreeSurface(box_p->right_flipped_p);

	if (box_p != NULL) EG_Free(box_p);
}

static void Flip(SDL_Rect *a_p)
{
	a_p->x = 320 - a_p->x - a_p->w;
	a_p->y = 240 - a_p->y - a_p->h;
}

EG_BOOL EG_Skin_Box_Paint(EG_Skin_Box *box_p, SDL_Surface *s_p, int x, int y, unsigned int w, unsigned int h)
{
	SDL_Color c;
	SDL_Rect a, a2, a3;

	unsigned int total_h;

	if (box_p == NULL) return EG_FALSE;
	if (s_p == NULL) return EG_FALSE;

	c = EG_Skin_Box_GetFillColor(box_p);

	a.x = x; a.y = y; a.w = w; a.h = h;
	if ( EG_IsUpsideDown() ) Flip(&a);

	/* Paint box area in fill color:
	 */
	SDL_FillRect(s_p, &a, SDL_MapRGB(s_p->format, c.r, c.g, c.b));

	/* Draw box boarder:
	 */
	if ( ! EG_IsUpsideDown() ) {

		/* ---- TOP LEFT:
		 */
		a2.w = 0; a2.h = 0; a2.x = a.x; a2.y = a.y;
		a3.w = (a.w>>1)+(a.w&1); a3.h = a.h; a3.x = 0; a3.y = 0;
		SDL_BlitSurface(box_p->top_p, &a3, s_p, &a2);

		/* ---- TOP RIGHT:
		 */
		a2.w = 0; a2.h = 0; a2.x = a.x + (a.w>>1)+(a.w&1); a2.y = a.y;
		a3.w = (a.w)>>1; a3.h = a.h; a3.x = box_p->top_p->w - (a.w>>1); a3.y = 0;
		SDL_BlitSurface(box_p->top_p, &a3, s_p, &a2);

		/* ---- LEFT TOP:
		 */
		total_h = (a.h - box_p->top_p->h - box_p->bottom_p->h);

		a2.w = 0; a2.h = 0; a2.x = a.x; a2.y = a.y + box_p->top_p->h;
		a3.w = a.w; a3.h = (total_h>>1)+(total_h&1); a3.x = 0; a3.y = 0;
		SDL_BlitSurface(box_p->left_p, &a3, s_p, &a2);

		/* ---- LEFT BOTTOM:
		 */
		a2.w = 0; a2.h = 0; a2.x = a.x; a2.y = a.y + box_p->top_p->h + (total_h>>1)+(total_h&1);
		a3.w = a.w; a3.h = (total_h>>1); a3.x = 0; a3.y = box_p->left_p->h - (total_h>>1);
		SDL_BlitSurface(box_p->left_p, &a3, s_p, &a2);

		/* ---- RIGHT TOP:
		 */
		a2.w = 0; a2.h = 0; a2.x = a.x + a.w - box_p->right_p->w; a2.y = a.y + box_p->top_p->h;
		a3.w = a.w; a3.h = (total_h>>1)+(total_h&1); a3.x = 0; a3.y = 0;
		SDL_BlitSurface(box_p->right_p, &a3, s_p, &a2);

		/* ---- RIGHT BOTTOM:
		 */
		a2.w = 0; a2.h = 0; a2.x = a.x + a.w - box_p->right_p->w; a2.y = a.y + box_p->top_p->h + (total_h>>1)+(total_h&1);
		a3.w = a.w; a3.h = (total_h)>>1; a3.x = 0; a3.y = box_p->left_p->h - ( (total_h)>>1);
		SDL_BlitSurface(box_p->right_p, &a3, s_p, &a2);

		/* ---- BOTTOM LEFT:
		 */
		a2.w = 0; a2.h = 0; a2.x = a.x; a2.y = a.y + a.h - box_p->bottom_p->h;
		a3.w = (a.w>>1)+(a.w&1); a3.h = a.h; a3.x = 0; a3.y = 0;
		SDL_BlitSurface(box_p->bottom_p, &a3, s_p, &a2);

		/* ---- BOTTOM RIGHT:
		 */
		a2.w = 0; a2.h = 0; a2.x = a.x + (a.w>>1)+(a.w&1); a2.y = a.y + (a.h) - box_p->bottom_p->h;
		a3.w = (a.w>>1); a3.h = a.h; a3.x = box_p->bottom_p->w - ((a.w)>>1); a3.y = 0;
		SDL_BlitSurface(box_p->bottom_p, &a3, s_p, &a2);

	} else {

		/* ---- TOP LEFT:
		 */
		a2.w = 0; a2.h = 0; a2.x = a.x; a2.y = a.y + a.h - box_p->top_flipped_p->h;
		a3.w = (a.w>>1)+(a.w&1); a3.h = a.h; a3.x = 0; a3.y = 0;
		SDL_BlitSurface(box_p->top_flipped_p, &a3, s_p, &a2);

		/* ---- TOP RIGHT:
		 */
		a2.w = 0; a2.h = 0; a2.x = a.x + (a.w>>1)+(a.w&1); a2.y = a.y + a.h - box_p->top_flipped_p->h;
		a3.w = (a.w)>>1; a3.h = a.h; a3.x = box_p->top_flipped_p->w - (a.w>>1); a3.y = 0;
		SDL_BlitSurface(box_p->top_flipped_p, &a3, s_p, &a2);

		/* ---- LEFT TOP:
		 */
		total_h = (a.h - box_p->top_p->h - box_p->bottom_p->h);

		a2.w = 0; a2.h = 0; a2.x = a.x + a.w - box_p->left_flipped_p->w; a2.y = a.y + box_p->bottom_flipped_p->h;
		a3.w = a.w; a3.h = (total_h>>1)+(total_h&1); a3.x = 0; a3.y = 0;
		SDL_BlitSurface(box_p->left_flipped_p, &a3, s_p, &a2);

		/* ---- LEFT BOTTOM:
		 */
		a2.w = 0; a2.h = 0; a2.x = a.x + a.w - box_p->left_flipped_p->w;
		a2.y = a.y + box_p->bottom_flipped_p->h + (total_h>>1)+(total_h&1);
		a3.w = a.w; a3.h = (total_h>>1); a3.x = 0; a3.y = box_p->left_flipped_p->h - (total_h>>1);
		SDL_BlitSurface(box_p->left_flipped_p, &a3, s_p, &a2);

		/* ---- RIGHT TOP:
		 */
		a2.w = 0; a2.h = 0; a2.x = a.x; a2.y = a.y + box_p->bottom_flipped_p->h;
		a3.w = a.w; a3.h = (total_h>>1)+(total_h&1); a3.x = 0; a3.y = 0;
		SDL_BlitSurface(box_p->right_flipped_p, &a3, s_p, &a2);

		/* ---- RIGHT BOTTOM:
		 */
		a2.w = 0; a2.h = 0; a2.x = a.x; a2.y = a.y + box_p->bottom_flipped_p->h + (total_h>>1)+(total_h&1);
		a3.w = a.w; a3.h = (total_h)>>1; a3.x = 0; a3.y = box_p->left_flipped_p->h - ( (total_h)>>1);
		SDL_BlitSurface(box_p->right_flipped_p, &a3, s_p, &a2);

		/* ---- BOTTOM LEFT:
		 */
		a2.w = 0; a2.h = 0; a2.x = a.x; a2.y = a.y;
		a3.w = (a.w>>1)+(a.w&1); a3.h = a.h; a3.x = 0; a3.y = 0;
		SDL_BlitSurface(box_p->bottom_flipped_p, &a3, s_p, &a2);

		/* ---- BOTTOM RIGHT:
		 */
		a2.w = 0; a2.h = 0; a2.x = a.x + (a.w>>1)+(a.w&1); a2.y = a.y;
		a3.w = (a.w>>1); a3.h = a.h; a3.x = box_p->bottom_flipped_p->w - ((a.w)>>1); a3.y = 0;
		SDL_BlitSurface(box_p->bottom_flipped_p, &a3, s_p, &a2);
	}
//	SDL_UpdateRect(s_p, a.x, a.y, a.w, a.h);

	return EG_TRUE;
}


EG_BOOL EG_Skin_Box_SetFillColor(EG_Skin_Box *box_p, unsigned char red, unsigned char green, unsigned char blue)
{
	SDL_Color c;

	if (box_p == NULL) return EG_FALSE;

	c.r = red; c.g = green; c.b = blue; c.unused = 0;
	box_p->fill_color = c;
	return EG_TRUE;
}

SDL_Color EG_Skin_Box_GetFillColor(EG_Skin_Box *box_p)
{
	if (box_p == NULL) return (SDL_Color) {0,0,0,0};
	else return box_p->fill_color;
}

Uint8 EG_Skin_Box_GetColorR(EG_Skin_Box *box_p)
{
	if (box_p == NULL) return 0;
	else return box_p->fill_color.r;
}

Uint8 EG_Skin_Box_GetColorG(EG_Skin_Box *box_p)
{
	if (box_p == NULL) return 0;
	else return box_p->fill_color.g;
}

Uint8 EG_Skin_Box_GetColorB(EG_Skin_Box *box_p)
{
	if (box_p == NULL) return 0;
	else return box_p->fill_color.b;
}




EG_Skin_Button* EG_Skin_Button_CreateNew(const char *buttons_file_p, unsigned int button_count)
{
	EG_Skin_Button *button_p;

	/* Limit # of buttons:
	 */
	if (button_count<1 || button_count>EG_SKIN_BUTTON_MAXCOUNT)
		return(NULL);

	/* Create and initialize button structure:
 	 */
	if ( (button_p = (EG_Skin_Button*) EG_Malloc( sizeof(EG_Skin_Button) )) == NULL ) {
		return(NULL);
	}

	button_p->buttons_p = button_p->buttons_flipped_p = NULL;
	button_p->button_count = button_count;

	/* Load buttons:
	 */
	if ( (button_p->buttons_p = LoadBMP(buttons_file_p)) == NULL) {
		EG_Free(button_p);
		return NULL;
	}

	if ( (button_p->buttons_flipped_p = LoadFlippedBMP(buttons_file_p)) == NULL) {
		SDL_FreeSurface(button_p->buttons_p);
		EG_Free(button_p);
		return NULL;
	}

	return button_p;
}

void EG_Skin_Button_SetColorKey(EG_Skin_Button *button_p, unsigned char r, unsigned char g, unsigned char b)
{
	if (button_p == NULL) return;

	if (button_p->buttons_p != NULL)
		SDL_SetColorKey(button_p->buttons_p, SDL_SRCCOLORKEY, SDL_MapRGB(button_p->buttons_p->format, r, g, b) );
	if (button_p->buttons_flipped_p != NULL)
		SDL_SetColorKey(button_p->buttons_flipped_p, SDL_SRCCOLORKEY, SDL_MapRGB(button_p->buttons_flipped_p->format, r, g, b) );
}

void EG_Skin_Button_Free(EG_Skin_Button *button_p)
{
	if (button_p == NULL) return;

	if (button_p->buttons_p != NULL) SDL_FreeSurface(button_p->buttons_p);
	if (button_p->buttons_flipped_p != NULL) SDL_FreeSurface(button_p->buttons_flipped_p);

	EG_Free(button_p);
}


EG_BOOL EG_Skin_Button_Paint(EG_Skin_Button *button_p, SDL_Surface *s_p, unsigned int index, int x, int y, unsigned int w, unsigned int h)
{
	SDL_Surface *d_p;
	SDL_Rect a, a_b, a2, a3;

	if (button_p == NULL) return EG_FALSE;
	if (s_p == NULL) return EG_FALSE;
	if (index >= button_p->button_count) return EG_FALSE;

	/* Calculate which button to paint
	 */
	a.x = x; a.y = y; a.w = w; a.h = h;
	if ( EG_IsUpsideDown() ) {
		d_p = button_p->buttons_flipped_p;
		Flip(&a);
		index = button_p->button_count - 1 - index;
	} else {
		d_p = button_p->buttons_p;
	}

	/* Calc area of button graphic to display:
  	 */
	a_b.h = button_p->buttons_p->h / button_p->button_count;
	a_b.w = button_p->buttons_p->w;
	a_b.x = 0;
	a_b.y = a_b.h * index;

	/* Paint button:
	 */
	a2.w = 0; a2.h = 0; a2.x = a.x; a2.y = a.y;
	a3.w = (a.w>>1)+(a.w&1); a3.h = (a.h>>1)+(a.h&1); a3.x = 0; a3.y = a_b.y;
	SDL_BlitSurface(d_p, &a3, s_p, &a2);

	a2.w = 0; a2.h = 0; a2.x = a.x + (a.w>>1)+(a.w&1); a2.y = a.y;
	a3.w = (a.w>>1)+(a.w&1); a3.h = (a.h>>1)+(a.h&1); a3.x = a_b.w - (a.w>>1); a3.y = a_b.y;
	SDL_BlitSurface(d_p, &a3, s_p, &a2);

	a2.w = 0; a2.h = 0; a2.x = a.x; a2.y = a.y + (a.h>>1) + (a.h&1);
	a3.w = (a.w>>1)+(a.w&1); a3.h = (a.h>>1); a3.x = 0; a3.y = a_b.y + a_b.h - (a.h>>1);
	SDL_BlitSurface(d_p, &a3, s_p, &a2);

	a2.w = 0; a2.h = 0; a2.x = a.x + (a.w>>1)+(a.w&1); a2.y = a.y + (a.h>>1) + (a.h&1);
	a3.w = (a.w>>1)+(a.w&1); a3.h = (a.h>>1); a3.x = a_b.w - (a.w>>1); a3.y = a_b.y + a_b.h - (a.h>>1);
	SDL_BlitSurface(d_p, &a3, s_p, &a2);

//	SDL_UpdateRect(s_p, a.x, a.y, a.w, a.h);

	return EG_TRUE;
}
