#
#      Makefile.am for src/ximage
#


@SET_MAKE@

SHELL = /bin/sh

include $(top_srcdir)/Makefile.def

#
#   Makefile definitions
#
XPROG  = xhistogram   xmovie   ximage_example
MANP   = xhistogram.m xmovie.m ximage_example.m
CINCL  = Cmap.i Histogram.i ImageForm.i ImageOptionsMenu.i ImageToolsMenu.i \
	 ImageVisualsMenu.i ShellWids.i Visual.i WidUtils.i Xcolor.i \
	 Xdialog.i FormDialog.i Xhistogram.i XmenuBar.i Xpty.i Xslice.i \
	 layout.i ximage_display.i callbacks.i logPos.i ximage.i

bin_PROGRAMS=$(PROG) $(XPROG)
man1_MANS=$(MANP)

LDADD = ../../lib/libxiteX.la ../../lib/libxite.la $(LIBGUI)

xhistogram_SOURCES     = xhistogram_main.c
xmovie_SOURCES         = xmovie_main.c
ximage_example_SOURCES = ximage_example_main.c
