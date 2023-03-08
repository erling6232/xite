
/*
  This header-file is produced automatically (at least in part) by
  makeHeader of BLAB, Ifi, UiO.
*/


/*

________________________________________________________________

        thresMl.h
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

#ifndef _THRESML_H_
#define _THRESML_H_

_XITE_CPLUSPLUS_BEGIN

#include <xite/biff.h>
#include <xite/histo.h>


extern _XITE_WIN_DLL int thresMlAppScale  _XITE_PARAMS(( IBAND inband, IBAND outband, int *thresholds, int nt, double scale ));
extern _XITE_WIN_DLL int thresMlAppMean  _XITE_PARAMS(( IBAND inband, IBAND outband, int *thresholds, int nt, histogram histo ));
extern _XITE_WIN_DLL int thresMlAppMedian  _XITE_PARAMS(( IBAND inband, IBAND outband, int *thresholds, int nt, histogram histo ));
extern _XITE_WIN_DLL int histoCentroid  _XITE_PARAMS(( histogram hin, histogram hout, int m, int method ));
extern _XITE_WIN_DLL int thresMlCentroid  _XITE_PARAMS(( histogram histo, histogram thres, int m, int method, int *nt ));
extern _XITE_WIN_DLL int mkComCur  _XITE_PARAMS(( IBAND band, histogram cc ));
extern _XITE_WIN_DLL int thresMlComCur  _XITE_PARAMS(( IBAND band, int *thresholds, int nt ));
extern _XITE_WIN_DLL int thresMlReddi  _XITE_PARAMS(( histogram h, int *t, int nt ));
extern _XITE_WIN_DLL int thresMlWaHa  _XITE_PARAMS(( IBAND gray, IBAND tri, IBAND edge, int thVect[], int edgeTh, int numTh, int population, int n, int *totTh ));

_XITE_CPLUSPLUS_END

#endif /* _THRESML_H_ */
