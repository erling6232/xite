
/*
  This header-file is produced automatically (at least in part) by
  makeHeader of BLAB, Ifi, UiO.
*/


/*

________________________________________________________________

        convert.h
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

#ifndef _CONVERT_H_
#define _CONVERT_H_

_XITE_CPLUSPLUS_BEGIN

#include <xite/biff.h>
#include <xite/color.h>


extern _XITE_WIN_DLL int ascii2biff  _XITE_PARAMS(( IBAND *band, char *asciifile, IPIXTYP pixtyp ));
extern _XITE_WIN_DLL int biff2ascii  _XITE_PARAMS(( IBAND band, char *filename, char *type, int co ));
extern _XITE_WIN_DLL int biff2raw  _XITE_PARAMS(( IBAND band, char *filename, char *type, int pgm ));
extern _XITE_WIN_DLL IBAND mkConvertBand  _XITE_PARAMS(( IBAND b, IPIXTYP pixtyp ));
extern _XITE_WIN_DLL int convertBand  _XITE_PARAMS(( IBAND b1, IBAND b2 ));
extern _XITE_WIN_DLL int biff_swap  _XITE_PARAMS(( IBAND band ));
extern _XITE_WIN_DLL int pseudo2rgb  _XITE_PARAMS(( IBAND band, IMAGE img, Color_tab coltab, int ctlen ));

_XITE_CPLUSPLUS_END

#endif /* _CONVERT_H_ */
