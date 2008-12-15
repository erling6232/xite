#
#      Makefile.am for src/kncn
#


@SET_MAKE@

SHELL = /bin/sh
configdir = @configdir@

include $(configdir)/@common_makefile@

#
#   Makefile definitions
#
PROG  = kncn mbkncn
MANP  = kncn.m mbkncn.m
CINCL =  kncn.i

bin_PROGRAMS=$(PROG) $(XPROG)

LDADD = libxite.la

kncn_SOURCES   = kncn_main.c
mbkncn_SOURCES = mbkncn_main.c
