#
#      Makefile.am for src/matlab
#


@SET_MAKE@

SHELL = /bin/sh

include $(top_srcdir)/Makefile.def

#
#   Makefile definitions
#
#MATOBJ  = biff2matlab.o matlab2biff.o             # Objects using MATLAB lib
MATPROG = biff2matlab matlab2biff                 # Programs using MATLAB lib
MATMEX  = loadBiff saveBiff                       # MATLAB mex-files
MATHELP = loadBiff.m saveBiff.m                   # MATLAB .m help-file
CINCL	= matlab.i
MANP	= biff2matlab.1 matlab2biff.1

bin_PROGRAMS=$(MATPROG) $(XPROG)
man1_MANS=$(MANP)

LDADD = ../../lib/libxite.la

biff2matlab_SOURCES = biff2matlab_main.c
matlab2biff_SOURCES = matlab2biff_main.c
