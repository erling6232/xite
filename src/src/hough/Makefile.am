#
#      Makefile.am for src/hough
#


@SET_MAKE@

SHELL = /bin/sh
configdir = @configdir@

include $(configdir)/@common_makefile@

#
#   Makefile definitions
#
PROG  = houghCircle houghLine
MANP  = houghCircle.m houghLine.m
CINCL =  hough.i

bin_PROGRAMS=$(PROG) $(XPROG)

LDADD = libxite.la

houghCircle_SOURCES = houghCircle_main.c  
houghLine_SOURCES = houghLine_main.c
