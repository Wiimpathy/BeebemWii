/****************************************************************************/
/*              Beebem - (c) David Alan Gilbert 1994                        */
/*              ------------------------------------                        */
/* This program may be distributed freely within the following restrictions:*/
/*                                                                          */
/* 1) You may not charge for this program or for any part of it.            */
/* 2) This copyright message must be distributed with all copies.           */
/* 3) This program must be distributed complete with source code.  Binary   */
/*    only distribution is not permitted.                                   */
/* 4) The author offers no warrenties, or guarentees etc. - you use it at   */
/*    your own risk.  If it messes something up or destroys your computer   */
/*    thats YOUR problem.                                                   */
/* 5) You may use small sections of code from this program in your own      */
/*    applications - but you must acknowledge its use.  If you plan to use  */
/*    large sections then please ask the author.                            */
/*                                                                          */
/* If you do not agree with any of the above then please do not use this    */
/* program.                                                                 */
/* Please report any problems to the author at beebem@treblig.org           */
/****************************************************************************/
/* Beebemulator - memory subsystem - David Alan Gilbert 16/10/94 */
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

#include <iostream>

#include "6502core.h"
#include "disc8271.h"
#include "main.h"
#include "sysvia.h"
#include "uservia.h"
#include "video.h"
#include "atodconv.h"
#include "beebconfig_data.h"
#include "messagebox.h"

#include "uefstate.h"
#include "messages.h"

int WritableRoms = 0;

/* Each Rom now has a Ram/Rom flag */
int RomWritable[16] = {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1};

static int PagedRomReg;

static int RomModified=0; /* Rom changed - needs copying back */
static int SWRamModified=0; /* SW Ram changed - needs saving and restoring */
unsigned char WholeRam[65536];
static unsigned char Roms[16][16384];

/*----------------------------------------------------------------------------*/
/* Perform hardware address wrap around */
static unsigned int WrapAddr(int in) {
  unsigned int offsets[]={0x4000,0x6000,0x3000,0x5800};
  if (in<0x8000) return(in);
  in+=offsets[(IC32State & 0x30)>>4];
  in&=0x7fff;
  return(in);
}; /* WrapAddr */

/*----------------------------------------------------------------------------*/
/* This is for the use of the video routines.  It returns a pointer to
   a continuous area of 'n' bytes containing the contents of the
   'n' bytes of beeb memory starting at address 'a', with wrap around
   at 0x8000.  Potentially this routine may return a pointer into  a static
   buffer - so use the contents before recalling it
   'n' must be less than 1K in length.
   See 'BeebMemPtrWithWrapMo7' for use in Mode 7 - its a special case.
*/
char *BeebMemPtrWithWrap(int a, int n) {
  static char tmpBuf[1024];
  char *tmpBufPtr;
  int EndAddr=a+n-1;
  int toCopy;

  a=WrapAddr(a);
  EndAddr=WrapAddr(EndAddr);

  if (a<=EndAddr) {
    return((char *)WholeRam+a);
  };

  toCopy=0x8000-a;
  if (toCopy>n) toCopy=n;
  if (toCopy>0) memcpy(tmpBuf,WholeRam+a,toCopy);
  tmpBufPtr=tmpBuf+toCopy;
  toCopy=n-toCopy;
  if (toCopy>0) memcpy(tmpBufPtr,WholeRam+EndAddr-(toCopy-1),toCopy); /* Should that -1 be there ? */

  return(tmpBuf);
}; /* BeebMemPtrWithWrap */

/*----------------------------------------------------------------------------*/
/* Perform hardware address wrap around - for mode 7*/
static unsigned int WrapAddrMo7(int in) {
  if (in<0x8000) return(in);
  in+=0x7c00;
  in&=0x7fff;
  return(in);
}; /* WrapAddrMo7 */

/*----------------------------------------------------------------------------*/
/* Special case of BeebMemPtrWithWrap for use in mode 7
*/
char *BeebMemPtrWithWrapMo7(int a, int n) {
  static char tmpBuf[1024];
  char *tmpBufPtr;
  int EndAddr=a+n-1;
  int toCopy;

  a=WrapAddrMo7(a);
  EndAddr=WrapAddrMo7(EndAddr);

  if (a<=EndAddr) {
    return((char *)WholeRam+a);
  };

  toCopy=0x8000-a;
  if (toCopy>n) return((char *)WholeRam+a);
  if (toCopy>0) memcpy(tmpBuf,WholeRam+a,toCopy);
  tmpBufPtr=tmpBuf+toCopy;
  toCopy=n-toCopy;
  if (toCopy>0) memcpy(tmpBufPtr,WholeRam+EndAddr-(toCopy-1),toCopy); /* Should that -1 be there ? */

  return(tmpBuf);
}; /* BeebMemPtrWithWrapMo7 */

/*----------------------------------------------------------------------------*/
int BeebReadMem(int Address) {
  static int extracycleprompt=0;

 /* We now presume that the caller has checked to see if the address is below fc00
    and if so does a direct read */ 
 /* if (Address<0xfc00) return(WholeRam[Address]); */
  if (Address>=0xff00) return(WholeRam[Address]);

  Cycles++;
  extracycleprompt++;
  if (extracycleprompt & 8) Cycles++;
  /* OK - its IO space - lets check some system devices */
  /* VIA's first - games seem to do really heavy reaing of these */
  /* Can read from a via using either of the two 16 bytes blocks */
  if ((Address & ~0xf)==0xfe40 || (Address & ~0xf)==0xfe50) return(SysVIARead(Address & 0xf));
  if ((Address & ~0xf)==0xfe60 || (Address & ~0xf)==0xfe70) return(UserVIARead(Address & 0xf));
  if ((Address & ~7)==0xfe00) return(CRTCRead(Address & 0x7));
  if ((Address & ~0xf)==0xfe20) return(VideoULARead(Address & 0xf));
  if ((Address & ~0x1f)==0xfe80) return(Disc8271_read(Address & 0x7));
  if ((Address & ~0x1f)==0xfea0) return(0xfe); /* Disable econet */
  if ((Address & ~0x1f)==0xfec0) return(AtoDRead(Address & 0xf));
  if ((Address & ~0x1f)==0xfee0) return(0xfe); /* Disable tube */
  return(0);
} /* BeebReadMem */

/*----------------------------------------------------------------------------*/
static void DoRomChange(int NewBank) {
  /* Speed up hack - if we are switching to the same rom, then don't bother */
  if (NewBank==PagedRomReg) return;
     
  if (RomModified) {
    memcpy(Roms[PagedRomReg],WholeRam+0x8000,0x4000);
    RomModified=0;
  };

  PagedRomReg=NewBank;
  memcpy(WholeRam+0x8000,Roms[PagedRomReg],0x4000);
}; /* DoRomChange */

/*----------------------------------------------------------------------------*/
void BeebWriteMem(int Address, int Value) {
  static int extracycleprompt=0;
/*  fprintf(stderr,"Write %x to 0x%x\n",Value,Address); */

  /* Now we presume that the caller has validated the address as beingwithin
  main ram and hence the following line is not required */
  if (Address<0x8000) {
    WholeRam[Address]=Value;
    return;
  }

  if (Address<0xc000) {
    if (WritableRoms || RomWritable[PagedRomReg]) {
      WholeRam[Address]=Value;
      RomModified=1;
      if (RomWritable[PagedRomReg])
        SWRamModified=1;
    }
    return;
  }

  Cycles++;
  extracycleprompt++;
  if (extracycleprompt & 8) Cycles++;

  if ((Address>=0xfc00) && (Address<=0xfeff)) {
    /* Check for some hardware */
    if ((Address & ~0xf)==0xfe20) {
      VideoULAWrite(Address & 0xf, Value);
      return;
    }

    /* Can write to a via using either of the two 16 bytes blocks */
    if ((Address & ~0xf)==0xfe40 || (Address & ~0xf)==0xfe50) {
      SysVIAWrite((Address & 0xf),Value);
      return;
    }

    /* Can write to a via using either of the two 16 bytes blocks */
    if ((Address & ~0xf)==0xfe60 || (Address & ~0xf)==0xfe70) {
      UserVIAWrite((Address & 0xf),Value);
      return;
    }

    if (Address==0xfe30) {
      DoRomChange(Value & 0xf);
      return;
    }

    /*std::cerr << "Write *0x" << hex << Address << "=0x" << Value << dec << "\n"; */
    if ((Address & ~0x7)==0xfe00) {
      CRTCWrite(Address & 0x7, Value);
      return;
    }

    /* Should get the serial system, now that the CRTC has been extracted */
//    if ((Address & ~0x1f)==0xfe00) {
//      std::cerr << "Write *0x" << hex << Address << "=0x" << Value << dec << "\n";
//    };

    if ((Address & ~0x1f)==0xfe80) {
      Disc8271_write((Address & 7),Value);
      return;
    }

    if ((Address & ~0x1f)==0xfec0) {
      AtoDWrite((Address & 0xf),Value);
      return;
    }

    if (Address==0xfc01) exit(0);
    return;
  }
}

/*----------------------------------------------------------------------------*/
static void ReadRom(char *name,int bank) {
  FILE *InFile;
  char fullname[256];
  int romsize;

/*  sprintf(fullname,"/var/lib/beebem/romimg/%s",name); */
//==sprintf(fullname,"../romimg/%s",name);

	//sprintf(fullname, "%s/%s",ROMS_DIR, name);
	sprintf(fullname, "%s%s", config.files.romimg_dir, name);

	printf("fullname = [%s]\n", fullname);

  if (InFile=fopen(fullname,"rb"),InFile==NULL) {
//#ifdef WIN32
    char errstr[200];
	sprintf(errstr, BEEBMEM_READROMERROR_M, name);
//    MessageBox(NULL,errstr,"BBC Emulator",MB_OK|MB_ICONERROR);
//	exit(1);
//#else
    fprintf(stderr,"Could not open %s rom file\n",name);

	EG_MessageBox(frame_buffer_p, EG_MESSAGEBOX_STOP, BEEBMEM_READROMERROR_T, errstr, "Exit BeebEm", NULL, NULL, NULL, 0);

    abort();
//#endif
  }

  romsize = fread(Roms[bank],1,16384,InFile);
  if (romsize!=16384 && romsize!=8192) {
//#ifdef WIN32
    char errstr[200];
	sprintf(errstr, BEEBMEM_WRONGROMIMAGESIZE_M, name);
//    MessageBox(NULL,errstr,"BBC Emulator",MB_OK|MB_ICONERROR);
//	exit(1);

	EG_MessageBox(frame_buffer_p, EG_MESSAGEBOX_STOP, BEEBMEM_WRONGROMIMAGESIZE_T, errstr, "Exit BeebEm", NULL, NULL, NULL, 0);

//#else
    fprintf(stderr,"Could not read %s\n",name);
    abort();
//#endif
  }

  fclose(InFile);

  /* Write Protect the ROMS read in at startup */
  RomWritable[bank] = 0;

} /* ReadRom */

/*----------------------------------------------------------------------------*/
char *ReadRomTitle( int bank, char *Title, int BufSize )
{
	int i;

	// Copy the ROM Title to the Buffer
	for( i=0; (( i<(BufSize-1)) && ( Roms[bank][i+9]>30)); i++ )
		Title[i] = Roms[bank][i+9];

	// Add terminating NULL.
	Title[i] = '\0';

	return Title;
}

/*----------------------------------------------------------------------------*/
void BeebMemInit(void) {
  FILE *InFile;
  char fullname[256]="";

//#ifndef WIN32
  char buf[224], col1[224], col2[224];
  int col3;

//  if ((InFile = fopen("/var/lib/beebem/romimg/romimg.rc", "r")) == NULL) {
//==    if ((InFile = fopen("../romimg.rc", "r")) == NULL) {
//	if ((InFile = fopen(RC_DIR"/romimg.rc", "r")) == NULL) {

	printf("[%s]\n", config.files.romimg_rc_loc);

	if ((InFile = fopen(config.files.romimg_rc_loc, "r")) == NULL) {  

  fprintf(stderr, "Could not open romimg.rc file\n");

	char errstr[256];
        sprintf(errstr, BEEBMEM_COULDNOTOPENROMIMAGE_M, config.files.romimg_rc_loc);
	EG_MessageBox(frame_buffer_p, EG_MESSAGEBOX_STOP, BEEBMEM_COULDNOTOPENROMIMAGE_T, errstr, "Exit BeebEm", NULL, NULL, NULL, 0);

    abort();
  }

  while (fgets(buf, 224, InFile) != NULL) {
    sscanf(buf, "%s\t%s\t%i", col1, col2, &col3);

	printf("<%s><%s>\n",col1, col2);


    if (strcmp(col1, "OS") == 0) {

		printf("found os: \n");

//      strcpy(fullname, "/var/lib/beebem/romimg/");
//==	strcpy(fullname, "../romimg/");
	//strcpy(fullname,OS_DIR);
	strcpy(fullname, config.files.romimg_dir);
      strcat(fullname, col2);


    }
		
    else if (strcmp(col1, "ROM") == 0)
      ReadRom(col2, col3);		
  }
  
  fclose(InFile);
 

 
printf("[%s]\n", fullname);
  if (InFile=fopen(fullname,"rb"),InFile==NULL) {
    fprintf(stderr,"Could not open OS rom file\n");

        char errstr[256];
        sprintf(errstr, BEEBMEM_COULDNOTREADOSROM_M, fullname);
        EG_MessageBox(frame_buffer_p, EG_MESSAGEBOX_STOP, BEEBMEM_COULDNOTREADOSROM_T, errstr, "Exit BeebEm", NULL, NULL, NULL, 0);

    abort();
  }

  if (fread(WholeRam+0xc000,1,16384,InFile)!=16384) {
    fprintf(stderr,"Could not read OS\n");

        EG_MessageBox(frame_buffer_p, EG_MESSAGEBOX_STOP, BEEBMEM_OSROMWRONGSIZE_T, BEEBMEM_OSROMWRONGSIZE_M, "Exit BeebEm", NULL, NULL, NULL, 0);

    abort();
  }
  fclose(InFile);

//#else
//  /* Read all ROM files in the beebfile directory */
//  HANDLE handle;
//  WIN32_FIND_DATA filedata;
//  int finished = FALSE;
//  int romslot = 0xf;
//
//  handle = FindFirstFile("beebfile/*.*",&filedata);
//  if (handle != INVALID_HANDLE_VALUE)
//  {
//    while (romslot >= 0 && !finished)
//    {
//      if (!(filedata.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) &&
//          strcmp(filedata.cFileName,"OS12")!=0 && strcmp(filedata.cFileName,"os12")!=0)
//      {
//        ReadRom(filedata.cFileName,romslot);
//	    romslot--;
//      }
//      finished = !FindNextFile(handle,&filedata);
//    }
//    FindClose(handle);
//  }
//  if (romslot == 0xf)
//  {
//    MessageBox(NULL,"There are no ROMs in the 'beebfile' directory",
//                 "BBC Emulator",MB_OK|MB_ICONERROR);
//    exit(1);
//  }
//
//  /* Load OS */
//  strcpy(fullname, "beebfile/os12");
//  if (InFile=fopen(fullname,"rb"),InFile==NULL) {
//    char errstr[200];
//	sprintf(errstr, "Cannot open OS image file:\n  %s", fullname);
//    MessageBox(NULL,errstr,"BBC Emulator",MB_OK|MB_ICONERROR);
//	exit(1);
//  }
//
//  if (fread(WholeRam+0xc000,1,16384,InFile)!=16384) {
//    char errstr[200];
//	sprintf(errstr, "OS image is wrong size:\n  %s", fullname);
//    MessageBox(NULL,errstr,"BBC Emulator",MB_OK|MB_ICONERROR);
//	exit(1);
//  }
//  fclose(InFile);
//
//#endif

  /* Put first ROM in */
  memcpy(WholeRam+0x8000,Roms[0xf],0x4000);
  PagedRomReg=0xf;
  RomModified=0;
} /* BeebMemInit */

/*-------------------------------------------------------------------------*/
//==void SaveMemState(unsigned char *RomState,
//==				  unsigned char *MemState,
//==				  unsigned char *SWRamState)
//=={
//==	memcpy(MemState, WholeRam, 32768);
//==
//==	/* Save SW RAM state if it is selected and it has been modified */
//==	if (SWRamModified && RomWritable[PagedRomReg])
//==	{
//==		RomState[0] = 1;
//==		memcpy(SWRamState, WholeRam+0x8000, 16384);
//==	}
//==}

/*-------------------------------------------------------------------------*/
void SaveMemUEF(FILE *SUEF) {
        unsigned char RAMCount;


//+>
	// If Sideways Ram has changed, sync it before writing the roms...
	if (RomModified)
		memcpy(Roms[PagedRomReg],WholeRam+0x8000,0x4000);
//<+



//==        switch (MachineType) {
//==        case 0:
//==        case 3:
                fput16(0x0461,SUEF); // Memory Control State
                fput32(2,SUEF);
                fputc(PagedRomReg,SUEF);



//==                fputc(ACCCON,SUEF);
		fputc(0, SUEF); // Don't need ACCess CONtrol register (master).
	
		//if (SWRamModified)
		//	fputc(1, SUEF);
		//else
		//	fputc(0, SUEF);






//==                break;
//==
//==        case 1:
//==                fput16(0x0461,SUEF); // Memory Control State
//==                fput32(3,SUEF);
//==                fputc(PagedRomReg|(MemSel<<7)|(PrvEn<<6),SUEF);
//==                fputc((ShEn<<7)|(Prvs8<<6)|(Prvs4<<5)|(Prvs1<<4),SUEF);
//==                fputc(HidAdd,SUEF);
//==                break;
//==
//==        case 2:
//==                fput16(0x0461,SUEF); // Memory Control State
//==                fput32(2,SUEF);
//==                fputc(PagedRomReg|(MemSel<<7),SUEF);
//==                fputc((Sh_Display<<7),SUEF);
//==                break;
//==        }

        fput16(0x0462,SUEF); // Main Memory
        fput32(32768,SUEF);
        fwrite(WholeRam,1,32768,SUEF);

//==        switch (MachineType) {
//==        case 1:
//==                fput16(0x0463,SUEF); // Shadow RAM
//==                fput32(20480,SUEF);
//==                fwrite(ShadowRam,1,20480,SUEF);
//==                fput16(0x0464,SUEF); // Private RAM
//==                fput32(12288,SUEF);
//==                fwrite(Private,1,12288,SUEF);
//==                fput16(0x046D,SUEF); // IntegraB Hidden RAM
//==                fput32(256,SUEF);
//==                fwrite(Hidden,1,256,SUEF);
//==                break;
//==
//==        case 2:
//==                fput16(0x0463,SUEF); // Shadow RAM
//==                fput32(32768,SUEF);
//==                fwrite(ShadowRAM,1,32768,SUEF);
//==                fput16(0x0464,SUEF); // Private RAM
//==                fput32(12288,SUEF);
//==                fwrite(Private,1,12288,SUEF);
//==                break;
//==
//==        case 3:
//==                fput16(0x0463,SUEF); // Shadow RAM
//==                fput32(32770,SUEF);
//==                fput16(0,SUEF);
//==                fwrite(ShadowRAM,1,32768,SUEF);
//==                fput16(0x0464,SUEF); // Private RAM
//==                fput32(4096,SUEF);
//==                fwrite(PrivateRAM,1,4096,SUEF);
//==                fput16(0x0465,SUEF); // Filing System RAM
//==                fput32(8192,SUEF);
//==                fwrite(FSRam,1,8192,SUEF);
//==                break;
//==        }
        for (RAMCount=0;RAMCount<16;RAMCount++) {
                if (RomWritable[RAMCount]) {
			printf("Saving Sideways ram bank %d\n", RAMCount);
                        fput16(0x0466,SUEF); // ROM bank
                        fput32(16385,SUEF);
                        fputc(RAMCount,SUEF);
                        fwrite(Roms[RAMCount],1,16384,SUEF);
                }
        }
}

void LoadRomRegsUEF(FILE *SUEF) {
	int ACCCON; // throw away this value, should be zero as for master. (see Save above).

        PagedRomReg=fgetc(SUEF);

//## Don't need this (I think)...
//==        ROMSEL=PagedRomReg & 0xf;

//## Dump this value, is for master.
        ACCCON=fgetc(SUEF);

	//////////////////////////////////////////////////////////////////////////
	//SWRamModified = ACCCON;
	//////////////////////////////////////////////////////////////////////////


//==        switch (MachineType) {
//==        case 1:
//==                MemSel=(PagedRomReg >> 7) & 1;
//==                PrvEn=(PagedRomReg >> 6) & 1;
//==                PagedRomReg&=0xf;
//==                ShEn=(ACCCON>>7) & 1;
//==                Prvs8=(ACCCON>>6) & 1;
//==                Prvs4=(ACCCON>>5) & 1;
//==                Prvs1=(ACCCON>>4) & 1;
//==                HidAdd=fgetc(SUEF);
//==                break;
//==
//==        case 2:
//==                MemSel=(PagedRomReg >> 7) & 1;
//==                PagedRomReg&=0xf;
//==                Sh_Display=(ACCCON>>7) & 1;
//==                break;
//==
//==        case 3:
//==                PRAM=PagedRomReg & 128;
//==                Sh_Display=ACCCON & 1;
//==                Sh_CPUX=ACCCON & 4;
//==                Sh_CPUE=ACCCON & 2;
//==                FRAM=ACCCON & 8;
//==                break;
//==        }
}

void LoadMainMemUEF(FILE *SUEF) {
        fread(WholeRam,1,32768,SUEF);
}

//==void LoadShadMemUEF(FILE *SUEF) {
//==        int SAddr;
//==        switch (MachineType) {
//==        case 1:
//==                fread(ShadowRam,1,20480,SUEF);
//==                break;
//==        case 2:
//==                fread(ShadowRAM,1,32768,SUEF);
//==                break;
//==        case 3:
//==                SAddr=fget16(SUEF);
//==                fread(ShadowRAM+SAddr,1,32768,SUEF);
//==                break;
//==        }
//==}

//==void LoadPrivMemUEF(FILE *SUEF) {
//==        switch (MachineType) {
//==        case 1:
//==                fread(Private,1,12288,SUEF);
//==                break;
//==        case 2:
//==                fread(Private,1,12288,SUEF);
//==                break;
//==        case 3:
//==                fread(PrivateRAM,1,4096,SUEF);
//==                break;
//==        }
//==}

// Is master specific.
//==void LoadFileMemUEF(FILE *SUEF) {
//==        fread(FSRam,1,8192,SUEF);
//==}

//==void LoadIntegraBHiddenMemUEF(FILE *SUEF) {
//==        fread(Hidden,1,256,SUEF);
//==}

void LoadSWRMMemUEF(FILE *SUEF) {
        int Rom;
        Rom=fgetc(SUEF);
        RomWritable[Rom]=1;
        fread(Roms[Rom],1,16384,SUEF);

//+>
	// Load paged ROM in main mem.
	if (PagedRomReg == Rom) {
		printf("Changing rom to %d\n", Rom);
		memcpy(WholeRam+0x8000,Roms[Rom],0x4000);
	}
//<+

	printf("Loading Sideways ram bank (%d) %d\n", PagedRomReg, Rom);
}



/*-------------------------------------------------------------------------*/
//==void RestoreMemState(unsigned char *RomState,
//==					 unsigned char *MemState,
//==					 unsigned char *SWRamState)
//=={
//==	memcpy(WholeRam, MemState, 32768);
//==
//==	/* Restore SW RAM state if it is in use */
//==	if (RomState[0] == 1)
//==	{
//==		RomModified = 1;
//==		SWRamModified = 1;
//==		PagedRomReg = 0;      /* Use rom slot 0 */
//==		RomWritable[0] = 1;
//==		memcpy(WholeRam+0x8000, SWRamState, 16384);
//==	}
//==}

/*-------------------------------------------------------------------------*/
/* dump the contents of mainram into 2 16 K files */
void beebmem_dumpstate(void) {
  FILE *bottom,*top;

  bottom=fopen("memdump_bottom","wb");
  top=fopen("memdump_top","wb");
  if ((bottom==NULL) || (top==NULL)) {
    std::cerr << "Couldn't open memory dump files\n";
    return;
  };

  fwrite(WholeRam,1,16384,bottom);
  fwrite(WholeRam+16384,1,16384,top);
  fclose(bottom);
  fclose(top);
}; /* beebmem_dumpstate */
