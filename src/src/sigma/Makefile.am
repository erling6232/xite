#
#      Makefile.am for src/sigma
#


@SET_MAKE@

SHELL = /bin/sh
configdir = @configdir@

include $(configdir)/@common_makefile@

#
#   Makefile definitions
#
PROG  = sigma
MANP  = sigma.m
CINCL =  sigma.i

bin_PROGRAMS=$(PROG) $(XPROG)

LDADD = libxite.la

sigma_SOURCES = sigma_main.c
