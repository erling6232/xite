
/*
  This header-file is produced automatically (at least in part) by
  makeHeader of BLAB, Ifi, UiO.
*/


/*

________________________________________________________________

        Xslice.h
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

#ifndef _Xslice_H_
#define _Xslice_H_

_XITE_CPLUSPLUS_BEGIN

#include <X11/Intrinsic.h>
#include <xite/biff.h>
#include <xite/callbacks.h>


extern void xslice  _XITE_PARAMS(( Widget wid, IMAGE image, Colormap map, int planes ));
extern IMAGE colorbar_image _XITE_PARAMS(( long xsize, long ysize, long low, long high, Visual *visual, char *title ));
extern Widget colorbar_widget _XITE_PARAMS(( Widget parent, char *resource_name, char *shell_resource_name, int colortab, long xsize, long ysize, long low, long high, Visual *visual, int depth, char *title, image_callbacks *callbacks ));

_XITE_CPLUSPLUS_END

#endif /* _Xslice_H_ */
