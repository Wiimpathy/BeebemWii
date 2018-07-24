#include <stdio.h>
#include <SDL.h>
#include <string.h>
#include <pthread.h>

#include <gui/gui.h>

typedef struct {
        EG_Window *window_p;
        EG_Widget *progress_bar_p, *caption_p;

	pthread_t thread_p;

	pthread_mutex_t displayed_mutex;
	int displayed;

} ProgressDialog;

ProgressDialog*	ProgressDialog_Create(SDL_Surface *surface_p, const char *caption_p, float position);
void 		ProgressDialog_Free(ProgressDialog *pd_p);
void	 	ProgressDialog_SetPosition(ProgressDialog *pd_p, float position);
float 		ProgressDialog_GetPosition(ProgressDialog *pd_p);
void 		ProgressDialog_Show(ProgressDialog *pd_p);
void 		ProgressDialog_Hide(ProgressDialog *pd_p);
void 	ProgressDialog_SetCaption(ProgressDialog *pd_p, const char* caption_p);
