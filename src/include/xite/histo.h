
/*
  This header-file is produced automatically (at least in part) by
  makeHeader of BLAB, Ifi, UiO.
*/


/*

________________________________________________________________

        histo.h
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

#ifndef _HISTO_H_
#define _HISTO_H_

_XITE_CPLUSPLUS_BEGIN

typedef int histogram[256];

#include <xite/biff.h>

extern _XITE_WIN_DLL void combineTransf  _XITE_PARAMS(( histogram h1, histogram h2, histogram h3 ));
extern _XITE_WIN_DLL BiffStatus gammaAdjust  _XITE_PARAMS(( IBAND inband, IBAND outband, double adjust ));
extern _XITE_WIN_DLL BiffStatus histoEq  _XITE_PARAMS(( IBAND b1, IBAND b2, int graylevels ));
#ifdef _COLOR_H_
extern _XITE_WIN_DLL void histoEqCol  _XITE_PARAMS(( IBAND b, Color_tab ct, int graylevels ));
#endif
extern _XITE_WIN_DLL int histoNorm _XITE_PARAMS(( IBAND b1, IBAND b2, double my, double sigma ));
extern _XITE_WIN_DLL int histoSpecify _XITE_PARAMS(( IBAND b1, IBAND b2, histogram histo ));
extern _XITE_WIN_DLL int histoTransf _XITE_PARAMS(( IBAND inband, IBAND outband, histogram hist ));
extern _XITE_WIN_DLL void invertHisto _XITE_PARAMS(( histogram histo, histogram invHisto ));
extern _XITE_WIN_DLL void mkCumHisto _XITE_PARAMS(( histogram histo, histogram cumHisto ));
extern _XITE_WIN_DLL BiffStatus mkIhistogram _XITE_PARAMS(( IBAND band, int histo[], int limits[], int n ));
extern _XITE_WIN_DLL BiffStatus mkDhistogram _XITE_PARAMS(( IBAND band, int histo[], double limits[], int n ));
extern _XITE_WIN_DLL BiffStatus mkHisto _XITE_PARAMS(( IBAND band, histogram histo ));
extern _XITE_WIN_DLL void scaleHisto _XITE_PARAMS(( histogram cumHisto, histogram scHisto, int range ));
#ifdef _COLOR_H_
extern _XITE_WIN_DLL void scaled2col _XITE_PARAMS(( histogram scHisto, Color_tab colTab ));
#endif

_XITE_CPLUSPLUS_END

#endif /* _HISTO_H_ */
