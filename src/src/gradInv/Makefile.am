#
#      Makefile.am for src/gradInv
#


@SET_MAKE@

SHELL = /bin/sh
configdir = @configdir@

include $(configdir)/@common_makefile@

#
#   Makefile definitions
#
PROG  = gradInv
MANP  = gradInv.m
CINCL =  gradInv.i

bin_PROGRAMS=$(PROG) $(XPROG)

LDADD = ../../lib/libxite.la

gradInv_SOURCES = gradInv_main.c
