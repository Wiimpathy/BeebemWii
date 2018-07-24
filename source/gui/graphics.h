#ifndef _EG_GRAPHICS_H_
#define _EG_GRAPHICS_H_


#include <SDL/SDL.h>
#include <types.h>
#include <functions.h>
#include <graphics_colors.h>

#include <graphics_font.h>

#define EG_PATTERN_COUNT 16



#define EG_GRAPHICS_BORDER_NORMAL	0
#define EG_GRAPHICS_BORDER_RAISED	1
#define EG_GRAPHICS_BORDER_SUNK		2


/* A Polygon edge:
 */
struct _EG_Polygon_Edge {
	int	x, y;
        struct _EG_Polygon_Edge *next_p;
};
typedef struct _EG_Polygon_Edge EG_Polygon_Edge;


/* The polygon struct:
 */
typedef struct {
	unsigned int	edge_count;

	EG_Polygon_Edge	*first_edge_p;
	EG_Polygon_Edge	*last_edge_p;
} EG_Polygon;


/* API:
 */
#ifdef __cplusplus
extern "C"{
#endif

EG_Polygon* 	EG_Polygon_New(void);
EG_BOOL 	EG_Polygon_AddEdge(EG_Polygon *polygon_p, int x, int y);
void		EG_Polygon_Free(EG_Polygon *polygon_p);


EG_BOOL	 	EG_Graphics_Init(char *font_path_p);
void 		EG_Graphics_Free(void);

void 		EG_Graphics_PutString(SDL_Surface *surface_p, int x, int y, char *string_p, int type);

void 		EG_Graphics_SetPattern(int pattern);
int 		EG_Graphics_GetPattern(void);
void 		EG_Graphics_SetForeColor(Uint8 color);
Uint8 		EG_Graphics_GetForeColor(void);
void 		EG_Graphics_SetBackColor(Uint8 color);
Uint8 		EG_Graphics_GetBackColor(void);

void 		EG_Graphics_DrawPoint(SDL_Surface *surface_p, int x, int y);
void EG_Graphics_DrawFastPoint(SDL_Surface *surface_p, int x, int y);
unsigned int 	EG_Graphics_GetPixel(SDL_Surface *surface_p, int x, int y);

void 		EG_Graphics_DrawLine(SDL_Surface *surface_p, int x0, int y0, int x1, int y1);

void 		EG_Graphics_DrawFilledRect(SDL_Surface *surface_p, int x, int y, unsigned int w, unsigned int h);
void 		EG_Graphics_DrawFilledPolygon(SDL_Surface *surface_p, EG_Polygon *polygon_p);

void 	EG_Graphics_UpdateSurface(SDL_Surface *surface_p, int x, int y, unsigned int w, unsigned int h);
void 		EG_Graphics_DrawBorder(SDL_Surface *surface_p, int x, int y, unsigned int w, unsigned int h, int type);

void EG_Graphics_Lock(SDL_Surface *surface_p);
void EG_Graphics_Unlock(SDL_Surface *surface_p);

#ifdef __cplusplus
}
#endif



#endif
