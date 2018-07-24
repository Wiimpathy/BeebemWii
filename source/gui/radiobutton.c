/* START OF tickbox.c ----------------------------------------------------------
 *
 *	A Radio Button (is actually the TickBox really).
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

#include <radiobutton.h>
#include <radiobutton_private.h>

#include <window.h>
#include <window_private.h>

#include <widget.h>
#include <widget_private.h>

#include <SDL/SDL.h>

EG_Widget* EG_RadioButton_Create(const char *name_ptr, SDL_Color color
 , const char *caption_ptr, SDL_Rect dimension)
{
	EG_Widget *widget_ptr;

	if ( (widget_ptr = EG_TickBox_Create(name_ptr, color, caption_ptr
	 , dimension)) != NULL)
		EG_TickBox_SetIcons(widget_ptr, 13, 12);

	return(widget_ptr);
}



