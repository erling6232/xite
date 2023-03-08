#
#      Makefile.am for src/fft
#


@SET_MAKE@

SHELL = /bin/sh

include $(top_srcdir)/Makefile.def

#
#   Makefile definitions
#
PROG    = fft2d fftMagPhase
XPROG   = xfft
#SCRIPTS = fftReal fftImag fftAmp fftPhase fftPower fftMult fftMakeLp \
#	  fftMakeHp fftMakeBp fftMakeBs fftDisplay
MANP    = fft2d.1 xfft.1 fftMagPhase.1
CINCL   = fft.i

bin_PROGRAMS=$(PROG) $(XPROG)
man1_MANS=$(MANP)

LDADD = ../../lib/libxite.la

fft2d_SOURCES       = fft2d_main.c
fftMagPhase_SOURCES = fftMagPhase_main.c
xfft_SOURCES        = xfft_main.c
xfft_LDADD = ../../lib/libxiteX.la $(LDADD) $(LIBGUI)
