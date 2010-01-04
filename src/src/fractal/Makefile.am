#
#      Makefile.am for src/fractal
#


@SET_MAKE@

SHELL = /bin/sh
configdir = @configdir@

include $(configdir)/@common_makefile@

#
#   Makefile definitions
#
PROG  = minmaxTexture mkFractalSra
XPROG =
MANP  = minmaxTexture.m mkFractalSra.m
CINCL = fractal.i

bin_PROGRAMS=$(PROG) $(XPROG)
man1_MANS=$(MANP)

LDADD = ../../lib/libxite.la

minmaxTexture_SOURCES = minmaxTexture_main.c
mkFractalSra_SOURCES  = mkFractalSra_main.c
