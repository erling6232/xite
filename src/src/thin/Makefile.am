#
#      Makefile.am for src/thin
#


@SET_MAKE@

SHELL = /bin/sh
configdir = @configdir@

include $(configdir)/@common_makefile@

#
#   Makefile definitions
#
PROG	= thn_har thn_zs thn_lc
MANP	= thn_har.m thn_lc.m thn_zs.m 
CINCL   = thin.i

bin_PROGRAMS=$(PROG) $(XPROG)

LDADD = libxite.la

thn_har_SOURCES = thn_har_main.c
thn_zs_SOURCES  = thn_zs_main.c
thn_lc_SOURCES  = thn_lc_main.c
