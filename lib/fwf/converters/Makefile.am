#
#      Makefile definitions for lib/fwf/converters
#


@SET_MAKE@

SHELL = /bin/sh
configdir = @configdir@

#
FWFOBJ  = Pen.o StrToPmap.o choosecol.o long.o strarray.o icon.o
FWFINCL = Converters.i Pen.i StrToPmap.i choosecol.i long.i strarray.i icon.i

include $(configdir)/@common_makefile@

#
#   Makefile definitions
#

noinst_LTLIBRARIES = libfwfconverters.la
AM_CPPFLAGS = -I$(top_srcdir)/include

libfwfconverters_la_SOURCES = Pen.c StrToPmap.c choosecol.c long.c strarray.c icon.c
