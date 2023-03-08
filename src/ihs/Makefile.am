#
#      Makefile.am for src/ihs
#


@SET_MAKE@

SHELL = /bin/sh

include $(top_srcdir)/Makefile.def

#
#   Makefile definitions
#
PROG  = ihs2rgb rainbow rgb2ihs
MANP  = ihs2rgb.1 rainbow.1 rgb2ihs.1
CINCL =  ihs.i

bin_PROGRAMS=$(PROG) $(XPROG)
man1_MANS=$(MANP)

LDADD = ../../lib/libxite.la

ihs2rgb_SOURCES = ihs2rgb_main.c
rainbow_SOURCES = rainbow_main.c
rgb2ihs_SOURCES = rgb2ihs_main.c
