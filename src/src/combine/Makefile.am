#
#      Makefile.am for src/color
#


@SET_MAKE@

SHELL = /bin/sh
configdir = @configdir@

include $(configdir)/@common_makefile@

#
#   Makefile definitions
#
PROG  = combine
XPROG   =
MANP  = combine.m
CINCL =  combine.i

bin_PROGRAMS=$(PROG) $(XPROG)

LDADD = libxite.la

combine_SOURCES = combine_main.c
