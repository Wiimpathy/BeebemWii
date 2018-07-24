/* START OF gui.h ---------------------------------------------------------------
 *
 *	User include file for EG.
 *
 *	---
 *	THIS GUI IS TOTALLY *BROKEN*! PLEASE DO NOT USE IT!
 *	---
 */

#include <gui/types.h>

#include <gui/window.h>
#include <gui/widget.h>
#include <gui/box.h>
#include <gui/label.h>
#include <gui/button.h>
#include <gui/tickbox.h>
#include <gui/radiogroup.h>
#include <gui/radiobutton.h>
#include <gui/slidebar.h>
#include <gui/togglebutton.h>
#include <gui/progressbar.h>
#include <gui/image.h>

#include <gui/skin.h>

#include <gui/sdl.h>
#include <gui/graphics.h>


#include <SDL.h>


/* Helper macros:
 */

/* Some macros to calculate coordinates:
 * --------------------------------------
 */

/* Horizontally splits an SDL_Rect into 'column_count' cells and returns a new
 * SDL_Rect representing the actual cell 'column'.
 *
 * Vertically you specify a 'row' and a 'row_count' in half characters+1, 1 is
 * subtracted from the total 'row_count' (makes most widgets look nice). So one
 * row = 9-1=8 pixels, two rows = 9*2-1=17. (Works quite well!)
 *
 * (I'm trying to get rid of the -> '+ (column_count>1?1:0)', but don't know
 *  how.)
 */
#       define CELL(rect, row, column, row_count, column_count, spacer) \
        ( (SDL_Rect) { \
        rect.x + (spacer) + ( ((column)-1) * (spacer) ) + ( ( ( rect.w \
        - ((spacer)*2) - ( ((column_count)-1) * (spacer)) ) ) \
        * ( ( ((column)-1) <<12 ) / (column_count) ) >>12 ) \
        , rect.y + (row) * 5 \
        , ( ( rect.w - ((spacer) * 2) - ( ((column_count) - 1) * (spacer)) ) \
        / (column_count) ) + ((column_count)>1?1:0) \
        , ((row_count)*5)-1 \
        } )

/* Returns a subset of a rect:
 */
#       define SUBSET(rect, xd, yd, x2d, y2d) \
        ( (SDL_Rect) { \
        rect.x +=(xd), rect.y +=(yd), rect.w +=(x2d), rect.h +=(y2d) \
        } )

/* Returns the right most 'percentage' of an SDL_Rect, where 'percentage' is
 * a value between 0 and 1.
 *
 * (Say you have a full text row on the window and want the first 40% to be a
 * label and the remaining 60% to be five cells holding toggle button widgets,
 * use this with the C_n_n definitions below to split up the SDL_Rect easily.)
 */
#       define RIGHT(rect, percentage) ( (SDL_Rect) { \
         rect.x + (int) (rect.w * (1-(percentage)) ) \
         , rect.y \
         , rect.w - (int) (rect.w * (1-(percentage)) ) \
         , rect.h } )



/* Returns the left most 'percentage' of an SDL_Rect, where 'percentage' is
 * a value between 0 and 1.
 */
#       define LEFT(rect, percentage) ( (SDL_Rect) { \
          rect.x \
        , rect.y \
        , rect.w * (percentage) \
        , rect.h } )

/* Convenient macros for positioning widgets around the page/tab:
 *
 * Note: coordinates are NOT relative to a tab (parent widget), they are
 * always absolute (relative to the widgets parent window).
 *
 * Sucks I know but you can work around it (until EG2 is finished) by using the
 * definitions below to calculate a 'cell' within the area you want, then use
 * the SDL_Rect returned as the source for 'sub-cells' etc..
 *
 * They look a bit repetitive, but it's easier to use them (even with the
 * duplicatation) rather than CELL directly.  The first numeric is the column,
 * the second the range.
 *
 * (SPACER is obviously the space in pixels between cells.)
 *
 * (The results do look quite nice, and they are really easy to use, beats
 * calc'n coordinates by hand!)
 */

#       define SPACER   2

#       define C_1_1(rect, r, r_c) CELL(rect, r, 1, r_c, 1, SPACER)

#       define C_1_2(rect, r, r_c) CELL(rect, r, 1, r_c, 2, SPACER)
#       define C_2_2(rect, r, r_c) CELL(rect, r, 2, r_c, 2, SPACER)

#       define C_1_3(rect, r, r_c) CELL(rect, r, 1, r_c, 3, SPACER)
#       define C_2_3(rect, r, r_c) CELL(rect, r, 2, r_c, 3, SPACER)
#       define C_3_3(rect, r, r_c) CELL(rect, r, 3, r_c, 3, SPACER)

#       define C_1_4(rect, r, r_c) CELL(rect, r, 1, r_c, 4, SPACER)
#       define C_2_4(rect, r, r_c) CELL(rect, r, 2, r_c, 4, SPACER)
#       define C_3_4(rect, r, r_c) CELL(rect, r, 3, r_c, 4, SPACER)
#       define C_4_4(rect, r, r_c) CELL(rect, r, 4, r_c, 4, SPACER)

#       define C_1_5(rect, r, r_c) CELL(rect, r, 1, r_c, 5, SPACER)
#       define C_2_5(rect, r, r_c) CELL(rect, r, 2, r_c, 5, SPACER)
#       define C_3_5(rect, r, r_c) CELL(rect, r, 3, r_c, 5, SPACER)
#       define C_4_5(rect, r, r_c) CELL(rect, r, 4, r_c, 5, SPACER)
#       define C_5_5(rect, r, r_c) CELL(rect, r, 5, r_c, 5, SPACER)

#       define C_1_6(rect, r, r_c) CELL(rect, r, 1, r_c, 6, SPACER)
#       define C_2_6(rect, r, r_c) CELL(rect, r, 2, r_c, 6, SPACER)
#       define C_3_6(rect, r, r_c) CELL(rect, r, 3, r_c, 6, SPACER)
#       define C_4_6(rect, r, r_c) CELL(rect, r, 4, r_c, 6, SPACER)
#       define C_5_6(rect, r, r_c) CELL(rect, r, 5, r_c, 6, SPACER)
#       define C_6_6(rect, r, r_c) CELL(rect, r, 6, r_c, 6, SPACER)

#       define C_1_7(rect, r, r_c) CELL(rect, r, 1, r_c, 7, SPACER)
#       define C_2_7(rect, r, r_c) CELL(rect, r, 2, r_c, 7, SPACER)
#       define C_3_7(rect, r, r_c) CELL(rect, r, 3, r_c, 7, SPACER)
#       define C_4_7(rect, r, r_c) CELL(rect, r, 4, r_c, 7, SPACER)
#       define C_5_7(rect, r, r_c) CELL(rect, r, 5, r_c, 7, SPACER)
#       define C_6_7(rect, r, r_c) CELL(rect, r, 6, r_c, 7, SPACER)
#       define C_7_7(rect, r, r_c) CELL(rect, r, 7, r_c, 7, SPACER)

#       define C_1_8(rect, r, r_c) CELL(rect, r, 1, r_c, 8, SPACER)
#       define C_2_8(rect, r, r_c) CELL(rect, r, 2, r_c, 8, SPACER)
#       define C_3_8(rect, r, r_c) CELL(rect, r, 3, r_c, 8, SPACER)
#       define C_4_8(rect, r, r_c) CELL(rect, r, 4, r_c, 8, SPACER)
#       define C_5_8(rect, r, r_c) CELL(rect, r, 5, r_c, 8, SPACER)
#       define C_6_8(rect, r, r_c) CELL(rect, r, 6, r_c, 8, SPACER)
#       define C_7_8(rect, r, r_c) CELL(rect, r, 7, r_c, 8, SPACER)
#       define C_8_8(rect, r, r_c) CELL(rect, r, 8, r_c, 8, SPACER)

#       define C_1_9(rect, r, r_c) CELL(rect, r, 1, r_c, 9, SPACER)
#       define C_2_9(rect, r, r_c) CELL(rect, r, 2, r_c, 9, SPACER)
#       define C_3_9(rect, r, r_c) CELL(rect, r, 3, r_c, 9, SPACER)
#       define C_4_9(rect, r, r_c) CELL(rect, r, 4, r_c, 9, SPACER)
#       define C_5_9(rect, r, r_c) CELL(rect, r, 5, r_c, 9, SPACER)
#       define C_6_9(rect, r, r_c) CELL(rect, r, 6, r_c, 9, SPACER)
#       define C_7_9(rect, r, r_c) CELL(rect, r, 7, r_c, 9, SPACER)
#       define C_8_9(rect, r, r_c) CELL(rect, r, 8, r_c, 9, SPACER)
#       define C_9_9(rect, r, r_c) CELL(rect, r, 9, r_c, 9, SPACER)

#       define C_1_10(rect, r, r_c) CELL(rect, r, 1, r_c, 10, SPACER)
#       define C_2_10(rect, r, r_c) CELL(rect, r, 2, r_c, 10, SPACER)
#       define C_3_10(rect, r, r_c) CELL(rect, r, 3, r_c, 10, SPACER)
#       define C_4_10(rect, r, r_c) CELL(rect, r, 4, r_c, 10, SPACER)
#       define C_5_10(rect, r, r_c) CELL(rect, r, 5, r_c, 10, SPACER)
#       define C_6_10(rect, r, r_c) CELL(rect, r, 6, r_c, 10, SPACER)
#       define C_7_10(rect, r, r_c) CELL(rect, r, 7, r_c, 10, SPACER)
#       define C_8_10(rect, r, r_c) CELL(rect, r, 8, r_c, 10, SPACER)
#       define C_9_10(rect, r, r_c) CELL(rect, r, 9, r_c, 10, SPACER)
#       define C_10_10(rect, r, r_c) CELL(rect, r, 10, r_c, 10, SPACER)

#       define C_1_11(rect, r, r_c) CELL(rect, r, 1, r_c, 11, SPACER)
#       define C_2_11(rect, r, r_c) CELL(rect, r, 2, r_c, 11, SPACER)
#       define C_3_11(rect, r, r_c) CELL(rect, r, 3, r_c, 11, SPACER)
#       define C_4_11(rect, r, r_c) CELL(rect, r, 4, r_c, 11, SPACER)
#       define C_5_11(rect, r, r_c) CELL(rect, r, 5, r_c, 11, SPACER)
#       define C_6_11(rect, r, r_c) CELL(rect, r, 6, r_c, 11, SPACER)
#       define C_7_11(rect, r, r_c) CELL(rect, r, 7, r_c, 11, SPACER)
#       define C_8_11(rect, r, r_c) CELL(rect, r, 8, r_c, 11, SPACER)
#       define C_9_11(rect, r, r_c) CELL(rect, r, 9, r_c, 11, SPACER)
#       define C_10_11(rect, r, r_c) CELL(rect, r, 10, r_c, 11, SPACER)
#       define C_11_11(rect, r, r_c) CELL(rect, r, 11, r_c, 11, SPACER)

