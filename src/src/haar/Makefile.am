#
#      Makefile.am for src/haar
#


@SET_MAKE@

SHELL = /bin/sh
configdir = @configdir@

include $(configdir)/@common_makefile@

#
#   Makefile definitions
#
PROG  = haar2d haarTexture
MANP  = haar.m haarTexture.m
CINCL =  haar.i

bin_PROGRAMS=$(PROG) $(XPROG)

LDADD = ../../lib/libxite.la

haar2d_SOURCES = haar2d_main.c
haarTexture_SOURCES = haarTexture_main.c
