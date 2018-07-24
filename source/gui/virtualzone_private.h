/* START OF virtualzone_private.h -------------------------------------------
 *
 *      Allows a bitmap to represent buttons, useful for virtual keyboards.
 *
 *	---
 *	THIS GUI IS TOTALLY *BROKEN*! PLEASE DO NOT USE IT!
 *	---
 */


#ifndef _EG_VIRTUALZONE_PRIVATE_H_
#define _EG_VIRTUALZONE_PRIVATE_H_


#if HAVE_CONFIG_H
#       include <config.h>
#endif

#include <types.h>

#include <SDL/SDL.h>

#include <window.h>
#include <widget.h>

#include <virtualzone.h>


/* Get EG_VirtualZone struct.
 */

#define EG_VIRTUALZONE_GET_STRUCT_PTR(w, v, r)  \
        if (w == NULL ){ \
                EG_Log(EG_LOG_ERROR, dL"EG_Widget is NULL.", dR); \
                return( r ); \
        } \
        if (EG_Widget_GetType( w ) != EG_Widget_Type_VirtualZone){ \
                EG_Log(EG_LOG_ERROR, dL"EG_Widget payload is not of type EG_VirtualZone.", dR); \
                return( r ); \
        } \
        if ( (v = (EG_VirtualZone*) EG_Widget_GetPayload( w )) == NULL){ \
                EG_Log(EG_LOG_ERROR, dL"EG_Widget payload is NULL.", dR); \
                return( r ); \
        } \
        EG_ASSERT_MALLOC( v );

#define EG_VIRTUALZONE_GET_STRUCT_PTR_VOID(w, v)  \
        if (w == NULL ){ \
                EG_Log(EG_LOG_ERROR, dL"EG_Widget is NULL.", dR); \
                return; \
        } \
        if (EG_Widget_GetType( w ) != EG_Widget_Type_VirtualZone){ \
                EG_Log(EG_LOG_ERROR, dL"EG_Widget payload is not of type EG_VirtualZone.", dR); \
                return; \
        } \
        if ( (v = (EG_VirtualZone*) EG_Widget_GetPayload( w )) == NULL){ \
                EG_Log(EG_LOG_ERROR, dL"EG_Widget payload is NULL.", dR); \
                return; \
        } \
        EG_ASSERT_MALLOC( v );




/* EG_Widget payload:
 */

typedef struct {
	EG_BOOL		active;
	SDL_Rect 	normal, focused, pressed, held;
	void 		*user_ptr;
} EG_VirtualZone_Area;

typedef struct {
	/* The source graphic SDL surface:
	 */
	SDL_Surface 		*surface_normal_ptr, *surface_flipped_ptr;

	/* The area of the source gaphic surface that forms
	 * all virtual zones:
	 */
	SDL_Rect		surface_area;

	/* A button is depressed (i.e.: key down),
	 * or a button is held down (i.e.: toggled shift held down)
	 */
	EG_BOOL 		depressed;
	EG_BOOL 		held_down;	

	/* An array of EG_VirtualZone_Area structs that contain
	 * the zone data:
	 */
	EG_VirtualZone_Area 	*array_ptr;
	unsigned int		zone_count;

	/* The zone (virtual button) that has focus (-1 = none):
	 */
	int 			zone_focused;

	/* Allow EG_Window to handle focus:
	 */
	EG_BOOL 		i_handle_focus;
} EG_VirtualZone;
#define EG_AsVirtualZone(n) ((EG_VirtualZone*) n)


/* Private and friend functions:
 */

/* END OF virtualzone_private.h -----------------------------------------------
 */
#endif                                                                           
