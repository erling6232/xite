/* Generated by wbuild from "Common.w"
** (generator version $Revision$ of $Date$)
*/
#include <xite/includes.h>
#include <X11/IntrinsicP.h>
#include <X11/StringDefs.h>
#line 930 "Common.w"
#include XITE_STDIO_H
#include <ctype.h>
#line 931 "Common.w"
#include <X11/Xmu/Converters.h>
#include <X11/Xmu/CharSet.h>
#line 932 "Common.w"
#include <Xfwf/Converters.h>
#include <Xfwf/CommonP.h>
#define focus_detail(detail) (detail ==NotifyAncestor ?"NotifyAncestor":detail ==NotifyVirtual ?"NotifyVirtual":detail ==NotifyInferior ?"NotifyInferior":detail ==NotifyNonlinear ?"NotifyNonlinear":detail ==NotifyNonlinearVirtual ?"NotifyNonlinearVirtual":detail ==NotifyPointer ?"NotifyPointer":detail ==NotifyPointerRoot ?"NotifyPointerRoot":detail ==NotifyDetailNone ?"NotifyDetailNone":"???")


#line 804 "Common.w"
static void focusIn(
#if NeedFunctionPrototypes
Widget,XEvent*,String*,Cardinal*
#endif
);
#line 825 "Common.w"
static void focusOut(
#if NeedFunctionPrototypes
Widget,XEvent*,String*,Cardinal*
#endif
);
#line 841 "Common.w"
static void traverseDown(
#if NeedFunctionPrototypes
Widget,XEvent*,String*,Cardinal*
#endif
);
#line 850 "Common.w"
static void traverseUp(
#if NeedFunctionPrototypes
Widget,XEvent*,String*,Cardinal*
#endif
);
#line 859 "Common.w"
static void traverseLeft(
#if NeedFunctionPrototypes
Widget,XEvent*,String*,Cardinal*
#endif
);
#line 868 "Common.w"
static void traverseRight(
#if NeedFunctionPrototypes
Widget,XEvent*,String*,Cardinal*
#endif
);
#line 880 "Common.w"
static void traverseNext(
#if NeedFunctionPrototypes
Widget,XEvent*,String*,Cardinal*
#endif
);
#line 889 "Common.w"
static void traversePrev(
#if NeedFunctionPrototypes
Widget,XEvent*,String*,Cardinal*
#endif
);
#line 900 "Common.w"
static void traverseNextTop(
#if NeedFunctionPrototypes
Widget,XEvent*,String*,Cardinal*
#endif
);
#line 909 "Common.w"
static void traverseHome(
#if NeedFunctionPrototypes
Widget,XEvent*,String*,Cardinal*
#endif
);
#line 919 "Common.w"
static void traverseCurrent(
#if NeedFunctionPrototypes
Widget,XEvent*,String*,Cardinal*
#endif
);

static XtActionsRec actionsList[] = {
{"focusIn", focusIn},
{"focusOut", focusOut},
{"traverseDown", traverseDown},
{"traverseUp", traverseUp},
{"traverseLeft", traverseLeft},
{"traverseRight", traverseRight},
{"traverseNext", traverseNext},
{"traversePrev", traversePrev},
{"traverseNextTop", traverseNextTop},
{"traverseHome", traverseHome},
{"traverseCurrent", traverseCurrent},
};
static void _resolve_inheritance(
#if NeedFunctionPrototypes
WidgetClass
#endif
);
#line 176 "Common.w"
static void class_initialize(
#if NeedFunctionPrototypes
void
#endif
);
#line 205 "Common.w"
static void initialize(
#if NeedFunctionPrototypes
Widget ,Widget,ArgList ,Cardinal *
#endif
);
#line 234 "Common.w"
static Boolean  set_values(
#if NeedFunctionPrototypes
Widget ,Widget ,Widget,ArgList ,Cardinal *
#endif
);
#line 271 "Common.w"
static void compute_inside(
#if NeedFunctionPrototypes
Widget,Position *,Position *,Dimension *,Dimension *
#endif
);
#line 283 "Common.w"
static Dimension  total_frame_width(
#if NeedFunctionPrototypes
Widget
#endif
);
#line 290 "Common.w"
static void expose(
#if NeedFunctionPrototypes
Widget,XEvent *,Region 
#endif
);
#line 302 "Common.w"
static void destroy(
#if NeedFunctionPrototypes
Widget
#endif
);
#line 320 "Common.w"
static void highlight_border(
#if NeedFunctionPrototypes
Widget
#endif
);
#line 349 "Common.w"
static void unhighlight_border(
#if NeedFunctionPrototypes
Widget
#endif
);
#line 374 "Common.w"
static Boolean  accept_focus(
#if NeedFunctionPrototypes
Widget,Time *
#endif
);
#line 396 "Common.w"
static Boolean  would_accept_focus(
#if NeedFunctionPrototypes
Widget
#endif
);
#line 506 "Common.w"
static void traverse(
#if NeedFunctionPrototypes
Widget,TraversalDirection ,Widget ,Time *
#endif
);
#line 764 "Common.w"
static Boolean  lighter_color(
#if NeedFunctionPrototypes
Widget,Pixel ,Pixel *
#endif
);
#line 774 "Common.w"
static Boolean  darker_color(
#if NeedFunctionPrototypes
Widget,Pixel ,Pixel *
#endif
);
#line 541 "Common.w"
#define done(type, value) do {\
	if (to->addr != NULL) {\
	    if (to->size < sizeof(type)) {\
	        to->size = sizeof(type);\
	        return False;\
	    }\
	    *(type*)(to->addr) = (value);\
        } else {\
	    static type static_val;\
	    static_val = (value);\
	    to->addr = (XtPointer)&static_val;\
        }\
        to->size = sizeof(type);\
        return True;\
    }while (0 )


#line 558 "Common.w"
static Boolean  cvtStringToAlignment(
#if NeedFunctionPrototypes
Display *,XrmValuePtr ,Cardinal *,XrmValuePtr ,XrmValuePtr ,XtPointer *
#endif
);
#line 593 "Common.w"
static Boolean  cvtAlignmentToString(
#if NeedFunctionPrototypes
Display *,XrmValuePtr ,Cardinal *,XrmValuePtr ,XrmValuePtr ,XtPointer *
#endif
);
#line 622 "Common.w"
static char  extraTranslations[] = "\
	<FocusIn>: focusIn()\n\
	<FocusOut>: focusOut()\n\
	<Key>Up: traverseUp()\n\
	<Key>Down: traverseDown()\n\
	<Key>Left: traverseLeft()\n\
	<Key>Right: traverseRight()\n\
	<Key>Next: traverseNext()\n\
	~Shift<Key>Tab: traverseNext()\n\
	<Key>Prior: traversePrev()\n\
	Shift<Key>Tab: traversePrev()\n\
	<Key>KP_Enter: traverseNextTop()\n\
	<Key>Home: traverseHome()";
#line 640 "Common.w"
static void create_bordergc(
#if NeedFunctionPrototypes
Widget
#endif
);
#line 667 "Common.w"
static Boolean  traverse_to_direction(
#if NeedFunctionPrototypes
Widget,TraversalDirection ,int ,int ,Widget *,int *
#endif
);
#line 716 "Common.w"
static void traverse_to_next(
#if NeedFunctionPrototypes
Widget,Widget ,Time *
#endif
);
#line 733 "Common.w"
static void traverse_to_prev(
#if NeedFunctionPrototypes
Widget,Widget ,Time *
#endif
);
#line 746 "Common.w"
static void traverse_to_next_top(
#if NeedFunctionPrototypes
Widget,Widget ,Time *
#endif
);
#line 558 "Common.w"
/*ARGSUSED*/static Boolean  cvtStringToAlignment(display,args,num_args,from,to,converter_data)Display * display;XrmValuePtr  args;Cardinal * num_args;XrmValuePtr  from;XrmValuePtr  to;XtPointer * converter_data;
{
    Alignment a = 0;
    char c, *t, *s = (char*) from->addr;

    if (*num_args != 0)
	XtAppErrorMsg(XtDisplayToApplicationContext(display),
		      "cvtStringToAlignment", "wrongParameters",
		      "XtToolkitError",
		      "String to Alignment conversion needs no arguments",
		      (String*) NULL, (Cardinal*) NULL);

    while (*s) {
	for (; isspace(*s); s++) ;
	for (t = s; *t && ! isspace(*t); t++) ;
	c = *t;
	*t = '\0';
	if (XmuCompareISOLatin1(s, "top") == 0) a |= XfwfTop;
	else if (XmuCompareISOLatin1(s, "bottom") == 0) a |= XfwfBottom;
	else if (XmuCompareISOLatin1(s, "center") == 0) ; /* skip */
	else if (XmuCompareISOLatin1(s, "left") == 0) a |= XfwfLeft;
	else if (XmuCompareISOLatin1(s, "right") == 0) a |= XfwfRight;
	else {
	    XtDisplayStringConversionWarning(display, (char*) from->addr, 
					     "Alignment");
	    break;
	}
	*t = c;
	s = t;
    }
    done(Alignment, a);
}
#line 593 "Common.w"
/*ARGSUSED*/static Boolean  cvtAlignmentToString(display,args,num_args,from,to,converter_data)Display * display;XrmValuePtr  args;Cardinal * num_args;XrmValuePtr  from;XrmValuePtr  to;XtPointer * converter_data;
{
    Alignment *a = (Alignment*) from->addr;

    if (*num_args != 0)
	XtAppErrorMsg(XtDisplayToApplicationContext(display),
		      "cvtAlignmentToString", "wrongParameters",
		      "XtToolkitError",
		      "Alignment to String conversion needs no arguments",
		      (String*) NULL, (Cardinal*) NULL);
    switch (*a) {
    case XfwfCenter: done(String, "center");
    case XfwfBottom: done(String, "bottom");
    case XfwfTop: done(String, "top");
    case XfwfLeft: done(String, "left");
    case XfwfRight: done(String, "right");
    case XfwfBottom + XfwfLeft: done(String, "bottom left");
    case XfwfBottom + XfwfRight: done(String, "bottom right");
    case XfwfTop + XfwfLeft: done(String, "top left");
    case XfwfTop + XfwfRight: done(String, "top right");
    default: done(String, "unknown");
    }
}
#line 640 "Common.w"
/*ARGSUSED*/static void create_bordergc(self)Widget self;
{
    XtGCMask mask;
    XGCValues values;

    if (((XfwfCommonWidget)self)->xfwfCommon.bordergc) XtReleaseGC(self, ((XfwfCommonWidget)self)->xfwfCommon.bordergc);
    if (((XfwfCommonWidget)self)->xfwfCommon.highlightPixmap != None) {
	mask = GCFillStyle | GCTile;
	values.fill_style = FillTiled;
	values.tile = ((XfwfCommonWidget)self)->xfwfCommon.highlightPixmap;
    } else {
	mask = GCFillStyle | GCForeground;
	values.fill_style = FillSolid;
	values.foreground = ((XfwfCommonWidget)self)->xfwfCommon.highlightColor;
    }
    ((XfwfCommonWidget)self)->xfwfCommon.bordergc = XtGetGC(self, mask, &values);
}
#line 667 "Common.w"
/*ARGSUSED*/static Boolean  traverse_to_direction(self,dir,x,y,found,distance)Widget self;TraversalDirection  dir;int  x;int  y;Widget * found;int * distance;
{
    int i;
    Widget child, w;
    Position rx, ry;
    int dist;
    Boolean found_child = False;

    if (! ((XfwfCommonWidget)self)->xfwfCommon.traversalOn) return False;
    /*
     * First recurse to all descendants
     */
    for (i = 0; i < ((XfwfCommonWidget)self)->composite.num_children; i++)
        if (XtIsSubclass(((XfwfCommonWidget)self)->composite.children[i], xfwfCommonWidgetClass)
            && traverse_to_direction(((XfwfCommonWidget)self)->composite.children[i], dir, x, y, found, distance))
            found_child = True;
    if (found_child) return True;
    /*
     * No child found, now check own position and distance
     */
    switch (dir) {
    case TraverseHome: rx = 0; ry = 0; break;
    case TraverseLeft: rx = ((XfwfCommonWidget)self)->core.width; ry = ((XfwfCommonWidget)self)->core.height/2; break;
    case TraverseDown: rx = ((XfwfCommonWidget)self)->core.width/2; ry = 0; break;
    case TraverseRight: rx = 0; ry = ((XfwfCommonWidget)self)->core.height/2; break;
    case TraverseUp: rx = ((XfwfCommonWidget)self)->core.width/2; ry = ((XfwfCommonWidget)self)->core.height; break;
    }
    XtTranslateCoords(self, rx, ry, &rx, &ry);
    if ((dir == TraverseUp && ry > y)
        || (dir == TraverseLeft && rx > x)
        || (dir == TraverseDown && ry < y)
        || (dir == TraverseRight && rx < x)) return False;
    dist = (rx - x)*(rx - x) + (ry - y)*(ry - y);
    if (dist >= *distance) return False;
    /*
     * We are the best so far, but do we want the focus?
     */
    if (! ((XfwfCommonWidgetClass)self->core.widget_class)->xfwfCommon_class.would_accept_focus(self)) return False;
    *distance = dist;
    *found = self;
    return True;
}
#line 716 "Common.w"
/*ARGSUSED*/static void traverse_to_next(self,current,time)Widget self;Widget  current;Time * time;
{
    int i = 0;
    Widget parent = XtParent(self);

    while (i < ((XfwfCommonWidget)self)->composite.num_children && ((XfwfCommonWidget)self)->composite.children[i] != current) i++;
    for (i++; i < ((XfwfCommonWidget)self)->composite.num_children; i++)
        if (XtCallAcceptFocus(((XfwfCommonWidget)self)->composite.children[i], time)) return;
    if (XtIsSubclass(parent, xfwfCommonWidgetClass))
        ((XfwfCommonWidgetClass)parent->core.widget_class)->xfwfCommon_class.traverse(parent, TraverseNext, self, time);
}
#line 733 "Common.w"
/*ARGSUSED*/static void traverse_to_prev(self,current,time)Widget self;Widget  current;Time * time;
{
    int i = 0;
    Widget parent = XtParent(self);

    while (i < ((XfwfCommonWidget)self)->composite.num_children && ((XfwfCommonWidget)self)->composite.children[i] != current) i++;
    for (i--; i >= 0; i--)
        if (XtCallAcceptFocus(((XfwfCommonWidget)self)->composite.children[i], time)) return;
    if (XtIsSubclass(parent, xfwfCommonWidgetClass))
        ((XfwfCommonWidgetClass)parent->core.widget_class)->xfwfCommon_class.traverse(parent, TraversePrev, self, time);
}
#line 746 "Common.w"
/*ARGSUSED*/static void traverse_to_next_top(self,current,time)Widget self;Widget  current;Time * time;
{
    Widget parent = XtParent(self);

    if (XtIsSubclass(parent, xfwfCommonWidgetClass))
	((XfwfCommonWidgetClass)parent->core.widget_class)->xfwfCommon_class.traverse(parent, TraverseNextTop, current, time);
    else
	XtCallCallbackList(self, ((XfwfCommonWidget)self)->xfwfCommon.nextTop, NULL);
}

static XtResource resources[] = {
#line 90 "Common.w"
{XtNtraversalOn,XtCTraversalOn,XtRBoolean,sizeof(((XfwfCommonRec*)NULL)->xfwfCommon.traversalOn),XtOffsetOf(XfwfCommonRec,xfwfCommon.traversalOn),XtRImmediate,(XtPointer)True },
#line 98 "Common.w"
{XtNhighlightThickness,XtCHighlightThickness,XtRDimension,sizeof(((XfwfCommonRec*)NULL)->xfwfCommon.highlightThickness),XtOffsetOf(XfwfCommonRec,xfwfCommon.highlightThickness),XtRImmediate,(XtPointer)2 },
#line 106 "Common.w"
{XtNhighlightColor,XtCHighlightColor,XtRPixel,sizeof(((XfwfCommonRec*)NULL)->xfwfCommon.highlightColor),XtOffsetOf(XfwfCommonRec,xfwfCommon.highlightColor),XtRString,(XtPointer)XtDefaultForeground },
#line 113 "Common.w"
{XtNhighlightPixmap,XtCHighlightPixmap,XtRPixmap,sizeof(((XfwfCommonRec*)NULL)->xfwfCommon.highlightPixmap),XtOffsetOf(XfwfCommonRec,xfwfCommon.highlightPixmap),XtRImmediate,(XtPointer)None },
#line 123 "Common.w"
{XtNnextTop,XtCNextTop,XtRCallback,sizeof(((XfwfCommonRec*)NULL)->xfwfCommon.nextTop),XtOffsetOf(XfwfCommonRec,xfwfCommon.nextTop),XtRImmediate,(XtPointer)NULL },
#line 129 "Common.w"
{XtNuserData,XtCUserData,XtRPointer,sizeof(((XfwfCommonRec*)NULL)->xfwfCommon.userData),XtOffsetOf(XfwfCommonRec,xfwfCommon.userData),XtRImmediate,(XtPointer)NULL },
};

XfwfCommonClassRec xfwfCommonClassRec = {
{ /* core_class part */
(WidgetClass) &compositeClassRec,
"XfwfCommon",
sizeof(XfwfCommonRec),
class_initialize,
_resolve_inheritance,
FALSE,
initialize,
NULL,
XtInheritRealize,
actionsList,
11,
resources,
6,
NULLQUARK,
True ,
XtExposeCompressMultiple ,
True ,
False ,
destroy,
XtInheritResize,
expose,
set_values,
NULL,
XtInheritSetValuesAlmost,
NULL,
accept_focus,
XtVersion,
NULL,
NULL,
XtInheritQueryGeometry,
XtInheritDisplayAccelerator,
NULL 
},
{ /* composite_class part */
XtInheritGeometryManager,
XtInheritChangeManaged,
XtInheritInsertChild,
XtInheritDeleteChild,
NULL
},
{ /* XfwfCommon_class part */
compute_inside,
total_frame_width,
highlight_border,
unhighlight_border,
would_accept_focus,
traverse,
lighter_color,
darker_color,
NULL ,
},
};
WidgetClass xfwfCommonWidgetClass = (WidgetClass) &xfwfCommonClassRec;
/*ARGSUSED*/
#line 804 "Common.w"
static void focusIn(self,event,params,num_params)Widget self;XEvent*event;String*params;Cardinal*num_params;
{
    Time time = CurrentTime;

    if (event->type != FocusIn)
	XtError("focusIn action may only be bound to FocusIn events");
    if (! ((XfwfCommonWidget)self)->xfwfCommon.traversalOn)
	return;
    if (event->xfocus.detail == NotifyAncestor
	|| event->xfocus.detail == NotifyInferior
	|| event->xfocus.detail == NotifyNonlinear) {
	if (! ((XfwfCommonWidget)self)->xfwfCommon.traversal_focus) (void) ((XfwfCommonWidgetClass)self->core.widget_class)->core_class.accept_focus(self, &time);
    } else if (((XfwfCommonWidget)self)->xfwfCommon.traversal_focus) {
	((XfwfCommonWidgetClass)self->core.widget_class)->xfwfCommon_class.unhighlight_border(self);
	((XfwfCommonWidget)self)->xfwfCommon.traversal_focus = False;
    }
}

/*ARGSUSED*/
#line 825 "Common.w"
static void focusOut(self,event,params,num_params)Widget self;XEvent*event;String*params;Cardinal*num_params;
{
    if (event->type != FocusOut)
	XtError("focusOut action may only be bound to FocusOut events");
    if (((XfwfCommonWidget)self)->xfwfCommon.traversal_focus) {
	((XfwfCommonWidgetClass)self->core.widget_class)->xfwfCommon_class.unhighlight_border(self);
	((XfwfCommonWidget)self)->xfwfCommon.traversal_focus = False;
    }
}

/*ARGSUSED*/
#line 841 "Common.w"
static void traverseDown(self,event,params,num_params)Widget self;XEvent*event;String*params;Cardinal*num_params;
{
    ((XfwfCommonWidgetClass)self->core.widget_class)->xfwfCommon_class.traverse(self, TraverseDown, self, &event->xkey.time);
}

/*ARGSUSED*/
#line 850 "Common.w"
static void traverseUp(self,event,params,num_params)Widget self;XEvent*event;String*params;Cardinal*num_params;
{
    ((XfwfCommonWidgetClass)self->core.widget_class)->xfwfCommon_class.traverse(self, TraverseUp, self, &event->xkey.time);
}

/*ARGSUSED*/
#line 859 "Common.w"
static void traverseLeft(self,event,params,num_params)Widget self;XEvent*event;String*params;Cardinal*num_params;
{
    ((XfwfCommonWidgetClass)self->core.widget_class)->xfwfCommon_class.traverse(self, TraverseLeft, self, &event->xkey.time);
}

/*ARGSUSED*/
#line 868 "Common.w"
static void traverseRight(self,event,params,num_params)Widget self;XEvent*event;String*params;Cardinal*num_params;
{
    ((XfwfCommonWidgetClass)self->core.widget_class)->xfwfCommon_class.traverse(self, TraverseRight, self, &event->xkey.time);
}

/*ARGSUSED*/
#line 880 "Common.w"
static void traverseNext(self,event,params,num_params)Widget self;XEvent*event;String*params;Cardinal*num_params;
{
    ((XfwfCommonWidgetClass)self->core.widget_class)->xfwfCommon_class.traverse(self, TraverseNext, self, &event->xkey.time);
}

/*ARGSUSED*/
#line 889 "Common.w"
static void traversePrev(self,event,params,num_params)Widget self;XEvent*event;String*params;Cardinal*num_params;
{
    ((XfwfCommonWidgetClass)self->core.widget_class)->xfwfCommon_class.traverse(self, TraversePrev, self, &event->xkey.time);
}

/*ARGSUSED*/
#line 900 "Common.w"
static void traverseNextTop(self,event,params,num_params)Widget self;XEvent*event;String*params;Cardinal*num_params;
{
    ((XfwfCommonWidgetClass)self->core.widget_class)->xfwfCommon_class.traverse(self, TraverseNextTop, self, &event->xkey.time);
}

/*ARGSUSED*/
#line 909 "Common.w"
static void traverseHome(self,event,params,num_params)Widget self;XEvent*event;String*params;Cardinal*num_params;
{
    ((XfwfCommonWidgetClass)self->core.widget_class)->xfwfCommon_class.traverse(self, TraverseHome, self, &event->xkey.time);
}

/*ARGSUSED*/
#line 919 "Common.w"
static void traverseCurrent(self,event,params,num_params)Widget self;XEvent*event;String*params;Cardinal*num_params;
{
    Time time = CurrentTime;

    if (((XfwfCommonWidget)self)->xfwfCommon.traversalOn) (void) ((XfwfCommonWidgetClass)self->core.widget_class)->core_class.accept_focus(self, &time);
}

static void _resolve_inheritance(class)
WidgetClass class;
{
  XfwfCommonWidgetClass c = (XfwfCommonWidgetClass) class;
  XfwfCommonWidgetClass super;
  static CompositeClassExtensionRec extension_rec = {
    NULL, NULLQUARK, XtCompositeExtensionVersion,
    sizeof(CompositeClassExtensionRec), True};
  CompositeClassExtensionRec *ext;
  ext = (CompositeClassExtensionRec *)XtMalloc(sizeof(*ext));
  *ext = extension_rec;
  ext->next_extension = c->composite_class.extension;
  c->composite_class.extension = (XtPointer)ext;
  if (class == xfwfCommonWidgetClass) return;
  super = (XfwfCommonWidgetClass)class->core_class.superclass;
  if (c->xfwfCommon_class.compute_inside == XtInherit_compute_inside)
    c->xfwfCommon_class.compute_inside = super->xfwfCommon_class.compute_inside;
  if (c->xfwfCommon_class.total_frame_width == XtInherit_total_frame_width)
    c->xfwfCommon_class.total_frame_width = super->xfwfCommon_class.total_frame_width;
  if (c->xfwfCommon_class.highlight_border == XtInherit_highlight_border)
    c->xfwfCommon_class.highlight_border = super->xfwfCommon_class.highlight_border;
  if (c->xfwfCommon_class.unhighlight_border == XtInherit_unhighlight_border)
    c->xfwfCommon_class.unhighlight_border = super->xfwfCommon_class.unhighlight_border;
  if (c->xfwfCommon_class.would_accept_focus == XtInherit_would_accept_focus)
    c->xfwfCommon_class.would_accept_focus = super->xfwfCommon_class.would_accept_focus;
  if (c->xfwfCommon_class.traverse == XtInherit_traverse)
    c->xfwfCommon_class.traverse = super->xfwfCommon_class.traverse;
  if (c->xfwfCommon_class.lighter_color == XtInherit_lighter_color)
    c->xfwfCommon_class.lighter_color = super->xfwfCommon_class.lighter_color;
  if (c->xfwfCommon_class.darker_color == XtInherit_darker_color)
    c->xfwfCommon_class.darker_color = super->xfwfCommon_class.darker_color;
}
#line 176 "Common.w"
/*ARGSUSED*/static void class_initialize()
{
#ifndef NO_XPM
    static XtConvertArgRec args[] = {
    { XtWidgetBaseOffset, 0, sizeof(Widget) } };
#endif

    XtSetTypeConverter(XtRString, "Alignment", cvtStringToAlignment,
		       NULL, 0, XtCacheNone, NULL);
    XtSetTypeConverter("Alignment", XtRString, cvtAlignmentToString,
		       NULL, 0, XtCacheNone, NULL);
    /* init_icon_quarks(); */
#ifndef NO_XPM
    XtSetTypeConverter(XtRString, "Icon", cvtStringToIcon,
		       args, XtNumber(args), XtCacheNone, NULL);
#endif
    XtSetTypeConverter(XtRString, "StringArray", cvtStringToStringArray,
		       NULL, 0, XtCacheNone, NULL);
}
#line 205 "Common.w"
/*ARGSUSED*/static void initialize(request,self,args,num_args)Widget  request;Widget self;ArgList  args;Cardinal * num_args;
{
    Dimension frame;

    if (((XfwfCommonWidgetClass)self->core.widget_class)->xfwfCommon_class.traversal_trans == NULL)
	((XfwfCommonWidgetClass)self->core.widget_class)->xfwfCommon_class.traversal_trans = XtParseTranslationTable(extraTranslations);
    if (((XfwfCommonWidget)self)->xfwfCommon.traversalOn) {
	XtAugmentTranslations(self, ((XfwfCommonWidgetClass)self->core.widget_class)->xfwfCommon_class.traversal_trans);
	((XfwfCommonWidgetClass)self->core.widget_class)->core_class.visible_interest = True;
    }
    ((XfwfCommonWidget)self)->xfwfCommon.traversal_focus = False;
    ((XfwfCommonWidget)self)->xfwfCommon.bordergc = NULL;
    frame = ((XfwfCommonWidgetClass)self->core.widget_class)->xfwfCommon_class.total_frame_width(self);
    if (((XfwfCommonWidget)self)->core.width < 2 * frame) ((XfwfCommonWidget)self)->core.width = 2 * frame;
    if (((XfwfCommonWidget)self)->core.height < 2 * frame) ((XfwfCommonWidget)self)->core.height = 2 * frame;
    if (((XfwfCommonWidget)self)->core.width == 0) ((XfwfCommonWidget)self)->core.width = 2;
    if (((XfwfCommonWidget)self)->core.height == 0) ((XfwfCommonWidget)self)->core.height = 2;
    create_bordergc(self);
}
#line 234 "Common.w"
/*ARGSUSED*/static Boolean  set_values(old,request,self,args,num_args)Widget  old;Widget  request;Widget self;ArgList  args;Cardinal * num_args;
{
    Boolean need_redraw = False;
    Widget parent = XtParent(self);
    Time time = CurrentTime;

    if (((XfwfCommonWidget)self)->xfwfCommon.traversalOn != ((XfwfCommonWidget)old)->xfwfCommon.traversalOn && ((XfwfCommonWidget)self)->xfwfCommon.traversalOn) {
	XtAugmentTranslations(self, ((XfwfCommonWidgetClass)self->core.widget_class)->xfwfCommon_class.traversal_trans); 
	((XfwfCommonWidgetClass)self->core.widget_class)->core_class.visible_interest = True;
    }
    if ((((XfwfCommonWidget)self)->core.sensitive != ((XfwfCommonWidget)old)->core.sensitive
	 || ((XfwfCommonWidget)self)->core.ancestor_sensitive != ((XfwfCommonWidget)old)->core.ancestor_sensitive
	 || ((XfwfCommonWidget)self)->xfwfCommon.traversalOn != ((XfwfCommonWidget)old)->xfwfCommon.traversalOn)
	&& ((XfwfCommonWidget)self)->xfwfCommon.traversal_focus) {
	if (XtIsSubclass(parent, xfwfCommonWidgetClass))
	    ((XfwfCommonWidgetClass)parent->core.widget_class)->xfwfCommon_class.traverse(parent, TraverseHome, self, &time);
    }
    if (((XfwfCommonWidget)self)->xfwfCommon.highlightThickness != ((XfwfCommonWidget)old)->xfwfCommon.highlightThickness)
	need_redraw = True;
    if (((XfwfCommonWidget)self)->xfwfCommon.highlightPixmap != ((XfwfCommonWidget)old)->xfwfCommon.highlightPixmap) {
	create_bordergc(self);
	need_redraw = True;
    } else if (((XfwfCommonWidget)self)->xfwfCommon.highlightColor != ((XfwfCommonWidget)old)->xfwfCommon.highlightColor) {
	((XfwfCommonWidget)self)->xfwfCommon.highlightPixmap = None;
	create_bordergc(self);
	need_redraw = True;
    }
    return need_redraw;
}
#line 271 "Common.w"
/*ARGSUSED*/static void compute_inside(self,x,y,w,h)Widget self;Position * x;Position * y;Dimension * w;Dimension * h;
{
    *x = ((XfwfCommonWidget)self)->xfwfCommon.highlightThickness;
    *y = ((XfwfCommonWidget)self)->xfwfCommon.highlightThickness;
    *w = ((XfwfCommonWidget)self)->core.width - 2 * ((XfwfCommonWidget)self)->xfwfCommon.highlightThickness;
    *h = ((XfwfCommonWidget)self)->core.height - 2 * ((XfwfCommonWidget)self)->xfwfCommon.highlightThickness;
}
#line 283 "Common.w"
/*ARGSUSED*/static Dimension  total_frame_width(self)Widget self;
{
    return ((XfwfCommonWidget)self)->xfwfCommon.highlightThickness;
}
#line 290 "Common.w"
/*ARGSUSED*/static void expose(self,event,region)Widget self;XEvent * event;Region  region;
{
    if (! XtIsRealized(self)) return;
    if (region != NULL) XSetRegion(XtDisplay(self), ((XfwfCommonWidget)self)->xfwfCommon.bordergc, region);
    if (((XfwfCommonWidget)self)->xfwfCommon.traversal_focus) ((XfwfCommonWidgetClass)self->core.widget_class)->xfwfCommon_class.highlight_border(self);
    if (region != NULL) XSetClipMask(XtDisplay(self), ((XfwfCommonWidget)self)->xfwfCommon.bordergc, None);
}
#line 302 "Common.w"
/*ARGSUSED*/static void destroy(self)Widget self;
{
    Widget parent = XtParent(self);
    Time time = CurrentTime;

    if (((XfwfCommonWidget)self)->xfwfCommon.traversal_focus) {
	((XfwfCommonWidget)self)->core.sensitive = False;
	if (XtIsSubclass(parent, xfwfCommonWidgetClass))
	    ((XfwfCommonWidgetClass)parent->core.widget_class)->xfwfCommon_class.traverse(parent, TraverseHome, self, &time);
    }
}
#line 320 "Common.w"
/*ARGSUSED*/static void highlight_border(self)Widget self;
{
    XRectangle rect[4];

    if (((XfwfCommonWidget)self)->xfwfCommon.highlightThickness == 0) return;

    rect[0].x = 0;
    rect[0].y = 0;
    rect[0].width = ((XfwfCommonWidget)self)->core.width;
    rect[0].height = ((XfwfCommonWidget)self)->xfwfCommon.highlightThickness;

    rect[1].x = 0;
    rect[1].y = 0;
    rect[1].width = ((XfwfCommonWidget)self)->xfwfCommon.highlightThickness;
    rect[1].height = ((XfwfCommonWidget)self)->core.height;

    rect[2].x = ((XfwfCommonWidget)self)->core.width - ((XfwfCommonWidget)self)->xfwfCommon.highlightThickness;
    rect[2].y = 0;
    rect[2].width = ((XfwfCommonWidget)self)->xfwfCommon.highlightThickness;
    rect[2].height = ((XfwfCommonWidget)self)->core.height;

    rect[3].x = 0;
    rect[3].y = ((XfwfCommonWidget)self)->core.height - ((XfwfCommonWidget)self)->xfwfCommon.highlightThickness;
    rect[3].width = ((XfwfCommonWidget)self)->core.width;
    rect[3].height = ((XfwfCommonWidget)self)->xfwfCommon.highlightThickness;

    XFillRectangles(XtDisplay(self), XtWindow(self), ((XfwfCommonWidget)self)->xfwfCommon.bordergc, &rect[0], 4);
}
#line 349 "Common.w"
/*ARGSUSED*/static void unhighlight_border(self)Widget self;
{
    if (((XfwfCommonWidget)self)->xfwfCommon.highlightThickness == 0) return;

    XClearArea(XtDisplay(self), XtWindow(self), 
	       0, 0, ((XfwfCommonWidget)self)->core.width, ((XfwfCommonWidget)self)->xfwfCommon.highlightThickness, False);
    XClearArea(XtDisplay(self), XtWindow(self),
	       0, 0, ((XfwfCommonWidget)self)->xfwfCommon.highlightThickness, ((XfwfCommonWidget)self)->core.height, False);
    XClearArea(XtDisplay(self), XtWindow(self),
	       ((XfwfCommonWidget)self)->core.width - ((XfwfCommonWidget)self)->xfwfCommon.highlightThickness, 0, 
	       ((XfwfCommonWidget)self)->xfwfCommon.highlightThickness, ((XfwfCommonWidget)self)->core.height, False);
    XClearArea(XtDisplay(self), XtWindow(self),
	       0, ((XfwfCommonWidget)self)->core.height - ((XfwfCommonWidget)self)->xfwfCommon.highlightThickness,
	       ((XfwfCommonWidget)self)->core.width, ((XfwfCommonWidget)self)->xfwfCommon.highlightThickness, False);
}
#line 374 "Common.w"
/*ARGSUSED*/static Boolean  accept_focus(self,time)Widget self;Time * time;
{
    int i;

    if (! XtIsRealized(self) || ! ((XfwfCommonWidget)self)->core.sensitive || ! ((XfwfCommonWidget)self)->xfwfCommon.traversalOn
	|| ! ((XfwfCommonWidget)self)->core.visible || ! ((XfwfCommonWidget)self)->core.ancestor_sensitive || ! ((XfwfCommonWidget)self)->core.managed
	|| ! ((XfwfCommonWidget)self)->core.mapped_when_managed || ((XfwfCommonWidget)self)->core.being_destroyed) return False;
    for (i = 0; i < ((XfwfCommonWidget)self)->composite.num_children; i++)
        if (XtCallAcceptFocus(((XfwfCommonWidget)self)->composite.children[i], time)) return True;
    if (! ((XfwfCommonWidget)self)->xfwfCommon.traversal_focus) {
	XSetInputFocus(XtDisplay(self), XtWindow(self), RevertToParent, *time);
	((XfwfCommonWidget)self)->xfwfCommon.traversal_focus = True;
	((XfwfCommonWidgetClass)self->core.widget_class)->xfwfCommon_class.highlight_border(self);
    }
    return True;
}
#line 396 "Common.w"
/*ARGSUSED*/static Boolean  would_accept_focus(self)Widget self;
{
    int i;
    Widget child;

    if (! XtIsRealized(self) || ! ((XfwfCommonWidget)self)->core.sensitive || ! ((XfwfCommonWidget)self)->core.visible || ! ((XfwfCommonWidget)self)->xfwfCommon.traversalOn)
        return False;
    else {
        for (i = 0; i < ((XfwfCommonWidget)self)->composite.num_children; i++) {
            child = ((XfwfCommonWidget)self)->composite.children[i];
            if (XtIsSubclass(child, xfwfCommonWidgetClass)
                && ((XfwfCommonWidgetClass)child->core.widget_class)->xfwfCommon_class.would_accept_focus(child))
                return False;
	}
        return True;
    }
}
#define LARGE_NUMBER 2000000000 


#line 506 "Common.w"
/*ARGSUSED*/static void traverse(self,dir,current,time)Widget self;TraversalDirection  dir;Widget  current;Time * time;
{
    Widget w, parent = XtParent(self);
    Position x, y;
    int distance = LARGE_NUMBER;

    if (dir == TraverseNextTop)
	traverse_to_next_top(self, current, time);
    else if (dir == TraverseNext)
        traverse_to_next(self, current, time);
    else if (dir == TraversePrev)
        traverse_to_prev(self, current, time);
    else if (XtIsSubclass(parent, xfwfCommonWidgetClass))
        ((XfwfCommonWidgetClass)parent->core.widget_class)->xfwfCommon_class.traverse(parent, dir, current, time);
    else {
        switch (dir) {
        case TraverseHome: x = 0; y = 0; break;
        case TraverseLeft: x = 0; y = ((XfwfCommonWidget)current)->core.height/2; break;
        case TraverseDown: x = ((XfwfCommonWidget)current)->core.width/2; y = ((XfwfCommonWidget)current)->core.height; break;
        case TraverseRight: x = ((XfwfCommonWidget)current)->core.width; y = ((XfwfCommonWidget)current)->core.height/2; break;
        case TraverseUp: x = ((XfwfCommonWidget)current)->core.width/2; y = 0; break;
        }
        if (dir != TraverseHome) XtTranslateCoords(current, x, y, &x, &y);
        if (traverse_to_direction(self, dir, x, y, &w, &distance))
            XtCallAcceptFocus(w, time);
    }
}
#line 764 "Common.w"
/*ARGSUSED*/static Boolean  lighter_color(self,base,result)Widget self;Pixel  base;Pixel * result;
{
    return choose_color(self, 1.35, base, result);
}
#line 774 "Common.w"
/*ARGSUSED*/static Boolean  darker_color(self,base,result)Widget self;Pixel  base;Pixel * result;
{
    return choose_color(self, 0.6, base, result);
}
#line 29 "Common.w"
#line 34 "Common.w"
#line 35 "Common.w"
#line 36 "Common.w"
#line 37 "Common.w"
#line 38 "Common.w"
#line 44 "Common.w"
#line 45 "Common.w"
#line 46 "Common.w"
#line 47 "Common.w"
#line 53 "Common.w"
#line 62 "Common.w"
/*ARGSUSED*/void XfwfCallComputeInside(self,x,y,w,h)Widget self;Position * x;Position * y;Dimension * w;Dimension * h;
{
    if (XtIsSubclass(self, xfwfCommonWidgetClass) && ((XfwfCommonWidgetClass)self->core.widget_class)->xfwfCommon_class.compute_inside)
	((XfwfCommonWidgetClass)self->core.widget_class)->xfwfCommon_class.compute_inside(self, x, y, w, h);
    else 
	XtWarning
	    ("XfwfCallComputeInside only applies to subclasses of Common");
}
#line 75 "Common.w"
/*ARGSUSED*/Dimension  XfwfCallFrameWidth(self)Widget self;
{
    if (XtIsSubclass(self, xfwfCommonWidgetClass) && ((XfwfCommonWidgetClass)self->core.widget_class)->xfwfCommon_class.total_frame_width)
	return ((XfwfCommonWidgetClass)self->core.widget_class)->xfwfCommon_class.total_frame_width(self);
    else 
	XtWarning
	    ("XfwfCallFrameWidth only applies to subclasses of Common");
}
