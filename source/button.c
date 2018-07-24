#include <string.h>
#include <stdint.h>
#include <SDL.h>
#include "button.h"
#include "keyboard.h"


#define BUTTON_COUNT			10


#define BUTTON_KEY_LEFT			0
#define BUTTON_KEY_RIGHT		1
#define BUTTON_KEY_UP			2
#define BUTTON_KEY_DOWN			3
#define BUTTON_KEY_CLICK		4

#define BUTTON_BUT_LEFT			5
#define BUTTON_BUT_RIGHT		6
#define BUTTON_BUT_UP			7
#define BUTTON_BUT_DOWN			8
#define BUTTON_BUT_CLICK		9


typedef struct {
	int button, state, delay;
} Button_Button;


static Button_Button button[BUTTON_COUNT];
static int auto_repeat_delay;

void Button_Init(void)
{
	auto_repeat_delay = 50;
	Button_Reset();
}

void Button_Quit(void)
{
	// Nothing.
}

void Button_Reset(void)
{
	memset(&button, 0, sizeof(Button_Button) * BUTTON_COUNT);

	button[BUTTON_KEY_LEFT].button = 	SDL_USEREVENT_BUTTON_LEFT;
	button[BUTTON_KEY_RIGHT].button = 	SDL_USEREVENT_BUTTON_RIGHT;
	button[BUTTON_KEY_UP].button = 		SDL_USEREVENT_BUTTON_UP;
	button[BUTTON_KEY_DOWN].button = 	SDL_USEREVENT_BUTTON_DOWN;
	button[BUTTON_KEY_CLICK].button = 	SDL_USEREVENT_BUTTON_CLICK;
	
	button[BUTTON_BUT_LEFT].button = 	SDL_USEREVENT_BUTTON_LEFT;
	button[BUTTON_BUT_RIGHT].button = 	SDL_USEREVENT_BUTTON_RIGHT;
	button[BUTTON_BUT_UP].button = 		SDL_USEREVENT_BUTTON_UP;
	button[BUTTON_BUT_DOWN].button = 	SDL_USEREVENT_BUTTON_DOWN;
	button[BUTTON_BUT_CLICK].button = 	SDL_USEREVENT_BUTTON_CLICK;
}


static void RaiseButtonEvent(int button)
{
	SDL_Event event;

//	printf("RAISING EVENT %d\n", button);

        event.type = SDL_USEREVENT;
        event.user.code = SDL_USEREVENT_BUTTON;

        event.user.data1 = (void*) ((intptr_t) button);

	SDL_PushEvent(&event);
}

static void ButtonDown(int index)
{
//	printf("button down index=%d button=%d\n", index, button[index].button);
	button[index].state = 1;
        button[index].delay = auto_repeat_delay;
        RaiseButtonEvent(button[index].button);
}

static void ButtonDown_NoRepeat(int index)
{
//	printf("button down norepeat index=%d button=%d\n", index, button[index].button);
	button[index].state = 1;
	button[index].delay = 0;
	RaiseButtonEvent(button[index].button);
}

static void ButtonUp(int index)
{
//	printf("button up index=%d button=%d\n", index, button[index].button);
	button[index].state = 0;
}


void Button_SetAutoRepeatDelay(int new_delay)
{
	auto_repeat_delay = new_delay;
}

int Button_GetAutoRepeatDelay(void)
{
	return auto_repeat_delay;
}

void Button_RaiseAutoRepeatEvents(void)
{
	int i;

	for (i=0; i<BUTTON_COUNT; i++)
		if (button[i].state == 1 && button[i].delay > 0)
			if (--button[i].delay == 0) {
				button[i].delay = auto_repeat_delay;

				RaiseButtonEvent(button[i].button);
			}
}

static void do_fake_key(SDLKey key, Uint8 pushed)
{
	SDL_Event fake_key_event;

	fake_key_event.key.keysym.unicode = 0;
	fake_key_event.key.keysym.sym = key;
	if (!pushed)
	{
		fake_key_event.type = SDL_KEYUP;
		fake_key_event.key.state = SDL_RELEASED;
	}
	else
	{
		fake_key_event.type = SDL_KEYDOWN;
		fake_key_event.key.state = SDL_PRESSED;
	}
	SDL_PushEvent(&fake_key_event);
}


#include <unistd.h>
#include <gccore.h>
#include <ogcsys.h>

static void pushKeyboardEvent( SDLKey key, bool pressed )
{
  SDL_Event event;
  event.type = pressed ? SDL_KEYDOWN : SDL_KEYUP;
  event.key.state = pressed ? SDL_PRESSED : SDL_RELEASED;
  event.key.keysym.scancode = 0;
  event.key.keysym.sym = key;
  event.key.keysym.unicode = 0;
  SDL_PushEvent( &event );
}


void Button_TranslateEvent(SDL_Event *event_p, int is_flipped)
{
	static int gp2x_button_down = 0;
	static int gp2x_button_up = 0;

	static int previous_is_flipped = 0;

#ifdef GEKKO
	Uint8 oldhat = 0;
#endif

	if (previous_is_flipped != is_flipped) {
		gp2x_button_down = gp2x_button_up = 0;
		previous_is_flipped = is_flipped;
	}
	

	/* If we have an SDL event then process it:
	 */
	if (event_p == NULL) return;

	/* Term:
	 */
	if (event_p->type == SDL_QUIT) {
//		printf("is quit\n");
		RaiseButtonEvent(SDL_USEREVENT_BUTTON_QUIT);
		return;

	/* Keyboard:
	 */
	} else if (event_p->type == SDL_KEYDOWN) {
		// QUIT:
		if ( event_p->key.keysym.sym == SDLK_ESCAPE) {
//			printf("is quit\n");
			RaiseButtonEvent(SDL_USEREVENT_BUTTON_QUIT);
		} else

		// LEFT:
		if ( (! is_flipped && event_p->key.keysym.sym == SDLK_LEFT)
			|| (is_flipped && event_p->key.keysym.sym == SDLK_RIGHT)
		   ) {
//			printf("calling left %d\n", BUTTON_KEY_LEFT);
			ButtonDown(BUTTON_KEY_LEFT);
		} else
	
		// RIGHT:
		if ( (! is_flipped && event_p->key.keysym.sym == SDLK_RIGHT)
			|| (is_flipped && event_p->key.keysym.sym == SDLK_LEFT)
		   ) {
//			printf("calling right %d\n", BUTTON_KEY_RIGHT);
			ButtonDown(BUTTON_KEY_RIGHT);
		} else
	
		// UP:
		if ( (! is_flipped && event_p->key.keysym.sym == SDLK_UP)
			|| (is_flipped && event_p->key.keysym.sym == SDLK_DOWN)
		   ) {
			ButtonDown(BUTTON_KEY_UP);
		} else

		// DOWN:
		if ( (! is_flipped && event_p->key.keysym.sym == SDLK_DOWN)
			|| (is_flipped && event_p->key.keysym.sym == SDLK_UP)
		   ) {
			ButtonDown(BUTTON_KEY_DOWN);
		} else

		// CLICK:
		if ( event_p->key.keysym.sym == SDLK_RETURN) {
			ButtonDown_NoRepeat(BUTTON_KEY_CLICK);
			RaiseButtonEvent(SDL_USEREVENT_BUTTON_CLICK_DOWN);
		}
	
	} else if (event_p->type == SDL_KEYUP) {

		// LEFT:
		if ( (! is_flipped && event_p->key.keysym.sym == SDLK_LEFT)
			|| (is_flipped && event_p->key.keysym.sym == SDLK_RIGHT)
		   ) {
			ButtonUp(BUTTON_KEY_LEFT);
		} else
	
		// RIGHT:
		if ( (! is_flipped && event_p->key.keysym.sym == SDLK_RIGHT)
			|| (is_flipped && event_p->key.keysym.sym == SDLK_LEFT)
		   ) {
			ButtonUp(BUTTON_KEY_RIGHT);
		} else
	
		// UP:
		if ( (! is_flipped && event_p->key.keysym.sym == SDLK_UP)
			|| (is_flipped && event_p->key.keysym.sym == SDLK_DOWN)
		   ) {
			ButtonUp(BUTTON_KEY_UP);
		} else
	
		// DOWN:
		if ( (! is_flipped && event_p->key.keysym.sym == SDLK_DOWN)
			|| (is_flipped && event_p->key.keysym.sym == SDLK_UP)
		   ) {
			ButtonUp(BUTTON_KEY_DOWN);
		} else

		// CLICK:
		if ( event_p->key.keysym.sym == SDLK_RETURN) {
//			printf("***** CLICK\n");
			ButtonUp(BUTTON_KEY_CLICK);
			RaiseButtonEvent(SDL_USEREVENT_BUTTON_CLICK_UP);
		}
	}
#ifdef GEKKO
	// WII Buttons:
	else if (event_p->type == SDL_JOYHATMOTION) {

		pushKeyboardEvent( SDLK_UP, false );
		pushKeyboardEvent( SDLK_DOWN, false ); 
		pushKeyboardEvent( SDLK_LEFT, false);
		pushKeyboardEvent( SDLK_RIGHT, false );

		if (event_p->jhat.value&SDL_HAT_UP)
			pushKeyboardEvent( SDLK_UP, true );
		else if(event_p->jhat.value&SDL_HAT_DOWN)
			pushKeyboardEvent( SDLK_DOWN, true );
		else if(event_p->jhat.value&SDL_HAT_LEFT)
			pushKeyboardEvent( SDLK_LEFT, true );
		else if(event_p->jhat.value&SDL_HAT_RIGHT)
			pushKeyboardEvent( SDLK_RIGHT, true );
	} else if (event_p->type == SDL_JOYBUTTONDOWN) {

		if (event_p->jbutton.button
		 == WIIGC1_BUTTON_A || event_p->jbutton.button
		 == WIIGC1_BUTTON_X || event_p->jbutton.button
		 == WIIGC1_BUTTON_Y || event_p->jbutton.button
		 == CLASSIC1_BUTTON_A)  {
                        ButtonDown_NoRepeat(BUTTON_KEY_CLICK);
                        RaiseButtonEvent(SDL_USEREVENT_BUTTON_CLICK_DOWN);
		}


		if (event_p->jbutton.button == WIIGC1_BUTTON_B || event_p->jbutton.button == CLASSIC1_BUTTON_B)
			RaiseButtonEvent(SDL_USEREVENT_BUTTON_QUIT);

	} else if (event_p->type == SDL_JOYBUTTONUP) {

		if (event_p->jbutton.button
		 == WIIGC1_BUTTON_A || event_p->jbutton.button
		 == WIIGC1_BUTTON_X || event_p->jbutton.button
		 == WIIGC1_BUTTON_Y || event_p->jbutton.button
		 == CLASSIC1_BUTTON_A)  {
                        ButtonUp(BUTTON_KEY_CLICK);
                        RaiseButtonEvent(SDL_USEREVENT_BUTTON_CLICK_UP);
		}
	}
#endif
}

