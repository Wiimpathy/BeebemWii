/* START OF virtualzone.h ----------------------------------------------
 *
 * 	Allows a bitmap to represent buttons, useful for virtual keyboards.
 *
 *	---
 *	THIS GUI IS TOTALLY *BROKEN*! PLEASE DO NOT USE IT!
 *	---
 */


#ifndef _EG_VIRTUALZONE_H_
#define _EG_VIRTUALZONE_H_


#if HAVE_CONFIG_H
#       include <config.h>
#endif

#include <types.h>
#include <sdl.h>

#include <window.h>
#include <widget.h>

#include <SDL/SDL.h>


/* Widget type: [TODO] Calc. hash.
 */
#define EG_Widget_Type_VirtualZone 0x666

/* Public functions:
 */

#ifdef __cplusplus
extern "C"{
#endif
//	EG_Widget*	EG_Button_Create(const char *name_ptr, SDL_Color color, int alignment, const char *caption_ptr, SDL_Rect dimension);
//
//	EG_BOOL 	EG_Button_SetCaption(EG_Widget *widget_ptr, const char *caption_ptr);
//	const char* 	EG_Button_GetCaption(EG_Widget *widget_ptr);
//
//	EG_BOOL 	EG_Button_SetAlignment(EG_Widget *widget_ptr, int alignment);
//	int 		EG_Button_GetAlignment(EG_Widget *widget_ptr);

	EG_Widget* 	EG_VirtualZone_Create(const char *name_ptr, unsigned int zone_count);

	void 		EG_VirtualZone_SetAllZonesArea(EG_Widget *widget_ptr, SDL_Rect area);
	SDL_Rect 	EG_VirtualZone_GetAllZonesArea(EG_Widget *widget_ptr);

	EG_BOOL		EG_VirtualZone_UseThisSurfaceAsSource(EG_Widget *widget_ptr, SDL_Surface *surface_ptr, SDL_Rect total_zone_area, SDL_Surface *frame_buffer_format_ptr);

	EG_BOOL 	EG_VirtualZone_SetFocusedZone(EG_Widget *widget_ptr, unsigned int zone_index);
	void 		EG_VirtualZone_ClearFocus(EG_Widget *widget_ptr);
	int 		EG_VirtualZone_GetFocusedZone(EG_Widget *widget_ptr);

	EG_BOOL 	EG_VirtualZone_SetZone(EG_Widget *widget_ptr, unsigned int zone_index, SDL_Rect normal, SDL_Rect focused, SDL_Rect pressed, SDL_Rect held, void *user_ptr);

	void EG_VirtualZone_ClearZone(EG_Widget *widget_ptr, unsigned int zone_index);


	unsigned int EG_VirtualZone_GetZoneCount(EG_Widget *widget_ptr);


#ifdef __cplusplus
}
#endif


//#define EG_Button_Destroy			EG_Widget_CallDestroy
//
//#define EG_Button_SetMyCallback_OnEvent		EG_Widget_SetUserCallback_OnEvent
//#define EG_Button_ClearMyCallback_OnEvent	EG_Widget_ClearUserCallback_OnEvent
//
//#define EG_Button_SetMyCallback_OnClick		EG_Widget_SetUserCallback_OnClick
//#define EG_Button_ClearMyCallback_OnClick	EG_Widget_ClearUserCallback_OnClick
//
//#define EG_Button_SetBackgroundColor		EG_Widget_SetBackgroundColor
//#define EG_Button_GetBackgroundColor		EG_Widget_GetBackgroundColor
//
//#define EG_Button_GetFocus			EG_Window_SetFocusToThisWidget
//
//#define EG_Button_Repaint			EG_Widget_Repaint
//#define EG_Button_RepaintLot			EG_Widget_RepaintLot
//
//#define EG_Button_Hide				EG_Window_HideWidget
//#define EG_Button_Show				EG_Window_ShowWidget
//
//#define EG_Button_Enable			EG_Window_EnableWidget
//#define EG_Button_Disable			EG_Window_DisableWidget
//
//#define EG_Button_IsVisible			EG_Widget_IsVisible
//#define EG_Button_IsEnabled			EG_Widget_IsEnabled
//
//#define EG_Button_GetID				EG_Widget_GetID
//#define EG_Button_GetName			EG_Widget_GetName

/* END OF virtualzone.h -------------------------------------------------------------
 */
#endif



