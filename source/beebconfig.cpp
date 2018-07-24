#include "beebconfig.h"
#include "sysvia.h"
#include "main.h"
#include "disc8271.h"
#include "video.h"
#include "keyboard.h"
#include "messagebox.h"
#include "messages.h"
#include "stick.h"

//#include "virtualkeyboard.h"


#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#ifdef GEKKO
#include <gccore.h>
#include <ogcsys.h>
#include <dirent.h>
#endif

#include <gui/gui.h>

/* BeebEm configuration:
 */
BeebConfig config;


void ClearXX(void)
{
//	unsigned char *p = (unsigned char*) config.files.xx;
//
//	for(int i=0; i<1024*5; i++)
//		*p++ = 'X';
}

void PrintXX(char* name_p)
{
//	unsigned char *p = (unsigned char*) config.files.xx;
//
//	printf("%s:\n", name_p);
//
//	for(int i=0; i<1024*5; i++) {
//		printf("%c", (int) *p == 0x00 ? '.' : *p);
//		p++;
//	}
//
//	printf("\n");
//	SDL_Delay(1000);
}



static int LoadDiscImage(const char *filename_p)
{
	char filepath[256+1], mappingpath[256+1], *tmp_p;
	FILE *test;
	int is_invalid_format;

	/* If no filename given then do nothing:
	 * No name is considered successful.
	 */
	if (filename_p == NULL || strlen(filename_p) == 0)
		return 1;

#ifdef GEKKO
	strcpy(filepath, filename_p);

	/* FIXME : Reset Beebem when we run new disc. Workaround for audio going mad! */
	if(forceReset)
		mainWin->Reset();
	forceReset = 0;
#endif
	printf("Trying to load disc image: '%s'\n", filepath);

	/* Make sure the file exists:
 	 */
	test = fopen(filepath, "r");
	if (test) fclose(test); else {
		printf("Disc image does not exist.\n");
		EG_MessageBox(frame_buffer_p, EG_MESSAGEBOX_STOP
		 , BEEBCONFIG_IMAGEDOESNOTEXIST_T
		 , BEEBCONFIG_IMAGEDOESNOTEXIST_M, "Continue"
		 , NULL, NULL, NULL, 0);

		return 0;
	}
///////
	
	/* Is the file extension legal (BeebEm disc images MUST
	 * have one):
	 */
	is_invalid_format = 1;
	if (strlen(filepath) >= 4) {

		/* single sided disc image:
		 */
		if (strcasecmp(filepath + strlen(filepath)-4, ".ssd") == 0) {
//			strcpy(config.drive_0_path, filepath);
			LoadSimpleDiscImage(filepath, 0, 0, 80);
			is_invalid_format = 0;
		}

		/* double sided disc image:
	 	 */
		if (strcasecmp(filepath + strlen(filepath)-4, ".dsd") == 0) {
//			strcpy(config.drive_0_path, filepath);
			LoadSimpleDSDiscImage(filepath, 0, 80);
			is_invalid_format = 0;
		}
	}


	/* If file extension is not recognized, warn user and fail:
	 */
	if (is_invalid_format == 1) {
		EG_MessageBox(frame_buffer_p, EG_MESSAGEBOX_STOP
		 , BEEBCONFIG_ILLEGALFILEEXTENSION_T, BEEBCONFIG_ILLEGALFILEEXTENSION_M, "Continue"
		 , NULL, NULL, NULL, 0);
		return 0;
	}


	/* Load the disc image's mapping file (if there is one):
	 */

	/* Try loading mapping file from the same dir as the disc image:
	 */
	strcpy(mappingpath, filepath);
	mappingpath[strlen(mappingpath)-4] = '\0';
	strcat(mappingpath, ".kbd");

	if (LoadKeyboardMapping(mappingpath) ) {
		printf("Loaded mapping file '%s'.\n", mappingpath);
	} else {

		/* Try and load mapping file from /kbd/
	 	 */
		mappingpath[0]='\0';
		if ( (tmp_p=strrchr(filepath, (int) '/')) != NULL) {
			tmp_p++;

			strcpy(mappingpath, config.files.kbd_files_dir);
			strcat(mappingpath, tmp_p);
			mappingpath[strlen(mappingpath)-4] = '\0';
			strcat(mappingpath, ".kbd");
		}

		if ( ! LoadKeyboardMapping(mappingpath) ) {
			printf("Unable to determine a mapping file for this disc image, falling back to defaults.\n");
	                EG_MessageBox(frame_buffer_p, EG_MESSAGEBOX_STOP
       	         	 , BEEBCONFIG_NOMAPPINGFILE_T, BEEBCONFIG_NOMAPPINGFILE_M, "Continue", NULL, NULL, NULL, 0);
		} else {
			printf("Loaded mapping file '%s'.\n", mappingpath);
		}
	}

	/* Tell emulator core beeb is being 'Shift booted' + depress SHIFT on Beeb.
	 */
	if (mainWin!=NULL) mainWin->ShiftBooted = true;
	BeebKeyDown(0, 0);
	// [TODO] Depress shift on the virtual keyboard.

	return 1;
}


int Config_LoadDiscConfig(const char *filename_p)
{
	int ret;

	/* Reset config to default settings:
	 */
        if ( ! LoadKeyboardMapping(config.files.default_config_loc) )
                printf("Unable to load default keyboard mapping file '%s'.\n", config.files.default_config_loc);
        else
                printf("Loaded default keyboard mapping file '%s'.\n", config.files.default_config_loc);


	/* Try and load disc image and it's specific settings:
	 */
	ret = LoadDiscImage(filename_p);


	/* Set FORCED settings:
	 */
        if ( ! LoadKeyboardMapping(config.files.forced_config_loc) )
                printf("Unable to load force keyboard mapping file '%s'.\n", config.files.forced_config_loc);
        else
                printf("Loaded default force mapping file '%s'.\n", config.files.forced_config_loc);

	/* Successful:
	 */
	return ret;
}



static void Initialize_Files(const char *executable_name_p)
{
	char cwd[MAX_DIR_LEN];

	/* Default /apps/beebem folder given by the check in main.cpp
	 */
	strcpy(cwd, executable_name_p);

	/* Root dir:
	 */
	strcpy(config.files.root_dir, cwd);
	strcat(config.files.root_dir, "/");

	/* Font location:
	 */
	strcpy(config.files.font_loc, cwd);
	strcat(config.files.font_loc, "/resources/font5x8.bmp");

	/* Teletext font location:
	 */
	strcpy(config.files.teletext_font_loc, cwd);
	strcat(config.files.teletext_font_loc, "/resources/teletext.bmp");

	/* Owl location:
 	 */
	strcpy(config.files.owl_loc, cwd);
	strcat(config.files.owl_loc, "/resources/owl.bmp");

	/* ROM image configuration file location:
	 */
	strcpy(config.files.romimg_rc_loc, cwd);
	strcat(config.files.romimg_rc_loc, "/config/romimg.rc");

	/* romimg_dir:
	 */
	strcpy(config.files.romimg_dir, cwd);
	strcat(config.files.romimg_dir, "/roms/");

	/* default configuration location:
	 */
	strcpy(config.files.default_config_loc, cwd);
	strcat(config.files.default_config_loc, "/config/default.kbd");

	/* forced configuration location:
	 */
	strcpy(config.files.forced_config_loc, cwd);
	strcat(config.files.forced_config_loc, "/config/forced.kbd");

	/* Default disc images dir:
	 */
	strcpy(config.files.disc_images_dir, cwd);
	strcat(config.files.disc_images_dir, "/discs/");

	/* Default save state dir:
 	 */
	strcpy(config.files.save_state_dir, cwd);
	strcat(config.files.save_state_dir, "/saves/");

	/* Keyboard files archive dir:
	 */
	strcpy(config.files.kbd_files_dir, cwd);
	strcat(config.files.kbd_files_dir, "/kbd/");

	/* Virtual keyboard graphic location:
	 */
	strcpy(config.files.virtual_keyboard_loc, cwd);
	strcat(config.files.virtual_keyboard_loc, "/resources/virtualkeyboard.bmp");


	/* GUI font loc:
	 */
//	strcpy(config.files.gui_font_loc, cwd);
//	strcat(config.files.gui_font_loc, "/resources/font5x8.bmp");

	/* Drives:
 	 */
	strcpy(config.drive_0_path, "");
	strcpy(config.drive_1_path, "");
}


static void RunStartupScript(void)
{
	char script[MAX_DIR_LEN];

	strcpy(script, config.files.root_dir);
	strcat(script, "startup.cnf");

        if ( ! LoadKeyboardMapping(script) )
                printf("Unable to load startup script '%s'.\n", script);
        else
                printf("Ran startup script '%s'.\n", script);
}


/* Initialize and Free resources associated with the configuration.
 */
void Config_Initialize(const char *executable_name_p)
{
	memset( &config, 0, sizeof(BeebConfig) );


	/* VSYNC
	 */
	config.settings.vsyncoffset = 32;
	
	/* Use skinned widgets by default:
	 */
	config.settings.skin = 1;

	/* Initialize to hardwired values - the default.cfg file will
	 * override these:
	 */
	Initialize_Files(executable_name_p);

	/* Run startup.cnf - sets defaults from a file rather than
	 * the internal hardwiring above:
	 */
	RunStartupScript();

	/* Load the default.cfg and forced.cfg file:
	 */
	(void)Config_LoadDiscConfig(NULL);
}

void Config_FreeResources()
{
	// None allocated.
}




void RemoveLeadingWhiteSpace(char *p)
{
        char *pp = p;
        int i=0;

        if (p == NULL) return;

        while ( isspace( (int) *pp++) )
                i++;

        if (i > 0) strcpy(p, p+i);
}

void RemoveTrailingWhiteSpace(char *p)
{
        int i;

        if (p == NULL || strlen(p)<=0) return;

        for(i=strlen(p)-1; i >= 0 && isspace(p[i]); i--)
                p[i]='\0';
}




/* SET options:
 */

int ParseSetOption(char *p, char **name_pp, char **value_pp)
{
        char *name_p, *value_p;

        RemoveLeadingWhiteSpace(p);
        if (strncasecmp(p, "SET", 3) != 0) return 0;

        /* Dump the actual SET command:
         */
        p+=3;

        /* Get name and values the command sets:
         */
        name_p = p;

        if ( (value_p=strchr(p, '=')) == NULL)
                return 0;
        else
                *value_p++='\0';

        /* Remove white space:
         */
        RemoveLeadingWhiteSpace(name_p);
        RemoveTrailingWhiteSpace(name_p);

        RemoveLeadingWhiteSpace(value_p);
        RemoveTrailingWhiteSpace(value_p);

        *name_pp = name_p;
        *value_pp = value_p;

        return 1;
}

static void AppendForwardSlashIfNone(char *p)
{
	if (p == NULL) return;
	if (strlen(p) <= 0) return;

	if (p[strlen(p)-1] != '/')
		strcat(p, "/");
}


void ActionSetOption(char *name_p, char *value_p)
{
#ifdef GP2X
        int value;

        /* Set processor speed:
         */
        if ( !strcasecmp (name_p, "speed")) {
                value = atoi(value_p);

		/* Up speed slightly for old config files when running 0.6+ etc:
		 */
		if (value<170) value=170;


		/* Make sure speed is within GP2X supported range:
		 */
                if (value>=100 && value<=260) {
                        printf("Setting processor speed to %d\n", value);
                        GP2x_SetCPUSpeed(value);
                } else {
                        printf("Invalid processor speed, please enter a value between 100 and 260Mhz.\n");
                }
        }

        /* Set volume:
         */
        if ( !strcasecmp (name_p, "volume")) {
                value = atoi(value_p);
                if (value>=0 && value<=100) {
                        value= (value>>1)<<1;
                        printf("Setting volume to %d\n", value);
                        OSS_Mixer_SetVolume(value);
                } else {
                        printf("Invalid volume, please enter a value between 0 and 100.\n");
                }
        }
#endif
        /* Set volume:
         */
        int value;
        if ( !strcasecmp (name_p, "volume")) {
                value = atoi(value_p);

		if (value>=0 && value<=255) {
                        //value= (value>>1)<<1;
                        printf("Setting volume to %d\n", value);
                        SetVolume(value);
                } else {
                        printf("Invalid volume, please enter a value between 0 and 100.\n");
                }
        }


        /* Set audio latency:
         */
        if ( !strcasecmp (name_p, "latency")) {
		if ( (!strcasecmp (value_p, "1")) || (!strcasecmp (value_p, "true")) || (!strcasecmp (value_p, "yes")) || (!strcasecmp (value_p, "y")) ) {
			config.settings.latency = 1;
		} else {
			config.settings.latency = 0;
		}

        }

        /* Set 240p mode:
         */
        if ( !strcasecmp (name_p, "240p")) {
		if ( (!strcasecmp (value_p, "1")) || (!strcasecmp (value_p, "true")) || (!strcasecmp (value_p, "yes")) || (!strcasecmp (value_p, "y")) ) {
			config.settings.doublestrike = 1;
		} else {
			config.settings.doublestrike = 0;
		}
		Set240p(config.settings.doublestrike);
        }
//

	/* Set Bilinear filter:
	 */
	if ( !strcasecmp (name_p, "enable_filter")) {
		if ( (!strcasecmp (value_p, "1")) || (!strcasecmp (value_p, "true")) || (!strcasecmp (value_p, "yes")) || (!strcasecmp (value_p, "y")) ) {
			config.settings.enable_filter = 1;
		} else {
			config.settings.enable_filter = 0;
		}
		SetBilinear(config.settings.enable_filter);
	}


	/* Set disable_cursor:
	 */
	if ( !strcasecmp (name_p, "disable_cursor")) {
		if ( (!strcasecmp (value_p, "1")) || (!strcasecmp (value_p, "true")) || (!strcasecmp (value_p, "yes")) || (!strcasecmp (value_p, "y")) ) {
			config.settings.disable_cursor = 1;
			printf("Setting hardware cursor to: DISABLED\n");
		} else {
			config.settings.disable_cursor = 0;
			printf("Setting hardware cursor to: ENABLED\n");
		}
	}

        /* Set vscale:
         */
        if ( !strcasecmp (name_p, "vscale")) {
                if ( !strcasecmp (value_p, "centered")) {
                        printf("Setting screen centering to: centered.\n");
                        SetVideoCentered();
                } else if ( !strcasecmp (value_p, "centred")) {
                        printf("Setting screen centering to: centered.\n");
                        SetVideoCentered();
                } else if ( !strcasecmp (value_p, "top")) {
                        printf("Setting screen centering to: top.\n");
                        SetVideoTop();
                } else if ( !strcasecmp (value_p, "bottom")) {
                        printf("Setting screen centering to: bottom.\n");
                        SetVideoBottom();
                } else if ( !strcasecmp (value_p, "scaled")) {
                        printf("Setting screen centering to: scaled.\n");
                        SetVideoScaled();
                } else {
                        printf("Invalid vscale, please enter 'centered', 'centred', 'top', 'bottom' or 'scaled'.\n");
                }
        }

        /* Set orientation:
         */
        if ( !strcasecmp (name_p, "orientation")) {
                if ( !strcasecmp (value_p, "normal")) {
                        printf("Setting screen orientation to normal.\n");
			// This class is going (as it's useless anyway, so this will be cleaned up later.
                        if (mainWin!=NULL) mainWin->NormaliseScreenOrientation();
			EG_FlipNormalSideUp();
                } else if ( !strcasecmp (value_p, "flipped")) {
                        printf("Setting screen orientation to flipped.\n");
			// ditto
                        if (mainWin!=NULL) mainWin->ReverseScreenOrientation();
			EG_FlipUpsideDown();
                } else
                        printf("Invalid screen orientation, please enter either 'normal' or 'flipped'.\n");
        }

	/* VSync nudge:
	 */
	if ( !strcasecmp (name_p, "vsyncoffset")) {
		if ( !strcasecmp (value_p, "nudge")) {
			Video_SetVHoldDown();
		} else if ( !strcasecmp (value_p, "normal")) {
			Video_SetVHoldNormal();
		} else
			printf("Invalid vsyncoffset, please enter either 'normal' or 'nudge'.\n");
	}


	/* Use skin'd widgets (not saved in .kbd files):
	 */
	if ( !strcasecmp (name_p, "skin")) {
		if ( !strcasecmp (value_p, "yes"))
			config.settings.skin = 1;
		else if ( !strcasecmp (value_p, "no"))
			config.settings.skin = 0;
		else
			printf("Invalid skin mode, please enter either 'on' or 'off'.\n");
	}


        /* Disc R/W tab:
         */
        if ( !strcasecmp (name_p, "fd0_tab")) {
                if ( !strcasecmp (value_p, "ro")) {
                        printf("Setting DF0 R/W tab to Read only.\n");
                        DiscWriteEnable(0, 0);
                } else if ( !strcasecmp (value_p, "rw")) {
                        printf("Setting DF0 R/W tab to Read/Write.\n");
                        DiscWriteEnable(0, 1);
                } else
                        printf("'%s' '%s' Invalid DF0 read/write tab state, please enter either 'ro' or 'rw'.\n", name_p, value_p);
        }

        if ( !strcasecmp (name_p, "fd1_tab")) {
                if ( !strcasecmp (value_p, "ro")) {
                        printf("Setting DF1 R/W tab to Read only.\n");
                        DiscWriteEnable(1, 0);
                } else if ( !strcasecmp (value_p, "rw")) {
                        printf("Setting DF1 R/W tab to Read/Write.\n");
                        DiscWriteEnable(1, 1);
                } else
                        printf("Invalid DF1 read/write tab state, please enter either 'ro' or 'rw'.\n");
        }


	/* Stick mode:
  	 */
        if ( !strcasecmp (name_p, "stick")) {
                if ( !strcasecmp (value_p, "multi")) {
                        printf("Setting stick to MULTI mode.\n");
			StickManager_Clear();
			StickManager_SetMode(STICKMODE_MULTI);
                } else if ( !strcasecmp (value_p, "horizontal")) {
                        printf("Setting stick to HORIZONTAL mode.\n");
			StickManager_Clear();
			StickManager_SetMode(STICKMODE_HORIZONTAL);
                } else if ( !strcasecmp (value_p, "vertical")) {
                        printf("Setting stick to VERTICAL mode.\n");
			StickManager_Clear();
			StickManager_SetMode(STICKMODE_VERTICAL);
                } else if ( !strcasecmp (value_p, "smart")) {
                        printf("Setting stick to SMART mode.\n");
			StickManager_Clear();
			StickManager_SetMode(STICKMODE_CRITICAL);
                } else
                        printf("Invalid stick mode, please enter either 'smart', 'multi', 'horizontal' or 'vertical'.\n");
        }




	/* Default file locations and directories:
	 */
	if ( !strcasecmp (name_p, "gui_font_loc")) {
		if (value_p[0] != '/') {
			strcpy(config.files.font_loc, config.files.root_dir);
			strcat(config.files.font_loc, value_p);
		} else {
			strcpy(config.files.font_loc, value_p);
		}

		printf("Setting GUI_FONT_LOC to \"%s\"\n", config.files.font_loc);
	}

	/* Default teletext font location:
	 */
	if ( !strcasecmp (name_p, "teletext_font_loc")) {
		if (value_p[0] != '/') {
			strcpy(config.files.teletext_font_loc, config.files.root_dir);
			strcat(config.files.teletext_font_loc, value_p);
		} else {
			strcpy(config.files.teletext_font_loc, value_p);
		}

		printf("Setting TELETEXT_FONT_LOC to \"%s\"\n", config.files.teletext_font_loc);
	}

	/* Default owl locations and directories:
	 */
	if ( !strcasecmp (name_p, "gui_owl_loc")) {
		if (value_p[0] != '/') {
			strcpy(config.files.owl_loc, config.files.root_dir);
			strcat(config.files.owl_loc, value_p);
		} else {
			strcpy(config.files.owl_loc, value_p);
		}

		printf("Setting GUI_OWL_LOC to \"%s\"\n", config.files.owl_loc);
	}

	/* Default ROMs config file location:
	 */
	if ( !strcasecmp (name_p, "roms_config_loc")) {
		if (value_p[0] != '/') {
			strcpy(config.files.romimg_rc_loc, config.files.root_dir);
			strcat(config.files.romimg_rc_loc, value_p);
		} else {
			strcpy(config.files.romimg_rc_loc, value_p);
		}

		printf("Setting ROM_CONFIG_LOC to \"%s\"\n", config.files.romimg_rc_loc);
	}

	/* Default ROM image dir:
	 */
	if ( !strcasecmp (name_p, "roms_dir")) {
		if (value_p[0] != '/') {
			strcpy(config.files.romimg_dir, config.files.root_dir);
			strcat(config.files.romimg_dir, value_p);
		} else {
			strcpy(config.files.romimg_dir, value_p);
		}
		AppendForwardSlashIfNone(config.files.romimg_dir);

		printf("Setting ROMS_DIR to \"%s\"\n", config.files.romimg_dir);
	}

	/* Default config loc:
	 */
	if ( !strcasecmp (name_p, "default_config_loc")) {
		if (value_p[0] != '/') {
			strcpy(config.files.default_config_loc, config.files.root_dir);
			strcat(config.files.default_config_loc, value_p);
		} else {
			strcpy(config.files.default_config_loc, value_p);
		}

		printf("Setting DEFAULT_CONFIG_LOC to \"%s\"\n", config.files.default_config_loc);
	}

	/* Forced config loc:
	 */
	if ( !strcasecmp (name_p, "forced_config_loc")) {
		if (value_p[0] != '/') {
			strcpy(config.files.forced_config_loc, config.files.root_dir);
			strcat(config.files.forced_config_loc, value_p);
		} else {
			strcpy(config.files.forced_config_loc, value_p);
		}

		printf("Setting FORCED_CONFIG_LOC to \"%s\"\n", config.files.forced_config_loc);
	}

	/* Default disc images dir:
	 */
	if ( !strcasecmp (name_p, "disc_images_dir")) {
		if (value_p[0] != '/') {
			strcpy(config.files.disc_images_dir, config.files.root_dir);
			strcat(config.files.disc_images_dir, value_p);
		} else {
			strcpy(config.files.disc_images_dir, value_p);
		}
		AppendForwardSlashIfNone(config.files.disc_images_dir);

		printf("Setting DISC_IMAGES_DIR to \"%s\"\n", config.files.disc_images_dir);
	}

	/* Default save state dir:
	 */
	if ( !strcasecmp (name_p, "saves_dir")) {
		if (value_p[0] != '/') {
			strcpy(config.files.save_state_dir, config.files.root_dir);
			strcat(config.files.save_state_dir, value_p);
		} else {
			strcpy(config.files.save_state_dir, value_p);
		}
		AppendForwardSlashIfNone(config.files.save_state_dir);

		printf("Setting SAVES_DIR to \"%s\"\n", config.files.save_state_dir);
	}

	/* Keyboard mapping files dir:
	 */
	if ( !strcasecmp (name_p, "kbd_mappings_dir")) {
		if (value_p[0] != '/') {
			strcpy(config.files.kbd_files_dir, config.files.root_dir);
			strcat(config.files.kbd_files_dir, value_p);
		} else {
			strcpy(config.files.kbd_files_dir, value_p);
		}
		AppendForwardSlashIfNone(config.files.kbd_files_dir);

		printf("Setting KBD_MAPPINGS_DIR to \"%s\"\n", config.files.kbd_files_dir);
	}

	/* Virtual keyboard location:
	 */
	if ( !strcasecmp (name_p, "virtual_keyboard_loc") ) {
		if (value_p[0] != '/') {
			strcpy(config.files.virtual_keyboard_loc, config.files.root_dir);
			strcat(config.files.virtual_keyboard_loc, value_p);
		} else {
			strcpy(config.files.virtual_keyboard_loc, value_p);
		}

		printf("Setting VIRTUAL_KEYBOARD_LOC to \"%s\"\n", config.files.virtual_keyboard_loc);
	}

//	/* GUI font location:
//	 */
//	if ( !strcasecmp (name_p, "gui_font_loc") ) {
//		if (value_p[0] != '/') {
//			strcpy(config.files.gui_font_loc, config.files.root_dir);
//			strcat(config.files.gui_font_loc, value_p);
//		} else {
//			strcpy(config.files.gui_font_loc, value_p);
//		}
//
//		printf("Setting GUI_FONT_LOC to \"%s\"\n", config.files.gui_font_loc);
//	}


}

int ProcessSetOption(char *p)
{
        char *pp, *name_p, *value_p;
        int ret=0;

        if (p == NULL) return 0;

        if ( (pp=(char*)malloc(strlen(p)+1)) == NULL) {
                printf("*** Unable to process SET option. Not enough RAM.\n");
                return 0;
        }
        strcpy(pp, p);

        if (ParseSetOption(pp, &name_p, &value_p)) {
                ActionSetOption(name_p, value_p);
                ret=1;
        }

        free(pp);
        return ret;
}

const char* Config_GetDiscImageFilename(unsigned int drive)
{
	if (drive == 0) return config.drive_0_path;
	else return config.drive_1_path;
}

const char* Config_GetDiscImagePath(void)
{
	return (const char*) config.files.disc_images_dir;
}

const char* Config_GetSaveStatePath(void)
{
	return (const char*) config.files.save_state_dir;
}
