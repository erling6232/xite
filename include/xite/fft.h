
/*
  This header-file is produced automatically (at least in part) by
  makeHeader of BLAB, Ifi, UiO.
*/


/*

________________________________________________________________

        fft.h
        $Id$
        Copyright 1990, Blab, UiO
        Image processing lab, Department of Informatics
        University of Oslo
        E-mail: blab@ifi.uio.no
________________________________________________________________
  
  Permission to use, copy, modify and distribute this software and its
  documentation for any purpose and without fee is hereby granted, 
  provided that this copyright notice appear in all copies and that 
  both that copyright notice and this permission notice appear in supporting
  documentation and that the name of B-lab, Department of Informatics or
  University of Oslo not be used in advertising or publicity pertaining 
  to distribution of the software without specific, written prior permission.

  B-LAB DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE, INCLUDING ALL
  IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS, IN NO EVENT SHALL B-LAB
  BE LIABLE FOR ANY SPECIAL, INDIRECT OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
  WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION
  OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN 
  CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 

*/

#ifndef _FFT_H_
#define _FFT_H_

_XITE_CPLUSPLUS_BEGIN

#include <xite/biff.h>
#include <xite/ideal.h>
#include <xite/arithmetic.h>
#include <xite/geometry.h>

/* Macros for compatibility with previous versions */

#define fftReal(in, out)  real((IBAND) (in),     (IBAND) (out))
#define fftImag(in, out)  imag((IBAND) (in),     (IBAND) (out))
#define fftPhase(in, out) phase((IBAND) (in),    (IBAND) (out))
#define fftPower(in, out) power((IBAND) (in),    (IBAND) (out), 1.0)
#define fftAmp(in, out)   absValue((IBAND) (in), (IBAND) (out))
#define fftDisplay(in, out, scale) { \
    logarithm(in, out, DOUBLE_MIN, DOUBLE_MAX, scale, -DOUBLE_MAX); \
    shift_band(out, out, Ixsize(out) / 2, Iysize(out) / 2); \
}
#define fftMult(in1, in2, out) \
  multiply((IBAND) (in1), (IBAND) (in2), (IBAND) (out), 1.0)

#define fftMakeLp(b, c, s)    lowpassf((IBAND) (b),  c, s,       HAMMING)
#define fftMakeHp(b, c, s)    highpassf((IBAND) (b), c, &(s),    HAMMING)
#define fftMakeBp(b, l, h, s) bandpassf((IBAND) (b), l, h, s,    HAMMING)
#define fftMakeBs(b, l, h, s) bandstopf((IBAND) (b), l, h, &(s), HAMMING)


extern _XITE_WIN_DLL int fft2d  _XITE_PARAMS(( IBAND in_band, IBAND out_band, int inverse, double scale ));

_XITE_CPLUSPLUS_END

#endif /* _FFT_H_ */
