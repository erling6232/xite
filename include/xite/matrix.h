

/*C*

________________________________________________________________

        matrix.h
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

#ifndef _XITE_MATRIX_H_
#define _XITE_MATRIX_H_

#include <xite/xite_funcproto.h>

_XITE_CPLUSPLUS_BEGIN

typedef float M_elemtype;

typedef struct _xiteMatrix {
  int rows, cols;
  M_elemtype *elem;
} xiteMatrixRec;

typedef struct _xiteMatrix *xiteMatrix;

extern _XITE_WIN_DLL void MFree _XITE_PARAMS(( xiteMatrix a ));
extern _XITE_WIN_DLL xiteMatrix MData _XITE_PARAMS(( int l, int s, void *data ));
extern _XITE_WIN_DLL xiteMatrix MMult _XITE_PARAMS(( xiteMatrix a, xiteMatrix b ));
extern _XITE_WIN_DLL xiteMatrix MPseudoInv _XITE_PARAMS(( xiteMatrix A ));
 
_XITE_CPLUSPLUS_END
 
#endif /* _XITE_MATRIX_H_ */