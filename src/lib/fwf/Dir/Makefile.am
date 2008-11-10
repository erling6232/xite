#
#      Makefile definitions for lib/fwf/Dir
#


@SET_MAKE@

SHELL = /bin/sh
configdir = @configdir@

#
FWFOBJ  = RegExp.o Directory.o DirMgr.o
FWFINCL = RegExp.i Directory.i DirMgr.i

include $(configdir)/@common_makefile@

#
#   Makefile definitions
#

noinst_LTLIBRARIES = libfwfDir.la

libfwfDir_la_SOURCES = RegExp.c Directory.c DirMgr.c
