#
#      Makefile definitions for lib/fwf/Prompt
#


@SET_MAKE@

SHELL = /bin/sh
configdir = @configdir@

#
FWFOBJ  = Prompt.o
FWFINCL = Prompt.i PromptP.i
ROFF3   = XfwfPrompt.man

include $(configdir)/@common_makefile@

#
#   Makefile definitions
#

noinst_LTLIBRARIES = libfwfPrompt.la
AM_CPPFLAGS = -I$(top_srcdir)/include

libfwfPrompt_la_SOURCES = Prompt.c
