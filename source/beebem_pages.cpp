/* The BeebEm GUI pages:
 *
 * A simple GUI for the GP2X.
 */
 


#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>
#include <stdio.h>

#include <gui/log.h>
#include <SDL.h>
#include "messagebox.h"
#include "sysvia.h"
#include "main.h"
#include "disc8271.h"
#include "video.h"
#include "beebconfig.h"
#include "beebem_pages.h"
#include "keyboard.h"
#include "button.h"
#include "6502core.h"
#include "sdl.h"
#include "keyboardmapper.h"
#include "fileselector.h"
#include "screensaver.h"
#include "uefstate.h"

#include "stick.h"
#include "messages.h"


/* Configuration state:
 */
extern BeebConfig config;


/* Quit option from main:
 */
void QuitEmulator(void);

#ifdef GEKKO
int forceReset = 0;
#endif

/* Default GUI size (fixed and none scalable for GP2x, unlike UNIX BeebEm):
 */
#define GUI_WIDTH       320
#define GUI_HEIGHT      240


/* BeebEm Pages:
 */

typedef struct {
	EG_Window 	*win;

	EG_Widget	*lbl_title;
	EG_Widget	*box_sunk_page_list_area;

	/* Menu options:
	 */
	EG_Widget	*but_run_disc;
	EG_Widget	*but_save_state;
	EG_Widget	*but_load_state;

	EG_Widget	*but_insert_disc_in_0;
	EG_Widget	*but_insert_disc_in_1;
	EG_Widget	*but_configure_game_options;
	EG_Widget	*but_configure_game_buttons;
	EG_Widget	*but_save_game_config;

	EG_Widget	*but_screensaver;

	EG_Widget	*but_volume;
	EG_Widget	*but_stick;

	EG_Widget	*but_reset;
	EG_Widget	*but_quit;

	/* Okay:
	 */
	EG_Widget	*but_okay;

} Page_Menu;

typedef struct {
	EG_Window	*win;

	EG_Widget	*box_speed;
	EG_Widget	*lbl_speed;
	EG_Widget	*rgp_speed;
	EG_Widget	*rbt_speed_150;
	EG_Widget	*rbt_speed_160;
	EG_Widget	*rbt_speed_170;
	EG_Widget	*rbt_speed_180;
	EG_Widget	*rbt_speed_190;
	EG_Widget	*rbt_speed_200;
	EG_Widget	*rbt_speed_210;
	EG_Widget	*rbt_speed_220;
	EG_Widget	*rbt_speed_230;
	EG_Widget	*rbt_speed_240;

	EG_Widget	*lbl_latency;
	EG_Widget	*lbl_latencytxt;
	EG_Widget	*lbl_latencyhelp;

	EG_Widget	*lbl_latency_yes;
	EG_Widget	*lbl_latency_no;

	EG_Widget	*box_scaling;
	EG_Widget	*lbl_scaling;
	EG_Widget	*rgp_scaling;
	EG_Widget	*rbt_scaling_top;
	EG_Widget	*rbt_scaling_centered;
	EG_Widget	*rbt_scaling_bottom;
	EG_Widget	*rbt_scaling_scaled;

	EG_Widget	*tck_flipped;
	EG_Widget	*tck_cursor;
	EG_Widget	*tck_nudgevhold;

	EG_Widget	*box_protection;
	EG_Widget	*lbl_protection;
	EG_Widget	*lbl_protection_0;
	EG_Widget	*rgp_protection_0;
	EG_Widget	*rbt_protection_0_rw;
	EG_Widget	*rbt_protection_0_ro;
	EG_Widget	*lbl_protection_1;
	EG_Widget	*rgp_protection_1;
	EG_Widget	*rbt_protection_1_rw;
	EG_Widget	*rbt_protection_1_ro;

	EG_Widget	*but_back;

} Page_GameOptions1;



typedef struct {
	EG_Window	*win;

	EG_Widget	*box, *lbl;

	EG_Widget	*lbl_left, *but_left;
	EG_Widget	*lbl_right, *but_right;
	EG_Widget	*lbl_up, *but_up;
	EG_Widget	*lbl_down, *but_down;
	EG_Widget	*lbl_click, *but_z;
	/* Player #1 buttons */
	EG_Widget	*lbl_a, *but_a;
	EG_Widget	*lbl_b, *but_b;
	EG_Widget	*lbl_x, *but_x;
	EG_Widget	*lbl_y, *but_y;
	EG_Widget	*lbl_classic_a, *but_classic_a;
	EG_Widget	*lbl_classic_b, *but_classic_b;
	EG_Widget	*lbl_classic_x, *but_classic_x;
	EG_Widget	*lbl_classic_y, *but_classic_y;
	EG_Widget	*lbl_select, *but_minus;
	EG_Widget	*lbl_classic_l, *but_classic_l;
	EG_Widget	*lbl_classic_r, *but_classic_r;	

	EG_Widget	*box_f, *lbl_f;

	EG_Widget	*lbl_f_left,  *but_f_left;
	EG_Widget	*lbl_f_right, *but_f_right;
	EG_Widget	*lbl_f_up,    *but_f_up;
	EG_Widget	*lbl_f_down,  *but_f_down;
	EG_Widget	*lbl_f_click, *but_f_z;

	/* Player #2 buttons */
	EG_Widget	*lbl_f_a, *but_f_a;
	EG_Widget	*lbl_f_b, *but_f_b;
	EG_Widget	*lbl_f_x, *but_f_x;
	EG_Widget	*lbl_f_y, *but_f_y;
	EG_Widget	*lbl_f_classic_a, *but_f_classic_a;
	EG_Widget	*lbl_f_classic_b, *but_f_classic_b;
	EG_Widget	*lbl_f_classic_x, *but_f_classic_x;
	EG_Widget	*lbl_f_classic_y, *but_f_classic_y;
	EG_Widget	*lbl_f_classic_l, *but_f_classic_l;
	EG_Widget	*lbl_f_classic_r, *but_f_classic_r;	
	//EG_Widget	*lbl_f_select, *but_f_minus;
	EG_Widget	*but_back;

} Page_GameOptions2;




typedef struct {
	EG_Window	*win;

	EG_Widget	*lbl_title;
	EG_Widget	*box;

	EG_Widget	*rgp;
	EG_Widget	*rbt_0, *rbt_10, *rbt_20, *rbt_30, *rbt_40, *rbt_50, *rbt_60, *rbt_70, *rbt_80, *rbt_90, *rbt_100;

} Page_Volume;



/* Global GUI instance struct:
 */
typedef struct {
	Page_Menu menu;
	Page_GameOptions1 game_options_1;
	Page_GameOptions2 game_options_2;
	Page_Volume volume;

	/* Flags:
	 */
	EG_BOOL		config_changed;

} BeebEm_Pages;


/* GUI instance variable:
 */
static BeebEm_Pages gui, *gui_p = &gui;

static SDL_Color default_colors[256];
static SDL_Color frame_buffer_colors[256];

static void BackupPalette(SDL_Surface *surface_p, SDL_Color *palette_p)
{
	int i;

	for(i=0; i<surface_p->format->palette->ncolors; i++)
		palette_p[i] = surface_p->format->palette->colors[i];
}

void BeebEmPages_SetPalette(SDL_Surface *surface_p)
{
	BackupPalette(surface_p, default_colors);
}

/* Private GUI functions:
 */
static EG_BOOL 	Menu_Make(SDL_Surface *frame_buffer_p, BeebEm_Pages *gui_p);
static void 	Menu_Destroy(BeebEm_Pages *gui_p);

static EG_BOOL	GameOptions1_Make(SDL_Surface *frame_buffer_p, BeebEm_Pages *gui_p);
static void 	GameOptions1_Destroy(BeebEm_Pages *gui_p);
static void 	BeebEmPages_ShowGameOptions1Page(BeebEm_Pages *gui_p);

static EG_BOOL	GameOptions2_Make(SDL_Surface *frame_buffer_p, BeebEm_Pages *gui_p);
static void 	GameOptions2_Destroy(BeebEm_Pages *gui_p);
static void 	BeebEmPages_ShowGameOptions2Page(BeebEm_Pages *gui_p);

static EG_BOOL	Volume_Make(SDL_Surface *frame_buffer_p, BeebEm_Pages *gui_p);
static void	Volume_Destroy(BeebEm_Pages *gui_p);
static void 	BeebEmPages_ShowVolumePage(BeebEm_Pages *gui_p);

static void 	GameOptions2_MapKeyNames(BeebEm_Pages *gui_p);

/*
 *
 * I N T E R F A C E :
 *
 */

static EG_Skin_Box *window_box_p = NULL;
static EG_Skin_Button *button_button_p = NULL;
static EG_Skin_Button *togglebutton_button_p = NULL;
static EG_Skin_Button *tickbox_button_p = NULL;


void BeebEmPages_Skin_Make(SDL_Surface *frame_buffer_p)
{
	if (!config.settings.skin) return;

//#ifdef GP2X
	char window_s[4][1024];

	strcpy(window_s[0], config.files.root_dir);
	strcat(window_s[0], "/resources/window_top.bmp");

	strcpy(window_s[1], config.files.root_dir);
	strcat(window_s[1], "/resources/window_bottom.bmp");

	strcpy(window_s[2], config.files.root_dir);
	strcat(window_s[2], "/resources/window_left.bmp");

	strcpy(window_s[3], config.files.root_dir);
	strcat(window_s[3], "/resources/window_right.bmp");

	window_box_p = EG_Skin_Box_CreateNew(window_s[0], window_s[1], window_s[2], window_s[3]);

/*#else
        window_box_p = EG_Skin_Box_CreateNew("../resources/window_top.bmp", "../resources/window_bottom.bmp",
        "../resources/window_left.bmp", "../resources/window_right.bmp");
#endif*/
        if (window_box_p == NULL) {
		// log error
	} else {
		SDL_Color cc = MENU_COLORS;

		EG_Skin_Box_SetFillColor(window_box_p, cc.r, cc.g, cc.b);
		EG_Window_SetDefaultSkin(window_box_p);
	}


	/* Configure button skin:
	 */
//#ifdef GP2X
	char button_s[1024];

	strcpy(button_s, config.files.root_dir);
	strcat(button_s, "/resources/button1.bmp");

	button_button_p = EG_Skin_Button_CreateNew(button_s, 3);
/*#else
        button_button_p = EG_Skin_Button_CreateNew("../resources/button1.bmp", 3);
#endif*/
        if (button_button_p == NULL) {
		// log error
        } else {
//	        EG_Skin_Button_SetColorKey(button_button_p, 255, 0, 0);
		EG_Button_SetDefaultSkin(button_button_p);
	}



	/* Configure toggle button skin:
	 */
//#ifdef GP2X
	char togglebutton_s[1024];

	strcpy(togglebutton_s, config.files.root_dir);
	strcat(togglebutton_s, "/resources/button3.bmp");


	togglebutton_button_p = EG_Skin_Button_CreateNew(togglebutton_s, 6);
/*#else
        togglebutton_button_p = EG_Skin_Button_CreateNew("../resources/button3.bmp", 6);
#endif*/
        if (togglebutton_button_p == NULL) {
		// log error
        } else {
//	        EG_Skin_Button_SetColorKey(togglebutton_button_p, 255, 0, 0);
		EG_ToggleButton_SetDefaultSkin(togglebutton_button_p);
	}



	/* Configure tick box skin:
	 */
//#ifdef GP2X
	char tickbox_s[1024];

	strcpy(tickbox_s, config.files.root_dir);
	strcat(tickbox_s, "/resources/tickbox_border.bmp");

	tickbox_button_p = EG_Skin_Button_CreateNew(tickbox_s, 6);
/*#else
        tickbox_button_p = EG_Skin_Button_CreateNew("../resources/tickbox_border.bmp", 6);
#endif*/
        if (tickbox_button_p == NULL) {
		// log error
        } else {
//	        EG_Skin_Button_SetColorKey(tickbox_button_p, 255, 0, 0);
		EG_TickBox_SetDefaultSkin(tickbox_button_p);
	}




}

void BeebEmPages_Skin_Free(void)
{
	if (!config.settings.skin) return;

	if (window_box_p!=NULL) EG_Skin_Box_Free(window_box_p);
	if (button_button_p!=NULL) EG_Skin_Button_Free(button_button_p);
	if (togglebutton_button_p!=NULL) EG_Skin_Button_Free(togglebutton_button_p);
	if (tickbox_button_p!=NULL) EG_Skin_Button_Free(tickbox_button_p);
}


/* Initialize the GUI pages:
 */
EG_BOOL BeebEmPages_Create(SDL_Surface *frame_buffer_p)
{
	/* Clear gui structure (sets all pointers to NULL):
	 */
	memset(&gui, 0, sizeof(gui));

	/* Set palette - just copy the frame buffers.
	 */
//	BeebEmPages_SetPalette(frame_buffer_p);

	
	// Load in the virtual keyboard image and use it's palette:
	SDL_Surface *tmp2_p = SDL_LoadBMP(config.files.virtual_keyboard_loc);
	if (tmp2_p != NULL) {
	SDL_SetPalette(frame_buffer_p, SDL_LOGPAL|SDL_PHYSPAL, tmp2_p->format->palette->colors, 0, tmp2_p->format->palette->ncolors);
	SDL_FreeSurface(tmp2_p);
	}


	/* Configure window skin:
	 */
	BeebEmPages_Skin_Make(frame_buffer_p);


	/* Create Menu page:
	 */
	if (Menu_Make(frame_buffer_p, gui_p) != EG_TRUE) {
		EG_Log(EG_LOG_FATAL, dL"Unable to create menu - exiting!", dR);
		return EG_FALSE;
	}

	/* Create Game Options pages:
	 */
	if (GameOptions1_Make(frame_buffer_p, gui_p) != EG_TRUE) {
		EG_Log(EG_LOG_FATAL
		 , dL"Unable to create Game Options 1 page - exiting!", dR);
		return EG_FALSE;
	}

	if (GameOptions2_Make(frame_buffer_p, gui_p) != EG_TRUE) {
		EG_Log(EG_LOG_FATAL
		 , dL"Unable to create Game Options 2 page - exiting!", dR);
		return EG_FALSE;
	}

	/* Create Volume page:
	 */
	if (Volume_Make(frame_buffer_p, gui_p) != EG_TRUE) {
		EG_Log(EG_LOG_FATAL
		 , dL"Unable to create Volume page - exiting!", dR);
		return EG_FALSE;
	}

	return EG_TRUE;
}

/* Destroy GUI pages and free all associated resources:
 */
void BeebEmPages_Destroy(void)
{

	/* Free page resources:
	 */

	Menu_Destroy(gui_p);
	GameOptions1_Destroy(gui_p);
	GameOptions2_Destroy(gui_p);
	Volume_Destroy(gui_p);

	BeebEmPages_Skin_Free();

	/* Trash our GUI instance just to be sure:
	 */
	memset(&gui, 0, sizeof(gui));
}

/* This is our main event loop, all GUI page processing is handled
 * here:
 */

/* This variable should only be directly referenced in ProcessEvents, for
 * everything else set it via ChangeMenuTo.
 */
static EG_Window *window_p = NULL;

/* Will switch the displayed window (page) in a way ProcessEvents
 * can understand. If NULL is passed as the new window, the current
 * window is hidden and no new window is shown.
 */
static void ChangeMenuTo(EG_Window *new_window_p)
{
	/* If a window is currently shown, hide it:
	 */
	if (window_p != NULL)
		EG_Window_Hide(window_p);

	/* If a new window is to be shown, show it:
	 */
	if (new_window_p != NULL) {
		window_p = new_window_p;
		EG_Window_Show(new_window_p); // [TODO] make this window_p
	}
}

static void ProcessEvents()
{
	SDL_Surface *frame_buffer_p;
	SDL_Event event;
	int old_delay;

	frame_buffer_p = EG_Window_GetSurface(window_p);

	Button_Reset();
	old_delay = Button_GetAutoRepeatDelay();
	Button_SetAutoRepeatDelay(5);

	do {


		EG_Graphics_Lock(frame_buffer_p);


		while ( SDL_PollEvent(&event) ) {


			if (event.type != SDL_USEREVENT) {
				Button_TranslateEvent(&event, mainWin->ScreenIsReversed());
			} else {
				switch ( (intptr_t) event.user.data1) {
				case SDL_USEREVENT_BUTTON_QUIT:
					EG_Window_Hide(window_p);
					break;
				case SDL_USEREVENT_BUTTON_LEFT:
					EG_Window_MoveSmartFocusLeft(window_p);
					break;

				case SDL_USEREVENT_BUTTON_RIGHT:
					EG_Window_MoveSmartFocusRight(window_p);
					break;

				case SDL_USEREVENT_BUTTON_UP:
					EG_Window_MoveSmartFocusUp(window_p);
					break;

				case SDL_USEREVENT_BUTTON_DOWN:
					EG_Window_MoveSmartFocusDown(window_p);
					break;

				case SDL_USEREVENT_BUTTON_CLICK_DOWN:
					EG_Window_Trigger_Select_Down(window_p);
					break;

				case SDL_USEREVENT_BUTTON_CLICK_UP:
					EG_Window_Trigger_Select_Up(window_p);
					break;
				}
			}

			/* Send this event to the window:
		 	 */
			EG_Window_ProcessEvent(window_p, &event, 0, 0);
		}

		/* Slight delay:
		 */
		SDL_Delay(40);	// 25fps
		Button_RaiseAutoRepeatEvents();
		SDL_UpdateRect(frame_buffer_p, 0,0,0,0);

		/* Make sure EG house keeping is triggered every 40ms:
		 */
		EG_Window_ProcessEvent(window_p, NULL, 0, 0);

		EG_Graphics_Unlock(frame_buffer_p);

	} while (! EG_Window_IsHidden(window_p) );

	Button_Reset();
	Button_SetAutoRepeatDelay(old_delay);
}



/*
 *
 * M E N U :
 *
 */

void BeebEmPages_RenderBackgroundImage(SDL_Surface *surface_p)
{
	EG_Graphics_SetPattern(10);
	EG_Graphics_SetForeColor(SDL_MapRGB(surface_p->format,96, 96 ,96));
	EG_Graphics_SetBackColor(SDL_MapRGB(surface_p->format,0,  0  ,96));
	EG_Graphics_DrawFilledRect(surface_p, 0, 0, surface_p->w, surface_p->h);
}

#include <aesndlib.h>

void BeebEmPages_ShowMenu(void)
{
	SDL_Surface *surface_p;

	/* Freeze sound:
	 */
#ifdef GEKKO
	PauseAudio();
#endif
	/* Store frame buffer's palette and set to menus palette:
	 */
	surface_p = EG_Window_GetSurface(gui_p->menu.win);
	if (surface_p == NULL) return;

	/* Clear screen:
	 */

	SDL_Color c= MENU_COLORS;
	SDL_FillRect(surface_p, NULL, SDL_MapRGB(surface_p->format, c.r, c.g, c.b));	
	SDL_UpdateRect(surface_p, 0,0,0,0);

//	BackupPalette(surface_p, frame_buffer_colors);

	/* Draw background:
	 */

	/* Display the menu:
	 */
	ChangeMenuTo(gui_p->menu.win);

	/* We freeze the emulator and call our own SDL event handler
	 * here. This event handler is also used for the other pages
	 * if launched via this menu.
	 *
	 * (Some pages may also work via alternative event handlers later).
	 */
	ProcessEvents();	

	/* Restore frame buffer's palette:
	 */
//	SDL_SetPalette(surface_p, SDL_LOGPAL|SDL_PHYSPAL, frame_buffer_colors, 0, 256);

	/* Unfreeze sound:
	 */
#ifdef GEKKO
	ResumeAudio();
#endif
}

static void Menu_Callback_Okay(EG_Widget*, void* /*user_p*/)
{
	ChangeMenuTo(NULL);
}

static void Menu_Callback_Reset(EG_Widget*, void*)
{
//	Init6502core();
//	Disc8271_reset();

	if (EG_MessageBox(frame_buffer_p, EG_MESSAGEBOX_QUESTION, BEEBEM_PAGES_RESET_T, BEEBEM_PAGES_RESET_M, "Yes", "No", NULL, NULL, 2) == 1) {
	ChangeMenuTo(NULL);
	if (mainWin!=NULL) mainWin->Reset();
	}
}

static void Menu_Callback_Game_Options_1(EG_Widget*, void* user_p)
{
	BeebEm_Pages *gui_p = (BeebEm_Pages*) user_p;

	BeebEmPages_ShowGameOptions1Page(gui_p);
}

static void Menu_Callback_Game_Options_2(EG_Widget*, void* user_p)
{
	BeebEm_Pages *gui_p = (BeebEm_Pages*) user_p;
	BeebEmPages_ShowGameOptions2Page(gui_p);
}

static void Menu_Callback_Volume(EG_Widget*, void* user_p)
{
	BeebEm_Pages *gui_p = (BeebEm_Pages*) user_p;

	BeebEmPages_ShowVolumePage(gui_p);
}

static void Menu_Callback_ScreenSaver(EG_Widget*, void* /*user_p*/)
{
//	const char *filename_p;
	//BeebEm_Pages *gui_p = (BeebEm_Pages*) user_p;

	char filename[1024];

	FileSelector_CreateBlankDiscImage(frame_buffer_p, (const char*) Config_GetDiscImagePath(), "dsd,ssd", filename);
	if (strlen(filename)>0) {

			printf("Creating blank image: '%s'\n", filename);

		if (strcasecmp(filename+(strlen(filename)-4), ".ssd")) 
			CreateDiscImage((char*)filename, 0, 1, 80);
		else if (strcasecmp(filename+(strlen(filename)-4), ".dsd"))
			CreateDiscImage((char*)filename, 0, 2, 80);

//			EG_MessageBox(frame_buffer_p, EG_MESSAGEBOX_INFORMATION,"Successful", "State loaded OK!", "Continue", NULL, NULL, NULL, 0);
	}
#ifndef GEKKO
	sync();
#endif
}

static void Menu_Callback_SaveConfig(EG_Widget*, void*)
{
	// [TODO] If no disc inserted then add error message:
        if (strlen(config.drive_0_path) == 0) {
		printf("No disc in drive 0, nothing to map against.\n");

		EG_MessageBox(frame_buffer_p, EG_MESSAGEBOX_STOP, BEEBEM_PAGES_SAVECONFIGNODISC_T, BEEBEM_PAGES_SAVECONFIGNODISC_M, "Continue", NULL, NULL, NULL, 0);
		return;
	}

	if (BBC_Keyboard_SaveConfig() == 0) {
		EG_MessageBox(frame_buffer_p, EG_MESSAGEBOX_STOP, BEEBEM_PAGES_SAVECONFIGFAILED_T, BEEBEM_PAGES_SAVECONFIGNODISC_M, "Continue", NULL, NULL, NULL, 0);
	} else {
		EG_MessageBox(frame_buffer_p, EG_MESSAGEBOX_INFORMATION, BEEBEM_PAGES_SAVECONFIGSUCCESS_T, BEEBEM_PAGES_SAVECONFIGSUCCESS_M, "Continue", NULL, NULL, NULL, 0);
	}
}

static void Menu_Callback_Quit(EG_Widget*, void*)
{
	if (EG_MessageBox(frame_buffer_p, EG_MESSAGEBOX_QUESTION, BEEBEM_PAGES_QUIT_T, BEEBEM_PAGES_QUIT_M, "Yes", "No", NULL, NULL, 2) == 1) {
	ChangeMenuTo(NULL);
	QuitEmulator();
	}

	// Just in case sync on exit.
	//sync();
}


// Temp save/load state callbacks.
static void Menu_Callback_SaveState(EG_Widget*, void*)
{
//	char filename[256];
//	int bank;

//	const char *filename_p;
	char filename[1024];

//	EG_Draw_FlushEventQueue();
//	bank = EG_MessageBox(frame_buffer_p, EG_MESSAGEBOX_QUESTION, "Save state", "Please select a bank to save too.", "1", "2", "3", "4", 1);
//
//	strcpy(filename, config.files.root_dir);
//
//	if ( strlen(filename) > 0 && ( ! ( filename[strlen(filename)-1] == '/'
//	 || filename[strlen(filename)-1] == '\\') ) ) strcat(filename, "/");
//
//	strcat(filename, "saved_state_bank_");
//	filename[strlen(filename)+1]=0;
//	filename[strlen(filename)]='0'+ bank;
//	strcat(filename, ".uef");

	FileSelector_SaveState(frame_buffer_p, (const char*) Config_GetSaveStatePath(), "uef", filename);
	if (strlen(filename) != 0) {
		printf("Saving to bank: '%s'\n", filename);
		SaveUEFState((char*) filename);

		FileSelector_RecreateStatesSelector(frame_buffer_p, (const char*) Config_GetSaveStatePath(), "uef", filename);
	}
}

static void Menu_Callback_LoadState(EG_Widget*, void* /* user_p */)
{
	FILE *test;
//	const char *filename_p;
	//BeebEm_Pages *gui_p = (BeebEm_Pages*) user_p;

	char filename[1024];

	FileSelector_LoadState(frame_buffer_p, (const char*) Config_GetSaveStatePath(), "uef", filename);
	if (strlen(filename) != 0) {
		printf("Loading from bank: '%s'\n", filename);

	      	test = fopen(filename, "r");
	        if (test) {
	                fclose(test);
	                printf("Trying to restore saved state.\n");

			ChangeMenuTo(NULL);
			
			//gui_p->menu.win
			SDL_FillRect(frame_buffer_p, NULL, 0);
			SDL_UpdateRect(frame_buffer_p, 0,0,0,0);

	                LoadUEFState( (char*) filename);

			FileSelector_RecreateStatesSelector(frame_buffer_p, (const char*) Config_GetSaveStatePath(), "uef", filename);

			//EG_Window_RepaintLot(gui_p->menu.win);
			//EG_MessageBox(frame_buffer_p, EG_MESSAGEBOX_INFORMATION,"Successful", "State loaded OK!", "Continue", NULL, NULL, NULL, 0);

	        } else {
			EG_MessageBox(frame_buffer_p, EG_MESSAGEBOX_STOP,BEEBEM_PAGES_LOADSTATEFAILED_T, BEEBEM_PAGES_LOADSTATEFAILED_M, "Continue", NULL, NULL, NULL, 0);
		}
	}
}

static void Menu_Callback_RunDisc(EG_Widget*, void*)
{
//	const char *filename_p;
	char filename[1024];

	forceReset = 1;
	FileSelector_Load(frame_buffer_p, (const char*) Config_GetDiscImagePath(), "ssd,dsd", filename);
	if (strlen(filename) != 0) {

		ChangeMenuTo(NULL);

		SDL_FillRect(frame_buffer_p, NULL, 0);
		SDL_UpdateRect(frame_buffer_p, 0,0,0,0);

		if (mainWin!=NULL) mainWin->Reset();
		Config_LoadDiscConfig(filename);
	}
}

static void Menu_Callback_InsertDisc0(EG_Widget*, void*)
{
//	const char *filename_p;
	char filename[1024];

	FileSelector_Insert0(frame_buffer_p, (const char*) Config_GetDiscImagePath(), "ssd,dsd", filename);
	if (strlen(filename) >= 4) {

                /* single sided disc image:
                 */
                if (strcasecmp(filename + strlen(filename)-4, ".ssd") == 0) {
                        strcpy(config.drive_0_path, filename);
                        LoadSimpleDiscImage((char*) filename, 0, 0, 80);
                }

                /* double sided disc image:
                 */
                if (strcasecmp(filename + strlen(filename)-4, ".dsd") == 0) {
                        strcpy(config.drive_0_path, filename);
                        LoadSimpleDSDiscImage((char*)filename, 0, 80);
                }
	}
}

static void Menu_Callback_InsertDisc1(EG_Widget*, void*)
{
//	const char *filename_p;
	char filename[1024];

	FileSelector_Insert1(frame_buffer_p, (const char*) Config_GetDiscImagePath(), "ssd,dsd", filename);
	if (strlen(filename) >= 4) {

                /* single sided disc image:
                 */
                if (strcasecmp(filename + strlen(filename)-4, ".ssd") == 0) {
                        strcpy(config.drive_0_path, filename);
                        LoadSimpleDiscImage((char*) filename, 1, 0, 80);
                }

                /* double sided disc image:
                 */
                if (strcasecmp(filename + strlen(filename)-4, ".dsd") == 0) {
                        strcpy(config.drive_0_path, filename);
                        LoadSimpleDSDiscImage((char*)filename, 1, 80);
                }
	}
}

static void Menu_Callback_Stick(EG_Widget*, void* /*user_p*/)
{
	ListBox *listbox_p;
//	BeebEm_Pages *gui_p = (BeebEm_Pages*) user_p;
	int choice;

	if ( (listbox_p=ListBox_Create(frame_buffer_p)) == NULL) {
		fprintf(stderr, "Failed to create Stick mode list.\n");
		EG_MessageBox(frame_buffer_p, EG_MESSAGEBOX_STOP, BEEBEM_PAGES_STICKLISTFAILED_T, BEEBEM_PAGES_STICKLISTFAILED_M, "Continue", NULL, NULL, NULL, 0);
		return;
	}

	ListBox_SetTitle(listbox_p, "Stick mode:");
	ListBox_SetAlignment(listbox_p, LISTBOX_ALIGN_LEFT);

#ifdef GEKKO
	ListBox_AddEntry(listbox_p, "NOT USED!", STICKMODE_CRITICAL, NULL);
	ListBox_SetDefaultIndex(listbox_p, 1 );
#else
	ListBox_AddEntry(listbox_p, "Smart: BeebEm 0.5 and before stick control.", STICKMODE_CRITICAL, NULL);
	ListBox_AddEntry(listbox_p, "Multi-key: Diagonals simulate multiple key presses.", STICKMODE_MULTI, NULL);
	ListBox_AddEntry(listbox_p, "Horizontal: Diagonals favour horizontal movement.", STICKMODE_HORIZONTAL, NULL);
	ListBox_AddEntry(listbox_p, "Vertical: Diagonals favour vertical movement.", STICKMODE_VERTICAL, NULL);

	// StickManager_GetMode modes are indexed from 0, so should be ok.
	ListBox_SetDefaultIndex(listbox_p, StickManager_GetMode() );
#endif
	choice = ListBox_Run(listbox_p);
	ListBox_Free(listbox_p);

	if (choice>=0) StickManager_SetMode(choice);
	//GameOptions2_UpdateStickButton(gui_p);
}




static EG_BOOL Menu_Make(SDL_Surface *frame_buffer_p, BeebEm_Pages *gui_p)
{
	SDL_Color c, c2, c3, title_col, help_col;
	SDL_Rect r, r2, r3, r4, r5;

	EG_Widget *w_p;



	/* Default colors:
	 */
	c = c2 = c3 = MENU_COLORS;


	title_col = EG_COLOR_ALT;
	help_col = (SDL_Color) {255-64, 255-64, 0};






	/* Create menu window:
	 */
//	r = (SDL_Rect) { (GUI_WIDTH/4)/2, (GUI_HEIGHT/4)/2
//	 , GUI_WIDTH-(GUI_WIDTH/4), GUI_HEIGHT-(GUI_HEIGHT/4)};

	//r = (SDL_Rect) {48, 16, GUI_WIDTH-96, GUI_HEIGHT-32}; 
	//r = (SDL_Rect) {0,0, GUI_WIDTH, GUI_HEIGHT};

	r = (SDL_Rect) {0,0,320,240};


	gui_p->menu.win = EG_Window_Create("menu_win", frame_buffer_p, c, r);
//	r2 = r; r2.x = 0; r2.y = 0;
	r2 = EG_Window_GetDimension(gui_p->menu.win);
	r2.x +=2; r2.w -=4;

	// For border:
//	r2.x += 16; r2.w -= 32;
//	r2.y += 16; r2.h -= 32;

	/* BeebEm GP2x & Wii (VERSION) label: 
	 */
				r2.w-=10; r2.x+=5;
				r2.y+=4;
#ifdef GEKKO
				gui_p->menu.lbl_title = EG_Label_Create("menu_title", title_col
				 , EG_LABEL_ALIGN_CENTER, "BeebEm for WII (" VERSION ")", C_1_1(r2, 1, 2));
#else
				gui_p->menu.lbl_title = EG_Label_Create("menu_title", title_col
				 , EG_LABEL_ALIGN_CENTER, "BeebEm for GP2X (" VERSION ")", C_1_1(r2, 1, 2));
#endif
				(void) EG_Window_AddWidget(gui_p->menu.win, gui_p->menu.lbl_title);

	/* Box around list of pages:
	 */
				r3 = C_1_1(r2, 4, 38);
				r3.w/=2;
				gui_p->menu.box_sunk_page_list_area = EG_Box_Create("menu_box"
				 , EG_BOX_BORDER_SUNK, c, r3);
				EG_Window_AddWidget(gui_p->menu.win
				 , gui_p->menu.box_sunk_page_list_area);
				r3.x+=4; r3.w-=8;


	r4 = r3; //r4.w/=2;

	/* Load in source surface:
  	 */
			SDL_Surface *owl_p = SDL_LoadBMP(config.files.owl_loc);

			if (owl_p != NULL) {
				r5 = r3; r5.x+=r5.w+10; //r5.y+=6;

				w_p = EG_Image_Create("menu_owl", c, EG_IMAGE_ALIGN_CENTERED, owl_p, r5, (SDL_Color) {255,255,255,0});
				EG_Window_AddWidget(gui_p->menu.win, w_p);

				SDL_FreeSurface(owl_p);
			} else {
				fprintf(stderr, "Failed to load 'owl' image.\n");
			}

	/* Insert and auto run new game disc button:
	 */
 	gui_p->menu.but_run_disc = EG_Button_Create("menu_run_disc", c3
	 , EG_BUTTON_ALIGN_CENTER, "Run new game"
	 , C_1_1(r4, 1, 3) );
	(void) EG_Button_SetMyCallback_OnClick(gui_p->menu.but_run_disc, Menu_Callback_RunDisc, gui_p);
	(void) EG_Window_AddWidget(gui_p->menu.win, gui_p->menu.but_run_disc);
//	(void) EG_Button_DrawWithoutBorder(gui_p->menu.but_run_disc);





	/* Temp save/load state
	 */
	gui_p->menu.but_load_state = EG_Button_Create("menu_load_state", c3
	 , EG_BUTTON_ALIGN_CENTER, "Load state" 
	 , C_1_1(r4, 4, 3) );
	(void) EG_Window_AddWidget(gui_p->menu.win, gui_p->menu.but_load_state);
	(void) EG_Button_SetMyCallback_OnClick(gui_p->menu.but_load_state, Menu_Callback_LoadState, gui_p);


	gui_p->menu.but_save_state = EG_Button_Create("menu_save_state", c3
	 , EG_BUTTON_ALIGN_CENTER, "Save state"
	 , C_1_1(r4, 7, 3) );
	(void) EG_Window_AddWidget(gui_p->menu.win, gui_p->menu.but_save_state);
	(void) EG_Button_SetMyCallback_OnClick(gui_p->menu.but_save_state, Menu_Callback_SaveState, gui_p);





	/* Change disc in drive 0 button:
	 */
	gui_p->menu.but_insert_disc_in_0 = EG_Button_Create("menu_change0", c3
	 , EG_BUTTON_ALIGN_CENTER
	 , "Change disc in drive 0", C_1_1(r4, 10, 3) );
	(void) EG_Window_AddWidget(gui_p->menu.win
	 , gui_p->menu.but_insert_disc_in_0);
	(void) EG_Button_SetMyCallback_OnClick(gui_p->menu.but_insert_disc_in_0, Menu_Callback_InsertDisc0, gui_p);


	/* Change disc in drive 1 button:
	 */
	gui_p->menu.but_insert_disc_in_1 = EG_Button_Create("menu_change1", c3
	 , EG_BUTTON_ALIGN_CENTER
	 , "Change disc in drive 1", C_1_1(r4, 13, 3) );
	(void) EG_Window_AddWidget(gui_p->menu.win
	 , gui_p->menu.but_insert_disc_in_1);
	(void) EG_Button_SetMyCallback_OnClick(gui_p->menu.but_insert_disc_in_1, Menu_Callback_InsertDisc1, gui_p);


	/* Configure settings for game disc (in drive 0) button:
	 */
	gui_p->menu.but_configure_game_options = EG_Button_Create("menu_config1", c3
	 , EG_BUTTON_ALIGN_CENTER, "Configure game options"
	 , C_1_1(r4, 16, 3) );
	(void) EG_Button_SetMyCallback_OnClick(gui_p->menu.but_configure_game_options
	 , Menu_Callback_Game_Options_1, gui_p);
	(void) EG_Window_AddWidget(gui_p->menu.win, gui_p->menu.but_configure_game_options);


	gui_p->menu.but_configure_game_buttons = EG_Button_Create("menu_config2", c3
	 , EG_BUTTON_ALIGN_CENTER, "Configure game buttons"
	 , C_1_1(r4, 19, 3) );
	(void) EG_Button_SetMyCallback_OnClick(gui_p->menu.but_configure_game_buttons
	 , Menu_Callback_Game_Options_2, gui_p);
	(void) EG_Window_AddWidget(gui_p->menu.win, gui_p->menu.but_configure_game_buttons);


	gui_p->menu.but_save_game_config = EG_Button_Create("menu_config3", c3
	 , EG_BUTTON_ALIGN_CENTER, "Save game configuration"
	 , C_1_1(r4, 22, 3) );
	(void) EG_Button_SetMyCallback_OnClick(gui_p->menu.but_save_game_config
	 , Menu_Callback_SaveConfig, gui_p);
	(void) EG_Window_AddWidget(gui_p->menu.win, gui_p->menu.but_save_game_config);





	/* Screen Saver: [
 * */
	gui_p->menu.but_screensaver = EG_Button_Create("menu_screensaver", c3
	 , EG_BUTTON_ALIGN_CENTER, "Create blank disc image",  C_1_1(r4, 25, 3) );
	(void) EG_Button_SetMyCallback_OnClick(gui_p->menu.but_screensaver
	 , Menu_Callback_ScreenSaver, gui_p);

	(void) EG_Window_AddWidget(gui_p->menu.win, gui_p->menu.but_screensaver);




	/* Volume:
	 */
	gui_p->menu.but_volume = EG_Button_Create("menu_volume", c3
	 , EG_BUTTON_ALIGN_CENTER, "Change volume"
	 ,  C_1_1(r4, 28, 3) );
	(void) EG_Button_SetMyCallback_OnClick(gui_p->menu.but_volume
	 , Menu_Callback_Volume, gui_p);

	(void) EG_Window_AddWidget(gui_p->menu.win, gui_p->menu.but_volume);


	/* Stick:
  	 */
	gui_p->menu.but_stick = EG_Button_Create("menu_stick", c3
	 , EG_BUTTON_ALIGN_CENTER, "Change stick mode"
	 ,  C_1_1(r4, 31, 3) );
	(void) EG_Button_SetMyCallback_OnClick(gui_p->menu.but_stick, Menu_Callback_Stick, gui_p);
	(void) EG_Window_AddWidget(gui_p->menu.win, gui_p->menu.but_stick);


	/* Reset emulator button:
	 */
	gui_p->menu.but_reset = EG_Button_Create("menu_reset", c3
	 , EG_BUTTON_ALIGN_CENTER, "Reset", C_1_1(r4, 34, 3) );
	(void) EG_Button_SetMyCallback_OnClick(gui_p->menu.but_reset
	 , Menu_Callback_Reset, gui_p);
	(void) EG_Window_AddWidget(gui_p->menu.win, gui_p->menu.but_reset);



/* [TODO] About, Using BeebEm GP2x, Game specific help
 */


	/* Add Okay button:
	 */
	r.x = r.y = 0;

	//r2.w/=2;

	r2.y-=3;

	gui_p->menu.but_okay = EG_Button_Create("menu_okay_button", c3
	 , EG_BUTTON_ALIGN_CENTER, "Return to emulator", C_1_2(r2, 43, 4) );
	(void) EG_Button_SetMyCallback_OnClick(gui_p->menu.but_okay
	 , Menu_Callback_Okay, gui_p);
	(void) EG_Window_AddWidget(gui_p->menu.win, gui_p->menu.but_okay);

	/* Set focus to okay button when window is opened:
	 */
	EG_Window_SetFocusToThisWidget(gui_p->menu.but_okay);

	/* Quit BeebEm and return to GP2x menu button:
	 */
	gui_p->menu.but_quit = EG_Button_Create("menu_quit", c3
	 , EG_BUTTON_ALIGN_CENTER, "Quit"
	 , C_2_2(r2, 43, 4) );
	(void) EG_Button_SetMyCallback_OnClick(gui_p->menu.but_quit
	 , Menu_Callback_Quit, gui_p);
	(void) EG_Window_AddWidget(gui_p->menu.win, gui_p->menu.but_quit);




	/* Set smart focus peers for all buttons:
	 */
	EG_Widget_SetSmartFocusPeerUp(gui_p->menu.but_run_disc
	 , gui_p->menu.but_okay);
//	EG_Widget_SetSmartFocusPeerDown(gui_p->menu.but_run_disc
//	 , gui_p->menu.but_insert_disc_in_0);



	EG_Widget_SetSmartFocusPeerDown(gui_p->menu.but_run_disc
	 , gui_p->menu.but_load_state);

	EG_Widget_SetSmartFocusPeerUp(gui_p->menu.but_load_state
	 , gui_p->menu.but_run_disc);
	EG_Widget_SetSmartFocusPeerDown(gui_p->menu.but_load_state
	 , gui_p->menu.but_save_state);


	EG_Widget_SetSmartFocusPeerUp(gui_p->menu.but_save_state
	 , gui_p->menu.but_load_state);
	EG_Widget_SetSmartFocusPeerDown(gui_p->menu.but_save_state
	 , gui_p->menu.but_insert_disc_in_0);

	EG_Widget_SetSmartFocusPeerUp(gui_p->menu.but_insert_disc_in_0
	 , gui_p->menu.but_save_state);


//	EG_Widget_SetSmartFocusPeerUp(gui_p->menu.but_insert_disc_in_0
//	 , gui_p->menu.but_run_disc);
	EG_Widget_SetSmartFocusPeerDown(gui_p->menu.but_insert_disc_in_0
	 , gui_p->menu.but_insert_disc_in_1);
//	EG_Widget_SetSmartFocusPeerDown(gui_p->menu.but_insert_disc_in_0, gui_p->menu.but_configure_game_options);


	EG_Widget_SetSmartFocusPeerUp(gui_p->menu.but_insert_disc_in_1
	 , gui_p->menu.but_insert_disc_in_0);
	EG_Widget_SetSmartFocusPeerDown(gui_p->menu.but_insert_disc_in_1
	 , gui_p->menu.but_configure_game_options);



	EG_Widget_SetSmartFocusPeerUp(gui_p->menu.but_configure_game_options
	 , gui_p->menu.but_insert_disc_in_1);
//	EG_Widget_SetSmartFocusPeerUp(gui_p->menu.but_configure_game_options, gui_p->menu.but_insert_disc_in_0);

	EG_Widget_SetSmartFocusPeerDown(gui_p->menu.but_configure_game_options
	 , gui_p->menu.but_configure_game_buttons);

	EG_Widget_SetSmartFocusPeerUp(gui_p->menu.but_configure_game_buttons
	 , gui_p->menu.but_configure_game_options);
	EG_Widget_SetSmartFocusPeerDown(gui_p->menu.but_configure_game_buttons
	 , gui_p->menu.but_save_game_config);

	EG_Widget_SetSmartFocusPeerUp(gui_p->menu.but_save_game_config
	 , gui_p->menu.but_configure_game_buttons);
	EG_Widget_SetSmartFocusPeerDown(gui_p->menu.but_save_game_config
	 , gui_p->menu.but_screensaver);


	EG_Widget_SetSmartFocusPeerUp(gui_p->menu.but_screensaver
	 , gui_p->menu.but_save_game_config);
	EG_Widget_SetSmartFocusPeerDown(gui_p->menu.but_screensaver
	 , gui_p->menu.but_volume);

	EG_Widget_SetSmartFocusPeerUp(gui_p->menu.but_volume
	 , gui_p->menu.but_screensaver);
	EG_Widget_SetSmartFocusPeerDown(gui_p->menu.but_volume
	 , gui_p->menu.but_stick);


	EG_Widget_SetSmartFocusPeerUp(gui_p->menu.but_stick
	 , gui_p->menu.but_volume);
	EG_Widget_SetSmartFocusPeerDown(gui_p->menu.but_stick
	 , gui_p->menu.but_reset);


	EG_Widget_SetSmartFocusPeerUp(gui_p->menu.but_reset
	 , gui_p->menu.but_stick);
	EG_Widget_SetSmartFocusPeerDown(gui_p->menu.but_reset
	 , gui_p->menu.but_okay);

	EG_Widget_SetSmartFocusPeerUp(gui_p->menu.but_okay
	 , gui_p->menu.but_reset);
	EG_Widget_SetSmartFocusPeerDown(gui_p->menu.but_okay
	 , gui_p->menu.but_run_disc);
	EG_Widget_SetSmartFocusPeerRight(gui_p->menu.but_okay
	 , gui_p->menu.but_quit);

//	EG_Widget_SetSmartFocusPeerUp(gui_p->menu.but_quit
//	 , gui_p->menu.but_reset);
//	EG_Widget_SetSmartFocusPeerDown(gui_p->menu.but_quit
//	 , gui_p->menu.but_okay);

	EG_Widget_SetSmartFocusPeerLeft(gui_p->menu.but_quit
	 , gui_p->menu.but_okay);



	return EG_TRUE;
}

static void Menu_Destroy(BeebEm_Pages *gui_p)
{
	EG_Window_DestroyAllChildWidgets(gui_p->menu.win);
	EG_Window_Destroy(gui_p->menu.win);
}




/*
 *
 * G A M E   O P T I O N S   1:
 *
 */

static void BeebEmPages_ShowGameOptions1Page(BeebEm_Pages *gui_p)
{
	// [TODO] When disabling widgets works if cpu = 0 then disable all.


#ifdef GEKKO
	/* Audio latency:
	 */
	if ( config.settings.latency )
		EG_RadioGroup_Select(gui_p->game_options_1.lbl_latency_yes);
	else
		EG_RadioGroup_Select(gui_p->game_options_1.lbl_latency_no);
#else
	/* Processor Speed:
	 */
	switch (config.settings.speed) {
	case 170:
		EG_RadioGroup_Select(gui_p->game_options_1.rbt_speed_150);
		break;
	
	case 180:
		EG_RadioGroup_Select(gui_p->game_options_1.rbt_speed_160);
		break;

	case 190:
		EG_RadioGroup_Select(gui_p->game_options_1.rbt_speed_170);
		break;
	
	case 200:
		EG_RadioGroup_Select(gui_p->game_options_1.rbt_speed_180);
		break;
	
	case 210:
		EG_RadioGroup_Select(gui_p->game_options_1.rbt_speed_190);
		break;
	
	case 220:
		EG_RadioGroup_Select(gui_p->game_options_1.rbt_speed_200);
		break;
	
	case 230:
		EG_RadioGroup_Select(gui_p->game_options_1.rbt_speed_210);
		break;
	
	case 240:
		EG_RadioGroup_Select(gui_p->game_options_1.rbt_speed_220);
		break;
	
	case 250:
		EG_RadioGroup_Select(gui_p->game_options_1.rbt_speed_230);
		break;
	
	case 260:
		EG_RadioGroup_Select(gui_p->game_options_1.rbt_speed_240);
		break;
	
	default:
		EG_RadioGroup_Select(gui_p->game_options_1.rbt_speed_180);
		break;
	}
#endif
	
	/* Scaling:
	 */
	switch (config.settings.vscale)	{
	case CENTER:
		EG_RadioGroup_Select(gui_p->game_options_1.rbt_scaling_centered);
		break;
	case TOP:
		EG_RadioGroup_Select(gui_p->game_options_1.rbt_scaling_top);
		break;
	case BOTTOM:
		EG_RadioGroup_Select(gui_p->game_options_1.rbt_scaling_bottom);
		break;
	case SCALED:
		EG_RadioGroup_Select(gui_p->game_options_1.rbt_scaling_scaled);
		break;
	default:
		EG_RadioGroup_Select(gui_p->game_options_1.rbt_scaling_scaled);
		break;
	}



	/* Floppy 0:
	 */
	if (IsDiscWritable(0))
		EG_RadioGroup_Select(gui_p->game_options_1.rbt_protection_0_rw);
	else
		EG_RadioGroup_Select(gui_p->game_options_1.rbt_protection_0_ro);


	/* Floppy 1:
	 */
	if (IsDiscWritable(1))
		EG_RadioGroup_Select(gui_p->game_options_1.rbt_protection_1_rw);
	else
		EG_RadioGroup_Select(gui_p->game_options_1.rbt_protection_1_ro);


	/* Set 240p mode:
	 */
	if (! config.settings.doublestrike )
		EG_TickBox_Untick(gui_p->game_options_1.tck_flipped);
	else
		EG_TickBox_Tick(gui_p->game_options_1.tck_flipped);


	/* Set Bilinear filter:
	 */
	if (! config.settings.enable_filter )
		EG_TickBox_Untick(gui_p->game_options_1.tck_cursor);
	else
		EG_TickBox_Tick(gui_p->game_options_1.tck_cursor);	


	/* VSyncOffset:
	 */
	if (config.settings.vsyncoffset != 32)
		EG_TickBox_Tick(gui_p->game_options_1.tck_nudgevhold);
	else
		EG_TickBox_Untick(gui_p->game_options_1.tck_nudgevhold);


	/* Set focus to back button:
	 */
	EG_Window_SetFocusToThisWidget(gui_p->game_options_1.but_back);

        ChangeMenuTo(gui_p->game_options_1.win);
}

static void GameOptions1_Callback_Latency(EG_Widget*, void* user_p)
{
	int latency = ((int) (intptr_t) user_p);
	char buf[64];

	sprintf(buf, "%d", latency);

	printf("[%s]\n", buf);
	ActionSetOption("LATENCY", buf);
}

static void GameOptions1_Callback_Speed(EG_Widget*, void* user_p)
{
	int speed = ((int) (intptr_t) user_p);
	char buf[64];

	sprintf(buf, "%d", speed);

	printf("[%s]\n", buf);
	ActionSetOption("SPEED", buf);
}

static void GameOptions1_Callback_Scaling_Top(EG_Widget*, void*)
{
	ActionSetOption("VSCALE", "TOP");
}

static void GameOptions1_Callback_Scaling_Centered(EG_Widget*, void*)
{
	ActionSetOption("VSCALE", "CENTERED");
}

static void GameOptions1_Callback_Scaling_Bottom(EG_Widget*, void*)
{
	ActionSetOption("VSCALE", "BOTTOM");
}

static void GameOptions1_Callback_Scaling_Scaled(EG_Widget*, void*)
{
	ActionSetOption("VSCALE", "SCALED");
}

static void GameOptions1_Callback_Protection_0_RW(EG_Widget*, void*)
{
	ActionSetOption("FD0_TAB", "RW");
}

static void GameOptions1_Callback_Protection_0_RO(EG_Widget*, void*)
{
	ActionSetOption("FD0_TAB", "RO");
}

static void GameOptions1_Callback_Protection_1_RW(EG_Widget*, void*)
{
	ActionSetOption("FD1_TAB", "RW");
}

static void GameOptions1_Callback_Protection_1_RO(EG_Widget*, void*)
{
	ActionSetOption("FD1_TAB", "RO");
}

static void GameOptions1_Callback_Back(EG_Widget*, void* user_p)
{
	BeebEm_Pages *gui_p = (BeebEm_Pages*) user_p;	

	ChangeMenuTo(gui_p->menu.win);
}

static void GameOptions1_Callback_Nudge(EG_Widget *widget_p, void*)
{
	if (EG_TickBox_IsTicked(widget_p))
		Video_SetVHoldDown();
	else
		Video_SetVHoldNormal();
}


static void GameOptions1_Callback_240p(EG_Widget *widget_p, void*)
{
	config.settings.doublestrike^=1;

	char mode[64];
	sprintf(mode, "%d", config.settings.doublestrike);

	ActionSetOption("240p", mode);
}


static void GameOptions1_Callback_Filter(EG_Widget*, void* /* user_p */)
{
	config.settings.enable_filter^=1;

	char mode[64];
	sprintf(mode, "%d", config.settings.enable_filter);

	ActionSetOption("ENABLE_FILTER", mode);
}




static EG_BOOL GameOptions1_Make(SDL_Surface *frame_buffer_p, BeebEm_Pages *gui_p)
{
	SDL_Color c;
	SDL_Rect r, r2, r3;

//	c = (SDL_Color) {0,255-64,255-64,0};
	c = MENU_COLORS;

//	r = (SDL_Rect) {16, 16+8, GUI_WIDTH-32, GUI_HEIGHT-32-16};
 r = (SDL_Rect) {0,0, GUI_WIDTH, GUI_HEIGHT};

	gui_p->game_options_1.win = EG_Window_Create("game_options_1_win", frame_buffer_p, c, r);
	r2 = EG_Window_GetDimension(gui_p->game_options_1.win); r2.x+=2; r2.w -=4;

	r2.x+=4; r2.w-=8; r2.y+=4; r2.h-=8;


	/* Processor speed:
	 * 150 		160 		170		180		190
	 * 200 		210		220		230 		240
	 */

	gui_p->game_options_1.lbl_speed = EG_Label_Create("game_options_1_speed_lbl", EG_COLOR_ALT
	 , EG_LABEL_ALIGN_CENTER, "Audio:", C_1_1(r2, 1, 2));
	(void) EG_Window_AddWidget(gui_p->game_options_1.win, gui_p->game_options_1.lbl_speed);

	r3 = C_1_1(r2, 4, 8);
	gui_p->game_options_1.box_speed = EG_Box_Create("game_options_1_box_speed", EG_BOX_BORDER_SUNK, c, r3);
	(void) EG_Window_AddWidget(gui_p->game_options_1.win, gui_p->game_options_1.box_speed);
	r3.x+=5; r3.w-=10;

	gui_p->game_options_1.lbl_latency = EG_RadioGroup_Create("game_options_1_latency");

	gui_p->game_options_1.lbl_latencytxt = EG_Label_Create("game_options_1_latencytxt_lbl", c
	 , EG_LABEL_ALIGN_LEFT, "Audio latency:", C_1_4(r3, 1, 3));
	(void) EG_Window_AddWidget(gui_p->game_options_1.win, gui_p->game_options_1.lbl_latencytxt);


	gui_p->game_options_1.lbl_latencyhelp = EG_Label_Create("game_options_1_latencyhelp_lbl", c
	 , EG_LABEL_ALIGN_LEFT, "(Better quality but adds a delay)", C_1_4(r3, 4, 3));
	(void) EG_Window_AddWidget(gui_p->game_options_1.win, gui_p->game_options_1.lbl_latencyhelp);

	gui_p->game_options_1.lbl_latency_yes = EG_ToggleButton_Create(
	 "game_options_1_speed_190", c, "Yes", C_4_4(r3, 1, 3) );
	(void) EG_Button_SetMyCallback_OnClick(gui_p->game_options_1.lbl_latency_yes, GameOptions1_Callback_Latency, (void*) 1);
	EG_RadioGroup_AddButton(gui_p->game_options_1.lbl_latency
	 , gui_p->game_options_1.lbl_latency_yes);

	gui_p->game_options_1.lbl_latency_no = EG_ToggleButton_Create(
	 "game_options_1_speed_240", c, "No", C_4_4(r3, 4, 3) );
	(void) EG_Button_SetMyCallback_OnClick(gui_p->game_options_1.lbl_latency_no, GameOptions1_Callback_Latency, (void*) 0);
	EG_RadioGroup_AddButton(gui_p->game_options_1.lbl_latency
	 , gui_p->game_options_1.lbl_latency_no);


	(void) EG_Window_AddWidget(gui_p->game_options_1.win, gui_p->game_options_1.lbl_latency);

	/* Video scaling mode is:
	 * Default		Top, 		Centered, 	Bottom		Scaled
	 */

	gui_p->game_options_1.lbl_scaling = EG_Label_Create("game_options_1_scaling_lbl", EG_COLOR_ALT
	 , EG_LABEL_ALIGN_CENTER, "Video:", C_1_1(r2, 13, 2));
	(void) EG_Window_AddWidget(gui_p->game_options_1.win, gui_p->game_options_1.lbl_scaling);

	r3 = C_1_1(r2, 16, 5+6+3);
	gui_p->game_options_1.box_scaling = EG_Box_Create("game_options_1_box_scaling", EG_BOX_BORDER_SUNK, c, r3);
	(void) EG_Window_AddWidget(gui_p->game_options_1.win, gui_p->game_options_1.box_scaling);
	r3.x+=5; r3.w-=10;

	gui_p->game_options_1.rgp_scaling = EG_RadioGroup_Create("game_options_1_scaling_rgp");

	gui_p->game_options_1.rbt_scaling_top = EG_ToggleButton_Create(
	 "game_options_1_scaling_top", c, "Top", C_1_4(r3, 1, 3) );
	(void) EG_Button_SetMyCallback_OnClick(gui_p->game_options_1.rbt_scaling_top, GameOptions1_Callback_Scaling_Top, NULL);
	EG_RadioGroup_AddButton(gui_p->game_options_1.rgp_scaling
	 , gui_p->game_options_1.rbt_scaling_top);

	gui_p->game_options_1.rbt_scaling_centered = EG_ToggleButton_Create(
	 "game_options_1_scaling_centered", c, "Centered", C_2_4(r3, 1, 3) );
	(void) EG_Button_SetMyCallback_OnClick(gui_p->game_options_1.rbt_scaling_centered, GameOptions1_Callback_Scaling_Centered, NULL);
	EG_RadioGroup_AddButton(gui_p->game_options_1.rgp_scaling
	 , gui_p->game_options_1.rbt_scaling_centered);

	gui_p->game_options_1.rbt_scaling_bottom = EG_ToggleButton_Create(
	 "game_options_1_scaling_bottom", c, "Bottom", C_3_4(r3, 1, 3) );
	(void) EG_Button_SetMyCallback_OnClick(gui_p->game_options_1.rbt_scaling_bottom, GameOptions1_Callback_Scaling_Bottom, NULL);
	EG_RadioGroup_AddButton(gui_p->game_options_1.rgp_scaling
	 , gui_p->game_options_1.rbt_scaling_bottom);

	gui_p->game_options_1.rbt_scaling_scaled = EG_ToggleButton_Create(
	 "game_options_1_scaling_scaled", c, "Scaled", C_4_4(r3, 1, 3) );
	(void) EG_Button_SetMyCallback_OnClick(gui_p->game_options_1.rbt_scaling_scaled, GameOptions1_Callback_Scaling_Scaled, NULL);
	EG_RadioGroup_AddButton(gui_p->game_options_1.rgp_scaling
	 , gui_p->game_options_1.rbt_scaling_scaled);

	EG_Window_AddWidget(gui_p->game_options_1.win, gui_p->game_options_1.rgp_scaling);




	/* Video modes tickbox:
	 */
	gui_p->game_options_1.tck_flipped = EG_TickBox_Create("game_options_1_flipped", c, "Use 240p mode.", C_1_1(r3, 4, 3) );
	(void) EG_Button_SetMyCallback_OnClick(gui_p->game_options_1.tck_flipped, GameOptions1_Callback_240p, gui_p);
	(void) EG_Window_AddWidget(gui_p->game_options_1.win, gui_p->game_options_1.tck_flipped);

	gui_p->game_options_1.tck_cursor = EG_TickBox_Create("game_options_1_cursor", c, "Enable Bilinear filter.", C_1_1(r3, 4+3, 3) );
	(void) EG_Button_SetMyCallback_OnClick(gui_p->game_options_1.tck_cursor, GameOptions1_Callback_Filter, gui_p);
	(void) EG_Window_AddWidget(gui_p->game_options_1.win, gui_p->game_options_1.tck_cursor);

	gui_p->game_options_1.tck_nudgevhold = EG_TickBox_Create("game_options_1_nudge", c, "Nudge CRT (useful for FireTrack/Castle Quest etc).", C_1_1(r3, 4+6, 3) );
	(void) EG_Button_SetMyCallback_OnClick(gui_p->game_options_1.tck_nudgevhold, GameOptions1_Callback_Nudge, gui_p);
	(void) EG_Window_AddWidget(gui_p->game_options_1.win, gui_p->game_options_1.tck_nudgevhold);





	/* Write protection:
	 * 0: Writable, 	Read-only
	 * 1: Writable,		Read-only
	 */

	gui_p->game_options_1.lbl_protection = EG_Label_Create("game_options_1_protection_lbl", EG_COLOR_ALT
	 , EG_LABEL_ALIGN_CENTER, "Floppy drives:", C_1_1(r2, 22+6+3, 2));
	(void) EG_Window_AddWidget(gui_p->game_options_1.win, gui_p->game_options_1.lbl_protection);

	r3 = C_1_1(r2, 25+6+3, 8);
	gui_p->game_options_1.box_protection = EG_Box_Create("game_options_1_box_protection", EG_BOX_BORDER_SUNK, c, r3);
	(void) EG_Window_AddWidget(gui_p->game_options_1.win, gui_p->game_options_1.box_protection);
	r3.x+=4; r3.w-=8;

	gui_p->game_options_1.lbl_protection_0 = EG_Label_Create("game_options_1_protection_0_lbl", c
	 , EG_LABEL_ALIGN_LEFT, "floppy 0:", C_1_3(r3, 1, 3));
	(void) EG_Window_AddWidget(gui_p->game_options_1.win, gui_p->game_options_1.lbl_protection_0);

	gui_p->game_options_1.rgp_protection_0 = EG_RadioGroup_Create("game_options_1_protection_0_rgp");

	gui_p->game_options_1.rbt_protection_0_rw = EG_ToggleButton_Create("game_options_1_protection_0_rw", c, "R/W", C_2_3(r3, 1, 3) );
	(void) EG_Button_SetMyCallback_OnClick(gui_p->game_options_1.rbt_protection_0_rw, GameOptions1_Callback_Protection_0_RW, NULL);
	EG_RadioGroup_AddButton(gui_p->game_options_1.rgp_protection_0, gui_p->game_options_1.rbt_protection_0_rw);

	gui_p->game_options_1.rbt_protection_0_ro = EG_ToggleButton_Create("game_options_1_protection_0_ro", c, "R/O", C_3_3(r3, 1, 3) );
	(void) EG_Button_SetMyCallback_OnClick(gui_p->game_options_1.rbt_protection_0_ro, GameOptions1_Callback_Protection_0_RO, NULL);
	EG_RadioGroup_AddButton(gui_p->game_options_1.rgp_protection_0, gui_p->game_options_1.rbt_protection_0_ro);

	EG_Window_AddWidget(gui_p->game_options_1.win, gui_p->game_options_1.rgp_protection_0);

	gui_p->game_options_1.lbl_protection_1 = EG_Label_Create("game_options_1_protection_1_lbl", c
	 , EG_LABEL_ALIGN_LEFT, "floppy 1:", C_1_3(r3, 4, 3));
	(void) EG_Window_AddWidget(gui_p->game_options_1.win, gui_p->game_options_1.lbl_protection_1);

	gui_p->game_options_1.rgp_protection_1 = EG_RadioGroup_Create("game_options_1_protection_1_rgp");

	gui_p->game_options_1.rbt_protection_1_rw = EG_ToggleButton_Create(
	 "game_options_1_protection_1_rw", c, "R/W", C_2_3(r3, 4, 3) );
	(void) EG_Button_SetMyCallback_OnClick(gui_p->game_options_1.rbt_protection_1_rw, GameOptions1_Callback_Protection_1_RW, NULL);
	EG_RadioGroup_AddButton(gui_p->game_options_1.rgp_protection_1
	 , gui_p->game_options_1.rbt_protection_1_rw);

	gui_p->game_options_1.rbt_protection_1_ro = EG_ToggleButton_Create(
	 "game_options_1_protection_1_ro", c, "R/O", C_3_3(r3, 4, 3) );
	(void) EG_Button_SetMyCallback_OnClick(gui_p->game_options_1.rbt_protection_1_ro, GameOptions1_Callback_Protection_1_RO, NULL);
	EG_RadioGroup_AddButton(gui_p->game_options_1.rgp_protection_1
	 , gui_p->game_options_1.rbt_protection_1_ro);

	EG_Window_AddWidget(gui_p->game_options_1.win, gui_p->game_options_1.rgp_protection_1);

	/* Add back button:
	 */
	r = C_4_4(r2, 34+10-2, 3);
	r.y+=3;

	gui_p->game_options_1.but_back = EG_Button_Create("game_options_1_back_button", c
	 , EG_BUTTON_ALIGN_CENTER, "Back to menu", r );
	(void) EG_Button_SetMyCallback_OnClick(gui_p->game_options_1.but_back
	 , GameOptions1_Callback_Back, gui_p);
	(void) EG_Window_AddWidget(gui_p->game_options_1.win, gui_p->game_options_1.but_back);

	/* Set focus to cancel button when window is opened:
	 */
	EG_Window_SetFocusToThisWidget(gui_p->game_options_1.but_back);

	/* Focus:
	 */

EG_Widget_SetSmartFocusPeerRight(gui_p->game_options_1.rbt_speed_150	, gui_p->game_options_1.rbt_speed_160);
EG_Widget_SetSmartFocusPeerDown(gui_p->game_options_1.rbt_speed_150	, gui_p->game_options_1.rbt_speed_200);

EG_Widget_SetSmartFocusPeerLeft(gui_p->game_options_1.rbt_speed_160	, gui_p->game_options_1.rbt_speed_150);
EG_Widget_SetSmartFocusPeerRight(gui_p->game_options_1.rbt_speed_160	, gui_p->game_options_1.rbt_speed_170);
EG_Widget_SetSmartFocusPeerDown(gui_p->game_options_1.rbt_speed_160	, gui_p->game_options_1.rbt_speed_210);

EG_Widget_SetSmartFocusPeerLeft(gui_p->game_options_1.rbt_speed_170	, gui_p->game_options_1.rbt_speed_160);
EG_Widget_SetSmartFocusPeerRight(gui_p->game_options_1.rbt_speed_170	, gui_p->game_options_1.rbt_speed_180);
EG_Widget_SetSmartFocusPeerDown(gui_p->game_options_1.rbt_speed_170	, gui_p->game_options_1.rbt_speed_220);

EG_Widget_SetSmartFocusPeerLeft(gui_p->game_options_1.rbt_speed_180	, gui_p->game_options_1.rbt_speed_170);
EG_Widget_SetSmartFocusPeerRight(gui_p->game_options_1.rbt_speed_180	, gui_p->game_options_1.rbt_speed_190);
EG_Widget_SetSmartFocusPeerDown(gui_p->game_options_1.rbt_speed_180	, gui_p->game_options_1.rbt_speed_230);

EG_Widget_SetSmartFocusPeerLeft(gui_p->game_options_1.rbt_speed_190	, gui_p->game_options_1.rbt_speed_180);
EG_Widget_SetSmartFocusPeerDown(gui_p->game_options_1.rbt_speed_190	, gui_p->game_options_1.rbt_speed_240);

EG_Widget_SetSmartFocusPeerRight(gui_p->game_options_1.rbt_speed_200	, gui_p->game_options_1.rbt_speed_210);
EG_Widget_SetSmartFocusPeerUp(gui_p->game_options_1.rbt_speed_200	, gui_p->game_options_1.rbt_speed_150);
EG_Widget_SetSmartFocusPeerDown(gui_p->game_options_1.rbt_speed_200	, gui_p->game_options_1.rbt_scaling_top);

EG_Widget_SetSmartFocusPeerLeft(gui_p->game_options_1.rbt_speed_210	, gui_p->game_options_1.rbt_speed_200);
EG_Widget_SetSmartFocusPeerRight(gui_p->game_options_1.rbt_speed_210	, gui_p->game_options_1.rbt_speed_220);
EG_Widget_SetSmartFocusPeerUp(gui_p->game_options_1.rbt_speed_210	, gui_p->game_options_1.rbt_speed_160);
EG_Widget_SetSmartFocusPeerDown(gui_p->game_options_1.rbt_speed_210	, gui_p->game_options_1.rbt_scaling_top);

EG_Widget_SetSmartFocusPeerLeft(gui_p->game_options_1.rbt_speed_220	, gui_p->game_options_1.rbt_speed_210);
EG_Widget_SetSmartFocusPeerRight(gui_p->game_options_1.rbt_speed_220	, gui_p->game_options_1.rbt_speed_230);
EG_Widget_SetSmartFocusPeerUp(gui_p->game_options_1.rbt_speed_220	, gui_p->game_options_1.rbt_speed_170);
EG_Widget_SetSmartFocusPeerDown(gui_p->game_options_1.rbt_speed_220	, gui_p->game_options_1.rbt_scaling_centered);

EG_Widget_SetSmartFocusPeerLeft(gui_p->game_options_1.rbt_speed_230	, gui_p->game_options_1.rbt_speed_220);
EG_Widget_SetSmartFocusPeerRight(gui_p->game_options_1.rbt_speed_230	, gui_p->game_options_1.rbt_speed_240);
EG_Widget_SetSmartFocusPeerUp(gui_p->game_options_1.rbt_speed_230	, gui_p->game_options_1.rbt_speed_180);
EG_Widget_SetSmartFocusPeerDown(gui_p->game_options_1.rbt_speed_230	, gui_p->game_options_1.rbt_scaling_bottom);

EG_Widget_SetSmartFocusPeerLeft(gui_p->game_options_1.rbt_speed_240	, gui_p->game_options_1.rbt_speed_230);
EG_Widget_SetSmartFocusPeerUp(gui_p->game_options_1.rbt_speed_240	, gui_p->game_options_1.rbt_speed_190);
EG_Widget_SetSmartFocusPeerDown(gui_p->game_options_1.rbt_speed_240	, gui_p->game_options_1.rbt_scaling_scaled);

EG_Widget_SetSmartFocusPeerRight(gui_p->game_options_1.rbt_scaling_top	, gui_p->game_options_1.rbt_scaling_centered);
EG_Widget_SetSmartFocusPeerUp(gui_p->game_options_1.rbt_scaling_top	, gui_p->game_options_1.lbl_latency_no);
EG_Widget_SetSmartFocusPeerDown(gui_p->game_options_1.rbt_scaling_top	, gui_p->game_options_1.tck_flipped);

EG_Widget_SetSmartFocusPeerLeft(gui_p->game_options_1.rbt_scaling_centered	, gui_p->game_options_1.rbt_scaling_top);
EG_Widget_SetSmartFocusPeerRight(gui_p->game_options_1.rbt_scaling_centered	, gui_p->game_options_1.rbt_scaling_bottom);
EG_Widget_SetSmartFocusPeerUp(gui_p->game_options_1.rbt_scaling_centered	, gui_p->game_options_1.lbl_latency_no);
EG_Widget_SetSmartFocusPeerDown(gui_p->game_options_1.rbt_scaling_centered	, gui_p->game_options_1.tck_flipped);


EG_Widget_SetSmartFocusPeerLeft(gui_p->game_options_1.rbt_scaling_bottom	, gui_p->game_options_1.rbt_scaling_centered);
EG_Widget_SetSmartFocusPeerRight(gui_p->game_options_1.rbt_scaling_bottom	, gui_p->game_options_1.rbt_scaling_scaled);
EG_Widget_SetSmartFocusPeerUp(gui_p->game_options_1.rbt_scaling_bottom	, gui_p->game_options_1.lbl_latency_no);
EG_Widget_SetSmartFocusPeerDown(gui_p->game_options_1.rbt_scaling_bottom	, gui_p->game_options_1.tck_flipped);


EG_Widget_SetSmartFocusPeerDown(gui_p->game_options_1.lbl_latency_yes	, gui_p->game_options_1.lbl_latency_no);
EG_Widget_SetSmartFocusPeerUp(gui_p->game_options_1.lbl_latency_no	, gui_p->game_options_1.lbl_latency_yes);


EG_Widget_SetSmartFocusPeerLeft(gui_p->game_options_1.rbt_scaling_scaled	, gui_p->game_options_1.rbt_scaling_bottom);
EG_Widget_SetSmartFocusPeerUp(gui_p->game_options_1.rbt_scaling_scaled	, gui_p->game_options_1.lbl_latency_no);
EG_Widget_SetSmartFocusPeerDown(gui_p->game_options_1.rbt_scaling_scaled	, gui_p->game_options_1.tck_flipped);


EG_Widget_SetSmartFocusPeerUp(gui_p->game_options_1.tck_flipped			, gui_p->game_options_1.rbt_scaling_top);
EG_Widget_SetSmartFocusPeerDown(gui_p->game_options_1.tck_flipped		, gui_p->game_options_1.tck_cursor);

EG_Widget_SetSmartFocusPeerUp(gui_p->game_options_1.tck_cursor			, gui_p->game_options_1.tck_flipped);
EG_Widget_SetSmartFocusPeerDown(gui_p->game_options_1.tck_cursor		, gui_p->game_options_1.tck_nudgevhold);


EG_Widget_SetSmartFocusPeerUp(gui_p->game_options_1.tck_nudgevhold		, gui_p->game_options_1.tck_cursor);
EG_Widget_SetSmartFocusPeerDown(gui_p->game_options_1.tck_nudgevhold		, gui_p->game_options_1.rbt_protection_0_ro);




EG_Widget_SetSmartFocusPeerRight(gui_p->game_options_1.rbt_protection_0_rw	, gui_p->game_options_1.rbt_protection_0_ro);
EG_Widget_SetSmartFocusPeerUp(gui_p->game_options_1.rbt_protection_0_rw	, gui_p->game_options_1.tck_nudgevhold);
EG_Widget_SetSmartFocusPeerDown(gui_p->game_options_1.rbt_protection_0_rw	, gui_p->game_options_1.rbt_protection_1_rw);

EG_Widget_SetSmartFocusPeerLeft(gui_p->game_options_1.rbt_protection_0_ro	, gui_p->game_options_1.rbt_protection_0_rw);
EG_Widget_SetSmartFocusPeerUp(gui_p->game_options_1.rbt_protection_0_ro	, gui_p->game_options_1.tck_nudgevhold);
EG_Widget_SetSmartFocusPeerDown(gui_p->game_options_1.rbt_protection_0_ro	, gui_p->game_options_1.rbt_protection_1_ro);

EG_Widget_SetSmartFocusPeerRight(gui_p->game_options_1.rbt_protection_1_rw	, gui_p->game_options_1.rbt_protection_1_ro);
EG_Widget_SetSmartFocusPeerUp(gui_p->game_options_1.rbt_protection_1_rw	, gui_p->game_options_1.rbt_protection_0_rw);

EG_Widget_SetSmartFocusPeerLeft(gui_p->game_options_1.rbt_protection_1_ro	, gui_p->game_options_1.rbt_protection_1_rw);
EG_Widget_SetSmartFocusPeerUp(gui_p->game_options_1.rbt_protection_1_ro	, gui_p->game_options_1.rbt_protection_0_ro);
EG_Widget_SetSmartFocusPeerDown(gui_p->game_options_1.rbt_protection_1_ro	, gui_p->game_options_1.but_back);

EG_Widget_SetSmartFocusPeerUp(gui_p->game_options_1.but_back	, gui_p->game_options_1.rbt_protection_1_ro);

	return EG_TRUE;
}

static void GameOptions1_Destroy(BeebEm_Pages *gui_p)
{
	EG_Window_DestroyAllChildWidgets(gui_p->game_options_1.win);
	EG_Window_Destroy(gui_p->game_options_1.win);
}



/*
 *
 * G A M E   O P T I O N S   2:
 *
 */


static void BeebEmPages_ShowGameOptions2Page(BeebEm_Pages *gui_p)
{
	/* Set focus to back button:
	 */
	EG_Window_SetFocusToThisWidget(gui_p->game_options_2.but_back);
	GameOptions2_MapKeyNames(gui_p);
	ChangeMenuTo(gui_p->game_options_2.win);
}

static void GameOptions2_Callback_Back(EG_Widget*, void* user_p)
{
	BeebEm_Pages *gui_p = (BeebEm_Pages*) user_p;

	ChangeMenuTo(gui_p->menu.win);
}

/*static void GameOptions2_UpdateStickButton(BeebEm_Pages *gui_p)
{
		switch (StickManager_GetMode() ) {

		case STICKMODE_CRITICAL:
			EG_Button_SetCaption(gui_p->game_options_2.but_stick, "Stick mode: Smart.");
			break;

		case STICKMODE_HORIZONTAL:
			EG_Button_SetCaption(gui_p->game_options_2.but_stick, "Stick mode: Horizontal.");
			break;

		case STICKMODE_VERTICAL:
			EG_Button_SetCaption(gui_p->game_options_2.but_stick, " Stick mode: Vertical.");
			break;

		case STICKMODE_MULTI:
		default:
			EG_Button_SetCaption(gui_p->game_options_2.but_stick, " Stick mode: Multi-key.");
			break;
	}
}*/

/*static void GameOptions2_Callback_Stick(EG_Widget*, void* user_p)
{
	ListBox *listbox_p;
	BeebEm_Pages *gui_p = (BeebEm_Pages*) user_p;
	int choice;

	if ( (listbox_p=ListBox_Create(frame_buffer_p)) == NULL) {
		fprintf(stderr, "Failed to create Stick mode list.\n");
		EG_MessageBox(frame_buffer_p, EG_MESSAGEBOX_STOP, BEEBEM_PAGES_STICKLISTFAILED_T, BEEBEM_PAGES_STICKLISTFAILED_M, "Continue", NULL, NULL, NULL, 0);
		return;
	}

	ListBox_SetTitle(listbox_p, "Stick mode:");
	ListBox_SetAlignment(listbox_p, LISTBOX_ALIGN_LEFT);

	ListBox_AddEntry(listbox_p, "Smart: BeebEm 0.5 and before stick control.", STICKMODE_CRITICAL, NULL);
	ListBox_AddEntry(listbox_p, "Multi-key: Diagonals simulate multiple key presses.", STICKMODE_MULTI, NULL);
	ListBox_AddEntry(listbox_p, "Horizontal: Diagonals favour horizontal movement.", STICKMODE_HORIZONTAL, NULL);
	ListBox_AddEntry(listbox_p, "Vertical: Diagonals favour vertical movement.", STICKMODE_VERTICAL, NULL);

	// StickManager_GetMode modes are indexed from 0, so should be ok.
	ListBox_SetDefaultIndex(listbox_p, StickManager_GetMode() );

	choice = ListBox_Run(listbox_p);
	ListBox_Free(listbox_p);

	if (choice>=0) StickManager_SetMode(choice);
	//GameOptions2_UpdateStickButton(gui_p);
}*/

static char* GameOptions2_GetKeyName(int key_number)
{
	static char name_buf[100+1];

	if (key_number == -1) {
		strcpy(name_buf, "NOTHING");
	} else {
		strcpy( name_buf, Keyboard_GetKeyName(key_number) );
		if(strncasecmp(name_buf, "BBC_", 4) == 0)
			strcpy(name_buf, name_buf+4);
	}

	return name_buf;
}


static void GameOptions2_MapKeyNames(BeebEm_Pages *gui_p)
{
	//GameOptions2_UpdateStickButton(gui_p);

	EG_Button_SetCaption(gui_p->game_options_2.but_left, GameOptions2_GetKeyName( Keyboard_GetButtonMapping(WIIGC1_BUTTON_LEFT) ) );
	EG_Button_SetCaption(gui_p->game_options_2.but_right, GameOptions2_GetKeyName( Keyboard_GetButtonMapping(WIIGC1_BUTTON_RIGHT) ) );
	EG_Button_SetCaption(gui_p->game_options_2.but_up, GameOptions2_GetKeyName( Keyboard_GetButtonMapping(WIIGC1_BUTTON_UP) ) );
	EG_Button_SetCaption(gui_p->game_options_2.but_down, GameOptions2_GetKeyName( Keyboard_GetButtonMapping(WIIGC1_BUTTON_DOWN) ) );



	EG_Button_SetCaption(gui_p->game_options_2.but_a, GameOptions2_GetKeyName( Keyboard_GetButtonMapping(WIIGC1_BUTTON_A) ) );
	EG_Button_SetCaption(gui_p->game_options_2.but_b, GameOptions2_GetKeyName( Keyboard_GetButtonMapping(WIIGC1_BUTTON_B) ) );
	EG_Button_SetCaption(gui_p->game_options_2.but_x, GameOptions2_GetKeyName( Keyboard_GetButtonMapping(WIIGC1_BUTTON_X) ) );
	EG_Button_SetCaption(gui_p->game_options_2.but_y, GameOptions2_GetKeyName( Keyboard_GetButtonMapping(WIIGC1_BUTTON_Y) ) );

	EG_Button_SetCaption(gui_p->game_options_2.but_minus, GameOptions2_GetKeyName( Keyboard_GetButtonMapping(WIIGC1_BUTTON_MINUS) ) );
	EG_Button_SetCaption(gui_p->game_options_2.but_z, GameOptions2_GetKeyName( Keyboard_GetButtonMapping(WIIGC1_BUTTON_TRIGGER_Z) ) );


	EG_Button_SetCaption(gui_p->game_options_2.but_classic_a, GameOptions2_GetKeyName( Keyboard_GetButtonMapping(CLASSIC1_BUTTON_A) ) );
	EG_Button_SetCaption(gui_p->game_options_2.but_classic_b, GameOptions2_GetKeyName( Keyboard_GetButtonMapping(CLASSIC1_BUTTON_B) ) );
	EG_Button_SetCaption(gui_p->game_options_2.but_classic_x, GameOptions2_GetKeyName( Keyboard_GetButtonMapping(CLASSIC1_BUTTON_X) ) );
	EG_Button_SetCaption(gui_p->game_options_2.but_classic_y, GameOptions2_GetKeyName( Keyboard_GetButtonMapping(CLASSIC1_BUTTON_Y) ) );

	EG_Button_SetCaption(gui_p->game_options_2.but_classic_l, GameOptions2_GetKeyName( Keyboard_GetButtonMapping(CLASSIC1_BUTTON_L) ) );
	EG_Button_SetCaption(gui_p->game_options_2.but_classic_r, GameOptions2_GetKeyName( Keyboard_GetButtonMapping(CLASSIC1_BUTTON_R) ) );



	/*EG_Button_SetCaption(gui_p->game_options_2.but_volup, GameOptions2_GetKeyName( Keyboard_GetButtonMapping(GP2X_BUTTON_VOLUP) ) );
	EG_Button_SetCaption(gui_p->game_options_2.but_voldown, GameOptions2_GetKeyName( Keyboard_GetButtonMapping(GP2X_BUTTON_VOLDOWN) ) );*/



	EG_Button_SetCaption(gui_p->game_options_2.but_f_left, GameOptions2_GetKeyName( Keyboard_GetButtonMapping(WIIGC2_BUTTON_LEFT) ) );
	EG_Button_SetCaption(gui_p->game_options_2.but_f_right, GameOptions2_GetKeyName( Keyboard_GetButtonMapping(WIIGC2_BUTTON_RIGHT) ) );
	EG_Button_SetCaption(gui_p->game_options_2.but_f_up, GameOptions2_GetKeyName( Keyboard_GetButtonMapping(WIIGC2_BUTTON_UP) ) );
	EG_Button_SetCaption(gui_p->game_options_2.but_f_down, GameOptions2_GetKeyName( Keyboard_GetButtonMapping(WIIGC2_BUTTON_DOWN) ) );



	EG_Button_SetCaption(gui_p->game_options_2.but_f_a, GameOptions2_GetKeyName( Keyboard_GetButtonMapping(WIIGC2_BUTTON_A) ) );
	EG_Button_SetCaption(gui_p->game_options_2.but_f_b, GameOptions2_GetKeyName( Keyboard_GetButtonMapping(WIIGC2_BUTTON_B) ) );
	EG_Button_SetCaption(gui_p->game_options_2.but_f_x, GameOptions2_GetKeyName( Keyboard_GetButtonMapping(WIIGC2_BUTTON_X) ) );
	EG_Button_SetCaption(gui_p->game_options_2.but_f_y, GameOptions2_GetKeyName( Keyboard_GetButtonMapping(WIIGC2_BUTTON_Y) ) );

	EG_Button_SetCaption(gui_p->game_options_2.but_f_classic_a, GameOptions2_GetKeyName( Keyboard_GetButtonMapping(CLASSIC2_BUTTON_A) ) );
	EG_Button_SetCaption(gui_p->game_options_2.but_f_classic_b, GameOptions2_GetKeyName( Keyboard_GetButtonMapping(CLASSIC2_BUTTON_B) ) );
	EG_Button_SetCaption(gui_p->game_options_2.but_f_classic_x, GameOptions2_GetKeyName( Keyboard_GetButtonMapping(CLASSIC2_BUTTON_X) ) );
	EG_Button_SetCaption(gui_p->game_options_2.but_f_classic_y, GameOptions2_GetKeyName( Keyboard_GetButtonMapping(CLASSIC2_BUTTON_Y) ) );


	EG_Button_SetCaption(gui_p->game_options_2.but_f_classic_l, GameOptions2_GetKeyName( Keyboard_GetButtonMapping(CLASSIC2_BUTTON_L) ) );
	EG_Button_SetCaption(gui_p->game_options_2.but_f_classic_r, GameOptions2_GetKeyName( Keyboard_GetButtonMapping(CLASSIC2_BUTTON_R) ) );



	/*EG_Button_SetCaption(gui_p->game_options_2.but_f_volup, GameOptions2_GetKeyName( Keyboard_GetButtonMapping(GP2X_R_BUTTON_VOLUP) ) );
	EG_Button_SetCaption(gui_p->game_options_2.but_f_voldown, GameOptions2_GetKeyName( Keyboard_GetButtonMapping(GP2X_R_BUTTON_VOLDOWN) ) );*/



	/* Set focus to cancel button when window is opened:
	 */
	EG_Window_SetFocusToThisWidget(gui_p->game_options_2.but_back);
}

static void GameOptions2_Callback_MapStickLeft(EG_Widget*, void* user_p)
{
	BeebEm_Pages *gui_p = (BeebEm_Pages*) user_p;
	int new_key;

	new_key = BBCKeyboard_Show( EG_Window_GetSurface(gui_p->game_options_2.win) , Keyboard_GetButtonMapping(WIIGC1_BUTTON_LEFT) );

	if (new_key>=0) {
		EG_Button_SetCaption(gui_p->game_options_2.but_left, GameOptions2_GetKeyName(new_key) );
		Keyboard_SetKey(WIIGC1_BUTTON_LEFT, new_key);
	}
}

static void GameOptions2_Callback_MapStickRight(EG_Widget*, void* user_p)
{
	BeebEm_Pages *gui_p = (BeebEm_Pages*) user_p;
	int new_key;

	new_key = BBCKeyboard_Show( EG_Window_GetSurface(gui_p->game_options_2.win) , Keyboard_GetButtonMapping(WIIGC1_BUTTON_RIGHT) );

	if (new_key>=0) {
		EG_Button_SetCaption(gui_p->game_options_2.but_right, GameOptions2_GetKeyName(new_key) );
		Keyboard_SetKey(WIIGC1_BUTTON_RIGHT, new_key);
	}
}


static void GameOptions2_Callback_MapStickUp(EG_Widget*, void* user_p)
{
	BeebEm_Pages *gui_p = (BeebEm_Pages*) user_p;
	int new_key;

	new_key = BBCKeyboard_Show( EG_Window_GetSurface(gui_p->game_options_2.win) , Keyboard_GetButtonMapping(WIIGC1_BUTTON_UP) );

	if (new_key>=0) {
		EG_Button_SetCaption(gui_p->game_options_2.but_up, GameOptions2_GetKeyName(new_key) );
		Keyboard_SetKey(WIIGC1_BUTTON_UP, new_key);
	}
}


static void GameOptions2_Callback_MapStickDown(EG_Widget*, void* user_p)
{
	BeebEm_Pages *gui_p = (BeebEm_Pages*) user_p;
	int new_key;

	new_key = BBCKeyboard_Show( EG_Window_GetSurface(gui_p->game_options_2.win) , Keyboard_GetButtonMapping(WIIGC1_BUTTON_DOWN) );

	if (new_key>=0) {
		EG_Button_SetCaption(gui_p->game_options_2.but_down, GameOptions2_GetKeyName(new_key) );
		Keyboard_SetKey(WIIGC1_BUTTON_DOWN, new_key);
	}
}

static void GameOptions2_Callback_MapStickClick(EG_Widget*, void* user_p)
{
	BeebEm_Pages *gui_p = (BeebEm_Pages*) user_p;
	int new_key;

	new_key = BBCKeyboard_Show( EG_Window_GetSurface(gui_p->game_options_2.win) , Keyboard_GetButtonMapping(WIIGC1_BUTTON_TRIGGER_Z) );

	if (new_key>=0) {
		EG_Button_SetCaption(gui_p->game_options_2.but_z, GameOptions2_GetKeyName(new_key) );
		Keyboard_SetKey(WIIGC1_BUTTON_TRIGGER_Z, new_key);
	}
}

static void GameOptions2_Callback_MapA(EG_Widget*, void* user_p)
{
	BeebEm_Pages *gui_p = (BeebEm_Pages*) user_p;
	int new_key;

	new_key = BBCKeyboard_Show( EG_Window_GetSurface(gui_p->game_options_2.win) , Keyboard_GetButtonMapping(WIIGC1_BUTTON_A) );

	if (new_key>=0) {
		EG_Button_SetCaption(gui_p->game_options_2.but_a, GameOptions2_GetKeyName(new_key) );
		Keyboard_SetKey(WIIGC1_BUTTON_A, new_key);
	}
}

static void GameOptions2_Callback_MapB(EG_Widget*, void* user_p)
{
	BeebEm_Pages *gui_p = (BeebEm_Pages*) user_p;
	int new_key;

	new_key = BBCKeyboard_Show( EG_Window_GetSurface(gui_p->game_options_2.win) , Keyboard_GetButtonMapping(WIIGC1_BUTTON_B) );

	if (new_key>=0) {
		EG_Button_SetCaption(gui_p->game_options_2.but_b, GameOptions2_GetKeyName(new_key) );
		Keyboard_SetKey(WIIGC1_BUTTON_B, new_key);
	}
}

static void GameOptions2_Callback_MapX(EG_Widget*, void* user_p)
{
	BeebEm_Pages *gui_p = (BeebEm_Pages*) user_p;
	int new_key;

	new_key = BBCKeyboard_Show( EG_Window_GetSurface(gui_p->game_options_2.win) , Keyboard_GetButtonMapping(WIIGC1_BUTTON_X) );

	if (new_key>=0) {
		EG_Button_SetCaption(gui_p->game_options_2.but_x, GameOptions2_GetKeyName(new_key) );
		Keyboard_SetKey(WIIGC1_BUTTON_X, new_key);
	}
}

static void GameOptions2_Callback_MapY(EG_Widget*, void* user_p)
{
	BeebEm_Pages *gui_p = (BeebEm_Pages*) user_p;
	int new_key;

	new_key = BBCKeyboard_Show( EG_Window_GetSurface(gui_p->game_options_2.win) , Keyboard_GetButtonMapping(WIIGC1_BUTTON_Y) );

	if (new_key>=0) {
		EG_Button_SetCaption(gui_p->game_options_2.but_y, GameOptions2_GetKeyName(new_key) );
		Keyboard_SetKey(WIIGC1_BUTTON_Y, new_key);
	}
}

static void GameOptions2_Callback_MapMinusZ(EG_Widget*, void* user_p)
{
	BeebEm_Pages *gui_p = (BeebEm_Pages*) user_p;
	int new_key;

	new_key = BBCKeyboard_Show( EG_Window_GetSurface(gui_p->game_options_2.win) , Keyboard_GetButtonMapping(WIIGC1_BUTTON_MINUS) );

	if (new_key>=0) {
		EG_Button_SetCaption(gui_p->game_options_2.but_minus, GameOptions2_GetKeyName(new_key) );
		Keyboard_SetKey(WIIGC1_BUTTON_MINUS, new_key);
	}
}


static void GameOptions2_Callback_MapClassicA(EG_Widget*, void* user_p)
{
	BeebEm_Pages *gui_p = (BeebEm_Pages*) user_p;
	int new_key;

	new_key = BBCKeyboard_Show( EG_Window_GetSurface(gui_p->game_options_2.win) , Keyboard_GetButtonMapping(CLASSIC1_BUTTON_A) );

	if (new_key>=0) {
		EG_Button_SetCaption(gui_p->game_options_2.but_classic_a, GameOptions2_GetKeyName(new_key) );
		Keyboard_SetKey(CLASSIC1_BUTTON_A, new_key);
	}
}
static void GameOptions2_Callback_MapClassicB(EG_Widget*, void* user_p)
{
	BeebEm_Pages *gui_p = (BeebEm_Pages*) user_p;
	int new_key;

	new_key = BBCKeyboard_Show( EG_Window_GetSurface(gui_p->game_options_2.win) , Keyboard_GetButtonMapping(CLASSIC1_BUTTON_B) );

	if (new_key>=0) {
		EG_Button_SetCaption(gui_p->game_options_2.but_classic_b, GameOptions2_GetKeyName(new_key) );
		Keyboard_SetKey(CLASSIC1_BUTTON_B, new_key);
	}
}


static void GameOptions2_Callback_MapClassicX(EG_Widget*, void* user_p)
{
	BeebEm_Pages *gui_p = (BeebEm_Pages*) user_p;
	int new_key;

	new_key = BBCKeyboard_Show( EG_Window_GetSurface(gui_p->game_options_2.win) , Keyboard_GetButtonMapping(CLASSIC1_BUTTON_X) );

	if (new_key>=0) {
		EG_Button_SetCaption(gui_p->game_options_2.but_classic_x, GameOptions2_GetKeyName(new_key) );
		Keyboard_SetKey(CLASSIC1_BUTTON_X, new_key);
	}
}

static void GameOptions2_Callback_MapClassicY(EG_Widget*, void* user_p)
{
	BeebEm_Pages *gui_p = (BeebEm_Pages*) user_p;
	int new_key;

	new_key = BBCKeyboard_Show( EG_Window_GetSurface(gui_p->game_options_2.win) , Keyboard_GetButtonMapping(CLASSIC1_BUTTON_Y) );

	if (new_key>=0) {
		EG_Button_SetCaption(gui_p->game_options_2.but_classic_y, GameOptions2_GetKeyName(new_key) );
		Keyboard_SetKey(CLASSIC1_BUTTON_Y, new_key);
	}
}

static void GameOptions2_Callback_MapClassicL(EG_Widget*, void* user_p)
{
	BeebEm_Pages *gui_p = (BeebEm_Pages*) user_p;
	int new_key;

	new_key = BBCKeyboard_Show( EG_Window_GetSurface(gui_p->game_options_2.win) , Keyboard_GetButtonMapping(CLASSIC1_BUTTON_L) );

	if (new_key>=0) {
		EG_Button_SetCaption(gui_p->game_options_2.but_classic_l, GameOptions2_GetKeyName(new_key) );
		Keyboard_SetKey(CLASSIC1_BUTTON_L, new_key);
	}
}

static void GameOptions2_Callback_MapClassicR(EG_Widget*, void* user_p)
{
	BeebEm_Pages *gui_p = (BeebEm_Pages*) user_p;
	int new_key;

	new_key = BBCKeyboard_Show( EG_Window_GetSurface(gui_p->game_options_2.win) , Keyboard_GetButtonMapping(CLASSIC1_BUTTON_R) );

	if (new_key>=0) {
		EG_Button_SetCaption(gui_p->game_options_2.but_classic_r, GameOptions2_GetKeyName(new_key) );
		Keyboard_SetKey(CLASSIC1_BUTTON_R, new_key);
	}
}





static void GameOptions2_Callback_Map_F_StickLeft(EG_Widget*, void* user_p)
{
	BeebEm_Pages *gui_p = (BeebEm_Pages*) user_p;
	int new_key;

	new_key = BBCKeyboard_Show( EG_Window_GetSurface(gui_p->game_options_2.win) , Keyboard_GetButtonMapping(WIIGC2_BUTTON_LEFT) );

	if (new_key>=0) {
		EG_Button_SetCaption(gui_p->game_options_2.but_f_left, GameOptions2_GetKeyName(new_key) );
		Keyboard_SetKey(WIIGC2_BUTTON_LEFT, new_key);
	}
}

static void GameOptions2_Callback_Map_F_StickRight(EG_Widget*, void* user_p)
{
	BeebEm_Pages *gui_p = (BeebEm_Pages*) user_p;
	int new_key;

	new_key = BBCKeyboard_Show( EG_Window_GetSurface(gui_p->game_options_2.win) , Keyboard_GetButtonMapping(WIIGC2_BUTTON_RIGHT) );

	if (new_key>=0) {
		EG_Button_SetCaption(gui_p->game_options_2.but_f_right, GameOptions2_GetKeyName(new_key) );
		Keyboard_SetKey(WIIGC2_BUTTON_RIGHT, new_key);
	}
}


static void GameOptions2_Callback_Map_F_StickUp(EG_Widget*, void* user_p)
{
	BeebEm_Pages *gui_p = (BeebEm_Pages*) user_p;
	int new_key;

	new_key = BBCKeyboard_Show( EG_Window_GetSurface(gui_p->game_options_2.win) , Keyboard_GetButtonMapping(WIIGC2_BUTTON_UP) );

	if (new_key>=0) {
		EG_Button_SetCaption(gui_p->game_options_2.but_f_up, GameOptions2_GetKeyName(new_key) );
		Keyboard_SetKey(WIIGC2_BUTTON_UP, new_key);
	}
}


static void GameOptions2_Callback_Map_F_StickDown(EG_Widget*, void* user_p)
{
	BeebEm_Pages *gui_p = (BeebEm_Pages*) user_p;
	int new_key;

	new_key = BBCKeyboard_Show( EG_Window_GetSurface(gui_p->game_options_2.win) , Keyboard_GetButtonMapping(WIIGC2_BUTTON_DOWN) );

	if (new_key>=0) {
		EG_Button_SetCaption(gui_p->game_options_2.but_f_down, GameOptions2_GetKeyName(new_key) );
		Keyboard_SetKey(WIIGC2_BUTTON_DOWN, new_key);
	}
}

static void GameOptions2_Callback_Map_F_StickClick(EG_Widget*, void* user_p)
{
	/*BeebEm_Pages *gui_p = (BeebEm_Pages*) user_p;
	int new_key;

	new_key = BBCKeyboard_Show( EG_Window_GetSurface(gui_p->game_options_2.win) , Keyboard_GetButtonMapping(WIIGC2_BUTTON_TRIGGER_Z) );

	if (new_key>=0) {
		EG_Button_SetCaption(gui_p->game_options_2.but_f_z, GameOptions2_GetKeyName(new_key) );
		Keyboard_SetKey(WIIGC2_BUTTON_TRIGGER_Z, new_key);
	}*/
}

static void GameOptions2_Callback_Map_F_A(EG_Widget*, void* user_p)
{
	BeebEm_Pages *gui_p = (BeebEm_Pages*) user_p;
	int new_key;

	new_key = BBCKeyboard_Show( EG_Window_GetSurface(gui_p->game_options_2.win) , Keyboard_GetButtonMapping(WIIGC2_BUTTON_A) );

	if (new_key>=0) {
		EG_Button_SetCaption(gui_p->game_options_2.but_f_a, GameOptions2_GetKeyName(new_key) );
		Keyboard_SetKey(WIIGC2_BUTTON_A, new_key);
	}
}

static void GameOptions2_Callback_Map_F_B(EG_Widget*, void* user_p)
{
	BeebEm_Pages *gui_p = (BeebEm_Pages*) user_p;
	int new_key;

	new_key = BBCKeyboard_Show( EG_Window_GetSurface(gui_p->game_options_2.win) , Keyboard_GetButtonMapping(WIIGC2_BUTTON_B) );

	if (new_key>=0) {
		EG_Button_SetCaption(gui_p->game_options_2.but_f_b, GameOptions2_GetKeyName(new_key) );
		Keyboard_SetKey(WIIGC2_BUTTON_B, new_key);
	}
}

static void GameOptions2_Callback_Map_F_X(EG_Widget*, void* user_p)
{
	BeebEm_Pages *gui_p = (BeebEm_Pages*) user_p;
	int new_key;

	new_key = BBCKeyboard_Show( EG_Window_GetSurface(gui_p->game_options_2.win) , Keyboard_GetButtonMapping(WIIGC2_BUTTON_X) );

	if (new_key>=0) {
		EG_Button_SetCaption(gui_p->game_options_2.but_f_x, GameOptions2_GetKeyName(new_key) );
		Keyboard_SetKey(WIIGC2_BUTTON_X, new_key);
	}
}

static void GameOptions2_Callback_Map_F_Y(EG_Widget*, void* user_p)
{
	BeebEm_Pages *gui_p = (BeebEm_Pages*) user_p;
	int new_key;

	new_key = BBCKeyboard_Show( EG_Window_GetSurface(gui_p->game_options_2.win) , Keyboard_GetButtonMapping(WIIGC2_BUTTON_Y) );

	if (new_key>=0) {
		EG_Button_SetCaption(gui_p->game_options_2.but_f_y, GameOptions2_GetKeyName(new_key) );
		Keyboard_SetKey(WIIGC2_BUTTON_Y, new_key);
	}
}

static void GameOptions2_Callback_Map_F_MapMinusZ(EG_Widget*, void* user_p)
{
	/*BeebEm_Pages *gui_p = (BeebEm_Pages*) user_p;
	int new_key;

	new_key = BBCKeyboard_Show( EG_Window_GetSurface(gui_p->game_options_2.win) , Keyboard_GetButtonMapping(WIIGC2_BUTTON_MINUS) );

	if (new_key>=0) {
		EG_Button_SetCaption(gui_p->game_options_2.but_f_minus, GameOptions2_GetKeyName(new_key) );
		Keyboard_SetKey(WIIGC2_BUTTON_MINUS, new_key);
	}*/
}


static void GameOptions2_Callback_2_MapClassicA(EG_Widget*, void* user_p)
{
	BeebEm_Pages *gui_p = (BeebEm_Pages*) user_p;
	int new_key;

	new_key = BBCKeyboard_Show( EG_Window_GetSurface(gui_p->game_options_2.win) , Keyboard_GetButtonMapping(CLASSIC2_BUTTON_A) );

	if (new_key>=0) {
		EG_Button_SetCaption(gui_p->game_options_2.but_f_classic_a, GameOptions2_GetKeyName(new_key) );
		Keyboard_SetKey(CLASSIC2_BUTTON_A, new_key);
	}
}
static void GameOptions2_Callback_2_MapClassicB(EG_Widget*, void* user_p)
{
	BeebEm_Pages *gui_p = (BeebEm_Pages*) user_p;
	int new_key;

	new_key = BBCKeyboard_Show( EG_Window_GetSurface(gui_p->game_options_2.win) , Keyboard_GetButtonMapping(CLASSIC2_BUTTON_B) );

	if (new_key>=0) {
		EG_Button_SetCaption(gui_p->game_options_2.but_f_classic_b, GameOptions2_GetKeyName(new_key) );
		Keyboard_SetKey(CLASSIC2_BUTTON_B, new_key);
	}
}


static void GameOptions2_Callback_2_MapClassicX(EG_Widget*, void* user_p)
{
	BeebEm_Pages *gui_p = (BeebEm_Pages*) user_p;
	int new_key;

	new_key = BBCKeyboard_Show( EG_Window_GetSurface(gui_p->game_options_2.win) , Keyboard_GetButtonMapping(CLASSIC2_BUTTON_X) );

	if (new_key>=0) {
		EG_Button_SetCaption(gui_p->game_options_2.but_f_classic_x, GameOptions2_GetKeyName(new_key) );
		Keyboard_SetKey(CLASSIC2_BUTTON_X, new_key);
	}
}

static void GameOptions2_Callback_2_MapClassicY(EG_Widget*, void* user_p)
{
	BeebEm_Pages *gui_p = (BeebEm_Pages*) user_p;
	int new_key;

	new_key = BBCKeyboard_Show( EG_Window_GetSurface(gui_p->game_options_2.win) , Keyboard_GetButtonMapping(CLASSIC2_BUTTON_Y) );

	if (new_key>=0) {
		EG_Button_SetCaption(gui_p->game_options_2.but_f_classic_y, GameOptions2_GetKeyName(new_key) );
		Keyboard_SetKey(CLASSIC2_BUTTON_Y, new_key);
	}
}

static void GameOptions2_Callback_2_MapClassicL(EG_Widget*, void* user_p)
{
	BeebEm_Pages *gui_p = (BeebEm_Pages*) user_p;
	int new_key;

	new_key = BBCKeyboard_Show( EG_Window_GetSurface(gui_p->game_options_2.win) , Keyboard_GetButtonMapping(CLASSIC2_BUTTON_L) );

	if (new_key>=0) {
		EG_Button_SetCaption(gui_p->game_options_2.but_f_classic_l, GameOptions2_GetKeyName(new_key) );
		Keyboard_SetKey(CLASSIC2_BUTTON_L, new_key);
	}
}

static void GameOptions2_Callback_2_MapClassicR(EG_Widget*, void* user_p)
{
	BeebEm_Pages *gui_p = (BeebEm_Pages*) user_p;
	int new_key;

	new_key = BBCKeyboard_Show( EG_Window_GetSurface(gui_p->game_options_2.win) , Keyboard_GetButtonMapping(CLASSIC2_BUTTON_R) );

	if (new_key>=0) {
		EG_Button_SetCaption(gui_p->game_options_2.but_f_classic_r, GameOptions2_GetKeyName(new_key) );
		Keyboard_SetKey(CLASSIC2_BUTTON_R, new_key);
	}
}





static EG_BOOL GameOptions2_Make(SDL_Surface *frame_buffer_p, BeebEm_Pages *gui_p)
{

	SDL_Color c;
	SDL_Rect r, r2, r3;

//	c = (SDL_Color) {255-64,0, 255-64, 0};
	c = MENU_COLORS;

//	r = (SDL_Rect) {8, 4, GUI_WIDTH-16, GUI_HEIGHT-8};
	 r = (SDL_Rect) {0,0, GUI_WIDTH, GUI_HEIGHT};
	gui_p->game_options_2.win = EG_Window_Create("game_options_2_win", frame_buffer_p, c, r);
	r2 = EG_Window_GetDimension(gui_p->game_options_2.win); r2.x+=2; r2.w -=4;

	r2.x+=3; r2.w-=8; r2.y+=4; r2.h-=8;


	/* Player 1 buttons */
	gui_p->game_options_2.lbl = EG_Label_Create("game_options_2_lbl", EG_COLOR_ALT
	 , EG_LABEL_ALIGN_CENTER, "Player 1 buttons:", C_1_1(r2, 22, 2));
	(void) EG_Window_AddWidget(gui_p->game_options_2.win, gui_p->game_options_2.lbl);

	r3 = C_1_1(r2, 24, 18);

	gui_p->game_options_2.box = EG_Box_Create("game_options_2_box", EG_BOX_BORDER_SUNK, c, r3);
	(void) EG_Window_AddWidget(gui_p->game_options_2.win, gui_p->game_options_2.box);
	r3.x+=4; r3.w-=8;

	SDL_Rect nr;

	/* Left		Right		Up		Down
	 * <Key name>	<Key name>	<Key name>	<Key name>
	 */
	gui_p->game_options_2.lbl_left = EG_Label_Create("game_options_2_left_lbl", c
	 , EG_LABEL_ALIGN_CENTER, "LEFT", C_1_4(r3, 1, 2));
	nr=C_1_4(r3, 3, 2); nr.y-=2; nr.h+=3; (void) EG_Window_AddWidget(gui_p->game_options_2.win, gui_p->game_options_2.lbl_left);
	gui_p->game_options_2.but_left = EG_Button_Create("game_options_2_left_but", c
	 , EG_BUTTON_ALIGN_CENTER, "NOTHING", nr );
	(void) EG_Button_SetMyCallback_OnClick(gui_p->game_options_2.but_left, GameOptions2_Callback_MapStickLeft, gui_p);
	(void) EG_Window_AddWidget(gui_p->game_options_2.win, gui_p->game_options_2.but_left);

	gui_p->game_options_2.lbl_right = EG_Label_Create("game_options_2_right_lbl", c
	 , EG_LABEL_ALIGN_CENTER, "RIGHT", C_2_4(r3, 1, 2));
	(void) EG_Window_AddWidget(gui_p->game_options_2.win, gui_p->game_options_2.lbl_right);
	nr=C_2_4(r3, 3, 2); nr.y-=2; nr.h+=3; gui_p->game_options_2.but_right = EG_Button_Create("game_options_2_right_but", c
	 , EG_BUTTON_ALIGN_CENTER, "NOTHING", nr );
	(void) EG_Button_SetMyCallback_OnClick(gui_p->game_options_2.but_right, GameOptions2_Callback_MapStickRight, gui_p);
	(void) EG_Window_AddWidget(gui_p->game_options_2.win, gui_p->game_options_2.but_right);

	gui_p->game_options_2.lbl_up = EG_Label_Create("game_options_2_up_lbl", c
	 , EG_LABEL_ALIGN_CENTER, "UP", C_3_4(r3, 1, 2));
	(void) EG_Window_AddWidget(gui_p->game_options_2.win, gui_p->game_options_2.lbl_up);
	nr=C_3_4(r3, 3, 2); nr.y-=2; nr.h+=3; gui_p->game_options_2.but_up = EG_Button_Create("game_options_2_up_but", c
	 , EG_BUTTON_ALIGN_CENTER, "NOTHING", nr );
	(void) EG_Button_SetMyCallback_OnClick(gui_p->game_options_2.but_up, GameOptions2_Callback_MapStickUp, gui_p);
	(void) EG_Window_AddWidget(gui_p->game_options_2.win, gui_p->game_options_2.but_up);

	gui_p->game_options_2.lbl_down = EG_Label_Create("game_options_2_down_lbl", c
	 , EG_LABEL_ALIGN_CENTER, "DOWN", C_4_4(r3, 1, 2));
	(void) EG_Window_AddWidget(gui_p->game_options_2.win, gui_p->game_options_2.lbl_down);
	nr=C_4_4(r3, 3, 2); nr.y-=2; nr.h+=3; gui_p->game_options_2.but_down = EG_Button_Create("game_options_2_down_but", c
	 , EG_BUTTON_ALIGN_CENTER, "NOTHING", nr );
	(void) EG_Button_SetMyCallback_OnClick(gui_p->game_options_2.but_down, GameOptions2_Callback_MapStickDown, gui_p);
	(void) EG_Window_AddWidget(gui_p->game_options_2.win, gui_p->game_options_2.but_down);

	/* A            B               X               Y
	 * <Key name>   <Key name>      <Key name>      <Key name>
 	 */
	gui_p->game_options_2.lbl_a = EG_Label_Create("game_options_2_a_lbl", c
	 , EG_LABEL_ALIGN_CENTER, "Wii/GC A", C_1_4(r3, 5, 2));
	(void) EG_Window_AddWidget(gui_p->game_options_2.win, gui_p->game_options_2.lbl_a);
	nr=C_1_4(r3, 7, 2); nr.y-=2; nr.h+=3; gui_p->game_options_2.but_a = EG_Button_Create("game_options_2_a_but", c
	 , EG_BUTTON_ALIGN_CENTER, "NOTHING", nr );
	(void) EG_Button_SetMyCallback_OnClick(gui_p->game_options_2.but_a, GameOptions2_Callback_MapA, gui_p);
	(void) EG_Window_AddWidget(gui_p->game_options_2.win, gui_p->game_options_2.but_a);

	gui_p->game_options_2.lbl_b = EG_Label_Create("game_options_2_b_lbl", c
	 , EG_LABEL_ALIGN_CENTER, "Wii/GC B", C_2_4(r3, 5, 2));
	(void) EG_Window_AddWidget(gui_p->game_options_2.win, gui_p->game_options_2.lbl_b);
	nr=C_2_4(r3, 7, 2); nr.y-=2; nr.h+=3; gui_p->game_options_2.but_b = EG_Button_Create("game_options_2_b_but", c
	 , EG_BUTTON_ALIGN_CENTER, "NOTHING", nr );
	(void) EG_Button_SetMyCallback_OnClick(gui_p->game_options_2.but_b, GameOptions2_Callback_MapB, gui_p);
	(void) EG_Window_AddWidget(gui_p->game_options_2.win, gui_p->game_options_2.but_b);

	gui_p->game_options_2.lbl_x = EG_Label_Create("game_options_2_x_lbl", c
	 , EG_LABEL_ALIGN_CENTER, "Wii/GC 1/X", C_3_4(r3, 5, 2));
	(void) EG_Window_AddWidget(gui_p->game_options_2.win, gui_p->game_options_2.lbl_x);
	nr=C_3_4(r3, 7, 2); nr.y-=2; nr.h+=3; gui_p->game_options_2.but_x = EG_Button_Create("game_options_2_x_but", c
	 , EG_BUTTON_ALIGN_CENTER, "NOTHING", nr );
	(void) EG_Button_SetMyCallback_OnClick(gui_p->game_options_2.but_x, GameOptions2_Callback_MapX, gui_p);
	(void) EG_Window_AddWidget(gui_p->game_options_2.win, gui_p->game_options_2.but_x);

	gui_p->game_options_2.lbl_y = EG_Label_Create("game_options_2_y_lbl", c
	 , EG_LABEL_ALIGN_CENTER, "Wii/GC 2/Y", C_4_4(r3, 5, 2));
	(void) EG_Window_AddWidget(gui_p->game_options_2.win, gui_p->game_options_2.lbl_y);
	nr=C_4_4(r3, 7, 2); nr.y-=2; nr.h+=3; gui_p->game_options_2.but_y = EG_Button_Create("game_options_2_down_but", c
	 , EG_BUTTON_ALIGN_CENTER, "NOTHING", nr );
	(void) EG_Button_SetMyCallback_OnClick(gui_p->game_options_2.but_y, GameOptions2_Callback_MapY, gui_p);
	(void) EG_Window_AddWidget(gui_p->game_options_2.win, gui_p->game_options_2.but_y);

	/* L            R               Select          Click
	 * <Key name>   <Key name>      <Key name>      <Key name>
	 */


	gui_p->game_options_2.lbl_classic_a = EG_Label_Create("game_options_2_classic_a_lbl", c
	 , EG_LABEL_ALIGN_CENTER, "Classic A", C_1_4(r3, 9, 2));
	(void) EG_Window_AddWidget(gui_p->game_options_2.win, gui_p->game_options_2.lbl_classic_a);
	nr=C_1_4(r3, 11, 2); nr.y-=2; nr.h+=3; gui_p->game_options_2.but_classic_a = EG_Button_Create("game_options_2_classic_a_but", c
	 , EG_BUTTON_ALIGN_CENTER, "NOTHING", nr );
	(void) EG_Button_SetMyCallback_OnClick(gui_p->game_options_2.but_classic_a, GameOptions2_Callback_MapClassicA, gui_p);
	(void) EG_Window_AddWidget(gui_p->game_options_2.win, gui_p->game_options_2.but_classic_a);

	gui_p->game_options_2.lbl_classic_b = EG_Label_Create("game_options_2_classic_b_lbl", c
	 , EG_LABEL_ALIGN_CENTER, "Classic B", C_2_4(r3, 9, 2));
	(void) EG_Window_AddWidget(gui_p->game_options_2.win, gui_p->game_options_2.lbl_classic_b);
	nr=C_2_4(r3, 11, 2); nr.y-=2; nr.h+=3; gui_p->game_options_2.but_classic_b = EG_Button_Create("game_options_2_classic_b_but", c
	 , EG_BUTTON_ALIGN_CENTER, "NOTHING", nr );
	(void) EG_Button_SetMyCallback_OnClick(gui_p->game_options_2.but_classic_b, GameOptions2_Callback_MapClassicB, gui_p);
	(void) EG_Window_AddWidget(gui_p->game_options_2.win, gui_p->game_options_2.but_classic_b);

	gui_p->game_options_2.lbl_classic_x = EG_Label_Create("game_options_2_classic_x_lbl", c
	 , EG_LABEL_ALIGN_CENTER, "Classic X", C_3_4(r3, 9, 2));
	(void) EG_Window_AddWidget(gui_p->game_options_2.win, gui_p->game_options_2.lbl_classic_x);
	nr=C_3_4(r3, 11, 2); nr.y-=2; nr.h+=3; gui_p->game_options_2.but_classic_x = EG_Button_Create("game_options_2_classic_x_but", c
	 , EG_BUTTON_ALIGN_CENTER, "NOTHING", nr );
	(void) EG_Button_SetMyCallback_OnClick(gui_p->game_options_2.but_classic_x, GameOptions2_Callback_MapClassicX, gui_p);
	(void) EG_Window_AddWidget(gui_p->game_options_2.win, gui_p->game_options_2.but_classic_x);

	gui_p->game_options_2.lbl_classic_y = EG_Label_Create("game_options_2_classic_y_lbl", c
	 , EG_LABEL_ALIGN_CENTER, "Classic Y", C_4_4(r3, 9, 2));
	(void) EG_Window_AddWidget(gui_p->game_options_2.win, gui_p->game_options_2.lbl_classic_y);
	nr=C_4_4(r3, 11, 2); nr.y-=2; nr.h+=3; gui_p->game_options_2.but_classic_y = EG_Button_Create("game_options_2_classic_y_but", c
	 , EG_BUTTON_ALIGN_CENTER, "NOTHING", nr );
	(void) EG_Button_SetMyCallback_OnClick(gui_p->game_options_2.but_classic_y, GameOptions2_Callback_MapClassicY, gui_p);
	(void) EG_Window_AddWidget(gui_p->game_options_2.win, gui_p->game_options_2.but_classic_y);

	/* Volume Up	Volume Down
	 * <Key name>	<Key name>
 	 */
	gui_p->game_options_2.lbl_classic_l = EG_Label_Create("game_options_2_classic_l_lbl", c
	 , EG_LABEL_ALIGN_CENTER, "Classic L", C_1_4(r3, 13, 2));
	(void) EG_Window_AddWidget(gui_p->game_options_2.win, gui_p->game_options_2.lbl_classic_l);
	nr=C_2_4(r3, 15, 2); nr.y-=2; nr.h+=3; gui_p->game_options_2.but_classic_l = EG_Button_Create("game_options_2_classic_l_but", c
	 , EG_BUTTON_ALIGN_CENTER, "NOTHING", nr );
	(void) EG_Button_SetMyCallback_OnClick(gui_p->game_options_2.but_classic_l, GameOptions2_Callback_MapClassicL, gui_p);
	(void) EG_Window_AddWidget(gui_p->game_options_2.win, gui_p->game_options_2.but_classic_l);

	gui_p->game_options_2.lbl_classic_r = EG_Label_Create("game_options_2_classic_r_lbl", c
	 , EG_LABEL_ALIGN_CENTER, "Classic R", C_2_4(r3, 13, 2));
	(void) EG_Window_AddWidget(gui_p->game_options_2.win, gui_p->game_options_2.lbl_classic_r);
	nr=C_1_4(r3, 15, 2); nr.y-=2; nr.h+=3; gui_p->game_options_2.but_classic_r = EG_Button_Create("game_options_2_classic_r_but", c
	 , EG_BUTTON_ALIGN_CENTER, "NOTHING", nr );
	(void) EG_Button_SetMyCallback_OnClick(gui_p->game_options_2.but_classic_r, GameOptions2_Callback_MapClassicR, gui_p);
	(void) EG_Window_AddWidget(gui_p->game_options_2.win, gui_p->game_options_2.but_classic_r);


	/* Player 2 buttons */
	gui_p->game_options_2.lbl_f = EG_Label_Create("game_options_2_lbl_f", EG_COLOR_ALT
	 , EG_LABEL_ALIGN_CENTER, "Player 2 buttons:", C_1_1(r2, 1, 2));
	(void) EG_Window_AddWidget(gui_p->game_options_2.win, gui_p->game_options_2.lbl_f);

	r3 = C_1_1(r2, 3, 18);
	gui_p->game_options_2.box_f = EG_Box_Create("game_options_2_box_f", EG_BOX_BORDER_SUNK, c, r3);
	(void) EG_Window_AddWidget(gui_p->game_options_2.win, gui_p->game_options_2.box_f);
	r3.x+=4; r3.w-=8;


	// Left		Right		Up		Down
	// <Key name>	<Key name>	<Key name>	<Key name>
	//
	gui_p->game_options_2.lbl_f_left = EG_Label_Create("game_options_2_f_left_lbl", c
	 , EG_LABEL_ALIGN_CENTER, "LEFT", C_1_4(r3, 1, 2));
	(void) EG_Window_AddWidget(gui_p->game_options_2.win, gui_p->game_options_2.lbl_f_left);
	nr=C_1_4(r3, 3, 2); nr.y-=2; nr.h+=3; gui_p->game_options_2.but_f_left = EG_Button_Create("game_options_2_f_left_but", c
	 , EG_BUTTON_ALIGN_CENTER, "NOTHING", nr );
	(void) EG_Button_SetMyCallback_OnClick(gui_p->game_options_2.but_f_left, GameOptions2_Callback_Map_F_StickLeft, gui_p);
	(void) EG_Window_AddWidget(gui_p->game_options_2.win, gui_p->game_options_2.but_f_left);

	gui_p->game_options_2.lbl_f_right = EG_Label_Create("game_options_2_f_right_lbl", c
	 , EG_LABEL_ALIGN_CENTER, "RIGHT", C_2_4(r3, 1, 2));
	(void) EG_Window_AddWidget(gui_p->game_options_2.win, gui_p->game_options_2.lbl_f_right);
	nr=C_2_4(r3, 3, 2); nr.y-=2; nr.h+=3; gui_p->game_options_2.but_f_right = EG_Button_Create("game_options_2_f_right_but", c
	 , EG_BUTTON_ALIGN_CENTER, "NOTHING", nr );
	(void) EG_Button_SetMyCallback_OnClick(gui_p->game_options_2.but_f_right, GameOptions2_Callback_Map_F_StickRight, gui_p);
	(void) EG_Window_AddWidget(gui_p->game_options_2.win, gui_p->game_options_2.but_f_right);

	gui_p->game_options_2.lbl_f_up = EG_Label_Create("game_options_2_f_up_lbl", c
	 , EG_LABEL_ALIGN_CENTER, "UP", C_3_4(r3, 1, 2));
	(void) EG_Window_AddWidget(gui_p->game_options_2.win, gui_p->game_options_2.lbl_f_up);
	nr=C_3_4(r3, 3, 2); nr.y-=2; nr.h+=3; gui_p->game_options_2.but_f_up = EG_Button_Create("game_options_2_f_up_but", c
	 , EG_BUTTON_ALIGN_CENTER, "NOTHING", nr );
	(void) EG_Button_SetMyCallback_OnClick(gui_p->game_options_2.but_f_up, GameOptions2_Callback_Map_F_StickUp, gui_p);
	(void) EG_Window_AddWidget(gui_p->game_options_2.win, gui_p->game_options_2.but_f_up);

	gui_p->game_options_2.lbl_f_down = EG_Label_Create("game_options_2_f_down_lbl", c
	 , EG_LABEL_ALIGN_CENTER, "DOWN", C_4_4(r3, 1, 2));
	(void) EG_Window_AddWidget(gui_p->game_options_2.win, gui_p->game_options_2.lbl_f_down);
	nr=C_4_4(r3, 3, 2); nr.y-=2; nr.h+=3; gui_p->game_options_2.but_f_down = EG_Button_Create("game_options_2_f_down_but", c
	 , EG_BUTTON_ALIGN_CENTER, "NOTHING", nr );
	(void) EG_Button_SetMyCallback_OnClick(gui_p->game_options_2.but_f_down, GameOptions2_Callback_Map_F_StickDown, gui_p);
	(void) EG_Window_AddWidget(gui_p->game_options_2.win, gui_p->game_options_2.but_f_down);


	// A            B               X               Y
	// <Key name>   <Key name>      <Key name>      <Key name>
 	//
	gui_p->game_options_2.lbl_f_a = EG_Label_Create("game_options_2_f_a_lbl", c
	 , EG_LABEL_ALIGN_CENTER, "Wii/GC A", C_1_4(r3, 5, 2));
	(void) EG_Window_AddWidget(gui_p->game_options_2.win, gui_p->game_options_2.lbl_f_a);
	nr=C_1_4(r3, 7, 2); nr.y-=2; nr.h+=3; gui_p->game_options_2.but_f_a = EG_Button_Create("game_options_2_f_a_but", c
	 , EG_BUTTON_ALIGN_CENTER, "NOTHING", nr );
	(void) EG_Button_SetMyCallback_OnClick(gui_p->game_options_2.but_f_a, GameOptions2_Callback_Map_F_A, gui_p);
	(void) EG_Window_AddWidget(gui_p->game_options_2.win, gui_p->game_options_2.but_f_a);

	gui_p->game_options_2.lbl_f_b = EG_Label_Create("game_options_2_f_b_lbl", c
	 , EG_LABEL_ALIGN_CENTER, "Wii/GC B", C_2_4(r3, 5, 2));
	(void) EG_Window_AddWidget(gui_p->game_options_2.win, gui_p->game_options_2.lbl_f_b);
	nr=C_2_4(r3, 7, 2); nr.y-=2; nr.h+=3; gui_p->game_options_2.but_f_b = EG_Button_Create("game_options_2_f_b_but", c
	 , EG_BUTTON_ALIGN_CENTER, "NOTHING", nr );
	(void) EG_Button_SetMyCallback_OnClick(gui_p->game_options_2.but_f_b, GameOptions2_Callback_Map_F_B, gui_p);
	(void) EG_Window_AddWidget(gui_p->game_options_2.win, gui_p->game_options_2.but_f_b);

	gui_p->game_options_2.lbl_f_x = EG_Label_Create("game_options_2_f_x_lbl", c
	 , EG_LABEL_ALIGN_CENTER, "Wii/GC 1/X", C_3_4(r3, 5, 2));
	(void) EG_Window_AddWidget(gui_p->game_options_2.win, gui_p->game_options_2.lbl_f_x);
	nr=C_3_4(r3, 7, 2); nr.y-=2; nr.h+=3; gui_p->game_options_2.but_f_x = EG_Button_Create("game_options_2_f_x_but", c
	 , EG_BUTTON_ALIGN_CENTER, "NOTHING", nr );
	(void) EG_Button_SetMyCallback_OnClick(gui_p->game_options_2.but_f_x, GameOptions2_Callback_Map_F_X, gui_p);
	(void) EG_Window_AddWidget(gui_p->game_options_2.win, gui_p->game_options_2.but_f_x);

	gui_p->game_options_2.lbl_f_y = EG_Label_Create("game_options_2_f_y_lbl", c
	 , EG_LABEL_ALIGN_CENTER, "Wii/GC 2/Y", C_4_4(r3, 5, 2));
	(void) EG_Window_AddWidget(gui_p->game_options_2.win, gui_p->game_options_2.lbl_f_y);
	nr=C_4_4(r3, 7, 2); nr.y-=2; nr.h+=3; gui_p->game_options_2.but_f_y = EG_Button_Create("game_options_2_f_y_but", c
	 , EG_BUTTON_ALIGN_CENTER, "NOTHING", nr );
	(void) EG_Button_SetMyCallback_OnClick(gui_p->game_options_2.but_f_y, GameOptions2_Callback_Map_F_Y, gui_p);
	(void) EG_Window_AddWidget(gui_p->game_options_2.win, gui_p->game_options_2.but_f_y);


	// L            R               Select          Click
	// <Key name>   <Key name>      <Key name>      <Key name>
	//
	gui_p->game_options_2.lbl_f_classic_a = EG_Label_Create("game_options_2_f_classic_a_lbl", c
	 , EG_LABEL_ALIGN_CENTER, "Classic A", C_1_4(r3, 9, 2));
	(void) EG_Window_AddWidget(gui_p->game_options_2.win, gui_p->game_options_2.lbl_f_classic_a);
	nr=C_1_4(r3, 11, 2); nr.y-=2; nr.h+=3; gui_p->game_options_2.but_f_classic_a = EG_Button_Create("game_options_2_f_classic_a_but", c
	 , EG_BUTTON_ALIGN_CENTER, "NOTHING", nr );
	(void) EG_Button_SetMyCallback_OnClick(gui_p->game_options_2.but_f_classic_a, GameOptions2_Callback_2_MapClassicA, gui_p);
	(void) EG_Window_AddWidget(gui_p->game_options_2.win, gui_p->game_options_2.but_f_classic_a);

	gui_p->game_options_2.lbl_f_classic_b = EG_Label_Create("game_options_2_f_classic_b_lbl", c
	 , EG_LABEL_ALIGN_CENTER, "Classic B", C_2_4(r3, 9, 2));
	(void) EG_Window_AddWidget(gui_p->game_options_2.win, gui_p->game_options_2.lbl_f_classic_b);
	nr=C_2_4(r3, 11, 2); nr.y-=2; nr.h+=3; gui_p->game_options_2.but_f_classic_b = EG_Button_Create("game_options_2_f_classic_b_but", c
	 , EG_BUTTON_ALIGN_CENTER, "NOTHING", nr );
	(void) EG_Button_SetMyCallback_OnClick(gui_p->game_options_2.but_f_classic_b, GameOptions2_Callback_2_MapClassicB, gui_p);
	(void) EG_Window_AddWidget(gui_p->game_options_2.win, gui_p->game_options_2.but_f_classic_b);

	gui_p->game_options_2.lbl_f_classic_x = EG_Label_Create("game_options_2_f_classic_x_lbl", c
	 , EG_LABEL_ALIGN_CENTER, "Classic X", C_3_4(r3, 9, 2));
	(void) EG_Window_AddWidget(gui_p->game_options_2.win, gui_p->game_options_2.lbl_f_classic_x);
	nr=C_3_4(r3, 11, 2); nr.y-=2; nr.h+=3; gui_p->game_options_2.but_f_classic_x = EG_Button_Create("game_options_2_f_classic_x_but", c
	 , EG_BUTTON_ALIGN_CENTER, "NOTHING", nr );
	(void) EG_Button_SetMyCallback_OnClick(gui_p->game_options_2.but_f_classic_x, GameOptions2_Callback_2_MapClassicX, gui_p);
	(void) EG_Window_AddWidget(gui_p->game_options_2.win, gui_p->game_options_2.but_f_classic_x);

	gui_p->game_options_2.lbl_f_classic_y = EG_Label_Create("game_options_2_f_classic_y_lbl", c
	 , EG_LABEL_ALIGN_CENTER, "Classic Y", C_4_4(r3, 9, 2));
	(void) EG_Window_AddWidget(gui_p->game_options_2.win, gui_p->game_options_2.lbl_f_classic_y);
	nr=C_4_4(r3, 11, 2); nr.y-=2; nr.h+=3; gui_p->game_options_2.but_f_classic_y = EG_Button_Create("game_options_2_f_classic_y_but", c
	 , EG_BUTTON_ALIGN_CENTER, "NOTHING", nr );
	(void) EG_Button_SetMyCallback_OnClick(gui_p->game_options_2.but_f_classic_y, GameOptions2_Callback_2_MapClassicY, gui_p);
	(void) EG_Window_AddWidget(gui_p->game_options_2.win, gui_p->game_options_2.but_f_classic_y);

	// Volume Up	Volume Down
	// <Key name>	<Key name>
 	//
	gui_p->game_options_2.lbl_f_classic_l = EG_Label_Create("game_options_2_f_classic_l_lbl", c
	 , EG_LABEL_ALIGN_CENTER, "Classic L", C_1_4(r3, 13, 2));
	(void) EG_Window_AddWidget(gui_p->game_options_2.win, gui_p->game_options_2.lbl_f_classic_l);
	nr=C_2_4(r3, 15, 2); nr.y-=2; nr.h+=3; gui_p->game_options_2.but_f_classic_l = EG_Button_Create("game_options_2_f_classic_l_but", c
	 , EG_BUTTON_ALIGN_CENTER, "NOTHING", nr );
	(void) EG_Button_SetMyCallback_OnClick(gui_p->game_options_2.but_f_classic_l, GameOptions2_Callback_2_MapClassicL, gui_p);
	(void) EG_Window_AddWidget(gui_p->game_options_2.win, gui_p->game_options_2.but_f_classic_l);

	gui_p->game_options_2.lbl_f_classic_r = EG_Label_Create("game_options_2_f_classic_r_lbl", c
	 , EG_LABEL_ALIGN_CENTER, "Classic R", C_2_4(r3, 13, 2));
	(void) EG_Window_AddWidget(gui_p->game_options_2.win, gui_p->game_options_2.lbl_f_classic_r);
	nr=C_1_4(r3, 15, 2); nr.y-=2; nr.h+=3; gui_p->game_options_2.but_f_classic_r = EG_Button_Create("game_options_2_f_classic_r_but", c
	 , EG_BUTTON_ALIGN_CENTER, "NOTHING", nr );
	(void) EG_Button_SetMyCallback_OnClick(gui_p->game_options_2.but_f_classic_r, GameOptions2_Callback_2_MapClassicR, gui_p);
	(void) EG_Window_AddWidget(gui_p->game_options_2.win, gui_p->game_options_2.but_f_classic_r);


	/* Add back button:
	 */
	gui_p->game_options_2.but_back = EG_Button_Create("game_options_2_back_button", c
	 , EG_BUTTON_ALIGN_CENTER, "Back to menu", C_4_4(r2, 43, 3) );
	(void) EG_Button_SetMyCallback_OnClick(gui_p->game_options_2.but_back
	 , GameOptions2_Callback_Back, gui_p);
	(void) EG_Window_AddWidget(gui_p->game_options_2.win, gui_p->game_options_2.but_back);


//	/* Add stick mode button:
// 	 */
//	gui_p->game_options_2.but_stick = EG_Button_Create("game_options_2_stick_button", c, EG_BUTTON_ALIGN_LEFT, "", C_1_2(r2, 44, 3) );
//	(void) EG_Button_SetMyCallback_OnClick(gui_p->game_options_2.but_stick, GameOptions2_Callback_Stick, gui_p);
//	(void) EG_Window_AddWidget(gui_p->game_options_2.win, gui_p->game_options_2.but_stick);


	/* Focus:
	*/

	/* PLAYER #2 */
	EG_Widget_SetSmartFocusPeerRight(gui_p->game_options_2.but_f_left		, gui_p->game_options_2.but_f_right);
	EG_Widget_SetSmartFocusPeerDown(gui_p->game_options_2.but_f_left		, gui_p->game_options_2.but_f_a);

	EG_Widget_SetSmartFocusPeerLeft(gui_p->game_options_2.but_f_right		, gui_p->game_options_2.but_f_left);
	EG_Widget_SetSmartFocusPeerRight(gui_p->game_options_2.but_f_right		, gui_p->game_options_2.but_f_up);
	EG_Widget_SetSmartFocusPeerDown(gui_p->game_options_2.but_f_right		, gui_p->game_options_2.but_f_b);

	EG_Widget_SetSmartFocusPeerLeft(gui_p->game_options_2.but_f_up		, gui_p->game_options_2.but_f_right);
	EG_Widget_SetSmartFocusPeerRight(gui_p->game_options_2.but_f_up		, gui_p->game_options_2.but_f_down);
	EG_Widget_SetSmartFocusPeerDown(gui_p->game_options_2.but_f_up		, gui_p->game_options_2.but_f_x);

	EG_Widget_SetSmartFocusPeerLeft(gui_p->game_options_2.but_f_down		, gui_p->game_options_2.but_f_up);
	EG_Widget_SetSmartFocusPeerDown(gui_p->game_options_2.but_f_down		, gui_p->game_options_2.but_f_y);

	EG_Widget_SetSmartFocusPeerRight(gui_p->game_options_2.but_f_a		, gui_p->game_options_2.but_f_b);
	EG_Widget_SetSmartFocusPeerUp(gui_p->game_options_2.but_f_a		, gui_p->game_options_2.but_f_left);
	EG_Widget_SetSmartFocusPeerDown(gui_p->game_options_2.but_f_a		, gui_p->game_options_2.but_f_classic_a);

	EG_Widget_SetSmartFocusPeerLeft(gui_p->game_options_2.but_f_b		, gui_p->game_options_2.but_f_a);
	EG_Widget_SetSmartFocusPeerRight(gui_p->game_options_2.but_f_b		, gui_p->game_options_2.but_f_x);
	EG_Widget_SetSmartFocusPeerUp(gui_p->game_options_2.but_f_b		, gui_p->game_options_2.but_f_right);
	EG_Widget_SetSmartFocusPeerDown(gui_p->game_options_2.but_f_b		, gui_p->game_options_2.but_f_classic_b);

	EG_Widget_SetSmartFocusPeerLeft(gui_p->game_options_2.but_f_x		, gui_p->game_options_2.but_f_b);
	EG_Widget_SetSmartFocusPeerRight(gui_p->game_options_2.but_f_x		, gui_p->game_options_2.but_f_y);
	EG_Widget_SetSmartFocusPeerUp(gui_p->game_options_2.but_f_x		, gui_p->game_options_2.but_f_up);
	EG_Widget_SetSmartFocusPeerDown(gui_p->game_options_2.but_f_x		, gui_p->game_options_2.but_f_classic_x);

	EG_Widget_SetSmartFocusPeerLeft(gui_p->game_options_2.but_f_y		, gui_p->game_options_2.but_f_x);
	EG_Widget_SetSmartFocusPeerUp(gui_p->game_options_2.but_f_y		, gui_p->game_options_2.but_f_down);
	EG_Widget_SetSmartFocusPeerDown(gui_p->game_options_2.but_f_y		, gui_p->game_options_2.but_f_classic_y);

	EG_Widget_SetSmartFocusPeerRight(gui_p->game_options_2.but_f_classic_a		, gui_p->game_options_2.but_f_classic_b);
	EG_Widget_SetSmartFocusPeerUp(gui_p->game_options_2.but_f_classic_a		, gui_p->game_options_2.but_f_a);
	EG_Widget_SetSmartFocusPeerDown(gui_p->game_options_2.but_f_classic_a		, gui_p->game_options_2.but_f_classic_r);

	EG_Widget_SetSmartFocusPeerLeft(gui_p->game_options_2.but_f_classic_b		, gui_p->game_options_2.but_f_classic_a);
	EG_Widget_SetSmartFocusPeerRight(gui_p->game_options_2.but_f_classic_b		, gui_p->game_options_2.but_f_classic_x);
	EG_Widget_SetSmartFocusPeerUp(gui_p->game_options_2.but_f_classic_b		, gui_p->game_options_2.but_f_b);
	EG_Widget_SetSmartFocusPeerDown(gui_p->game_options_2.but_f_classic_b		, gui_p->game_options_2.but_f_classic_l);

	EG_Widget_SetSmartFocusPeerLeft(gui_p->game_options_2.but_f_classic_x	, gui_p->game_options_2.but_f_classic_b);
	EG_Widget_SetSmartFocusPeerRight(gui_p->game_options_2.but_f_classic_x	, gui_p->game_options_2.but_f_classic_y);
	EG_Widget_SetSmartFocusPeerUp(gui_p->game_options_2.but_f_classic_x		, gui_p->game_options_2.but_f_x);

	EG_Widget_SetSmartFocusPeerLeft(gui_p->game_options_2.but_f_classic_y		, gui_p->game_options_2.but_f_classic_x);
	EG_Widget_SetSmartFocusPeerUp(gui_p->game_options_2.but_f_classic_y		, gui_p->game_options_2.but_f_y);

	EG_Widget_SetSmartFocusPeerRight(gui_p->game_options_2.but_f_classic_r	, gui_p->game_options_2.but_f_classic_l);
	EG_Widget_SetSmartFocusPeerUp(gui_p->game_options_2.but_f_classic_r		, gui_p->game_options_2.but_f_classic_a);

	EG_Widget_SetSmartFocusPeerLeft(gui_p->game_options_2.but_f_classic_l	, gui_p->game_options_2.but_f_classic_r);
	EG_Widget_SetSmartFocusPeerUp(gui_p->game_options_2.but_f_classic_l		, gui_p->game_options_2.but_f_classic_b);


	/* PLAYER #1 */
	EG_Widget_SetSmartFocusPeerRight(gui_p->game_options_2.but_left		, gui_p->game_options_2.but_right);
	EG_Widget_SetSmartFocusPeerDown(gui_p->game_options_2.but_left		, gui_p->game_options_2.but_a);
	EG_Widget_SetSmartFocusPeerUp(gui_p->game_options_2.but_left		, gui_p->game_options_2.but_f_classic_r);

	EG_Widget_SetSmartFocusPeerLeft(gui_p->game_options_2.but_right		, gui_p->game_options_2.but_left);
	EG_Widget_SetSmartFocusPeerRight(gui_p->game_options_2.but_right		, gui_p->game_options_2.but_up);
	EG_Widget_SetSmartFocusPeerDown(gui_p->game_options_2.but_right		, gui_p->game_options_2.but_b);
	EG_Widget_SetSmartFocusPeerUp(gui_p->game_options_2.but_right		, gui_p->game_options_2.but_f_classic_l);

	EG_Widget_SetSmartFocusPeerLeft(gui_p->game_options_2.but_up		, gui_p->game_options_2.but_right);
	EG_Widget_SetSmartFocusPeerRight(gui_p->game_options_2.but_up		, gui_p->game_options_2.but_down);
	EG_Widget_SetSmartFocusPeerDown(gui_p->game_options_2.but_up		, gui_p->game_options_2.but_x);
	EG_Widget_SetSmartFocusPeerUp(gui_p->game_options_2.but_up		, gui_p->game_options_2.but_f_classic_x);

	EG_Widget_SetSmartFocusPeerLeft(gui_p->game_options_2.but_down		, gui_p->game_options_2.but_up);
	EG_Widget_SetSmartFocusPeerDown(gui_p->game_options_2.but_down		, gui_p->game_options_2.but_y);
	EG_Widget_SetSmartFocusPeerUp(gui_p->game_options_2.but_down		, gui_p->game_options_2.but_f_classic_y);

	EG_Widget_SetSmartFocusPeerRight(gui_p->game_options_2.but_a		, gui_p->game_options_2.but_b);
	EG_Widget_SetSmartFocusPeerUp(gui_p->game_options_2.but_a		, gui_p->game_options_2.but_left);
	EG_Widget_SetSmartFocusPeerDown(gui_p->game_options_2.but_a		, gui_p->game_options_2.but_classic_a);

	EG_Widget_SetSmartFocusPeerLeft(gui_p->game_options_2.but_b		, gui_p->game_options_2.but_a);
	EG_Widget_SetSmartFocusPeerRight(gui_p->game_options_2.but_b		, gui_p->game_options_2.but_x);
	EG_Widget_SetSmartFocusPeerUp(gui_p->game_options_2.but_b		, gui_p->game_options_2.but_right);
	EG_Widget_SetSmartFocusPeerDown(gui_p->game_options_2.but_b		, gui_p->game_options_2.but_classic_b);

	EG_Widget_SetSmartFocusPeerLeft(gui_p->game_options_2.but_x		, gui_p->game_options_2.but_b);
	EG_Widget_SetSmartFocusPeerRight(gui_p->game_options_2.but_x		, gui_p->game_options_2.but_y);
	EG_Widget_SetSmartFocusPeerUp(gui_p->game_options_2.but_x		, gui_p->game_options_2.but_up);
	EG_Widget_SetSmartFocusPeerDown(gui_p->game_options_2.but_x		, gui_p->game_options_2.but_classic_x);

	EG_Widget_SetSmartFocusPeerLeft(gui_p->game_options_2.but_y		, gui_p->game_options_2.but_x);
	EG_Widget_SetSmartFocusPeerUp(gui_p->game_options_2.but_y		, gui_p->game_options_2.but_down);
	EG_Widget_SetSmartFocusPeerDown(gui_p->game_options_2.but_y		, gui_p->game_options_2.but_classic_y);

	EG_Widget_SetSmartFocusPeerRight(gui_p->game_options_2.but_classic_a		, gui_p->game_options_2.but_classic_b);
	EG_Widget_SetSmartFocusPeerUp(gui_p->game_options_2.but_classic_a		, gui_p->game_options_2.but_a);
	EG_Widget_SetSmartFocusPeerDown(gui_p->game_options_2.but_classic_a		, gui_p->game_options_2.but_classic_r);

	EG_Widget_SetSmartFocusPeerLeft(gui_p->game_options_2.but_classic_b		, gui_p->game_options_2.but_classic_a);
	EG_Widget_SetSmartFocusPeerRight(gui_p->game_options_2.but_classic_b		, gui_p->game_options_2.but_classic_x);
	EG_Widget_SetSmartFocusPeerUp(gui_p->game_options_2.but_classic_b		, gui_p->game_options_2.but_b);
	EG_Widget_SetSmartFocusPeerDown(gui_p->game_options_2.but_classic_b		, gui_p->game_options_2.but_classic_l);

	EG_Widget_SetSmartFocusPeerLeft(gui_p->game_options_2.but_classic_x	, gui_p->game_options_2.but_classic_b);
	EG_Widget_SetSmartFocusPeerRight(gui_p->game_options_2.but_classic_x	, gui_p->game_options_2.but_classic_y);
	EG_Widget_SetSmartFocusPeerUp(gui_p->game_options_2.but_classic_x		, gui_p->game_options_2.but_x);

	EG_Widget_SetSmartFocusPeerLeft(gui_p->game_options_2.but_classic_y		, gui_p->game_options_2.but_classic_x);
	EG_Widget_SetSmartFocusPeerUp(gui_p->game_options_2.but_classic_y		, gui_p->game_options_2.but_y);
	EG_Widget_SetSmartFocusPeerDown(gui_p->game_options_2.but_classic_y		, gui_p->game_options_2.but_back);

	EG_Widget_SetSmartFocusPeerRight(gui_p->game_options_2.but_classic_r	, gui_p->game_options_2.but_classic_l);
	EG_Widget_SetSmartFocusPeerUp(gui_p->game_options_2.but_classic_r		, gui_p->game_options_2.but_classic_a);
	EG_Widget_SetSmartFocusPeerLeft(gui_p->game_options_2.but_classic_l	, gui_p->game_options_2.but_classic_r);
	EG_Widget_SetSmartFocusPeerUp(gui_p->game_options_2.but_classic_l		, gui_p->game_options_2.but_classic_b);
	EG_Widget_SetSmartFocusPeerUp(gui_p->game_options_2.but_back		, gui_p->game_options_2.but_classic_y);

	return EG_TRUE;
}

static void GameOptions2_Destroy(BeebEm_Pages *gui_p)
{
	EG_Window_DestroyAllChildWidgets(gui_p->game_options_2.win);
	EG_Window_Destroy(gui_p->game_options_2.win);
}







/*
 *
 * V O L U M E :
 *
 */


static void BeebEmPages_ShowVolumePage(BeebEm_Pages *gui_p)
{
	unsigned int volume;

#ifdef GP2X
	volume = (OSS_Mixer_GetVolume()>>1)<<1;
#endif

	// Set focus and select widget that matches volume.


	// [TODO] Make rbt_0 - 100 an array...

	//switch(volume) {
	switch(config.settings.volume) {
		case 0:
			EG_RadioGroup_Select(gui_p->volume.rbt_0);
			break;
		case 25:
			EG_RadioGroup_Select(gui_p->volume.rbt_10);
			break;
		case 50:
			EG_RadioGroup_Select(gui_p->volume.rbt_20);
			break;
		case 75:
			EG_RadioGroup_Select(gui_p->volume.rbt_30);
			break;
		case 100:
			EG_RadioGroup_Select(gui_p->volume.rbt_40);
			break;
		case 125:
			EG_RadioGroup_Select(gui_p->volume.rbt_50);
			break;
		case 150:
			EG_RadioGroup_Select(gui_p->volume.rbt_60);
			break;
		case 175:
			EG_RadioGroup_Select(gui_p->volume.rbt_70);
			break;
		case 200:
			EG_RadioGroup_Select(gui_p->volume.rbt_80);
			break;
		case 225:
			EG_RadioGroup_Select(gui_p->volume.rbt_90);
			break;
		case 255:
			EG_RadioGroup_Select(gui_p->volume.rbt_100);
			break;
	}

//        ChangeMenuTo(gui_p->volume.win);

                window_p = gui_p->volume.win;
                EG_Window_Show(window_p); // [TODO] make this window_p


}


// [TODO] Use one callback for this.

static void Menu_Callback_Volume_0(EG_Widget*, void* user_p)
{
        BeebEm_Pages *gui_p = (BeebEm_Pages*) user_p;

	ActionSetOption("VOLUME", "0");
        ChangeMenuTo(gui_p->menu.win);
}

static void Menu_Callback_Volume_10(EG_Widget*, void* user_p)
{
        BeebEm_Pages *gui_p = (BeebEm_Pages*) user_p;

	ActionSetOption("VOLUME", "25");
        ChangeMenuTo(gui_p->menu.win);
}

static void Menu_Callback_Volume_20(EG_Widget*, void* user_p)
{
        BeebEm_Pages *gui_p = (BeebEm_Pages*) user_p;

	ActionSetOption("VOLUME", "50");
        ChangeMenuTo(gui_p->menu.win);
}

static void Menu_Callback_Volume_30(EG_Widget*, void* user_p)
{
        BeebEm_Pages *gui_p = (BeebEm_Pages*) user_p;

	ActionSetOption("VOLUME", "75");
        ChangeMenuTo(gui_p->menu.win);
}

static void Menu_Callback_Volume_40(EG_Widget*, void* user_p)
{
        BeebEm_Pages *gui_p = (BeebEm_Pages*) user_p;

	ActionSetOption("VOLUME", "100");
        ChangeMenuTo(gui_p->menu.win);
}

static void Menu_Callback_Volume_50(EG_Widget*, void* user_p)
{
        BeebEm_Pages *gui_p = (BeebEm_Pages*) user_p;

	ActionSetOption("VOLUME", "125");
        ChangeMenuTo(gui_p->menu.win);
}

static void Menu_Callback_Volume_60(EG_Widget*, void* user_p)
{
        BeebEm_Pages *gui_p = (BeebEm_Pages*) user_p;

	ActionSetOption("VOLUME", "150");
        ChangeMenuTo(gui_p->menu.win);
}

static void Menu_Callback_Volume_70(EG_Widget*, void* user_p)
{
        BeebEm_Pages *gui_p = (BeebEm_Pages*) user_p;

	ActionSetOption("VOLUME", "175");
        ChangeMenuTo(gui_p->menu.win);
}

static void Menu_Callback_Volume_80(EG_Widget*, void* user_p)
{
        BeebEm_Pages *gui_p = (BeebEm_Pages*) user_p;

	ActionSetOption("VOLUME", "200");
        ChangeMenuTo(gui_p->menu.win);
}

static void Menu_Callback_Volume_90(EG_Widget*, void* user_p)
{
        BeebEm_Pages *gui_p = (BeebEm_Pages*) user_p;

	ActionSetOption("VOLUME", "225");
        ChangeMenuTo(gui_p->menu.win);
}

static void Menu_Callback_Volume_100(EG_Widget*, void* user_p)
{
        BeebEm_Pages *gui_p = (BeebEm_Pages*) user_p;

	ActionSetOption("VOLUME", "255");
        ChangeMenuTo(gui_p->menu.win);
}

static EG_BOOL Volume_Make(SDL_Surface *frame_buffer_p, BeebEm_Pages *gui_p)
{
	SDL_Color c;
	SDL_Rect r, r2, r3;

	/* Default colors:
	 */
//	c = (SDL_Color) {0, 255-64, 0, 0};
	c = DARKER_MENU_COLORS;

	r = (SDL_Rect) { 32, GUI_HEIGHT/2 - 32, GUI_WIDTH - 64, 64 };
	gui_p->volume.win = EG_Window_Create("volume_win", frame_buffer_p, c, r);
	r2 = EG_Window_GetDimension(gui_p->volume.win); r2.x+=2; r2.w -=4;

	r2.x+=4; r2.w-=8;

	gui_p->volume.lbl_title = EG_Label_Create("volume_title", EG_COLOR_ALT, EG_LABEL_ALIGN_CENTER, "Volume:", C_1_1(r2, 1, 2));
	(void) EG_Window_AddWidget(gui_p->volume.win, gui_p->volume.lbl_title);

	r3 = C_1_1(r2, 4, 6);
	gui_p->volume.box = EG_Box_Create("volume_box", EG_BOX_BORDER_SUNK, c, r3);
	(void) EG_Window_AddWidget(gui_p->volume.win, gui_p->volume.box);
	r3.x+=4; r3.w-=8;



// gui_p->options.rgp_protection_0 = EG_RadioGroup_Create("options_protection_0_rgp");
//
//        gui_p->options.rbt_protection_0_rw = EG_ToggleButton_Create(
//         "options_protection_0_rw", c2, "R/W", C_2_6(r3, 44, 4) );
//        EG_RadioGroup_AddButton(gui_p->options.rgp_protection_0
//         , gui_p->options.rbt_protection_0_rw);
//
//        gui_p->options.rbt_protection_0_ro = EG_ToggleButton_Create(
//         "options_protection_0_ro", c2, "R/O", C_3_6(r3, 44, 4) );
//        EG_RadioGroup_AddButton(gui_p->options.rgp_protection_0
//         , gui_p->options.rbt_protection_0_ro);
//
//       EG_TabPage_AddWidget(gui_p->options.tpg_options, gui_p->options.rgp_protection_0);

	
	gui_p->volume.rgp = EG_RadioGroup_Create("volume_rgp");

	gui_p->volume.rbt_0 = EG_ToggleButton_Create("volume_0", c, "0", C_1_11(r3, 1, 4) );
	(void) EG_ToggleButton_SetMyCallback_OnClick(gui_p->volume.rbt_0, Menu_Callback_Volume_0, gui_p);
	EG_RadioGroup_AddButton(gui_p->volume.rgp, gui_p->volume.rbt_0);

	gui_p->volume.rbt_10 = EG_ToggleButton_Create("volume_10", c, "1", C_2_11(r3, 1, 4) );
	(void) EG_ToggleButton_SetMyCallback_OnClick(gui_p->volume.rbt_10, Menu_Callback_Volume_10, gui_p);
	EG_RadioGroup_AddButton(gui_p->volume.rgp, gui_p->volume.rbt_10);

	gui_p->volume.rbt_20 = EG_ToggleButton_Create("volume_20", c, "2", C_3_11(r3, 1, 4) );
	(void) EG_ToggleButton_SetMyCallback_OnClick(gui_p->volume.rbt_20, Menu_Callback_Volume_20, gui_p);
	EG_RadioGroup_AddButton(gui_p->volume.rgp, gui_p->volume.rbt_20);

	gui_p->volume.rbt_30 = EG_ToggleButton_Create("volume_30", c, "3", C_4_11(r3, 1, 4) );
	(void) EG_ToggleButton_SetMyCallback_OnClick(gui_p->volume.rbt_30, Menu_Callback_Volume_30, gui_p);
	EG_RadioGroup_AddButton(gui_p->volume.rgp, gui_p->volume.rbt_30);

	gui_p->volume.rbt_40 = EG_ToggleButton_Create("volume_40", c, "4", C_5_11(r3, 1, 4) );
	(void) EG_ToggleButton_SetMyCallback_OnClick(gui_p->volume.rbt_40, Menu_Callback_Volume_40, gui_p);
	EG_RadioGroup_AddButton(gui_p->volume.rgp, gui_p->volume.rbt_40);

	gui_p->volume.rbt_50 = EG_ToggleButton_Create("volume_50", c, "5", C_6_11(r3, 1, 4) );
	(void) EG_ToggleButton_SetMyCallback_OnClick(gui_p->volume.rbt_50, Menu_Callback_Volume_50, gui_p);
	EG_RadioGroup_AddButton(gui_p->volume.rgp, gui_p->volume.rbt_50);

	gui_p->volume.rbt_60 = EG_ToggleButton_Create("volume_60", c, "6", C_7_11(r3, 1, 4) );
	(void) EG_ToggleButton_SetMyCallback_OnClick(gui_p->volume.rbt_60, Menu_Callback_Volume_60, gui_p);
	EG_RadioGroup_AddButton(gui_p->volume.rgp, gui_p->volume.rbt_60);

	gui_p->volume.rbt_70 = EG_ToggleButton_Create("volume_70", c, "7", C_8_11(r3, 1, 4) );
	(void) EG_ToggleButton_SetMyCallback_OnClick(gui_p->volume.rbt_70, Menu_Callback_Volume_70, gui_p);
	EG_RadioGroup_AddButton(gui_p->volume.rgp, gui_p->volume.rbt_70);

	gui_p->volume.rbt_80 = EG_ToggleButton_Create("volume_80", c, "8", C_9_11(r3, 1, 4) );
	(void) EG_ToggleButton_SetMyCallback_OnClick(gui_p->volume.rbt_80, Menu_Callback_Volume_80, gui_p);
	EG_RadioGroup_AddButton(gui_p->volume.rgp, gui_p->volume.rbt_80);

	gui_p->volume.rbt_90 = EG_ToggleButton_Create("volume_90", c, "9", C_10_11(r3, 1, 4) );
	(void) EG_ToggleButton_SetMyCallback_OnClick(gui_p->volume.rbt_90, Menu_Callback_Volume_90, gui_p);
	EG_RadioGroup_AddButton(gui_p->volume.rgp, gui_p->volume.rbt_90);

	gui_p->volume.rbt_100 = EG_ToggleButton_Create("volume_100", c, "10", C_11_11(r3, 1, 4) );
	(void) EG_ToggleButton_SetMyCallback_OnClick(gui_p->volume.rbt_100, Menu_Callback_Volume_100, gui_p);
	EG_RadioGroup_AddButton(gui_p->volume.rgp, gui_p->volume.rbt_100);

	EG_Window_AddWidget(gui_p->volume.win, gui_p->volume.rgp);

	EG_Widget_SetSmartFocusPeerLeft (gui_p->volume.rbt_0, gui_p->volume.rbt_100);
	EG_Widget_SetSmartFocusPeerRight(gui_p->volume.rbt_0, gui_p->volume.rbt_10);

	EG_Widget_SetSmartFocusPeerLeft (gui_p->volume.rbt_10, gui_p->volume.rbt_0);
	EG_Widget_SetSmartFocusPeerRight(gui_p->volume.rbt_10, gui_p->volume.rbt_20);

	EG_Widget_SetSmartFocusPeerLeft (gui_p->volume.rbt_20, gui_p->volume.rbt_10);
	EG_Widget_SetSmartFocusPeerRight(gui_p->volume.rbt_20, gui_p->volume.rbt_30);

	EG_Widget_SetSmartFocusPeerLeft (gui_p->volume.rbt_30, gui_p->volume.rbt_20);
	EG_Widget_SetSmartFocusPeerRight(gui_p->volume.rbt_30, gui_p->volume.rbt_40);

	EG_Widget_SetSmartFocusPeerLeft (gui_p->volume.rbt_40, gui_p->volume.rbt_30);
	EG_Widget_SetSmartFocusPeerRight(gui_p->volume.rbt_40, gui_p->volume.rbt_50);

	EG_Widget_SetSmartFocusPeerLeft (gui_p->volume.rbt_50, gui_p->volume.rbt_40);
	EG_Widget_SetSmartFocusPeerRight(gui_p->volume.rbt_50, gui_p->volume.rbt_60);

	EG_Widget_SetSmartFocusPeerLeft (gui_p->volume.rbt_60, gui_p->volume.rbt_50);
	EG_Widget_SetSmartFocusPeerRight(gui_p->volume.rbt_60, gui_p->volume.rbt_70);

	EG_Widget_SetSmartFocusPeerLeft (gui_p->volume.rbt_70, gui_p->volume.rbt_60);
	EG_Widget_SetSmartFocusPeerRight(gui_p->volume.rbt_70, gui_p->volume.rbt_80);

	EG_Widget_SetSmartFocusPeerLeft (gui_p->volume.rbt_80, gui_p->volume.rbt_70);
	EG_Widget_SetSmartFocusPeerRight(gui_p->volume.rbt_80, gui_p->volume.rbt_90);

	EG_Widget_SetSmartFocusPeerLeft (gui_p->volume.rbt_90, gui_p->volume.rbt_80);
	EG_Widget_SetSmartFocusPeerRight(gui_p->volume.rbt_90, gui_p->volume.rbt_100);

	EG_Widget_SetSmartFocusPeerLeft (gui_p->volume.rbt_100, gui_p->volume.rbt_90);
	EG_Widget_SetSmartFocusPeerRight(gui_p->volume.rbt_100, gui_p->volume.rbt_0);

        return EG_TRUE;
}

static void Volume_Destroy(BeebEm_Pages *gui_p)
{
        EG_Window_DestroyAllChildWidgets(gui_p->volume.win);
        EG_Window_Destroy(gui_p->volume.win);
}

