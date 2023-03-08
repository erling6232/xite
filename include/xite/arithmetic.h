
/*
  This header-file is produced automatically (at least in part) by
  makeHeader of BLAB, Ifi, UiO.
*/


/*C*

________________________________________________________________

        arithmetic.h
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

#ifndef _ARITHMETIC_H_
#define _ARITHMETIC_H_

_XITE_CPLUSPLUS_BEGIN

#include <xite/biff.h>

#define colMax( img, bandno, x) colMaxBand(img[bandno], x)
#define rowMax( img, bandno, x) rowMaxBand(img[bandno], x)
#define colMin( img, bandno, x) colMinBand(img[bandno], x)
#define rowMin( img, bandno, x) rowMinBand(img[bandno], x)
#define colMaxArr( img, bandno, vector, size, min, max) \
  colMaxArrBand(img[bandno], vector, size, min, max)
#define rowMaxArr( img, bandno, vector, size, min, max) \
  rowMaxArrBand(img[bandno], vector, size, min, max)
#define colMinArr( img, bandno, vector, size, min, max) \
  colMinArrBand(img[bandno], vector, size, min, max)
#define rowMinArr( img, bandno, vector, size, min, max) \
  rowMinArrBand(img[bandno], vector, size, min, max)
#define column( img, bandno, col, vector, size, min, max) \
  columnBand(img[bandno], col, vector, size, min, max)
#define row( img, bandno, row, vector, size, min, max) \
  rowBand(img[bandno], row, vector, size, min, max)


extern _XITE_WIN_DLL int absDiff  _XITE_PARAMS(( IBAND input1, IBAND input2, IBAND output ));
extern _XITE_WIN_DLL int addw  _XITE_PARAMS(( IBAND band1, IBAND band2, IBAND output, double w1, double w2, double offset ));
extern _XITE_WIN_DLL int average  _XITE_PARAMS(( IMAGE inimg, ID_BAND outband, double scale ));
extern _XITE_WIN_DLL double colMaxBand  _XITE_PARAMS(( IBAND band, int x ));
extern _XITE_WIN_DLL int colMaxArrBand  _XITE_PARAMS(( IBAND band, double **vector, long *size, double *min, double *max ));
extern _XITE_WIN_DLL double colMinBand  _XITE_PARAMS(( IBAND band, int x ));
extern _XITE_WIN_DLL int colMinArrBand  _XITE_PARAMS(( IBAND band, double **vector, long *size, double *min, double *max ));
extern _XITE_WIN_DLL int columnBand  _XITE_PARAMS(( IBAND band, int col, double **vector, long *size, double *min, double *max ));
extern _XITE_WIN_DLL int curveToBand _XITE_PARAMS(( IBAND band, double *curve, double minval, double maxval, double scale, double cur, double fg, double bg ));
extern _XITE_WIN_DLL int divide  _XITE_PARAMS(( IBAND band1, IBAND band2, IBAND output, double scale ));
extern _XITE_WIN_DLL BiffStatus logarithm _XITE_PARAMS(( IBAND in_band, IBAND out_band, double eps, double offset, double scale, double minimum ));
extern _XITE_WIN_DLL int logical_and  _XITE_PARAMS(( IBAND input1, IBAND input2, IBAND output ));
extern _XITE_WIN_DLL int logical_band  _XITE_PARAMS(( IBAND input1, IBAND input2, IBAND output ));
extern _XITE_WIN_DLL int logical_bandc  _XITE_PARAMS(( IBAND input, IBAND output, UNS_BYTE c ));
extern _XITE_WIN_DLL int logical_bnot  _XITE_PARAMS(( IBAND input, IBAND output ));
extern _XITE_WIN_DLL int logical_bor  _XITE_PARAMS(( IBAND input1, IBAND input2, IBAND output ));
extern _XITE_WIN_DLL int logical_borc  _XITE_PARAMS(( IBAND input, IBAND output, UNS_BYTE c ));
extern _XITE_WIN_DLL int logical_bxor  _XITE_PARAMS(( IBAND input1, IBAND input2, IBAND output ));
extern _XITE_WIN_DLL int logical_bxorc  _XITE_PARAMS(( IBAND input, IBAND output, UNS_BYTE c ));
extern _XITE_WIN_DLL int logical_eq  _XITE_PARAMS(( IBAND input1, IBAND input2, IBAND output ));
extern _XITE_WIN_DLL int logical_eqc  _XITE_PARAMS(( IBAND input, IBAND output, UNS_BYTE c ));
extern _XITE_WIN_DLL int logical_ge  _XITE_PARAMS(( IBAND input1, IBAND input2, IBAND output ));
extern _XITE_WIN_DLL int logical_gec  _XITE_PARAMS(( IBAND input, IBAND output, UNS_BYTE c ));
extern _XITE_WIN_DLL int logical_gt  _XITE_PARAMS(( IBAND input1, IBAND input2, IBAND output ));
extern _XITE_WIN_DLL int logical_gtc  _XITE_PARAMS(( IBAND input, IBAND output, UNS_BYTE c ));
extern _XITE_WIN_DLL int logical_le  _XITE_PARAMS(( IBAND input1, IBAND input2, IBAND output ));
extern _XITE_WIN_DLL int logical_lec  _XITE_PARAMS(( IBAND input, IBAND output, UNS_BYTE c ));
extern _XITE_WIN_DLL int logical_lshift  _XITE_PARAMS(( IBAND input, IBAND output, int n ));
extern _XITE_WIN_DLL int logical_lt  _XITE_PARAMS(( IBAND input1, IBAND input2, IBAND output ));
extern _XITE_WIN_DLL int logical_ltc  _XITE_PARAMS(( IBAND input, IBAND output, UNS_BYTE c ));
extern _XITE_WIN_DLL int logical_mask  _XITE_PARAMS(( IBAND input1, IBAND input2, IBAND output ));
extern _XITE_WIN_DLL int logical_ne  _XITE_PARAMS(( IBAND input1, IBAND input2, IBAND output ));
extern _XITE_WIN_DLL int logical_nec  _XITE_PARAMS(( IBAND input, IBAND output, UNS_BYTE c ));
extern _XITE_WIN_DLL int logical_not  _XITE_PARAMS(( IBAND input, IBAND output ));
extern _XITE_WIN_DLL int logical_or  _XITE_PARAMS(( IBAND input1, IBAND input2, IBAND output ));
extern _XITE_WIN_DLL int logical_rshift  _XITE_PARAMS(( IBAND input, IBAND output, int n ));
extern _XITE_WIN_DLL int logical_xor  _XITE_PARAMS(( IBAND input1, IBAND input2, IBAND output ));
extern _XITE_WIN_DLL int multiply  _XITE_PARAMS(( IBAND band1, IBAND band2, IBAND output, double scale ));
extern _XITE_WIN_DLL int profile  _XITE_PARAMS(( IBAND band, double x, double y, double direction, double step, double dist, int ip, double *arr, int maxlen ));
extern _XITE_WIN_DLL double rms  _XITE_PARAMS(( IBAND band1, IBAND band2, int edge ));
extern _XITE_WIN_DLL int rowBand  _XITE_PARAMS(( IBAND band, int rowno, double **vector, long *size, double *min, double *max ));
extern _XITE_WIN_DLL double rowMaxBand  _XITE_PARAMS(( IBAND band, int y ));
extern _XITE_WIN_DLL int rowMaxArrBand  _XITE_PARAMS(( IBAND band, double **vector, long *size, double *min, double *max ));
extern _XITE_WIN_DLL double rowMinBand  _XITE_PARAMS(( IBAND band, int y ));
extern _XITE_WIN_DLL int rowMinArrBand  _XITE_PARAMS(( IBAND band, double **vector, long *size, double *min, double *max ));
extern _XITE_WIN_DLL int scale  _XITE_PARAMS(( IBAND input, IBAND output, double Scale, double offset ));
extern _XITE_WIN_DLL int signDiff  _XITE_PARAMS(( IBAND input1, IBAND input2, IBAND output, double scale, double offset ));
extern _XITE_WIN_DLL int square  _XITE_PARAMS(( IBAND input, IBAND output, double scale ));
extern _XITE_WIN_DLL int squareRoot  _XITE_PARAMS(( IBAND input, IBAND output, double scale ));
extern _XITE_WIN_DLL int real  _XITE_PARAMS(( IBAND input, IBAND output ));
extern _XITE_WIN_DLL int imag  _XITE_PARAMS(( IBAND input, IBAND output ));
extern _XITE_WIN_DLL int phase  _XITE_PARAMS(( IBAND input, IBAND output ));
extern _XITE_WIN_DLL int power  _XITE_PARAMS(( IBAND input, IBAND output, double scale ));

_XITE_CPLUSPLUS_END

#endif /* _ARITHMETIC_H_ */
