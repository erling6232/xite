#
#      Makefile.am for src/haar
#


@SET_MAKE@

SHELL = /bin/sh

include $(top_srcdir)/Makefile.def

#
#   Makefile definitions
#
PROG  = haar2d haarTexture
MANP  = haar.1 haarTexture.1
CINCL =  haar.i

bin_PROGRAMS=$(PROG) $(XPROG)
man1_MANS=$(MANP)
man3_MANS=haar1d.3 haar2d.3 haar.3 haari1d.3 haari2d.3

LDADD = ../../lib/libxite.la

haar2d_SOURCES = haar2d_main.c
haarTexture_SOURCES = haarTexture_main.c
