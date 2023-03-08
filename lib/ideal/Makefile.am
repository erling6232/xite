#
#      Makefile.am for lib/ideal
#


@SET_MAKE@

SHELL = /bin/sh

include $(top_srcdir)/Makefile.def

#
#   Makefile definitions
#

noinst_LTLIBRARIES = libxiteideal.la

MANF =  highpass.3 bandpass.3 bandstop.3 lowpassu.3 lowpass.3 \
	fftMakeLp.3 fftMakeHp.3 fftMakeBp.3 fftMakeBs.3 highpassf.3 \
	bandpassf.3 bandstopf.3 lowpassIdealf.3 butterworthf.3 \
	exponentialf.3 parseFilterTypeName.3 lowpassf.3 \
	hamming.3 hanning.3 rectangle.3 triangle.3 parseWindowName.3 \
	bartlett.3 window.3

man3_MANS=$(MANF)

libxiteideal_la_SOURCES = lowpass.c highpass.c bandpass.c bandstop.c window.c \
	butterworth.c exponential.c lowpassIdeal.c

highpass.3:	lowpass.3
bandpass.3:	lowpass.3
bandstop.3:	lowpass.3
lowpassu.3:	lowpass.3
fftMakeLp.3:	lowpassf.3
fftMakeHp.3:	lowpassf.3
fftMakeBp.3:	lowpassf.3
fftMakeBs.3:	lowpassf.3
highpassf.3:	lowpassf.3
bandpassf.3:	lowpassf.3
bandstopf.3:	lowpassf.3
lowpassIdealf.3:	lowpassf.3
butterworthf.3:	lowpassf.3
exponentialf.3:	lowpassf.3
parseFilterTypeName.3:	lowpassf.3
lowpassf.3:	lowpass.3
hamming.3:	bartlett.3
hanning.3:	bartlett.3
rectangle.3:	bartlett.3
triangle.3:	bartlett.3
parseWindowName.3:	bartlett.3
bartlett.3:	window.3
