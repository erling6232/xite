#
#      Makefile.am for src/scatter
#


@SET_MAKE@

SHELL = /bin/sh
configdir = @configdir@

include $(configdir)/@common_makefile@

#
#   Makefile definitions
#
PROG  = scatter
MANP  = scatter.m
CINCL =  scatter.i

bin_PROGRAMS=$(PROG) $(XPROG)

LDADD = libxite.la

scatter_SOURCES = scatter_main.c
