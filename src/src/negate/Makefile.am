#
#      Makefile.am for src/negate
#


@SET_MAKE@

SHELL = /bin/sh
configdir = @configdir@

include $(configdir)/@common_makefile@

#
#   Makefile definitions
#
PROG  = negate absValue complexConjug
MANP  = negate.m absValue.m complexConjug.m
CINCL =  negate.i

bin_PROGRAMS=$(PROG) $(XPROG)

LDADD = libxite.la

negate_SOURCES        = negate_main.c
absValue_SOURCES      = absValue_main.c
complexConjug_SOURCES = complexConjug_main.c
