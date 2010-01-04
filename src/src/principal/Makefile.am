#
#      Makefile.am for src/principal
#


@SET_MAKE@

SHELL = /bin/sh
configdir = @configdir@

include $(configdir)/@common_makefile@

#
#   Makefile definitions
#
PROG  = linearTrans
MANP  = linearTrans.m
CINCL =  principal.i

bin_PROGRAMS=$(PROG) $(XPROG)
man1_MANS=$(MANP)

LDADD = ../../lib/libxite.la

linearTrans_SOURCES = linearTrans_main.c
