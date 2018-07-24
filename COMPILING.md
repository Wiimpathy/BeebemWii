## DEVKIT AND LIBRARIES

- DevkitPPC r32

- libogc 1.8.20 compiled from source + gettod_r tv_usec fix : https://github.com/devkitPro/libogc/pull/52/commits

- A custom SDL Wii with aesnd audio + SDL_SetVolume(AESND_SetVoiceVolume) : https://github.com/Wiimpathy/sdl-wii/tree/VideoAltOptions-AudioAesnd

- libaesnd from libogc 1.8.17. With latest libaesnd, it's crashing at startup.

## NOTES

Optimization flags -02 and others result in no sound?

The sound is acceptable now but I had a lot of issues with audio. The key was to use memcpy instead of SDL_MixAudio in the callback(?!).
The stock SDL wii with asnd gives bad results too.

The joystick mode used in some games (elite, Starship Command etc.) isn't tested nor implemented.

There's a workaround for 'Run new game' using a forceReset variable to call reset. Without this the sound from the previously loaded game goes on for a few seconds which is annoying.
Another related issue is the 'load state' that takes more time than it should. In need of a proper fix!

Some thread functions are disabled in progressdialog.cpp.

The 'Change stick mode' is not used. What was it? What did that improve and in which games?

Better video *256 resolution solution?
