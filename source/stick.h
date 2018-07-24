#ifndef STICK_HEADER
#define STICK_HEADER

#include "sdl.h"
#include "keyboard.h"

#define STICKMODE_CRITICAL	0
#define STICKMODE_MULTI         1
#define STICKMODE_HORIZONTAL    2
#define STICKMODE_VERTICAL      3

#define STICKBUTTON_PRESSED     1
#define STICKBUTTON_RELEASED    0

#ifdef __cplusplus
extern "C"{
#endif
	void StickManager_SetMode(unsigned int mode);
	unsigned int StickManager_GetMode(void);
	void StickManager_Clear(void);

	void StickManager_Initialize(void);
	void StickManager_Quit(void);

	void StickManager_Up(unsigned int is_released);
	void StickManager_Down(unsigned int is_released);
	void StickManager_Left(unsigned int is_released);
	void StickManager_Right(unsigned int is_released);

	void StickManager_UpLeft(unsigned int is_released);
	void StickManager_UpRight(unsigned int is_released);
	void StickManager_DownLeft(unsigned int is_released);
	void StickManager_DownRight(unsigned int is_released);
#ifdef __cplusplus
}
#endif


#endif
