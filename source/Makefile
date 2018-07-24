INCLUDES = -I. -I/usr/include/g++ `sdl-config --cflags`
DEPINCLUDES = $(INCLUDES) $(SDINCLUDES) -I/usr/include/linux -I/usr/include/g++

#CC = gcc -g -Wall -ffast-math -O3 -march=athlon-xp -fomit-frame-pointer -funroll-loops -fno-trapping-math -malign-double -mmmx -m3dnow

CC = gcc -pg
#-mcpu=arm920 -mtune=arm920t -msoft-float -ffast-math -falign-functions=32 -finline-functions -funroll-all-loops


#-fstrict-aliasing -mstructure-size-boundary=32 -fexpensive-optimizations -fweb -frename-registers -fomit-frame-pointer -falign-functions=32 -falign-loops -falign-labels -falign-jumps -finline -finline-functions -fno-builtin -fno-common -funroll-all-loops

#CFLAGS = -O6 $(INCLUDES) $(SDINCLUDES) -Wall -funroll-all-loops -DBEEB_DOTIME $(RESOURCES)
CFLAGS = $(INCLUDES) $(SDINCLUDES) -Wall -g -DBEEB_DOTIME $(RESOURCES) -DSOUNDSUPPORT
# -DEG_DEBUG -DSOUNDSUPPORT
# -DPRODUCTION -DSOUNDSUPPORT

#RESOURCES=-DROMS_DIR=\"/mnt/sd/beebem/\" -DRC_DIR=\"/mnt/sd/beebem/\" -DOS_DIR=\"/mnt/sd/beebem/\" -DDEFAULT_DISC=\"S:80:/mnt/sd/beebem/Zalaga.ssd\"
RESOURCES=-DROMS_DIR=\"../romimg/\" -DRC_DIR=\"../\" -DOS_DIR=\"../romimg/\" \
 -DDEFAULT_DISC=\"S:80:/home/max/test/share/beebem/media/discs/games.ssd\" -DVERSION=\"0.6\"

# -DSOUNDSUPPORT (from above CFLAGS)
LINKFLAGS = -lstdc++ `sdl-config --libs`
LIBS =
LIBFILES = gui/libeg.a

SRCS =  6502core.cc beebmem.cc beebwin.cc disc8271.cc main.cc mode7font.c\
	sysvia.cc uservia.cc via.cc video.cc beebsound.cc atodconv.cc sdl.cc \
	keyboard.cc beebconfig.cc beebem_pages.cc screensaver.c stick.cc \
	virtualkeyboard.cc button.c fileselector.cc progressdialog.cc \
	keyboardmapper.cc messagebox.cc uefstate.cc listbox.cc fileselector_saveas.cc

OBJS =	6502core.o beebmem.o beebwin.o disc8271.o main.o  mode7font.o\
	sysvia.o uservia.o via.o video.o beebsound.o atodconv.o sdl.o \
	keyboard.o beebconfig.o beebem_pages.o screensaver.o stick.o \
	virtualkeyboard.o button.o fileselector.o progressdialog.o \
	keyboardmapper.o messagebox.o uefstate.o listbox.o fileselector_saveas.o

.cc.o:
	$(CC) -c $(CFLAGS) $<

beebem: $(OBJS) $(LIBFILES)
	$(CC) -o beebem $(OBJS) $(LIBFILES) $(LIBS) $(LINKFLAGS)

gui/libeg.a:
	(cd gui; make)

depend:
	makedepend -- $(DEPINCLUDES) -- $(SRCS)

clean:
	rm -f $(OBJS) beebem

reallyclean:
#	(cd dagCL; make clean)
#	(cd dagXCL; make clean)
	make clean
	(cd gui; make clean)
#	rm makefile makefile.bak

# DO NOT DELETE THIS LINE -- make depend depends on it.

6502core.o: /usr/include/SDL/SDL.h /usr/include/SDL/SDL_main.h
6502core.o: /usr/include/SDL/SDL_stdinc.h /usr/include/SDL/SDL_config.h
6502core.o: /usr/include/SDL/SDL_platform.h /usr/include/sys/types.h
6502core.o: /usr/include/features.h /usr/include/sys/cdefs.h
6502core.o: /usr/include/bits/wordsize.h /usr/include/gnu/stubs.h
6502core.o: /usr/include/gnu/stubs-32.h /usr/include/bits/types.h
6502core.o: /usr/include/linux/stddef.h /usr/include/bits/typesizes.h
6502core.o: /usr/include/linux/time.h /usr/include/linux/types.h
6502core.o: /usr/include/linux/posix_types.h /usr/include/linux/stddef.h
6502core.o: /usr/include/asm/posix_types.h
6502core.o: /usr/include/asm-i386/posix_types.h /usr/include/asm/types.h
6502core.o: /usr/include/asm-i386/types.h /usr/include/endian.h
6502core.o: /usr/include/bits/endian.h /usr/include/sys/select.h
6502core.o: /usr/include/bits/select.h /usr/include/bits/sigset.h
6502core.o: /usr/include/bits/time.h /usr/include/sys/sysmacros.h
6502core.o: /usr/include/bits/pthreadtypes.h /usr/include/stdio.h
6502core.o: /usr/include/libio.h /usr/include/_G_config.h
6502core.o: /usr/include/wchar.h /usr/include/bits/wchar.h
6502core.o: /usr/include/gconv.h /usr/include/bits/stdio_lim.h
6502core.o: /usr/include/bits/sys_errlist.h /usr/include/stdlib.h
6502core.o: /usr/include/bits/waitflags.h /usr/include/bits/waitstatus.h
6502core.o: /usr/include/xlocale.h /usr/include/alloca.h
6502core.o: /usr/include/string.h /usr/include/strings.h
6502core.o: /usr/include/inttypes.h /usr/include/stdint.h
6502core.o: /usr/include/ctype.h /usr/include/iconv.h
6502core.o: /usr/include/SDL/begin_code.h /usr/include/SDL/close_code.h
6502core.o: /usr/include/SDL/SDL_audio.h /usr/include/SDL/SDL_error.h
6502core.o: /usr/include/SDL/SDL_endian.h /usr/include/SDL/SDL_mutex.h
6502core.o: /usr/include/SDL/SDL_thread.h /usr/include/SDL/SDL_rwops.h
6502core.o: /usr/include/SDL/SDL_cdrom.h /usr/include/SDL/SDL_cpuinfo.h
6502core.o: /usr/include/SDL/SDL_events.h /usr/include/SDL/SDL_active.h
6502core.o: /usr/include/SDL/SDL_keyboard.h /usr/include/SDL/SDL_keysym.h
6502core.o: /usr/include/SDL/SDL_mouse.h /usr/include/SDL/SDL_video.h
6502core.o: /usr/include/SDL/SDL_joystick.h /usr/include/SDL/SDL_quit.h
6502core.o: /usr/include/SDL/SDL_loadso.h /usr/include/SDL/SDL_timer.h
6502core.o: /usr/include/SDL/SDL_version.h 6502core.h port.h
6502core.o: /usr/include/linux/limits.h beebwin.h sdl.h beebmem.h beebsound.h
6502core.o: disc8271.h sysvia.h via.h uservia.h video.h atodconv.h
6502core.o: beebconfig.h messagebox.h ./gui/types.h ./gui/widget.h
6502core.o: ./gui/window.h ./gui/box.h ./gui/sdl.h ./gui/label.h
6502core.o: ./gui/button.h ./gui/widget_private.h uefstate.h messages.h
beebmem.o: /usr/include/ctype.h /usr/include/features.h
beebmem.o: /usr/include/sys/cdefs.h /usr/include/bits/wordsize.h
beebmem.o: /usr/include/gnu/stubs.h /usr/include/gnu/stubs-32.h
beebmem.o: /usr/include/bits/types.h /usr/include/linux/stddef.h
beebmem.o: /usr/include/bits/typesizes.h /usr/include/endian.h
beebmem.o: /usr/include/bits/endian.h /usr/include/xlocale.h
beebmem.o: /usr/include/stdio.h /usr/include/libio.h /usr/include/_G_config.h
beebmem.o: /usr/include/wchar.h /usr/include/bits/wchar.h
beebmem.o: /usr/include/gconv.h /usr/include/bits/stdio_lim.h
beebmem.o: /usr/include/bits/sys_errlist.h /usr/include/stdlib.h
beebmem.o: /usr/include/bits/waitflags.h /usr/include/bits/waitstatus.h
beebmem.o: /usr/include/sys/types.h /usr/include/linux/time.h
beebmem.o: /usr/include/linux/types.h /usr/include/linux/posix_types.h
beebmem.o: /usr/include/linux/stddef.h /usr/include/asm/posix_types.h
beebmem.o: /usr/include/asm-i386/posix_types.h /usr/include/asm/types.h
beebmem.o: /usr/include/asm-i386/types.h /usr/include/sys/select.h
beebmem.o: /usr/include/bits/select.h /usr/include/bits/sigset.h
beebmem.o: /usr/include/bits/time.h /usr/include/sys/sysmacros.h
beebmem.o: /usr/include/bits/pthreadtypes.h /usr/include/alloca.h 6502core.h
beebmem.o: port.h /usr/include/linux/limits.h beebwin.h /usr/include/string.h
beebmem.o: /usr/include/SDL/SDL.h /usr/include/SDL/SDL_main.h
beebmem.o: /usr/include/SDL/SDL_stdinc.h /usr/include/SDL/SDL_config.h
beebmem.o: /usr/include/SDL/SDL_platform.h /usr/include/strings.h
beebmem.o: /usr/include/inttypes.h /usr/include/stdint.h /usr/include/iconv.h
beebmem.o: /usr/include/SDL/begin_code.h /usr/include/SDL/close_code.h
beebmem.o: /usr/include/SDL/SDL_audio.h /usr/include/SDL/SDL_error.h
beebmem.o: /usr/include/SDL/SDL_endian.h /usr/include/SDL/SDL_mutex.h
beebmem.o: /usr/include/SDL/SDL_thread.h /usr/include/SDL/SDL_rwops.h
beebmem.o: /usr/include/SDL/SDL_cdrom.h /usr/include/SDL/SDL_cpuinfo.h
beebmem.o: /usr/include/SDL/SDL_events.h /usr/include/SDL/SDL_active.h
beebmem.o: /usr/include/SDL/SDL_keyboard.h /usr/include/SDL/SDL_keysym.h
beebmem.o: /usr/include/SDL/SDL_mouse.h /usr/include/SDL/SDL_video.h
beebmem.o: /usr/include/SDL/SDL_joystick.h /usr/include/SDL/SDL_quit.h
beebmem.o: /usr/include/SDL/SDL_loadso.h /usr/include/SDL/SDL_timer.h
beebmem.o: /usr/include/SDL/SDL_version.h sdl.h disc8271.h main.h sysvia.h
beebmem.o: via.h uservia.h video.h atodconv.h beebconfig_data.h beebconfig.h
beebmem.o: messagebox.h ./gui/types.h ./gui/widget.h ./gui/window.h
beebmem.o: ./gui/box.h ./gui/sdl.h ./gui/label.h ./gui/button.h
beebmem.o: ./gui/widget_private.h uefstate.h messages.h
beebwin.o: /usr/include/string.h /usr/include/features.h
beebwin.o: /usr/include/sys/cdefs.h /usr/include/bits/wordsize.h
beebwin.o: /usr/include/gnu/stubs.h /usr/include/gnu/stubs-32.h
beebwin.o: /usr/include/linux/stddef.h /usr/include/xlocale.h
beebwin.o: /usr/include/stdio.h /usr/include/bits/types.h
beebwin.o: /usr/include/bits/typesizes.h /usr/include/libio.h
beebwin.o: /usr/include/_G_config.h /usr/include/wchar.h
beebwin.o: /usr/include/bits/wchar.h /usr/include/gconv.h
beebwin.o: /usr/include/bits/stdio_lim.h /usr/include/bits/sys_errlist.h
beebwin.o: 6502core.h port.h /usr/include/linux/limits.h beebwin.h
beebwin.o: /usr/include/stdlib.h /usr/include/bits/waitflags.h
beebwin.o: /usr/include/bits/waitstatus.h /usr/include/endian.h
beebwin.o: /usr/include/bits/endian.h /usr/include/sys/types.h
beebwin.o: /usr/include/linux/time.h /usr/include/linux/types.h
beebwin.o: /usr/include/linux/posix_types.h /usr/include/linux/stddef.h
beebwin.o: /usr/include/asm/posix_types.h /usr/include/asm-i386/posix_types.h
beebwin.o: /usr/include/asm/types.h /usr/include/asm-i386/types.h
beebwin.o: /usr/include/sys/select.h /usr/include/bits/select.h
beebwin.o: /usr/include/bits/sigset.h /usr/include/bits/time.h
beebwin.o: /usr/include/sys/sysmacros.h /usr/include/bits/pthreadtypes.h
beebwin.o: /usr/include/alloca.h /usr/include/SDL/SDL.h
beebwin.o: /usr/include/SDL/SDL_main.h /usr/include/SDL/SDL_stdinc.h
beebwin.o: /usr/include/SDL/SDL_config.h /usr/include/SDL/SDL_platform.h
beebwin.o: /usr/include/strings.h /usr/include/inttypes.h
beebwin.o: /usr/include/stdint.h /usr/include/ctype.h /usr/include/iconv.h
beebwin.o: /usr/include/SDL/begin_code.h /usr/include/SDL/close_code.h
beebwin.o: /usr/include/SDL/SDL_audio.h /usr/include/SDL/SDL_error.h
beebwin.o: /usr/include/SDL/SDL_endian.h /usr/include/SDL/SDL_mutex.h
beebwin.o: /usr/include/SDL/SDL_thread.h /usr/include/SDL/SDL_rwops.h
beebwin.o: /usr/include/SDL/SDL_cdrom.h /usr/include/SDL/SDL_cpuinfo.h
beebwin.o: /usr/include/SDL/SDL_events.h /usr/include/SDL/SDL_active.h
beebwin.o: /usr/include/SDL/SDL_keyboard.h /usr/include/SDL/SDL_keysym.h
beebwin.o: /usr/include/SDL/SDL_mouse.h /usr/include/SDL/SDL_video.h
beebwin.o: /usr/include/SDL/SDL_joystick.h /usr/include/SDL/SDL_quit.h
beebwin.o: /usr/include/SDL/SDL_loadso.h /usr/include/SDL/SDL_timer.h
beebwin.o: /usr/include/SDL/SDL_version.h sdl.h beebmem.h beebsound.h
beebwin.o: disc8271.h sysvia.h via.h uservia.h video.h beebconfig.h
beebwin.o: beebconfig_data.h virtualkeyboard.h
disc8271.o: /usr/include/stdio.h /usr/include/features.h
disc8271.o: /usr/include/sys/cdefs.h /usr/include/bits/wordsize.h
disc8271.o: /usr/include/gnu/stubs.h /usr/include/gnu/stubs-32.h
disc8271.o: /usr/include/linux/stddef.h /usr/include/bits/types.h
disc8271.o: /usr/include/bits/typesizes.h /usr/include/libio.h
disc8271.o: /usr/include/_G_config.h /usr/include/wchar.h
disc8271.o: /usr/include/bits/wchar.h /usr/include/gconv.h
disc8271.o: /usr/include/bits/stdio_lim.h /usr/include/bits/sys_errlist.h
disc8271.o: /usr/include/stdlib.h /usr/include/bits/waitflags.h
disc8271.o: /usr/include/bits/waitstatus.h /usr/include/endian.h
disc8271.o: /usr/include/bits/endian.h /usr/include/xlocale.h
disc8271.o: /usr/include/sys/types.h /usr/include/linux/time.h
disc8271.o: /usr/include/linux/types.h /usr/include/linux/posix_types.h
disc8271.o: /usr/include/linux/stddef.h /usr/include/asm/posix_types.h
disc8271.o: /usr/include/asm-i386/posix_types.h /usr/include/asm/types.h
disc8271.o: /usr/include/asm-i386/types.h /usr/include/sys/select.h
disc8271.o: /usr/include/bits/select.h /usr/include/bits/sigset.h
disc8271.o: /usr/include/bits/time.h /usr/include/sys/sysmacros.h
disc8271.o: /usr/include/bits/pthreadtypes.h /usr/include/alloca.h
disc8271.o: /usr/include/string.h 6502core.h port.h
disc8271.o: /usr/include/linux/limits.h beebwin.h /usr/include/SDL/SDL.h
disc8271.o: /usr/include/SDL/SDL_main.h /usr/include/SDL/SDL_stdinc.h
disc8271.o: /usr/include/SDL/SDL_config.h /usr/include/SDL/SDL_platform.h
disc8271.o: /usr/include/strings.h /usr/include/inttypes.h
disc8271.o: /usr/include/stdint.h /usr/include/ctype.h /usr/include/iconv.h
disc8271.o: /usr/include/SDL/begin_code.h /usr/include/SDL/close_code.h
disc8271.o: /usr/include/SDL/SDL_audio.h /usr/include/SDL/SDL_error.h
disc8271.o: /usr/include/SDL/SDL_endian.h /usr/include/SDL/SDL_mutex.h
disc8271.o: /usr/include/SDL/SDL_thread.h /usr/include/SDL/SDL_rwops.h
disc8271.o: /usr/include/SDL/SDL_cdrom.h /usr/include/SDL/SDL_cpuinfo.h
disc8271.o: /usr/include/SDL/SDL_events.h /usr/include/SDL/SDL_active.h
disc8271.o: /usr/include/SDL/SDL_keyboard.h /usr/include/SDL/SDL_keysym.h
disc8271.o: /usr/include/SDL/SDL_mouse.h /usr/include/SDL/SDL_video.h
disc8271.o: /usr/include/SDL/SDL_joystick.h /usr/include/SDL/SDL_quit.h
disc8271.o: /usr/include/SDL/SDL_loadso.h /usr/include/SDL/SDL_timer.h
disc8271.o: /usr/include/SDL/SDL_version.h sdl.h disc8271.h uefstate.h
disc8271.o: beebconfig_data.h beebconfig.h messages.h main.h messagebox.h
disc8271.o: ./gui/types.h ./gui/widget.h ./gui/window.h ./gui/box.h
disc8271.o: ./gui/sdl.h ./gui/label.h ./gui/button.h ./gui/widget_private.h
main.o: /usr/include/linux/signal.h /usr/include/asm/signal.h
main.o: /usr/include/asm-i386/signal.h /usr/include/linux/types.h
main.o: /usr/include/linux/posix_types.h /usr/include/linux/stddef.h
main.o: /usr/include/asm/posix_types.h /usr/include/asm-i386/posix_types.h
main.o: /usr/include/asm/types.h /usr/include/asm-i386/types.h
main.o: /usr/include/linux/time.h /usr/include/asm-generic/signal.h
main.o: /usr/include/asm/siginfo.h /usr/include/asm-i386/siginfo.h
main.o: /usr/include/asm-generic/siginfo.h /usr/include/stdio.h
main.o: /usr/include/features.h /usr/include/sys/cdefs.h
main.o: /usr/include/bits/wordsize.h /usr/include/gnu/stubs.h
main.o: /usr/include/gnu/stubs-32.h /usr/include/linux/stddef.h
main.o: /usr/include/bits/types.h /usr/include/bits/typesizes.h
main.o: /usr/include/libio.h /usr/include/_G_config.h /usr/include/wchar.h
main.o: /usr/include/bits/wchar.h /usr/include/gconv.h
main.o: /usr/include/bits/stdio_lim.h /usr/include/bits/sys_errlist.h
main.o: /usr/include/stdlib.h /usr/include/bits/waitflags.h
main.o: /usr/include/bits/waitstatus.h /usr/include/endian.h
main.o: /usr/include/bits/endian.h /usr/include/xlocale.h
main.o: /usr/include/sys/types.h /usr/include/linux/time.h
main.o: /usr/include/sys/select.h /usr/include/bits/select.h
main.o: /usr/include/bits/sigset.h /usr/include/bits/time.h
main.o: /usr/include/sys/sysmacros.h /usr/include/bits/pthreadtypes.h
main.o: /usr/include/alloca.h 6502core.h port.h /usr/include/linux/limits.h
main.o: beebwin.h /usr/include/string.h /usr/include/SDL/SDL.h
main.o: /usr/include/SDL/SDL_main.h /usr/include/SDL/SDL_stdinc.h
main.o: /usr/include/SDL/SDL_config.h /usr/include/SDL/SDL_platform.h
main.o: /usr/include/strings.h /usr/include/inttypes.h /usr/include/stdint.h
main.o: /usr/include/ctype.h /usr/include/iconv.h
main.o: /usr/include/SDL/begin_code.h /usr/include/SDL/close_code.h
main.o: /usr/include/SDL/SDL_audio.h /usr/include/SDL/SDL_error.h
main.o: /usr/include/SDL/SDL_endian.h /usr/include/SDL/SDL_mutex.h
main.o: /usr/include/SDL/SDL_thread.h /usr/include/SDL/SDL_rwops.h
main.o: /usr/include/SDL/SDL_cdrom.h /usr/include/SDL/SDL_cpuinfo.h
main.o: /usr/include/SDL/SDL_events.h /usr/include/SDL/SDL_active.h
main.o: /usr/include/SDL/SDL_keyboard.h /usr/include/SDL/SDL_keysym.h
main.o: /usr/include/SDL/SDL_mouse.h /usr/include/SDL/SDL_video.h
main.o: /usr/include/SDL/SDL_joystick.h /usr/include/SDL/SDL_quit.h
main.o: /usr/include/SDL/SDL_loadso.h /usr/include/SDL/SDL_timer.h
main.o: /usr/include/SDL/SDL_version.h sdl.h beebmem.h beebsound.h sysvia.h
main.o: via.h uservia.h disc8271.h video.h keyboard.h beebconfig.h
main.o: beebconfig_data.h virtualkeyboard.h button.h messagebox.h
main.o: ./gui/types.h ./gui/widget.h ./gui/window.h ./gui/box.h ./gui/sdl.h
main.o: ./gui/label.h ./gui/button.h ./gui/widget_private.h ./gui/gui.h
main.o: ./gui/tickbox.h ./gui/radiogroup.h ./gui/radiobutton.h
main.o: ./gui/slidebar.h ./gui/togglebutton.h ./gui/progressbar.h
main.o: ./gui/image.h ./gui/graphics.h ./gui/functions.h ./gui/line.h
main.o: ./gui/log.h ./gui/graphics_colors.h ./gui/graphics_font.h
main.o: ./gui/graphics_font.h ./gui/graphics_fontmap.h beebem_pages.h
main.o: keyboardmapper.h uefstate.h listbox.h fileselector.h header
sysvia.o: 6502core.h port.h /usr/include/linux/limits.h beebwin.h
sysvia.o: /usr/include/string.h /usr/include/features.h
sysvia.o: /usr/include/sys/cdefs.h /usr/include/bits/wordsize.h
sysvia.o: /usr/include/gnu/stubs.h /usr/include/gnu/stubs-32.h
sysvia.o: /usr/include/linux/stddef.h /usr/include/xlocale.h
sysvia.o: /usr/include/stdlib.h /usr/include/bits/waitflags.h
sysvia.o: /usr/include/bits/waitstatus.h /usr/include/endian.h
sysvia.o: /usr/include/bits/endian.h /usr/include/sys/types.h
sysvia.o: /usr/include/bits/types.h /usr/include/bits/typesizes.h
sysvia.o: /usr/include/linux/time.h /usr/include/linux/types.h
sysvia.o: /usr/include/linux/posix_types.h /usr/include/linux/stddef.h
sysvia.o: /usr/include/asm/posix_types.h /usr/include/asm-i386/posix_types.h
sysvia.o: /usr/include/asm/types.h /usr/include/asm-i386/types.h
sysvia.o: /usr/include/sys/select.h /usr/include/bits/select.h
sysvia.o: /usr/include/bits/sigset.h /usr/include/bits/time.h
sysvia.o: /usr/include/sys/sysmacros.h /usr/include/bits/pthreadtypes.h
sysvia.o: /usr/include/alloca.h /usr/include/SDL/SDL.h
sysvia.o: /usr/include/SDL/SDL_main.h /usr/include/SDL/SDL_stdinc.h
sysvia.o: /usr/include/SDL/SDL_config.h /usr/include/SDL/SDL_platform.h
sysvia.o: /usr/include/stdio.h /usr/include/libio.h /usr/include/_G_config.h
sysvia.o: /usr/include/wchar.h /usr/include/bits/wchar.h /usr/include/gconv.h
sysvia.o: /usr/include/bits/stdio_lim.h /usr/include/bits/sys_errlist.h
sysvia.o: /usr/include/strings.h /usr/include/inttypes.h
sysvia.o: /usr/include/stdint.h /usr/include/ctype.h /usr/include/iconv.h
sysvia.o: /usr/include/SDL/begin_code.h /usr/include/SDL/close_code.h
sysvia.o: /usr/include/SDL/SDL_audio.h /usr/include/SDL/SDL_error.h
sysvia.o: /usr/include/SDL/SDL_endian.h /usr/include/SDL/SDL_mutex.h
sysvia.o: /usr/include/SDL/SDL_thread.h /usr/include/SDL/SDL_rwops.h
sysvia.o: /usr/include/SDL/SDL_cdrom.h /usr/include/SDL/SDL_cpuinfo.h
sysvia.o: /usr/include/SDL/SDL_events.h /usr/include/SDL/SDL_active.h
sysvia.o: /usr/include/SDL/SDL_keyboard.h /usr/include/SDL/SDL_keysym.h
sysvia.o: /usr/include/SDL/SDL_mouse.h /usr/include/SDL/SDL_video.h
sysvia.o: /usr/include/SDL/SDL_joystick.h /usr/include/SDL/SDL_quit.h
sysvia.o: /usr/include/SDL/SDL_loadso.h /usr/include/SDL/SDL_timer.h
sysvia.o: /usr/include/SDL/SDL_version.h sdl.h beebsound.h sysvia.h via.h
uservia.o: 6502core.h port.h /usr/include/linux/limits.h beebwin.h
uservia.o: /usr/include/string.h /usr/include/features.h
uservia.o: /usr/include/sys/cdefs.h /usr/include/bits/wordsize.h
uservia.o: /usr/include/gnu/stubs.h /usr/include/gnu/stubs-32.h
uservia.o: /usr/include/linux/stddef.h /usr/include/xlocale.h
uservia.o: /usr/include/stdlib.h /usr/include/bits/waitflags.h
uservia.o: /usr/include/bits/waitstatus.h /usr/include/endian.h
uservia.o: /usr/include/bits/endian.h /usr/include/sys/types.h
uservia.o: /usr/include/bits/types.h /usr/include/bits/typesizes.h
uservia.o: /usr/include/linux/time.h /usr/include/linux/types.h
uservia.o: /usr/include/linux/posix_types.h /usr/include/linux/stddef.h
uservia.o: /usr/include/asm/posix_types.h /usr/include/asm-i386/posix_types.h
uservia.o: /usr/include/asm/types.h /usr/include/asm-i386/types.h
uservia.o: /usr/include/sys/select.h /usr/include/bits/select.h
uservia.o: /usr/include/bits/sigset.h /usr/include/bits/time.h
uservia.o: /usr/include/sys/sysmacros.h /usr/include/bits/pthreadtypes.h
uservia.o: /usr/include/alloca.h /usr/include/SDL/SDL.h
uservia.o: /usr/include/SDL/SDL_main.h /usr/include/SDL/SDL_stdinc.h
uservia.o: /usr/include/SDL/SDL_config.h /usr/include/SDL/SDL_platform.h
uservia.o: /usr/include/stdio.h /usr/include/libio.h /usr/include/_G_config.h
uservia.o: /usr/include/wchar.h /usr/include/bits/wchar.h
uservia.o: /usr/include/gconv.h /usr/include/bits/stdio_lim.h
uservia.o: /usr/include/bits/sys_errlist.h /usr/include/strings.h
uservia.o: /usr/include/inttypes.h /usr/include/stdint.h /usr/include/ctype.h
uservia.o: /usr/include/iconv.h /usr/include/SDL/begin_code.h
uservia.o: /usr/include/SDL/close_code.h /usr/include/SDL/SDL_audio.h
uservia.o: /usr/include/SDL/SDL_error.h /usr/include/SDL/SDL_endian.h
uservia.o: /usr/include/SDL/SDL_mutex.h /usr/include/SDL/SDL_thread.h
uservia.o: /usr/include/SDL/SDL_rwops.h /usr/include/SDL/SDL_cdrom.h
uservia.o: /usr/include/SDL/SDL_cpuinfo.h /usr/include/SDL/SDL_events.h
uservia.o: /usr/include/SDL/SDL_active.h /usr/include/SDL/SDL_keyboard.h
uservia.o: /usr/include/SDL/SDL_keysym.h /usr/include/SDL/SDL_mouse.h
uservia.o: /usr/include/SDL/SDL_video.h /usr/include/SDL/SDL_joystick.h
uservia.o: /usr/include/SDL/SDL_quit.h /usr/include/SDL/SDL_loadso.h
uservia.o: /usr/include/SDL/SDL_timer.h /usr/include/SDL/SDL_version.h sdl.h
uservia.o: uservia.h via.h
via.o: via.h /usr/include/stdio.h /usr/include/features.h
via.o: /usr/include/sys/cdefs.h /usr/include/bits/wordsize.h
via.o: /usr/include/gnu/stubs.h /usr/include/gnu/stubs-32.h
via.o: /usr/include/linux/stddef.h /usr/include/bits/types.h
via.o: /usr/include/bits/typesizes.h /usr/include/libio.h
via.o: /usr/include/_G_config.h /usr/include/wchar.h
via.o: /usr/include/bits/wchar.h /usr/include/gconv.h
via.o: /usr/include/bits/stdio_lim.h /usr/include/bits/sys_errlist.h
via.o: uefstate.h
video.o: /usr/include/stdlib.h /usr/include/features.h
video.o: /usr/include/sys/cdefs.h /usr/include/bits/wordsize.h
video.o: /usr/include/gnu/stubs.h /usr/include/gnu/stubs-32.h
video.o: /usr/include/linux/stddef.h /usr/include/bits/waitflags.h
video.o: /usr/include/bits/waitstatus.h /usr/include/endian.h
video.o: /usr/include/bits/endian.h /usr/include/xlocale.h
video.o: /usr/include/sys/types.h /usr/include/bits/types.h
video.o: /usr/include/bits/typesizes.h /usr/include/linux/time.h
video.o: /usr/include/linux/types.h /usr/include/linux/posix_types.h
video.o: /usr/include/linux/stddef.h /usr/include/asm/posix_types.h
video.o: /usr/include/asm-i386/posix_types.h /usr/include/asm/types.h
video.o: /usr/include/asm-i386/types.h /usr/include/sys/select.h
video.o: /usr/include/bits/select.h /usr/include/bits/sigset.h
video.o: /usr/include/bits/time.h /usr/include/sys/sysmacros.h
video.o: /usr/include/bits/pthreadtypes.h /usr/include/alloca.h
video.o: /usr/include/SDL/SDL.h /usr/include/SDL/SDL_main.h
video.o: /usr/include/SDL/SDL_stdinc.h /usr/include/SDL/SDL_config.h
video.o: /usr/include/SDL/SDL_platform.h /usr/include/stdio.h
video.o: /usr/include/libio.h /usr/include/_G_config.h /usr/include/wchar.h
video.o: /usr/include/bits/wchar.h /usr/include/gconv.h
video.o: /usr/include/bits/stdio_lim.h /usr/include/bits/sys_errlist.h
video.o: /usr/include/string.h /usr/include/strings.h /usr/include/inttypes.h
video.o: /usr/include/stdint.h /usr/include/ctype.h /usr/include/iconv.h
video.o: /usr/include/SDL/begin_code.h /usr/include/SDL/close_code.h
video.o: /usr/include/SDL/SDL_audio.h /usr/include/SDL/SDL_error.h
video.o: /usr/include/SDL/SDL_endian.h /usr/include/SDL/SDL_mutex.h
video.o: /usr/include/SDL/SDL_thread.h /usr/include/SDL/SDL_rwops.h
video.o: /usr/include/SDL/SDL_cdrom.h /usr/include/SDL/SDL_cpuinfo.h
video.o: /usr/include/SDL/SDL_events.h /usr/include/SDL/SDL_active.h
video.o: /usr/include/SDL/SDL_keyboard.h /usr/include/SDL/SDL_keysym.h
video.o: /usr/include/SDL/SDL_mouse.h /usr/include/SDL/SDL_video.h
video.o: /usr/include/SDL/SDL_joystick.h /usr/include/SDL/SDL_quit.h
video.o: /usr/include/SDL/SDL_loadso.h /usr/include/SDL/SDL_timer.h
video.o: /usr/include/SDL/SDL_version.h 6502core.h port.h
video.o: /usr/include/linux/limits.h beebwin.h sdl.h beebmem.h main.h
video.o: mode7font.h sysvia.h via.h video.h beebconfig.h beebconfig_data.h
video.o: uefstate.h virtualkeyboard.h
atodconv.o: /usr/include/stdio.h /usr/include/features.h
atodconv.o: /usr/include/sys/cdefs.h /usr/include/bits/wordsize.h
atodconv.o: /usr/include/gnu/stubs.h /usr/include/gnu/stubs-32.h
atodconv.o: /usr/include/linux/stddef.h /usr/include/bits/types.h
atodconv.o: /usr/include/bits/typesizes.h /usr/include/libio.h
atodconv.o: /usr/include/_G_config.h /usr/include/wchar.h
atodconv.o: /usr/include/bits/wchar.h /usr/include/gconv.h
atodconv.o: /usr/include/bits/stdio_lim.h /usr/include/bits/sys_errlist.h
atodconv.o: 6502core.h port.h /usr/include/linux/limits.h beebwin.h
atodconv.o: /usr/include/string.h /usr/include/xlocale.h
atodconv.o: /usr/include/stdlib.h /usr/include/bits/waitflags.h
atodconv.o: /usr/include/bits/waitstatus.h /usr/include/endian.h
atodconv.o: /usr/include/bits/endian.h /usr/include/sys/types.h
atodconv.o: /usr/include/linux/time.h /usr/include/linux/types.h
atodconv.o: /usr/include/linux/posix_types.h /usr/include/linux/stddef.h
atodconv.o: /usr/include/asm/posix_types.h
atodconv.o: /usr/include/asm-i386/posix_types.h /usr/include/asm/types.h
atodconv.o: /usr/include/asm-i386/types.h /usr/include/sys/select.h
atodconv.o: /usr/include/bits/select.h /usr/include/bits/sigset.h
atodconv.o: /usr/include/bits/time.h /usr/include/sys/sysmacros.h
atodconv.o: /usr/include/bits/pthreadtypes.h /usr/include/alloca.h
atodconv.o: /usr/include/SDL/SDL.h /usr/include/SDL/SDL_main.h
atodconv.o: /usr/include/SDL/SDL_stdinc.h /usr/include/SDL/SDL_config.h
atodconv.o: /usr/include/SDL/SDL_platform.h /usr/include/strings.h
atodconv.o: /usr/include/inttypes.h /usr/include/stdint.h
atodconv.o: /usr/include/ctype.h /usr/include/iconv.h
atodconv.o: /usr/include/SDL/begin_code.h /usr/include/SDL/close_code.h
atodconv.o: /usr/include/SDL/SDL_audio.h /usr/include/SDL/SDL_error.h
atodconv.o: /usr/include/SDL/SDL_endian.h /usr/include/SDL/SDL_mutex.h
atodconv.o: /usr/include/SDL/SDL_thread.h /usr/include/SDL/SDL_rwops.h
atodconv.o: /usr/include/SDL/SDL_cdrom.h /usr/include/SDL/SDL_cpuinfo.h
atodconv.o: /usr/include/SDL/SDL_events.h /usr/include/SDL/SDL_active.h
atodconv.o: /usr/include/SDL/SDL_keyboard.h /usr/include/SDL/SDL_keysym.h
atodconv.o: /usr/include/SDL/SDL_mouse.h /usr/include/SDL/SDL_video.h
atodconv.o: /usr/include/SDL/SDL_joystick.h /usr/include/SDL/SDL_quit.h
atodconv.o: /usr/include/SDL/SDL_loadso.h /usr/include/SDL/SDL_timer.h
atodconv.o: /usr/include/SDL/SDL_version.h sdl.h atodconv.h
sdl.o: /usr/include/SDL/SDL.h /usr/include/SDL/SDL_main.h
sdl.o: /usr/include/SDL/SDL_stdinc.h /usr/include/SDL/SDL_config.h
sdl.o: /usr/include/SDL/SDL_platform.h /usr/include/sys/types.h
sdl.o: /usr/include/features.h /usr/include/sys/cdefs.h
sdl.o: /usr/include/bits/wordsize.h /usr/include/gnu/stubs.h
sdl.o: /usr/include/gnu/stubs-32.h /usr/include/bits/types.h
sdl.o: /usr/include/linux/stddef.h /usr/include/bits/typesizes.h
sdl.o: /usr/include/linux/time.h /usr/include/linux/types.h
sdl.o: /usr/include/linux/posix_types.h /usr/include/linux/stddef.h
sdl.o: /usr/include/asm/posix_types.h /usr/include/asm-i386/posix_types.h
sdl.o: /usr/include/asm/types.h /usr/include/asm-i386/types.h
sdl.o: /usr/include/endian.h /usr/include/bits/endian.h
sdl.o: /usr/include/sys/select.h /usr/include/bits/select.h
sdl.o: /usr/include/bits/sigset.h /usr/include/bits/time.h
sdl.o: /usr/include/sys/sysmacros.h /usr/include/bits/pthreadtypes.h
sdl.o: /usr/include/stdio.h /usr/include/libio.h /usr/include/_G_config.h
sdl.o: /usr/include/wchar.h /usr/include/bits/wchar.h /usr/include/gconv.h
sdl.o: /usr/include/bits/stdio_lim.h /usr/include/bits/sys_errlist.h
sdl.o: /usr/include/stdlib.h /usr/include/bits/waitflags.h
sdl.o: /usr/include/bits/waitstatus.h /usr/include/xlocale.h
sdl.o: /usr/include/alloca.h /usr/include/string.h /usr/include/strings.h
sdl.o: /usr/include/inttypes.h /usr/include/stdint.h /usr/include/ctype.h
sdl.o: /usr/include/iconv.h /usr/include/SDL/begin_code.h
sdl.o: /usr/include/SDL/close_code.h /usr/include/SDL/SDL_audio.h
sdl.o: /usr/include/SDL/SDL_error.h /usr/include/SDL/SDL_endian.h
sdl.o: /usr/include/SDL/SDL_mutex.h /usr/include/SDL/SDL_thread.h
sdl.o: /usr/include/SDL/SDL_rwops.h /usr/include/SDL/SDL_cdrom.h
sdl.o: /usr/include/SDL/SDL_cpuinfo.h /usr/include/SDL/SDL_events.h
sdl.o: /usr/include/SDL/SDL_active.h /usr/include/SDL/SDL_keyboard.h
sdl.o: /usr/include/SDL/SDL_keysym.h /usr/include/SDL/SDL_mouse.h
sdl.o: /usr/include/SDL/SDL_video.h /usr/include/SDL/SDL_joystick.h
sdl.o: /usr/include/SDL/SDL_quit.h /usr/include/SDL/SDL_loadso.h
sdl.o: /usr/include/SDL/SDL_timer.h /usr/include/SDL/SDL_version.h
sdl.o: /usr/include/sys/mman.h /usr/include/bits/mman.h
sdl.o: /usr/include/sys/stat.h /usr/include/bits/stat.h
sdl.o: /usr/include/linux/fcntl.h /usr/include/asm/fcntl.h
sdl.o: /usr/include/asm-i386/fcntl.h /usr/include/asm-generic/fcntl.h
sdl.o: /usr/include/sys/ioctl.h /usr/include/bits/ioctls.h
sdl.o: /usr/include/asm/ioctls.h /usr/include/asm-i386/ioctls.h
sdl.o: /usr/include/asm/ioctl.h /usr/include/asm-i386/ioctl.h
sdl.o: /usr/include/asm-generic/ioctl.h /usr/include/bits/ioctl-types.h
sdl.o: /usr/include/sys/ttydefaults.h /usr/include/sys/soundcard.h
sdl.o: /usr/include/linux/soundcard.h /usr/include/linux/ioctl.h
sdl.o: /usr/include/linux/patchkey.h /usr/include/sys/time.h
sdl.o: /usr/include/sys/resource.h /usr/include/bits/resource.h
sdl.o: /usr/include/linux/unistd.h /usr/include/asm/unistd.h
sdl.o: /usr/include/asm-i386/unistd.h beebconfig.h beebconfig_data.h
keyboard.o: /usr/include/string.h /usr/include/features.h
keyboard.o: /usr/include/sys/cdefs.h /usr/include/bits/wordsize.h
keyboard.o: /usr/include/gnu/stubs.h /usr/include/gnu/stubs-32.h
keyboard.o: /usr/include/linux/stddef.h /usr/include/xlocale.h
keyboard.o: /usr/include/ctype.h /usr/include/bits/types.h
keyboard.o: /usr/include/bits/typesizes.h /usr/include/endian.h
keyboard.o: /usr/include/bits/endian.h /usr/include/stdlib.h
keyboard.o: /usr/include/bits/waitflags.h /usr/include/bits/waitstatus.h
keyboard.o: /usr/include/sys/types.h /usr/include/linux/time.h
keyboard.o: /usr/include/linux/types.h /usr/include/linux/posix_types.h
keyboard.o: /usr/include/linux/stddef.h /usr/include/asm/posix_types.h
keyboard.o: /usr/include/asm-i386/posix_types.h /usr/include/asm/types.h
keyboard.o: /usr/include/asm-i386/types.h /usr/include/sys/select.h
keyboard.o: /usr/include/bits/select.h /usr/include/bits/sigset.h
keyboard.o: /usr/include/bits/time.h /usr/include/sys/sysmacros.h
keyboard.o: /usr/include/bits/pthreadtypes.h /usr/include/alloca.h keyboard.h
keyboard.o: /usr/include/SDL/SDL.h /usr/include/SDL/SDL_main.h
keyboard.o: /usr/include/SDL/SDL_stdinc.h /usr/include/SDL/SDL_config.h
keyboard.o: /usr/include/SDL/SDL_platform.h /usr/include/stdio.h
keyboard.o: /usr/include/libio.h /usr/include/_G_config.h
keyboard.o: /usr/include/wchar.h /usr/include/bits/wchar.h
keyboard.o: /usr/include/gconv.h /usr/include/bits/stdio_lim.h
keyboard.o: /usr/include/bits/sys_errlist.h /usr/include/strings.h
keyboard.o: /usr/include/inttypes.h /usr/include/stdint.h
keyboard.o: /usr/include/iconv.h /usr/include/SDL/begin_code.h
keyboard.o: /usr/include/SDL/close_code.h /usr/include/SDL/SDL_audio.h
keyboard.o: /usr/include/SDL/SDL_error.h /usr/include/SDL/SDL_endian.h
keyboard.o: /usr/include/SDL/SDL_mutex.h /usr/include/SDL/SDL_thread.h
keyboard.o: /usr/include/SDL/SDL_rwops.h /usr/include/SDL/SDL_cdrom.h
keyboard.o: /usr/include/SDL/SDL_cpuinfo.h /usr/include/SDL/SDL_events.h
keyboard.o: /usr/include/SDL/SDL_active.h /usr/include/SDL/SDL_keyboard.h
keyboard.o: /usr/include/SDL/SDL_keysym.h /usr/include/SDL/SDL_mouse.h
keyboard.o: /usr/include/SDL/SDL_video.h /usr/include/SDL/SDL_joystick.h
keyboard.o: /usr/include/SDL/SDL_quit.h /usr/include/SDL/SDL_loadso.h
keyboard.o: /usr/include/SDL/SDL_timer.h /usr/include/SDL/SDL_version.h
keyboard.o: beebconfig.h beebconfig_data.h sysvia.h via.h main.h beebwin.h
keyboard.o: port.h /usr/include/linux/limits.h sdl.h disc8271.h video.h
beebconfig.o: beebconfig.h sysvia.h via.h /usr/include/stdio.h
beebconfig.o: /usr/include/features.h /usr/include/sys/cdefs.h
beebconfig.o: /usr/include/bits/wordsize.h /usr/include/gnu/stubs.h
beebconfig.o: /usr/include/gnu/stubs-32.h /usr/include/linux/stddef.h
beebconfig.o: /usr/include/bits/types.h /usr/include/bits/typesizes.h
beebconfig.o: /usr/include/libio.h /usr/include/_G_config.h
beebconfig.o: /usr/include/wchar.h /usr/include/bits/wchar.h
beebconfig.o: /usr/include/gconv.h /usr/include/bits/stdio_lim.h
beebconfig.o: /usr/include/bits/sys_errlist.h main.h beebwin.h
beebconfig.o: /usr/include/string.h /usr/include/xlocale.h
beebconfig.o: /usr/include/stdlib.h /usr/include/bits/waitflags.h
beebconfig.o: /usr/include/bits/waitstatus.h /usr/include/endian.h
beebconfig.o: /usr/include/bits/endian.h /usr/include/sys/types.h
beebconfig.o: /usr/include/linux/time.h /usr/include/linux/types.h
beebconfig.o: /usr/include/linux/posix_types.h /usr/include/linux/stddef.h
beebconfig.o: /usr/include/asm/posix_types.h
beebconfig.o: /usr/include/asm-i386/posix_types.h /usr/include/asm/types.h
beebconfig.o: /usr/include/asm-i386/types.h /usr/include/sys/select.h
beebconfig.o: /usr/include/bits/select.h /usr/include/bits/sigset.h
beebconfig.o: /usr/include/bits/time.h /usr/include/sys/sysmacros.h
beebconfig.o: /usr/include/bits/pthreadtypes.h /usr/include/alloca.h
beebconfig.o: /usr/include/SDL/SDL.h /usr/include/SDL/SDL_main.h
beebconfig.o: /usr/include/SDL/SDL_stdinc.h /usr/include/SDL/SDL_config.h
beebconfig.o: /usr/include/SDL/SDL_platform.h /usr/include/strings.h
beebconfig.o: /usr/include/inttypes.h /usr/include/stdint.h
beebconfig.o: /usr/include/ctype.h /usr/include/iconv.h
beebconfig.o: /usr/include/SDL/begin_code.h /usr/include/SDL/close_code.h
beebconfig.o: /usr/include/SDL/SDL_audio.h /usr/include/SDL/SDL_error.h
beebconfig.o: /usr/include/SDL/SDL_endian.h /usr/include/SDL/SDL_mutex.h
beebconfig.o: /usr/include/SDL/SDL_thread.h /usr/include/SDL/SDL_rwops.h
beebconfig.o: /usr/include/SDL/SDL_cdrom.h /usr/include/SDL/SDL_cpuinfo.h
beebconfig.o: /usr/include/SDL/SDL_events.h /usr/include/SDL/SDL_active.h
beebconfig.o: /usr/include/SDL/SDL_keyboard.h /usr/include/SDL/SDL_keysym.h
beebconfig.o: /usr/include/SDL/SDL_mouse.h /usr/include/SDL/SDL_video.h
beebconfig.o: /usr/include/SDL/SDL_joystick.h /usr/include/SDL/SDL_quit.h
beebconfig.o: /usr/include/SDL/SDL_loadso.h /usr/include/SDL/SDL_timer.h
beebconfig.o: /usr/include/SDL/SDL_version.h port.h
beebconfig.o: /usr/include/linux/limits.h sdl.h disc8271.h video.h keyboard.h
beebconfig.o: messagebox.h ./gui/types.h ./gui/widget.h ./gui/window.h
beebconfig.o: ./gui/box.h ./gui/sdl.h ./gui/label.h ./gui/button.h
beebconfig.o: ./gui/widget_private.h messages.h /usr/include/linux/unistd.h
beebconfig.o: /usr/include/asm/unistd.h /usr/include/asm-i386/unistd.h
beebconfig.o: ./gui/gui.h ./gui/tickbox.h ./gui/radiogroup.h
beebconfig.o: ./gui/radiobutton.h ./gui/slidebar.h ./gui/togglebutton.h
beebconfig.o: ./gui/progressbar.h ./gui/image.h ./gui/graphics.h
beebconfig.o: ./gui/functions.h ./gui/line.h ./gui/log.h
beebconfig.o: ./gui/graphics_colors.h ./gui/graphics_font.h
beebconfig.o: ./gui/graphics_font.h ./gui/graphics_fontmap.h
beebem_pages.o: /usr/include/string.h /usr/include/features.h
beebem_pages.o: /usr/include/sys/cdefs.h /usr/include/bits/wordsize.h
beebem_pages.o: /usr/include/gnu/stubs.h /usr/include/gnu/stubs-32.h
beebem_pages.o: /usr/include/linux/stddef.h /usr/include/xlocale.h
beebem_pages.o: /usr/include/ctype.h /usr/include/bits/types.h
beebem_pages.o: /usr/include/bits/typesizes.h /usr/include/endian.h
beebem_pages.o: /usr/include/bits/endian.h /usr/include/stdlib.h
beebem_pages.o: /usr/include/bits/waitflags.h /usr/include/bits/waitstatus.h
beebem_pages.o: /usr/include/sys/types.h /usr/include/linux/time.h
beebem_pages.o: /usr/include/linux/types.h /usr/include/linux/posix_types.h
beebem_pages.o: /usr/include/linux/stddef.h /usr/include/asm/posix_types.h
beebem_pages.o: /usr/include/asm-i386/posix_types.h /usr/include/asm/types.h
beebem_pages.o: /usr/include/asm-i386/types.h /usr/include/sys/select.h
beebem_pages.o: /usr/include/bits/select.h /usr/include/bits/sigset.h
beebem_pages.o: /usr/include/bits/time.h /usr/include/sys/sysmacros.h
beebem_pages.o: /usr/include/bits/pthreadtypes.h /usr/include/alloca.h
beebem_pages.o: /usr/include/linux/unistd.h /usr/include/asm/unistd.h
beebem_pages.o: /usr/include/asm-i386/unistd.h /usr/include/stdint.h
beebem_pages.o: /usr/include/bits/wchar.h /usr/include/stdio.h
beebem_pages.o: /usr/include/libio.h /usr/include/_G_config.h
beebem_pages.o: /usr/include/wchar.h /usr/include/gconv.h
beebem_pages.o: /usr/include/bits/stdio_lim.h /usr/include/bits/sys_errlist.h
beebem_pages.o: ./gui/log.h ./gui/line.h /usr/include/SDL/SDL.h
beebem_pages.o: /usr/include/SDL/SDL_main.h /usr/include/SDL/SDL_stdinc.h
beebem_pages.o: /usr/include/SDL/SDL_config.h /usr/include/SDL/SDL_platform.h
beebem_pages.o: /usr/include/strings.h /usr/include/inttypes.h
beebem_pages.o: /usr/include/iconv.h /usr/include/SDL/begin_code.h
beebem_pages.o: /usr/include/SDL/close_code.h /usr/include/SDL/SDL_audio.h
beebem_pages.o: /usr/include/SDL/SDL_error.h /usr/include/SDL/SDL_endian.h
beebem_pages.o: /usr/include/SDL/SDL_mutex.h /usr/include/SDL/SDL_thread.h
beebem_pages.o: /usr/include/SDL/SDL_rwops.h /usr/include/SDL/SDL_cdrom.h
beebem_pages.o: /usr/include/SDL/SDL_cpuinfo.h /usr/include/SDL/SDL_events.h
beebem_pages.o: /usr/include/SDL/SDL_active.h /usr/include/SDL/SDL_keyboard.h
beebem_pages.o: /usr/include/SDL/SDL_keysym.h /usr/include/SDL/SDL_mouse.h
beebem_pages.o: /usr/include/SDL/SDL_video.h /usr/include/SDL/SDL_joystick.h
beebem_pages.o: /usr/include/SDL/SDL_quit.h /usr/include/SDL/SDL_loadso.h
beebem_pages.o: /usr/include/SDL/SDL_timer.h /usr/include/SDL/SDL_version.h
beebem_pages.o: messagebox.h ./gui/types.h ./gui/widget.h ./gui/window.h
beebem_pages.o: ./gui/box.h ./gui/sdl.h ./gui/label.h ./gui/button.h
beebem_pages.o: ./gui/widget_private.h sysvia.h via.h main.h beebwin.h port.h
beebem_pages.o: /usr/include/linux/limits.h sdl.h disc8271.h video.h
beebem_pages.o: beebconfig.h beebem_pages.h ./gui/gui.h ./gui/tickbox.h
beebem_pages.o: ./gui/radiogroup.h ./gui/radiobutton.h ./gui/slidebar.h
beebem_pages.o: ./gui/togglebutton.h ./gui/progressbar.h ./gui/image.h
beebem_pages.o: ./gui/graphics.h ./gui/functions.h ./gui/graphics_colors.h
beebem_pages.o: ./gui/graphics_font.h ./gui/graphics_font.h
beebem_pages.o: ./gui/graphics_fontmap.h keyboard.h button.h 6502core.h
beebem_pages.o: keyboardmapper.h fileselector.h listbox.h screensaver.h
beebem_pages.o: uefstate.h messages.h
screensaver.o: ./gui/graphics.h /usr/include/SDL/SDL.h
screensaver.o: /usr/include/SDL/SDL_main.h /usr/include/SDL/SDL_stdinc.h
screensaver.o: /usr/include/SDL/SDL_config.h /usr/include/SDL/SDL_platform.h
screensaver.o: /usr/include/sys/types.h /usr/include/features.h
screensaver.o: /usr/include/sys/cdefs.h /usr/include/bits/wordsize.h
screensaver.o: /usr/include/gnu/stubs.h /usr/include/gnu/stubs-32.h
screensaver.o: /usr/include/bits/types.h /usr/include/linux/stddef.h
screensaver.o: /usr/include/bits/typesizes.h /usr/include/linux/time.h
screensaver.o: /usr/include/linux/types.h /usr/include/linux/posix_types.h
screensaver.o: /usr/include/linux/stddef.h /usr/include/asm/posix_types.h
screensaver.o: /usr/include/asm-i386/posix_types.h /usr/include/asm/types.h
screensaver.o: /usr/include/asm-i386/types.h /usr/include/endian.h
screensaver.o: /usr/include/bits/endian.h /usr/include/sys/select.h
screensaver.o: /usr/include/bits/select.h /usr/include/bits/sigset.h
screensaver.o: /usr/include/bits/time.h /usr/include/sys/sysmacros.h
screensaver.o: /usr/include/bits/pthreadtypes.h /usr/include/stdio.h
screensaver.o: /usr/include/libio.h /usr/include/_G_config.h
screensaver.o: /usr/include/wchar.h /usr/include/bits/wchar.h
screensaver.o: /usr/include/gconv.h /usr/include/bits/stdio_lim.h
screensaver.o: /usr/include/bits/sys_errlist.h /usr/include/stdlib.h
screensaver.o: /usr/include/bits/waitflags.h /usr/include/bits/waitstatus.h
screensaver.o: /usr/include/xlocale.h /usr/include/alloca.h
screensaver.o: /usr/include/string.h /usr/include/strings.h
screensaver.o: /usr/include/inttypes.h /usr/include/stdint.h
screensaver.o: /usr/include/ctype.h /usr/include/iconv.h
screensaver.o: /usr/include/SDL/begin_code.h /usr/include/SDL/close_code.h
screensaver.o: /usr/include/SDL/SDL_audio.h /usr/include/SDL/SDL_error.h
screensaver.o: /usr/include/SDL/SDL_endian.h /usr/include/SDL/SDL_mutex.h
screensaver.o: /usr/include/SDL/SDL_thread.h /usr/include/SDL/SDL_rwops.h
screensaver.o: /usr/include/SDL/SDL_cdrom.h /usr/include/SDL/SDL_cpuinfo.h
screensaver.o: /usr/include/SDL/SDL_events.h /usr/include/SDL/SDL_active.h
screensaver.o: /usr/include/SDL/SDL_keyboard.h /usr/include/SDL/SDL_keysym.h
screensaver.o: /usr/include/SDL/SDL_mouse.h /usr/include/SDL/SDL_video.h
screensaver.o: /usr/include/SDL/SDL_joystick.h /usr/include/SDL/SDL_quit.h
screensaver.o: /usr/include/SDL/SDL_loadso.h /usr/include/SDL/SDL_timer.h
screensaver.o: /usr/include/SDL/SDL_version.h ./gui/types.h ./gui/functions.h
screensaver.o: ./gui/line.h ./gui/log.h ./gui/graphics_colors.h
screensaver.o: ./gui/graphics_font.h ./gui/graphics_font.h
screensaver.o: ./gui/graphics_fontmap.h /usr/include/math.h
screensaver.o: /usr/include/bits/huge_val.h /usr/include/bits/huge_valf.h
screensaver.o: /usr/include/bits/huge_vall.h /usr/include/bits/inf.h
screensaver.o: /usr/include/bits/nan.h /usr/include/bits/mathdef.h
screensaver.o: /usr/include/bits/mathcalls.h
virtualkeyboard.o: beebconfig.h beebconfig_data.h sysvia.h via.h
virtualkeyboard.o: /usr/include/stdio.h /usr/include/features.h
virtualkeyboard.o: /usr/include/sys/cdefs.h /usr/include/bits/wordsize.h
virtualkeyboard.o: /usr/include/gnu/stubs.h /usr/include/gnu/stubs-32.h
virtualkeyboard.o: /usr/include/linux/stddef.h /usr/include/bits/types.h
virtualkeyboard.o: /usr/include/bits/typesizes.h /usr/include/libio.h
virtualkeyboard.o: /usr/include/_G_config.h /usr/include/wchar.h
virtualkeyboard.o: /usr/include/bits/wchar.h /usr/include/gconv.h
virtualkeyboard.o: /usr/include/bits/stdio_lim.h
virtualkeyboard.o: /usr/include/bits/sys_errlist.h main.h beebwin.h
virtualkeyboard.o: /usr/include/string.h /usr/include/xlocale.h
virtualkeyboard.o: /usr/include/stdlib.h /usr/include/bits/waitflags.h
virtualkeyboard.o: /usr/include/bits/waitstatus.h /usr/include/endian.h
virtualkeyboard.o: /usr/include/bits/endian.h /usr/include/sys/types.h
virtualkeyboard.o: /usr/include/linux/time.h /usr/include/linux/types.h
virtualkeyboard.o: /usr/include/linux/posix_types.h
virtualkeyboard.o: /usr/include/linux/stddef.h /usr/include/asm/posix_types.h
virtualkeyboard.o: /usr/include/asm-i386/posix_types.h
virtualkeyboard.o: /usr/include/asm/types.h /usr/include/asm-i386/types.h
virtualkeyboard.o: /usr/include/sys/select.h /usr/include/bits/select.h
virtualkeyboard.o: /usr/include/bits/sigset.h /usr/include/bits/time.h
virtualkeyboard.o: /usr/include/sys/sysmacros.h
virtualkeyboard.o: /usr/include/bits/pthreadtypes.h /usr/include/alloca.h
virtualkeyboard.o: /usr/include/SDL/SDL.h /usr/include/SDL/SDL_main.h
virtualkeyboard.o: /usr/include/SDL/SDL_stdinc.h
virtualkeyboard.o: /usr/include/SDL/SDL_config.h
virtualkeyboard.o: /usr/include/SDL/SDL_platform.h /usr/include/strings.h
virtualkeyboard.o: /usr/include/inttypes.h /usr/include/stdint.h
virtualkeyboard.o: /usr/include/ctype.h /usr/include/iconv.h
virtualkeyboard.o: /usr/include/SDL/begin_code.h
virtualkeyboard.o: /usr/include/SDL/close_code.h /usr/include/SDL/SDL_audio.h
virtualkeyboard.o: /usr/include/SDL/SDL_error.h /usr/include/SDL/SDL_endian.h
virtualkeyboard.o: /usr/include/SDL/SDL_mutex.h /usr/include/SDL/SDL_thread.h
virtualkeyboard.o: /usr/include/SDL/SDL_rwops.h /usr/include/SDL/SDL_cdrom.h
virtualkeyboard.o: /usr/include/SDL/SDL_cpuinfo.h
virtualkeyboard.o: /usr/include/SDL/SDL_events.h
virtualkeyboard.o: /usr/include/SDL/SDL_active.h
virtualkeyboard.o: /usr/include/SDL/SDL_keyboard.h
virtualkeyboard.o: /usr/include/SDL/SDL_keysym.h /usr/include/SDL/SDL_mouse.h
virtualkeyboard.o: /usr/include/SDL/SDL_video.h
virtualkeyboard.o: /usr/include/SDL/SDL_joystick.h
virtualkeyboard.o: /usr/include/SDL/SDL_quit.h /usr/include/SDL/SDL_loadso.h
virtualkeyboard.o: /usr/include/SDL/SDL_timer.h
virtualkeyboard.o: /usr/include/SDL/SDL_version.h port.h
virtualkeyboard.o: /usr/include/linux/limits.h sdl.h disc8271.h 6502core.h
virtualkeyboard.o: virtualkeyboard.h keyboard.h
button.o: /usr/include/string.h /usr/include/features.h
button.o: /usr/include/sys/cdefs.h /usr/include/bits/wordsize.h
button.o: /usr/include/gnu/stubs.h /usr/include/gnu/stubs-32.h
button.o: /usr/include/linux/stddef.h /usr/include/xlocale.h
button.o: /usr/include/stdint.h /usr/include/bits/wchar.h
button.o: /usr/include/SDL/SDL.h /usr/include/SDL/SDL_main.h
button.o: /usr/include/SDL/SDL_stdinc.h /usr/include/SDL/SDL_config.h
button.o: /usr/include/SDL/SDL_platform.h /usr/include/sys/types.h
button.o: /usr/include/bits/types.h /usr/include/bits/typesizes.h
button.o: /usr/include/linux/time.h /usr/include/linux/types.h
button.o: /usr/include/linux/posix_types.h /usr/include/linux/stddef.h
button.o: /usr/include/asm/posix_types.h /usr/include/asm-i386/posix_types.h
button.o: /usr/include/asm/types.h /usr/include/asm-i386/types.h
button.o: /usr/include/endian.h /usr/include/bits/endian.h
button.o: /usr/include/sys/select.h /usr/include/bits/select.h
button.o: /usr/include/bits/sigset.h /usr/include/bits/time.h
button.o: /usr/include/sys/sysmacros.h /usr/include/bits/pthreadtypes.h
button.o: /usr/include/stdio.h /usr/include/libio.h /usr/include/_G_config.h
button.o: /usr/include/wchar.h /usr/include/gconv.h
button.o: /usr/include/bits/stdio_lim.h /usr/include/bits/sys_errlist.h
button.o: /usr/include/stdlib.h /usr/include/bits/waitflags.h
button.o: /usr/include/bits/waitstatus.h /usr/include/alloca.h
button.o: /usr/include/strings.h /usr/include/inttypes.h /usr/include/ctype.h
button.o: /usr/include/iconv.h /usr/include/SDL/begin_code.h
button.o: /usr/include/SDL/close_code.h /usr/include/SDL/SDL_audio.h
button.o: /usr/include/SDL/SDL_error.h /usr/include/SDL/SDL_endian.h
button.o: /usr/include/SDL/SDL_mutex.h /usr/include/SDL/SDL_thread.h
button.o: /usr/include/SDL/SDL_rwops.h /usr/include/SDL/SDL_cdrom.h
button.o: /usr/include/SDL/SDL_cpuinfo.h /usr/include/SDL/SDL_events.h
button.o: /usr/include/SDL/SDL_active.h /usr/include/SDL/SDL_keyboard.h
button.o: /usr/include/SDL/SDL_keysym.h /usr/include/SDL/SDL_mouse.h
button.o: /usr/include/SDL/SDL_video.h /usr/include/SDL/SDL_joystick.h
button.o: /usr/include/SDL/SDL_quit.h /usr/include/SDL/SDL_loadso.h
button.o: /usr/include/SDL/SDL_timer.h /usr/include/SDL/SDL_version.h
button.o: button.h keyboard.h
fileselector.o: /usr/include/sys/types.h /usr/include/features.h
fileselector.o: /usr/include/sys/cdefs.h /usr/include/bits/wordsize.h
fileselector.o: /usr/include/gnu/stubs.h /usr/include/gnu/stubs-32.h
fileselector.o: /usr/include/bits/types.h /usr/include/linux/stddef.h
fileselector.o: /usr/include/bits/typesizes.h /usr/include/linux/time.h
fileselector.o: /usr/include/linux/types.h /usr/include/linux/posix_types.h
fileselector.o: /usr/include/linux/stddef.h /usr/include/asm/posix_types.h
fileselector.o: /usr/include/asm-i386/posix_types.h /usr/include/asm/types.h
fileselector.o: /usr/include/asm-i386/types.h /usr/include/endian.h
fileselector.o: /usr/include/bits/endian.h /usr/include/sys/select.h
fileselector.o: /usr/include/bits/select.h /usr/include/bits/sigset.h
fileselector.o: /usr/include/bits/time.h /usr/include/sys/sysmacros.h
fileselector.o: /usr/include/bits/pthreadtypes.h /usr/include/linux/dirent.h
fileselector.o: fileselector.h /usr/include/stdio.h /usr/include/libio.h
fileselector.o: /usr/include/_G_config.h /usr/include/wchar.h
fileselector.o: /usr/include/bits/wchar.h /usr/include/gconv.h
fileselector.o: /usr/include/bits/stdio_lim.h /usr/include/bits/sys_errlist.h
fileselector.o: /usr/include/SDL/SDL.h /usr/include/SDL/SDL_main.h
fileselector.o: /usr/include/SDL/SDL_stdinc.h /usr/include/SDL/SDL_config.h
fileselector.o: /usr/include/SDL/SDL_platform.h /usr/include/stdlib.h
fileselector.o: /usr/include/bits/waitflags.h /usr/include/bits/waitstatus.h
fileselector.o: /usr/include/xlocale.h /usr/include/alloca.h
fileselector.o: /usr/include/string.h /usr/include/strings.h
fileselector.o: /usr/include/inttypes.h /usr/include/stdint.h
fileselector.o: /usr/include/ctype.h /usr/include/iconv.h
fileselector.o: /usr/include/SDL/begin_code.h /usr/include/SDL/close_code.h
fileselector.o: /usr/include/SDL/SDL_audio.h /usr/include/SDL/SDL_error.h
fileselector.o: /usr/include/SDL/SDL_endian.h /usr/include/SDL/SDL_mutex.h
fileselector.o: /usr/include/SDL/SDL_thread.h /usr/include/SDL/SDL_rwops.h
fileselector.o: /usr/include/SDL/SDL_cdrom.h /usr/include/SDL/SDL_cpuinfo.h
fileselector.o: /usr/include/SDL/SDL_events.h /usr/include/SDL/SDL_active.h
fileselector.o: /usr/include/SDL/SDL_keyboard.h /usr/include/SDL/SDL_keysym.h
fileselector.o: /usr/include/SDL/SDL_mouse.h /usr/include/SDL/SDL_video.h
fileselector.o: /usr/include/SDL/SDL_joystick.h /usr/include/SDL/SDL_quit.h
fileselector.o: /usr/include/SDL/SDL_loadso.h /usr/include/SDL/SDL_timer.h
fileselector.o: /usr/include/SDL/SDL_version.h listbox.h ./gui/gui.h
fileselector.o: ./gui/types.h ./gui/window.h ./gui/widget.h ./gui/box.h
fileselector.o: ./gui/sdl.h ./gui/label.h ./gui/button.h ./gui/tickbox.h
fileselector.o: ./gui/radiogroup.h ./gui/radiobutton.h ./gui/slidebar.h
fileselector.o: ./gui/togglebutton.h ./gui/progressbar.h ./gui/image.h
fileselector.o: ./gui/graphics.h ./gui/functions.h ./gui/line.h ./gui/log.h
fileselector.o: ./gui/graphics_colors.h ./gui/graphics_font.h
fileselector.o: ./gui/graphics_font.h ./gui/graphics_fontmap.h messagebox.h
fileselector.o: ./gui/widget_private.h progressdialog.h
fileselector.o: /usr/include/pthread.h /usr/include/linux/sched.h
fileselector.o: /usr/include/linux/auxvec.h /usr/include/asm/auxvec.h
fileselector.o: /usr/include/asm-i386/auxvec.h /usr/include/linux/signal.h
fileselector.o: /usr/include/asm/signal.h /usr/include/asm-i386/signal.h
fileselector.o: /usr/include/linux/time.h /usr/include/asm-generic/signal.h
fileselector.o: /usr/include/asm/siginfo.h /usr/include/asm-i386/siginfo.h
fileselector.o: /usr/include/asm-generic/siginfo.h /usr/include/bits/setjmp.h
fileselector.o: fileselector_saveas.h beebconfig.h messages.h
progressdialog.o: progressdialog.h /usr/include/stdio.h
progressdialog.o: /usr/include/features.h /usr/include/sys/cdefs.h
progressdialog.o: /usr/include/bits/wordsize.h /usr/include/gnu/stubs.h
progressdialog.o: /usr/include/gnu/stubs-32.h /usr/include/linux/stddef.h
progressdialog.o: /usr/include/bits/types.h /usr/include/bits/typesizes.h
progressdialog.o: /usr/include/libio.h /usr/include/_G_config.h
progressdialog.o: /usr/include/wchar.h /usr/include/bits/wchar.h
progressdialog.o: /usr/include/gconv.h /usr/include/bits/stdio_lim.h
progressdialog.o: /usr/include/bits/sys_errlist.h /usr/include/SDL/SDL.h
progressdialog.o: /usr/include/SDL/SDL_main.h /usr/include/SDL/SDL_stdinc.h
progressdialog.o: /usr/include/SDL/SDL_config.h
progressdialog.o: /usr/include/SDL/SDL_platform.h /usr/include/sys/types.h
progressdialog.o: /usr/include/linux/time.h /usr/include/linux/types.h
progressdialog.o: /usr/include/linux/posix_types.h
progressdialog.o: /usr/include/linux/stddef.h /usr/include/asm/posix_types.h
progressdialog.o: /usr/include/asm-i386/posix_types.h
progressdialog.o: /usr/include/asm/types.h /usr/include/asm-i386/types.h
progressdialog.o: /usr/include/endian.h /usr/include/bits/endian.h
progressdialog.o: /usr/include/sys/select.h /usr/include/bits/select.h
progressdialog.o: /usr/include/bits/sigset.h /usr/include/bits/time.h
progressdialog.o: /usr/include/sys/sysmacros.h
progressdialog.o: /usr/include/bits/pthreadtypes.h /usr/include/stdlib.h
progressdialog.o: /usr/include/bits/waitflags.h
progressdialog.o: /usr/include/bits/waitstatus.h /usr/include/xlocale.h
progressdialog.o: /usr/include/alloca.h /usr/include/string.h
progressdialog.o: /usr/include/strings.h /usr/include/inttypes.h
progressdialog.o: /usr/include/stdint.h /usr/include/ctype.h
progressdialog.o: /usr/include/iconv.h /usr/include/SDL/begin_code.h
progressdialog.o: /usr/include/SDL/close_code.h /usr/include/SDL/SDL_audio.h
progressdialog.o: /usr/include/SDL/SDL_error.h /usr/include/SDL/SDL_endian.h
progressdialog.o: /usr/include/SDL/SDL_mutex.h /usr/include/SDL/SDL_thread.h
progressdialog.o: /usr/include/SDL/SDL_rwops.h /usr/include/SDL/SDL_cdrom.h
progressdialog.o: /usr/include/SDL/SDL_cpuinfo.h
progressdialog.o: /usr/include/SDL/SDL_events.h /usr/include/SDL/SDL_active.h
progressdialog.o: /usr/include/SDL/SDL_keyboard.h
progressdialog.o: /usr/include/SDL/SDL_keysym.h /usr/include/SDL/SDL_mouse.h
progressdialog.o: /usr/include/SDL/SDL_video.h
progressdialog.o: /usr/include/SDL/SDL_joystick.h /usr/include/SDL/SDL_quit.h
progressdialog.o: /usr/include/SDL/SDL_loadso.h /usr/include/SDL/SDL_timer.h
progressdialog.o: /usr/include/SDL/SDL_version.h /usr/include/pthread.h
progressdialog.o: /usr/include/linux/sched.h /usr/include/linux/auxvec.h
progressdialog.o: /usr/include/asm/auxvec.h /usr/include/asm-i386/auxvec.h
progressdialog.o: /usr/include/linux/signal.h /usr/include/asm/signal.h
progressdialog.o: /usr/include/asm-i386/signal.h /usr/include/linux/time.h
progressdialog.o: /usr/include/asm-generic/signal.h
progressdialog.o: /usr/include/asm/siginfo.h /usr/include/asm-i386/siginfo.h
progressdialog.o: /usr/include/asm-generic/siginfo.h
progressdialog.o: /usr/include/bits/setjmp.h ./gui/gui.h ./gui/types.h
progressdialog.o: ./gui/window.h ./gui/widget.h ./gui/box.h ./gui/sdl.h
progressdialog.o: ./gui/label.h ./gui/button.h ./gui/tickbox.h
progressdialog.o: ./gui/radiogroup.h ./gui/radiobutton.h ./gui/slidebar.h
progressdialog.o: ./gui/togglebutton.h ./gui/progressbar.h ./gui/image.h
progressdialog.o: ./gui/graphics.h ./gui/functions.h ./gui/line.h ./gui/log.h
progressdialog.o: ./gui/graphics_colors.h ./gui/graphics_font.h
progressdialog.o: ./gui/graphics_font.h ./gui/graphics_fontmap.h
keyboardmapper.o: /usr/include/string.h /usr/include/features.h
keyboardmapper.o: /usr/include/sys/cdefs.h /usr/include/bits/wordsize.h
keyboardmapper.o: /usr/include/gnu/stubs.h /usr/include/gnu/stubs-32.h
keyboardmapper.o: /usr/include/linux/stddef.h /usr/include/xlocale.h
keyboardmapper.o: /usr/include/ctype.h /usr/include/bits/types.h
keyboardmapper.o: /usr/include/bits/typesizes.h /usr/include/endian.h
keyboardmapper.o: /usr/include/bits/endian.h /usr/include/stdlib.h
keyboardmapper.o: /usr/include/bits/waitflags.h
keyboardmapper.o: /usr/include/bits/waitstatus.h /usr/include/sys/types.h
keyboardmapper.o: /usr/include/linux/time.h /usr/include/linux/types.h
keyboardmapper.o: /usr/include/linux/posix_types.h
keyboardmapper.o: /usr/include/linux/stddef.h /usr/include/asm/posix_types.h
keyboardmapper.o: /usr/include/asm-i386/posix_types.h
keyboardmapper.o: /usr/include/asm/types.h /usr/include/asm-i386/types.h
keyboardmapper.o: /usr/include/sys/select.h /usr/include/bits/select.h
keyboardmapper.o: /usr/include/bits/sigset.h /usr/include/bits/time.h
keyboardmapper.o: /usr/include/sys/sysmacros.h
keyboardmapper.o: /usr/include/bits/pthreadtypes.h /usr/include/alloca.h
keyboardmapper.o: /usr/include/linux/unistd.h /usr/include/asm/unistd.h
keyboardmapper.o: /usr/include/asm-i386/unistd.h /usr/include/stdint.h
keyboardmapper.o: /usr/include/bits/wchar.h /usr/include/stdio.h
keyboardmapper.o: /usr/include/libio.h /usr/include/_G_config.h
keyboardmapper.o: /usr/include/wchar.h /usr/include/gconv.h
keyboardmapper.o: /usr/include/bits/stdio_lim.h
keyboardmapper.o: /usr/include/bits/sys_errlist.h ./gui/log.h ./gui/line.h
keyboardmapper.o: /usr/include/SDL/SDL.h /usr/include/SDL/SDL_main.h
keyboardmapper.o: /usr/include/SDL/SDL_stdinc.h /usr/include/SDL/SDL_config.h
keyboardmapper.o: /usr/include/SDL/SDL_platform.h /usr/include/strings.h
keyboardmapper.o: /usr/include/inttypes.h /usr/include/iconv.h
keyboardmapper.o: /usr/include/SDL/begin_code.h /usr/include/SDL/close_code.h
keyboardmapper.o: /usr/include/SDL/SDL_audio.h /usr/include/SDL/SDL_error.h
keyboardmapper.o: /usr/include/SDL/SDL_endian.h /usr/include/SDL/SDL_mutex.h
keyboardmapper.o: /usr/include/SDL/SDL_thread.h /usr/include/SDL/SDL_rwops.h
keyboardmapper.o: /usr/include/SDL/SDL_cdrom.h /usr/include/SDL/SDL_cpuinfo.h
keyboardmapper.o: /usr/include/SDL/SDL_events.h /usr/include/SDL/SDL_active.h
keyboardmapper.o: /usr/include/SDL/SDL_keyboard.h
keyboardmapper.o: /usr/include/SDL/SDL_keysym.h /usr/include/SDL/SDL_mouse.h
keyboardmapper.o: /usr/include/SDL/SDL_video.h
keyboardmapper.o: /usr/include/SDL/SDL_joystick.h /usr/include/SDL/SDL_quit.h
keyboardmapper.o: /usr/include/SDL/SDL_loadso.h /usr/include/SDL/SDL_timer.h
keyboardmapper.o: /usr/include/SDL/SDL_version.h sysvia.h via.h main.h
keyboardmapper.o: beebwin.h port.h /usr/include/linux/limits.h sdl.h
keyboardmapper.o: disc8271.h video.h beebconfig.h beebconfig_data.h
keyboardmapper.o: beebem_pages.h ./gui/gui.h ./gui/types.h ./gui/window.h
keyboardmapper.o: ./gui/widget.h ./gui/box.h ./gui/sdl.h ./gui/label.h
keyboardmapper.o: ./gui/button.h ./gui/tickbox.h ./gui/radiogroup.h
keyboardmapper.o: ./gui/radiobutton.h ./gui/slidebar.h ./gui/togglebutton.h
keyboardmapper.o: ./gui/progressbar.h ./gui/image.h ./gui/graphics.h
keyboardmapper.o: ./gui/functions.h ./gui/graphics_colors.h
keyboardmapper.o: ./gui/graphics_font.h ./gui/graphics_font.h
keyboardmapper.o: ./gui/graphics_fontmap.h keyboard.h button.h 6502core.h
keyboardmapper.o: screensaver.h
messagebox.o: /usr/include/stdint.h /usr/include/features.h
messagebox.o: /usr/include/sys/cdefs.h /usr/include/bits/wordsize.h
messagebox.o: /usr/include/gnu/stubs.h /usr/include/gnu/stubs-32.h
messagebox.o: /usr/include/bits/wchar.h ./gui/log.h ./gui/line.h
messagebox.o: /usr/include/stdio.h /usr/include/linux/stddef.h
messagebox.o: /usr/include/bits/types.h /usr/include/bits/typesizes.h
messagebox.o: /usr/include/libio.h /usr/include/_G_config.h
messagebox.o: /usr/include/wchar.h /usr/include/gconv.h
messagebox.o: /usr/include/bits/stdio_lim.h /usr/include/bits/sys_errlist.h
messagebox.o: /usr/include/stdlib.h /usr/include/bits/waitflags.h
messagebox.o: /usr/include/bits/waitstatus.h /usr/include/endian.h
messagebox.o: /usr/include/bits/endian.h /usr/include/xlocale.h
messagebox.o: /usr/include/sys/types.h /usr/include/linux/time.h
messagebox.o: /usr/include/linux/types.h /usr/include/linux/posix_types.h
messagebox.o: /usr/include/linux/stddef.h /usr/include/asm/posix_types.h
messagebox.o: /usr/include/asm-i386/posix_types.h /usr/include/asm/types.h
messagebox.o: /usr/include/asm-i386/types.h /usr/include/sys/select.h
messagebox.o: /usr/include/bits/select.h /usr/include/bits/sigset.h
messagebox.o: /usr/include/bits/time.h /usr/include/sys/sysmacros.h
messagebox.o: /usr/include/bits/pthreadtypes.h /usr/include/alloca.h
messagebox.o: /usr/include/string.h main.h beebwin.h /usr/include/SDL/SDL.h
messagebox.o: /usr/include/SDL/SDL_main.h /usr/include/SDL/SDL_stdinc.h
messagebox.o: /usr/include/SDL/SDL_config.h /usr/include/SDL/SDL_platform.h
messagebox.o: /usr/include/strings.h /usr/include/inttypes.h
messagebox.o: /usr/include/ctype.h /usr/include/iconv.h
messagebox.o: /usr/include/SDL/begin_code.h /usr/include/SDL/close_code.h
messagebox.o: /usr/include/SDL/SDL_audio.h /usr/include/SDL/SDL_error.h
messagebox.o: /usr/include/SDL/SDL_endian.h /usr/include/SDL/SDL_mutex.h
messagebox.o: /usr/include/SDL/SDL_thread.h /usr/include/SDL/SDL_rwops.h
messagebox.o: /usr/include/SDL/SDL_cdrom.h /usr/include/SDL/SDL_cpuinfo.h
messagebox.o: /usr/include/SDL/SDL_events.h /usr/include/SDL/SDL_active.h
messagebox.o: /usr/include/SDL/SDL_keyboard.h /usr/include/SDL/SDL_keysym.h
messagebox.o: /usr/include/SDL/SDL_mouse.h /usr/include/SDL/SDL_video.h
messagebox.o: /usr/include/SDL/SDL_joystick.h /usr/include/SDL/SDL_quit.h
messagebox.o: /usr/include/SDL/SDL_loadso.h /usr/include/SDL/SDL_timer.h
messagebox.o: /usr/include/SDL/SDL_version.h port.h
messagebox.o: /usr/include/linux/limits.h sdl.h messagebox.h ./gui/types.h
messagebox.o: ./gui/widget.h ./gui/window.h ./gui/box.h ./gui/sdl.h
messagebox.o: ./gui/label.h ./gui/button.h ./gui/widget_private.h
messagebox.o: messagebox_private.h button.h ./gui/gui.h ./gui/tickbox.h
messagebox.o: ./gui/radiogroup.h ./gui/radiobutton.h ./gui/slidebar.h
messagebox.o: ./gui/togglebutton.h ./gui/progressbar.h ./gui/image.h
messagebox.o: ./gui/graphics.h ./gui/functions.h ./gui/graphics_colors.h
messagebox.o: ./gui/graphics_font.h ./gui/graphics_font.h
messagebox.o: ./gui/graphics_fontmap.h
uefstate.o: /usr/include/stdio.h /usr/include/features.h
uefstate.o: /usr/include/sys/cdefs.h /usr/include/bits/wordsize.h
uefstate.o: /usr/include/gnu/stubs.h /usr/include/gnu/stubs-32.h
uefstate.o: /usr/include/linux/stddef.h /usr/include/bits/types.h
uefstate.o: /usr/include/bits/typesizes.h /usr/include/libio.h
uefstate.o: /usr/include/_G_config.h /usr/include/wchar.h
uefstate.o: /usr/include/bits/wchar.h /usr/include/gconv.h
uefstate.o: /usr/include/bits/stdio_lim.h /usr/include/bits/sys_errlist.h
uefstate.o: 6502core.h port.h /usr/include/linux/limits.h beebwin.h
uefstate.o: /usr/include/string.h /usr/include/xlocale.h
uefstate.o: /usr/include/stdlib.h /usr/include/bits/waitflags.h
uefstate.o: /usr/include/bits/waitstatus.h /usr/include/endian.h
uefstate.o: /usr/include/bits/endian.h /usr/include/sys/types.h
uefstate.o: /usr/include/linux/time.h /usr/include/linux/types.h
uefstate.o: /usr/include/linux/posix_types.h /usr/include/linux/stddef.h
uefstate.o: /usr/include/asm/posix_types.h
uefstate.o: /usr/include/asm-i386/posix_types.h /usr/include/asm/types.h
uefstate.o: /usr/include/asm-i386/types.h /usr/include/sys/select.h
uefstate.o: /usr/include/bits/select.h /usr/include/bits/sigset.h
uefstate.o: /usr/include/bits/time.h /usr/include/sys/sysmacros.h
uefstate.o: /usr/include/bits/pthreadtypes.h /usr/include/alloca.h
uefstate.o: /usr/include/SDL/SDL.h /usr/include/SDL/SDL_main.h
uefstate.o: /usr/include/SDL/SDL_stdinc.h /usr/include/SDL/SDL_config.h
uefstate.o: /usr/include/SDL/SDL_platform.h /usr/include/strings.h
uefstate.o: /usr/include/inttypes.h /usr/include/stdint.h
uefstate.o: /usr/include/ctype.h /usr/include/iconv.h
uefstate.o: /usr/include/SDL/begin_code.h /usr/include/SDL/close_code.h
uefstate.o: /usr/include/SDL/SDL_audio.h /usr/include/SDL/SDL_error.h
uefstate.o: /usr/include/SDL/SDL_endian.h /usr/include/SDL/SDL_mutex.h
uefstate.o: /usr/include/SDL/SDL_thread.h /usr/include/SDL/SDL_rwops.h
uefstate.o: /usr/include/SDL/SDL_cdrom.h /usr/include/SDL/SDL_cpuinfo.h
uefstate.o: /usr/include/SDL/SDL_events.h /usr/include/SDL/SDL_active.h
uefstate.o: /usr/include/SDL/SDL_keyboard.h /usr/include/SDL/SDL_keysym.h
uefstate.o: /usr/include/SDL/SDL_mouse.h /usr/include/SDL/SDL_video.h
uefstate.o: /usr/include/SDL/SDL_joystick.h /usr/include/SDL/SDL_quit.h
uefstate.o: /usr/include/SDL/SDL_loadso.h /usr/include/SDL/SDL_timer.h
uefstate.o: /usr/include/SDL/SDL_version.h sdl.h beebmem.h video.h via.h
uefstate.o: main.h beebsound.h disc8271.h beebconfig.h messagebox.h
uefstate.o: ./gui/types.h ./gui/widget.h ./gui/window.h ./gui/box.h
uefstate.o: ./gui/sdl.h ./gui/label.h ./gui/button.h ./gui/widget_private.h
uefstate.o: progressdialog.h /usr/include/pthread.h
uefstate.o: /usr/include/linux/sched.h /usr/include/linux/auxvec.h
uefstate.o: /usr/include/asm/auxvec.h /usr/include/asm-i386/auxvec.h
uefstate.o: /usr/include/linux/signal.h /usr/include/asm/signal.h
uefstate.o: /usr/include/asm-i386/signal.h /usr/include/linux/time.h
uefstate.o: /usr/include/asm-generic/signal.h /usr/include/asm/siginfo.h
uefstate.o: /usr/include/asm-i386/siginfo.h
uefstate.o: /usr/include/asm-generic/siginfo.h /usr/include/bits/setjmp.h
uefstate.o: ./gui/gui.h ./gui/tickbox.h ./gui/radiogroup.h
uefstate.o: ./gui/radiobutton.h ./gui/slidebar.h ./gui/togglebutton.h
uefstate.o: ./gui/progressbar.h ./gui/image.h ./gui/graphics.h
uefstate.o: ./gui/functions.h ./gui/line.h ./gui/log.h
uefstate.o: ./gui/graphics_colors.h ./gui/graphics_font.h
uefstate.o: ./gui/graphics_font.h ./gui/graphics_fontmap.h messages.h
listbox.o: button.h /usr/include/SDL/SDL.h /usr/include/SDL/SDL_main.h
listbox.o: /usr/include/SDL/SDL_stdinc.h /usr/include/SDL/SDL_config.h
listbox.o: /usr/include/SDL/SDL_platform.h /usr/include/sys/types.h
listbox.o: /usr/include/features.h /usr/include/sys/cdefs.h
listbox.o: /usr/include/bits/wordsize.h /usr/include/gnu/stubs.h
listbox.o: /usr/include/gnu/stubs-32.h /usr/include/bits/types.h
listbox.o: /usr/include/linux/stddef.h /usr/include/bits/typesizes.h
listbox.o: /usr/include/linux/time.h /usr/include/linux/types.h
listbox.o: /usr/include/linux/posix_types.h /usr/include/linux/stddef.h
listbox.o: /usr/include/asm/posix_types.h /usr/include/asm-i386/posix_types.h
listbox.o: /usr/include/asm/types.h /usr/include/asm-i386/types.h
listbox.o: /usr/include/endian.h /usr/include/bits/endian.h
listbox.o: /usr/include/sys/select.h /usr/include/bits/select.h
listbox.o: /usr/include/bits/sigset.h /usr/include/bits/time.h
listbox.o: /usr/include/sys/sysmacros.h /usr/include/bits/pthreadtypes.h
listbox.o: /usr/include/stdio.h /usr/include/libio.h /usr/include/_G_config.h
listbox.o: /usr/include/wchar.h /usr/include/bits/wchar.h
listbox.o: /usr/include/gconv.h /usr/include/bits/stdio_lim.h
listbox.o: /usr/include/bits/sys_errlist.h /usr/include/stdlib.h
listbox.o: /usr/include/bits/waitflags.h /usr/include/bits/waitstatus.h
listbox.o: /usr/include/xlocale.h /usr/include/alloca.h /usr/include/string.h
listbox.o: /usr/include/strings.h /usr/include/inttypes.h
listbox.o: /usr/include/stdint.h /usr/include/ctype.h /usr/include/iconv.h
listbox.o: /usr/include/SDL/begin_code.h /usr/include/SDL/close_code.h
listbox.o: /usr/include/SDL/SDL_audio.h /usr/include/SDL/SDL_error.h
listbox.o: /usr/include/SDL/SDL_endian.h /usr/include/SDL/SDL_mutex.h
listbox.o: /usr/include/SDL/SDL_thread.h /usr/include/SDL/SDL_rwops.h
listbox.o: /usr/include/SDL/SDL_cdrom.h /usr/include/SDL/SDL_cpuinfo.h
listbox.o: /usr/include/SDL/SDL_events.h /usr/include/SDL/SDL_active.h
listbox.o: /usr/include/SDL/SDL_keyboard.h /usr/include/SDL/SDL_keysym.h
listbox.o: /usr/include/SDL/SDL_mouse.h /usr/include/SDL/SDL_video.h
listbox.o: /usr/include/SDL/SDL_joystick.h /usr/include/SDL/SDL_quit.h
listbox.o: /usr/include/SDL/SDL_loadso.h /usr/include/SDL/SDL_timer.h
listbox.o: /usr/include/SDL/SDL_version.h main.h beebwin.h port.h
listbox.o: /usr/include/linux/limits.h sdl.h ./gui/gui.h ./gui/types.h
listbox.o: ./gui/window.h ./gui/widget.h ./gui/box.h ./gui/sdl.h
listbox.o: ./gui/label.h ./gui/button.h ./gui/tickbox.h ./gui/radiogroup.h
listbox.o: ./gui/radiobutton.h ./gui/slidebar.h ./gui/togglebutton.h
listbox.o: ./gui/progressbar.h ./gui/image.h ./gui/graphics.h
listbox.o: ./gui/functions.h ./gui/line.h ./gui/log.h ./gui/graphics_colors.h
listbox.o: ./gui/graphics_font.h ./gui/graphics_font.h
listbox.o: ./gui/graphics_fontmap.h listbox.h keyboard.h progressdialog.h
listbox.o: /usr/include/pthread.h /usr/include/linux/sched.h
listbox.o: /usr/include/linux/auxvec.h /usr/include/asm/auxvec.h
listbox.o: /usr/include/asm-i386/auxvec.h /usr/include/linux/signal.h
listbox.o: /usr/include/asm/signal.h /usr/include/asm-i386/signal.h
listbox.o: /usr/include/linux/time.h /usr/include/asm-generic/signal.h
listbox.o: /usr/include/asm/siginfo.h /usr/include/asm-i386/siginfo.h
listbox.o: /usr/include/asm-generic/siginfo.h /usr/include/bits/setjmp.h
fileselector_saveas.o: /usr/include/string.h /usr/include/features.h
fileselector_saveas.o: /usr/include/sys/cdefs.h /usr/include/bits/wordsize.h
fileselector_saveas.o: /usr/include/gnu/stubs.h /usr/include/gnu/stubs-32.h
fileselector_saveas.o: /usr/include/linux/stddef.h /usr/include/xlocale.h
fileselector_saveas.o: /usr/include/ctype.h /usr/include/bits/types.h
fileselector_saveas.o: /usr/include/bits/typesizes.h /usr/include/endian.h
fileselector_saveas.o: /usr/include/bits/endian.h /usr/include/stdlib.h
fileselector_saveas.o: /usr/include/bits/waitflags.h
fileselector_saveas.o: /usr/include/bits/waitstatus.h
fileselector_saveas.o: /usr/include/sys/types.h /usr/include/linux/time.h
fileselector_saveas.o: /usr/include/linux/types.h
fileselector_saveas.o: /usr/include/linux/posix_types.h
fileselector_saveas.o: /usr/include/linux/stddef.h
fileselector_saveas.o: /usr/include/asm/posix_types.h
fileselector_saveas.o: /usr/include/asm-i386/posix_types.h
fileselector_saveas.o: /usr/include/asm/types.h /usr/include/asm-i386/types.h
fileselector_saveas.o: /usr/include/sys/select.h /usr/include/bits/select.h
fileselector_saveas.o: /usr/include/bits/sigset.h /usr/include/bits/time.h
fileselector_saveas.o: /usr/include/sys/sysmacros.h
fileselector_saveas.o: /usr/include/bits/pthreadtypes.h /usr/include/alloca.h
fileselector_saveas.o: /usr/include/linux/unistd.h /usr/include/asm/unistd.h
fileselector_saveas.o: /usr/include/asm-i386/unistd.h /usr/include/stdint.h
fileselector_saveas.o: /usr/include/bits/wchar.h /usr/include/stdio.h
fileselector_saveas.o: /usr/include/libio.h /usr/include/_G_config.h
fileselector_saveas.o: /usr/include/wchar.h /usr/include/gconv.h
fileselector_saveas.o: /usr/include/bits/stdio_lim.h
fileselector_saveas.o: /usr/include/bits/sys_errlist.h ./gui/log.h
fileselector_saveas.o: ./gui/line.h /usr/include/SDL/SDL.h
fileselector_saveas.o: /usr/include/SDL/SDL_main.h
fileselector_saveas.o: /usr/include/SDL/SDL_stdinc.h
fileselector_saveas.o: /usr/include/SDL/SDL_config.h
fileselector_saveas.o: /usr/include/SDL/SDL_platform.h /usr/include/strings.h
fileselector_saveas.o: /usr/include/inttypes.h /usr/include/iconv.h
fileselector_saveas.o: /usr/include/SDL/begin_code.h
fileselector_saveas.o: /usr/include/SDL/close_code.h
fileselector_saveas.o: /usr/include/SDL/SDL_audio.h
fileselector_saveas.o: /usr/include/SDL/SDL_error.h
fileselector_saveas.o: /usr/include/SDL/SDL_endian.h
fileselector_saveas.o: /usr/include/SDL/SDL_mutex.h
fileselector_saveas.o: /usr/include/SDL/SDL_thread.h
fileselector_saveas.o: /usr/include/SDL/SDL_rwops.h
fileselector_saveas.o: /usr/include/SDL/SDL_cdrom.h
fileselector_saveas.o: /usr/include/SDL/SDL_cpuinfo.h
fileselector_saveas.o: /usr/include/SDL/SDL_events.h
fileselector_saveas.o: /usr/include/SDL/SDL_active.h
fileselector_saveas.o: /usr/include/SDL/SDL_keyboard.h
fileselector_saveas.o: /usr/include/SDL/SDL_keysym.h
fileselector_saveas.o: /usr/include/SDL/SDL_mouse.h
fileselector_saveas.o: /usr/include/SDL/SDL_video.h
fileselector_saveas.o: /usr/include/SDL/SDL_joystick.h
fileselector_saveas.o: /usr/include/SDL/SDL_quit.h
fileselector_saveas.o: /usr/include/SDL/SDL_loadso.h
fileselector_saveas.o: /usr/include/SDL/SDL_timer.h
fileselector_saveas.o: /usr/include/SDL/SDL_version.h sysvia.h via.h main.h
fileselector_saveas.o: beebwin.h port.h /usr/include/linux/limits.h sdl.h
fileselector_saveas.o: disc8271.h video.h beebconfig.h beebconfig_data.h
fileselector_saveas.o: beebem_pages.h ./gui/gui.h ./gui/types.h
fileselector_saveas.o: ./gui/window.h ./gui/widget.h ./gui/box.h ./gui/sdl.h
fileselector_saveas.o: ./gui/label.h ./gui/button.h ./gui/tickbox.h
fileselector_saveas.o: ./gui/radiogroup.h ./gui/radiobutton.h
fileselector_saveas.o: ./gui/slidebar.h ./gui/togglebutton.h
fileselector_saveas.o: ./gui/progressbar.h ./gui/image.h ./gui/graphics.h
fileselector_saveas.o: ./gui/functions.h ./gui/graphics_colors.h
fileselector_saveas.o: ./gui/graphics_font.h ./gui/graphics_font.h
fileselector_saveas.o: ./gui/graphics_fontmap.h keyboard.h button.h
fileselector_saveas.o: 6502core.h screensaver.h
