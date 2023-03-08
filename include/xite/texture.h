
/*
  This header-file is produced automatically (at least in part) by
  makeHeader of BLAB, Ifi, UiO.
*/


/*

________________________________________________________________

        texture.h
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

#ifndef _TEXTURE_H_
#define _TEXTURE_H_

_XITE_CPLUSPLUS_BEGIN

#include <xite/biff.h>


extern _XITE_WIN_DLL double glrlSre  _XITE_PARAMS(( II_BAND inband, int maxlength, int noofruns ));
extern _XITE_WIN_DLL double glrlLre  _XITE_PARAMS(( II_BAND inband, int maxlength, int noofruns ));
extern _XITE_WIN_DLL double glrlGln  _XITE_PARAMS(( II_BAND inband, int maxlength, int noofruns ));
extern _XITE_WIN_DLL double glrlRln  _XITE_PARAMS(( II_BAND inband, int maxlength, int noofruns ));
extern _XITE_WIN_DLL double glrlRp  _XITE_PARAMS(( II_BAND inband, int maxlength, int noofruns, int noofpixels ));
extern _XITE_WIN_DLL double glrlSgre  _XITE_PARAMS(( II_BAND inband, int maxlength, int noofruns ));
extern _XITE_WIN_DLL double glrlLgre  _XITE_PARAMS(( II_BAND inband, int maxlength, int noofruns ));
extern _XITE_WIN_DLL int glRunLength  _XITE_PARAMS(( IBAND input, II_BAND output, int hori, int vert, int average, int noofgl, int *maxlength, int *noofruns ));
extern _XITE_WIN_DLL int glcm  _XITE_PARAMS(( IBAND input, ID_BAND output, int dx, int dy, int nogreylevel ));
extern _XITE_WIN_DLL double glcmAsm  _XITE_PARAMS(( ID_BAND inband ));
extern _XITE_WIN_DLL double glcmCont  _XITE_PARAMS(( ID_BAND inband ));
extern _XITE_WIN_DLL double glcmCorr  _XITE_PARAMS(( ID_BAND inband ));
extern _XITE_WIN_DLL double glcmIdm  _XITE_PARAMS(( ID_BAND inband ));
extern _XITE_WIN_DLL double glcmEntropy  _XITE_PARAMS(( ID_BAND inband ));
extern _XITE_WIN_DLL double glcmVar  _XITE_PARAMS(( ID_BAND inband ));
extern _XITE_WIN_DLL double glcmProm  _XITE_PARAMS(( ID_BAND inband ));
extern _XITE_WIN_DLL double glcmShade  _XITE_PARAMS(( ID_BAND inband ));
extern _XITE_WIN_DLL double glcmInertia  _XITE_PARAMS(( ID_BAND inband ));
extern _XITE_WIN_DLL double glcmDiagmoment  _XITE_PARAMS(( ID_BAND inband ));
extern _XITE_WIN_DLL int peanoScan  _XITE_PARAMS(( IBAND inband, IBAND outband, int blockPow ));

_XITE_CPLUSPLUS_END

#endif /* _TEXTURE_H_ */
