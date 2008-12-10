#
#      Makefile.am for lib/ideal
#


@SET_MAKE@

SHELL = /bin/sh
configdir = @configdir@

include $(configdir)/@common_makefile@

#
#   Makefile definitions
#

noinst_LTLIBRARIES = libxiteideal.la

libxiteideal_la_SOURCES = lowpass.c highpass.c bandpass.c bandstop.c window.c \
	butterworth.c exponential.c lowpassIdeal.c
