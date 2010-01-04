#
#      Makefile.am for src/morph
#


@SET_MAKE@

SHELL = /bin/sh
configdir = @configdir@

include $(configdir)/@common_makefile@

#
#   Makefile definitions
#
PROG  = morphDilate   morphErode   morphOpen   morphClose mkMorph
MANP  = morphDilate.m mkMorph.m
CINCL =  morph.i

bin_PROGRAMS=$(PROG) $(XPROG)
man1_MANS=$(MANP)

LDADD = ../../lib/libxite.la

morphDilate_SOURCES = morphDilate_main.c
morphErode_SOURCES  = morphErode_main.c
morphOpen_SOURCES   = morphOpen_main.c
morphClose_SOURCES  = morphClose_main.c
mkMorph_SOURCES     = mkMorph_main.c
