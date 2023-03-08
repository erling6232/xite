#
#      Makefile.am for src/stdiff
#


@SET_MAKE@

SHELL = /bin/sh

include $(top_srcdir)/Makefile.def

#
#   Makefile definitions
#
PROG  = maxSimilar3x3 mean stdev stdiff stvar
MANP  = maxSimilar3x3.1 mean.1 stdev.1 stdiff.1 stvar.1
CINCL =  stdiff.i

bin_PROGRAMS=$(PROG) $(XPROG)
man1_MANS=$(MANP)

LDADD = ../../lib/libxite.la

maxSimilar3x3_SOURCES = maxSimilar3x3_main.c
mean_SOURCES          = mean_main.c
stdev_SOURCES         = stdev_main.c
stdiff_SOURCES        = stdiff_main.c
stvar_SOURCES         = stvar_main.c
