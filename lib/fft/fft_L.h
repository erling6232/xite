
/*
  This header-file is produced automatically (at least in part) by
  makeHeader of BLAB, Ifi, UiO.
*/


/*

________________________________________________________________

        fft_L.h
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

#ifndef _FFT_L_H_
#define _FFT_L_H_

#include <xite/includes.h>

_XITE_CPLUSPLUS_BEGIN

/* ======================================================================
  copyright 1990  Richard H. Krukar all rights reserved

        Permission granted to buy, sell, or steal this software is granted.
    The author retains the right to distribute this software freely, but
    is not responsible for it's quality or maintainance.
 */

#define TLEN (4096)
#define UNROL (128)

#ifndef PI
# define PI (3.141592653589793238462643)
#endif

/* ======================================================================= */

struct complex_ri { float r,i; };

extern void dintime  _XITE_PARAMS(( struct complex_ri *x, int length, float *wtab ));
extern void bitrev  _XITE_PARAMS(( float *x, int length ));
extern void bitrev512  _XITE_PARAMS(( float x[], int length ));
extern void idintime  _XITE_PARAMS(( struct complex_ri x[], int length, float *wtab ));
extern void dint  _XITE_PARAMS(( float *x, int length, float *wtab ));
extern void fft  _XITE_PARAMS(( float x[], int length ));
extern void idint  _XITE_PARAMS(( float *x, int length, float *wtab ));

_XITE_CPLUSPLUS_END

#endif /* _FFT_H_ */
