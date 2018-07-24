#include "progressdialog.h"


static SDL_Rect CalcRectCentered(SDL_Surface *s, int width, int height)
{
        SDL_Rect tmp;
        tmp.x = (320-width)/2;
        tmp.y = (240-height)/2;
        tmp.w = width;
        tmp.h = height;
        return(tmp);
}


/* The dialog is shown in a separate thread.
 *
 * (EG is not thread safe, so don't try and show two dialogs!)
 */
static void* DisplayProcess(void *v_p)
{
	SDL_Event event;
	ProgressDialog *pd_p = (ProgressDialog*) v_p;

	if (pd_p == NULL)
		return 0;
#ifdef GEKKO
	pd_p->displayed=1;
#else
	pthread_mutex_lock(&pd_p->displayed_mutex);
	pd_p->displayed=1;
	pthread_mutex_unlock(&pd_p->displayed_mutex);
#endif

	while(! EG_Window_IsHidden(pd_p->window_p)) {

		EG_Graphics_Lock(EG_Window_GetSurface(pd_p->window_p));

		while ( SDL_PollEvent(&event) ) {
			EG_Window_ProcessEvent(pd_p->window_p, &event, 0, 0);

			if (event.type == SDL_QUIT)
				EG_Window_Hide(pd_p->window_p);

		}
		SDL_Delay(40);
		EG_Window_ProcessEvent(pd_p->window_p, NULL, 0, 0);

		EG_Graphics_Unlock(EG_Window_GetSurface(pd_p->window_p));
		SDL_UpdateRect(EG_Window_GetSurface(pd_p->window_p), 0,0,0,0);
	}
#ifdef GEKKO
	pd_p->displayed=0;
#else
	pthread_mutex_lock(&pd_p->displayed_mutex);
	pd_p->displayed=0;
	pthread_mutex_unlock(&pd_p->displayed_mutex);
#endif
	return 0;
}

//static void DisplayDialog(ProgressDialog *pd_p)
//{

//}

ProgressDialog* ProgressDialog_Create(SDL_Surface *surface_p, const char *caption_p, float position)
{
	ProgressDialog *pd_p;
	SDL_Rect win, r;
	SDL_Color col = DARKER_MENU_COLORS;
	SDL_Color col2 = EG_COLOR_NORMAL;
	char default_caption[]="Progress:";

        if ( (pd_p=(ProgressDialog*)EG_Malloc( sizeof(ProgressDialog) ) ) == NULL) {
                fprintf(stderr, "ProgressDialog: Unable to malloc struct.\n");
                return NULL;
        }

	win = CalcRectCentered(surface_p, 200, 30);
	pd_p->window_p = EG_Window_Create("win_progressdialog", surface_p, col, win);

	r = win; r.x+=4; r.w-=8;

	if (caption_p == NULL || strlen(caption_p) == 0)
		caption_p = default_caption;
	pd_p->caption_p = EG_Label_Create("progressdialog_title", EG_COLOR_ALT, EG_LABEL_ALIGN_CENTER, caption_p, C_1_1(r, 1, 2));
        EG_Window_AddWidget(pd_p->window_p, pd_p->caption_p);

	pd_p->progress_bar_p = EG_ProgressBar_Create("progressdialog_bar", col, col2, C_1_1(r, 3, 2));
	EG_Window_AddWidget(pd_p->window_p, pd_p->progress_bar_p);

	if (position == -1.0)
		EG_ProgressBar_PositionIsUnknown(pd_p->progress_bar_p);
#ifndef GEKKO
	pd_p->displayed_mutex = (pthread_mutex_t) PTHREAD_MUTEX_INITIALIZER;
#endif
	pd_p->displayed = 0;

	return pd_p;
}

void ProgressDialog_SetCaption(ProgressDialog *pd_p, const char* caption_p)
{
	if (pd_p == NULL)
		return;

	EG_Label_SetCaption(pd_p->caption_p, caption_p);
}

void ProgressDialog_SetPosition(ProgressDialog *pd_p, float position)
{
	if (pd_p == NULL)
		return;

	EG_ProgressBar_SetPosition(pd_p->progress_bar_p, position);
}

float ProgressDialog_GetPosition(ProgressDialog *pd_p)
{
	if (pd_p == NULL)
		return 0.0;

	return EG_ProgressBar_GetPosition(pd_p->progress_bar_p);
}

void ProgressDialog_Show(ProgressDialog *pd_p)
{
	if (pd_p == NULL)
		return;

        if (EG_ProgressBar_PositionIsUnknown(pd_p->progress_bar_p)) {
                EG_ProgressBar_SetPosition(pd_p->progress_bar_p, 0.0);
		EG_ProgressBar_PositionIsUnknown(pd_p->progress_bar_p);
        }

	if (EG_Window_IsHidden(pd_p->window_p))
		EG_Window_Show(pd_p->window_p);

#ifdef GEKKO
	// FIXME : issue with pthread include. Should use LWP_CreateThread and others lwp functions directly? 
	if (pd_p->displayed == 0) {
		/*if (pthread_create(&pd_p->thread_p, NULL, DisplayProcess, (void*) pd_p) != 0) {
			fprintf(stderr, "ProgressDialog: Unable to create thread (pthread_create).\n");
		}*/
	}
#else
	pthread_mutex_lock(&pd_p->displayed_mutex);
	if (pd_p->displayed == 0) {
		if (pthread_create(&pd_p->thread_p, NULL, DisplayProcess, (void*) pd_p) != 0) {
			fprintf(stderr, "ProgressDialog: Unable to create thread (pthread_create).\n");
		}
	}
	pthread_mutex_unlock(&pd_p->displayed_mutex);
#endif
}

void ProgressDialog_Hide(ProgressDialog *pd_p)
{
	if (pd_p == NULL)
		return;

	// This will cause the other threads loop to end.

	if (! EG_Window_IsHidden(pd_p->window_p))
		EG_Window_Hide(pd_p->window_p);
}

void ProgressDialog_Free(ProgressDialog *pd_p)
{
	int d;

	if (pd_p == NULL)
		return;

	if (! EG_Window_IsHidden(pd_p->window_p))
		ProgressDialog_Hide(pd_p);

	// Block until other thread ends.
	d=1; while (d) {
		//pthread_mutex_lock(&pd_p->displayed_mutex);
		d = pd_p->displayed;
		//pthread_mutex_unlock(&pd_p->displayed_mutex);
	}

	// Now can safely dump resources
	EG_Window_DestroyAllChildWidgets(pd_p->window_p);
	EG_Window_Destroy(pd_p->window_p);
	EG_Free(pd_p);
}


