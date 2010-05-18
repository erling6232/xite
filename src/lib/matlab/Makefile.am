#
#      Makefile.am for lib/matlab
#


@SET_MAKE@

SHELL = /bin/sh

include $(top_srcdir)/Makefile.def

#
#   Makefile definitions
#

noinst_LTLIBRARIES = libxitematlab.la

MANF =  band2matlab.3 biff2matlab.3 saveBiffAsMatlab.3 \
	matlab2band.3 matlab2biff.3 saveMatlabAsBiff.3 readMatrices.3

man3_MANS=$(MANF)

libxitematlab_la_SOURCES = biff2matlab.c matlab2biff.c

band2matlab.3:	biff2matlab.3
saveBiffAsMatlab.3:	biff2matlab.3
matlab2band.3:	matlab2biff.3
saveMatlabAsBiff.3:	matlab2biff.3
readMatrices.3:	matlab2biff.3
