#
#      Makefile.am for src/ideal
#


@SET_MAKE@

SHELL = /bin/sh

include $(top_srcdir)/Makefile.def

#
#   Makefile definitions
#
PROG  = lowpass highpass bandpass bandstop window butterworth exponential \
	lowpassIdeal
MANP  = lowpass.1 window.1 xfilter.1
CINCL = ideal.i
XPROG = xfilter

bin_PROGRAMS=$(PROG) $(XPROG)
man1_MANS=$(MANP)

LDADD = ../../lib/libxite.la

lowpass_SOURCES      = lowpass_main.c
highpass_SOURCES     = highpass_main.c
bandpass_SOURCES     = bandpass_main.c
bandstop_SOURCES     = bandstop_main.c
window_SOURCES       = window_main.c
butterworth_SOURCES  = butterworth_main.c
exponential_SOURCES  = exponential_main.c
lowpassIdeal_SOURCES = lowpassIdeal_main.c
xfilter_SOURCES      = xfilter_main.c
xfilter_LDADD = ../../lib/libxiteX.la $(LDADD) $(LIBGUI)
