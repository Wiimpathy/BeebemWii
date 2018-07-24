/* Various file selectors.
 *
 * This is rubbish code... Please don't look at it, it's a nightmare...
 */

#include <sys/types.h>
#include <dirent.h>

#include "fileselector.h"
#include "messagebox.h"
#include "progressdialog.h"
#include "fileselector_saveas.h"
#include "beebconfig.h"
#include "messages.h"





/* FileTypes:
 *
 * Builds a list of 'file extension types' from a source string.
 *
 * E.g.: "ssd,dsd"
 *
 * Restrictions:
 *
 * + Number of types is limited to MAX_FILE_TYPES.
 * + Source string length is limited to MAX_FILE_TYPES_STRING_LEN chars
 * + You CANNOT add any white space: legal="ssd,dsd", illegal=" ssd ,  dsd".
 *
 * Example:
 *
 * unsigned int i;
 * FileTypes *ft_p;
 *
 * if ( (ft_p=FileTypes_Create("ssd,dsd")) == NULL) {
 * 	fprintf(stderr, "Failed.\n");
 * } else {
 *	for (i=0; i<FileTypes_Count(ft_p); i++)
 *		fprintf(stdout, "%03d [%s]\n", (int)i
 *		 , FileTypes_GetType(ft_p, i) );
 * }
 */

#define MAX_FILE_TYPES 10
#define MAX_FILE_TYPES_STRING_LEN (MAX_FILE_TYPES * 4)

#define MAX_FILENAME_LENGTH	1024

typedef struct {
	char file_types[MAX_FILE_TYPES_STRING_LEN+1];
	unsigned int count;
	char *types_p[MAX_FILE_TYPES];
} FileTypes;


typedef struct selector_level {
	ListBox *listbox_p;

	char path[MAX_FILENAME_LENGTH+1];

	selector_level *parent_p, *child_p;
} SelectorLevel;




static SelectorLevel *load_sel_lev_p;
static SelectorLevel *save_sel_lev_p;

static SelectorLevel *insert0_sel_lev_p;
static SelectorLevel *insert1_sel_lev_p;

static SelectorLevel *save_state_sel_lev_p;
static SelectorLevel *load_state_sel_lev_p;

static SelectorLevel *create_blank_disc_sel_lev_p;




static FileTypes* FileTypes_Create(const char *types_p)
{
	FileTypes *ft_p;
	char *p, *pp;

	/* If file types list is empty or too long, quit with nothing.
 	 */
	if (types_p == NULL || strlen(types_p) == 0
	 || strlen(types_p) > MAX_FILE_TYPES_STRING_LEN) {
		fprintf(stderr, "FileTypes: Source string is empty.\n");
		return NULL;
	}

	/* Allocate and Initialize return value.
  	 */
	if ( (ft_p=(FileTypes*)EG_Malloc( sizeof(FileTypes) ) ) == NULL) {
		fprintf(stderr, "FileTypes: Unable to malloc struct.\n");
		return NULL;
	}

	ft_p->file_types[0] = '\0';
	for(ft_p->count=0; ft_p->count<MAX_FILE_TYPES; ft_p->count++)
		ft_p->types_p[ft_p->count]=ft_p->file_types;
	ft_p->count = 0;
	
	/* Make a copy of the source list, and append a separator so can read
  	 * last entry (",").
 	 */
	strcpy(ft_p->file_types, types_p);
	if (ft_p->file_types[strlen(ft_p->file_types)-1] != ',')
		strcat(ft_p->file_types, ",");

	/* Split the list by changing the separator (",") into a \NUL, and
  	 * storing the start of the segment in the type list array.
 	 */
	p=ft_p->file_types; ft_p->count=0;
	while ( ft_p->count < MAX_FILE_TYPES && (pp=strstr(p, ",")) != NULL) {
		*pp='\0';
		ft_p->types_p[ft_p->count++] = p;
		p = ++pp;
	}

	return ft_p;
}

static void FileTypes_Free(FileTypes *ft_p)
{
	if (ft_p == NULL)
		return;

	EG_Free(ft_p);
}

static unsigned int FileTypes_Count(FileTypes *ft_p)
{
	if (ft_p == NULL)
		return 0;

	return ft_p->count;
}

static const char* FileTypes_GetType(FileTypes *ft_p, unsigned int index)
{
	static const char *p = "";

	if (ft_p == NULL || index >= ft_p->count)
		return p;

	return ft_p->types_p[index];
}


static ListBox* CreateListBox(SDL_Surface *frame_buffer_p, const char *path_p, const char *caption_p, const char *file_types_p, EG_BOOL is_save)
{
	ListBox *listbox_p;
	FileTypes *ft_p;

	DIR *dir_p;
	struct dirent *entry_p;

	char caption[255+1];

	int allow_parent;


				
	/* Quit if not enough information given.
 	 */
	if (frame_buffer_p == NULL || path_p == NULL) {
		fprintf(stderr, "Not enough information provided to open file selector.\n");
		return NULL;
	}


	/* If root dir then don't add (parent) selector item.
 	 */
	if ( strlen(path_p)==0 || strcmp(path_p, "/") == 0 )
		allow_parent = 0;
	else
		allow_parent = 1;

	/* Create file selector.
 	 */
	if ( (listbox_p=ListBox_Create(frame_buffer_p)) == NULL) {
		fprintf(stderr, "Failed to create File Selector.\n");
		return NULL;
	}

	/* Make caption.
  	 */
	if (caption_p != NULL) {
		strncpy(caption, caption_p, 255);
		caption[255]='\0';
	}
	strncat(caption, ": ", 255);
	caption[255]='\0';
	strncat(caption, path_p, 255);
	caption[255]='\0';

	ListBox_SetTitle(listbox_p, caption);
	ListBox_SetAlignment(listbox_p, LISTBOX_ALIGN_LEFT);

	/* Create FileTypes list.
  	 */
	if ( (ft_p=FileTypes_Create(file_types_p)) == NULL) {
		fprintf(stderr, "Failed to create File Type list.\n");
		return NULL;
	}

	if ( (dir_p=opendir(path_p)) == NULL) {
		FileTypes_Free(ft_p);
		ListBox_Free(listbox_p);
		return NULL;
	}
	
	ListBox_AddEntry(listbox_p, "\05 (Cancel)", 0, NULL);

	if (is_save == EG_TRUE) {
//		ListBox_AddEntry(listbox_p, "\06 (Save as new)", 0, NULL);

		char save_as_name[100];
		unsigned int i;

		for (i=0; i<FileTypes_Count(ft_p); i++) {
			sprintf(save_as_name, "\06 (Save as new .%s)", FileTypes_GetType(ft_p, i));
			ListBox_AddEntry(listbox_p, save_as_name, 0, NULL);
		}
	}

	while ( (entry_p = readdir(dir_p)) != NULL) {
		char list_item[MAX_FILENAME_LENGTH+2+1];
		char *ext_p = strrchr(entry_p->d_name, '.') +1;
		unsigned int i;

		if ( strcmp(entry_p->d_name, ".") == 0 )
			continue;

		if ( strcmp(entry_p->d_name, "..") == 0) {
			if (allow_parent == 1)
				ListBox_AddEntry(listbox_p, "\07 (Parent)", 0, NULL);
			continue;
		}

		switch (entry_p->d_type) {

		case DT_REG:
			if ( (ext_p = strrchr(entry_p->d_name, '.')) != NULL) {
				ext_p++;
				for(i=0; i<FileTypes_Count(ft_p); i++)
					if ( strcmp(ext_p
					 , FileTypes_GetType(ft_p, i)) == 0 ) {
	
//					if (strlen(entry_p->d_name) <= 55) {
						list_item[0] = '\11'; list_item[1] = ' ';
						strncpy(list_item+2, entry_p->d_name, MAX_FILENAME_LENGTH);
						list_item[MAX_FILENAME_LENGTH+3]='\0';
						ListBox_AddEntry(listbox_p, list_item, 0, NULL);
//					} else {
//						char errmsg[10*1024];
//						sprintf(errmsg, FILESELECTOR_FILENAMETOOLONG_M, entry_p->d_name);
//						EG_MessageBox(frame_buffer_p, EG_MESSAGEBOX_STOP, FILESELECTOR_FILENAMETOOLONG_T, errmsg, "Continue", NULL, NULL, NULL, 0);
//					}
				}
			}
			break;

		case DT_DIR:
				list_item[0] = '\10'; list_item[1] = ' ';
				strncpy(list_item+2, entry_p->d_name, MAX_FILENAME_LENGTH);
				list_item[MAX_FILENAME_LENGTH+3]='\0';
				ListBox_AddEntry(listbox_p, list_item, 0, NULL);
			break;

		}
	}

	ListBox_SortEntries(listbox_p);


	// [TODO] Search for default selected filename (if is one)
	// ListBox_SetDefaultIndex(tmp_sel_lev_p->listbox_p, 1);

	FileTypes_Free(ft_p);

	return listbox_p;
}

#define SELECTOR_CANCEL 5
#define SELECTOR_SAVEAS 6
#define SELECTOR_PARENT 7
#define SELECTOR_DIR 8
#define SELECTOR_FILE 9

static int FileSelector_GetSelection(ListBox *listbox_p, const char **name_p_p)
{
	int selected;
	const char *name_p;
	static char file_extension[100];

	(void)ListBox_Show(listbox_p);
	selected = ListBox_Run(listbox_p);

	if ( (name_p=ListBox_GetEntryName(listbox_p, selected)) != NULL ) {
		switch (name_p[0]) {
			case SELECTOR_PARENT:
				selected = SELECTOR_PARENT;
				*name_p_p=NULL;
				printf("PARENT\n");
			break;

			case SELECTOR_SAVEAS:
				{
				selected = SELECTOR_SAVEAS;

				// returned name is the file extension if save as.
				strcpy(file_extension, name_p+strlen(name_p)-5);
				file_extension[4]=0;
				*name_p_p=file_extension;
				printf("SAVE_AS\n");
				}
			break;

			case SELECTOR_DIR:
				selected = SELECTOR_DIR;
				*name_p_p=name_p+2;
				printf("Dir=%s\n", name_p+2);
			break;

			case SELECTOR_FILE:
				selected = SELECTOR_FILE;
				*name_p_p=name_p+2;
				printf("File=%s\n", name_p+2);
			break;

			default:
			case SELECTOR_CANCEL:
				selected = SELECTOR_CANCEL;
				*name_p_p=NULL;
				printf("CANCEL\n");
			break;
		}
	} else {
		// CANCEL (escape from listbox, no selection made).
		printf("LISTBOX_ESCAPE\n");
		selected = SELECTOR_CANCEL;
		*name_p_p = NULL;
	}

	return selected;
}


static SelectorLevel* SelectorLevel_Create(const char *path_p, SelectorLevel *parent_p)
{
	SelectorLevel *sel_p;

	if ( (sel_p=(SelectorLevel*)EG_Malloc(sizeof(SelectorLevel)))!=NULL) {
		strncpy(sel_p->path, path_p, MAX_FILENAME_LENGTH);
		sel_p->path[MAX_FILENAME_LENGTH]='\0';

		sel_p->parent_p = parent_p;
		sel_p->child_p = NULL;

		sel_p->listbox_p = NULL;

		/* If the parent already has a child, then we write over it!
  		 */
		if (sel_p->parent_p != NULL) {
			if (sel_p->parent_p->child_p != NULL)
				fprintf(stderr, "SelectorLevel_Create: Wiping pointer to existing child!\n");
			sel_p->parent_p->child_p = sel_p;
		}
	}

	return sel_p;
}

// Will free the current level + all children.
// Frees the listbox if exists too.
static void SelectorLevel_Free(SelectorLevel *sel_p)
{
	if (sel_p == NULL)
		return;

	if (sel_p->parent_p != NULL)
		sel_p->parent_p->child_p = NULL;

	if (sel_p->child_p != NULL)
		SelectorLevel_Free(sel_p->child_p);

	if (sel_p->listbox_p != NULL)
		ListBox_Free(sel_p->listbox_p);

	EG_Free(sel_p);
}

// It doesn't matter where sel_p is in the chain, will free all.
// frees all the listboxes if they exist too.
static void SelectorLevel_FreeAll(SelectorLevel *sel_p)
{
	SelectorLevel *tmp2_sel_p, *tmp_sel_p = sel_p;

	if (sel_p == NULL)
		return;

	// Find root
	while (tmp_sel_p->parent_p != NULL)
		tmp_sel_p=tmp_sel_p->parent_p;

	// Delete them
	while (tmp_sel_p != NULL) {

		tmp2_sel_p = tmp_sel_p;
		tmp_sel_p = tmp_sel_p->child_p;

		if (tmp2_sel_p->listbox_p != NULL)
			ListBox_Free(tmp2_sel_p->listbox_p);

		EG_Free(tmp2_sel_p);
	}
}

static void FileSelector(SelectorLevel **user_sel_lev_p, SDL_Surface *frame_buffer_p, const char *path_p, const char *caption_p, const char *file_types_p, EG_BOOL is_save, EG_BOOL create_only, char *return_value_p)
{
//	ProgressDialog *pd_p;

	SelectorLevel *sel_lev_p, *tmp_sel_lev_p;

        char errmsg[10*1024];
	const char *selected_name_p;
	int d, selected;
	const char *return_name_p=NULL;

	char current_path[MAX_FILENAME_LENGTH+1];
	char *tmp_filename_p; 

//	/* Create and show progress dialog.
//	 */
//	pd_p=ProgressDialog_Create(frame_buffer_p, "Reading folder contents", -1.0);
//	ProgressDialog_Show(pd_p);

	// If just creating the object list, then free the existing one.
	if (*user_sel_lev_p != NULL && create_only == EG_TRUE) {
		SelectorLevel_FreeAll(*user_sel_lev_p); *user_sel_lev_p=NULL;
	}

	// If no object list then create one.
	if (*user_sel_lev_p == NULL) {

		strncpy(current_path, path_p, MAX_FILENAME_LENGTH);
		current_path[MAX_FILENAME_LENGTH]='\0';

		/* Create root of selector level chain
 	 	 */
		sel_lev_p = SelectorLevel_Create(current_path, NULL);

		/* Create root listbox.
	 	 */	
		if ( (sel_lev_p->listbox_p=CreateListBox(frame_buffer_p, current_path, caption_p, file_types_p, is_save)) == NULL) {

//			ProgressDialog_Hide(pd_p);
			SelectorLevel_Free(sel_lev_p);

			sprintf(errmsg, FILESELECTOR_UNABLETOOPENDIR_M, path_p);
			EG_MessageBox(frame_buffer_p, EG_MESSAGEBOX_STOP
			 , FILESELECTOR_UNABLETOOPENDIR_T, errmsg, "Continue", NULL, NULL, NULL, 0); 

			// if in create only mode then allow no return value to be set, crash for everything else.
			if (! (return_value_p == NULL && create_only == EG_TRUE) ) 
				strcpy(return_value_p, "");

			return;
		}
	} else {
		sel_lev_p = *user_sel_lev_p;

		strncpy(current_path, sel_lev_p->path, MAX_FILENAME_LENGTH);
		current_path[MAX_FILENAME_LENGTH]='\0';
	}

	// If just creating the list then quit now.
	if (create_only == EG_TRUE) {
		*user_sel_lev_p = sel_lev_p;
		if (! (return_value_p == NULL && create_only == EG_TRUE) )
			strcpy(return_value_p, "");
		return;
	}

//	ProgressDialog_Hide(pd_p);
	ListBox_Show(sel_lev_p->listbox_p);

	/* Main loop
 	 */
	d=1; while(d) {

		/* Get users selection from list.
 		 */
		selected = FileSelector_GetSelection(sel_lev_p->listbox_p, &selected_name_p);
		switch (selected) {

			case SELECTOR_SAVEAS:
				{
					char *tmp_p, new_name[MAX_FILENAME_LENGTH+1], current_file[MAX_FILENAME_LENGTH+1], *current_file_p;
					const char *new_name_p; 

					strcpy(current_file, Config_GetDiscImageFilename(0));

					current_file_p = strrchr(current_file, '/');
					if (current_file_p == NULL) current_file_p = current_file;
					else current_file_p++;

					if ( (tmp_p=strrchr(current_file, '.')) != NULL) *tmp_p='\0';

					// if a disc in drive then default to that name.
					new_name_p = FileSelector_SaveAs_Show(frame_buffer_p, current_file_p);

					if (new_name_p != NULL && strlen(new_name_p)>0) {
						new_name[0] = '\35'; new_name[1] = ' ';
						strcpy(new_name+2, new_name_p);

						// Test if file exists, if does warn and request overwrite.
						//EG_MessageBox(frame_buffer_p, EG_MESSAGEBOX_INFORMATION, "xyz", new_name, "Continue", NULL, NULL, NULL, 0);

						strcat(current_path, new_name_p);
						strcat(current_path, selected_name_p);
                                		return_name_p = (const char*) current_path;
                                		d=0;
					}
				}
				break;

			case SELECTOR_PARENT:
				if (sel_lev_p->parent_p != NULL) {
					ListBox_Hide(sel_lev_p->listbox_p);

					tmp_sel_lev_p = sel_lev_p;
					sel_lev_p = sel_lev_p->parent_p;

					SelectorLevel_Free(tmp_sel_lev_p);

					strcpy(current_path, sel_lev_p->path);

					SDL_Delay(100);
					ListBox_Show(sel_lev_p->listbox_p);					
				} else {
//					ProgressDialog_Show(pd_p);

					/* set current_dir as the parent.
					 */
					current_path[strlen(current_path)-1] = '\0';
					if ( (tmp_filename_p=strrchr(current_path, '/')) != NULL) {
						*tmp_filename_p++='/';
						*tmp_filename_p='\0';
					} else {
						current_path[0]='\0';
					}

					/* Create new selector level.
					 */
					tmp_sel_lev_p = SelectorLevel_Create(current_path, NULL);
					if ( (tmp_sel_lev_p->listbox_p=CreateListBox(frame_buffer_p, current_path, caption_p, file_types_p, is_save)) == NULL) {
//						ProgressDialog_Hide(pd_p);

						sprintf(errmsg, FILESELECTOR_UNABLETOOPENDIR2_M, path_p);
						EG_MessageBox(frame_buffer_p, EG_MESSAGEBOX_STOP
						 , FILESELECTOR_UNABLETOOPENDIR2_T, errmsg, "Continue", NULL, NULL, NULL, 0);
						d=0;
						return_name_p = NULL;
					}

//					ProgressDialog_Hide(pd_p);

					/* Free current selector level
					 */
					ListBox_Hide(sel_lev_p->listbox_p);
					SelectorLevel_Free(sel_lev_p);

					SDL_Delay(100);

					sel_lev_p = tmp_sel_lev_p;
					ListBox_Show(sel_lev_p->listbox_p);
				}
				break;

			case SELECTOR_DIR:

//				ProgressDialog_Show(pd_p);

				if (strlen(current_path)>0 && current_path[strlen(current_path)-1] != '/') {
					strncat(current_path, "/", MAX_FILENAME_LENGTH);
					current_path[MAX_FILENAME_LENGTH]='\0';
				}
				strncat(current_path, selected_name_p, MAX_FILENAME_LENGTH);
				current_path[MAX_FILENAME_LENGTH]='\0';

				strncat(current_path, "/", MAX_FILENAME_LENGTH);
				current_path[MAX_FILENAME_LENGTH]='\0';

				tmp_sel_lev_p = SelectorLevel_Create(current_path, sel_lev_p);
        			if ( (tmp_sel_lev_p->listbox_p=CreateListBox(frame_buffer_p, current_path, caption_p, file_types_p, is_save)) == NULL) {

//                			ProgressDialog_Hide(pd_p);
                			SelectorLevel_Free(tmp_sel_lev_p);

                			sprintf(errmsg, FILESELECTOR_UNABLETOOPENDIR2_M, path_p);
                			EG_MessageBox(frame_buffer_p, EG_MESSAGEBOX_STOP
                			 , FILESELECTOR_UNABLETOOPENDIR2_T, errmsg, "Continue", NULL, NULL, NULL, 0);
 					break;
	       			}
//				ProgressDialog_Hide(pd_p);

				// Make this new level the current one
				ListBox_Hide(sel_lev_p->listbox_p);
				SDL_Delay(100);
				sel_lev_p = tmp_sel_lev_p;
				ListBox_Show(sel_lev_p->listbox_p);
				break;

			case SELECTOR_FILE:

				if (strlen(current_path)>0 && current_path[strlen(current_path)-1] != '/') {
					strncat(current_path, "/", MAX_FILENAME_LENGTH);
					current_path[MAX_FILENAME_LENGTH]='\0';
				}

				strncat(current_path, selected_name_p, MAX_FILENAME_LENGTH);
				current_path[MAX_FILENAME_LENGTH]='\0';
				return_name_p = (const char*) current_path;
				d=0;
				break;

			case SELECTOR_CANCEL:
			default:
				d=0;
				break;
		}
	}

//	SelectorLevel_FreeAll(sel_lev_p);
//	ProgressDialog_Free(pd_p);

	ListBox_Hide(sel_lev_p->listbox_p);
	*user_sel_lev_p = sel_lev_p;

	if (return_name_p == NULL)
		strcpy(return_value_p, "");
	else
		strcpy(return_value_p, return_name_p);

	return;
}


void FileSelector_Initialize(void)
{
	load_sel_lev_p = NULL;
	save_sel_lev_p = NULL;

	insert0_sel_lev_p = NULL;
	insert1_sel_lev_p = NULL;

	save_state_sel_lev_p = NULL;
	load_state_sel_lev_p = NULL;

	create_blank_disc_sel_lev_p = NULL;
}

void FileSelector_Quit(void)
{
	SelectorLevel_FreeAll(load_sel_lev_p); load_sel_lev_p=NULL;

	SelectorLevel_FreeAll(save_sel_lev_p); save_sel_lev_p=NULL;

	SelectorLevel_FreeAll(insert0_sel_lev_p); insert0_sel_lev_p=NULL;
	SelectorLevel_FreeAll(insert1_sel_lev_p); insert1_sel_lev_p=NULL;

	SelectorLevel_FreeAll(save_state_sel_lev_p); save_state_sel_lev_p=NULL;
	SelectorLevel_FreeAll(load_state_sel_lev_p); load_state_sel_lev_p=NULL;

	SelectorLevel_FreeAll(create_blank_disc_sel_lev_p); create_blank_disc_sel_lev_p=NULL;
}

void FileSelector_Load(SDL_Surface *frame_buffer_p, const char *path_p, const char *file_types_p, char *selection_p)
{
	FileSelector(&load_sel_lev_p, frame_buffer_p, path_p, "Load", file_types_p, EG_FALSE, EG_FALSE, selection_p);
}

void FileSelector_Save(SDL_Surface *frame_buffer_p, const char *path_p, const char *file_types_p, char *selection_p)
{
	FileSelector(&save_sel_lev_p, frame_buffer_p, path_p, "Save", file_types_p, EG_TRUE, EG_FALSE, selection_p);
}


void FileSelector_Insert0(SDL_Surface *frame_buffer_p, const char *path_p, const char *file_types_p, char *selection_p)
{
	FileSelector(&insert0_sel_lev_p, frame_buffer_p, path_p, "Insert 0", file_types_p, EG_FALSE, EG_FALSE, selection_p);
}

void FileSelector_Insert1(SDL_Surface *frame_buffer_p, const char *path_p, const char *file_types_p, char *selection_p)
{
	FileSelector(&insert1_sel_lev_p, frame_buffer_p, path_p, "Insert 1", file_types_p, EG_FALSE, EG_FALSE, selection_p);
}

void FileSelector_SaveState(SDL_Surface *frame_buffer_p, const char *path_p, const char *file_types_p, char *selection_p)
{
	FileSelector(&save_state_sel_lev_p, frame_buffer_p, path_p, "Save state", file_types_p, EG_TRUE, EG_FALSE, selection_p);

	// Load & save state selectors must be recreated next time to add the new item to the list.
	if (strlen(selection_p)>0) {
		SelectorLevel_FreeAll(load_state_sel_lev_p); load_state_sel_lev_p=NULL;
		SelectorLevel_FreeAll(save_state_sel_lev_p); save_state_sel_lev_p=NULL;
	}
}

void FileSelector_LoadState(SDL_Surface *frame_buffer_p, const char *path_p, const char *file_types_p, char *selection_p)
{
	FileSelector(&load_state_sel_lev_p, frame_buffer_p, path_p, "Load state", file_types_p, EG_FALSE, EG_FALSE, selection_p);
}

void FileSelector_CreateBlankDiscImage(SDL_Surface *frame_buffer_p, const char *path_p, const char *file_types_p, char *selection_p)
{
	FileSelector(&create_blank_disc_sel_lev_p, frame_buffer_p, path_p, "Create blank", file_types_p, EG_TRUE, EG_FALSE, selection_p);

	SelectorLevel_FreeAll(load_sel_lev_p); load_sel_lev_p=NULL;
        SelectorLevel_FreeAll(insert0_sel_lev_p); insert0_sel_lev_p=NULL;
        SelectorLevel_FreeAll(insert1_sel_lev_p); insert1_sel_lev_p=NULL;
}

void FileSelector_RecreateStatesSelector(SDL_Surface *frame_buffer_p, const char *path_p, const char *file_types_p, const char *default_p)
{
	unsigned int i;
	const char *tmp_p, *tmp2_p;

	// "/mnt/sd/beebem05/saves/AlienDestroyers2_2.uef"
	// "x AlienDestroyers2_2.uef"

	// Do nothing if no default sent.
	if (default_p == NULL || strlen(default_p)==0) {
		return;
	}

	// Recreate save state listbox:
	SelectorLevel_FreeAll(save_state_sel_lev_p); save_state_sel_lev_p=NULL;
	(void) FileSelector(&save_state_sel_lev_p, frame_buffer_p, path_p, "Save state", file_types_p, EG_TRUE, EG_TRUE, NULL);
	ListBox_SetDefaultIndex(save_state_sel_lev_p->listbox_p, 5);

	for (i=0; i<ListBox_EntryCount(save_state_sel_lev_p->listbox_p); i++) {
		tmp_p = ListBox_GetEntryName(save_state_sel_lev_p->listbox_p, i);
		if (tmp_p != NULL) {
			tmp_p+=2;

			tmp2_p = default_p;
			if (strlen(tmp_p) < strlen(tmp2_p) )
				tmp2_p += strlen(tmp2_p) - strlen(tmp_p);

			printf("[%s] - [%s]\n", tmp_p, tmp2_p);

			if (strcasecmp(tmp_p, tmp2_p) == 0) {
				ListBox_SetDefaultIndex(save_state_sel_lev_p->listbox_p, i);
				break;
			}
		}
	}

	// Recreate load state listbox:
	SelectorLevel_FreeAll(load_state_sel_lev_p); load_state_sel_lev_p=NULL;
	FileSelector(&load_state_sel_lev_p, frame_buffer_p, path_p, "Load state", file_types_p, EG_FALSE, EG_TRUE, NULL);
	if(i>0)	ListBox_SetDefaultIndex(load_state_sel_lev_p->listbox_p, i-1);

}


