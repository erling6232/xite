/*
* $XConsortium: LabelP.h,v 1.27 91/06/22 19:34:58 rws Exp $
*/


/***********************************************************
Copyright 1987, 1988 by Digital Equipment Corporation, Maynard, Massachusetts,
and the Massachusetts Institute of Technology, Cambridge, Massachusetts.

                        All Rights Reserved

Permission to use, copy, modify, and distribute this software and its 
documentation for any purpose and without fee is hereby granted, 
provided that the above copyright notice appear in all copies and that
both that copyright notice and this permission notice appear in 
supporting documentation, and that the names of Digital or MIT not be
used in advertising or publicity pertaining to distribution of the
software without specific, written prior permission.  

DIGITAL DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE, INCLUDING
ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS, IN NO EVENT SHALL
DIGITAL BE LIABLE FOR ANY SPECIAL, INDIRECT OR CONSEQUENTIAL DAMAGES OR
ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS,
WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION,
ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS
SOFTWARE.

******************************************************************/

/* 
 * ColormapP.h - Private definitions for Colormap widget.
 * 
 */

#ifndef _COLORMAPP_H_
#define _COLORMAPP_H_

_XITE_CPLUSPLUS_BEGIN

/***********************************************************************
 *
 * Colormap Widget Private Data
 *
 ***********************************************************************/

#include "Colormap.h"
#include <X11/Xaw/LabelP.h>

/* New fields for the Colormap widget class record */

typedef struct {int foo;} ColormapClassPart;

/* Full class record declaration */
typedef struct _ColormapClassRec {
    CoreClassPart	core_class;
    SimpleClassPart	simple_class;
    LabelClassPart	label_class;
    ColormapClassPart   colormap_class;
} ColormapClassRec;

extern ColormapClassRec colormapClassRec;

/* New fields for the Colormap widget record */
typedef struct {
    /* resources */
    Visual *visual;
} ColormapPart;


/****************************************************************
 *
 * Full instance record declaration
 *
 ****************************************************************/

typedef struct _ColormapRec {
    CorePart	 core;
    SimplePart	 simple;
    LabelPart	 label;
    ColormapPart colormap;
} ColormapRec;

_XITE_CPLUSPLUS_END

#endif /* _COLORMAPP_H_ */
