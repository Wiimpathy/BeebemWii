/* Simple list box
 */

#if HAVE_CONFIG_H
#       include <config.h>
#endif

#include "button.h"
#include "main.h"

#include <gui/gui.h>
#include <gui/functions.h>

#include <SDL.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>

#include "beebconfig.h"
#include "beebconfig_data.h"

#include "listbox.h"
#include "keyboard.h"
#include "progressdialog.h"

static int d;
static int return_value;

#define MAX_ROWS 15
#define SCROLLBAR_WIDTH 12


/* This should fudge the message box centering problem for now, all hail the new
 * GUI, may this one RIP...
 */
static SDL_Rect CalcRectCentered(SDL_Surface *s, int width, int height)
{
	SDL_Rect tmp;
	//int sh;

	tmp.x = (320-width)/2;
	tmp.y = (240-height)/2;
	tmp.w = width;
	tmp.h = height;

	return(tmp);
}

static void SetColorToNormal(ListBox *listbox_p, EG_Widget *widget_p)
{
	SDL_Color c = listbox_p->color;
	EG_Button_SetBackgroundColor(widget_p, c);
}

static void SetColorToSelected(ListBox *listbox_p, EG_Widget *widget_p)
{
	SDL_Color c = listbox_p->color;
	c.r = (uint) (c.r * 0.7); c.g = (uint) (c.g * 0.7); c.b = (uint) (c.b * 0.7);
	EG_Button_SetBackgroundColor(widget_p, c);
	EG_Button_RepaintLot(widget_p);
}

#define LISTBOX_CHAR_WIDTH 57
static void UpdateList(ListBox *listbox_p)
{
	char name[LISTBOX_CHAR_WIDTH+1];

	unsigned int i;

	for (i=0; i< listbox_p->listbox_entries; i++) {
//		EG_Button_SetCaption(listbox_p->buttons_p[i], ListBox_GetEntryName(listbox_p, listbox_p->offset+i));

		strncpy(name, ListBox_GetEntryName(listbox_p, listbox_p->offset+i), LISTBOX_CHAR_WIDTH);
		name[LISTBOX_CHAR_WIDTH]='\0';
		EG_Button_SetCaption(listbox_p->buttons_p[i], name);


		SetColorToNormal(listbox_p, listbox_p->buttons_p[i]);
	}

	SetColorToSelected(listbox_p, listbox_p->buttons_p[listbox_p->selected]);
	EG_Button_GetFocus(listbox_p->buttons_p[listbox_p->selected]);
}

static void MoveUp(ListBox *listbox_p)
{
	SetColorToNormal(listbox_p, listbox_p->buttons_p[listbox_p->selected]);

	printf("%d %d %d (%d)\n", listbox_p->offset, listbox_p->selected, listbox_p->count, listbox_p->offset+listbox_p->selected);

	if ( listbox_p->selected == 0 && listbox_p->offset == 0) {
		printf("GOTO BOTTOM\n");

		listbox_p->offset = listbox_p->count - listbox_p->listbox_entries;
		listbox_p->selected = listbox_p->listbox_entries-1;

		EG_SlideBar_SetPosition(listbox_p->slidebar_p, (listbox_p->offset)*10);
		UpdateList(listbox_p);
		return;
	}

	if (listbox_p->selected == 0) {
		if (listbox_p->selected+listbox_p->offset > 0) {
			listbox_p->offset--;
			UpdateList(listbox_p);
		}
	}

	if (listbox_p->selected>0) {
		listbox_p->selected--;
		EG_Button_GetFocus(listbox_p->buttons_p[listbox_p->selected]);
	}

	SetColorToSelected(listbox_p, listbox_p->buttons_p[listbox_p->selected]);
	EG_SlideBar_SetPosition(listbox_p->slidebar_p, (listbox_p->offset)*10);
}

static void MoveDown(ListBox *listbox_p)
{
	SetColorToNormal(listbox_p, listbox_p->buttons_p[listbox_p->selected]);

	printf("%d %d %d (%d)\n", listbox_p->offset, listbox_p->selected, listbox_p->count, listbox_p->offset+listbox_p->selected);

	if ( ( (unsigned int) listbox_p->selected + listbox_p->offset) == listbox_p->count-1) {
		printf("GOTO TOP\n");

		listbox_p->offset=0;
		listbox_p->selected=0;
		EG_SlideBar_SetPosition(listbox_p->slidebar_p, (listbox_p->offset)*10);
        	UpdateList(listbox_p);
		return;
	}

	if (listbox_p->selected == (int) listbox_p->listbox_entries-1) {
		if (listbox_p->selected+listbox_p->offset < (int) ListBox_EntryCount(listbox_p)-1) {
			listbox_p->offset++;
			UpdateList(listbox_p);
		}
	}

	if (listbox_p->selected< (int) listbox_p->listbox_entries-1) {
		listbox_p->selected++;
		EG_Button_GetFocus(listbox_p->buttons_p[listbox_p->selected]);
	}

	SetColorToSelected(listbox_p, listbox_p->buttons_p[listbox_p->selected]);
	EG_SlideBar_SetPosition(listbox_p->slidebar_p, (listbox_p->offset)*10);
}

static void PageDown(ListBox *listbox_p)
{
	listbox_p->selected+= listbox_p->listbox_entries-1;
	if (listbox_p->selected>=(int)listbox_p->listbox_entries) {
		listbox_p->offset+= (listbox_p->selected-listbox_p->listbox_entries+1);
		listbox_p->selected = listbox_p->listbox_entries-1;
	}
	
	if (listbox_p->offset+listbox_p->selected >= (int)ListBox_EntryCount(listbox_p)) {
		listbox_p->offset = ListBox_EntryCount(listbox_p)-listbox_p->listbox_entries;
		listbox_p->selected = listbox_p->listbox_entries-1;
	}

//	EG_SlideBar_SetPosition(listbox_p->slidebar_p, (listbox_p->offset+listbox_p->selected) * 10);
	EG_SlideBar_SetPosition(listbox_p->slidebar_p, (listbox_p->offset)*10);
	UpdateList(listbox_p);
}

static void PageUp(ListBox *listbox_p)
{
	listbox_p->selected-= listbox_p->listbox_entries-1;
	if (listbox_p->selected < 0) {
		listbox_p->offset+= listbox_p->selected;
		listbox_p->selected = 0;
	}

	if (listbox_p->offset<0) 
		listbox_p->offset=0;

//	EG_SlideBar_SetPosition(listbox_p->slidebar_p, (listbox_p->offset-listbox_p->listbox_entries)*10);
	EG_SlideBar_SetPosition(listbox_p->slidebar_p, (listbox_p->offset)*10);
	UpdateList(listbox_p);
}

#define DOUBLE_CLICK_THRESHOLD 200
static void CALLBACK_Button_Pressed(EG_Widget *widget_ptr, void *user_ptr)
{
	static unsigned int last_ticks=0, last_id=0;
	unsigned int current_ticks, current_id, difference;

//	int value = ((int) (intptr_t) user_ptr);
	ListBox *listbox_p = (ListBox*) user_ptr;


	int value = (int) strtol(EG_Widget_GetName(widget_ptr), NULL, 10);


	// Set focus to clicked button.
	SetColorToNormal(listbox_p, listbox_p->buttons_p[listbox_p->selected]);
	listbox_p->selected = value;
	SetColorToSelected(listbox_p, listbox_p->buttons_p[listbox_p->selected]);



	// Emulate 'double click' selection (as EG doesn't support double clicks).

	current_ticks = SDL_GetTicks();
	current_id = EG_Widget_GetID(widget_ptr);

	// Calc. time difference. After ~47 days SDL_Ticks will wrap, so handle that too.
	if (last_ticks > current_ticks)
		difference = current_ticks + ( ((Uint32) 0xffffffff) - last_ticks);
	else
		difference = current_ticks - last_ticks;

	if (difference <= DOUBLE_CLICK_THRESHOLD && last_id == current_id) {
		d=1;
		return_value = listbox_p->offset+listbox_p->selected;
	}
	last_ticks = current_ticks;
	last_id = current_id;
}

static void CALLBACK_Slidebar_OnChange(EG_Widget *widget_p, void *v_p)
{
	ListBox *listbox_p = (ListBox*) v_p;

	listbox_p->offset = (EG_SlideBar_GetPosition(widget_p)+1) / 10;
	UpdateList(listbox_p);
}

static void CALLBACK_Slidebar_OnGotFocus(EG_Widget *widget_p, void *)
{
//	ListBox *listbox_p = (ListBox*) v_p;

//	EG_Button_GetFocus(buttons_p[selected]);
}


ListBox* ListBox_Create(SDL_Surface *surface_p)
{
	int i;
	ListBox *new_list;

	if ( (new_list = (ListBox*)EG_Malloc(sizeof(ListBox)) ) == NULL)
		return NULL;

	new_list->title[0] = '\0';
//	new_list->color = MENU_COLORS;
	new_list->color = DARKER_MENU_COLORS;
	new_list->count = 0;
	new_list->entries_p = NULL;
	new_list->alignment = EG_BUTTON_ALIGN_CENTER;

	new_list->window_p = NULL;
	new_list->listbox_entries = 0;
	new_list->slidebar_p = NULL;
	
	for(i=0; i<MAX_ROWS; i++)
		new_list->buttons_p[i]=NULL;

	new_list->selected = 0;
	new_list->offset = 0;

	new_list->surface_p = surface_p;

	new_list->default_index = 0;

	return new_list;
}

void	ListBox_SortEntries(ListBox *listbox_p)
{
	unsigned int ii, i;
	ListBox_Entry tmp;
//	ProgressDialog  *pd_p=ProgressDialog_Create(frame_buffer_p, "Sorting directory entries", 0.0);

	if (listbox_p == NULL || ListBox_EntryCount(listbox_p)<2)
		return;

//	if (pd_p!=NULL) ProgressDialog_Show(pd_p);

	for (ii=0; ii<ListBox_EntryCount(listbox_p); ii++) {
//		if (pd_p!=NULL) ProgressDialog_SetPosition(pd_p, ( 1 / ((float) ListBox_EntryCount(listbox_p)) * ii));

//		SDL_Delay(10);
//		printf("%d %f\n", ii, 1 / ((float) ListBox_EntryCount(listbox_p)) * ii  );

		for (i=0; i<ListBox_EntryCount(listbox_p)-1; i++)
			if (strcasecmp(ListBox_GetEntryName(listbox_p, i)
			 , ListBox_GetEntryName(listbox_p, i+1)) > 0) {

				strcpy(tmp.name, listbox_p->entries_p[i].name);
				tmp.value = listbox_p->entries_p[i].value;
				tmp.user_p = listbox_p->entries_p[i].user_p;

				strcpy(listbox_p->entries_p[i].name
				 , listbox_p->entries_p[i+1].name);
				listbox_p->entries_p[i].value = listbox_p->entries_p[i+1].value;
				listbox_p->entries_p[i].user_p = listbox_p->entries_p[i+1].user_p;

				strcpy(listbox_p->entries_p[i+1].name, tmp.name);
				listbox_p->entries_p[i+1].value = tmp.value;
				listbox_p->entries_p[i+1].user_p = tmp.user_p;
			}
	}
//	if (pd_p!=NULL) ProgressDialog_Free(pd_p);

}

void	ListBox_SetColor(ListBox *listbox_p, SDL_Color color)
{
	if (listbox_p == NULL)
		return;

	listbox_p->color = color;
}

void	ListBox_SetTitle(ListBox *listbox_p, const char *title_p)
{
	if (listbox_p == NULL || title_p == NULL)
		return;

	strncpy(listbox_p->title, title_p, LISTBOX_CAPTIONSIZE);
	if (strlen(title_p)>LISTBOX_CAPTIONSIZE)
		listbox_p->title[LISTBOX_CAPTIONSIZE]='\0';
}

void	ListBox_Free(ListBox *listbox_p)
{
	if (listbox_p == NULL)
		return;

	EG_Window_DestroyAllChildWidgets(listbox_p->window_p);
	EG_Window_Destroy(listbox_p->window_p);

	if (listbox_p->entries_p != NULL)
		free(listbox_p->entries_p);

	EG_Free(listbox_p);
}

EG_BOOL ListBox_AddEntry(ListBox *listbox_p, const char *name_p
 , int user_value, void *user_p)
{
	ListBox_Entry *new_entries_p;

	if (listbox_p == NULL)
		return EG_FALSE;

	if (listbox_p->entries_p == NULL) {
		if ( (listbox_p->entries_p = (ListBox_Entry*)malloc(
		 sizeof(ListBox_Entry)) ) == NULL)
			return EG_FALSE;

	} else {
		if ( (new_entries_p = (ListBox_Entry*)realloc(
		 listbox_p->entries_p, sizeof(ListBox_Entry) * 
		 (listbox_p->count+1) ) ) != NULL)
			listbox_p->entries_p = new_entries_p;
	}
	listbox_p->count++;

	strncpy(listbox_p->entries_p[listbox_p->count-1].name, name_p
	 , LISTBOX_ENTRY_NAMESIZE);
	listbox_p->entries_p[listbox_p->count-1].name[LISTBOX_ENTRY_NAMESIZE]
	 = '\0';

	listbox_p->entries_p[listbox_p->count-1].value = user_value;
	listbox_p->entries_p[listbox_p->count-1].user_p = user_p;

	return EG_TRUE;
}

void ListBox_DeleteEntry(ListBox *listbox_p)
{
	ListBox_Entry *new_entries_p;

	if (listbox_p == NULL)
		return;

	if (listbox_p->entries_p == NULL)
		return;

	// Zero out
	listbox_p->entries_p[listbox_p->count-1].name[0] = '\0';
	listbox_p->entries_p[listbox_p->count-1].value = 0;
	listbox_p->entries_p[listbox_p->count-1].user_p = NULL;

	if (listbox_p->count == 1) {
		printf("deleting only entry\n");
		free(listbox_p->entries_p);
		listbox_p->entries_p = NULL;
	} else {
		// Assume making smaller cant fall, but don't just over-write the pointer (if it does fail we just leak memory).
		// Should use EG wrappers, but EG doesn't have a realloc function.
		printf("Deleting last entry\n");
		if ( (new_entries_p = (ListBox_Entry*)realloc(listbox_p->entries_p, sizeof(ListBox_Entry) * (listbox_p->count-1) ) ) != NULL)
			listbox_p->entries_p = new_entries_p;
	}

	listbox_p->count--;
}

unsigned int ListBox_EntryCount(ListBox *listbox_p)
{
	if (listbox_p == NULL)
		return 0;

	return listbox_p->count;
}

int ListBox_GetEntryValue(ListBox *listbox_p, unsigned int index)
{
	if (listbox_p == NULL || listbox_p->count == 0
	 || listbox_p->entries_p == NULL
	 || index >= listbox_p->count)
		return 0;

	return listbox_p->entries_p[index].value;
}

void* ListBox_GetEntryUserPtr(ListBox *listbox_p, unsigned int index)
{
	if (listbox_p == NULL || listbox_p->count == 0
	 || listbox_p->entries_p == NULL
	 || index >= listbox_p->count)
		return 0;

	return listbox_p->entries_p[index].user_p;
}

EG_BOOL ListBox_SetAlignment(ListBox *listbox_p, int alignment)
{
	if (listbox_p == NULL)
		return EG_FALSE;

	listbox_p->alignment = alignment;

	return EG_TRUE;
}


const char* ListBox_GetEntryName(ListBox *listbox_p, unsigned int index)
{
	if (listbox_p == NULL || listbox_p->count == 0
	 || listbox_p->entries_p == NULL
	 || index >= listbox_p->count)
		return NULL;

	return listbox_p->entries_p[index].name;
}


// Indexed from zero
void ListBox_SetDefaultIndex(ListBox *listbox_p, int index)
{
	if (listbox_p == NULL)
		return;

	if (index <0)
		index = 0;

	if (index >= (int) listbox_p->count)
		index = listbox_p->count-1;

	listbox_p->default_index = (unsigned int) index;
}




static void CreateWindow(ListBox *listbox_p)
{
	static EG_Skin_Button *button_p=NULL;

	unsigned int i, y;
	SDL_Rect win, r;
	EG_Widget *widget_p;
	
	int def_value;

	if (listbox_p == NULL)
		return;

	if (EG_Window_GetDefaultSkin() != NULL) {
#	ifdef GP2X
		char button_s[1024];

		strcpy(button_s, config.files.root_dir);
		strcat(button_s, "/resources/button2.bmp");
		button_p = EG_Skin_Button_CreateNew(button_s, 3);
#	else
		button_p = EG_Skin_Button_CreateNew("../resources/button2.bmp", 3);
#	endif
//		EG_Skin_Button_SetColorKey(button_p, 255, 0, 0);
	}


	def_value = listbox_p->default_index;

//	if (def_value <0 || def_value >= (int) listbox_p->count)
//		def_value = 0;


	/* Determine how many rows we will create
  	 */
	listbox_p->listbox_entries = (listbox_p->count < MAX_ROWS ? listbox_p->count : MAX_ROWS);

	/* Create window
  	 */
//	win = CalcRectCentered(listbox_p->surface_p, 40 + /* 40*5 */ 52*5 + (listbox_p->count > MAX_ROWS ? SCROLLBAR_WIDTH : 0), 8*2 + 2 + (listbox_p->title[0] != '\0' ? 8 : 0) + ( listbox_p->listbox_entries * 10) );

	win = CalcRectCentered(listbox_p->surface_p, 50 + /* 40*5 */ 52*5 + (listbox_p->count > MAX_ROWS ? SCROLLBAR_WIDTH : 0), 8*2 + 2 + 5 + (listbox_p->title[0] != '\0' ? 8 : 0) + ( listbox_p->listbox_entries * 10) );
	listbox_p->window_p = EG_Window_Create("win_listbox", listbox_p->surface_p, listbox_p->color, win);

	r = win; r.w-=10; r.x+=5; r.h-=5; r.y+=1;

	if (listbox_p->title[0] != '\0') {
		widget_p = EG_Label_Create("listbox_title", /* listbox_p->color */ EG_COLOR_ALT, EG_LABEL_ALIGN_CENTER, listbox_p->title, C_1_1(r, 1, 2));
		EG_Window_AddWidget(listbox_p->window_p, widget_p);
		y = 3;
	} else {
		y = 1;
	}

	/* Border around list
	 */
	r = C_1_1(r, y, 2 + listbox_p->listbox_entries*2);

	widget_p = EG_Box_Create("listbox_box", EG_BOX_BORDER_SUNK, listbox_p->color, r );
	EG_Window_AddWidget(listbox_p->window_p, widget_p);
	r.w -= 8; r.h -= 8; r.x +=4; r.y +=4;

	/* Slide bar
	 */
	if (listbox_p->count > MAX_ROWS) {
		SDL_Rect r2;
	
		r2.x = r.x+r.w-SCROLLBAR_WIDTH;
		r2.w = SCROLLBAR_WIDTH;
		r2.y = r.y;
		r2.h = r.h;

		widget_p = EG_SlideBar_Create("listbox_slidebar", listbox_p->color, EG_SLIDEBAR_VERTICAL, r2);
		EG_Window_AddWidget(listbox_p->window_p, widget_p);
		r.w -= SCROLLBAR_WIDTH;

		EG_SlideBar_SetLength(widget_p, listbox_p->count * 10);
		EG_SlideBar_SetStep(widget_p, 10);

		EG_SlideBar_SetMyCallback_OnChange(widget_p, CALLBACK_Slidebar_OnChange, listbox_p);
		EG_SlideBar_SetMyCallback_OnGotFocus(widget_p, CALLBACK_Slidebar_OnGotFocus, listbox_p);

		listbox_p->slidebar_p = widget_p;
	}

	/* List entries
	 */
	for (i=0; i < listbox_p->listbox_entries; i++) {
		char name[20];

		SDL_Rect r3 = C_1_1(r, (i*2), 2);
		r3.h++;

		sprintf(name, "%d\n", i);

		widget_p = EG_Button_Create(name
		 , listbox_p->color, listbox_p->alignment
		 , ListBox_GetEntryName(listbox_p, i)
		 , /* C_1_1(r, (i*2), 2) */ r3);

		// If loading the skin fails, we still want no border around the button.
		if (button_p == NULL) EG_Button_DrawWithoutBorder(widget_p);
		
		EG_Button_SetSkin(widget_p, button_p);
		EG_Button_SetMyCallback_OnClick(widget_p, CALLBACK_Button_Pressed, (void*) listbox_p);
		EG_Window_AddWidget(listbox_p->window_p, widget_p);

		listbox_p->buttons_p[i] = widget_p;
	}


	/* Set default
 	 */
	if (def_value < (int) listbox_p->listbox_entries) {
		listbox_p->offset = 0;
		listbox_p->selected = def_value;
	} else if (def_value > (int) ((listbox_p->count-1)-listbox_p->listbox_entries) ) {
		listbox_p->offset = listbox_p->count-listbox_p->listbox_entries;
		listbox_p->selected = def_value - listbox_p->offset;

		printf("****************** %d %d\n", listbox_p->offset, listbox_p->selected);
	} else {
		listbox_p->selected = 0;
		listbox_p->offset = def_value;
	}


//     listbox_p->selected=listbox_p->offset=0;
      EG_Button_GetFocus(listbox_p->buttons_p[listbox_p->selected]);
      SetColorToSelected(listbox_p, listbox_p->buttons_p[listbox_p->selected]);

//	selected= def_value % MAX_ROWS;
//	offset= def_value - selected;
	UpdateList(listbox_p);
	EG_SlideBar_SetPosition(listbox_p->slidebar_p, (listbox_p->offset)*10);
	//EG_SlideBar_SetPosition(slidebar_p, (offset+NUMBER_OF_ENTRIES)*10);
}



void ListBox_Show(ListBox *listbox_p)
{
	if (listbox_p == NULL || listbox_p->count == 0)
		return;

	EG_Window_Show(listbox_p->window_p);
}

void ListBox_Hide(ListBox *listbox_p)
{
	if (listbox_p == NULL || listbox_p->count == 0)
		return;

	EG_Window_Hide(listbox_p->window_p);
}

int ListBox_Run(ListBox *listbox_p)
{
	int old_delay;
//	unsigned int i;
//	SDL_Rect win, r;
//	EG_Widget *widget_p;
	SDL_Event event;

//	listbox_p = requested_listbox_p;

	return_value = -1;


	if (listbox_p->count == 0) {
		return 0;
	}

	/* If window wasn't previously created, create it
 	 */
	if (listbox_p->window_p == NULL)
		CreateWindow(listbox_p);




	EG_Draw_FlushEventQueue();

        Button_Reset();
        old_delay = Button_GetAutoRepeatDelay();
        Button_SetAutoRepeatDelay(5);

	if (EG_Window_IsHidden(listbox_p->window_p))
		EG_Window_Show(listbox_p->window_p);

//	d=listbox_p->selected=listbox_p->offset=0;
//	EG_Button_GetFocus(listbox_p->buttons_p[listbox_p->selected]);
//	SetColorToSelected(listbox_p, listbox_p->buttons_p[listbox_p->selected]);

	d=0;
	while(!d){
		EG_Graphics_Lock(listbox_p->surface_p);

		while ( SDL_PollEvent(&event) ) {

			if (event.type != SDL_USEREVENT) {

				switch (event.type) {


				case SDL_MOUSEBUTTONDOWN:
					if (event.button.button == SDL_BUTTON_WHEELUP)
						MoveUp(listbox_p);
					if (event.button.button == SDL_BUTTON_WHEELDOWN)
						MoveDown(listbox_p);
					// No break; as mouse events also need to be
					// passed to EG.

				case SDL_KEYUP:
					if (event.key.keysym.sym == SDLK_PAGEDOWN)
						PageDown(listbox_p);
					if (event.key.keysym.sym == SDLK_PAGEUP)
						PageUp(listbox_p);
					if (event.key.keysym.sym == SDLK_ESCAPE)
						d=1;

#				ifdef GP2X
				case SDL_JOYBUTTONUP:
					if (event.jbutton.button == GP2X_BUTTON_SELECT || event.jbutton.button == GP2X_BUTTON_START)
						d=1;
					if (event.jbutton.button == GP2X_BUTTON_L)
						if (! mainWin->ScreenIsReversed())
							PageUp(listbox_p);
						else
							PageDown(listbox_p);
					if (event.jbutton.button == GP2X_BUTTON_R)
						if (! mainWin->ScreenIsReversed())
							PageDown(listbox_p);
						else
							PageUp(listbox_p);
#				endif
				}

				Button_TranslateEvent(&event, mainWin->ScreenIsReversed());

                        } else {
                                switch ( (intptr_t) event.user.data1) {
                                case SDL_USEREVENT_BUTTON_QUIT:
                                        d=1;
                                        break;
      //                          case SDL_USEREVENT_BUTTON_RIGHT:
//                                        EG_Window_MoveSmartFocusLeft(window_p);
    //                                    break;
//                                case SDL_USEREVENT_BUTTON_LEFT:
//                                        EG_Window_MoveSmartFocusRight(window_p);
  //                                      break;

                                case SDL_USEREVENT_BUTTON_UP:
//					EG_Window_MoveSmartFocusUp(window_p);
					MoveUp(listbox_p);
					break;
				case SDL_USEREVENT_BUTTON_DOWN:
//					EG_Window_MoveSmartFocusDown(window_p);
					MoveDown(listbox_p);
					break;

                                case SDL_USEREVENT_BUTTON_CLICK_DOWN:
                                        EG_Window_Trigger_Select_Down(listbox_p->window_p);
                                        break;
                                case SDL_USEREVENT_BUTTON_CLICK_UP:
                                        EG_Window_Trigger_Select_Up(listbox_p->window_p);

					return_value = listbox_p->offset+listbox_p->selected;
					d=1;
                                        break;
                                }
                        }


	
                        /* Send this event to the window:
			 */
                        EG_Window_ProcessEvent(listbox_p->window_p, &event, 0, 0);
                }

                /* Slight delay:
		 */
                SDL_Delay(40);  // 25fps
                Button_RaiseAutoRepeatEvents();
                SDL_UpdateRect(frame_buffer_p, 0,0,0,0);
                SDL_UpdateRect(listbox_p->surface_p, 0,0,0,0);

                /* Make sure EG house keeping is triggered every 40ms:
                 */
                EG_Window_ProcessEvent(listbox_p->window_p, NULL, 0, 0);
                EG_Graphics_Unlock(listbox_p->surface_p);
	}
//	EG_Window_Hide(listbox_p->window_p);

	Button_Reset();
        Button_SetAutoRepeatDelay(old_delay);

//        EG_Window_DestroyAllChildWidgets(window_p);
//        EG_Window_Destroy(window_p);

        return return_value;
}




