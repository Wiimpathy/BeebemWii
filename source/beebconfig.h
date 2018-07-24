#ifndef BEEBCONFIG_HEADER
#define BEEBCONFIG_HEADER

/* BeebEm configuration structures:
 *
 * All configurable details are stored here.
 */

#define MAX_DIR_LEN 256+1
//#define MAX_DIR_LEN 1024+1

void ClearXX(void);
void PrintXX(char*);

typedef struct {
	char	root_dir[MAX_DIR_LEN];
	char	font_loc[MAX_DIR_LEN];
	char	teletext_font_loc[MAX_DIR_LEN];
	char	owl_loc[MAX_DIR_LEN];
	char	romimg_rc_loc[MAX_DIR_LEN];
	char	romimg_dir[MAX_DIR_LEN];
	char	default_config_loc[MAX_DIR_LEN];
	char	forced_config_loc[MAX_DIR_LEN];
	char	disc_images_dir[MAX_DIR_LEN];
	char	save_state_dir[MAX_DIR_LEN];
	char	kbd_files_dir[MAX_DIR_LEN];
	char	virtual_keyboard_loc[MAX_DIR_LEN];


//	char	xx[1024*5];
} BeebConfig_Files;

enum Scaling {CENTER, TOP, BOTTOM, SCALED};
enum Orientation {NORMAL, REVERSED};
enum Protection8271 {RO_8271, RW_8271};
enum Stick{CRITICAL, MULTI, HORIZONTAL, VERTICAL};

/* Not all settings are read from here, but these values should
 * be set via routines that control the setting:
 */
typedef struct {
#ifdef GEKKO
	unsigned int	latency;
	int	        doublestrike;
	int		enable_filter;
#else
	unsigned int	speed;
#endif
	unsigned int 	volume;
	Scaling		vscale;
	Orientation	orientation;
	Protection8271	protection_8271_0;
	Protection8271	protection_8271_1;
	Stick		stick;
	int		disable_cursor;
	int		vsyncoffset;
	int		skin;
} BeebConfig_Settings;

typedef struct {
        int x, y, z;

} BeebConfig_Diagnostic;

/* Main configuration structure for an instance of BeebEm:
 */
typedef struct {
	BeebConfig_Files files;
	BeebConfig_Settings settings;
        BeebConfig_Diagnostic diagostic;

	char		drive_0_path[MAX_DIR_LEN];
	char		drive_1_path[MAX_DIR_LEN];
} BeebConfig;



/* Initialize/Free:
 */



void 	Config_Initialize(const char *executable_name_p);
void 	Config_FreeResources();
int 	Config_LoadDiscConfig(const char *filename_p);

const char* Config_GetDiscImageFilename(unsigned int drive);
const char* Config_GetDiscImagePath(void);
const char* Config_GetSaveStatePath(void);

/* SET options:
 */

void 	RemoveLeadingWhiteSpace(char *p);
void 	RemoveTrailingWhiteSpace(char *p);
int 	ParseSetOption(char *p, char **name_pp, char **value_pp);
void 	ActionSetOption(char *name_p, char *value_p);
int 	ProcessSetOption(char *p);


/* Accessor wrappers:
 */
#define Config_DisableCursor (config.settings.disable_cursor)

#endif
