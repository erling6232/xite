

/*C*

________________________________________________________________

        zernike.h
        $Id$
        Copyright 1996, Blab, UiO
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

#ifndef _ZERNIKE_H_
#define _ZERNIKE_H_

_XITE_CPLUSPLUS_BEGIN

#ifndef NULL
#define NULL 0
#endif

#define ZER_INFINITY 1.223e10
#define ZER_TINY 1e-6

#define ZER_NO_MEMORY -10
#define ZER_ERROR -1
#define ZER_OK 0



extern _XITE_WIN_DLL int zer_mom _XITE_PARAMS((IBAND inband, int n, int m, DCOMPLEX *res));
extern _XITE_WIN_DLL int zer_con _XITE_PARAMS((int n, IBAND inband, IBAND outband));
extern _XITE_WIN_DLL int zer_rec _XITE_PARAMS((int order, IBAND inband, IBAND outband));
extern _XITE_WIN_DLL int zer_pol _XITE_PARAMS((int n, int m, double x, double y, DCOMPLEX *res));


_XITE_CPLUSPLUS_END

#endif /* _ZERNIKE_H_ */
