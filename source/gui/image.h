/* START OF image.h ------------------------------------------------------------
 *
 *	Simple image widget
 *
 *	---
 *	THIS GUI IS TOTALLY *BROKEN*! PLEASE DO NOT USE IT!
 *	---
 */


#ifndef _DW_IMAGE_H_
#define _DW_IMAGE_H_


#if HAVE_CONFIG_H
#       include <config.h>
#endif

#include <types.h>
#include <sdl.h>

#include <window.h>
#include <widget.h>

#include <SDL/SDL.h>


/* Widget type:
 */
#define EG_Widget_Type_Image 0x5eb06a74

/* Alignment:
 */
#define EG_IMAGE_ALIGN_CENTERED		0


#ifdef __cplusplus
extern "C"{
#endif
	/* Public functions:
	 */
	EG_Widget* 	EG_Image_Create(const char *name_ptr, SDL_Color color, int alignment, SDL_Surface *source_image_ptr
	 , SDL_Rect dimension, SDL_Color color_key);

	int 		EG_Image_GetAlignment(EG_Widget *widget_ptr);
	EG_BOOL 	EG_Image_SetAlignment(EG_Widget *widget_ptr, int);
#ifdef __cplusplus
}
#endif

#define EG_Image_SetMyCallback_OnEvent          EG_Widget_SetUserCallback_OnEvent
#define EG_Image_ClearMyCallback_OnEvent        EG_Widget_ClearUserCallback_OnEvent

#define EG_Image_Destroy			EG_Widget_CallDestroy

#define EG_Image_Hide				EG_Window_HideWidget
#define EG_Image_Show				EG_Window_ShowWidget

#define EG_Image_IsVisible			EG_Widget_IsVisible
#define EG_Image_IsEnabled			EG_Widget_IsEnabled

#define EG_Image_SetBackgroundColor		EG_Widget_SetBackgroundColor
#define EG_Image_GetBackgroundColor		EG_Widget_GetBackgroundColor

#define EG_Image_Repaint			EG_Widget_Repaint
#define EG_Image_RepaintLot			EG_Widget_RepaintLot

#define EG_Image_GetID				EG_Widget_GetID
#define EG_Image_GetName			EG_Widget_GetName

/* END OF image.h --------------------------------------------------------------
 */
#endif



