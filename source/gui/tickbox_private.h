/* START OF tickbox_private.h --------------------------------------------------
 *
 *	A simple tickbox widget.
 *
 *	---
 *	THIS GUI IS TOTALLY *BROKEN*! PLEASE DO NOT USE IT!
 *	---
 */


#ifndef _DW_TICKBOX_PRIVATE_H_
#define _DW_TICKBOX_PRIVATE_H_


#if HAVE_CONFIG_H
#       include <config.h>
#endif

#include <types.h>

#include <window.h>
#include <widget.h>

#include <tickbox.h>

#include <SDL/SDL.h>


/* Get EG_TickBox struct.
 */

#define EG_TICKBOX_GET_STRUCT_PTR(w, v, r)  \
	if (w == NULL ){ \
		EG_Log(EG_LOG_ERROR, dL"EG_Widget is NULL.", dR); \
		return( r ); \
	} \
	if (EG_Widget_GetType( w ) != EG_Widget_Type_TickBox){ \
		EG_Log(EG_LOG_ERROR, dL"EG_Widget payload is not of type EG_TickBox.", dR); \
		return( r ); \
	} \
	if ( (v = (EG_TickBox*) EG_Widget_GetPayload( w )) == NULL){ \
		EG_Log(EG_LOG_ERROR, dL"EG_Widget payload is NULL.", dR); \
		return(EG_FALSE); \
	} \
	EG_ASSERT_MALLOC( v );


/* EG_Widget payload:
 */

typedef struct{
	char caption[MAX_TICKBOX_CAPTIONSIZE+1];

	EG_Widget *parent_group_widget;

	char set_value;
	char unset_value;

	EG_BOOL is_ticked;

	EG_BOOL depressed;

        /* Either use a user defined skin for the button, or enable
         * the optionally set 'default skin'
         *
         * Defaults to: NULL, EG_TRUE:
         */
        EG_Skin_Button *skin_ptr;
        EG_BOOL         skin;
}EG_TickBox;
#define EG_AsTickBox(n) ((EG_TickBox*) n)


/* Private and friend functions:
 */

#ifdef __cplusplus
extern "C"{
#endif
//	EG_BOOL 	EG_TickBox_SetIcons(EG_Widget *widget_ptr, char set_icon, char unset_icon);
	EG_BOOL 	EG_TickBox_SetGroupParent(EG_Widget *tickbox_widget_ptr, EG_Widget *group_parent_widget_ptr);
#ifdef __cplusplus
}
#endif


/* END OF tickbox_private.h ----------------------------------------------------
 */
#endif



