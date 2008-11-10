#
#      Makefile definitions for lib/fwf/Group
#


@SET_MAKE@

SHELL = /bin/sh
configdir = @configdir@

#
FWFOBJ  = Group.o
FWFINCL = Group.i GroupP.i
ROFF3   = XfwfGroup.man

include $(configdir)/@common_makefile@

#
#   Makefile definitions
#

noinst_LTLIBRARIES = libfwfGroup.la

libfwfGroup_la_SOURCES = Group.c
