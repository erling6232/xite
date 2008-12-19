#
#      Makefile.am for lib/utils
#


@SET_MAKE@

SHELL = /bin/sh
configdir = @configdir@

include $(configdir)/@common_makefile@

#
#   Makefile definitions
#

noinst_LTLIBRARIES = libxiteutils.la

libxiteutils_la_SOURCES = mallocp.c envfindfile.c readswitch.c \
	tilde_expand.c gaussfunc.c utl_malloc.c message.c compu4.c \
	debug.c strings.c
