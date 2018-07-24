/* START OF image_private.h ----------------------------------------------------
 *
 *	Simple image widget
 *
 *	---
 *	THIS GUI IS TOTALLY *BROKEN*! PLEASE DO NOT USE IT!
 *	---
 */


#ifndef _DW_IMAGE_PRIVATE_H_
#define _DW_IMAGE_PRIVATE_H_

#if HAVE_CONFIG_H
#       include <config.h>
#endif

#include <types.h>
#include <SDL/SDL.h>

#include <image.h>

#include <window.h>
#include <widget.h>


/* Get EG_Image struct.
 */

#define EG_IMAGE_GET_STRUCT_PTR(w, v, r)  \
	if (w == NULL ){ \
		EG_Log(EG_LOG_ERROR, dL"EG_Widget is NULL.", dR); \
		return( r ); \
	} \
	if (EG_Widget_GetType( w ) != EG_Widget_Type_Image){ \
		EG_Log(EG_LOG_ERROR, dL"EG_Widget payload is not of type EG_Image.", dR); \
		return( r ); \
	} \
	if ( (v = (EG_Image*) EG_Widget_GetPayload( w )) == NULL){ \
                EG_Log(EG_LOG_ERROR, dL"EG_Widget payload is NULL.", dR); \
                return(EG_FALSE); \
        } \
        EG_ASSERT_MALLOC( (void*) v );


/* EG_Widget payload:
 */
typedef struct{
	int alignment;
	SDL_Surface *normal_image_ptr, *flipped_image_ptr;
}EG_Image;
#define EG_AsImage(n) ((EG_Image*) n)


/* Private and Friends Functions:
 */


/* END OF image_private.h ------------------------------------------------------
 */
#endif



