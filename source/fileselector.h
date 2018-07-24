#include <stdio.h>
#include <SDL.h>
#include <string.h>

#include "listbox.h"

void FileSelector_Load(SDL_Surface *frame_buffer_p, const char *path_p, const char *file_types_p, char *selection_p);
//const char* FileSelector_Save(SDL_Surface *frame_buffer_p, const char *path_p, const char *file_types_p);

void FileSelector_Insert0(SDL_Surface *frame_buffer_p, const char *path_p, const char *file_types_p, char *selection_p);
void FileSelector_Insert1(SDL_Surface *frame_buffer_p, const char *path_p, const char *file_types_p, char *selection_p);

void FileSelector_SaveState(SDL_Surface *frame_buffer_p, const char *path_p, const char *file_types_p, char *selection_p);
void FileSelector_LoadState(SDL_Surface *frame_buffer_p, const char *path_p, const char *file_types_p, char *selection_p);
void FileSelector_RecreateStatesSelector(SDL_Surface *frame_buffer_p, const char *path_p, const char *file_types_p, const char *default_p);

void FileSelector_CreateBlankDiscImage(SDL_Surface *frame_buffer_p, const char *path_p, const char *file_types_p, char *selection_p);

void FileSelector_Initialize(void);
void FileSelector_Quit(void);
