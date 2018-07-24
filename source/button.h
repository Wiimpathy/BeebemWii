#ifndef BUTTON_HEADER
#define BUTTON_HEADER

/* The User Event added to SDL Events. Use this to pickout Button events:
 */
#define SDL_USEREVENT_BUTTON				100

#define SDL_USEREVENT_BUTTON_LEFT			1
#define SDL_USEREVENT_BUTTON_RIGHT			2
#define SDL_USEREVENT_BUTTON_UP				3
#define SDL_USEREVENT_BUTTON_DOWN			4

#define SDL_USEREVENT_BUTTON_CLICK			5

#define SDL_USEREVENT_BUTTON_QUIT			6

#define SDL_USEREVENT_BUTTON_CLICK_DOWN			10
#define SDL_USEREVENT_BUTTON_CLICK_UP			11

#include <SDL.h>

#ifdef __cplusplus
extern "C" {
#endif
	void 		Button_Init(void);
	void		Button_Quit(void);
	void 		Button_TranslateEvent(SDL_Event *event_p, int is_flipped);
	void		Button_RaiseAutoRepeatEvents(void);
	void 		Button_Reset(void);
	void		Button_SetAutoRepeatDelay(int new_delay);
	int 		Button_GetAutoRepeatDelay(void);
#ifdef __cplusplus
}
#endif


#endif
