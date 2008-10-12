
/*
  This header-file is produced automatically (at least in part) by
  makeHeader of BLAB, Ifi, UiO.
*/


/*

________________________________________________________________

        geometry.h
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

#ifndef _GEOMETRY_H_
#define _GEOMETRY_H_

_XITE_CPLUSPLUS_BEGIN

#include <xite/biff.h>
#include <xite/matrix.h>

extern _XITE_WIN_DLL int affine  _XITE_PARAMS(( IBAND inband, IBAND outband, double x0, double x1, double x2, double y0, double y1, double y2, int ip, int bg ));
extern _XITE_WIN_DLL int quadratic  _XITE_PARAMS(( IBAND inband, IBAND outband, double *xa, double *ya, int ip, int bg ));
extern _XITE_WIN_DLL int rotate  _XITE_PARAMS(( IBAND inband, IBAND outband, double theta, double rx, double ry, int ip, int bg ));
extern _XITE_WIN_DLL int shift_band  _XITE_PARAMS(( IBAND in_band, IBAND out_band, int shiftx, int shifty ));
extern _XITE_WIN_DLL int transpose_band  _XITE_PARAMS(( IBAND I ));
extern _XITE_WIN_DLL int warp _XITE_PARAMS(( IMAGE inimg, IMAGE outimg, IBAND points, int npoint, int ncoeffs, int ip, int bg, int rss, int qr ));
extern _XITE_WIN_DLL int warp_resample _XITE_PARAMS(( IMAGE inimg, IMAGE outimg, double *xxa, double *yya, int ncoeffs, int ip, int bg ));
extern _XITE_WIN_DLL int oiWarpMapping _XITE_PARAMS(( xiteMatrix *M, xiteMatrix *Mp, xiteMatrix *Q, xiteMatrix *R, xiteMatrix *K, xiteMatrix *L, IBAND points, int npoints, int ncoeffs ));

_XITE_CPLUSPLUS_END

#endif /* _GEOMETRY_H_ */
