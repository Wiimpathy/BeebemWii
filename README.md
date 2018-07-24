# Beebem Wii

Introduction
============

Beebem Wii is a port of Beebem gp2x(0.6-1), a BBC micro emulator.
It's a computer by Acorn Computers released in the 1980s.


LOADING GAMES
=============

If you load Beebem from the Homebrew channel as a standalone homebrew, the default softwares folder is /apps/beebem/discs. Copy your .ssd or .dsd games in this folder.

To load a program, open the main menu, chose 'Run new game', and select the file. It will try to start the file automatically.
By default, some Wii controllers buttons are mapped to BBC keys. You may need to reconfigure the buttons per game. Go to 'Configure game buttons',
change the keys and select 'Save game configuration'. The configuration file will be saved in /apps/beebem/kbd.
A virtual keyboard is also available in-game.

CONTROLS
========

```
 Wiimote:

 A      : Confirm
 B      : Back
 +      : Open virtual keyboard
 Home   : Open main menu

 Classic controller:

 A      : Confirm
 B      : Back
 +      : Open virtual keyboard
 Home   : Open main menu

 GameCube controller:

 A      : Confirm
 B      : Back
 R      : Open virtual keyboard
 Start  : Open main menu
```

GAME OPTIONS
============

Here are the available settings in the 'Configure game options' :

### Audio

- Audio latency : When enabled, the sound is better. It's particularly useful for music since it drops less samples, but it adds a delay and sound effects loses sync.

### Video

- Top/Centered/Bottom/Scaled : It's scaled by default, it's the only option to show the whole screen by removing some lines. The BBC is 320*256 and here we're at 320*240.

- Use 240p mode : Enable double strike mode for low resolution display.

- Enable Bilinear filter : Toggle filter between sharp or smooth pixels.

- Nudge CRT (useful for FireTrack/Castle Quest etc) : This shifts the display vertically.

### Floppy drives

- floppy 0/1 R/W : Enable floppies read/write access.

PLUGIN
======

Beebem can be used as a plugin in WiiFlow or any compatible plugins loader. You'll find the necessary files in WiiFlow/plugins.
By default, the plugin looks for the games in a /roms/BBC folder that you'll need to create. Edit bbc.ini file if you want a different path (romdir).

NOTES
======

One extra button (Wiimote -/GC Z) can also be configured, but is not accessible in the menu. You have to edit the mapping files (.kbd) manually. Change this P1_MINUS value per game or globally in /apps/beebem/config/default.kbd.

Another option you can edit is DISABLE_CURSOR. Change value to YES to hide the cursor(not tested!).

All credits go to the original beebem authors. Dedicated to 'the Wii scene is dead' people. It's 2018 and there are still updates for Genesis Plus GX, Snes9x GX, USB loader GX, WiiFlow Lite etc. And there's a new homebrew game called Newo Asteroids too.


