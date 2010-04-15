#
#      Makefile.am for src/ihs
#


@SET_MAKE@

SHELL = /bin/sh

include $(top_srcdir)/Makefile.def

#
#   Makefile definitions
#
MANP_SH = cxite.1 xiteStdOpt.1
#SCRIPTS = startProg xiteStdOpt

bin_SCRIPTS=startProg xiteStdOpt
man1_MANS=$(MANP_SH)

#LDADD = ../../lib/libxite.la

startProg:	startProg.sh
	cp -p $(srcdir)/startProg.sh startProg

xiteStdOpt:	xiteStdOpt.sh
	cp -p $(srcdir)/xiteStdOpt.sh xiteStdOpt
