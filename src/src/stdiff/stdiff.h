
/*
  This header-file is produced automatically (at least in part) by
  makeHeader of BLAB, Ifi, UiO.
*/


/*

________________________________________________________________

        stdiff.h
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

#ifndef _STDIFF_H_
#define _STDIFF_H_

_XITE_CPLUSPLUS_BEGIN

#include <xite/biff.h>


extern _XITE_WIN_DLL int maxSimilar3x3  _XITE_PARAMS(( IBAND input, IBAND output ));
extern _XITE_WIN_DLL int mean  _XITE_PARAMS(( IBAND input, IBAND output, int deltax, int deltay ));
extern _XITE_WIN_DLL int stdev  _XITE_PARAMS(( IBAND input, IBAND output, int dx, int dy ));
extern _XITE_WIN_DLL int stdiff  _XITE_PARAMS(( IBAND input, IBAND output, int deltax, int deltay, double alpha, double beta, double m0, double s0 ));
extern _XITE_WIN_DLL int stvar  _XITE_PARAMS(( IBAND input, IBAND output, int deltax, int deltay, double kt, int t ));

_XITE_CPLUSPLUS_END

#endif /* _STDIFF_H_ */
