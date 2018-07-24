/* UI messages are stored here (for the most part).
 */

#ifndef BEEBMESSAGES_HEADER
#define BEEBMESSAGES_HEADER

#define BEEBEM_PAGES_RESET_T "Reset BBC:"
#define BEEBEM_PAGES_RESET_M "Reset the emulated BBC?"


#define BEEBEM_PAGES_SAVECONFIGNODISC_T "Save config FAILED:"
#define BEEBEM_PAGES_SAVECONFIGNODISC_M "Could not save configuration as no disc is currently in drive 0!"

#define BEEBEM_PAGES_SAVECONFIGFAILED_T BEEBEM_PAGES_SAVECONFIGNODISC_T
#define BEEBEM_PAGES_SAVECONFIGFAILED_M "Could not save configuration! Was unable to save to disc..."

#define BEEBEM_PAGES_SAVECONFIGSUCCESS_T "Save config SUCCEEDED:"
#define BEEBEM_PAGES_SAVECONFIGSUCCESS_M "Successfully saved this disc images configuration to disc."


#define BEEBEM_PAGES_QUIT_T "Quit BeebEm:"
#define BEEBEM_PAGES_QUIT_M "Quit BeebEm and return to WII menu?"


#define BEEBEM_PAGES_LOADSTATEFAILED_T "Loading state FAILED:"
#define BEEBEM_PAGES_LOADSTATEFAILED_M "Unable to open state file on file-system. Loading state FAILED..."

#define BEEBEM_PAGES_STICKLISTFAILED_T "Failed to create stick list box:"
#define BEEBEM_PAGES_STICKLISTFAILED_M "Unable to create a list box to list stick mode options!\n\n(I'd quit BeebEm now, and restart it, if I were you!)"

#define DISC8271_LOADDISC_T "Disc image:"
#define DISC8271_LOADDISC_M "Could not open disc file: '%s'"

#define DISC8271_INCORRECTFORMAT_T "Disc image format:"
#define DISC8271_INCORRECTFORMAT_M "WARNING - Incorrect disc type selected?\n\nThis disc file looks like a double sided disc image. Check files before copying them."

#define DISC8271_INCORRECTFORMAT2_T DISC8271_INCORRECTFORMAT_T
#define DISC8271_INCORRECTFORMAT2_M "WARNING - Incorrect disc type selected?\n\nThis disc file looks like a single sided disc image. Check files before copying them."

#define DISC8271_SAVETRACK_T DISC8271_LOADDISC_T
#define DISC8271_SAVETRACK_M "Could not open disc image '%s' for writing."

#define DISC8271_SAVETRACK2_T DISC8271_LOADDISC_T
#define DISC8271_SAVETRACK2_M "Could not write to disc image '%s'."

#define DISC8271_INVALIDDISCCATALOGUE_T DISC8271_LOADDISC_T
#define DISC8271_INVALIDDISCCATALOGUE_M "Invalid Disc Catalogue.\n\nThis disc image will get corrupted if files are written to it. Copy all the files to a new image to fix it."

#define DISC8271_CREATEDISCIMAGECANCEL_T DISC8271_LOADDISC_T
#define DISC8271_CREATEDISCIMAGECANCEL_M "File '%s' already exists! Overwrite it?"

#define DISC8271_OVERWRITEDISCIMAGECANCELLED_T "Cancelled overwrite"
#define DISC8271_OVERWRITEDISCIMAGECANCELLED_M "Cancelled creation of disc image. Your old disc image was NOT overwritten!"

#define DISC8271_COULDNOTCREATEFILE_T DISC8271_LOADDISC_T
#define DISC8271_COULDNOTCREATEFILE_M "Could not create '%s'. Creation of disc image failed."

#define DISC8271_CREATEDFILE_T DISC8271_LOADDISC_T
#define DISC8271_CREATEDFILE_M "Blank disc image successfully created."

#define DISC8271_CREATEFILEFAILEDWRITING_T DISC8271_LOADDISC_T
#define DISC8271_CREATEFILEFAILEDWRITING_M "Failed writing to disc image '%s'."


#define CORE6502_ILLEGALINSTRUCTION_T "Illegal Instruction:"
#define CORE6502_ILLEGALINSTRUCTION_M "Unsupported 6502 instruction 0x%02X at 0x%04X\n\nSKIP = instruction will be skipped\n\nQuit = dump memory and exit"


#define BEEBCONFIG_IMAGEDOESNOTEXIST_T "Loading disc image failed:"
#define BEEBCONFIG_IMAGEDOESNOTEXIST_M "The supplied command line disc image filename does not exist on the filesystem.\n\nPlease check the filename is correct.\n\n(Remember GP2X filenames are case sensitive. So MyImage does not equal 'myimage'.)"

#define BEEBCONFIG_ILLEGALFILEEXTENSION_T BEEBCONFIG_IMAGEDOESNOTEXIST_T
#define BEEBCONFIG_ILLEGALFILEEXTENSION_M "The supplied command line disc image filename does not have an .ssd or .dsd file extension.\n\nOnly disc images ending in .ssd or .dsd images are compatible with this version of BeebEm."

#define BEEBCONFIG_NOMAPPINGFILE_T "No associated mapping file found:"
#define BEEBCONFIG_NOMAPPINGFILE_M "There does not seem to be a keyboard mapping file associated with this disc image.\n\nSo falling back to default values instead.\n\nIn order to play this game, you may need to reconfigure some of the Wii buttons manually.\n\nOnce you have mapped the correct keys, press the 'Save game configuration' button on the main menu to store them for next time."


#define BEEBMEM_READROMERROR_T "Read ROM error:"
#define BEEBMEM_READROMERROR_M "Cannot open ROM image file '%s'!"

#define BEEBMEM_WRONGROMIMAGESIZE_T BEEBMEM_READROMERROR_T
#define BEEBMEM_WRONGROMIMAGESIZE_M "ROM image '%s' is the wrong size!"

#define BEEBMEM_COULDNOTOPENROMIMAGE_T BEEBMEM_READROMERROR_T
#define BEEBMEM_COULDNOTOPENROMIMAGE_M "Could not open ROM image '%s'."

#define BEEBMEM_COULDNOTREADOSROM_T BEEBMEM_READROMERROR_T
#define BEEBMEM_COULDNOTREADOSROM_M "Could not open OS ROM image '%s'!"

#define BEEBMEM_OSROMWRONGSIZE_T BEEBMEM_READROMERROR_T
#define BEEBMEM_OSROMWRONGSIZE_M "The OS ROM is the wrong size!"


#define FILESELECTOR_UNABLETOOPENDIR_T "File selector:"
#define FILESELECTOR_UNABLETOOPENDIR_M "Unable to open a directory named:\n\n'%s'.\n\nPlease check the value of DISC_IMAGES_DIR.\n\n(It's probably in ./config/default.kbd)."

#define FILESELECTOR_UNABLETOOPENDIR2_T FILESELECTOR_UNABLETOOPENDIR_T
#define FILESELECTOR_UNABLETOOPENDIR2_M "Unable to open '%s' directory. Failed to create directory entry list."

#define FILESELECTOR_FILENAMETOOLONG_T "File selector:"
#define FILESELECTOR_FILENAMETOOLONG_M "The filename: \n\n'%s'\n\nis too long to display in the file selector!"


#define UEFSTATE_OVERWRITESTATEIMAGE_T "Save state:"
#define UEFSTATE_OVERWRITESTATEIMAGE_M "File '%s' already exists. Overwrite it?"

#define UEFSTATE_CANCELLEDOVERWRITE_T "Save state:"
#define UEFSTATE_CANCELLEDOVERWRITE_M "Cancelled file overwrite for save state. Your old state image was NOT overwritten!"

#define UEFSTATE_SAVESTATESUCCESSFUL_T "Saving state succeeded:"
#define UEFSTATE_SAVESTATESUCCESSFUL_M "Current state saved to disc OK (probably)!"

#define UEFSTATE_SAVESTATEFAILED_T "Saving state FAILED:"
#define UEFSTATE_SAVESTATEFAILED_M "Failed to write state to '%s'"

#define UEFSTATE_FILEFORMATINCORRECT_T "Load state:"
#define UEFSTATE_FILEFORMATINCORRECT_M "This file is not an UEF File!"

#define UEFSTATE_CANNOTOPENFILE_T "Load state:"
#define UEFSTATE_CANNOTOPENFILE_M "Cannot open state file '%s'"


#endif
