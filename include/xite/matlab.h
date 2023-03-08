
/*C*

________________________________________________________________

        matlab.h
        $Id$
        Copyright 1994, Blab, UiO
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

#ifndef _MATLAB_H_
#define _MATLAB_H_

_XITE_CPLUSPLUS_BEGIN

#include <xite/matrix.h>

#undef COMPLEX     /* Defined in <matrix.h> and typedef in <xite/biff.h> */
#undef REAL        /* Defined in <matrix.h> and typedef in <xite/biff.h> */

#include <xite/biff.h>

extern _XITE_MATLAB_WIN_DLL int band2matlab  _XITE_PARAMS(( IBAND band, mxArray *matrix ));
extern _XITE_MATLAB_WIN_DLL int biff2matlab  _XITE_PARAMS(( IMAGE img, long *bandNo, long nbands, mxArray **matrices, char *matFileName ));
extern _XITE_MATLAB_WIN_DLL int matlab2band  _XITE_PARAMS(( const mxArray *matrix, IBAND band ));
extern _XITE_MATLAB_WIN_DLL int matlab2biff  _XITE_PARAMS(( IMAGE img, long *bandNo, long nbands, const mxArray **matrices, char *biffFileName ));
extern _XITE_MATLAB_WIN_DLL int readMatrices  _XITE_PARAMS(( mxArray ***ptrMatrices, long *nMat, char *matNames[], long nNames, char *fileName ));
extern _XITE_MATLAB_WIN_DLL int saveBiffAsMatlab  _XITE_PARAMS(( IMAGE img, char *matFileName ));
extern _XITE_MATLAB_WIN_DLL int saveMatlabAsBiff _XITE_PARAMS(( const mxArray **matrices, long nMat, char *biffFileName, char *historyText ));

_XITE_CPLUSPLUS_END

#endif /* _MATLAB_H_ */
