/* $XConsortium: FormP.h,v 1.20 91/05/02 16:20:29 swick Exp $ */
/* Copyright	Massachusetts Institute of Technology	1987 */


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



/*C*

________________________________________________________________

        ImageForm
        $Id$
        Copyright 1995, Blab, UiO
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


/* Form widget private definitions */

#ifndef _ImageFormP_H_
#define _ImageFormP_H_

_XITE_CPLUSPLUS_BEGIN

#include <xite/ImageForm.h>
#include <X11/ConstrainP.h>

#define XtREdgeType "EdgeType"

typedef enum {LayoutPending, LayoutInProgress, LayoutDone} LayoutState;
#define XtInheritLayout (( Boolean (*) _XITE_PARAMS(( ImageFormWidget, Dimension, Dimension, Boolean )) ) _XtInherit)

typedef struct {
    Boolean (*layout) _XITE_PARAMS(( ImageFormWidget, Dimension, Dimension, Boolean ));
} ImageFormClassPart;

/*
 * Layout(
 *	FormWidget w	- the widget whose children are to be configured
 *	Dimension w, h	- bounding box of layout to be calculated
 *
 *  Stores preferred geometry in w->form.preferred_{width,height}.
 *  If w->form.resize_in_layout is True, then a geometry request
 *  may be made for the preferred bounding box if necessary.
 *
 *  Returns True if a geometry request was granted, False otherwise.
 */

typedef struct _ImageFormClassRec {
    CoreClassPart	core_class;
    CompositeClassPart	composite_class;
    ConstraintClassPart	constraint_class;
    ImageFormClassPart	image_form_class;
} ImageFormClassRec;

extern ImageFormClassRec imageFormClassRec;

typedef struct _ImageFormPart {
    /* resources */
    int		default_spacing;    /* default distance between children */
    /* private state */
    Dimension	old_width, old_height; /* last known dimensions		 */
    int		no_refigure;	    /* no re-layout while > 0		 */
    Boolean	needs_relayout;	    /* next time no_refigure == 0	 */
    Boolean	resize_in_layout;   /* should layout() do geom request?  */
    Dimension	preferred_width, preferred_height; /* cached from layout */
    Boolean     resize_is_no_op;    /* Causes resize to take not action. */
} ImageFormPart;

typedef struct _ImageFormRec {
    CorePart		core;
    CompositePart	composite;
    ConstraintPart	constraint;
    ImageFormPart	image_form;
} ImageFormRec;

typedef struct _ImageFormConstraintsPart {
/*
 * Constraint Resources.
 */
    XtEdgeType	top, bottom,	/* where to drag edge on resize		*/
		left, right;
    int		dx;		/* desired horiz offset			*/
    int		dy;		/* desired vertical offset		*/
    Widget	horiz_base;	/* measure dx from here if non-null	*/
    Widget	vert_base;	/* measure dy from here if non-null	*/
    Boolean	allow_resize;	/* TRUE if child may request resize	*/

/*
 * Private contstraint resources.
 */

/*
 * What the size of this child would be if we did not impose the 
 * constraint the width and height must be greater than zero (0).
 */
    short	virtual_width, virtual_height;

/*
 * Temporary Storage for children's new possible possition.
 */

    Position new_x, new_y;

    LayoutState	layout_state;	/* temporary layout state		*/
    Boolean	deferred_resize; /* was resized while no_refigure is set */
} ImageFormConstraintsPart;

typedef struct _ImageFormConstraintsRec {
    ImageFormConstraintsPart image_form;
} ImageFormConstraintsRec, *ImageFormConstraints;

_XITE_CPLUSPLUS_END

#endif /* _ImageFormP_H_ */
