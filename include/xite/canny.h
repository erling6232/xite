
/*
  This header-file is produced automatically (at least in part) by
  makeHeader of BLAB, Ifi, UiO.
*/


/*

________________________________________________________________

        c.h
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

#ifndef _CANNY_H_
#define _CANNY_H_

_XITE_CPLUSPLUS_BEGIN

#include <xite/biff.h>

#define CANNY_MAXLN       50 	/* maximum filter length */
#define CANNY_MAXWDTH   1024	/* maximum image width */


extern _XITE_WIN_DLL int canny_filters  _XITE_PARAMS(( float sig, int k, int h1[], int h2[], int h3[], int verbose ));
extern _XITE_WIN_DLL double canny_scale  _XITE_PARAMS(( double sig, int k ));
extern _XITE_WIN_DLL int canny  _XITE_PARAMS(( IBAND input, IBAND magnitude, IBAND angle, IBAND laplace, int lngth, int h1[], int h2[], int h3[], double scale, int thin, int verbose ));

_XITE_CPLUSPLUS_END

#endif /* _CANNY_H_ */
