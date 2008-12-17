#
#      Makefile.am for src/resample
#


@SET_MAKE@

SHELL = /bin/sh
configdir = @configdir@

include $(configdir)/@common_makefile@

#
#   Makefile definitions
#
PROG  = resample
MANP  = resample.m
CINCL =  resample.i

bin_PROGRAMS=$(PROG) $(XPROG)

LDADD = libxite.la

resample_SOURCES = resample_main.c
