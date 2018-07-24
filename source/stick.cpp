/* Stick handler
 */

#include "stick.h"
#include "beebconfig.h"
#include "beebconfig_data.h"

typedef struct {
	unsigned int
	 beeb_up, 
	 beeb_down,
	 beeb_left,
	 beeb_right, 

	 button_up,
	 button_down,
	 button_left,
	 button_right,

	 button_upleft,
	 button_upright,
	 button_downleft,
	 button_downright,

	 mode;
} StickManager;

StickManager stick, *stick_p=&stick;


void StickManager_SetMode(unsigned int mode)
{
	switch(mode) {
	case STICKMODE_CRITICAL:
		config.settings.stick = CRITICAL;
		stick_p->mode = STICKMODE_CRITICAL;
		break;
	case STICKMODE_HORIZONTAL:
		config.settings.stick = HORIZONTAL;
		stick_p->mode = STICKMODE_HORIZONTAL;
		break;
	case STICKMODE_VERTICAL:
		config.settings.stick = VERTICAL;
		stick_p->mode = STICKMODE_VERTICAL;
		break;
	case STICKMODE_MULTI:
	default:
		config.settings.stick = MULTI;
		stick_p->mode = STICKMODE_MULTI;
		break;
	}
}

unsigned int StickManager_GetMode(void)
{
	return stick_p->mode;
}

void StickManager_Clear(void)
{
	stick_p->beeb_up = stick_p->beeb_down = stick_p->beeb_left
	 = stick_p->beeb_right = stick_p->button_up = stick_p->button_down
	 = stick_p->button_left = stick_p->button_right
	 = stick_p->button_upleft = stick_p->button_upright
	 = stick_p->button_downleft = stick_p->button_downright
	 = STICKBUTTON_RELEASED;
}

void StickManager_Initialize(void)
{
	StickManager_Clear();
	StickManager_SetMode(STICKMODE_CRITICAL);
}

void StickManager_Quit(void)
{
}

//UP		=	BEEB_UP - but BEEB_UP is released ONLY if UPLEFT or UPRIGHT is not down.
void StickManager_Up(unsigned int is_released)
{
#if 0
	printf("---> UP called with %s.\n", is_released ? "RELEASED" : "PRESSED");

	if (! is_released) {
		stick_p->button_up = STICKBUTTON_PRESSED;

		if (stick_p->beeb_up == STICKBUTTON_RELEASED) {
			stick_p->beeb_up = STICKBUTTON_PRESSED;
			HandleButtonPressForKeysym(1, gp2xButtonToSDLK(GP2X_BUTTON_UP) );
			printf("---> Beeb UP key pressed.\n");
		}
	} else {
		stick_p->button_up = STICKBUTTON_RELEASED;

		if (stick_p->beeb_up == STICKBUTTON_PRESSED) {
			if(stick_p->mode==STICKMODE_HORIZONTAL || (stick_p->button_upleft != STICKBUTTON_PRESSED && stick_p->button_upright != STICKBUTTON_PRESSED)){
				stick_p->beeb_up = STICKBUTTON_RELEASED;
				HandleButtonPressForKeysym(0, gp2xButtonToSDLK(GP2X_BUTTON_UP) );
				printf("---> Beeb UP key released.\n");
			}
		}
	}
#endif
}

//DOWN		=	BEEB_DOWN - but BEEB_DOWN is released ONLY if DOWNLEFT or DOWNRIGHT is not down.
void StickManager_Down(unsigned int is_released)
{
#if 0
	printf("---> DOWN called with %s.\n", is_released ? "RELEASED" : "PRESSED");

	if (! is_released) {
		stick_p->button_down = STICKBUTTON_PRESSED;

		if (stick_p->beeb_down == STICKBUTTON_RELEASED) {
			stick_p->beeb_down = STICKBUTTON_PRESSED;
			HandleButtonPressForKeysym(1, gp2xButtonToSDLK(GP2X_BUTTON_DOWN) );
			printf("---> Beeb DOWN key pressed.\n");
		}
	} else {
		stick_p->button_down = STICKBUTTON_RELEASED;

		if (stick_p->beeb_down == STICKBUTTON_PRESSED) {
			if (stick_p->mode==STICKMODE_HORIZONTAL || (stick_p->button_downleft != STICKBUTTON_PRESSED && stick_p->button_downright != STICKBUTTON_PRESSED)){
				stick_p->beeb_down = STICKBUTTON_RELEASED;
				HandleButtonPressForKeysym(0, gp2xButtonToSDLK(GP2X_BUTTON_DOWN) );
				printf("---> Beeb DOWN key released.\n");
			}
		}
	}
#endif
}

//LEFT		=	BEEB_LEFT - but BEEB_LEFT is released ONLY if UPLEFT or DOWNLEFT is not down.
void StickManager_Left(unsigned int is_released)
{
#if 0
	printf("---> LEFT called with %s.\n", is_released ? "RELEASED" : "PRESSED");

	if (! is_released) {
		stick_p->button_left = STICKBUTTON_PRESSED;

		if (stick_p->beeb_left == STICKBUTTON_RELEASED) {
			stick_p->beeb_left = STICKBUTTON_PRESSED;
			HandleButtonPressForKeysym(1, gp2xButtonToSDLK(GP2X_BUTTON_LEFT) );
			printf("---> Beeb LEFT key pressed.\n");
		}
	} else {
		stick_p->button_left = STICKBUTTON_RELEASED;

		if (stick_p->beeb_left == STICKBUTTON_PRESSED) {
			if ( stick_p->mode==STICKMODE_VERTICAL || (stick_p->button_downleft != STICKBUTTON_PRESSED && stick_p->button_upleft != STICKBUTTON_PRESSED)) {
				stick_p->beeb_left = STICKBUTTON_RELEASED;
				HandleButtonPressForKeysym(0, gp2xButtonToSDLK(GP2X_BUTTON_LEFT) );
				printf("---> Beeb LEFT key released.\n");
			}
		}
	}
#endif
}

//RIGHT		=	BEEB_RIGHT - but BEEB_RIGHT is released ONLY if UPRIGHT or DOWNRIGHT is not down.
void StickManager_Right(unsigned int is_released)
{
#if 0
	printf("---> RIGHT called with %s.\n", is_released ? "RELEASED" : "PRESSED");

	if (! is_released) {
		stick_p->button_right = STICKBUTTON_PRESSED;

		if (stick_p->beeb_right == STICKBUTTON_RELEASED) {
			stick_p->beeb_right = STICKBUTTON_PRESSED;
			HandleButtonPressForKeysym(1, gp2xButtonToSDLK(GP2X_BUTTON_RIGHT) );
			printf("---> Beeb RIGHT key pressed.\n");
		}
	} else {
		stick_p->button_right = STICKBUTTON_RELEASED;

		if (stick_p->beeb_right == STICKBUTTON_PRESSED) {
			if ( stick_p->mode==STICKMODE_VERTICAL || (stick_p->button_downright != STICKBUTTON_PRESSED && stick_p->button_upright != STICKBUTTON_PRESSED)) {
				stick_p->beeb_right = STICKBUTTON_RELEASED;
				HandleButtonPressForKeysym(0, gp2xButtonToSDLK(GP2X_BUTTON_RIGHT) );
				printf("---> Beeb RIGHT key released.\n");
			}
		}
	}
#endif
}

//UPLEFT		=	BEEB_UP, BEEB_LEFT - but BEEB_LEFT is released ONLY if LEFT is not down, and BEEB_UP is released ONLY if UP is not down.
void StickManager_UpLeft(unsigned int is_released)
{
#if 0
	printf("---> UPLEFT called with %s.\n", is_released ? "RELEASED" : "PRESSED");

	if (! is_released) {
		stick_p->button_upleft = STICKBUTTON_PRESSED;

		if (stick_p->mode!=STICKMODE_HORIZONTAL && stick_p->beeb_up == STICKBUTTON_RELEASED) {
			stick_p->beeb_up = STICKBUTTON_PRESSED;
			HandleButtonPressForKeysym(1, gp2xButtonToSDLK(GP2X_BUTTON_UP) );
			printf("---> Beeb UP key pressed.\n");
		}
		if (stick_p->mode!=STICKMODE_VERTICAL && stick_p->beeb_left == STICKBUTTON_RELEASED) {
			stick_p->beeb_left = STICKBUTTON_PRESSED;
			HandleButtonPressForKeysym(1, gp2xButtonToSDLK(GP2X_BUTTON_LEFT) );
			printf("---> Beeb LEFT key pressed.\n");
		}
	} else {
		stick_p->button_upleft = STICKBUTTON_RELEASED;

		if (stick_p->mode!=STICKMODE_HORIZONTAL && stick_p->beeb_up == STICKBUTTON_PRESSED) {
			if (stick_p->button_up != STICKBUTTON_PRESSED) {
				stick_p->beeb_up = STICKBUTTON_RELEASED;

				HandleButtonPressForKeysym(0, gp2xButtonToSDLK(GP2X_BUTTON_UP) );
				printf("---> Beeb UP key released.\n");
			}
		}
		if (stick_p->mode!=STICKMODE_VERTICAL && stick_p->beeb_left == STICKBUTTON_PRESSED) {
			if (stick_p->button_left != STICKBUTTON_PRESSED) {
				stick_p->beeb_left = STICKBUTTON_RELEASED;

				HandleButtonPressForKeysym(0, gp2xButtonToSDLK(GP2X_BUTTON_LEFT) );
				printf("---> Beeb LEFT key released.\n");
			}
		}
	}
#endif
}

//UPRIGHT		=	BEEB_UP, BEEB_RIGHT - but BEEB_RIGHT is released ONLY if RIGHT is not down, and BEEB_UP is released ONLY if UP is not down.
void StickManager_UpRight(unsigned int is_released)
{
#if 0
	printf("---> UPRIGHT called with %s.\n", is_released ? "RELEASED" : "PRESSED");

	if (! is_released) {
		stick_p->button_upright = STICKBUTTON_PRESSED;

		if (stick_p->mode!=STICKMODE_HORIZONTAL && stick_p->beeb_up == STICKBUTTON_RELEASED) {
			stick_p->beeb_up = STICKBUTTON_PRESSED;
			HandleButtonPressForKeysym(1, gp2xButtonToSDLK(GP2X_BUTTON_UP) );
			printf("---> Beeb UP key pressed.\n");
		}
		if (stick_p->mode!=STICKMODE_VERTICAL && stick_p->beeb_right == STICKBUTTON_RELEASED) {
			stick_p->beeb_right = STICKBUTTON_PRESSED;
			HandleButtonPressForKeysym(1, gp2xButtonToSDLK(GP2X_BUTTON_RIGHT) );
			printf("---> Beeb RIGHT key pressed.\n");
		}
	} else {
		stick_p->button_upright = STICKBUTTON_RELEASED;

		if (stick_p->mode!=STICKMODE_HORIZONTAL && stick_p->beeb_up == STICKBUTTON_PRESSED) {
			if (stick_p->button_up != STICKBUTTON_PRESSED) {
				stick_p->beeb_up = STICKBUTTON_RELEASED;

				HandleButtonPressForKeysym(0, gp2xButtonToSDLK(GP2X_BUTTON_UP) );
				printf("---> Beeb UP key released.\n");
			}
		}
		if (stick_p->mode!=STICKMODE_VERTICAL && stick_p->beeb_right == STICKBUTTON_PRESSED) {
			if (stick_p->button_right != STICKBUTTON_PRESSED) {
				stick_p->beeb_right = STICKBUTTON_RELEASED;

				HandleButtonPressForKeysym(0, gp2xButtonToSDLK(GP2X_BUTTON_RIGHT) );
				printf("---> Beeb RIGHT key released.\n");
			}
		}
	}
#endif
}

//DOWNLEFT	=	BEEB_DOWN, BEEB_LEFT - but BEEB_LEFT is released ONLY if LEFT is not down, and BEEB_DOWN is released ONLY if DOWN is not down.
void StickManager_DownLeft(unsigned int is_released)
{
#if 0
	printf("---> DOWNLEFT called with %s.\n", is_released ? "RELEASED" : "PRESSED");

	if (! is_released) {
		stick_p->button_downleft = STICKBUTTON_PRESSED;

		if (stick_p->mode!=STICKMODE_HORIZONTAL && stick_p->beeb_down == STICKBUTTON_RELEASED) {
			stick_p->beeb_down = STICKBUTTON_PRESSED;
			HandleButtonPressForKeysym(1, gp2xButtonToSDLK(GP2X_BUTTON_DOWN) );
			printf("---> Beeb DOWN key pressed.\n");
		}
		if (stick_p->mode!=STICKMODE_VERTICAL && stick_p->beeb_left == STICKBUTTON_RELEASED) {
			stick_p->beeb_left = STICKBUTTON_PRESSED;
			HandleButtonPressForKeysym(1, gp2xButtonToSDLK(GP2X_BUTTON_LEFT) );
			printf("---> Beeb LEFT key pressed.\n");
		}
	} else {
		stick_p->button_downleft = STICKBUTTON_RELEASED;

		if (stick_p->mode!=STICKMODE_HORIZONTAL && stick_p->beeb_down == STICKBUTTON_PRESSED) {
			if (stick_p->button_down != STICKBUTTON_PRESSED) {
				stick_p->beeb_down = STICKBUTTON_RELEASED;

				HandleButtonPressForKeysym(0, gp2xButtonToSDLK(GP2X_BUTTON_DOWN) );
				printf("---> Beeb DOWN key released.\n");
			}
		}
		if (stick_p->mode!=STICKMODE_VERTICAL && stick_p->beeb_left == STICKBUTTON_PRESSED) {
			if (stick_p->button_left != STICKBUTTON_PRESSED) {
				stick_p->beeb_left = STICKBUTTON_RELEASED;

				HandleButtonPressForKeysym(0, gp2xButtonToSDLK(GP2X_BUTTON_LEFT) );
				printf("---> Beeb LEFT key released.\n");
			}
		}
	}
#endif
}

//DOWNRIGHT	=	BEEB_DOWN, BEEB_RIGHT - but BEEB_RIGHT is released ONLY if RIGHT is not down, and BEEB_DOWN is released ONLY if DOWN is not down.
void StickManager_DownRight(unsigned int is_released)
{
#if 0
	printf("---> DOWNLEFT called with %s.\n", is_released ? "RELEASED" : "PRESSED");

	if (! is_released) {
		stick_p->button_downright = STICKBUTTON_PRESSED;

		if (stick_p->mode!=STICKMODE_HORIZONTAL && stick_p->beeb_down == STICKBUTTON_RELEASED) {
			stick_p->beeb_down = STICKBUTTON_PRESSED;
			HandleButtonPressForKeysym(1, gp2xButtonToSDLK(GP2X_BUTTON_DOWN) );
			printf("---> Beeb DOWN key pressed.\n");
		}
		if (stick_p->mode!=STICKMODE_VERTICAL && stick_p->beeb_right == STICKBUTTON_RELEASED) {
			stick_p->beeb_right = STICKBUTTON_PRESSED;
			HandleButtonPressForKeysym(1, gp2xButtonToSDLK(GP2X_BUTTON_RIGHT) );
			printf("---> Beeb RIGHT key pressed.\n");
		}
	} else {
		stick_p->button_downright = STICKBUTTON_RELEASED;

		if (stick_p->mode!=STICKMODE_HORIZONTAL && stick_p->beeb_down == STICKBUTTON_PRESSED) {
			if (stick_p->button_down != STICKBUTTON_PRESSED) {
				stick_p->beeb_down = STICKBUTTON_RELEASED;

				HandleButtonPressForKeysym(0, gp2xButtonToSDLK(GP2X_BUTTON_DOWN) );
				printf("---> Beeb DOWN key released.\n");
			}
		}
		if (stick_p->mode!=STICKMODE_VERTICAL && stick_p->beeb_right == STICKBUTTON_PRESSED) {
			if (stick_p->button_right != STICKBUTTON_PRESSED) {
				stick_p->beeb_right = STICKBUTTON_RELEASED;

				HandleButtonPressForKeysym(0, gp2xButtonToSDLK(GP2X_BUTTON_RIGHT) );
				printf("---> Beeb RIGHT key released.\n");
			}
		}
	}
#endif
}

