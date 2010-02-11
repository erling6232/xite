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
MANP  = ihs.m ihs2rgb.m rainbow.m rgb2ihs.m
CINCL =  ihs.i

bin_PROGRAMS=$(PROG) $(XPROG)
man1_MANS=$(MANP)

LDADD = ../../lib/libxite.la

ihs2rgb_SOURCES = ihs2rgb_main.c
rainbow_SOURCES = rainbow_main.c
rgb2ihs_SOURCES = rgb2ihs_main.c
