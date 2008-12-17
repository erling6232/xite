#
#      Makefile.am for src/ihs
#


@SET_MAKE@

SHELL = /bin/sh
configdir = @configdir@

include $(configdir)/@common_makefile@

#
#   Makefile definitions
#
MANP_SH = cxite.m xiteStdOpt.m
SCRIPTS = startProg xiteStdOpt

bin_PROGRAMS=$(PROG) $(XPROG)

LDADD = libxite.la
