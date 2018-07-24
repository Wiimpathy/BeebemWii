#ifndef KEYBOARDMAPPER_HEADER
#define KEYBOARDMAPPER_HEADER

#include <gui/gui.h>

int BBC_Keyboard_SaveConfig(void);
int BBCKeyboard_Show(SDL_Surface *frame_buffer_p, int current_key);
void BBCKeyboard_Destroy();

#endif
