#include <graphics.h>
#include <graphics_font.h>
#include <SDL/SDL.h>

/* The actual font map that EG uses:
 */
#include <graphics_font_iso_8869.h>


/* New drawing functions, these are ripped off from another project,
 * so the code may look a little strange. Although the original uses
 * an abstration layer and multiple 'drawing context' states, here I
 * only need the one. Hence EG_Graphic_State being badly hardwired
 * all over.
 */



/* SDL_Surface locking:
 */
#define LOCK(s)   {if(SDL_MUSTLOCK(s))(void)SDL_LockSurface(s);}
#define UNLOCK(s) {if(SDL_MUSTLOCK(s))(void)SDL_UnlockSurface(s);}

/* Drawing layer's state, this is hardwired globally as this code
 * is borrowed from another project (where it's totally abstracted),
 * but for BeebEm GP2x and EG1 we only need basic support, so one
 * global state is OK.
 */
typedef struct {
	int	pattern;
	Uint8	pen_color;
	Uint8	paper_color;

	EG_Font	*font_p;
} EG_Graphic_State;

/* Stock patterns:
 */
static Uint8 pattern[EG_PATTERN_COUNT][8] =
{
	/* 00 */ { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF },
	/* 01 */ { 0xFF, 0xFD, 0xFF, 0xDF, 0xFF, 0xFD, 0xFF, 0xDF },
	/* 02 */ { 0xEE, 0xFF, 0xFF, 0xBB, 0xFF, 0xEE, 0xFF, 0xBB },
	/* 03 */ { 0xDD, 0xFF, 0xAA, 0xFF, 0x55, 0xFF, 0xAA, 0xFF },
	/* 04 */ { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
	/* 05 */ { 0x55, 0xAA, 0x55, 0xFF, 0x55, 0xAA, 0x55, 0xFF },
	/* 06 */ { 0x55, 0xAA, 0x55, 0xAA, 0x55, 0xAA, 0x55, 0xAA },
	/* 07 */ { 0x55, 0xAA, 0x55, 0xAA, 0x55, 0xAA, 0x55, 0xAA },
	/* 08 */ { 0x55, 0xAA, 0x55, 0xAA, 0x55, 0xAA, 0x55, 0xAA },
	/* 09 */ { 0x55, 0xA8, 0x55, 0xAA, 0x55, 0x8A, 0x55, 0xAA },
	/* 0A */ { 0x55, 0xA8, 0x55, 0x8A, 0x55, 0xA8, 0x55, 0x8A },
	/* 0B */ { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
	/* 0C */ { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
	/* 0D */ { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
	/* 0E */ { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
	/* 0F */ { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }
};



/* Globals:
 */

/* Our hardwired graphics state:
 */
static EG_Graphic_State state;

/* Locking wrappers
 */
void EG_Graphics_Lock(SDL_Surface *surface_p)
{
	//LOCK(surface_p);
}

void EG_Graphics_Unlock(SDL_Surface *surface_p)
{
	//UNLOCK(surface_p);
}


/* Create polygon struct helpers:
 */
EG_Polygon* EG_Polygon_New(void)
{
        EG_Polygon *poly_p;

        if ( (poly_p=(EG_Polygon*)EG_Malloc(sizeof(EG_Polygon)))==NULL)
                return NULL;

        /* Initialize:
         */
        poly_p->edge_count = 0;
        poly_p->first_edge_p = NULL;
        poly_p->last_edge_p = NULL;

        return poly_p;
}

EG_BOOL EG_Polygon_AddEdge(EG_Polygon *polygon_p, int x, int y)
{
        EG_Polygon_Edge *edge_p;

        if (polygon_p == NULL) return EG_FALSE;

        /* Create the edge:
         */
        if ( (edge_p=(EG_Polygon_Edge*)EG_Malloc(sizeof(EG_Polygon_Edge)))==NULL) {
                return EG_FALSE;
        }

        /* Set its state:
         */
        edge_p->x = x;
        edge_p->y = y;
        edge_p->next_p = NULL;

        /* Attach it to polygon:
         */
        if (polygon_p->first_edge_p == NULL) {
                polygon_p->first_edge_p = edge_p;
                polygon_p->last_edge_p = edge_p;
        } else {
                polygon_p->last_edge_p->next_p = edge_p;
                polygon_p->last_edge_p=edge_p;
        }

        polygon_p->edge_count++;

        return EG_TRUE;
}

void EG_Polygon_Free(EG_Polygon *polygon_p)
{
        EG_Polygon_Edge *edge_p, *edge_next_p;

        if (polygon_p == NULL) return;

        /* Free edges list:
         */
        edge_p = polygon_p->first_edge_p;
        while (edge_p != NULL) {
                edge_next_p = edge_p->next_p;
                EG_Free(edge_p);
                edge_p = edge_next_p;
        }

        /* Free polygon container:
         */
        EG_Free(polygon_p);
}


EG_BOOL EG_Graphics_Init(char *font_path_p)
{
	SDL_Surface *tmp_p;

	state.pattern = 0;
	state.pen_color = 255; //SDL_MapRGB(frame_buffer_p->format, 0xff, 0xff, 0xff);
	state.paper_color = 0; //SDL_MapRGB(frame_buffer_p->format, 0x00, 0x00, 0x00);

	/* Create font:
	 */
        if ( (tmp_p = SDL_LoadBMP(font_path_p)) == NULL) {
                fprintf(stderr, "Could not load font [%s].\n", font_path_p);
                return EG_FALSE;
        } else {
		printf("Building font.\n");
                if ( (state.font_p = EG_Font_Import("ASCII", &font_8859_map, tmp_p, EG_COLOR_BLACK) ) == NULL) {
                        fprintf(stderr, "Could not create font from bitmap.\n");
                        return EG_FALSE;
                }
        }
	SDL_FreeSurface(tmp_p);

	return EG_TRUE;
}

void EG_Graphics_PutString(SDL_Surface *surface_p, int x, int y, char *string_p, int type)
{
	EG_Font_PutString(state.font_p, surface_p, string_p, x, y, type);
}

void EG_Graphics_Free(void)
{
	EG_Font_Destroy(state.font_p);
}

/* The very very slow graphic functions (good enough for now!):
 */
void EG_Graphics_SetPattern(int pattern)
{
        state.pattern = (pattern&(EG_PATTERN_COUNT-1));
}

int EG_Graphics_GetPattern(void)
{
        return state.pattern;
}

void EG_Graphics_SetForeColor(Uint8 color)
{
        state.pen_color = color;
}

Uint8 EG_Graphics_GetForeColor(void)
{
        return state.pen_color;
}

void EG_Graphics_SetBackColor(Uint8 color)
{
        state.paper_color = color;
}

Uint8 EG_Graphics_GetBackColor(void)
{
        return state.paper_color;
}

void EG_Graphics_DrawPoint(SDL_Surface *surface_p, int x, int y)
{
        SDL_Rect clipping;
        Uint8 color;
	Uint8 *buf_p;

//	if (surface_p == NULL) return;

        /* Determine pixel color:
         */
        if ( (pattern[state.pattern
	 & (EG_PATTERN_COUNT-1)][x & 7] & (1<<(y & 7)) ) != 0)
                color = state.pen_color;
        else
                color = state.paper_color;

//        /* Clip to clipping area:
//         */
//        SDL_GetClipRect(surface_p, &clipping);
//        if (x < clipping.x || x >= clipping.x + clipping.w
//            || y < clipping.y || y >= clipping.y + clipping.h)
//                return;

	/* If in 'flipped' mode, reverse frame_buffer:
	 */
	if (EG_IsUpsideDown() == EG_TRUE) {
		y=(surface_p->h-1)-y;
		x=(surface_p->w-1)-x;
	}

//	/* Do nothing if surface isn't 8 bit:
// 	 */
//	if (surface_p->format->BytesPerPixel != 1) {
//		fprintf(stderr, "SDL Surface is not 8 bit, can't draw.\n");
//		return;
//	}

	/* Draw the pixel:
	 */
//	LOCK(surface_p);
        buf_p = (Uint8*) surface_p->pixels + y * surface_p->pitch + x;
        *buf_p = color;
//	UNLOCK(surface_p);
}

void EG_Graphics_DrawFastPoint(SDL_Surface *surface_p, int x, int y)
{
        SDL_Rect clipping;
        Uint32 color;
	Uint8 *buf_p;

//	if (surface_p == NULL) return;

//        /* Determine pixel color:
//         */
        if ( (pattern[state.pattern
	 & (EG_PATTERN_COUNT-1)][x & 7] & (1<<(y & 7)) ) != 0)
                color = state.pen_color;
        else
                color = state.paper_color;

//        /* Clip to clipping area:
//         */
//        SDL_GetClipRect(surface_p, &clipping);
//        if (x < clipping.x || x >= clipping.x + clipping.w
//            || y < clipping.y || y >= clipping.y + clipping.h)
//                return;

	/* If in 'flipped' mode, reverse frame_buffer:
	 */
	if (EG_IsUpsideDown() == EG_TRUE) {
		y=(surface_p->h-1)-y;
		x=(surface_p->w-1)-x;
	}

//	/* Do nothing if surface isn't 8 bit:
// 	 */
//	if (surface_p->format->BytesPerPixel != 1) {
//		fprintf(stderr, "SDL Surface is not 8 bit, can't draw.\n");
//		return;
//	}

	/* Draw the pixel:
	 */
//	LOCK(surface_p);
        buf_p = (Uint8*) surface_p->pixels + y * surface_p->pitch + x;
        *buf_p = state.pen_color;
//	*buf_p=100;
//	UNLOCK(surface_p);
}



unsigned int EG_Graphics_GetPixel(SDL_Surface *surface_p, int x, int y)
{
	if (surface_p == NULL) return 0;

        /* Clip to bitmap:
         */
        if (x<0 || x>=surface_p->w || y<0 || y>=surface_p->h)
                return 0;

//	if (surface_p->format->BytesPerPixel != 1) {
//		fprintf(stderr, "SDL Surface is not 8 bit, can't read.\n");
//		return 0;
//	}

	switch (surface_p->format->BytesPerPixel) {

	case 1:
		{	
			Uint8 *bufp; 
			bufp = (Uint8*) surface_p->pixels + y * surface_p->pitch + x;
			return (unsigned int) *bufp;
		}
	case 2:
		{
			Uint16 *bufp; 
			bufp = (Uint16*) surface_p->pixels + y * surface_p->pitch + x;
			return (unsigned int) *bufp;
		}

	case 3:
		printf("EG_Graphics_GetPixel: 24bit is not supported...\n");
		return 0;

	case 4:
		{
			Uint32 *bufp;
			bufp = (Uint32*) surface_p->pixels + y * surface_p->pitch + x;
			return (unsigned int) *bufp;
		}
	default:
		return 0;
	}
}

void EG_Graphics_DrawLine(SDL_Surface *surface_p, int x0, int y0, int x1, int y1)
{
        int deltax, deltay, error, ystep, y;
        int x, steep = abs(y1 - y0) > abs(x1 - x0);

#       define swap(a, b) { int c = a; a = b; b = c; }

	LOCK(surface_p);

        if (steep) { swap(x0, y0); swap(x1, y1); }
        if (x0 > x1) { swap(x0, x1); swap(y0, y1); }

        deltax = x1 - x0;
        deltay = abs(y1 - y0);
        error = 0;
        y = y0;

        if (y0 < y1) ystep = 1; else ystep = -1;

        for (x = x0; x <= x1; x++) {
                if (steep)
                        EG_Graphics_DrawPoint(surface_p, y, x);
                else
                        EG_Graphics_DrawPoint(surface_p, x, y);

                error += deltay;
                if (error*2 >= deltax) { y+=ystep; error -= deltax; }
        }
	UNLOCK(surface_p);
}

void EG_Graphics_DrawFilledRect(SDL_Surface *surface_p, int x, int y, unsigned int w
 , unsigned int h)
{
        int i, i2;

	LOCK(surface_p);
	if (state.pattern == 0) {
		SDL_Rect r;

		if (EG_IsUpsideDown() == EG_TRUE) {
			y=(surface_p->h)-y-h;
			x=(surface_p->w)-x-w;
		}
		r.x = x; r.y = y; r.w = w; r.h = h;

		SDL_FillRect(surface_p, &r, state.pen_color);

	} else {
		for (i=y; i < y + (int) h; i++)
			for (i2=x; i2 < x + (int) w; i2++)
				EG_Graphics_DrawPoint(surface_p, i2, i);
	}
	UNLOCK(surface_p);
}

void EG_Graphics_DrawBorder(SDL_Surface *surface_p, int x, int y, unsigned int w
 , unsigned int h, int type)
{
	int old_pattern = EG_Graphics_GetPattern();
	Uint8 r,g,b,bright, dull, old_pen = EG_Graphics_GetForeColor();

	LOCK(surface_p);
	EG_Graphics_SetPattern(0);

	switch (type) {
		case EG_GRAPHICS_BORDER_RAISED:
			SDL_GetRGB(old_pen, surface_p->format, &r, &g, &b);

			bright = SDL_MapRGB(surface_p->format
			 , (int) (1.3333*r >255.0 ? 255.0 : 1.3333*r)
			 , (int) (1.3333*g >255.0 ? 255.0 : 1.3333*g)
			 , (int) (1.3333*b >255.0 ? 255.0 : 1.3333*b) );

			dull = SDL_MapRGB(surface_p->format
			 , (int) (r * 0.6666)
			 , (int) (g * 0.6666)
			 , (int) (b * 0.6666) );
			break;

		case EG_GRAPHICS_BORDER_SUNK:
			SDL_GetRGB(old_pen, surface_p->format, &r, &g, &b);

			bright = SDL_MapRGB(surface_p->format
			 , (int) (r * 0.6666)
			 , (int) (g * 0.6666)
			 , (int) (b * 0.6666) );

			dull = SDL_MapRGB(surface_p->format
			 , (int) (1.3333*r >255.0 ? 255.0 : 1.3333*r)
			 , (int) (1.3333*g >255.0 ? 255.0 : 1.3333*g)
			 , (int) (1.3333*b >255.0 ? 255.0 : 1.3333*b) );
			break;

		case EG_GRAPHICS_BORDER_NORMAL:
		default:
			bright = dull = old_pen;
			break;
	}

	EG_Graphics_SetForeColor(bright);
//	EG_Graphics_DrawLine(surface_p, x, y, x    , y+h-1);
//	EG_Graphics_DrawLine(surface_p, x, y, x+w-1, y    );
	EG_Graphics_DrawLine(surface_p, x, y+1, x    , y+h-2);
	EG_Graphics_DrawLine(surface_p, x+1, y, x+w-2, y    );




	EG_Graphics_SetForeColor(dull);
//	EG_Graphics_DrawLine(surface_p, x+w-1, y    , x+w-1, y+h-1);
//	EG_Graphics_DrawLine(surface_p, x    , y+h-1, x+w-1, y+h-1);
	EG_Graphics_DrawLine(surface_p, x+w-1, y+1    , x+w-1, y+h-2);
	EG_Graphics_DrawLine(surface_p, x+1    , y+h-1, x+w-2, y+h-1);


	EG_Graphics_SetPattern(old_pattern);
	EG_Graphics_SetForeColor(old_pen);
	UNLOCK(surface_p);
}

void EG_Graphics_UpdateSurface(SDL_Surface *surface_p, int x, int y
 , unsigned int w, unsigned int h)
{
        return;

        if (EG_IsUpsideDown() == EG_TRUE) {
                x = (surface_p->w-1)-x-w;
                y = (surface_p->h-1)-y-h;

                w+=1; h+=1;
        }

        printf("XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX %d %d %d %d\n", x, y, w, h);

	SDL_UpdateRect(surface_p, x, y, w, h);
}



/* Draw a filled polygon using the odd/even parity scanline algorithm.
 *
 * S	-	-	-	-	-	-	-	-	-
 */

typedef struct {
        int y_min, y_max, x_val, m;
} all_edges;

static void CreateAllEdgesList(all_edges *all_edges_p, EG_Polygon *polygon_p
 /*, EG_DC *dc_p*/)
{
        unsigned int i;
        int m, x, y;
        EG_Polygon_Edge *edge1_p, *edge2_p;

        edge2_p = polygon_p->first_edge_p;
        for(i=0; i < polygon_p->edge_count; i++) {

                /* Calc. next line:
                 */
                edge1_p = edge2_p;
                if (edge1_p->next_p == NULL)
                        edge2_p = polygon_p->first_edge_p;
                else
                        edge2_p = edge1_p->next_p;

                /* Calc. y-min, y_max and x values:
                 */
                if (edge1_p->y >= edge2_p->y) {
                        all_edges_p[i].y_min = edge2_p->y<<12;
                        all_edges_p[i].y_max = edge1_p->y<<12;
                        all_edges_p[i].x_val = edge2_p->x<<12;
                } else {
                        all_edges_p[i].y_min = edge1_p->y<<12;
                        all_edges_p[i].y_max = edge2_p->y<<12;
                        all_edges_p[i].x_val = edge1_p->x<<12;
                }

                /* Calc. m:
                 */
                y = edge2_p->y - edge1_p->y;
                x = edge2_p->x - edge1_p->x;

                if (x == 0) m = (y<<12);
                else m = (y<<12) / x;

                all_edges_p[i].m = m;
        }
}

static void SortGlobalList(all_edges **global_edges_pp, unsigned int global_count)
{
        unsigned int i, o;

        if (global_count == 0) return;

        /* Sort the Global Edge list:
         */
        for(o=0; o<global_count-1; o++)
                for(i=0; i<global_count-1; i++)
                        if
                        (
                        global_edges_pp[i+1]->y_min < global_edges_pp[i]->y_min

                        || ( global_edges_pp[i+1]->y_min == global_edges_pp[i]->y_min
                        && global_edges_pp[i+1]->y_max < global_edges_pp[i]->y_max )

                        || ( global_edges_pp[i+1]->y_min == global_edges_pp[i]->y_min
                        && global_edges_pp[i+1]->y_max == global_edges_pp[i]->y_max
                        && global_edges_pp[i+1]->x_val < global_edges_pp[i]->x_val )
                        ) {
                                all_edges *tmp_p = global_edges_pp[i];
                                global_edges_pp[i] = global_edges_pp[i+1];
                                global_edges_pp[i+1] = tmp_p;
                        }
}

static void SortActiveList(all_edges **active_edges_pp, unsigned int active_count)
{
        unsigned int i, o;

        if (active_count == 0) return;

        /* Sort the Global Edge list:
         */
        for(o=0; o<active_count-1; o++)
                for(i=0; i<active_count-1; i++)
                        if(active_edges_pp[i+1]->x_val<active_edges_pp[i]->x_val) {
                                all_edges *tmp_p = active_edges_pp[i];
                                active_edges_pp[i] = active_edges_pp[i+1];
                                active_edges_pp[i+1] = tmp_p;
                        }
}

static unsigned int CreateGlobalList(all_edges **global_edges_pp
 , all_edges *all_edges_p, EG_Polygon *polygon_p)
{
        unsigned int i, global_count;

        /* Initialize Global Edge list:
         */
        global_count=0;
        for(i=0; i<polygon_p->edge_count; i++)
                if(all_edges_p[i].m != 0)
                        global_edges_pp[global_count++] = &all_edges_p[i];

        SortGlobalList(global_edges_pp, global_count);

        return global_count;
}

#define ODD     0
#define EVEN    1

static void RenderScanlineAndUpdateActiveList(SDL_Surface *surface_p, all_edges **active_edges_pp
 , unsigned int active_count, int scanline)
{
        unsigned int i;
	int x_start, x_end, parity;

        /* Set the initial parity:
         */
        parity = EVEN;
	x_start=x_end=-1;

        /* Loop through entries in the active table:
         */
        for(i=0; i<active_count; i++) {
                if (parity == EVEN) {
                        parity = ODD;
                        x_start = active_edges_pp[i]->x_val >>12;
                } else {
                        parity = EVEN;
                        x_end = active_edges_pp[i]->x_val >>12;

                        EG_Graphics_DrawFilledRect(surface_p, x_start+1, scanline, x_end-x_start, 1);
                }
                active_edges_pp[i]->x_val += (1<<24) / active_edges_pp[i]->m;
        }
}

static unsigned int RemoveSpentEdgesFromActiveList(all_edges **active_edges_pp
 , unsigned int active_count, int scanline)
{
        unsigned int i,r;

        for(r=i=0; i<active_count; i++)
                if ( (active_edges_pp[i]->y_max >>12) != scanline)
                        active_edges_pp[r++] = active_edges_pp[i];

        return r;
}

static unsigned int MoveEdgesFromGlobalListToActiveList(all_edges **active_edges_pp
 , all_edges **global_edges_pp, unsigned int active_count, unsigned int global_count
 , int scanline)
{
        unsigned int i;

        /* The global edge list pointers are NULL'd as they are used. While this
         * would be a bad idea for the active edge list, I think it's OK here, as
         * we never re-sort the global list, and the NULL check is only in this
         * single routine.
         */

        /* Actually, thinking about it, maybe I should optimize this whole
         * poly routine...
         */

        for(i=0; i<global_count; i++)
                if (global_edges_pp[i] != NULL
                 && (global_edges_pp[i]->y_min >>12) == scanline ) {
                        active_edges_pp[active_count++] = global_edges_pp[i];
                        global_edges_pp[i]=NULL;
                }

        return active_count;
}

void EG_Graphics_DrawFilledPolygon(SDL_Surface *surface_p, EG_Polygon *polygon_p)
{
        int scanline;
        unsigned int global_count, active_count;
        all_edges *all_edges_p, **global_edges_pp, **active_edges_pp;

        if (surface_p == NULL) return;
        if (polygon_p == NULL) return;
        if (polygon_p->edge_count < 3) return;

        /* Allocate space for three arrays. All arrays have [edge_count] entries.
         *
         * Space allocated is split into the following arrays in the following order:
         *
         * 1) all_edges_p:     Array of 'all_edges' structs.
         * 2) global_edges_pp: Array of pointers to an 'all_edges' struct.
         * 3) active_edges_pp: Array of pointers to an 'all_edges' struct.
         */
        if ( (all_edges_p=(all_edges*)EG_Malloc(
         sizeof(all_edges)*(polygon_p->edge_count)
         + ( sizeof(all_edges*)*polygon_p->edge_count*2 )
         ) ) == NULL) {
                // [TODO] log error
                return;
        }
        global_edges_pp = (all_edges**) (all_edges_p + polygon_p->edge_count);
        active_edges_pp = global_edges_pp + polygon_p->edge_count;

        /* Create a list of all Edges from the polygon edges list:
         */
        CreateAllEdgesList(all_edges_p, polygon_p /*, dc_p*/);

        /* Create a sorted list of all global edges we will render from the edge list
         * (everything sorted with vectors of m (slope) equals 0 removed.):
         */
        global_count = CreateGlobalList(global_edges_pp, all_edges_p, polygon_p);

        /* Set the initial scan-line to the lowest line of the polygon:
         */
        scanline = global_edges_pp[0]->y_min >>12;

        /* Initialize the Active Edge list for the first time (active_count=0):
         */
        active_count=MoveEdgesFromGlobalListToActiveList(active_edges_pp
         , global_edges_pp, 0, global_count, scanline);

        /* Render the polygon using the edges in the active list (will change as
         * scan-line increases):
         */
        while (active_count > 0) {
                active_count=RemoveSpentEdgesFromActiveList(active_edges_pp
                 , active_count, scanline);

                active_count=MoveEdgesFromGlobalListToActiveList(active_edges_pp
                 , global_edges_pp, active_count, global_count, scanline);

                SortActiveList(active_edges_pp, active_count);

                RenderScanlineAndUpdateActiveList(surface_p, active_edges_pp, active_count
                 , scanline);

                scanline++;
        }

        /* Free allocated resources:
         */
        EG_Free(all_edges_p);
}

/*
 * E	-	-	-	-	-	-	-	-	-
 */
