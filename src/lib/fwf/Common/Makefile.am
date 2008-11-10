#
#      Makefile definitions for lib/fwf/Common
#


@SET_MAKE@

SHELL = /bin/sh
configdir = @configdir@

#
FWFOBJ  = Common.o
FWFINCL = Common.i CommonP.i
ROFF3   = XfwfCommon.man

include $(configdir)/@common_makefile@

#
#   Makefile definitions
#

noinst_LTLIBRARIES = libfwfCommon.la

libfwfCommon_la_SOURCES = Common.c
