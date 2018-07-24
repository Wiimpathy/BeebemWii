/* START OF image.c ------------------------------------------------------------
 *
 *	Image widget.
 *
 *	---
 *	THIS GUI IS TOTALLY *BROKEN*! PLEASE DO NOT USE IT!
 *	---
 */


#if HAVE_CONFIG_H
#       include <config.h>
#endif


#include <log.h>

#include <sdl.h>
#include <graphics.h>

#include <functions.h>

#include <image.h>
#include <image_private.h>

#include <window.h>
#include <window_private.h>

#include <widget_shared.h>

#include <widget.h>
#include <widget_private.h>

#include <SDL/SDL.h>


/* SDL_Surface locking:
 *  */
#define LOCK(s)   {if(SDL_MUSTLOCK(s))(void)SDL_LockSurface(s);}
#define UNLOCK(s) {if(SDL_MUSTLOCK(s))(void)SDL_UnlockSurface(s);}


/* Callbacks:
 */

static void Callback_Destroy(EG_Widget *widget_ptr)
{
        EG_Image *image_ptr;

        /* If widget is actually just a NULL, then do nothing but log error.
         */
        if (widget_ptr==NULL){
                EG_Log(EG_LOG_ERROR, dL"Tried to Destroy EG_Image with"
                 " widget_ptr=NULL", dR);
              return;
        }
	if (widget_ptr == NULL) {
		EG_Log(EG_LOG_ERROR, dL"EG_Widget is NULL.", dR);
		return;
	}
	if (EG_Widget_GetType(widget_ptr) != EG_Widget_Type_Image) {
		EG_Log(EG_LOG_ERROR, dL"EG_Widget payload is not of type EG_Image.", dR);
		return;
	}
	if ( (image_ptr = (EG_Image*) EG_Widget_GetPayload(widget_ptr)) == NULL){
		EG_Log(EG_LOG_ERROR, dL"EG_Widget payload is NULL.", dR);
		return;
	}
	EG_ASSERT_MALLOC( (void*) image_ptr);

	if (image_ptr->normal_image_ptr != NULL) SDL_FreeSurface(image_ptr->normal_image_ptr);
	if (image_ptr->flipped_image_ptr != NULL) SDL_FreeSurface(image_ptr->flipped_image_ptr);

	/* Call generic function to destroy widget + payload
	 */
	EG_Callback_Generic_Destroy(widget_ptr);
}

static EG_BOOL Callback_Paint(EG_Widget *widget_ptr, SDL_Rect area)
{
        EG_Image *image_ptr;
        EG_Window *window_ptr;
        
        SDL_Rect loc;

        SDL_Color color;
        SDL_Surface *surface_ptr;


        if ( EG_Shared_GetRenderingDetails(widget_ptr, area,
         (void*) &image_ptr, &window_ptr, &surface_ptr, &color
	 , &loc) != EG_TRUE )
                return(EG_TRUE);

#ifdef EG_DEBUG
        printf("SOMEONE CALLED PAINT FOR '%s' [repaint area (window)="
         "{%d, %d, %d, %d}:widget area (SDL_Surface)={%d, %d, %d, %d}]\n"
         , EG_Widget_GetName(widget_ptr), area.x, area.y, area.w, area.h
         , loc.x, loc.y, loc.w, loc.h);
#endif

	/* Render the widget.
	 */
	EG_Draw_Box(surface_ptr, &loc, &color);

        /* If in 'flipped' mode, reverse frame_buffer:
         */
        if (EG_IsUpsideDown() == EG_TRUE) {
		loc.x=surface_ptr->w - loc.x - loc.w + ((loc.w - image_ptr->flipped_image_ptr->w)/2);
		loc.y=surface_ptr->h - loc.y - loc.h + ((loc.h - image_ptr->flipped_image_ptr->h)/2);

		if (image_ptr->flipped_image_ptr != NULL) {
			SDL_BlitSurface(image_ptr->flipped_image_ptr, NULL, surface_ptr, &loc);
			SDL_UpdateRect(surface_ptr, loc.x, loc.y, loc.w, loc.h); 
		}
	} else {

		loc.x+=(loc.w - image_ptr->flipped_image_ptr->w)/2;
		loc.y+=(loc.h - image_ptr->flipped_image_ptr->h)/2;

		if (image_ptr->normal_image_ptr != NULL) {
			SDL_BlitSurface(image_ptr->normal_image_ptr, NULL, surface_ptr, &loc);
			SDL_UpdateRect(surface_ptr, loc.x, loc.y, loc.w, loc.h); 
		}
	}



	/* Paint succeeded so return true.
	 */
	return(EG_TRUE);
}


/* Private functions:
 */

static void InitializeWidget(EG_Widget *widget_ptr, SDL_Color color
 , SDL_Rect dimension, int alignment)
{
	/* Initialize callbacks:
	 */
	(void) EG_Widget_SetCallback_Destroy(widget_ptr
	 , Callback_Destroy);

	(void) EG_Widget_SetCallback_Paint(widget_ptr, Callback_Paint);

	(void) EG_Widget_SetCallback_SDL_Event(widget_ptr
	 , EG_Callback_Generic_SDL_Event);

	(void) EG_Widget_SetCallback_Visible(widget_ptr
	 , EG_Callback_Generic_Visible);

	(void) EG_Widget_SetCallback_Stopped(widget_ptr
	 , EG_Callback_Generic_Stopped);

	(void) EG_Widget_SetCallback_Enabled(widget_ptr
	 , EG_Callback_Generic_Enabled_NoSupport);

	(void) EG_Widget_SetCallback_GotFocus(widget_ptr
	 , EG_Callback_Generic_GotFocus_NoSupport);

	(void) EG_Widget_SetCallback_LostFocus(widget_ptr
	 , EG_Callback_Generic_LostFocus_NoSupport);

	(void) EG_Widget_SetCallback_Attach(widget_ptr
	 , EG_Callback_Generic_Attach);

	/* Initialize state:
	 */
	(void) EG_Widget_SetCanGetFocusToggle(widget_ptr, EG_FALSE);

	(void) EG_Widget_SetDimension(widget_ptr, dimension);
	(void) EG_Widget_SetBackgroundColor(widget_ptr, color);

	// Surfaces
	(void) EG_Image_SetAlignment(widget_ptr, alignment);
}

static SDL_Surface* CloneSurface(EG_Widget *w_p, SDL_Surface *source_p)
{
	SDL_Color col;
        SDL_Surface *new_surface_p;

	col = EG_Widget_GetBackgroundColor(w_p);

        if ( (new_surface_p=SDL_CreateRGBSurface(
           SDL_SWSURFACE, source_p->w, source_p->h, 32, 0, 0, 0, 0) ) == NULL) {
                fprintf(stderr, "Unable to create SDL Surface for XPM data"
                 ": %s\n", SDL_GetError());
                return NULL;
        }

	SDL_FillRect(new_surface_p, NULL, SDL_MapRGB(new_surface_p->format, col.r,col.g,col.b));
        SDL_BlitSurface(source_p, NULL, new_surface_p, NULL);

	SDL_UpdateRect(new_surface_p, 0,0,0,0);

        return new_surface_p;
}


static SDL_Surface* CloneSurfaceFlipped(EG_Widget *w_p, SDL_Surface *source_p, SDL_Color color_key)
{
        SDL_Surface *flipped_p;
        int x, y;
        Uint32 color, key;
	SDL_Color col;

	if ( (flipped_p=SDL_CreateRGBSurface(SDL_SWSURFACE, source_p->w, source_p->h, 32, 0, 0, 0, 0) ) == NULL) {
                fprintf(stderr, "Unable to create SDL Surface for XPM data"
                 ": %s\n", SDL_GetError());
                return NULL;
	}

	col = EG_Widget_GetBackgroundColor(w_p);
	SDL_FillRect(flipped_p, NULL, SDL_MapRGB(flipped_p->format, col.r,col.g,col.b));
	SDL_UpdateRect(flipped_p, 0,0,0,0);

        LOCK(flipped_p);
        LOCK(source_p);

	key = SDL_MapRGB(source_p->format, color_key.r, color_key.g, color_key.b);

        for(y=0; y<source_p->h; y++)
                for(x=0; x<source_p->w; x++) {
                        color = GetPixel(source_p, source_p->w-x-1, source_p->h-y-1);

			SDL_GetRGB(color, source_p->format, &col.r, &col.g, &col.b);

			if (color != key) {
				color = SDL_MapRGB(flipped_p->format, col.r, col.g, col.b);
                        	PutPixel(flipped_p, x, y, color);
			}
                }

        UNLOCK(source_p);
        UNLOCK(flipped_p);

        return flipped_p;
}

/* Public functions:
 */

EG_Widget* EG_Image_Create(const char *name_ptr, SDL_Color color, int alignment
 , SDL_Surface *source_image_ptr, SDL_Rect dimension, SDL_Color color_key)
{
	EG_Image *image_ptr;
	EG_Widget *widget_ptr;
	void *ptr;

	/* Create the new widget.
	 */
	SHARED__ALLOC_PAYLOAD_STRUCT( ptr, EG_Image
	 , "Unable to malloc EG_Image struct");
	image_ptr = (EG_Image*) ptr;

	SHARED__CREATE_NEW_EG_WIDGET(widget_ptr, name_ptr, EG_Widget_Type_Image
	 , image_ptr);

	SHARED__ATTACH_PAYLOAD_TO_WIDGET(widget_ptr, image_ptr);

	InitializeWidget(widget_ptr, color, dimension, alignment);

	/* Make normal and flipped copy of the source image.
	 */

	if (source_image_ptr != NULL) {
		SDL_SetColorKey(source_image_ptr, SDL_SRCCOLORKEY, SDL_MapRGB(source_image_ptr->format, color_key.r,color_key.g,color_key.b));
		image_ptr->normal_image_ptr=CloneSurface(widget_ptr, source_image_ptr);	
		image_ptr->flipped_image_ptr=CloneSurfaceFlipped(widget_ptr, source_image_ptr, color_key);
	}

	return(widget_ptr);
}

int EG_Image_GetAlignment(EG_Widget *widget_ptr)
{
	EG_Image *image_ptr;

	EG_IMAGE_GET_STRUCT_PTR(widget_ptr, image_ptr, EG_IMAGE_ALIGN_CENTERED);
	return(image_ptr->alignment);
}

EG_BOOL EG_Image_SetAlignment(EG_Widget *widget_ptr, int alignment)
{
        EG_Image *image_ptr;

	EG_IMAGE_GET_STRUCT_PTR(widget_ptr, image_ptr, EG_FALSE);
	image_ptr->alignment = alignment;

	(void) EG_Widget_RepaintLot(widget_ptr);

	return(EG_TRUE);
}



