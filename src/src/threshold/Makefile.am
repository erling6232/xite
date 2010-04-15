#
#      Makefile.am for src/threshold
#


@SET_MAKE@

SHELL = /bin/sh

include $(top_srcdir)/Makefile.def

#
#   Makefile definitions
#
PROG  = threshold thresLloyd thresOtsu thresPtile thresRidCal thresBernsen thresSigma thresMinErr thresUniErr
MANP  = threshold.1 thresLloyd.1 thresOtsu.1 thresPtile.1 thresRidCal.1 thresBernsen.1 thresSigma.1 thresMinErr.1 thresUniErr.1
CINCL =  threshold.i

bin_PROGRAMS=$(PROG) $(XPROG)
man1_MANS=$(MANP)

LDADD = ../../lib/libxite.la

threshold_SOURCES    = threshold_main.c
thresLloyd_SOURCES   = thresLloyd_main.c
thresOtsu_SOURCES    = thresOtsu_main.c
thresPtile_SOURCES   = thresPtile_main.c
thresRidCal_SOURCES  = thresRidCal_main.c
thresBernsen_SOURCES = thresBernsen_main.c
thresSigma_SOURCES   = thresSigma_main.c
thresMinErr_SOURCES  = thresMinErr_main.c
thresUniErr_SOURCES  = thresUniErr_main.c
