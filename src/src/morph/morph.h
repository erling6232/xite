
/*
  This header-file is produced automatically (at least in part) by
  makeHeader of BLAB, Ifi, UiO.
*/


/*

________________________________________________________________

        morph.h
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

#ifndef _MORPH_H_
#define _MORPH_H_

_XITE_CPLUSPLUS_BEGIN

#include <xite/biff.h>

typedef enum {
  SQUARE,
  ELLIPSE,
  LINE,
  CIRCLE,
  RECTANGLE,
  LINEAR,
  PARABOL,
  CONE
} morph_height_type;

extern _XITE_WIN_DLL int morphClose  _XITE_PARAMS(( IBAND inband, IBAND outband, ISS_BAND B ));
extern _XITE_WIN_DLL int morphDilate  _XITE_PARAMS(( IBAND inband, IBAND outband, ISS_BAND B ));
extern _XITE_WIN_DLL int morphErode  _XITE_PARAMS(( IBAND inband, IBAND outband, ISS_BAND B ));
extern _XITE_WIN_DLL int morphOpen  _XITE_PARAMS(( IBAND inband, IBAND outband, ISS_BAND B ));
extern _XITE_WIN_DLL int MorphScale _XITE_PARAMS(( IBAND band, int neg ));
extern _XITE_WIN_DLL IBAND mkMorphForm _XITE_PARAMS(( morph_height_type type, double xsize, double ysize, double theta ));
extern _XITE_WIN_DLL int mkMorphHeight _XITE_PARAMS(( IBAND band, int type, double xsize, double ysize, int zsize, double theta ));

_XITE_CPLUSPLUS_END

#endif /* _MORPH_H_ */

