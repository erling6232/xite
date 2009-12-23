/* $XConsortium: Form.c,v 1.48 91/07/22 18:08:25 ackerman Exp $ */

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

/*
  ImageForm is basically the same as the Athena Form widget, except that
  ImageForm calls XtQueryGeometry and possibly XtMakeGeometryRequest inside
  the resize method for each child of class Image or ImageOverlay. The call to
  XtQueryGeometry is added so that the Image widget can hint on a preferred
  size (depending on whether a fixed aspect ratio is desired). The call to
  XtMakeGeometryRequest is added to make the Image parent adapt to this
  size.

  ImageForm is supposed to be used as the parent of a menubar and image
  widget.
*/

static char *Id = "$Id$, Blab, UiO";

#include <xite/includes.h>
#include <X11/IntrinsicP.h>
#include <X11/StringDefs.h>
#include <X11/Xmu/Converters.h>
#include <X11/Xmu/CharSet.h>
#include <X11/Xaw/XawInit.h>
#include "ImageFormP.h"
#include <xite/Image.h>
#include <xite/debug.h>

/* Private Definitions */

static int default_value = -99999;

#define Offset(field) XtOffsetOf(ImageFormRec, image_form.field)
static XtResource resources[] = {
    {XtNdefaultDistance, XtCThickness, XtRInt, sizeof(int),
	Offset(default_spacing), XtRImmediate, (XtPointer)4}
};
#undef Offset

static XtEdgeType defEdge = XtRubber;

#define Offset(field) XtOffsetOf(ImageFormConstraintsRec, image_form.field)
static XtResource imageFormConstraintResources[] = {
    {XtNtop, XtCEdge, XtREdgeType, sizeof(XtEdgeType),
	Offset(top), XtREdgeType, (XtPointer)&defEdge},
    {XtNbottom, XtCEdge, XtREdgeType, sizeof(XtEdgeType),
	Offset(bottom), XtREdgeType, (XtPointer)&defEdge},
    {XtNleft, XtCEdge, XtREdgeType, sizeof(XtEdgeType),
	Offset(left), XtREdgeType, (XtPointer)&defEdge},
    {XtNright, XtCEdge, XtREdgeType, sizeof(XtEdgeType),
	Offset(right), XtREdgeType, (XtPointer)&defEdge},
    {XtNhorizDistance, XtCThickness, XtRInt, sizeof(int),
	Offset(dx), XtRInt, (XtPointer) &default_value},
    {XtNfromHoriz, XtCWidget, XtRWidget, sizeof(Widget),
	Offset(horiz_base), XtRWidget, (XtPointer)NULL},
    {XtNvertDistance, XtCThickness, XtRInt, sizeof(int),
	Offset(dy), XtRInt, (XtPointer) &default_value},
    {XtNfromVert, XtCWidget, XtRWidget, sizeof(Widget),
	Offset(vert_base), XtRWidget, (XtPointer)NULL},
    {XtNresizable, XtCBoolean, XtRBoolean, sizeof(Boolean),
	Offset(allow_resize), XtRImmediate, (XtPointer) FALSE},
};
#undef Offset

static void ClassInitialize(void);
static void ClassPartInitialize(WidgetClass class);
static void Initialize(Widget request, Widget new, ArgList args, Cardinal *num_args);
static void Resize(Widget w);
static void ConstraintInitialize(Widget request, Widget new, ArgList args, Cardinal *num_args);
static Boolean SetValues(Widget current, Widget request, Widget new, ArgList args, Cardinal *num_args);
static Boolean ConstraintSetValues(Widget current, Widget request, Widget new, ArgList args, Cardinal *num_args);
static XtGeometryResult GeometryManager(Widget w, XtWidgetGeometry *request, XtWidgetGeometry *reply);
static XtGeometryResult PreferredGeometry(Widget widget, XtWidgetGeometry *request, XtWidgetGeometry *reply);
static void ChangeManaged(Widget w);
static Boolean Layout(ImageFormWidget fw, Dimension width, Dimension height, Boolean force_relayout);

static void LayoutChild(Widget w);
static void ResizeChildren(Widget w);

ImageFormClassRec imageFormClassRec = {
  { /* core_class fields */
    /* superclass         */    (WidgetClass) &constraintClassRec,
    /* class_name         */    "ImageForm",
    /* widget_size        */    sizeof(ImageFormRec),
    /* class_initialize   */    ClassInitialize,
    /* class_part_init    */    ClassPartInitialize,
    /* class_inited       */    FALSE,
    /* initialize         */    Initialize,
    /* initialize_hook    */    NULL,
    /* realize            */    XtInheritRealize,
    /* actions            */    NULL,
    /* num_actions        */    0,
    /* resources          */    resources,
    /* num_resources      */    XtNumber(resources),
    /* xrm_class          */    NULLQUARK,
    /* compress_motion    */    TRUE,
    /* compress_exposure  */    TRUE,
    /* compress_enterleave*/    TRUE,
    /* visible_interest   */    FALSE,
    /* destroy            */    NULL,
    /* resize             */    Resize,
    /* expose             */    XtInheritExpose,
    /* set_values         */    SetValues,
    /* set_values_hook    */    NULL,
    /* set_values_almost  */    XtInheritSetValuesAlmost,
    /* get_values_hook    */    NULL,
    /* accept_focus       */    NULL,
    /* version            */    XtVersion,
    /* callback_private   */    NULL,
    /* tm_table           */    NULL,
    /* query_geometry     */	PreferredGeometry,
    /* display_accelerator*/	XtInheritDisplayAccelerator,
    /* extension          */	NULL
  },
  { /* composite_class fields */
    /* geometry_manager   */   GeometryManager,
    /* change_managed     */   ChangeManaged,
    /* insert_child       */   XtInheritInsertChild,
    /* delete_child       */   XtInheritDeleteChild,
    /* extension          */   NULL
  },
  { /* constraint_class fields */
    /* subresourses       */   imageFormConstraintResources,
    /* subresource_count  */   XtNumber(imageFormConstraintResources),
    /* constraint_size    */   sizeof(ImageFormConstraintsRec),
    /* initialize         */   ConstraintInitialize,
    /* destroy            */   NULL,
    /* set_values         */   ConstraintSetValues,
    /* extension          */   NULL
  },
  { /* imageform_class fields */
    /* layout             */   Layout
  }
};

WidgetClass imageFormWidgetClass = (WidgetClass)&imageFormClassRec;

/****************************************************************
 *
 * Private Procedures
 *
 ****************************************************************/


static XrmQuark	XtQChainLeft, XtQChainRight, XtQChainTop,
		XtQChainBottom, XtQRubber;

#define	done(address, type) \
	{ toVal->size = sizeof(type); \
	  toVal->addr = (caddr_t) address; \
	  return; \
	}

static void _CvtStringToEdgeType(XrmValuePtr args, Cardinal *num_args, XrmValuePtr fromVal, XrmValuePtr toVal)
{
    static XtEdgeType edgeType;
    XrmQuark q;
    char lowerName[1000];

    XmuCopyISOLatin1Lowered (lowerName, (char*)fromVal->addr);
    q = XrmStringToQuark(lowerName);
    if (q == XtQChainLeft) {
	edgeType = XtChainLeft;
	done(&edgeType, XtEdgeType)
    }
    if (q == XtQChainRight) {
	edgeType = XtChainRight;
	done(&edgeType, XtEdgeType)
    }
    if (q == XtQChainTop) {
	edgeType = XtChainTop;
	done(&edgeType, XtEdgeType)
    }
    if (q == XtQChainBottom) {
	edgeType = XtChainBottom;
	done(&edgeType, XtEdgeType)
    }
    if (q == XtQRubber) {
	edgeType = XtRubber;
	done(&edgeType, XtEdgeType)
    }
    XtStringConversionWarning(fromVal->addr, "edgeType");
    toVal->addr = NULL;
    toVal->size = 0;
}

static void ClassInitialize(void)
{
    static XtConvertArgRec parentCvtArgs[] = {
	{XtBaseOffset, (XtPointer)XtOffsetOf(WidgetRec, core.parent),
	     sizeof(Widget)}
    };
    XawInitializeWidgetSet();
    XtQChainLeft   = XrmPermStringToQuark("chainleft");
    XtQChainRight  = XrmPermStringToQuark("chainright");
    XtQChainTop    = XrmPermStringToQuark("chaintop");
    XtQChainBottom = XrmPermStringToQuark("chainbottom");
    XtQRubber      = XrmPermStringToQuark("rubber");

    XtAddConverter( XtRString, XtREdgeType, _CvtStringToEdgeType, NULL, 0 );
    XtSetTypeConverter (XtRString, XtRWidget, XmuNewCvtStringToWidget,
			parentCvtArgs, XtNumber(parentCvtArgs), XtCacheNone,
			NULL);
}

static void ClassPartInitialize(WidgetClass class)
{
    register ImageFormWidgetClass c = (ImageFormWidgetClass)class;
    register ImageFormWidgetClass super = (ImageFormWidgetClass) 
	c->core_class.superclass;

    if (c->image_form_class.layout == XtInheritLayout)
	c->image_form_class.layout = super->image_form_class.layout;
}

static void Initialize(Widget request, Widget new, ArgList args, Cardinal *num_args)
{
    ImageFormWidget fw = (ImageFormWidget)new;

    ENTER_FUNCTION_DEBUG("ImageForm.c: Initialize");

    fw->image_form.old_width = fw->core.width;
    fw->image_form.old_height = fw->core.height;
    fw->image_form.no_refigure = False;
    fw->image_form.needs_relayout = False;
    fw->image_form.resize_in_layout = True;
    fw->image_form.resize_is_no_op = False;

    LEAVE_FUNCTION_DEBUG("ImageForm.c: Initialize");
}

/*	Function Name: ChangeFormGeometry
 *	Description: Ask the parent to change the form widget's geometry.
 *	Arguments: w - the Form widget.
 *                 query_only - TRUE if this is only a query.
 *                 width, height - the new width and height.
 *                 ret_width, ret_height - the actual size the form is allowed
 *                                         to resize to.
 *	Returns: TRUE of children may always be resized.
 */

static Boolean ChangeFormGeometry(Widget w, Boolean query_only, Dimension width, Dimension height, Dimension *ret_width, Dimension *ret_height)
{
    ImageFormWidget fw = (ImageFormWidget) w;
    Boolean always_resize_children;
    XtGeometryResult result;
    XtWidgetGeometry request, return_request;

    /*
     * If we are already at the desired size then there is no need
     * to ask our parent of we can change size.
     */

    ENTER_FUNCTION_DEBUG("ImageForm.c: ChangeFormGeometry");

    if ( (width == fw->core.width) && (height == fw->core.height) ) {
      LEAVE_FUNCTION_DEBUG("ImageForm.c: ChangeFormGeometry");
      return(TRUE);
    }

    request.width = width;
    request.height = height;
    request.request_mode = CWWidth | CWHeight;
    if (query_only)
	request.request_mode |= XtCWQueryOnly;

    /*
     * Do no invoke the resize rules if our size changes here.
     */

    fw->image_form.resize_is_no_op = TRUE;

    result = XtMakeGeometryRequest(w, &request, &return_request);
    if (result == XtGeometryAlmost) {
	request = return_request;
	(void) XtMakeGeometryRequest(w, &request, &return_request);
	always_resize_children = FALSE;
    }
    else
	always_resize_children = (result == XtGeometryYes);

    fw->image_form.resize_is_no_op = FALSE;

    if (ret_width != NULL)
	*ret_width = request.width;
    if (ret_height != NULL)
	*ret_height = request.height;

    LEAVE_FUNCTION_DEBUG("ImageForm.c: ChangeFormGeometry");

    return(always_resize_children);
}

/*	Function Name: Layout
 *	Description: Moves all the children around.
 *	Arguments: fw - the Form widget.
 *                 width, height - ** UNUSED **.
 *                 force_relayout - will force the children to be
 *                                 moved, even if some go past the edge
 *                                 of the form.
 *	Returns: True if the children are allowed to move from their
 *               current locations to the new ones.
 */

static Boolean Layout(ImageFormWidget fw, Dimension width, Dimension height, Boolean force_relayout)
{
    int num_children = fw->composite.num_children;
    WidgetList children = fw->composite.children;
    Widget *childP;
    Dimension maxx, maxy;
    Boolean ret_val;

    ENTER_FUNCTION_DEBUG("ImageForm.c: Layout");

    for (childP = children; childP - children < num_children; childP++) {
	ImageFormConstraints image_form =
	  (ImageFormConstraints)(*childP)->core.constraints;
	image_form->image_form.layout_state = LayoutPending;
    }

    maxx = maxy = 1;
    for (childP = children; childP - children < num_children; childP++) {
	if (XtIsManaged(*childP)) {
	    register ImageFormConstraints image_form;
	    register Position x, y;

	    image_form = (ImageFormConstraints)(*childP)->core.constraints;

	    LayoutChild(*childP);

	    x = image_form->image_form.new_x + (*childP)->core.width + 
		((*childP)->core.border_width << 1);
	    if (x > (int)maxx)
		maxx = x;

	    y = image_form->image_form.new_y + (*childP)->core.height +
		((*childP)->core.border_width << 1);
	    if (y > (int)maxy)
		maxy = y;
	}
    }

    fw->image_form.preferred_width = (maxx += fw->image_form.default_spacing);
    fw->image_form.preferred_height = (maxy += fw->image_form.default_spacing);

    if (fw->image_form.resize_in_layout) {
	Boolean always_resize_children;

	always_resize_children = ChangeFormGeometry( (Widget) fw, FALSE,
						    maxx, maxy, NULL, NULL);

	fw->image_form.old_width  = fw->core.width;
	fw->image_form.old_height = fw->core.height;

	ret_val = (always_resize_children || ( (fw->core.width >= maxx) &&
					      (fw->core.height >= maxy)));

	if (force_relayout)
	    ret_val = TRUE;

	if (ret_val)
	    ResizeChildren((Widget) fw);
    }
    else
	ret_val = False;

    fw->image_form.needs_relayout = False;

    LEAVE_FUNCTION_DEBUG("ImageForm.c: Layout");

    return ret_val;
}

/*	Function Name: ResizeChildren
 *	Description: Resizes all children to new_x and new_y.
 *	Arguments: w - the form widget.
 *	Returns: none.
 */

static void ResizeChildren(Widget w)
{
    ImageFormWidget fw = (ImageFormWidget) w;
    int num_children = fw->composite.num_children;
    WidgetList children = fw->composite.children;
    Widget *childP;

    ENTER_FUNCTION_DEBUG("ImageForm.c: ResizeChildren");

    for (childP = children; childP - children < num_children; childP++) {
	ImageFormConstraints image_form;

	if (!XtIsManaged(*childP)) 
	    continue;

	image_form = (ImageFormConstraints)(*childP)->core.constraints;
	if (fw->image_form.no_refigure) {
/* 
 * I am changing the widget wrapper w/o modifing the window.  This is
 * risky, but I can get away with it since I am the parent of this
 * widget, and he must ask me for any geometry changes.
 *
 * The window will be updated when no_refigure is set back to False.
 */	
	    (*childP)->core.x = image_form->image_form.new_x;
	    (*childP)->core.y = image_form->image_form.new_y;
	}
	else
	    XtMoveWidget(*childP, image_form->image_form.new_x,
			 image_form->image_form.new_y);
    }

    LEAVE_FUNCTION_DEBUG("ImageForm.c: ResizeChildren");
}


static void LayoutChild(Widget w)
{
    ImageFormConstraints image_form =
      (ImageFormConstraints)w->core.constraints;
    Widget ref;

    ENTER_FUNCTION_DEBUG("ImageForm.c: LayoutChild");

    switch (image_form->image_form.layout_state) {

      case LayoutPending:
	image_form->image_form.layout_state = LayoutInProgress;
	break;

      case LayoutDone: {
	LEAVE_FUNCTION_DEBUG("ImageForm.c: LayoutChild");
	return;
      }

      case LayoutInProgress:
	{
	String subs[2];
	Cardinal num_subs = 2;
	subs[0] = w->core.name;
	subs[1] = w->core.parent->core.name;
	XtAppWarningMsg(XtWidgetToApplicationContext(w),
			"constraintLoop","xawFormLayout","XawToolkitError",
   "constraint loop detected while laying out child '%s' in FormWidget '%s'",
			subs, &num_subs);
	LEAVE_FUNCTION_DEBUG("ImageForm.c: LayoutChild");
	return;
	}
    }

    image_form->image_form.new_x = image_form->image_form.dx;
    image_form->image_form.new_y = image_form->image_form.dy;
    if ((ref = image_form->image_form.horiz_base) != (Widget)NULL) {
	ImageFormConstraints ref_form =
	  (ImageFormConstraints) ref->core.constraints;

	LayoutChild(ref);
	image_form->image_form.new_x += (ref_form->image_form.new_x + 
			     ref->core.width + (ref->core.border_width << 1));
    }
    if ((ref = image_form->image_form.vert_base) != (Widget)NULL) {
	ImageFormConstraints ref_form =
	  (ImageFormConstraints) ref->core.constraints;

	LayoutChild(ref);
	image_form->image_form.new_y += (ref_form->image_form.new_y + 
					 ref->core.height +
					 (ref->core.border_width << 1));
    }

    image_form->image_form.layout_state = LayoutDone;

    LEAVE_FUNCTION_DEBUG("ImageForm.c: LayoutChild");
}


static Position TransformCoord(register Position loc, Dimension old, Dimension new, XawEdgeType type)
{
    if (type == XtRubber) {
        if ( ((int) old) > 0)
	    loc = (int)(loc * new) / (int)old;
    }
    else if (type == XtChainBottom || type == XtChainRight)
      loc += (Position)new - (Position)old;

    /* I don't see any problem with returning values less than zero. */

    return (loc);
}

static void Resize(Widget w)
{
    ImageFormWidget fw = (ImageFormWidget)w;
    WidgetList children = fw->composite.children;
    int num_children = fw->composite.num_children;
    Widget *childP;
    Position x, y;
    Dimension width, height;

    ENTER_FUNCTION_DEBUG("ImageForm.c: Resize");

    if (!fw->image_form.resize_is_no_op)
	for (childP = children; childP - children < num_children; childP++) {
	    ImageFormConstraints image_form =
	      (ImageFormConstraints)(*childP)->core.constraints;

	    if (!XtIsManaged(*childP)) continue;
	    x = TransformCoord( (*childP)->core.x, fw->image_form.old_width,
			       fw->core.width, image_form->image_form.left );
	    y = TransformCoord( (*childP)->core.y, fw->image_form.old_height,
			       fw->core.height, image_form->image_form.top );

	    FPRINTF5("    %s: %d, %s: %d\n",
		     "image_form->image_form.virtual_width",
		     image_form->image_form.virtual_width,
		     "image_form->image_form.virtual_height",
		     image_form->image_form.virtual_height);

	    FPRINTF5("    %s: %d, %s: %d\n", "fw->image_form.old_width",
		     fw->image_form.old_width,
		     "fw->image_form.old_height"
		     , fw->image_form.old_height);

	    FPRINTF5("    %s: %d, %s: %d\n", "fw->core.width",
		     fw->core.width, "fw->core.height", fw->core.height);

	    image_form->image_form.virtual_width =
	      TransformCoord((Position)((*childP)->core.x
					+ image_form->image_form.virtual_width
					+ 2 * (*childP)->core.border_width),
			     fw->image_form.old_width, fw->core.width,
			     image_form->image_form.right )
		- (x + 2 * (*childP)->core.border_width);

	    image_form->image_form.virtual_height =
	      TransformCoord((Position)((*childP)->core.y +
					image_form->image_form.virtual_height
					+ 2 * (*childP)->core.border_width),
			     fw->image_form.old_height, fw->core.height,
			     image_form->image_form.bottom ) 
		- ( y + 2 * (*childP)->core.border_width);
	    
	    FPRINTF5("    %s: %d, %s: %d\n",
		     "image_form->image_form.virtual_width",
		     image_form->image_form.virtual_width,
		     "image_form->image_form.virtual_height",
		     image_form->image_form.virtual_height);

	    width = (Dimension) 
	      (image_form->image_form.virtual_width < 1) ?
		1 : image_form->image_form.virtual_width;
	    height = (Dimension)
	      (image_form->image_form.virtual_height < 1) ?
		1 : image_form->image_form.virtual_height;

	    if (!XtIsSubclass(*childP, imageWidgetClass))
	      XtConfigureWidget(*childP, x, y, (Dimension)width,
				(Dimension)height,
				(*childP)->core.border_width);
	    else {
	      XtWidgetGeometry intended, intend_return;
	      XtGeometryResult result;

	      intended.request_mode = CWWidth | CWHeight;
	      intended.width        = width;
	      intended.height       = height;

	      result = XtQueryGeometry(*childP, &intended, &intend_return);

	      if (result == XtGeometryNo) {
		image_form->image_form.virtual_width  = intended.width;
		image_form->image_form.virtual_height = intended.height;
	      }

	      if (result == XtGeometryYes) {

		image_form->image_form.virtual_width  = intend_return.width;
		image_form->image_form.virtual_height = intend_return.height;

		XtConfigureWidget(*childP, x, y, (Dimension)width,
				  (Dimension)height,
				  (*childP)->core.border_width);
	      } else {
		XtWidgetGeometry request, answer;
		XtGeometryResult res;
		Widget MenuBar;
		Dimension h;

		MenuBar = XtNameToWidget(w, "menubar");
		if (MenuBar)
		  XtVaGetValues(MenuBar, XtNheight, &h, NULL);
		else h = 0;

		image_form->image_form.virtual_width  = intend_return.width;
		image_form->image_form.virtual_height = intend_return.height;

		request.request_mode      = CWWidth | CWHeight;
		request.width             = intend_return.width;
		request.height            = intend_return.height + h;

		res = XtMakeGeometryRequest(w, &request, &answer);

		XtConfigureWidget(*childP, x, y,
				  (Dimension)request.width,
				  (Dimension)request.height - h,
				  (*childP)->core.border_width);
	      }
	    }
	  }

    fw->image_form.old_width = fw->core.width;
    fw->image_form.old_height = fw->core.height;

    LEAVE_FUNCTION_DEBUG("ImageForm.c: Resize");
}

/*
 * I don't want to even think about what ``Almost'' would mean - Chris.
 */

static XtGeometryResult GeometryManager(Widget w, XtWidgetGeometry *request, XtWidgetGeometry *reply)
{
    Dimension old_width, old_height;
    ImageFormWidget fw = (ImageFormWidget) XtParent(w);
    ImageFormConstraints image_form =
      (ImageFormConstraints) w->core.constraints;
    XtWidgetGeometry allowed;
    XtGeometryResult ret_val;

    ENTER_FUNCTION_DEBUG("ImageForm.c: GeometryManager");

    if ((request->request_mode & ~(XtCWQueryOnly | CWWidth | CWHeight)) ||
	!image_form->image_form.allow_resize) {

	/* If GeometryManager is invoked during a SetValues call on a child
         * then it is necessary to compute a new layout if ConstraintSetValues
         * allowed any constraint changes. */

	if (fw->image_form.needs_relayout) 
	  (*((ImageFormWidgetClass)fw->core.widget_class)->
	   image_form_class.layout)
	    (fw, 0, 0, True);

	LEAVE_FUNCTION_DEBUG("ImageForm.c: GeometryManager");
	return(XtGeometryNo);
    }

    if (request->request_mode & CWWidth)
	allowed.width = request->width;
    else
	allowed.width = w->core.width;

    if (request->request_mode & CWHeight)
	allowed.height = request->height;
    else
	allowed.height = w->core.height;

    if (allowed.width == w->core.width && allowed.height == w->core.height) {

	/* If GeometryManager is invoked during a SetValues call on a child
         * then it is necessary to compute a new layout if ConstraintSetValues
         * allowed any constraint changes. */

	if (fw->image_form.needs_relayout) 
	  (*((ImageFormWidgetClass)fw->core.widget_class)->
	   image_form_class.layout)
		(fw, 0, 0, True);

	LEAVE_FUNCTION_DEBUG("ImageForm.c: GeometryManager");
	return(XtGeometryNo);
    }

    /*
     * Remember the old size, and then set the size to the requested size.
     */

    old_width = w->core.width;
    old_height = w->core.height;
    w->core.width = allowed.width;
    w->core.height = allowed.height;

    if (request->request_mode & XtCWQueryOnly) {
	Boolean always_resize_children;
	Dimension ret_width, ret_height;

	fw->image_form.resize_in_layout = FALSE;

	(*((ImageFormWidgetClass)fw->core.widget_class)->
	 image_form_class.layout)
	  ( fw, w->core.width, w->core.height, FALSE );

	/*
	 * Reset the size of this child back to what it used to be.
	 */

	w->core.width = old_width;
	w->core.height = old_height;

	fw->image_form.resize_in_layout = TRUE;

	always_resize_children = ChangeFormGeometry(w, TRUE, 
				   fw->image_form.preferred_width,
				   fw->image_form.preferred_height,
				   &ret_width, &ret_height);

	if (always_resize_children || 
	    ((ret_width >= fw->image_form.preferred_width) &&
	     (ret_height >= fw->image_form.preferred_height)))
	{
	    ret_val = XtGeometryYes;
	}
	else
	    ret_val = XtGeometryNo;
    }
    else {
      if ((*((ImageFormWidgetClass)fw->core.widget_class)->
	   image_form_class.layout)
	  ( fw, w->core.width, w->core.height, FALSE))
	{
	    /* reset virtual width and height. */
	    image_form->image_form.virtual_width = w->core.width;
	    image_form->image_form.virtual_height = w->core.height;
	    if (fw->image_form.no_refigure) {
/* 
 * I am changing the widget wrapper w/o modifing the window.  This is
 * risky, but I can get away with it since I am the parent of this
 * widget, and he must ask me for any geometry changes.
 *
 * The window will be updated when no_refigure is set back to False.
 */	
		image_form->image_form.deferred_resize = True;
		ret_val = XtGeometryDone;
	    }
	    else 
		ret_val = XtGeometryYes;
	}
	else {
	    w->core.width = old_width;
	    w->core.height = old_height;
	    ret_val = XtGeometryNo;
	}
    }

    LEAVE_FUNCTION_DEBUG("ImageForm.c: GeometryManager");
    return(ret_val);
}


static Boolean SetValues(Widget current, Widget request, Widget new, ArgList args, Cardinal *num_args)
{
    return( FALSE );
}


static void ConstraintInitialize(Widget request, Widget new, ArgList args, Cardinal *num_args)
{
    ImageFormConstraints image_form =
      (ImageFormConstraints)new->core.constraints;
    ImageFormWidget fw = (ImageFormWidget)new->core.parent;

    ENTER_FUNCTION_DEBUG("ImageForm.c: ConstraintInitialize");

    image_form->image_form.virtual_width = (int) new->core.width;
    image_form->image_form.virtual_height = (int) new->core.height;

    if (image_form->image_form.dx == default_value)
        image_form->image_form.dx = fw->image_form.default_spacing;

    if (image_form->image_form.dy == default_value)
        image_form->image_form.dy = fw->image_form.default_spacing;

    image_form->image_form.deferred_resize = False;

    LEAVE_FUNCTION_DEBUG("ImageForm.c: ConstraintInitialize");
}

static Boolean ConstraintSetValues(Widget current, Widget request, Widget new, ArgList args, Cardinal *num_args)
{
  register ImageFormConstraints cfc =
    (ImageFormConstraints) current->core.constraints;
  register ImageFormConstraints nfc =
    (ImageFormConstraints) new->core.constraints;
  
  ENTER_FUNCTION_DEBUG("ImageForm.c: ConstraintSetValues");

  if (cfc->image_form.top          != nfc->image_form.top         ||
      cfc->image_form.bottom       != nfc->image_form.bottom      ||
      cfc->image_form.left         != nfc->image_form.left        ||
      cfc->image_form.right        != nfc->image_form.right       ||
      cfc->image_form.dx           != nfc->image_form.dx          ||
      cfc->image_form.dy           != nfc->image_form.dy          ||
      cfc->image_form.horiz_base   != nfc->image_form.horiz_base  ||
      cfc->image_form.vert_base    != nfc->image_form.vert_base) {

      ImageFormWidget fp = (ImageFormWidget) XtParent(new);

    /* If there are no subclass ConstraintSetValues procedures remaining
     * to be invoked, and if there is no geometry request about to be
     * made, then invoke the new layout now; else defer it. */

    if (XtClass(XtParent(new))  == imageFormWidgetClass	&&
	current->core.x		== new->core.x		&&
	current->core.y		== new->core.y		&&
	current->core.width	== new->core.width	&&
	current->core.height	== new->core.height	&&
	current->core.border_width == new->core.border_width)
	Layout(fp, 0, 0, True);
    else fp->image_form.needs_relayout = True;
  }

  LEAVE_FUNCTION_DEBUG("ImageForm.c: ConstraintSetValues");
  return( FALSE );
}

static void ChangeManaged(Widget w)
{
  ImageFormWidget fw = (ImageFormWidget)w;
  ImageFormConstraints image_form;
  WidgetList children, childP;
  int num_children = fw->composite.num_children;
  Widget child;

  /*
   * Reset virtual width and height for all children.
   */

  ENTER_FUNCTION_DEBUG("ImageForm.c: ChangeManaged");
  
  for (children = childP = fw->composite.children ;
       childP - children < num_children; childP++) {
    child = *childP;
    if (XtIsManaged(child)) {
      image_form = (ImageFormConstraints)child->core.constraints;

/*
 * If the size is one (1) then we must not change the virtual sizes, as
 * they contain useful information.  If someone actually wants a widget of
 * width or height one (1) in a form widget he will lose, can't win them all.
 *
 * Chris D. Peterson 2/9/89.
 */
	 
      if ( child->core.width != 1)
	image_form->image_form.virtual_width = (int) child->core.width;
      if ( child->core.height != 1)
	image_form->image_form.virtual_height = (int) child->core.height;
    }
  }
  (*((ImageFormWidgetClass)w->core.widget_class)->image_form_class.layout)
  	                                 ((ImageFormWidget) w, w->core.width, 
					  w->core.height, TRUE);

  LEAVE_FUNCTION_DEBUG("ImageForm.c: ChangeManaged");
}


static XtGeometryResult PreferredGeometry(Widget widget, XtWidgetGeometry *request, XtWidgetGeometry *reply)
{
    ImageFormWidget w = (ImageFormWidget)widget;
    
    ENTER_FUNCTION_DEBUG("ImageForm.c: PreferredGeometry");

    reply->width        = w->image_form.preferred_width;
    reply->height       = w->image_form.preferred_height;
    reply->request_mode = CWWidth | CWHeight;

    /**
    if (request->request_mode & (CWWidth | CWHeight) ==	(CWWidth | CWHeight)
	&& request->width  == reply->width
	&& request->height == reply->height) {
     **/
    if (  request->request_mode & (CWWidth | CWHeight) ==
    	    reply->request_mode & (CWWidth | CWHeight)
	  && request->width == reply->width
  	  && request->height == reply->height) {
      LEAVE_FUNCTION_DEBUG("ImageForm.c: PreferredGeometry");
      return XtGeometryYes;
    } else if (reply->width  == w->core.width &&
	       reply->height == w->core.height) {
      LEAVE_FUNCTION_DEBUG("ImageForm.c: PreferredGeometry");
      return XtGeometryNo;
    } else {
      LEAVE_FUNCTION_DEBUG("ImageForm.c: PreferredGeometry");
      return XtGeometryAlmost;
    }
}


/**********************************************************************
 *
 * Public routines
 *
 **********************************************************************/

/* 
 * Set or reset figuring (ignored if not realized)
 */

void XawImageFormDoLayout(Widget w,
# if NeedWidePrototypes
		int doit)
# else
		Boolean doit)
# endif
{
    register Widget *childP;
    register ImageFormWidget fw = (ImageFormWidget)w;
    register int num_children = fw->composite.num_children;
    register WidgetList children = fw->composite.children;

    ENTER_FUNCTION_DEBUG("ImageForm.c: XawImageFormDoLayout");

    if ( ((fw->image_form.no_refigure = !doit) == TRUE) || !XtIsRealized(w) ) {
      LEAVE_FUNCTION_DEBUG("ImageForm.c: XawImageFormDoLayout");
      return;
    }

    for (childP = children; childP - children < num_children; childP++) {
	register Widget w = *childP;
	if (XtIsManaged(w)) {
	    ImageFormConstraints image_form =
	      (ImageFormConstraints)w->core.constraints;

	    /*
	     * Xt Configure widget is too smart, and optimizes out
	     * my changes.
	     */

	    XMoveResizeWindow(XtDisplay(w), XtWindow(w),
			      w->core.x, w->core.y, 
			      w->core.width, w->core.height);

	    if (image_form->image_form.deferred_resize &&
		XtClass(w)->core_class.resize != (XtWidgetProc) NULL) {
		(*(XtClass(w)->core_class.resize))(w);
		image_form->image_form.deferred_resize = False;
	    }
	}
    }

    LEAVE_FUNCTION_DEBUG("ImageForm.c: XawImageFormDoLayout");
}
