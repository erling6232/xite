
/*
  This header-file is produced automatically (at least in part) by
  makeHeader of BLAB, Ifi, UiO.
*/


/*C*

________________________________________________________________

        hough.h
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

#ifndef _HOUGH_H_
#define _HOUGH_H_

_XITE_CPLUSPLUS_BEGIN

#include <xite/biff.h>


extern _XITE_WIN_DLL BiffStatus houghCircle  _XITE_PARAMS(( IBAND input, IUS_BAND output, int radius ));
extern _XITE_WIN_DLL BiffStatus houghCircles  _XITE_PARAMS(( IBAND input, IUS_IMAGE output, int rmin, int rmax ));
extern _XITE_WIN_DLL BiffStatus houghLine  _XITE_PARAMS(( IBAND input, IUS_BAND output ));
extern _XITE_WIN_DLL int drawCircle _XITE_PARAMS(( IUS_BAND band, int x, int y, int radius ));
extern _XITE_WIN_DLL int drawLine _XITE_PARAMS(( IBAND band, int theta, int rho ));

_XITE_CPLUSPLUS_END

#endif /* _HOUGH_H_ */
