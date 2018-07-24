/* START OF window.h -----------------------------------------------------------
 *
 *      Window API.
 *
 *	---
 *	THIS GUI IS TOTALLY *BROKEN*! PLEASE DO NOT USE IT!
 *	---
 */


#ifndef _DW_WINDOW_H_
#define _DW_WINDOW_H_

#if HAVE_CONFIG_H
#       include <config.h>
#endif

#include <types.h>
#include <SDL/SDL.h>

#include <widget.h>

#include <skin.h>

/* Should not need this until window is a widget (I suck so much...)
 */
#define EG_Window_Type_Window 0xEF619DA3

/* A windows child widget.
 */
typedef struct{ 
        long 		index;
        void 		*widget_ptr;
}EG_Child;
#define EG_AsChild(n) ((EG_Child*) n)

#define EG_WINDOW_MAX_NAME_SIZE                 256
#define EG_WINDOW_MAX_CHILD_WIDGETS             512

/* User SDL Events:
 */

#define EG_USER_SDL_EVENT_POLL			1

#define EG_USER_SDL_EVENT_FOCUS_FORWARD		10
#define EG_USER_SDL_EVENT_FOCUS_BACKWARD	11

#define EG_USER_SDL_EVENT_SMART_FOCUS_L		12
#define EG_USER_SDL_EVENT_SMART_FOCUS_R		13
#define EG_USER_SDL_EVENT_SMART_FOCUS_U		14
#define EG_USER_SDL_EVENT_SMART_FOCUS_D		15


/* Should be handled the same as SDLK_ENTER key down and key up events:
 */
#define EG_USER_SDL_TRIGGER_SELECT_DOWN		16
#define EG_USER_SDL_TRIGGER_SELECT_UP		17



/* Main Window struct, is instance data for each window.
 */
typedef struct{
        /* Window ID
         */
        char            name[EG_WINDOW_MAX_NAME_SIZE+1];
        unsigned long   ID;

        /* Source surface and area within that surface
         */
        SDL_Surface     *surface_ptr;
        SDL_Rect        dimension;

	SDL_Surface	*window_background_surface_ptr;

        /* User value Interface can ask Window to hold
         * (if you write your own Interface then you can
         *  set this value).
         */
        void            *hold_value_ptr;

        SDL_Color	background_color;

        EG_BOOL         hidden;

	EG_BOOL		smart_focus_locked;

        /* Array containing max number of child widgets
         */
	EG_Widget	*widget_ptr[EG_WINDOW_MAX_CHILD_WIDGETS];
        long		count;

	long		widget_with_focus;

	/* Either use a user defined skin for the window, or enable
	 * the optionally set 'default skin'
	 *
	 * Defaults to: NULL, EG_TRUE:
	 */
	EG_Skin_Box	*skin_box_ptr;
	EG_BOOL		skin_window;
}EG_Window;
#define EG_AsWindow(n) ((EG_Window*) n)

/* Public Functions:
 */

#ifdef __cplusplus
extern "C"{
#endif
	EG_Window* 	EG_Window_Create(char *name_ptr, SDL_Surface *surface_ptr, SDL_Color background_color, SDL_Rect dimension);
	void 		EG_Window_Destroy(EG_Window *window_ptr);

	void 		EG_Window_DestroyAllChildWidgets(EG_Window *window_ptr);

	EG_BOOL		EG_Window_SetSkin(EG_Window *window_ptr, EG_Skin_Box *box_ptr);
	EG_BOOL		EG_Window_UseDefaultSkin(EG_Window *window_ptr, EG_BOOL v);

	const char* 	EG_Window_GetName(EG_Window *window_ptr);
	SDL_Surface* 	EG_Window_GetSurface(EG_Window *window_ptr);
	void		EG_Window_SetSurface(EG_Window *window_ptr, SDL_Surface *surface_ptr);

	unsigned long 	EG_Window_GetID(EG_Window *window_ptr);

	SDL_Rect 	EG_Window_GetDimension(EG_Window *window_ptr);
	SDL_Color 	EG_Window_GetBackgroundColor(EG_Window *window_ptr);

	EG_BOOL 	EG_Window_IsHidden(EG_Window *window_ptr);

	EG_BOOL 	EG_Window_Repaint(EG_Window *window_ptr, SDL_Rect area);
	EG_BOOL 	EG_Window_RepaintLot(EG_Window *window_ptr);

	EG_BOOL 	EG_Window_ProcessEvent(EG_Window *window_ptr, SDL_Event *event_ptr, int x_displacement, int y_displacement);


	EG_BOOL 	EG_Window_SetBackgroundColor(EG_Window *window_ptr, SDL_Color color);

	EG_BOOL 	EG_Window_Hide(EG_Window *window_ptr);
	EG_BOOL 	EG_Window_Show(EG_Window *window_ptr);

	void 		EG_Window_ClearBackgroundCache(EG_Window *window_ptr);

	EG_BOOL 	EG_Window_AddWidget(EG_Window *window_ptr, void *widget_ptr);
	EG_BOOL 	EG_Window_RemoveWidget(EG_Window *window_ptr, void *widget_ptr);

	EG_BOOL 	EG_Window_SetFocusToThisWidget(EG_Widget *widget_ptr);

	EG_BOOL		EG_Window_ShowWidget(EG_Widget *widget_ptr);
	EG_BOOL 	EG_Window_HideWidget(EG_Widget *widget_ptr);

	EG_BOOL 	EG_Window_EnableWidget(EG_Widget *widget_ptr);
	EG_BOOL 	EG_Window_DisableWidget(EG_Widget *widget_ptr);

	Uint16 		EG_Window_GetXDisplacement(EG_Window *window_ptr);
	Uint16 		EG_Window_GetYDisplacement(EG_Window *window_ptr);
	Uint16 		EG_Window_GetWidth(EG_Window *window_ptr);
	Uint16 		EG_Window_GetHeight(EG_Window *window_ptr);

	SDL_Rect 	EG_Window_GetWidgetDrawingArea(EG_Window *window_ptr, EG_Widget *widget_ptr);

	EG_BOOL 	EG_Window_ThisWidgetHasFocus(EG_Widget *widget_ptr);

        void            EG_Window_ClearFocus(EG_Window *window_ptr);
        EG_BOOL         EG_Window_MoveFocusForward(EG_Window *window_ptr);
        EG_BOOL         EG_Window_MoveFocusBackward(EG_Window *window_ptr);

        void            EG_Window_MoveSmartFocusLeft(EG_Window *window_ptr);
        void            EG_Window_MoveSmartFocusRight(EG_Window *window_ptr);
        void            EG_Window_MoveSmartFocusUp(EG_Window *window_ptr);
        void            EG_Window_MoveSmartFocusDown(EG_Window *window_ptr);


	/* Global window functions:
	 */
	void		EG_Window_SetDefaultSkin(EG_Skin_Box *box_ptr);
	EG_Skin_Box*	EG_Window_GetDefaultSkin(void);
	void		EG_Window_ClearDefaultSkin();

/* Triggers: Allows alternative triggers to simulate keyboard events:
 */

	/* Return key:
	 */
	void 		EG_Window_Trigger_Select_Down(EG_Window *window_ptr);
	void 		EG_Window_Trigger_Select_Up(EG_Window *window_ptr);

	int		GetSDLUserEventValue1(SDL_Event *event_p);
	int		GetSDLUserEventValue2(SDL_Event *event_p);

	void 		EG_Window_Diagnositc_DumpChildren(EG_Window *window_ptr);

	EG_Window* 	EG_Widget_GetWindowPtr(EG_Widget *widget_ptr);

	EG_BOOL 	EG_Window_IsSmartMovementLocked(EG_Window *window_ptr);
#ifdef __cplusplus
}
#endif


/* END OF window.h -------------------------------------------------------------
 */
#endif

