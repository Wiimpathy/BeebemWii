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
/* User VIA support file for the beeb emulator - David Alan Gilbert 11/12/94 */
/* Modified from the system via */

#include <iostream>

#include "6502core.h"
#include "uservia.h"
#include "via.h"

extern int DumpAfterEach;
/* My raw VIA state */
VIAState UserVIAState;

/*--------------------------------------------------------------------------*/
static void UpdateIFRTopBit(void) {
  /* Update top bit of IFR */
  if (UserVIAState.ifr&(UserVIAState.ier&0x7f))
    UserVIAState.ifr|=0x80;
  else
    UserVIAState.ifr&=0x7f;
  intStatus&=~(1<<userVia);
  intStatus|=((UserVIAState.ifr & 128)?(1<<userVia):0);
}; /* UpdateIFRTopBit */

/*--------------------------------------------------------------------------*/
/* Address is in the range 0-f - with the fe60 stripped out */
void UserVIAWrite(int Address, int Value) {
  /* std::cerr << "UserVIAWrite: Address=0x" << hex << Address << " Value=0x" << Value << dec << " at " << TotalCycles << "\n";
  DumpRegs(); */
  switch (Address) {
    case 0:
      UserVIAState.orb=Value & 0xff;
      if ((UserVIAState.ifr & 1) && ((UserVIAState.pcr & 2)==0)) {
        UserVIAState.ifr&=0xfe;
	UpdateIFRTopBit();
      };
      break;

    case 1:
      UserVIAState.ora=Value & 0xff;
      UserVIAState.ifr&=0xfc;
      UpdateIFRTopBit();
      break;

    case 2:
      UserVIAState.ddrb=Value & 0xff;
      break;

    case 3:
      UserVIAState.ddra=Value & 0xff;
      break;

    case 4:
    case 6:
      /*std::cerr << "UserVia Reg4/6 Timer1 lo Counter Write val=0x " << hex << Value << dec << " at " << TotalCycles << "\n"; */
      UserVIAState.timer1l&=0xff00;
      UserVIAState.timer1l|=(Value & 0xff);
      break;

    case 5:
      /*std::cerr << "UserVia Reg5 Timer1 hi Counter Write val=0x" << hex << Value << dec  << " at " << TotalCycles << "\n"; */
      UserVIAState.timer1l&=0xff;
      UserVIAState.timer1l|=(Value & 0xff)<<8;
      UserVIAState.timer1c=UserVIAState.timer1l * 2;
      UserVIAState.ifr &=0xbf; /* clear timer 1 ifr */
      UserVIAState.timer1hasshot=0;
      /* If PB7 toggling enabled, then lower PB7 now */
      if (UserVIAState.acr & 128) {
        UserVIAState.orb&=0x7f;
        UserVIAState.irb&=0x7f;
      };
      UpdateIFRTopBit();
      break;

    case 7:
      /*std::cerr << "UserVia Reg7 Timer1 hi latch Write val=0x" << hex << Value << dec  << " at " << TotalCycles << "\n"; */
      UserVIAState.timer1l&=0xff;
      UserVIAState.timer1l|=(Value & 0xff)<<8;
      break;

    case 8:
      /* std::cerr << "UserVia Reg8 Timer2 lo Counter Write val=0x" << hex << Value << dec << "\n"; */
      UserVIAState.timer2l&=0xff00;
      UserVIAState.timer2l|=(Value & 0xff);
      break;

    case 9:
      /* std::cerr << "UserVia Reg9 Timer2 hi Counter Write val=0x" << hex << Value << dec << "\n";
      core_dumpstate(); */
      UserVIAState.timer2l&=0xff;
      UserVIAState.timer2l|=(Value & 0xff)<<8;
      UserVIAState.timer2c=UserVIAState.timer2l * 2;
      UserVIAState.ifr &=0xdf; /* clear timer 2 ifr */
      UpdateIFRTopBit();
      break;

    case 10:
      break;

    case 11:
      UserVIAState.acr=Value & 0xff;
      break;

    case 12:
      UserVIAState.pcr=Value & 0xff;
      break;

    case 13:
      UserVIAState.ifr&=~(Value & 0xff);
      UpdateIFRTopBit();
      break;

    case 14:
      /* std::cerr << "User VIA Write ier Value=" << Value << "\n"; */
      if (Value & 0x80)
        UserVIAState.ier|=Value & 0xff;
      else
        UserVIAState.ier&=~(Value & 0xff);
      UserVIAState.ier&=0x7f;
      UpdateIFRTopBit();
      break;

    case 15:
      UserVIAState.ora=Value & 0xff;
      break;
  } /* Address switch */
} /* UserVIAWrite */

/*--------------------------------------------------------------------------*/
/* Address is in the range 0-f - with the fe60 stripped out */
int UserVIARead(int Address) {
  int tmp;
  /* std::cerr << "UserVIARead: Address=0x" << hex << Address << dec << " at " << TotalCycles << "\n";
  DumpRegs(); */
  switch (Address) {
    case 0: /* IRB read */
      tmp=(UserVIAState.orb & UserVIAState.ddrb) | (UserVIAState.irb & (~UserVIAState.ddrb));;
      return(tmp);

    case 2:
      return(UserVIAState.ddrb);

    case 3:
      return(UserVIAState.ddra);

    case 4: /* Timer 1 lo counter */
      tmp=UserVIAState.timer1c / 2;
      UserVIAState.ifr&=0xbf; /* Clear bit 6 - timer 1 */
      UpdateIFRTopBit();
          if (UserVIAState.timer1c<=(UserVIAState.timer1l*2)) 
      return(tmp & 0xff);
	  else
	  return(0xff);

    case 5: /* Timer 1 ho counter */
      tmp=UserVIAState.timer1c /512;
          if (UserVIAState.timer1c<=(UserVIAState.timer1l*2)) 
      return(tmp & 0xff);
	  else
	  return(0xff);

    case 6: /* Timer 1 lo latch */
      return(UserVIAState.timer1l & 0xff);

    case 7: /* Timer 1 ho latch */
      return((UserVIAState.timer1l / 256) & 0xff);

    case 8: /* Timer 2 lo counter */
      tmp=UserVIAState.timer2c / 2;
      UserVIAState.ifr&=0xdf; /* Clear bit 5 - timer 2 */
      UpdateIFRTopBit();
          if (UserVIAState.timer2c<=(UserVIAState.timer2l*2)) 
	      return(tmp & 0xff);
	  else
		  return(0xff);

    case 9: /* Timer 2 ho counter */
          if (UserVIAState.timer2c<=(UserVIAState.timer2l*2)) 
		return((UserVIAState.timer2c / 512) & 0xff);
	  else
		  return(0xff);

    case 13:
      UpdateIFRTopBit();
      return(UserVIAState.ifr);
      break;

    case 14:
      return(UserVIAState.ier | 0x80);

    case 1:
      UserVIAState.ifr&=0xfc;
      UpdateIFRTopBit();
    case 15:
      return(255);
      break;
  } /* Address switch */
  return(0xff);
} /* UserVIARead */

/*--------------------------------------------------------------------------*/
void UserVIATriggerCA1Int(void) {
  /* We should be concerned with active edges etc. */
  UserVIAState.ifr|=2; /* CA1 */
  UpdateIFRTopBit();
}; /* UserVIATriggerCA1Int */

/*--------------------------------------------------------------------------*/
void UserVIA_poll_real(void) {
  int tCycles;
  if (UserVIAState.timer1c<0) {
        tCycles=abs(UserVIAState.timer1c);
    /*std::cerr << "UserVIA timer1c\n"; */
    UserVIAState.timer1c=(UserVIAState.timer1l * 2)-(tCycles-4);
    if ((UserVIAState.timer1hasshot==0) || (UserVIAState.acr & 0x40)) {
      /*std::cerr << "UserVIA timer1c - int at " << TotalCycles << "\n"; */
      UserVIAState.ifr|=0x40; /* Timer 1 interrupt */
      UpdateIFRTopBit();
      if (UserVIAState.acr & 0x80) {
        UserVIAState.orb^=0x80; /* Toggle PB7 */
        UserVIAState.irb^=0x80; /* Toggle PB7 */
      };
    };
    UserVIAState.timer1hasshot=1;
  } /* timer1c underflow */

  if (UserVIAState.timer2c<0) {
        tCycles=abs(UserVIAState.timer2c);
    /* std::cerr << "UserVIA timer2c\n"; */
    UserVIAState.timer2c=(UserVIAState.timer2l * 2)-(tCycles-4);
    if (!(UserVIAState.ifr&=0x20)) {
     /* std::cerr << "UserVIA timer2c - int\n"; */
      UserVIAState.ifr|=0x20; /* Timer 2 interrupt */
      UpdateIFRTopBit();
    };
  } /* timer2c underflow */

} /* UserVIA_poll */

/*--------------------------------------------------------------------------*/
void UserVIAReset(void) {
  VIAReset(&UserVIAState);
} /* UserVIAReset */

/*-------------------------------------------------------------------------*/
//==void SaveUserVIAState(unsigned char *StateData) {
//## handled in via.cc now
//==	SaveVIAState(&UserVIAState, StateData);
//==}

/*-------------------------------------------------------------------------*/
//==void RestoreUserVIAState(unsigned char *StateData) {
//== handled in via.cc now
//==	RestoreVIAState(&UserVIAState, StateData);
//==}

/*--------------------------------------------------------------------------*/
void uservia_dumpstate(void) {
  std::cerr << "Uservia:\n";
  via_dumpstate(&UserVIAState);
}; /* uservia_dumpstate */
