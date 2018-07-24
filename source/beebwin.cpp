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
/* The window which the beeb emulator displays stuff in */

#include <string.h>
#include <stdio.h>

#include "6502core.h"
#include "beebwin.h"
#include "beebmem.h"
#include "beebsound.h"
#include "disc8271.h"
#include "sysvia.h"
#include "uservia.h"
#include "video.h"

#include "beebconfig.h"
#include "beebconfig_data.h"

#include "virtualkeyboard.h"

extern unsigned int FrameCount;
extern int FastTable_Valid;

char* getScreenBufRow_p(unsigned int y){
	return (char*) frame_buffer_p->pixels+frame_buffer_p->pitch*y;
}


BeebWin::BeebWin() {
	Frame=0;
	ShiftBooted = false;

	onscreen_keyboard = 0;
	screen_orientation = 0;

	tape_led_state = caps_led_state = shift_led_state = 0;

}; /* BeebWin */

BeebWin::~BeebWin() {
}; /* Destructor */

void BeebWin::FindPaletteColors(void) {
	cols[0]=SDL_MapRGB(frame_buffer_p->format, 0x00, 0x00, 0x00);
	cols[1]=SDL_MapRGB(frame_buffer_p->format, 0xff, 0x00, 0x00);
	cols[2]=SDL_MapRGB(frame_buffer_p->format, 0x00, 0xff, 0x00);
	cols[3]=SDL_MapRGB(frame_buffer_p->format, 0xff, 0xff, 0x00); 
	cols[4]=SDL_MapRGB(frame_buffer_p->format, 0x00, 0x00, 0xff);
	cols[5]=SDL_MapRGB(frame_buffer_p->format, 0xff, 0x00, 0xff);
	cols[6]=SDL_MapRGB(frame_buffer_p->format, 0x00, 0xff, 0xff);
	cols[7]=SDL_MapRGB(frame_buffer_p->format, 0xff, 0xff, 0xff);
}

unsigned int BeebWin::currentscanline(void){	
	return CurrentScanLine;
}

void BeebWin::SetCapsLED(int state)
{
	caps_led_state = state ? 1 : 0;
}

void BeebWin::SetShiftLED(int state)
{
	shift_led_state = state ? 1 : 0;
}

void BeebWin::SetTapeLED(int state)
{
	tape_led_state = state ? 1 : 0;
}

int BeebWin::GetCapsLED(void)
{
	return caps_led_state;
}

int BeebWin::GetShiftLED(void)
{
	return shift_led_state;
}

int BeebWin::GetTapeLED(void)
{
	return tape_led_state;
}

//void BeebWin::RepaintTapeLED(void)
//{
//	VirtualKeyboard_RepaintTapeLED(tape_led_state);
//}

//void BeebWin::RepaintCapsLED(void)
//{
//	VirtualKeyboard_RepaintCapsLED(caps_led_state);
//}

//void BeebWin::RepaintShiftLED(void)
//{
//	VirtualKeyboard_RepaintShiftLED(shift_led_state);
//}

//void BeebWin::RepaintLEDs(void)
//{
//	RepaintTapeLED();
//	RepaintCapsLED();
//	RepaintShiftLED();
//}

unsigned int BeebWin::currentframe(void){
//==	return Frame;
	return FrameCount;
}

// [TODO] Loose starty and nlines, always updates the whole framebuffer anyway.
void BeebWin::updateLines(int starty, int nlines) {
//## This is no longer used, video.cc determines when the simulated vertical
//## retrace happens.
//==	Frame = Frame +1;	

/* P. SDL uses one screen buffer for none double buffered displays, so
 * SDL_UpdateRect is not needed for the GP2X.
 */
#ifndef GP2X
	if ( ((mainWin->currentframe())&1)==0)
		SDL_UpdateRect(frame_buffer_p, 0, 0, 0, 0);
#endif
}; /* BeebWin::updateLines */

void BeebWin::Reset(void) {
	printf("Reset.\n");
        BeebMemInit();
        Init6502core();
        SysVIAReset();
#ifdef SOUNDSUPPORT
        SoundInit();
#endif
        UserVIAReset();
        VideoInit();
        Disc8271_reset();
}

int BeebWin::OnScreenKeyboardShown(void) {
	return onscreen_keyboard;
}

void BeebWin::HideOnScreenKeyboard(void) {
	onscreen_keyboard = 0;
	//FastTable_Valid=0;
}

void BeebWin::ShowOnScreenKeyboard(void) {
	onscreen_keyboard = 1;
	//FastTable_Valid=0;
}

int BeebWin::ScreenIsReversed(void) {
	return screen_orientation;
}

void BeebWin::ReverseScreenOrientation(void) {
	screen_orientation = 1;
	config.settings.orientation = REVERSED;	
	FastTable_Valid=0;
}

void BeebWin::NormaliseScreenOrientation(void) {
	screen_orientation = 0;
	config.settings.orientation = NORMAL;
	FastTable_Valid=0;
}



EightUChars *BeebWin::GetLinePtr(int y) {

  if (y<0) y=0;
  if (y>=240) y=240-1;

  // [TODO] Check what uses CurrentScanLine, why's it set here?
  CurrentScanLine=(unsigned int) y;
  return( (EightUChars *) getScreenBufRow_p(y) );
};

SixteenUChars *BeebWin::GetLinePtr16(int y) {

  if (y<0) y=0;
  if (y>=240) y=240-1;

  // [TODO] Check what uses CurrentScanLine, why's it set here?
  CurrentScanLine=(unsigned int) y;

  return( (SixteenUChars *) getScreenBufRow_p(y) );
};
