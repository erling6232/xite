
/*
  This header-file is produced automatically (at least in part) by
  makeHeader of BLAB, Ifi, UiO.
*/


/*

________________________________________________________________

        statistics.h
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

#ifndef _STATISTICS_H_
#define _STATISTICS_H_

_XITE_CPLUSPLUS_BEGIN

#include <xite/biff.h>
#include <xite/histo.h>


extern _XITE_WIN_DLL int band_minmax  _XITE_PARAMS(( IBAND band, double *minval, double *maxval ));
extern _XITE_WIN_DLL int band_minmax_disp  _XITE_PARAMS(( IBAND band, float *minval, float *maxval ));
extern _XITE_WIN_DLL int histoMeanStd  _XITE_PARAMS(( IBAND b1, IBAND b2, double mean, double std ));
extern _XITE_WIN_DLL int hstatistics  _XITE_PARAMS(( histogram histo, int *min, int *max, int *median, double *mean, double *stdev ));
extern _XITE_WIN_DLL int statistics  _XITE_PARAMS(( IBAND b, int *min, int *max, int *median, double *mean, double *stdev ));
extern _XITE_WIN_DLL int statisticMask  _XITE_PARAMS(( IBAND band, IBAND mask, long *size, double *min, double *max, double *mean, double *stdev, int maskval ));
extern _XITE_WIN_DLL int statisticMasks _XITE_PARAMS(( IBAND band, IBAND mask, long *size, double *min, double *max, double *mean, double *stdev ));
extern _XITE_WIN_DLL int statistic  _XITE_PARAMS(( IBAND band, long *size, double *min, double *max, double *mean, double *stdev ));
extern _XITE_WIN_DLL int cstatistic  _XITE_PARAMS(( IBAND band, long *size, double *min, double *max, DCOMPLEX *mean, double *stdev ));

_XITE_CPLUSPLUS_END

#endif /* _STATISTICS_H_ */
