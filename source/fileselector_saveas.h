#ifndef SAVEASKEYBOARD_HEADER
#define SAVEASKEYBOARD_HEADER

#include <gui/gui.h>

const char* FileSelector_SaveAs_Show(SDL_Surface *frame_buffer_p, const char* default_filename_p);
void SaveAs_Keyboard_Destroy();

#endif
