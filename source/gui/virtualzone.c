/* START OF virtualzone.c ---------------------------------------------------------------
 *
 *	Allows a bitmap to represent buttons, useful for virtual keyboards.
 *
 *	---
 *	THIS GUI IS TOTALLY *BROKEN*! PLEASE DO NOT USE IT!
 *	---
 */

#if HAVE_CONFIG_H
#       include <config.h>
#endif

#include <log.h>

#include <functions.h>

#include <virtualzone.h>
#include <virtualzone_private.h>

#include <window.h>
#include <window_private.h>

#include <widget.h>
#include <widget_private.h>

#include <widget_shared.h>

#include <SDL/SDL.h>



/* Callbacks:
 */

static void Callback_Destroy(EG_Widget *widget_ptr)
{
        EG_VirtualZone *virtual_zone_ptr;

        /* If widget is actually just a NULL, then do nothing but log error.
         */
        if (widget_ptr==NULL){
                EG_Log(EG_LOG_ERROR, dL"Tried to Destroy Virtual Zone with"
                 " widget_ptr=NULL", dR);
              return;
        }
        /* If widget is not a VirtualZone, then do noting, but log
         * as error. (A serious one too!)
         */
        if (EG_Widget_GetType(widget_ptr) != EG_Widget_Type_VirtualZone){
                EG_Log(EG_LOG_ERROR, dL"EG_VirtualZone Destroy callback called"
                 " for none EG_VirtualZone widget.", dR);
        }else{
                if ( (virtual_zone_ptr= (EG_VirtualZone*) EG_Widget_GetPayload(widget_ptr)) != NULL){

                /* Free the zone array structs.
                 */
		if (virtual_zone_ptr->array_ptr != NULL)
	                EG_Free(virtual_zone_ptr->array_ptr);

		/* Free the EG_VirtualZone payload.
	 	 */
		EG_Free(virtual_zone_ptr);
                }
        }

        /* Free EG_Widget wrapper itself.
         */
        EG_Widget_Free(widget_ptr);
}

static EG_BOOL Callback_Paint(EG_Widget *widget_ptr, SDL_Rect area)
{
//	EG_Button *button_ptr;
	EG_VirtualZone *virtual_zone_ptr;
	EG_Window *window_ptr;

	SDL_Rect loc;
	SDL_Color color;
	SDL_Surface *surface_ptr;

	EG_BOOL bold;

	if ( EG_Shared_GetRenderingDetails(widget_ptr, area,
	 (void*) &virtual_zone_ptr, &window_ptr, &surface_ptr, &color, &loc) != EG_TRUE )
		return(EG_TRUE);

#ifdef EG_DEBUG
	printf("SOMEONE CALLED PAINT FOR '%s' [repaint area (window)="
	 "{%d, %d, %d, %d}:widget area (SDL_Surface)={%d, %d, %d, %d}]\n"
	 , EG_Widget_GetName(widget_ptr), area.x, area.y, area.w, area.h
	 , loc.x, loc.y, loc.w, loc.h);
#endif


        /* Render the widget:
         */


	/* Draw the whole widget:
	 */

	/* Highlight the zone with focus:
	 */





//	/* Dull down the button a bit when disabled.
//	 */
//	if (EG_Widget_IsEnabled(widget_ptr) != EG_TRUE){
//		color.r = (int) ( color.r * 0.9);
//		color.g = (int) ( color.g * 0.9);	
//		color.b = (int) ( color.b * 0.9);
//	}
//
//	/* Fill button area with button color.
//	 */
//	EG_Draw_Box(surface_ptr, &loc, &color);
//
//	/* Draw buttons border, if depressed (pressed), sunk, otherwise high.
//	 */
//	if (button_ptr->depressed == EG_TRUE){
//		EG_Draw_Border(surface_ptr,&loc, &color, EG_Draw_Border_BoxLow);
//		loc.x++; loc.y++;
//	}else{
//		EG_Draw_Border(surface_ptr,&loc,&color, EG_Draw_Border_BoxHigh);
//	}
//
//	/* Draw the caption.
//	 */
//	if (EG_Window_ThisWidgetHasFocus(widget_ptr) == EG_TRUE)
//		bold = EG_TRUE;
//	else
//		bold = EG_FALSE;	
//
//	EG_Draw_String(surface_ptr, &color, bold, &loc, button_ptr->alignment
//	 , button_ptr->caption );
	
	/* Paint succeeded so return true.
	 */
	return(EG_TRUE);
}

static EG_BOOL Callback_SDL_Event(EG_Widget *widget_ptr, SDL_Event *event_ptr)
{
//	EG_Button *button_ptr;
	EG_Window *window_ptr;
	EG_BOOL mouse_over_widget;

	EG_BOOL return_value = EG_FALSE;

	/* Populates variables needed to process the event.  If the event
	 * shouldn't have been passed to us, then logs the fact (for bug
	 * reporting) and returns false.  Quit the callback if this returns
	 * false.
	 */
//	if (EG_Shared_GetEventDetails(widget_ptr, event_ptr
//	 , (void*) &button_ptr, &window_ptr, &mouse_over_widget) != EG_TRUE)
//		return(return_value);

	/* Process SDL Event:
	 */


//	/* Is a mouse button event?
//	 */
//	if (event_ptr->type == SDL_MOUSEBUTTONDOWN || event_ptr->type
//	 == SDL_MOUSEBUTTONUP){
//
//		/* If depressed = false and left mouse button is pressed and
//		 * mouse pointer is within area of widget.
//		 */
//		if (button_ptr->depressed == EG_FALSE && event_ptr->button.state
//		 == SDL_PRESSED && event_ptr->button.button == SDL_BUTTON_LEFT
//		 && mouse_over_widget == EG_TRUE){
//
//			/* Set depressed = true, repaint widget.
//			 */
//			button_ptr->depressed=EG_TRUE;
//			(void) EG_Widget_RepaintLot(widget_ptr);
//		}
//
//		/* If depressed = true and left mouse button is released.
//		 */
//		if (button_ptr->depressed == EG_TRUE && event_ptr->button.state
//		 == SDL_RELEASED && event_ptr->button.button ==SDL_BUTTON_LEFT){
//	
//			/* If released with mouse pointer within area of widget
//			 * and --------the minimum click-time has been met. 
//			 */
//			if (mouse_over_widget == EG_TRUE && 
//			 EG_Widget_GetVisibleToggle(widget_ptr) == EG_TRUE
//			 && EG_Widget_GetStoppedToggle(widget_ptr) == EG_FALSE){
//			 	/* Call users 'OnClick' event.
//				 */
//				if (EG_Window_SetFocusToThisWidget(widget_ptr)
//				 != EG_TRUE)
//				 	EG_Log(EG_LOG_WARNING, dL"Could not"
//					 " move focus to pressed EG_Button."
//					 , dR);
//
//				EG_Widget_CallUserOnClick(widget_ptr);
//			}
//	
//			/* Regardless of mouse pointers location, set
//			 * depressed = false; repaint widget.
//			 */
//			button_ptr->depressed=EG_FALSE;
//			(void) EG_Widget_RepaintLot(widget_ptr);
//		}
//	}
//
//
//	/* If widget has focus and 'Enter' key is pressed.
//	 */
//
//	if (event_ptr->type == SDL_KEYDOWN
//	 && event_ptr->key.keysym.sym == SDLK_RETURN){
//
//		/* If this widget currently has focus on the window.
//		 */
//		if (EG_Window_ThisWidgetHasFocus(widget_ptr) == EG_TRUE){
//			button_ptr->depressed=EG_TRUE;
//			(void) EG_Widget_RepaintLot(widget_ptr);
//		}
//	}
//
//	if (event_ptr->type == SDL_KEYUP
//	 && event_ptr->key.keysym.sym == SDLK_RETURN){
//
//		/* If this widget currently has focus on the window.
//		 */
//		if (EG_Window_ThisWidgetHasFocus(widget_ptr) == EG_TRUE){
//			/* Eat this event, we don't want it passed to 
//			 * other widgets.
//			 */
//			return_value = EG_TRUE;
//
//			EG_Widget_CallUserOnClick(widget_ptr);
//
//			button_ptr->depressed=EG_FALSE;
//			(void) EG_Widget_RepaintLot(widget_ptr);
//
//		}
//	}
//
//	/* If mouse over widget, and users event callback is set, call users
//	 * callback.
//	 */
//	if (mouse_over_widget == EG_TRUE)
//		EG_Widget_CallUserOnEvent(widget_ptr, event_ptr);
//
	return(return_value);
}

/* Private function:
 */

static void InitializePayload(EG_VirtualZone *virtual_zone_ptr, unsigned int zone_count)
//static void InitializePayload(EG_Button *button_ptr)
{
	EG_VirtualZone_Area	*zone_array_ptr;

//	button_ptr->depressed = EG_FALSE;

	virtual_zone_ptr->surface_normal_ptr = NULL;
	virtual_zone_ptr->surface_flipped_ptr = NULL;

	virtual_zone_ptr->depressed = EG_FALSE;
	virtual_zone_ptr->held_down = EG_FALSE;

	virtual_zone_ptr->zone_count = zone_count;
	virtual_zone_ptr->zone_focused = -1;

	virtual_zone_ptr->i_handle_focus = EG_TRUE;

	/* Create the zone array:
	 */
	if ( (zone_array_ptr = (EG_VirtualZone_Area*) (EG_Malloc(sizeof(EG_VirtualZone_Area)*zone_count)) ) == NULL){
		virtual_zone_ptr->array_ptr = NULL;
		EG_Log(EG_LOG_WARNING, dL"Unable to malloc EG_VirtualZone_Area array (not enough RAM).", dR);
        } else {
		virtual_zone_ptr->array_ptr = zone_array_ptr;
	}
}

static void InitializeWidget(EG_Widget *widget_ptr)
//static void InitializeWidget(EG_Widget *widget_ptr, SDL_Color color
// , SDL_Rect dimension, int alignment, const char *caption_ptr)
{
	unsigned int i;

	/* Initialize callbacks:
	 */
	(void) EG_Widget_SetCallback_Destroy(widget_ptr, Callback_Destroy);

	(void) EG_Widget_SetCallback_Paint(widget_ptr, Callback_Paint);

	(void) EG_Widget_SetCallback_SDL_Event(widget_ptr, Callback_SDL_Event);



	(void) EG_Widget_SetCallback_Visible(widget_ptr
	 , EG_Callback_Generic_Visible);

	(void) EG_Widget_SetCallback_Stopped(widget_ptr
	 , EG_Callback_Generic_Stopped);

	(void) EG_Widget_SetCallback_Enabled(widget_ptr
	 , EG_Callback_Generic_Enabled);

	(void) EG_Widget_SetCallback_GotFocus(widget_ptr
	 , EG_Callback_Generic_GotFocus);

	(void) EG_Widget_SetCallback_LostFocus(widget_ptr
	 , EG_Callback_Generic_LostFocus);

	(void) EG_Widget_SetCallback_Attach(widget_ptr
	 , EG_Callback_Generic_Attach);


	/* Initialize state:
	 */
	for (i=0; i<EG_VirtualZone_GetZoneCount(widget_ptr); i++)
		EG_VirtualZone_ClearZone(widget_ptr, i);
}


/* Public functions:
 */

// Set callback - use stock function, but ignore user pointer.

EG_Widget* EG_VirtualZone_Create(const char *name_ptr, unsigned int zone_count)
{
//EG_Widget* EG_VirtualZone_Create(const char *name_ptr, SDL_Color color, int alignment
// , const char *caption_ptr, SDL_Rect dimension)
//{
//	EG_Button *button_ptr;
	EG_VirtualZone *virtual_zone_ptr;
	EG_Widget *widget_ptr;
	void *ptr;

	SHARED__ALLOC_PAYLOAD_STRUCT(ptr, EG_VirtualZone
	 , "Unable to malloc EG_VirtualZone struct");
	virtual_zone_ptr = (EG_VirtualZone*) ptr;        

        SHARED__CREATE_NEW_EG_WIDGET(widget_ptr, name_ptr, EG_Widget_Type_VirtualZone
         , virtual_zone_ptr);
        
        SHARED__ATTACH_PAYLOAD_TO_WIDGET(widget_ptr, virtual_zone_ptr);

	InitializePayload(virtual_zone_ptr, zone_count);
	InitializeWidget(widget_ptr);

	return(widget_ptr);
}


void EG_VirtualZone_SetAllZonesArea(EG_Widget *widget_ptr, SDL_Rect area)
{
	EG_VirtualZone *virtual_zone_ptr;

	EG_VIRTUALZONE_GET_STRUCT_PTR_VOID(widget_ptr, virtual_zone_ptr)
	virtual_zone_ptr->surface_area = area;
}

SDL_Rect EG_VirtualZone_GetAllZonesArea(EG_Widget *widget_ptr)
{
	EG_VirtualZone *virtual_zone_ptr;

	EG_VIRTUALZONE_GET_STRUCT_PTR(widget_ptr, virtual_zone_ptr
	 , ((SDL_Rect) {0,0,0,0}) )
	return virtual_zone_ptr->surface_area;
}

EG_BOOL EG_VirtualZone_UseThisSurfaceAsSource(EG_Widget *widget_ptr, SDL_Surface *surface_ptr, SDL_Rect total_zone_area, SDL_Surface *frame_buffer_format_ptr)
{
	EG_VirtualZone *virtual_zone_ptr;

	EG_VIRTUALZONE_GET_STRUCT_PTR(widget_ptr, virtual_zone_ptr, EG_FALSE);

	/* Make two new surfaces in screen format of the same size as the total zone area.
	 */

	/* Copy one normally.
	 */

	/* Copy the other flipped.
	 */

	return EG_TRUE;
}

unsigned int EG_VirtualZone_GetZoneCount(EG_Widget *widget_ptr)
{
	EG_VirtualZone *virtual_zone_ptr;

	EG_VIRTUALZONE_GET_STRUCT_PTR(widget_ptr, virtual_zone_ptr, 0);

	return virtual_zone_ptr->zone_count;
}

EG_BOOL EG_VirtualZone_SetZone(EG_Widget *widget_ptr, unsigned int zone_index, SDL_Rect normal, SDL_Rect focused, SDL_Rect pressed, SDL_Rect held, void *user_ptr)
{
	EG_VirtualZone *virtual_zone_ptr;

	EG_VIRTUALZONE_GET_STRUCT_PTR(widget_ptr, virtual_zone_ptr, EG_FALSE);

	/* Make sure zone index is within bounds:
	 */
	if (zone_index >= virtual_zone_ptr->zone_count)
		return EG_FALSE;

	/* Set the zone:
	 */
	virtual_zone_ptr->array_ptr[zone_index].active = EG_TRUE;
	virtual_zone_ptr->array_ptr[zone_index].normal = normal;
	virtual_zone_ptr->array_ptr[zone_index].focused = focused;
	virtual_zone_ptr->array_ptr[zone_index].pressed = pressed;
	virtual_zone_ptr->array_ptr[zone_index].held = held;
	virtual_zone_ptr->array_ptr[zone_index].user_ptr = user_ptr;

	return EG_TRUE;
}

void EG_VirtualZone_ClearZone(EG_Widget *widget_ptr, unsigned int zone_index)
{
	EG_VirtualZone *virtual_zone_ptr;

	EG_VIRTUALZONE_GET_STRUCT_PTR_VOID(widget_ptr, virtual_zone_ptr);
	
	/* Make sure zone index is within bounds:
	 */
	if (zone_index >= virtual_zone_ptr->zone_count)
		return;

	/* Clear zone data:
	 */
	virtual_zone_ptr->array_ptr[zone_index].active = EG_FALSE;
	virtual_zone_ptr->array_ptr[zone_index].normal = (SDL_Rect) {0,0,0,0};
	virtual_zone_ptr->array_ptr[zone_index].focused = (SDL_Rect) {0,0,0,0};
	virtual_zone_ptr->array_ptr[zone_index].pressed = (SDL_Rect) {0,0,0,0};
	virtual_zone_ptr->array_ptr[zone_index].held = (SDL_Rect) {0,0,0,0};
	virtual_zone_ptr->array_ptr[zone_index].user_ptr = NULL;
}

EG_BOOL EG_VirtualZone_SetFocusedZone(EG_Widget *widget_ptr, unsigned int zone_index)
{
	EG_VirtualZone *virtual_zone_ptr;

	EG_VIRTUALZONE_GET_STRUCT_PTR(widget_ptr, virtual_zone_ptr, EG_FALSE);

	/* Make sure zone index is within bounds:
	 */
	if (zone_index >= virtual_zone_ptr->zone_count)
		return EG_FALSE;

	virtual_zone_ptr->zone_focused = (int) zone_index;

	/* Repaint the widget:
	 */
	EG_Widget_RepaintLot(widget_ptr);

	return EG_TRUE;
}

/* -1 = none
 */
int EG_VirtualZone_GetFocusedZone(EG_Widget *widget_ptr)
{
	EG_VirtualZone *virtual_zone_ptr;

	EG_VIRTUALZONE_GET_STRUCT_PTR(widget_ptr, virtual_zone_ptr, 0);

	return virtual_zone_ptr->zone_focused;
}

void EG_VirtualZone_ClearFocus(EG_Widget *widget_ptr)
{
	EG_VirtualZone *virtual_zone_ptr;

	EG_VIRTUALZONE_GET_STRUCT_PTR_VOID(widget_ptr, virtual_zone_ptr);

	virtual_zone_ptr->zone_focused = -1;
}

// static New user callback (sets user value on a zone by zone basis rather than the widget itself).
// static New rendering routine that renders the whole widget.
// static New rendering routine that renders just the zone.

// static Handler for new I control focus thing on Window.

