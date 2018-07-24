#ifndef VIRTUALKEYBOARD_HEADER
#define VIRTUALKEYBOARD_HEADER

#include "beebwin.h"
#include <SDL.h>

int 	VirtualKeyboard_Initialize(SDL_Surface *frame_buffer_p);
void 	VirtualKeyboard_Render();
void 	VirtualKeyboard_FreeResources();

void 	VirtualKeyboard_Lock();
void 	VirtualKeyboard_Unlock();
int 	VirtualKeyboard_IsLocked();

void 	VirtualKeyboard_MoveLeft();
void 	VirtualKeyboard_MoveRight();
void 	VirtualKeyboard_MoveDown();
void 	VirtualKeyboard_MoveUp();
void	VirtualKeyboard_KeyDown();
void	VirtualKeyboard_KeyUp();


void 	VirtualKeyboard_RepaintTapeLED(int state);
void	VirtualKeyboard_RepaintCapsLED(int state);
void	VirtualKeyboard_RepaintShiftLED(int state);

void 	VirtualKeyboard_UpdateLEDs();

#endif
