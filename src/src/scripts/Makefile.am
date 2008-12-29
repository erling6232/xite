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
#SCRIPTS = startProg xiteStdOpt

bin_SCRIPTS=startProg xiteStdOpt

#LDADD = ../../lib/libxite.la

startProg:	startProg.sh
	cp -p $(srcdir)/startProg.sh startProg

xiteStdOpt:	xiteStdOpt.sh
	cp -p $(srcdir)/xiteStdOpt.sh xiteStdOpt
