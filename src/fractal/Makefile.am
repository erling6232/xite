#
#      Makefile.am for src/fractal
#


@SET_MAKE@

SHELL = /bin/sh

include $(top_srcdir)/Makefile.def

#
#   Makefile definitions
#
PROG  = minmaxTexture mkFractalSra
XPROG =
MANP  = minmaxTexture.1 mkFractalSra.1
CINCL = fractal.i

bin_PROGRAMS=$(PROG) $(XPROG)
man1_MANS=$(MANP)

LDADD = ../../lib/libxite.la

minmaxTexture_SOURCES = minmaxTexture_main.c
mkFractalSra_SOURCES  = mkFractalSra_main.c
