
/*
  This header-file is produced automatically (at least in part) by
  makeHeader of BLAB, Ifi, UiO.
*/


/*

________________________________________________________________

        convolve.h
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

#ifndef _CONVOLVE_H_
#define _CONVOLVE_H_

_XITE_CPLUSPLUS_BEGIN

#include <xite/biff.h>


extern _XITE_WIN_DLL BiffStatus conv3x3  _XITE_PARAMS(( IBAND input, IBAND output, int conv[9], double scale, double offset, int verbose ));
extern _XITE_WIN_DLL BiffStatus convolve  _XITE_PARAMS(( IBAND input, IBAND output, ISS_BAND convk, double scale, double offset, int verbose ));
extern _XITE_WIN_DLL int crossing  _XITE_PARAMS(( IBAND iband, IBAND oband, int t ));
extern _XITE_WIN_DLL int sobel  _XITE_PARAMS(( IBAND input, IBAND magnitude, IBAND angle, IBAND pseudo, int verbose ));
extern _XITE_WIN_DLL int prewitt  _XITE_PARAMS(( IBAND input, IBAND magnitude, IBAND angle, IBAND pseudo, int verbose ));
extern _XITE_WIN_DLL int roberts  _XITE_PARAMS(( IBAND input, IBAND magnitude, IBAND angle, IBAND pseudo, int verbose ));
extern _XITE_WIN_DLL int grad  _XITE_PARAMS(( IBAND input, IBAND magnitude, IBAND angle, IBAND pseudo, int verbose ));
extern _XITE_WIN_DLL int gradient  _XITE_PARAMS(( IBAND input, ISS_BAND convx, ISS_BAND convy, double xscale, double yscale, IBAND magnitude, IBAND angle, IBAND pseudo, int verbose ));
extern _XITE_WIN_DLL BiffStatus lapZeroCross  _XITE_PARAMS(( IBAND iband, IBAND oband, int a, int b, int c, double scale, double offset, int v ));
extern _XITE_WIN_DLL void laplace  _XITE_PARAMS(( IBAND input, IBAND output, int a, int b, int c, double scale, double offset, int verbose ));

_XITE_CPLUSPLUS_END

#endif /* _CONVOLVE_H_ */
