#include <gui/graphics.h>
#include <SDL.h>
#include <math.h>

#define PI 3.14159265358979323846264338327

inline static int rnd( int max )
{
        if (max == 0) return 0;
        return (rand() % max) + 1;
}

static void DrawCircle(SDL_Surface *surface_p, int x, int y, int r)
{
	float t;

	EG_Polygon *p;

	p = EG_Polygon_New();

	for(t=0; t < PI*2; t+=(PI*2/256) ) {
		EG_Polygon_AddEdge( p, x+(((float) r)*cos(t)), y+(((float) r)*sin(t)) );
	}

	EG_Graphics_DrawFilledPolygon(surface_p, p);
	EG_Polygon_Free(p);
}


void ScreenSaver(SDL_Surface *screen_ptr)
{
        SDL_Event event;
        int done = 0;

	SDL_FillRect(screen_ptr, NULL, SDL_MapRGB(screen_ptr->format, 0,0,0));        
 

       while (!done)
        {

                while (SDL_PollEvent(&event)) {
                        switch (event.type)
                        {
				case SDL_KEYUP:
                                case SDL_JOYBUTTONUP:
				case SDL_MOUSEBUTTONUP:
                                case SDL_QUIT:
                                        done=1;
                                        break;
                        }
                }

                /* Draw something:
                 */
                EG_Polygon *polygon_p;
                SDL_Rect r;
                unsigned int i;

                EG_Graphics_SetForeColor(SDL_MapRGB(screen_ptr->format, rnd(255), rnd(255), rnd(255)));
                EG_Graphics_SetBackColor(SDL_MapRGB(screen_ptr->format, rnd(255), rnd(255), rnd(255)));
                EG_Graphics_SetPattern(rnd(EG_PATTERN_COUNT) );

//                switch (rnd(5)) {
//
//                case 1:
//                        r.w = rnd(screen_ptr->w  - 20) + 20;
//                        r.h = rnd(screen_ptr->h - 20) + 20;
//                        r.x = rnd(screen_ptr->w  - r.w);
//                        r.y = rnd(screen_ptr->h  - r.h);
//
//                        EG_Graphics_DrawFilledRect(screen_ptr, r.x, r.y, r.w, r.h);
//                        break;
//                case 2:
//		case 3:
//		case 4:
                        polygon_p = EG_Polygon_New();
                        for (i=0; i<3+ (unsigned int) rnd(10) ; i++)
                                EG_Polygon_AddEdge(polygon_p, rnd(screen_ptr->w), rnd(screen_ptr->h) );

                        EG_Graphics_DrawFilledPolygon(screen_ptr, polygon_p);

                        EG_Polygon_Free(polygon_p);
//                        break;
//                case 5:
//			DrawCircle(screen_ptr, rnd(screen_ptr->w), rnd(screen_ptr->h), rnd(100));
//                        break;
//                }

                SDL_UpdateRect(screen_ptr,0,0,0,0);
                SDL_Delay(100);
        }

        SDL_FillRect(screen_ptr, NULL, SDL_MapRGB(screen_ptr->format, 0,0,0));
	SDL_UpdateRect(screen_ptr,0,0,0,0);
}

