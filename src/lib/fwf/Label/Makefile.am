#
#      Makefile definitions for lib/fwf/Label
#


@SET_MAKE@

SHELL = /bin/sh
configdir = @configdir@

#
FWFOBJ  = Label.o
FWFINCL = Label.i LabelP.i
ROFF3   = XfwfLabel.man

include $(configdir)/@common_makefile@

#
#   Makefile definitions
#

noinst_LTLIBRARIES = libfwfLabel.la

libfwfLabel_la_SOURCES = Label.c
