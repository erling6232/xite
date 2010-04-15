#
#      Makefile.am for src/zernike
#


@SET_MAKE@

SHELL = /bin/sh

include $(top_srcdir)/Makefile.def

#
#   Makefile definitions
#
PROG  = zernike
MANP  = zernike.1
CINCL = zernike.i

bin_PROGRAMS=$(PROG) $(XPROG)
man1_MANS=$(MANP)

LDADD = ../../lib/libxite.la

zernike_SOURCES = zernike_main.c
