
/*
  This header-file is produced automatically (at least in part) by
  makeHeader of BLAB, Ifi, UiO.
*/


/*

________________________________________________________________

        threshold.h
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

#ifndef _THRESHOLD_H_
#define _THRESHOLD_H_

_XITE_CPLUSPLUS_BEGIN

#include <xite/biff.h>
#include <xite/histo.h>


extern _XITE_WIN_DLL int thresBernsen  _XITE_PARAMS(( IBAND inband, IBAND outband, int cmin, int dx, int dy, int lpv, int hpv, int low ));
extern _XITE_WIN_DLL int thresLloyd  _XITE_PARAMS(( histogram h ));
extern _XITE_WIN_DLL int thresMinErrIter  _XITE_PARAMS(( histogram h, int tinit ));
extern _XITE_WIN_DLL int thresMinErrCrit  _XITE_PARAMS(( histogram h ));
extern _XITE_WIN_DLL int thresMinErrEqVar  _XITE_PARAMS(( histogram h, int tinit ));
extern _XITE_WIN_DLL int thresOtsu  _XITE_PARAMS(( histogram h ));
extern _XITE_WIN_DLL int thresPtile  _XITE_PARAMS(( histogram h, float pTile ));
extern _XITE_WIN_DLL int thresRidCal  _XITE_PARAMS(( histogram h ));
extern _XITE_WIN_DLL int thresSigma  _XITE_PARAMS(( histogram h, double x ));
extern _XITE_WIN_DLL int thresUniErr  _XITE_PARAMS(( IBAND band ));
extern _XITE_WIN_DLL int thresholdSpOut  _XITE_PARAMS(( IBAND b1, IBAND b2, int t, int lpv, int hpv ));
extern _XITE_WIN_DLL int threshold  _XITE_PARAMS(( IBAND b1, IBAND b2, int t ));

_XITE_CPLUSPLUS_END

#endif /* _THRESHOLD_H_ */
