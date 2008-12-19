#
#      Makefile.am for src/zernike
#


@SET_MAKE@

SHELL = /bin/sh
configdir = @configdir@

include $(configdir)/@common_makefile@

#
#   Makefile definitions
#
PROG  = zernike
MANP  = zernike.m
CINCL = zernike.i

bin_PROGRAMS=$(PROG) $(XPROG)

LDADD = libxite.la

zernike_SOURCES = zernike_main.c
