
/*
  This header-file is produced automatically (at least in part) by
  makeHeader of BLAB, Ifi, UiO.
*/


/*

________________________________________________________________

        median.h
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

#ifndef _MEDIAN_H_
#define _MEDIAN_H_

_XITE_CPLUSPLUS_BEGIN

#include <xite/biff.h>


extern _XITE_WIN_DLL int fractile  _XITE_PARAMS(( IBAND input, IBAND output, int dx, int dy, double frac ));
extern _XITE_WIN_DLL int fractile_mask  _XITE_PARAMS(( IBAND inband, IBAND outband, ISS_BAND mask, int mirror, double fractile ));
extern _XITE_WIN_DLL int lit  _XITE_PARAMS(( IBAND input, IBAND output, int dx, int dy, int dp ));
extern _XITE_WIN_DLL int maxHisto  _XITE_PARAMS(( IBAND input, IBAND output, int nx, int ny ));
extern _XITE_WIN_DLL int median  _XITE_PARAMS(( IBAND input, IBAND output, int dx, int dy ));
extern _XITE_WIN_DLL int mode  _XITE_PARAMS(( IBAND input, IBAND output, int dx, int dy ));
extern _XITE_WIN_DLL int pseudomedian3x3  _XITE_PARAMS(( IBAND input, IBAND output ));
extern _XITE_WIN_DLL int rank  _XITE_PARAMS(( IBAND input, IBAND output, int rank, int dx, int dy ));

_XITE_CPLUSPLUS_END

#endif /* _MEDIAN_H_ */
