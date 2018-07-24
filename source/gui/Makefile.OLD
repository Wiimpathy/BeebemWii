INCLUDES = -I. -I/usr/include/g++
DEPINCLUDES = $(INCLUDES) $(SDINCLUDES) -I/usr/include/linux -I/usr/include/g++

CC = gcc -pg
CFLAGS = -O6 $(INCLUDES) $(SDINCLUDES) -Wall `sdl-config --cflags` -DDATA_DIR=\"/home/max/test/share/beebem\" `sdl-config --cflags` -DEG_SLIDEBAR_ASTROFFONLY=1 
# -DWITH_DEBUG_OUTPUT -DEG_DEBUG

# -mcpu=arm920 -mtune=arm920t -msoft-float -ffast-math -falign-functions=32 -finline-functions -funroll-all-loops `sdl-open2x-config --cflags` -DDATA_DIR=\"/mnt/sd/beebem/\"
LINKFLAGS = -lstdc++ `sdl-config --libs`
SDINCLUDES = -I..
SRCS = sdl.c functions.c log.c window.c widget.c widget_shared.c label.c button.c box.c tickbox.c togglebutton.c \
	radiobutton.c radiogroup.c slidebar.c progressbar.c virtualzone.c graphics.c graphics_font.c \
	graphics_font_iso_8859.c image.c skin.c

OBJS = sdl.o functions.o log.o window.o widget.o widget_shared.o label.o button.o box.o tickbox.o togglebutton.o \
	radiobutton.o radiogroup.o slidebar.o progressbar.o virtualzone.o graphics.o graphics_font.o \
	graphics_font_iso_8859.o image.o skin.o
.cc.o:
	$(CC) -c $(CFLAGS) $<

libeg.a: $(OBJS)
	rm -f libeg.a
	ar rc libeg.a $(OBJS)

depend:
	makedepend -- $(DEPINCLUDES) -- $(SRCS)

clean:
	rm -f $(OBJS) libeg.a

# DO NOT DELETE THIS LINE -- make depend depends on it.

sdl.o: ../gui/sdl.h ../gui/types.h ../gui/line.h ../gui/log.h
sdl.o: /usr/include/stdio.h /usr/include/features.h /usr/include/sys/cdefs.h
sdl.o: /usr/include/bits/wordsize.h /usr/include/gnu/stubs.h
sdl.o: /usr/include/gnu/stubs-32.h /usr/include/linux/stddef.h
sdl.o: /usr/include/bits/types.h /usr/include/bits/typesizes.h
sdl.o: /usr/include/libio.h /usr/include/_G_config.h /usr/include/wchar.h
sdl.o: /usr/include/bits/wchar.h /usr/include/gconv.h
sdl.o: /usr/include/bits/stdio_lim.h /usr/include/bits/sys_errlist.h
sdl.o: /usr/include/stdlib.h /usr/include/sys/types.h
sdl.o: /usr/include/linux/time.h /usr/include/linux/types.h
sdl.o: /usr/include/linux/posix_types.h /usr/include/linux/stddef.h
sdl.o: /usr/include/asm/posix_types.h /usr/include/asm-i386/posix_types.h
sdl.o: /usr/include/asm/types.h /usr/include/asm-i386/types.h
sdl.o: /usr/include/endian.h /usr/include/bits/endian.h
sdl.o: /usr/include/sys/select.h /usr/include/bits/select.h
sdl.o: /usr/include/bits/sigset.h /usr/include/bits/time.h
sdl.o: /usr/include/sys/sysmacros.h /usr/include/bits/pthreadtypes.h
sdl.o: /usr/include/alloca.h /usr/include/string.h ../gui/graphics.h
sdl.o: ../gui/functions.h /usr/include/ctype.h ../gui/graphics_colors.h
sdl.o: ../gui/graphics_font.h graphics_font.h graphics_fontmap.h
functions.o: ../gui/functions.h /usr/include/stdio.h /usr/include/features.h
functions.o: /usr/include/sys/cdefs.h /usr/include/bits/wordsize.h
functions.o: /usr/include/gnu/stubs.h /usr/include/gnu/stubs-32.h
functions.o: /usr/include/linux/stddef.h /usr/include/bits/types.h
functions.o: /usr/include/bits/typesizes.h /usr/include/libio.h
functions.o: /usr/include/_G_config.h /usr/include/wchar.h
functions.o: /usr/include/bits/wchar.h /usr/include/gconv.h
functions.o: /usr/include/bits/stdio_lim.h /usr/include/bits/sys_errlist.h
functions.o: /usr/include/stdlib.h /usr/include/sys/types.h
functions.o: /usr/include/linux/time.h /usr/include/linux/types.h
functions.o: /usr/include/linux/posix_types.h /usr/include/linux/stddef.h
functions.o: /usr/include/asm/posix_types.h
functions.o: /usr/include/asm-i386/posix_types.h /usr/include/asm/types.h
functions.o: /usr/include/asm-i386/types.h /usr/include/endian.h
functions.o: /usr/include/bits/endian.h /usr/include/sys/select.h
functions.o: /usr/include/bits/select.h /usr/include/bits/sigset.h
functions.o: /usr/include/bits/time.h /usr/include/sys/sysmacros.h
functions.o: /usr/include/bits/pthreadtypes.h /usr/include/alloca.h
functions.o: /usr/include/string.h /usr/include/ctype.h ../gui/types.h
functions.o: ../gui/line.h ../gui/log.h
log.o: /usr/include/stdio.h /usr/include/features.h /usr/include/sys/cdefs.h
log.o: /usr/include/bits/wordsize.h /usr/include/gnu/stubs.h
log.o: /usr/include/gnu/stubs-32.h /usr/include/linux/stddef.h
log.o: /usr/include/bits/types.h /usr/include/bits/typesizes.h
log.o: /usr/include/libio.h /usr/include/_G_config.h /usr/include/wchar.h
log.o: /usr/include/bits/wchar.h /usr/include/gconv.h
log.o: /usr/include/bits/stdio_lim.h /usr/include/bits/sys_errlist.h
log.o: /usr/include/stdlib.h /usr/include/sys/types.h
log.o: /usr/include/linux/time.h /usr/include/linux/types.h
log.o: /usr/include/linux/posix_types.h /usr/include/linux/stddef.h
log.o: /usr/include/asm/posix_types.h /usr/include/asm-i386/posix_types.h
log.o: /usr/include/asm/types.h /usr/include/asm-i386/types.h
log.o: /usr/include/endian.h /usr/include/bits/endian.h
log.o: /usr/include/sys/select.h /usr/include/bits/select.h
log.o: /usr/include/bits/sigset.h /usr/include/bits/time.h
log.o: /usr/include/sys/sysmacros.h /usr/include/bits/pthreadtypes.h
log.o: /usr/include/alloca.h /usr/include/linux/errno.h
log.o: /usr/include/asm/errno.h /usr/include/asm-i386/errno.h
log.o: /usr/include/asm-generic/errno.h /usr/include/asm-generic/errno-base.h
log.o: /usr/include/string.h ../gui/log.h ../gui/line.h
window.o: ../gui/log.h ../gui/line.h /usr/include/stdio.h
window.o: /usr/include/features.h /usr/include/sys/cdefs.h
window.o: /usr/include/bits/wordsize.h /usr/include/gnu/stubs.h
window.o: /usr/include/gnu/stubs-32.h /usr/include/linux/stddef.h
window.o: /usr/include/bits/types.h /usr/include/bits/typesizes.h
window.o: /usr/include/libio.h /usr/include/_G_config.h /usr/include/wchar.h
window.o: /usr/include/bits/wchar.h /usr/include/gconv.h
window.o: /usr/include/bits/stdio_lim.h /usr/include/bits/sys_errlist.h
window.o: /usr/include/stdlib.h /usr/include/sys/types.h
window.o: /usr/include/linux/time.h /usr/include/linux/types.h
window.o: /usr/include/linux/posix_types.h /usr/include/linux/stddef.h
window.o: /usr/include/asm/posix_types.h /usr/include/asm-i386/posix_types.h
window.o: /usr/include/asm/types.h /usr/include/asm-i386/types.h
window.o: /usr/include/endian.h /usr/include/bits/endian.h
window.o: /usr/include/sys/select.h /usr/include/bits/select.h
window.o: /usr/include/bits/sigset.h /usr/include/bits/time.h
window.o: /usr/include/sys/sysmacros.h /usr/include/bits/pthreadtypes.h
window.o: /usr/include/alloca.h /usr/include/string.h ../gui/functions.h
window.o: /usr/include/ctype.h ../gui/types.h ../gui/sdl.h ../gui/window.h
window.o: ../gui/widget.h ../gui/window_private.h ../gui/widget_private.h
window.o: /usr/include/stdint.h
widget.o: ../gui/log.h ../gui/line.h /usr/include/stdio.h
widget.o: /usr/include/features.h /usr/include/sys/cdefs.h
widget.o: /usr/include/bits/wordsize.h /usr/include/gnu/stubs.h
widget.o: /usr/include/gnu/stubs-32.h /usr/include/linux/stddef.h
widget.o: /usr/include/bits/types.h /usr/include/bits/typesizes.h
widget.o: /usr/include/libio.h /usr/include/_G_config.h /usr/include/wchar.h
widget.o: /usr/include/bits/wchar.h /usr/include/gconv.h
widget.o: /usr/include/bits/stdio_lim.h /usr/include/bits/sys_errlist.h
widget.o: /usr/include/stdlib.h /usr/include/sys/types.h
widget.o: /usr/include/linux/time.h /usr/include/linux/types.h
widget.o: /usr/include/linux/posix_types.h /usr/include/linux/stddef.h
widget.o: /usr/include/asm/posix_types.h /usr/include/asm-i386/posix_types.h
widget.o: /usr/include/asm/types.h /usr/include/asm-i386/types.h
widget.o: /usr/include/endian.h /usr/include/bits/endian.h
widget.o: /usr/include/sys/select.h /usr/include/bits/select.h
widget.o: /usr/include/bits/sigset.h /usr/include/bits/time.h
widget.o: /usr/include/sys/sysmacros.h /usr/include/bits/pthreadtypes.h
widget.o: /usr/include/alloca.h /usr/include/string.h ../gui/functions.h
widget.o: /usr/include/ctype.h ../gui/types.h ../gui/widget.h
widget.o: ../gui/widget_private.h
widget_shared.o: ../gui/widget_shared.h ../gui/types.h ../gui/log.h
widget_shared.o: ../gui/line.h /usr/include/stdio.h /usr/include/features.h
widget_shared.o: /usr/include/sys/cdefs.h /usr/include/bits/wordsize.h
widget_shared.o: /usr/include/gnu/stubs.h /usr/include/gnu/stubs-32.h
widget_shared.o: /usr/include/linux/stddef.h /usr/include/bits/types.h
widget_shared.o: /usr/include/bits/typesizes.h /usr/include/libio.h
widget_shared.o: /usr/include/_G_config.h /usr/include/wchar.h
widget_shared.o: /usr/include/bits/wchar.h /usr/include/gconv.h
widget_shared.o: /usr/include/bits/stdio_lim.h
widget_shared.o: /usr/include/bits/sys_errlist.h /usr/include/stdlib.h
widget_shared.o: /usr/include/sys/types.h /usr/include/linux/time.h
widget_shared.o: /usr/include/linux/types.h /usr/include/linux/posix_types.h
widget_shared.o: /usr/include/linux/stddef.h /usr/include/asm/posix_types.h
widget_shared.o: /usr/include/asm-i386/posix_types.h /usr/include/asm/types.h
widget_shared.o: /usr/include/asm-i386/types.h /usr/include/endian.h
widget_shared.o: /usr/include/bits/endian.h /usr/include/sys/select.h
widget_shared.o: /usr/include/bits/select.h /usr/include/bits/sigset.h
widget_shared.o: /usr/include/bits/time.h /usr/include/sys/sysmacros.h
widget_shared.o: /usr/include/bits/pthreadtypes.h /usr/include/alloca.h
widget_shared.o: /usr/include/string.h ../gui/functions.h
widget_shared.o: /usr/include/ctype.h ../gui/widget.h ../gui/widget_private.h
widget_shared.o: ../gui/window.h ../gui/window_private.h
label.o: ../gui/log.h ../gui/line.h /usr/include/stdio.h
label.o: /usr/include/features.h /usr/include/sys/cdefs.h
label.o: /usr/include/bits/wordsize.h /usr/include/gnu/stubs.h
label.o: /usr/include/gnu/stubs-32.h /usr/include/linux/stddef.h
label.o: /usr/include/bits/types.h /usr/include/bits/typesizes.h
label.o: /usr/include/libio.h /usr/include/_G_config.h /usr/include/wchar.h
label.o: /usr/include/bits/wchar.h /usr/include/gconv.h
label.o: /usr/include/bits/stdio_lim.h /usr/include/bits/sys_errlist.h
label.o: /usr/include/stdlib.h /usr/include/sys/types.h
label.o: /usr/include/linux/time.h /usr/include/linux/types.h
label.o: /usr/include/linux/posix_types.h /usr/include/linux/stddef.h
label.o: /usr/include/asm/posix_types.h /usr/include/asm-i386/posix_types.h
label.o: /usr/include/asm/types.h /usr/include/asm-i386/types.h
label.o: /usr/include/endian.h /usr/include/bits/endian.h
label.o: /usr/include/sys/select.h /usr/include/bits/select.h
label.o: /usr/include/bits/sigset.h /usr/include/bits/time.h
label.o: /usr/include/sys/sysmacros.h /usr/include/bits/pthreadtypes.h
label.o: /usr/include/alloca.h /usr/include/string.h ../gui/sdl.h
label.o: ../gui/types.h ../gui/functions.h /usr/include/ctype.h
label.o: ../gui/label.h ../gui/window.h ../gui/widget.h
label.o: ../gui/label_private.h ../gui/window_private.h
label.o: ../gui/widget_shared.h ../gui/widget_private.h
button.o: ../gui/log.h ../gui/line.h /usr/include/stdio.h
button.o: /usr/include/features.h /usr/include/sys/cdefs.h
button.o: /usr/include/bits/wordsize.h /usr/include/gnu/stubs.h
button.o: /usr/include/gnu/stubs-32.h /usr/include/linux/stddef.h
button.o: /usr/include/bits/types.h /usr/include/bits/typesizes.h
button.o: /usr/include/libio.h /usr/include/_G_config.h /usr/include/wchar.h
button.o: /usr/include/bits/wchar.h /usr/include/gconv.h
button.o: /usr/include/bits/stdio_lim.h /usr/include/bits/sys_errlist.h
button.o: /usr/include/stdlib.h /usr/include/sys/types.h
button.o: /usr/include/linux/time.h /usr/include/linux/types.h
button.o: /usr/include/linux/posix_types.h /usr/include/linux/stddef.h
button.o: /usr/include/asm/posix_types.h /usr/include/asm-i386/posix_types.h
button.o: /usr/include/asm/types.h /usr/include/asm-i386/types.h
button.o: /usr/include/endian.h /usr/include/bits/endian.h
button.o: /usr/include/sys/select.h /usr/include/bits/select.h
button.o: /usr/include/bits/sigset.h /usr/include/bits/time.h
button.o: /usr/include/sys/sysmacros.h /usr/include/bits/pthreadtypes.h
button.o: /usr/include/alloca.h /usr/include/string.h ../gui/functions.h
button.o: /usr/include/ctype.h ../gui/types.h ../gui/button.h ../gui/sdl.h
button.o: ../gui/window.h ../gui/widget.h ../gui/button_private.h
button.o: ../gui/window_private.h ../gui/widget_private.h
button.o: ../gui/widget_shared.h ../gui/graphics.h ../gui/graphics_colors.h
button.o: ../gui/graphics_font.h graphics_font.h graphics_fontmap.h
box.o: ../gui/types.h ../gui/log.h ../gui/line.h /usr/include/stdio.h
box.o: /usr/include/features.h /usr/include/sys/cdefs.h
box.o: /usr/include/bits/wordsize.h /usr/include/gnu/stubs.h
box.o: /usr/include/gnu/stubs-32.h /usr/include/linux/stddef.h
box.o: /usr/include/bits/types.h /usr/include/bits/typesizes.h
box.o: /usr/include/libio.h /usr/include/_G_config.h /usr/include/wchar.h
box.o: /usr/include/bits/wchar.h /usr/include/gconv.h
box.o: /usr/include/bits/stdio_lim.h /usr/include/bits/sys_errlist.h
box.o: /usr/include/stdlib.h /usr/include/sys/types.h
box.o: /usr/include/linux/time.h /usr/include/linux/types.h
box.o: /usr/include/linux/posix_types.h /usr/include/linux/stddef.h
box.o: /usr/include/asm/posix_types.h /usr/include/asm-i386/posix_types.h
box.o: /usr/include/asm/types.h /usr/include/asm-i386/types.h
box.o: /usr/include/endian.h /usr/include/bits/endian.h
box.o: /usr/include/sys/select.h /usr/include/bits/select.h
box.o: /usr/include/bits/sigset.h /usr/include/bits/time.h
box.o: /usr/include/sys/sysmacros.h /usr/include/bits/pthreadtypes.h
box.o: /usr/include/alloca.h /usr/include/string.h ../gui/functions.h
box.o: /usr/include/ctype.h ../gui/box.h ../gui/sdl.h ../gui/window.h
box.o: ../gui/widget.h ../gui/box_private.h ../gui/widget_shared.h
box.o: ../gui/widget_private.h ../gui/window_private.h ../gui/graphics.h
box.o: ../gui/graphics_colors.h ../gui/graphics_font.h graphics_font.h
box.o: graphics_fontmap.h
tickbox.o: ../gui/log.h ../gui/line.h /usr/include/stdio.h
tickbox.o: /usr/include/features.h /usr/include/sys/cdefs.h
tickbox.o: /usr/include/bits/wordsize.h /usr/include/gnu/stubs.h
tickbox.o: /usr/include/gnu/stubs-32.h /usr/include/linux/stddef.h
tickbox.o: /usr/include/bits/types.h /usr/include/bits/typesizes.h
tickbox.o: /usr/include/libio.h /usr/include/_G_config.h /usr/include/wchar.h
tickbox.o: /usr/include/bits/wchar.h /usr/include/gconv.h
tickbox.o: /usr/include/bits/stdio_lim.h /usr/include/bits/sys_errlist.h
tickbox.o: /usr/include/stdlib.h /usr/include/sys/types.h
tickbox.o: /usr/include/linux/time.h /usr/include/linux/types.h
tickbox.o: /usr/include/linux/posix_types.h /usr/include/linux/stddef.h
tickbox.o: /usr/include/asm/posix_types.h /usr/include/asm-i386/posix_types.h
tickbox.o: /usr/include/asm/types.h /usr/include/asm-i386/types.h
tickbox.o: /usr/include/endian.h /usr/include/bits/endian.h
tickbox.o: /usr/include/sys/select.h /usr/include/bits/select.h
tickbox.o: /usr/include/bits/sigset.h /usr/include/bits/time.h
tickbox.o: /usr/include/sys/sysmacros.h /usr/include/bits/pthreadtypes.h
tickbox.o: /usr/include/alloca.h /usr/include/string.h ../gui/functions.h
tickbox.o: /usr/include/ctype.h ../gui/types.h ../gui/tickbox.h ../gui/sdl.h
tickbox.o: ../gui/window.h ../gui/widget.h ../gui/tickbox_private.h
tickbox.o: ../gui/radiogroup.h ../gui/radiogroup_private.h
tickbox.o: ../gui/window_private.h ../gui/widget_private.h
tickbox.o: ../gui/widget_shared.h ../gui/radiobutton.h ../gui/graphics.h
tickbox.o: ../gui/graphics_colors.h ../gui/graphics_font.h graphics_font.h
tickbox.o: graphics_fontmap.h
togglebutton.o: ../gui/log.h ../gui/line.h /usr/include/stdio.h
togglebutton.o: /usr/include/features.h /usr/include/sys/cdefs.h
togglebutton.o: /usr/include/bits/wordsize.h /usr/include/gnu/stubs.h
togglebutton.o: /usr/include/gnu/stubs-32.h /usr/include/linux/stddef.h
togglebutton.o: /usr/include/bits/types.h /usr/include/bits/typesizes.h
togglebutton.o: /usr/include/libio.h /usr/include/_G_config.h
togglebutton.o: /usr/include/wchar.h /usr/include/bits/wchar.h
togglebutton.o: /usr/include/gconv.h /usr/include/bits/stdio_lim.h
togglebutton.o: /usr/include/bits/sys_errlist.h /usr/include/stdlib.h
togglebutton.o: /usr/include/sys/types.h /usr/include/linux/time.h
togglebutton.o: /usr/include/linux/types.h /usr/include/linux/posix_types.h
togglebutton.o: /usr/include/linux/stddef.h /usr/include/asm/posix_types.h
togglebutton.o: /usr/include/asm-i386/posix_types.h /usr/include/asm/types.h
togglebutton.o: /usr/include/asm-i386/types.h /usr/include/endian.h
togglebutton.o: /usr/include/bits/endian.h /usr/include/sys/select.h
togglebutton.o: /usr/include/bits/select.h /usr/include/bits/sigset.h
togglebutton.o: /usr/include/bits/time.h /usr/include/sys/sysmacros.h
togglebutton.o: /usr/include/bits/pthreadtypes.h /usr/include/alloca.h
togglebutton.o: /usr/include/string.h ../gui/functions.h /usr/include/ctype.h
togglebutton.o: ../gui/types.h ../gui/togglebutton.h ../gui/sdl.h
togglebutton.o: ../gui/window.h ../gui/widget.h ../gui/togglebutton_private.h
togglebutton.o: ../gui/radiogroup.h ../gui/window_private.h
togglebutton.o: ../gui/widget_private.h ../gui/widget_shared.h
togglebutton.o: ../gui/graphics.h ../gui/graphics_colors.h
togglebutton.o: ../gui/graphics_font.h graphics_font.h graphics_fontmap.h
radiobutton.o: ../gui/log.h ../gui/line.h /usr/include/stdio.h
radiobutton.o: /usr/include/features.h /usr/include/sys/cdefs.h
radiobutton.o: /usr/include/bits/wordsize.h /usr/include/gnu/stubs.h
radiobutton.o: /usr/include/gnu/stubs-32.h /usr/include/linux/stddef.h
radiobutton.o: /usr/include/bits/types.h /usr/include/bits/typesizes.h
radiobutton.o: /usr/include/libio.h /usr/include/_G_config.h
radiobutton.o: /usr/include/wchar.h /usr/include/bits/wchar.h
radiobutton.o: /usr/include/gconv.h /usr/include/bits/stdio_lim.h
radiobutton.o: /usr/include/bits/sys_errlist.h /usr/include/stdlib.h
radiobutton.o: /usr/include/sys/types.h /usr/include/linux/time.h
radiobutton.o: /usr/include/linux/types.h /usr/include/linux/posix_types.h
radiobutton.o: /usr/include/linux/stddef.h /usr/include/asm/posix_types.h
radiobutton.o: /usr/include/asm-i386/posix_types.h /usr/include/asm/types.h
radiobutton.o: /usr/include/asm-i386/types.h /usr/include/endian.h
radiobutton.o: /usr/include/bits/endian.h /usr/include/sys/select.h
radiobutton.o: /usr/include/bits/select.h /usr/include/bits/sigset.h
radiobutton.o: /usr/include/bits/time.h /usr/include/sys/sysmacros.h
radiobutton.o: /usr/include/bits/pthreadtypes.h /usr/include/alloca.h
radiobutton.o: /usr/include/string.h ../gui/functions.h /usr/include/ctype.h
radiobutton.o: ../gui/types.h ../gui/radiobutton.h ../gui/sdl.h
radiobutton.o: ../gui/window.h ../gui/widget.h ../gui/tickbox.h
radiobutton.o: ../gui/radiobutton_private.h ../gui/tickbox_private.h
radiobutton.o: ../gui/window_private.h ../gui/widget_private.h
radiogroup.o: ../gui/log.h ../gui/line.h /usr/include/stdio.h
radiogroup.o: /usr/include/features.h /usr/include/sys/cdefs.h
radiogroup.o: /usr/include/bits/wordsize.h /usr/include/gnu/stubs.h
radiogroup.o: /usr/include/gnu/stubs-32.h /usr/include/linux/stddef.h
radiogroup.o: /usr/include/bits/types.h /usr/include/bits/typesizes.h
radiogroup.o: /usr/include/libio.h /usr/include/_G_config.h
radiogroup.o: /usr/include/wchar.h /usr/include/bits/wchar.h
radiogroup.o: /usr/include/gconv.h /usr/include/bits/stdio_lim.h
radiogroup.o: /usr/include/bits/sys_errlist.h /usr/include/stdlib.h
radiogroup.o: /usr/include/sys/types.h /usr/include/linux/time.h
radiogroup.o: /usr/include/linux/types.h /usr/include/linux/posix_types.h
radiogroup.o: /usr/include/linux/stddef.h /usr/include/asm/posix_types.h
radiogroup.o: /usr/include/asm-i386/posix_types.h /usr/include/asm/types.h
radiogroup.o: /usr/include/asm-i386/types.h /usr/include/endian.h
radiogroup.o: /usr/include/bits/endian.h /usr/include/sys/select.h
radiogroup.o: /usr/include/bits/select.h /usr/include/bits/sigset.h
radiogroup.o: /usr/include/bits/time.h /usr/include/sys/sysmacros.h
radiogroup.o: /usr/include/bits/pthreadtypes.h /usr/include/alloca.h
radiogroup.o: /usr/include/string.h ../gui/functions.h /usr/include/ctype.h
radiogroup.o: ../gui/types.h ../gui/radiogroup.h ../gui/widget.h
radiogroup.o: ../gui/window.h ../gui/radiogroup_private.h
radiogroup.o: ../gui/radiobutton.h ../gui/sdl.h ../gui/tickbox.h
radiogroup.o: ../gui/radiobutton_private.h ../gui/tickbox_private.h
radiogroup.o: ../gui/togglebutton.h ../gui/togglebutton_private.h
radiogroup.o: ../gui/window_private.h ../gui/widget_private.h
radiogroup.o: ../gui/widget_shared.h
slidebar.o: ../gui/log.h ../gui/line.h /usr/include/stdio.h
slidebar.o: /usr/include/features.h /usr/include/sys/cdefs.h
slidebar.o: /usr/include/bits/wordsize.h /usr/include/gnu/stubs.h
slidebar.o: /usr/include/gnu/stubs-32.h /usr/include/linux/stddef.h
slidebar.o: /usr/include/bits/types.h /usr/include/bits/typesizes.h
slidebar.o: /usr/include/libio.h /usr/include/_G_config.h
slidebar.o: /usr/include/wchar.h /usr/include/bits/wchar.h
slidebar.o: /usr/include/gconv.h /usr/include/bits/stdio_lim.h
slidebar.o: /usr/include/bits/sys_errlist.h /usr/include/stdlib.h
slidebar.o: /usr/include/sys/types.h /usr/include/linux/time.h
slidebar.o: /usr/include/linux/types.h /usr/include/linux/posix_types.h
slidebar.o: /usr/include/linux/stddef.h /usr/include/asm/posix_types.h
slidebar.o: /usr/include/asm-i386/posix_types.h /usr/include/asm/types.h
slidebar.o: /usr/include/asm-i386/types.h /usr/include/endian.h
slidebar.o: /usr/include/bits/endian.h /usr/include/sys/select.h
slidebar.o: /usr/include/bits/select.h /usr/include/bits/sigset.h
slidebar.o: /usr/include/bits/time.h /usr/include/sys/sysmacros.h
slidebar.o: /usr/include/bits/pthreadtypes.h /usr/include/alloca.h
slidebar.o: /usr/include/string.h ../gui/functions.h /usr/include/ctype.h
slidebar.o: ../gui/types.h ../gui/slidebar.h ../gui/widget.h ../gui/window.h
slidebar.o: ../gui/slidebar_private.h ../gui/window_private.h
slidebar.o: ../gui/widget_private.h ../gui/widget_shared.h ../gui/sdl.h
progressbar.o: ../gui/types.h ../gui/log.h ../gui/line.h /usr/include/stdio.h
progressbar.o: /usr/include/features.h /usr/include/sys/cdefs.h
progressbar.o: /usr/include/bits/wordsize.h /usr/include/gnu/stubs.h
progressbar.o: /usr/include/gnu/stubs-32.h /usr/include/linux/stddef.h
progressbar.o: /usr/include/bits/types.h /usr/include/bits/typesizes.h
progressbar.o: /usr/include/libio.h /usr/include/_G_config.h
progressbar.o: /usr/include/wchar.h /usr/include/bits/wchar.h
progressbar.o: /usr/include/gconv.h /usr/include/bits/stdio_lim.h
progressbar.o: /usr/include/bits/sys_errlist.h /usr/include/stdlib.h
progressbar.o: /usr/include/sys/types.h /usr/include/linux/time.h
progressbar.o: /usr/include/linux/types.h /usr/include/linux/posix_types.h
progressbar.o: /usr/include/linux/stddef.h /usr/include/asm/posix_types.h
progressbar.o: /usr/include/asm-i386/posix_types.h /usr/include/asm/types.h
progressbar.o: /usr/include/asm-i386/types.h /usr/include/endian.h
progressbar.o: /usr/include/bits/endian.h /usr/include/sys/select.h
progressbar.o: /usr/include/bits/select.h /usr/include/bits/sigset.h
progressbar.o: /usr/include/bits/time.h /usr/include/sys/sysmacros.h
progressbar.o: /usr/include/bits/pthreadtypes.h /usr/include/alloca.h
progressbar.o: /usr/include/string.h ../gui/functions.h /usr/include/ctype.h
progressbar.o: ../gui/progressbar.h ../gui/sdl.h ../gui/window.h
progressbar.o: ../gui/widget.h ../gui/progressbar_private.h
progressbar.o: ../gui/widget_shared.h ../gui/widget_private.h
progressbar.o: ../gui/window_private.h
virtualzone.o: ../gui/log.h ../gui/line.h /usr/include/stdio.h
virtualzone.o: /usr/include/features.h /usr/include/sys/cdefs.h
virtualzone.o: /usr/include/bits/wordsize.h /usr/include/gnu/stubs.h
virtualzone.o: /usr/include/gnu/stubs-32.h /usr/include/linux/stddef.h
virtualzone.o: /usr/include/bits/types.h /usr/include/bits/typesizes.h
virtualzone.o: /usr/include/libio.h /usr/include/_G_config.h
virtualzone.o: /usr/include/wchar.h /usr/include/bits/wchar.h
virtualzone.o: /usr/include/gconv.h /usr/include/bits/stdio_lim.h
virtualzone.o: /usr/include/bits/sys_errlist.h /usr/include/stdlib.h
virtualzone.o: /usr/include/sys/types.h /usr/include/linux/time.h
virtualzone.o: /usr/include/linux/types.h /usr/include/linux/posix_types.h
virtualzone.o: /usr/include/linux/stddef.h /usr/include/asm/posix_types.h
virtualzone.o: /usr/include/asm-i386/posix_types.h /usr/include/asm/types.h
virtualzone.o: /usr/include/asm-i386/types.h /usr/include/endian.h
virtualzone.o: /usr/include/bits/endian.h /usr/include/sys/select.h
virtualzone.o: /usr/include/bits/select.h /usr/include/bits/sigset.h
virtualzone.o: /usr/include/bits/time.h /usr/include/sys/sysmacros.h
virtualzone.o: /usr/include/bits/pthreadtypes.h /usr/include/alloca.h
virtualzone.o: /usr/include/string.h ../gui/functions.h /usr/include/ctype.h
virtualzone.o: ../gui/types.h ../gui/virtualzone.h ../gui/sdl.h
virtualzone.o: ../gui/window.h ../gui/widget.h ../gui/virtualzone_private.h
virtualzone.o: ../gui/window_private.h ../gui/widget_private.h
virtualzone.o: ../gui/widget_shared.h
graphics.o: ../gui/graphics.h ../gui/types.h ../gui/functions.h
graphics.o: /usr/include/stdio.h /usr/include/features.h
graphics.o: /usr/include/sys/cdefs.h /usr/include/bits/wordsize.h
graphics.o: /usr/include/gnu/stubs.h /usr/include/gnu/stubs-32.h
graphics.o: /usr/include/linux/stddef.h /usr/include/bits/types.h
graphics.o: /usr/include/bits/typesizes.h /usr/include/libio.h
graphics.o: /usr/include/_G_config.h /usr/include/wchar.h
graphics.o: /usr/include/bits/wchar.h /usr/include/gconv.h
graphics.o: /usr/include/bits/stdio_lim.h /usr/include/bits/sys_errlist.h
graphics.o: /usr/include/stdlib.h /usr/include/sys/types.h
graphics.o: /usr/include/linux/time.h /usr/include/linux/types.h
graphics.o: /usr/include/linux/posix_types.h /usr/include/linux/stddef.h
graphics.o: /usr/include/asm/posix_types.h
graphics.o: /usr/include/asm-i386/posix_types.h /usr/include/asm/types.h
graphics.o: /usr/include/asm-i386/types.h /usr/include/endian.h
graphics.o: /usr/include/bits/endian.h /usr/include/sys/select.h
graphics.o: /usr/include/bits/select.h /usr/include/bits/sigset.h
graphics.o: /usr/include/bits/time.h /usr/include/sys/sysmacros.h
graphics.o: /usr/include/bits/pthreadtypes.h /usr/include/alloca.h
graphics.o: /usr/include/string.h /usr/include/ctype.h ../gui/line.h
graphics.o: ../gui/log.h ../gui/graphics_colors.h ../gui/graphics_font.h
graphics.o: graphics_font.h graphics_fontmap.h ../gui/sdl.h
graphics.o: ../gui/graphics_font_iso_8869.h ../gui/graphics_fontmap.h
graphics_font.o: /usr/include/stdio.h /usr/include/features.h
graphics_font.o: /usr/include/sys/cdefs.h /usr/include/bits/wordsize.h
graphics_font.o: /usr/include/gnu/stubs.h /usr/include/gnu/stubs-32.h
graphics_font.o: /usr/include/linux/stddef.h /usr/include/bits/types.h
graphics_font.o: /usr/include/bits/typesizes.h /usr/include/libio.h
graphics_font.o: /usr/include/_G_config.h /usr/include/wchar.h
graphics_font.o: /usr/include/bits/wchar.h /usr/include/gconv.h
graphics_font.o: /usr/include/bits/stdio_lim.h
graphics_font.o: /usr/include/bits/sys_errlist.h /usr/include/string.h
graphics_font.o: ../gui/graphics_font.h graphics_font.h graphics_fontmap.h
graphics_font.o: ../gui/functions.h /usr/include/stdlib.h
graphics_font.o: /usr/include/sys/types.h /usr/include/linux/time.h
graphics_font.o: /usr/include/linux/types.h /usr/include/linux/posix_types.h
graphics_font.o: /usr/include/linux/stddef.h /usr/include/asm/posix_types.h
graphics_font.o: /usr/include/asm-i386/posix_types.h /usr/include/asm/types.h
graphics_font.o: /usr/include/asm-i386/types.h /usr/include/endian.h
graphics_font.o: /usr/include/bits/endian.h /usr/include/sys/select.h
graphics_font.o: /usr/include/bits/select.h /usr/include/bits/sigset.h
graphics_font.o: /usr/include/bits/time.h /usr/include/sys/sysmacros.h
graphics_font.o: /usr/include/bits/pthreadtypes.h /usr/include/alloca.h
graphics_font.o: /usr/include/ctype.h ../gui/types.h ../gui/line.h
graphics_font.o: ../gui/log.h ../gui/graphics.h ../gui/graphics_colors.h
graphics_font_iso_8859.o: ../gui/graphics_fontmap.h
image.o: ../gui/log.h ../gui/line.h /usr/include/stdio.h
image.o: /usr/include/features.h /usr/include/sys/cdefs.h
image.o: /usr/include/bits/wordsize.h /usr/include/gnu/stubs.h
image.o: /usr/include/gnu/stubs-32.h /usr/include/linux/stddef.h
image.o: /usr/include/bits/types.h /usr/include/bits/typesizes.h
image.o: /usr/include/libio.h /usr/include/_G_config.h /usr/include/wchar.h
image.o: /usr/include/bits/wchar.h /usr/include/gconv.h
image.o: /usr/include/bits/stdio_lim.h /usr/include/bits/sys_errlist.h
image.o: /usr/include/stdlib.h /usr/include/sys/types.h
image.o: /usr/include/linux/time.h /usr/include/linux/types.h
image.o: /usr/include/linux/posix_types.h /usr/include/linux/stddef.h
image.o: /usr/include/asm/posix_types.h /usr/include/asm-i386/posix_types.h
image.o: /usr/include/asm/types.h /usr/include/asm-i386/types.h
image.o: /usr/include/endian.h /usr/include/bits/endian.h
image.o: /usr/include/sys/select.h /usr/include/bits/select.h
image.o: /usr/include/bits/sigset.h /usr/include/bits/time.h
image.o: /usr/include/sys/sysmacros.h /usr/include/bits/pthreadtypes.h
image.o: /usr/include/alloca.h /usr/include/string.h ../gui/sdl.h
image.o: ../gui/types.h ../gui/graphics.h ../gui/functions.h
image.o: /usr/include/ctype.h ../gui/graphics_colors.h ../gui/graphics_font.h
image.o: graphics_font.h graphics_fontmap.h ../gui/image.h ../gui/window.h
image.o: ../gui/widget.h ../gui/image_private.h ../gui/window_private.h
image.o: ../gui/widget_shared.h ../gui/widget_private.h
