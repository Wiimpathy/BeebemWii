// UEF Game state code for BeebEm V1.38
// Note - This version will be the last of the V1.3 series.
// After that, BeebEm the V1.4 Series will commence.

// (C) June 2001 Richard Gellman

//==#if HAVE_CONFIG_H
//==#	include <config.h>
//==#endif

#include <stdio.h>
#include <iostream>
#include "6502core.h"
#include "beebmem.h"
#include "video.h"
#include "via.h"
#include "beebwin.h"
#include "main.h"
#include "beebsound.h"
#include "disc8271.h"
//==#include "disc1770.h"
//==#include "tube.h"
//==#include "serial.h"
#include "beebconfig.h"

#include "messagebox.h"
#include "progressdialog.h"
#include "messages.h"
#include "sysvia.h"

using namespace std;

FILE *UEFState;

void fput32(unsigned int word32,FILE *fileptr) {
	fputc(word32&255,fileptr);
	fputc((word32>>8)&255,fileptr);
	fputc((word32>>16)&255,fileptr);
	fputc((word32>>24)&255,fileptr);
}

void fput16(unsigned int word16,FILE *fileptr) {
	fputc(word16&255,fileptr);
	fputc((word16>>8)&255,fileptr);
}

unsigned int fget32(FILE *fileptr) {
	unsigned int tmpvar;
	tmpvar =fgetc(fileptr);
	tmpvar|=fgetc(fileptr)<<8;
	tmpvar|=fgetc(fileptr)<<16;
	tmpvar|=fgetc(fileptr)<<24;
	return(tmpvar);
}

unsigned int fget16(FILE *fileptr) {
	unsigned int tmpvar;
	tmpvar =fgetc(fileptr);
	tmpvar|=fgetc(fileptr)<<8;
	return(tmpvar);
}

void SaveEmuUEF(FILE *SUEF) {
        char EmuName[16]="...............";
        fput16(0x046C,SUEF);
        fput32(16,SUEF);
        // BeebEm Title Block
        strcpy(EmuName,"BeebEm-GP2X");
        EmuName[14]=0;
        EmuName[15]=5; // Version, 1.4
        fwrite(EmuName,16,1,SUEF);
        //
        fput16(0x046a,SUEF);
        fput32(16,SUEF);
        // Emulator Specifics
        // Note about this block: It should only be handled by beebem from uefstate.cpp if
        // the UEF has been determined to be from BeebEm (Block 046C)

//==        fputc(MachineType,SUEF);
	fputc(0,SUEF);

//==        fputc((NativeFDC)?0:1,SUEF);
	fputc(0, SUEF);

//==        fputc(TubeEnabled,SUEF);
	fputc(0, SUEF);

        fputc(0,SUEF); // Monitor type, reserved
        fputc(0,SUEF); // Speed Setting, reserved
        fput32(0,SUEF);
        fput32(0,SUEF);
        fput16(0,SUEF);
        fputc(0,SUEF);
}

void LoadEmuUEF(FILE *SUEF, int Version) {
	int MachineType, NativeFDC, TubeEnabled;

        MachineType=fgetc(SUEF);
//==        if (Version <= 8 && MachineType == 1)
//==                MachineType = 3;

        NativeFDC=(fgetc(SUEF)==0)?true:false;
        TubeEnabled=fgetc(SUEF);
//==        mainWin->ResetBeebSystem(MachineType,TubeEnabled,1);
//==        mainWin->UpdateModelType();

	// [TODO] Shouldn't there really be a function to reset...
        Init6502core();
        Disc8271_reset();


}

#define SAVE_STEP_INC (1.0 / 7.0)
#define SAVE_UPDATE_PD() {ProgressDialog_SetPosition(pd_p, step+=SAVE_STEP_INC); SDL_Delay(100);}
extern SDL_Surface *frame_buffer_p;
void SaveUEFState(char *StateName) {
	ProgressDialog *pd_p;
	float step = 0;


	/* If file exists prompt for over-write
	 */
	UEFState=fopen(StateName, "rb");
	if (UEFState != NULL) {
		fclose(UEFState);

		char errstr[200];
		sprintf(errstr, UEFSTATE_OVERWRITESTATEIMAGE_M, StateName);
		if (EG_MessageBox(frame_buffer_p, EG_MESSAGEBOX_QUESTION, UEFSTATE_OVERWRITESTATEIMAGE_T, errstr, "Yes", "No", NULL, NULL, 2) == 2) {
			 EG_MessageBox(frame_buffer_p, EG_MESSAGEBOX_INFORMATION, UEFSTATE_CANCELLEDOVERWRITE_T
			  , UEFSTATE_CANCELLEDOVERWRITE_M, "Continue", NULL, NULL, NULL, 0);
			return;
		}
	}



	UEFState=fopen(StateName,"wb");
	if (UEFState != NULL)
	{
		pd_p=ProgressDialog_Create(frame_buffer_p, "Saving current emulator state", 0);
		ProgressDialog_Show(pd_p);

		fprintf(UEFState,"UEF File!");
		fputc(0,UEFState); // UEF Header
		fputc(10,UEFState); fputc(0,UEFState); // Version
		SaveEmuUEF(UEFState); SAVE_UPDATE_PD();
		Save6502UEF(UEFState); SAVE_UPDATE_PD();
		SaveMemUEF(UEFState); SAVE_UPDATE_PD();
		SaveVideoUEF(UEFState); SAVE_UPDATE_PD();
		SaveVIAUEF(UEFState); SAVE_UPDATE_PD();
		//SaveSoundUEF(UEFState); SAVE_UPDATE_PD();
//==		if (MachineType!=3 && NativeFDC)
			Save8271UEF(UEFState); SAVE_UPDATE_PD();
//==		else
//==			Save1770UEF(UEFState);
//==		if (EnableTube) {
//==			SaveTubeUEF(UEFState);
//==			Save65C02UEF(UEFState);
//==			Save65C02MemUEF(UEFState);
//==		}
//==		SaveSerialUEF(UEFState);
		fclose(UEFState);
#ifndef GEKKO
		sync();
#endif

		SDL_Delay(500);
		ProgressDialog_Free(pd_p);

		EG_MessageBox(frame_buffer_p, EG_MESSAGEBOX_INFORMATION, UEFSTATE_SAVESTATESUCCESSFUL_T, UEFSTATE_SAVESTATESUCCESSFUL_M, "Continue", NULL, NULL, NULL, 0);

	}
	else
	{
		char errstr[256];
		sprintf(errstr, UEFSTATE_SAVESTATEFAILED_M, StateName);
//==		MessageBox(GETHWND,errstr,"BeebEm",MB_ICONERROR|MB_OK);
		cerr << "Failed to write state file: "; // << StateName << "\n";

		EG_MessageBox(frame_buffer_p, EG_MESSAGEBOX_STOP, UEFSTATE_SAVESTATEFAILED_T, errstr, "Continue", NULL, NULL, NULL, 0);
	}
}

void LoadUEFState(char *StateName) {
	char errmsg[256];
	char UEFId[10];
//--	int CompletionBits=0; // These bits should be filled in
//## Unused.
	long RPos=0,FLength,CPos;
	unsigned int Block,Length;
	int Version;

//	ProgressDialog *pd_p;

	strcpy(UEFId,"BlankFile");

	if (mainWin!=NULL) mainWin->Reset();

	printf("Loading UEF State...\n");

	//sync();
	UEFState=fopen(StateName,"rb");
	if (UEFState != NULL)
	{
//->		fseek(UEFState,NULL,SEEK_END);
//++
		fseek(UEFState,0,SEEK_END);
//<-
		FLength=ftell(UEFState);
		fseek(UEFState,0,SEEK_SET);  // Get File length for eof comparison.
		fread(UEFId,10,1,UEFState);
		if (strcmp(UEFId,"UEF File!")!=0) {
//==			MessageBox(GETHWND,"The file selected is not a UEF File.","BeebEm",MB_ICONERROR|MB_OK);

			EG_MessageBox(frame_buffer_p, EG_MESSAGEBOX_STOP, UEFSTATE_FILEFORMATINCORRECT_T, UEFSTATE_FILEFORMATINCORRECT_M, "Continue", NULL, NULL, NULL, 0);

			cerr << "The file selected is not a UEF File.\n";
			fclose(UEFState);
			return;
		}
		Version=fget16(UEFState);
//==		sprintf(errmsg,"UEF Version %x",Version);
//==		//MessageBox(GETHWND,errmsg,"BeebEm",MB_OK);
		printf("UEF Version %d\n", Version);
//		SDL_Delay(5000);

		RPos=ftell(UEFState);

//		pd_p=ProgressDialog_Create(frame_buffer_p, "Loading previously saved emulator state", -1);
//		ProgressDialog_Show(pd_p);

		while (ftell(UEFState)<FLength) {
			Block=fget16(UEFState);
			Length=fget32(UEFState);
			CPos=ftell(UEFState);
			sprintf(errmsg,"Block %04X - Length %d (%04X)",Block,Length,Length);
			//MessageBox(GETHWND,errmsg,"BeebEm",MB_ICONERROR|MB_OK);
			if (Block==0x046A) { printf("A\n"); LoadEmuUEF(UEFState,Version); }
			if (Block==0x0460) { printf("B\n"); Load6502UEF(UEFState); }
			if (Block==0x0461) { printf("C\n"); LoadRomRegsUEF(UEFState); }
			if (Block==0x0462) { printf("D\n"); LoadMainMemUEF(UEFState); }
			//if (Block==0x0463) LoadShadMemUEF(UEFState);
			//if (Block==0x0464) LoadPrivMemUEF(UEFState);
			//if (Block==0x0465) LoadFileMemUEF(UEFState);
			if (Block==0x0466) { printf("E\n"); LoadSWRMMemUEF(UEFState); }
			if (Block==0x0467) { printf("F\n"); LoadViaUEF(UEFState); }
			if (Block==0x0468) { printf("G\n"); LoadVideoUEF(UEFState); }
			//if (Block==0x046B) { printf("H\n"); LoadSoundUEF(UEFState); }
//==			if (Block==0x046D) LoadIntegraBHiddenMemUEF(UEFState);
			if (Block==0x046E) { printf("I\n"); Load8271UEF(UEFState); }
//==			if (Block==0x046F) Load1770UEF(UEFState,Version);
//==			if (Block==0x0470) LoadTubeUEF(UEFState);
//==			if (Block==0x0471) Load65C02UEF(UEFState);
//==			if (Block==0x0472) Load65C02MemUEF(UEFState);
//==			if (Block==0x0473) LoadSerialUEF(UEFState);
			fseek(UEFState,CPos+Length,SEEK_SET); // Skip unrecognised blocks (and over any gaps)
		}

		fclose(UEFState);

//		SDL_Delay(1500);
//              ProgressDialog_Free(pd_p);

//==		mainWin->SetRomMenu();
//==		mainWin->SetDiscWriteProtects();
	}
	else
	{
		char errstr[256];
		sprintf(errstr, UEFSTATE_CANNOTOPENFILE_M, StateName);
//==		MessageBox(GETHWND,errstr,"BeebEm",MB_ICONERROR|MB_OK);

		cerr << "Cannot open state file: " << StateName << "\n";

		EG_MessageBox(frame_buffer_p, EG_MESSAGEBOX_STOP, UEFSTATE_CANNOTOPENFILE_T, errstr, "Continue", NULL, NULL, NULL, 0);

	}



	/* Remap the keys:
	 */
	Config_LoadDiscConfig( GetDiscImagePath(0) );


	/* Make sure shift-booting is off:
  	 */
        mainWin->ShiftBooted = false;
        BeebKeyUp(0, 0);
}


