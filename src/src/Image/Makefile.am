#
#      Makefile.am for src/Image
#


@SET_MAKE@

SHELL = /bin/sh
configdir = @configdir@

include $(configdir)/@common_makefile@

#
#   Makefile definitions
#
XOBJ   = Image.o ImageFormat.o ImageBytePix.o ImageSignedBytePix.o \
	 ImageShortPix.o ImageUnsignedShortPix.o ImageLongPix.o \
	 ImageFloatPix.o ImageDoublePix.o ImageOverlay.o \
	 ImageOverlayPix.o ImageComplexPix.o ImageDoubleComplexPix.o
MANP   = Image.m ImageProtoPix.m ImageFormat.m ImageOverlay.m
CINCL  = Image.i ImageP.i ImageOverlay.i ImageFormat.i ImageOverlayP.i

bin_PROGRAMS=$(PROG) $(XPROG)

LDADD = libxite.la

lowpass_SOURCES      = lowpass_main.c
highpass_SOURCES     = highpass_main.c
bandpass_SOURCES     = bandpass_main.c
bandstop_SOURCES     = bandstop_main.c
window_SOURCES       = window_main.c
butterworth_SOURCES  = butterworth_main.c
exponential_SOURCES  = exponential_main.c
lowpassIdeal_SOURCES = lowpassIdeal_main.c
