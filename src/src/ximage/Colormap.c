/* $XConsortium: Label.c,v 1.92 91/07/24 16:56:28 converse Exp $ */

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

        Colormap
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

static char *Id = "$Id$, Blab, UiO";


/*
 * Colormap.c - Colormap widget
 * This is based on the Xaw Label widget. Most of the Label.c functionality
 * has been removed. What is left is basically a widget which can have
 * a different visual than its parent.
 */

#include <xite/includes.h>
#include <X11/IntrinsicP.h>
#include <X11/StringDefs.h>
#include <X11/Xos.h>
#include <X11/Xaw/XawInit.h>
#include <X11/Xmu/Drawing.h>
#include "ColormapP.h"
#include <stdio.h>
#include <ctype.h>

#ifdef CRAY
#define WORD64
#endif

/****************************************************************
 *
 * Full class record constant
 *
 ****************************************************************/

/* Private Data */

#define offset(field) XtOffsetOf(ColormapRec, field)
static XtResource resources[] = {
    {XtNvisual, XtCVisual, XtRVisual, sizeof(Visual*),
	offset(colormap.visual), XtRImmediate, CopyFromParent},
};
#undef offset

#ifndef FUNCPROTO
static void Realize(/* Widget w, Mask *valueMask, XSetWindowAttributes *attributes */);
#else /* FUNCPROTO */
static void Realize( Widget w, Mask *valueMask, XSetWindowAttributes *attributes );
#endif /* FUNCPROTO */

ColormapClassRec colormapClassRec = {
  {
/* core_class fields */	
#define superclass		(&simpleClassRec)
    /* superclass	  	*/	(WidgetClass) superclass,
    /* class_name	  	*/	"Colormap",
    /* widget_size	  	*/	sizeof(ColormapRec),
    /* class_initialize   	*/	NULL,
    /* class_part_initialize	*/	NULL,
    /* class_inited       	*/	FALSE,
    /* initialize	  	*/	NULL,
    /* initialize_hook		*/	NULL,
    /* realize		  	*/	Realize,
    /* actions		  	*/	NULL,
    /* num_actions	  	*/	0,
    /* resources	  	*/	resources,
    /* num_resources	  	*/	XtNumber(resources),
    /* xrm_class	  	*/	NULLQUARK,
    /* compress_motion	  	*/	TRUE,
    /* compress_exposure  	*/	TRUE,
    /* compress_enterleave	*/	TRUE,
    /* visible_interest	  	*/	FALSE,
    /* destroy		  	*/	NULL,
    /* resize		  	*/	XtInheritResize,
    /* expose		  	*/	XtInheritExpose,
    /* set_values	  	*/	NULL,
    /* set_values_hook		*/	NULL,
    /* set_values_almost	*/	XtInheritSetValuesAlmost,
    /* get_values_hook		*/	NULL,
    /* accept_focus	 	*/	NULL,
    /* version			*/	XtVersion,
    /* callback_private   	*/	NULL,
    /* tm_table		   	*/	NULL,
    /* query_geometry		*/	XtInheritQueryGeometry,
    /* display_accelerator	*/	XtInheritDisplayAccelerator,
    /* extension		*/	NULL
  },
/* Simple class fields initialization */
  {
    /* change_sensitive		*/	XtInheritChangeSensitive
  }
};

WidgetClass colormapWidgetClass = (WidgetClass)&colormapClassRec;

/****************************************************************
 *
 * Private Procedures
 *
 ****************************************************************/

/*      Function Name: ConvertCursor
 *      Description: Converts a name to a new cursor.
 *      Arguments: w - the simple widget.
 *      Returns: none.
 */

#ifndef FUNCPROTO
static void ConvertCursor(w)
Widget w;
#else /* FUNCPROTO */
static void ConvertCursor(Widget w)
#endif /* FUNCPROTO */
{
    SimpleWidget simple = (SimpleWidget) w;
    XrmValue from, to;
    Cursor cursor;
   
    if (simple->simple.cursor_name == NULL)
        return;

    from.addr = (XPointer) simple->simple.cursor_name;
    from.size = strlen((char *) from.addr) + 1;

    to.size = sizeof(Cursor);
    to.addr = (XPointer) &cursor;

    if (XtConvertAndStore(w, XtRString, &from, XtRColorCursor, &to)) {
        if ( cursor !=  None) 
            simple->simple.cursor = cursor;
    } 
    else {
        XtAppErrorMsg(XtWidgetToApplicationContext(w),
                      "convertFailed","ConvertCursor","XawError",
                      "Simple: ConvertCursor failed.",
                      (String *)NULL, (Cardinal *)NULL);
    }
}

#ifndef FUNCPROTO
static void Realize(w, valueMask, attributes)
register Widget w;
Mask *valueMask;
XSetWindowAttributes *attributes;
#else /* FUNCPROTO */
static void Realize(Widget w, Mask *valueMask, XSetWindowAttributes *attributes)
#endif /* FUNCPROTO */
{
    Pixmap border_pixmap;

    if (!XtIsSensitive(w)) {
        /* change border to gray; have to remember the old one,
         * so XtDestroyWidget deletes the proper one */
        if (((SimpleWidget)w)->simple.insensitive_border == None)
            ((SimpleWidget)w)->simple.insensitive_border =
                XmuCreateStippledPixmap(XtScreen(w),
                                        w->core.border_pixel, 
                                        w->core.background_pixel,
                                        w->core.depth);
        border_pixmap = w->core.border_pixmap;
        attributes->border_pixmap =
          w->core.border_pixmap = ((SimpleWidget)w)->simple.insensitive_border;

        *valueMask |= CWBorderPixmap;
        *valueMask &= ~CWBorderPixel;
    }

    ConvertCursor(w);

    if ((attributes->cursor = ((SimpleWidget)w)->simple.cursor) != None)
        *valueMask |= CWCursor;

    XtCreateWindow( w, (unsigned int)InputOutput,
		   ((ColormapWidget)w)->colormap.visual,
		   *valueMask, attributes );

    if (!XtIsSensitive(w))
        w->core.border_pixmap = border_pixmap;
}
