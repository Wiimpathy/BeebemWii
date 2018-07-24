/* Simple message box
 */

#if HAVE_CONFIG_H
#       include <config.h>
#endif

#include <stdint.h>
#include <gui/log.h>

#include "main.h"
#include "messagebox.h"
#include "messagebox_private.h"
#include "button.h"

#include <gui/gui.h>
#include <gui/functions.h>

#include <SDL.h>


/* Dimension of message box: 
 */
#define MSGBOX_MAX_ROWS 24
#define MSGBOX_MAX_COLS (40-4)


/* The EG Message Box:
 */

static int d;
static int return_value;
static EG_Window *window_ptr;


/* This should fudge the message box centering problem for now, all hail the new
 * GUI, may this one RIP...
 */
static SDL_Rect CalcRectCentered_MB(SDL_Surface *s, int width, int height)
{
        SDL_Rect tmp;

//	int sh;
//
//	if (s->h == 240 || s->h == 480)
//		sh = 480;
//	else
//		sh = 512;
//
  //      tmp.x = (640-width)/2;
    //    tmp.y = (sh-height)/2;
//
  //      tmp.w = width;
    //    tmp.h = height;


	tmp.x = (320-width)/2;
	tmp.y = (240-height)/2;
	tmp.w = width;
	tmp.h = height;

        return(tmp);
}



/* Formats source string into rows where the width is MSGBOX_MAX_COLS at
 * max. next_row_ptr must be at least MSGBOX_MAX_COLS+1 chars
 *
 * (Preprocess source text (as a copy) with ProcessAndCopyString first to remove
 * duplicate spaces and other crapness).
 */
static const char* FormatText(const char *source_ptr, char *next_row_ptr)
{
	int i, word_end;

	/* If source text is an empty string, return an empty string.
	 */
	if ( strlen(source_ptr)==0 ){
		next_row_ptr[0]=0;

		return source_ptr;
	}
	
	/* Loop incrementing index until max width, if a space/other (<=32)
	 * is found, mark position, if a carriage return is found
	 * break out of the loop and mark the position.
	 */
	for ( i=word_end=0; i<MSGBOX_MAX_COLS-1 && i<(int)strlen(source_ptr); i++ ){
		if ( source_ptr[i]==10){
			word_end=i;

			break;

		}else if ( source_ptr[i]<=32 ){
			word_end=i;
		}
	} i--;

	/* If no characters marked, (i.e.: a 10 found and loop broken) then next
	 * char is a carriage return, so return as blank string.
	 */
	if ( i<0 ){
		next_row_ptr[0]=0;
		source_ptr++;

	/* Otherwise extract exactly enough characters to nicely fit one line
	 * of text.
	 */
	}else{
		/* If the next character after 'max row width' is a seperator,
		 * inc. mark to this position. 
		 */
		if ( i+1<(int)strlen(source_ptr) && source_ptr[i+1]<=32 )
			word_end=i+1;

		/* If this row sucked-up all remaining characters in the source
		 * string, then up the mark to the end of the text (we'll take
		 * everything). 
		 */
		if ( i==(int)strlen(source_ptr)-1 )
			word_end=i;

		/* If no mark was set, then split at index
		 * (word is longer than max width so we must split it).
		 */
		if ( word_end==0 ){
			memcpy(next_row_ptr, source_ptr, i+1);
			next_row_ptr[i+1]=0;

			source_ptr+=i+1;

		/* If have a marked word end, then split here
		 */
		}else{
			memcpy(next_row_ptr, source_ptr, word_end+1);
			next_row_ptr[word_end+1]=0;

			source_ptr+=word_end+1;
		}
	}

	/* Remove carriage returns and spaces (probably don't need to check
	 * for leading stuff).
	 */
	while ( *source_ptr>0 && *source_ptr!=10 && *source_ptr<=32 )
		source_ptr++;

	while ( strlen(next_row_ptr)>0 
	 && next_row_ptr[strlen(next_row_ptr)-1]<=32 )
		next_row_ptr[strlen(next_row_ptr)-1]=0;

	/* Return a pointer to what remains of the source string.
	 */
	return source_ptr;
}

/* Same as memcpy but removes anything <=32 at start, or end of string and
 * duplicates <=32 within the string.
 *
 * Used to 'pre-process' the source string for the message box and get rid
 * of duplicate spaces prior to splitting the text.
 */
static void ProcessAndCopyString(char *dest_ptr, const char *source_ptr)
{
	int i=0;

	/* Remove spaces at start:
	 */
	while(source_ptr[i]!=0 && source_ptr[i]<=32 )
		i++;

	/* Copy text with at most one consecutive char <=32 unless it's 10,
	 * and convert all chars (except 10) that are <32 to spaces.
	 */
	while(source_ptr[i]!=0){
		if (source_ptr[i]==10)
			*(dest_ptr++)=source_ptr[i];
		else if ( ! (source_ptr[i]<=32 && source_ptr[i+1]<=32) )
			*(dest_ptr++)=source_ptr[i]<=32 ? 32 : source_ptr[i];

		i++;
	}
	*dest_ptr = 0;
}

static void MsgBox_Button_Press(EG_Widget *widget_ptr, void *user_ptr)
{
        EG_Window_Hide(window_ptr);
	d = 1;

	return_value = atoi(EG_Widget_GetName(widget_ptr));
}

int EG_MessageBox(SDL_Surface *surface_ptr, int type, const char *title_ptr
 , const char *text_ptr, const char *button1_ptr
 , const char *button2_ptr, const char *button3_ptr
 , const char *button4_ptr, int has_focus)
{
	int row_count, row, button_count;
	const char *message_ptr;
	char *next_row_ptr, *text_copy_ptr;

	int old_delay;

	EG_Widget *buttons_ptr[4];

	SDL_Colour col, col2;
	SDL_Rect win, loc;

	EG_Widget *widget_ptr;	

	printf("entered message box\n");

	/* Fudge so NULL buttons are handled. I.e.: but4, but2, but1 (but3 missing).
	 */
	if (button1_ptr == NULL) {
		button1_ptr = button2_ptr;
		button2_ptr = button3_ptr;
		button3_ptr = button4_ptr;
		button4_ptr = NULL;
	}
	if (button2_ptr == NULL) {
		button2_ptr = button3_ptr;
		button3_ptr = button4_ptr;
		button4_ptr = NULL;
	}
	if (button3_ptr == NULL) {
		button3_ptr = button4_ptr;
		button4_ptr = NULL;
	}


//	SDL_Event e;
	SDL_Event event;

	char icon[2][3]={"  ", "  "};

	if (surface_ptr == NULL || title_ptr == NULL || text_ptr == NULL)
		return 0;

	
	/* Flush SDL event queue:
	 */
	EG_Draw_FlushEventQueue();


	/* Make a copy of source text without useless spaces etc..
	 */
	if ( (text_copy_ptr=(char*)EG_Malloc(strlen(text_ptr)+1)) == NULL){
		qERROR("Unable to alloc EG_MessageBox text buffer.");
		return 0;
	}else{
		ProcessAndCopyString(text_copy_ptr, text_ptr);
	}

	/* Make a buffer for building each formatted line to feed to the
	 * message box dialog.
	 */
	if ( (next_row_ptr=(char*)EG_Malloc(MSGBOX_MAX_COLS+1)) == NULL){
		qERROR("Unable to alloc EG_MessageBox row buffer.");
		return 0;
	}

	/* Calc. number of rows.
	 */
	row_count = 0; message_ptr = text_copy_ptr;
	//printf(">%s<\n", message_ptr);
	while (strlen(message_ptr) > 0){
		message_ptr = FormatText(message_ptr, next_row_ptr);
		//printf(">%s<   #%s#\n", message_ptr, next_row_ptr);
		row_count++;
	}

	/* Create window.
	 */
//	col = MENU_COLORS;
//	col = DARKER_MENU_COLORS;

//	if (type == EG_MESSAGEBOX_STOP)
//		col = (SDL_Color) {170, 0, 0, 0};
//	else
//		col = VERY_DARK_MENU_COLORS;

	col = DARKER_MENU_COLORS;

	win = CalcRectCentered_MB(surface_ptr, 40 + 40*5
	 , 8*5 + ((row_count<24?row_count:24)*8) );	

	win.x-=10; win.w+=20;
	win.y-=5; win.h+=10;

	window_ptr = EG_Window_Create("win_msgbox", surface_ptr, col, win);

	win.x+=10; win.w-=20;
	win.y+=5; win.h-=10;


	/* Create title and box.
	 */
//--	loc = CalcRect(5+40, 5, win.w-15-40, 16);
	loc = CalcRect(2+20+win.x, 2+win.y, win.w-6-20, 8);
	widget_ptr = EG_Label_Create("lab_msgbox", EG_COLOR_ALT, EG_LABEL_ALIGN_CENTER
	 , title_ptr, loc);

	(void) EG_Window_AddWidget(window_ptr, widget_ptr);

//--	loc = CalcRect(5+40, 10+16, win.w-15-40
//--	 , ((row_count<24?row_count:24)*16)+16 );

	loc = CalcRect(win.x+2+20, win.y+5+8, win.w-7-20, ((row_count<24?row_count:24)*8)+8);
	widget_ptr = EG_Box_Create("box_msgbox", EG_BOX_BORDER_SUNK, col, loc);
	(void) EG_Window_AddWidget(window_ptr, widget_ptr);

	/* Create icon
	 */

//--	icon[0][0]=128+(type*2);    icon[0][1] = 129+(type*2);
//--	icon[1][0]=128+16+(type*2); icon[1][1] = 129+16+(type*2);
	icon[0][0]=16+(type*2);    icon[0][1] = 17+(type*2);
	icon[1][0]=16+6+(type*2); icon[1][1] = 17+6+(type*2);




//--	loc = CalcRect(10, (win.h/2)-16-6, 20, 16);
	loc = CalcRect(5+win.x, win.y+(win.h/2)-8-3, 10, 8);
	widget_ptr = EG_Label_Create(NULL, col,EG_LABEL_ALIGN_LEFT,icon[0],loc);
	EG_Window_AddWidget(window_ptr, widget_ptr);
//--	loc.y+=16;
	loc.y+=8;
	widget_ptr = EG_Label_Create(NULL, col,EG_LABEL_ALIGN_LEFT,icon[1],loc);
	EG_Window_AddWidget(window_ptr, widget_ptr);

	/* Create message.
	 */
	row = 0; message_ptr = text_copy_ptr;
	while (strlen(message_ptr) > 0 && row<24){
		message_ptr = FormatText(message_ptr, next_row_ptr);

//--		loc = CalcRect(50, 10+16+(row*16)+8, win.w-25-40, 16);
		loc = CalcRect(25+win.x, 5+8+(row*8)+4+win.y, win.w-12-20, 8);

		widget_ptr = EG_Label_Create(NULL, col, EG_LABEL_ALIGN_CENTER
		 , next_row_ptr, loc);
		(void) EG_Window_AddWidget(window_ptr, widget_ptr);

		row++;
	}

	/* Create buttons.
	 */
//--	loc = CalcRect(win.w -10, win.h -30, 0, 20);
	loc = CalcRect(win.x+win.w-5, win.y+win.h-15, 0, 10);

	loc.h+=4; loc.y-=2;
	
	loc.x-=4;


	button_count = 0;

	if (button4_ptr != NULL){
//--		loc.x -= (10*(strlen(button4_ptr)+1)); 
//--		loc.w = (10*(strlen(button4_ptr)+1));
		loc.x -= (5*(strlen(button4_ptr)+1));
		loc.w = (5*(strlen(button4_ptr)+1));
		button_count++;

		loc.w+=4;

		widget_ptr = EG_Button_Create("4", col, EG_BUTTON_ALIGN_CENTER
		 , button4_ptr, loc);

		(void) EG_Button_SetMyCallback_OnClick(widget_ptr
		 , MsgBox_Button_Press, NULL);
		(void) EG_Window_AddWidget(window_ptr, widget_ptr);

		if (has_focus == 4) (void) EG_Button_GetFocus(widget_ptr);

		buttons_ptr[3]=widget_ptr;
	}

	if (button3_ptr != NULL){
//--		loc.x -= (10*(strlen(button3_ptr)+1)); 
//--		loc.x -= (button_count>0?10:0);
//--		loc.w = (10*(strlen(button3_ptr)+1)); button_count++;
		loc.x -= (5*(strlen(button3_ptr)+1)); 
		loc.x -= (button_count>0?5:0);
		loc.w = (5*(strlen(button3_ptr)+1)); button_count++;

		loc.w+=4;

		widget_ptr = EG_Button_Create("3", col, EG_BUTTON_ALIGN_CENTER
		 , button3_ptr, loc);
		(void) EG_Button_SetMyCallback_OnClick(widget_ptr
		 , MsgBox_Button_Press, NULL);
		(void) EG_Window_AddWidget(window_ptr, widget_ptr);

		if (has_focus == 3) (void) EG_Button_GetFocus(widget_ptr);

		buttons_ptr[2]=widget_ptr;
	}

	if (button2_ptr != NULL){
//--		loc.x -= (10*(strlen(button2_ptr)+1));
//--		loc.x -= (button_count>0?10:0);
//--		loc.w = (10*(strlen(button2_ptr)+1)); button_count++;
		loc.x -= (5*(strlen(button2_ptr)+1));
		loc.x -= (button_count>0?5:0);
		loc.w = (5*(strlen(button2_ptr)+1)); button_count++;

		loc.w+=4;

		widget_ptr = EG_Button_Create("2", col, EG_BUTTON_ALIGN_CENTER
		 , button2_ptr, loc);
		(void) EG_Button_SetMyCallback_OnClick(widget_ptr
		 , MsgBox_Button_Press, NULL);
		(void) EG_Window_AddWidget(window_ptr, widget_ptr);
		if (has_focus == 2) (void) EG_Button_GetFocus(widget_ptr);

		buttons_ptr[1]=widget_ptr;
	}

	if (button1_ptr == NULL){ 
//--		loc.x -= (10*3); loc.w = (10*3); 
//--		loc.x -= (button_count>0?10:0);
		loc.x -= (5*3); loc.w = (5*3); 
		loc.x -= (button_count>0?5:0);

		loc.w+=4;

        	widget_ptr = EG_Button_Create("1", col, EG_BUTTON_ALIGN_CENTER
		 , "OK", loc);
	}else{
//--		loc.x -= (10*(strlen(button1_ptr)+1));
//--		loc.x -= (button_count>0?10:0);
//--		loc.w = (10*(strlen(button1_ptr)+1));
		loc.x -= (5*(strlen(button1_ptr)+1));
		loc.x -= (button_count>0?5:0);
		loc.w = (5*(strlen(button1_ptr)+1));

		loc.w+=4;

		widget_ptr = EG_Button_Create("1", col, EG_BUTTON_ALIGN_CENTER
		 , button1_ptr, loc);

		buttons_ptr[0]=widget_ptr;
	}
        (void) EG_Button_SetMyCallback_OnClick(widget_ptr, MsgBox_Button_Press
	 , NULL);
        (void) EG_Window_AddWidget(window_ptr, widget_ptr);
        if (has_focus == 1) (void) EG_Button_GetFocus(widget_ptr);


	/* Smart focus:
	 */
	if (button4_ptr != NULL)
		EG_Widget_SetSmartFocusPeerRight(buttons_ptr[3], buttons_ptr[2]);

	if (button3_ptr != NULL) {
		EG_Widget_SetSmartFocusPeerRight(buttons_ptr[2], buttons_ptr[1]);
		if (button4_ptr != NULL)
			EG_Widget_SetSmartFocusPeerLeft(buttons_ptr[2], buttons_ptr[3]);
	}

	if (button2_ptr != NULL) {
		EG_Widget_SetSmartFocusPeerRight(buttons_ptr[1], buttons_ptr[0]);
		if (button3_ptr != NULL)
			EG_Widget_SetSmartFocusPeerLeft(buttons_ptr[1], buttons_ptr[2]);
	}

	if (button1_ptr != NULL && button2_ptr != NULL)
		EG_Widget_SetSmartFocusPeerLeft(buttons_ptr[0], buttons_ptr[1]);



	/* Free copy of text and row buffer
	 */
	EG_Free(text_copy_ptr);
	EG_Free(next_row_ptr);

	/* Process window.
	 */
	EG_Window_Show(window_ptr);

        Button_Reset();
        old_delay = Button_GetAutoRepeatDelay();
        Button_SetAutoRepeatDelay(5);


//--	return_value = 1;
//--	d = 0;
//--	while(!d){
//--                while ( SDL_PollEvent(&e) ){
//--                        switch (e.type) {
//--                                case SDL_QUIT:
//--                                d=1;
//--                                break;
//--
//--                                case SDL_KEYDOWN:
//--                                if(e.key.keysym.sym == SDLK_ESCAPE)
//--                                        d=1;
//--                                break;
//--                        }
//--                        EG_Window_ProcessEvent(window_ptr, &e, 0, 0);
//--
//--                }
//--                SDL_Delay(50);
//--                EG_Window_ProcessEvent(window_ptr, NULL, 0,0);
//--		SDL_UpdateRect(surface_ptr, 0,0,0,0);
//--        }
 
	d=0;
	while(!d){

		EG_Graphics_Lock(surface_ptr);

                while ( SDL_PollEvent(&event) ) {

                        if (event.type != SDL_USEREVENT) {
                                Button_TranslateEvent(&event, mainWin->ScreenIsReversed());
                        } else {
                                switch ( (intptr_t) event.user.data1) {
                                case SDL_USEREVENT_BUTTON_QUIT:
                                        d=1;
                                        break;
                                case SDL_USEREVENT_BUTTON_RIGHT:
                                        EG_Window_MoveSmartFocusLeft(window_ptr);
                                        break;

                                case SDL_USEREVENT_BUTTON_LEFT:
                                        EG_Window_MoveSmartFocusRight(window_ptr);
                                        break;

//                                case SDL_USEREVENT_BUTTON_UP:
//                                        EG_Window_MoveSmartFocusUp(window_p);
//                                        break;

//                                case SDL_USEREVENT_BUTTON_DOWN:
//                                        EG_Window_MoveSmartFocusDown(window_p);
//                                        break;

                                case SDL_USEREVENT_BUTTON_CLICK_DOWN:
                                        EG_Window_Trigger_Select_Down(window_ptr);
                                        break;

                                case SDL_USEREVENT_BUTTON_CLICK_UP:
                                        EG_Window_Trigger_Select_Up(window_ptr);
                                        break;
                                }
                        }

                        /* Send this event to the window:
                         */
                        EG_Window_ProcessEvent(window_ptr, &event, 0, 0);
                }

                /* Slight delay:
                 */
                SDL_Delay(40);  // 25fps
                Button_RaiseAutoRepeatEvents();
//                SDL_UpdateRect(frame_buffer_p, 0,0,0,0);
		SDL_UpdateRect(surface_ptr, 0,0,0,0);

                /* Make sure EG house keeping is triggered every 40ms:
                 */
                EG_Window_ProcessEvent(window_ptr, NULL, 0, 0);

		EG_Graphics_Unlock(surface_ptr);

        }

        Button_Reset();
        Button_SetAutoRepeatDelay(old_delay);

        EG_Window_DestroyAllChildWidgets(window_ptr);
        EG_Window_Destroy(window_ptr);

	return return_value;
}

/* END OF messagebox.c --------------------------------------------------------
 */
