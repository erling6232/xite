
/*
  This header-file is produced automatically (at least in part) by
  makeHeader of BLAB, Ifi, UiO.
*/


/*C*

________________________________________________________________

        tiff.h
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

#ifndef _TIFF_H_
#define _TIFF_H_

_XITE_CPLUSPLUS_BEGIN

#include <xite/biff.h>
#include <tiffio.h>

#define BW        0
#define GRAYSCALE 1
#define PALETTE   2
#define RGB       3


extern _XITE_TIFF_WIN_DLL int biff2tiff  _XITE_PARAMS(( IMAGE img, TIFF *tif, int format, Color_cell *coltab, int colLen, uint16 compression ));
extern _XITE_TIFF_WIN_DLL int biff2tiff1  _XITE_PARAMS(( IBAND band, TIFF *tif, uint16 compression, UNS_BYTE threshold ));
extern _XITE_TIFF_WIN_DLL int biff2tiff16  _XITE_PARAMS(( IUS_BAND band, TIFF *tif, uint16 compression ));
extern _XITE_TIFF_WIN_DLL int biff2tiff16c  _XITE_PARAMS(( IUS_IMAGE img, TIFF *tif, uint16 compression ));
extern _XITE_TIFF_WIN_DLL int biff2tiff16s  _XITE_PARAMS(( ISS_BAND band, TIFF *tif, uint16 compression ));
extern _XITE_TIFF_WIN_DLL int biff2tiff16sc  _XITE_PARAMS(( ISS_IMAGE img, TIFF *tif, uint16 compression ));
extern _XITE_TIFF_WIN_DLL int biff2tiff32fp  _XITE_PARAMS(( IR_BAND band, TIFF *tif, uint16 compression ));
extern _XITE_TIFF_WIN_DLL int biff2tiff32fpc  _XITE_PARAMS(( IR_IMAGE img, TIFF *tif, uint16 compression ));
extern _XITE_TIFF_WIN_DLL int biff2tiff32i  _XITE_PARAMS(( II_BAND band, TIFF *tif, uint16 compression ));
extern _XITE_TIFF_WIN_DLL int biff2tiff32ic  _XITE_PARAMS(( II_IMAGE img, TIFF *tif, uint16 compression ));
extern _XITE_TIFF_WIN_DLL int biff2tiff64  _XITE_PARAMS(( ID_BAND band, TIFF *tif, uint16 compression ));
extern _XITE_TIFF_WIN_DLL int biff2tiff64c  _XITE_PARAMS(( ID_IMAGE img, TIFF *tif, uint16 compression ));
extern _XITE_TIFF_WIN_DLL int biff2tiff8  _XITE_PARAMS(( IBAND band, TIFF *tif, uint16 compression ));
extern _XITE_TIFF_WIN_DLL int biff2tiff8c  _XITE_PARAMS(( IMAGE img, TIFF *tif, uint16 compression ));
extern _XITE_TIFF_WIN_DLL int biff2tiff8p  _XITE_PARAMS(( IBAND band, TIFF *tif, Color_cell *coltab, int colLen, uint16 compression ));
extern _XITE_TIFF_WIN_DLL int biff2tiff8s  _XITE_PARAMS(( ISB_BAND band, TIFF *tif, uint16 compression ));
extern _XITE_TIFF_WIN_DLL int biff2tiff8sc  _XITE_PARAMS(( ISB_IMAGE img, TIFF *tif, uint16 compression ));
extern _XITE_TIFF_WIN_DLL IMAGE tiff2biff _XITE_PARAMS(( TIFF *tif, Color_cell **coltab, int *numColors ));
extern _XITE_TIFF_WIN_DLL void tiff2biff1  _XITE_PARAMS(( IBAND band, TIFF * tif, uint32 rows, uint32 cols, uint16 fillorder, int minisblack ));
extern _XITE_TIFF_WIN_DLL void tiff2biff16  _XITE_PARAMS(( IUS_BAND band, TIFF * tif, uint32 rows, uint32 cols ));
extern _XITE_TIFF_WIN_DLL void tiff2biff16c  _XITE_PARAMS(( IUS_IMAGE img, TIFF * tif, uint32 rows, uint32 cols ));
extern _XITE_TIFF_WIN_DLL void tiff2biff1p  _XITE_PARAMS(( IBAND band, TIFF * tif, uint32 rows, uint32 cols, uint16 fillorder, uint16 *redmap, uint16 *greenmap, uint16 *bluemap, Color_cell **coltab, uint16 bps, int *numColors ));
extern _XITE_TIFF_WIN_DLL void tiff2biff2  _XITE_PARAMS(( IBAND band, TIFF * tif, uint32 rows, uint32 cols, int minisblack, double maxval ));
extern _XITE_TIFF_WIN_DLL void tiff2biff2p  _XITE_PARAMS(( IBAND band, TIFF * tif, uint32 rows, uint32 cols, uint16 *redmap, uint16 *greenmap, uint16 *bluemap, Color_cell **coltab, uint16 bps, int *numColors ));
extern _XITE_TIFF_WIN_DLL void tiff2biff32fp  _XITE_PARAMS(( IR_BAND band, TIFF * tif, uint32 rows, uint32 cols ));
extern _XITE_TIFF_WIN_DLL void tiff2biff32fpc  _XITE_PARAMS(( IR_IMAGE img, TIFF * tif, uint32 rows, uint32 cols ));
extern _XITE_TIFF_WIN_DLL void tiff2biff32i  _XITE_PARAMS(( II_BAND band, TIFF * tif, uint32 rows, uint32 cols ));
extern _XITE_TIFF_WIN_DLL void tiff2biff32ic  _XITE_PARAMS(( II_IMAGE img, TIFF * tif, uint32 rows, uint32 cols ));
extern _XITE_TIFF_WIN_DLL void tiff2biff4  _XITE_PARAMS(( IBAND band, TIFF * tif, uint32 rows, uint32 cols, int minisblack, double maxval ));
extern _XITE_TIFF_WIN_DLL void tiff2biff4p  _XITE_PARAMS(( IBAND band, TIFF * tif, uint32 rows, uint32 cols, uint16 *redmap, uint16 *greenmap, uint16 *bluemap, Color_cell **coltab, uint16 bps, int *numColors ));
extern _XITE_TIFF_WIN_DLL void tiff2biff64  _XITE_PARAMS(( ID_BAND band, TIFF * tif, uint32 rows, uint32 cols ));
extern _XITE_TIFF_WIN_DLL void tiff2biff64c  _XITE_PARAMS(( ID_IMAGE img, TIFF * tif, uint32 rows, uint32 cols ));
extern _XITE_TIFF_WIN_DLL void tiff2biff8  _XITE_PARAMS(( IBAND band, TIFF * tif, uint32 rows, uint32 cols, int minisblack, double maxval ));
extern _XITE_TIFF_WIN_DLL void tiff2biff8c  _XITE_PARAMS(( IMAGE img, TIFF * tif, uint32 rows, uint32 cols ));
extern _XITE_TIFF_WIN_DLL void tiff2biff8p  _XITE_PARAMS(( IBAND band, TIFF * tif, uint32 rows, uint32 cols, uint16 *redmap, uint16 *greenmap, uint16 *bluemap, Color_cell **coltab, uint16 bps, int *numColors ));

_XITE_CPLUSPLUS_END

#endif /* _TIFF_H_ */
