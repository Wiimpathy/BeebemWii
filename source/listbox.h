#ifndef LISTBOX_H
#define LISTBOX_H

#include <SDL.h>
#include <gui/gui.h>

/* List entry:
 */
#define LISTBOX_ENTRY_NAMESIZE	256

typedef struct {
	char	name[LISTBOX_ENTRY_NAMESIZE+1];
	int	value;

	void	*user_p;

	// User click callback
} ListBox_Entry;

/* List box:
 */

#define LISTBOX_ALIGN_CENTER	EG_BUTTON_ALIGN_CENTER
#define LISTBOX_ALIGN_LEFT	EG_BUTTON_ALIGN_LEFT
#define LISTBOX_ALIGN_RIGHT	EG_BUTTON_ALIGN_RIGHT

#define LISTBOX_CAPTIONSIZE	64

#define MAX_ROWS 15

typedef struct {
	char	title[LISTBOX_CAPTIONSIZE+1];

	int	alignment;

	SDL_Color color;

	unsigned int count;
	ListBox_Entry *entries_p;

	EG_Window *window_p;
	unsigned int listbox_entries, default_index;

	EG_Widget *slidebar_p;
	EG_Widget *buttons_p[MAX_ROWS];

	int selected, offset;

	SDL_Surface *surface_p;

	// Quit callback
} ListBox;

ListBox* 	ListBox_Create(SDL_Surface *surface_p);
void 		ListBox_Free(ListBox *listbox_p);

void    	ListBox_SetTitle(ListBox *listbox_p, const char *title_p);
void		ListBox_SetColor(ListBox *listbox_p, SDL_Color color);
EG_BOOL 	ListBox_SetAlignment(ListBox *listbox_p, int alignment);


EG_BOOL 	ListBox_AddEntry(ListBox *listbox_p, const char *name_p, int user_value, void *user_p);
void 		ListBox_DeleteEntry(ListBox *listbox_p);
unsigned int 	ListBox_EntryCount(ListBox *listbox_p);
int 		ListBox_GetEntryValue(ListBox *listbox_p, unsigned int index);
void* 		ListBox_GetEntryUserPtr(ListBox *listbox_p, unsigned int index);
const char* 	ListBox_GetEntryName(ListBox *listbox_p, unsigned int index);

void    	ListBox_SortEntries(ListBox *listbox_p);

void 		ListBox_SetDefaultIndex(ListBox *listbox_p, int index);

void		ListBox_Show(ListBox *listbox_p);
int 		ListBox_Run(ListBox *listbox_p);
void		ListBox_Hide(ListBox *listbox_p);

#endif
