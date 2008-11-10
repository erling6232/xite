/* Generated by wbuild from "Group.w"
** (generator version $Revision$ of $Date$)
*/
#include <X11/IntrinsicP.h>
#include <X11/StringDefs.h>
/* #line 429 "Group.w" */
#include <Xfwf/Toggle.h>
/* #line 430 "Group.w" */
#include <stdio.h>
/* #line 431 "Group.w" */
#include <X11/Xmu/Converters.h>
#include <X11/Xmu/CharSet.h>
/* #line 432 "Group.w" */
#include <Xfwf/Converters.h>
#include <Xfwf/GroupP.h>
static void _resolve_inheritance(
#if NeedFunctionPrototypes
WidgetClass
#endif
);
/* #line 147 "Group.w" */
static void class_initialize(
#if NeedFunctionPrototypes
void
#endif
);
/* #line 160 "Group.w" */
static void initialize(
#if NeedFunctionPrototypes
Widget ,Widget,ArgList ,Cardinal *
#endif
);
/* #line 177 "Group.w" */
static Boolean  set_values(
#if NeedFunctionPrototypes
Widget ,Widget ,Widget,ArgList ,Cardinal *
#endif
);
/* #line 203 "Group.w" */
static void expose(
#if NeedFunctionPrototypes
Widget,XEvent *,Region 
#endif
);
/* #line 230 "Group.w" */
static void insert_child(
#if NeedFunctionPrototypes
Widget 
#endif
);
/* #line 256 "Group.w" */
static void make_textgc(
#if NeedFunctionPrototypes
Widget
#endif
);
/* #line 274 "Group.w" */
static void on_cb(
#if NeedFunctionPrototypes
Widget ,XtPointer ,XtPointer 
#endif
);
/* #line 307 "Group.w" */
static void off_cb(
#if NeedFunctionPrototypes
Widget ,XtPointer ,XtPointer 
#endif
);
/* #line 333 "Group.w" */
static void set_toggles(
#if NeedFunctionPrototypes
Widget
#endif
);
/* #line 358 "Group.w" */
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


/* #line 375 "Group.w" */
static Boolean  cvtStringToSelectionType(
#if NeedFunctionPrototypes
Display *,XrmValuePtr ,Cardinal *,XrmValuePtr ,XrmValuePtr ,XtPointer *
#endif
);
/* #line 407 "Group.w" */
static Boolean  cvtSelectionTypeToString(
#if NeedFunctionPrototypes
Display *,XrmValuePtr ,Cardinal *,XrmValuePtr ,XrmValuePtr ,XtPointer *
#endif
);
/* #line 256 "Group.w" */
/*ARGSUSED*/static void make_textgc(self)Widget self;
{
    XtGCMask mask;
    XGCValues values;

    if (((XfwfGroupWidget)self)->xfwfGroup.textgc != NULL) XtReleaseGC(self, ((XfwfGroupWidget)self)->xfwfGroup.textgc);
    values.background = ((XfwfGroupWidget)self)->core.background_pixel;
    values.foreground = ((XfwfGroupWidget)self)->xfwfGroup.foreground;
    values.font = ((XfwfGroupWidget)self)->xfwfGroup.font->fid;
    mask = GCFont | GCBackground | GCForeground;
    ((XfwfGroupWidget)self)->xfwfGroup.textgc = XtGetGC(self, mask, &values);
}
/* #line 274 "Group.w" */
/*ARGSUSED*/static void on_cb(toggle,client_data,call_data)Widget  toggle;XtPointer  client_data;XtPointer  call_data;
{
    Widget self = XtParent(toggle);
    Cardinal toggle_ord = (Cardinal) client_data;
    Cardinal t, i, bits = sizeof(((XfwfGroupWidget)self)->xfwfGroup.selection) * 8;

    switch (((XfwfGroupWidget)self)->xfwfGroup.selectionStyle) {
    case XfwfMultipleSelection:
	if (toggle_ord < bits) ((XfwfGroupWidget)self)->xfwfGroup.selection |= 1L << toggle_ord;
	break;
    case XfwfSingleSelection:
    case XfwfOneSelection:
	if (((XfwfGroupWidget)self)->xfwfGroup.selection != -1L)
	    for (t = 0, i = 0; i < ((XfwfGroupWidget)self)->composite.num_children; i++)
		if (XtIsSubclass(((XfwfGroupWidget)self)->composite.children[i], xfwfToggleWidgetClass)) {
		    if (((XfwfGroupWidget)self)->xfwfGroup.selection == t) {
			XtVaSetValues(((XfwfGroupWidget)self)->composite.children[i], XtNon, False, NULL);
			break;
		    }
		    t++;
		}
	((XfwfGroupWidget)self)->xfwfGroup.selection = toggle_ord;
	break;
    default: ;
    }
    XtCallCallbackList(self, ((XfwfGroupWidget)self)->xfwfGroup.activate, (XtPointer) ((XfwfGroupWidget)self)->xfwfGroup.selection);
}
/* #line 307 "Group.w" */
/*ARGSUSED*/static void off_cb(toggle,client_data,call_data)Widget  toggle;XtPointer  client_data;XtPointer  call_data;
{
    Widget self = XtParent(toggle);
    Cardinal toggle_ord = (Cardinal) client_data;
    Cardinal t, i, bits = sizeof(((XfwfGroupWidget)self)->xfwfGroup.selection) * 8;

    switch (((XfwfGroupWidget)self)->xfwfGroup.selectionStyle) {
    case XfwfOneSelection:
	XtVaSetValues(toggle, XtNon, True, NULL); /* Undo */
	break;
    case XfwfSingleSelection:
	((XfwfGroupWidget)self)->xfwfGroup.selection = -1L;			/* Nothing selected */
	break;
    case XfwfMultipleSelection:
	if (toggle_ord < bits) ((XfwfGroupWidget)self)->xfwfGroup.selection &= ~(1L << toggle_ord);
	break;
    default: ;
    }
    XtCallCallbackList(self, ((XfwfGroupWidget)self)->xfwfGroup.activate, (XtPointer) ((XfwfGroupWidget)self)->xfwfGroup.selection);
}
/* #line 333 "Group.w" */
/*ARGSUSED*/static void set_toggles(self)Widget self;
{
    Cardinal i, t;

    for (t = 0, i = 0; i < ((XfwfGroupWidget)self)->composite.num_children; i++)
	if (XtIsSubclass(((XfwfGroupWidget)self)->composite.children[i], xfwfToggleWidgetClass)) {
	    switch (((XfwfGroupWidget)self)->xfwfGroup.selectionStyle) {
	    case XfwfNoSelection:
		break;
	    case XfwfSingleSelection:
	    case XfwfOneSelection:
		XtVaSetValues(((XfwfGroupWidget)self)->composite.children[i], XtNon, t == ((XfwfGroupWidget)self)->xfwfGroup.selection, NULL);
		break;
	    case XfwfMultipleSelection: {
	      Boolean state;

	      XtVaGetValues(((XfwfGroupWidget)self)->composite.children[i], XtNon, &state, NULL);
	      if (state)
		((XfwfGroupWidget)self)->xfwfGroup.selection |=
		  (1L << t);

		XtVaSetValues(((XfwfGroupWidget)self)->composite.children[i],
			      XtNon, (((XfwfGroupWidget)self)->xfwfGroup.selection & (1L<<t)) != 0, NULL);
	        }
		break;
	    }
	    t++;
	}
}
/* #line 375 "Group.w" */
/*ARGSUSED*/static Boolean  cvtStringToSelectionType(display,args,num_args,from,to,converter_data)Display * display;XrmValuePtr  args;Cardinal * num_args;XrmValuePtr  from;XrmValuePtr  to;XtPointer * converter_data;
{
    String s = (String) from->addr;

    if (*num_args != 0)
	XtAppErrorMsg(XtDisplayToApplicationContext(display),
		      "cvtStringToSelectionType", "wrongParameters",
		      "XtToolkitError",
		      "String to SelectionType conversion needs no arguments",
		      (String*) NULL, (Cardinal*) NULL);

    if (XmuCompareISOLatin1(s, "no") == 0)
	done(SelectionType, XfwfNoSelection);
    if (XmuCompareISOLatin1(s, "none") == 0)
	done(SelectionType, XfwfNoSelection);
    if (XmuCompareISOLatin1(s, "single") == 0)
	done(SelectionType, XfwfSingleSelection);
    if (XmuCompareISOLatin1(s, "one") == 0)
	done(SelectionType, XfwfOneSelection);
    if (XmuCompareISOLatin1(s, "multi") == 0)
	done(SelectionType, XfwfMultipleSelection);
    if (XmuCompareISOLatin1(s, "multiple") == 0)
	done(SelectionType, XfwfMultipleSelection);

    XtDisplayStringConversionWarning(display, s, XtRSelectionType);
    done(SelectionType, XfwfSingleSelection);
}
/* #line 407 "Group.w" */
/*ARGSUSED*/static Boolean  cvtSelectionTypeToString(display,args,num_args,from,to,converter_data)Display * display;XrmValuePtr  args;Cardinal * num_args;XrmValuePtr  from;XrmValuePtr  to;XtPointer * converter_data;
{
    char s[30];

    if (*num_args != 0)
	XtAppErrorMsg(XtDisplayToApplicationContext(display),
		      "cvtStringToSelectionStyle", "wrongParameters",
		      "XtToolkitError",
		      "String to SelectionStyle conversion needs no arguments",
		      (String*) NULL, (Cardinal*) NULL);
    switch (*((SelectionType*) from->addr)) {
    case XfwfNoSelection: done(String, "none");
    case XfwfSingleSelection: done(String, "single");
    case XfwfOneSelection: done(String, "one");
    case XfwfMultipleSelection: done(String, "multiple");
    }
    XtDisplayStringConversionWarning(display, s, XtRSelectionType);
    done(String, "none");
}

static XtResource resources[] = {
/* #line 70 "Group.w" */
{XtNlabel,XtCLabel,XtRString,sizeof(((XfwfGroupRec*)NULL)->xfwfGroup.label),XtOffsetOf(XfwfGroupRec,xfwfGroup.label),XtRImmediate,(XtPointer)NULL },
/* #line 74 "Group.w" */
{XtNfont,XtCFont,XtRFontStruct,sizeof(((XfwfGroupRec*)NULL)->xfwfGroup.font),XtOffsetOf(XfwfGroupRec,xfwfGroup.font),XtRString,(XtPointer)XtDefaultFont },
/* #line 78 "Group.w" */
{XtNforeground,XtCForeground,XtRPixel,sizeof(((XfwfGroupRec*)NULL)->xfwfGroup.foreground),XtOffsetOf(XfwfGroupRec,xfwfGroup.foreground),XtRString,(XtPointer)XtDefaultForeground },
/* #line 88 "Group.w" */
{XtNselectionStyle,XtCSelectionStyle,XtRSelectionType,sizeof(((XfwfGroupRec*)NULL)->xfwfGroup.selectionStyle),XtOffsetOf(XfwfGroupRec,xfwfGroup.selectionStyle),XtRImmediate,(XtPointer)XfwfSingleSelection },
/* #line 100 "Group.w" */
{XtNselection,XtCSelection,XtRLong,sizeof(((XfwfGroupRec*)NULL)->xfwfGroup.selection),XtOffsetOf(XfwfGroupRec,xfwfGroup.selection),XtRImmediate,(XtPointer)0 },
/* #line 108 "Group.w" */
{XtNactivate,XtCActivate,XtRCallback,sizeof(((XfwfGroupRec*)NULL)->xfwfGroup.activate),XtOffsetOf(XfwfGroupRec,xfwfGroup.activate),XtRImmediate,(XtPointer)NULL },
/* #line 115 "Group.w" */
{XtNframeType,XtCFrameType,XtRFrameType,sizeof(((XfwfGroupRec*)NULL)->xfwfFrame.frameType),XtOffsetOf(XfwfGroupRec,xfwfFrame.frameType),XtRImmediate,(XtPointer)XfwfChiseled },
/* #line 121 "Group.w" */
{XtNinnerOffset,XtCInnerOffset,XtRDimension,sizeof(((XfwfGroupRec*)NULL)->xfwfFrame.innerOffset),XtOffsetOf(XfwfGroupRec,xfwfFrame.innerOffset),XtRImmediate,(XtPointer)3 },
};

XfwfGroupClassRec xfwfGroupClassRec = {
{ /* core_class part */
(WidgetClass) &xfwfRowColClassRec,
"XfwfGroup",
sizeof(XfwfGroupRec),
class_initialize,
_resolve_inheritance,
FALSE,
initialize,
NULL,
XtInheritRealize,
NULL,
0,
resources,
8,
NULLQUARK,
True ,
XtExposeCompressMultiple ,
True ,
False ,
NULL,
XtInheritResize,
expose,
set_values,
NULL,
XtInheritSetValuesAlmost,
NULL,
XtInheritAcceptFocus,
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
insert_child,
XtInheritDeleteChild,
NULL
},
{ /* XfwfCommon_class part */
XtInherit_compute_inside,
XtInherit_total_frame_width,
XtInherit_highlight_border,
XtInherit_unhighlight_border,
XtInherit_would_accept_focus,
XtInherit_traverse,
XtInherit_lighter_color,
XtInherit_darker_color,
NULL ,
},
{ /* XfwfFrame_class part */
0
},
{ /* XfwfBoard_class part */
XtInherit_set_abs_location,
},
{ /* XfwfRowCol_class part */
XtInherit_layout,
},
{ /* XfwfGroup_class part */
0
},
};
WidgetClass xfwfGroupWidgetClass = (WidgetClass) &xfwfGroupClassRec;
static void _resolve_inheritance(class)
WidgetClass class;
{
  XfwfGroupWidgetClass c = (XfwfGroupWidgetClass) class;
  XfwfGroupWidgetClass super;
  static CompositeClassExtensionRec extension_rec = {
    NULL, NULLQUARK, XtCompositeExtensionVersion,
    sizeof(CompositeClassExtensionRec), True};
  CompositeClassExtensionRec *ext;
  ext = (XtPointer)XtMalloc(sizeof(*ext));
  *ext = extension_rec;
  ext->next_extension = c->composite_class.extension;
  c->composite_class.extension = ext;
  if (class == xfwfGroupWidgetClass) return;
  super = (XfwfGroupWidgetClass)class->core_class.superclass;
}
/* #line 147 "Group.w" */
/*ARGSUSED*/static void class_initialize()
{
    XtAddConverter(XtRString, XtRLong, XmuCvtStringToLong, NULL, 0);
    XtSetTypeConverter(XtRLong, XtRString, XfwfCvtLongToString,
		       NULL, 0, XtCacheNone, NULL);
    XtSetTypeConverter(XtRString, XtRSelectionType, cvtStringToSelectionType,
		       NULL, 0, XtCacheNone, NULL);
    XtSetTypeConverter(XtRSelectionType, XtRString, cvtSelectionTypeToString,
		       NULL, 0, XtCacheNone, NULL);
}
/* #line 160 "Group.w" */
/*ARGSUSED*/static void initialize(request,self,args,num_args)Widget  request;Widget self;ArgList  args;Cardinal * num_args;
{
    ((XfwfGroupWidget)self)->xfwfGroup.toggle_ord = 0;
    ((XfwfGroupWidget)self)->xfwfGroup.textgc = NULL;
    make_textgc(self);
    if (((XfwfGroupWidget)self)->xfwfGroup.selectionStyle == XfwfOneSelection && ((XfwfGroupWidget)self)->xfwfGroup.selection == -1L) {
	XtWarning
	    ("Illegal combination of selectionStyle and selection resources");
	((XfwfGroupWidget)self)->xfwfGroup.selection = 0;
    }
}
/* #line 177 "Group.w" */
/*ARGSUSED*/static Boolean  set_values(old,request,self,args,num_args)Widget  old;Widget  request;Widget self;ArgList  args;Cardinal * num_args;
{
    Boolean need_redraw = False;

    if (((XfwfGroupWidget)old)->xfwfGroup.label != ((XfwfGroupWidget)self)->xfwfGroup.label) {
	XtFree(((XfwfGroupWidget)old)->xfwfGroup.label);
	((XfwfGroupWidget)self)->xfwfGroup.label = XtNewString(((XfwfGroupWidget)self)->xfwfGroup.label);
	need_redraw = True;
    }
    if (((XfwfGroupWidget)self)->xfwfGroup.font != ((XfwfGroupWidget)old)->xfwfGroup.font) {
	make_textgc(self);
	if (((XfwfGroupWidget)self)->xfwfGroup.label != NULL) need_redraw = True;
    }
    if (((XfwfGroupWidget)old)->xfwfGroup.selection != ((XfwfGroupWidget)self)->xfwfGroup.selection
	|| ((XfwfGroupWidget)old)->xfwfGroup.selectionStyle != ((XfwfGroupWidget)self)->xfwfGroup.selectionStyle) {
	if (((XfwfGroupWidget)self)->xfwfGroup.selectionStyle == XfwfOneSelection && ((XfwfGroupWidget)self)->xfwfGroup.selection == -1L)
	    ((XfwfGroupWidget)self)->xfwfGroup.selection = 0;
	set_toggles(self);
    }
    return need_redraw;
}
/* #line 203 "Group.w" */
/*ARGSUSED*/static void expose(self,event,region)Widget self;XEvent * event;Region  region;
{
    Dimension w, h;
    Position x, y;

    if (! XtIsRealized(self)) return;
    xfwfRowColClassRec.core_class.expose(self, event, region);
    ((XfwfGroupWidgetClass)self->core.widget_class)->xfwfCommon_class.compute_inside(self, &x, &y, &w, &h);
    if (((XfwfGroupWidget)self)->xfwfGroup.label)
    XDrawImageString(XtDisplay(self), XtWindow(self), ((XfwfGroupWidget)self)->xfwfGroup.textgc, x + 3,
		     y - ((XfwfGroupWidget)self)->xfwfFrame.innerOffset, ((XfwfGroupWidget)self)->xfwfGroup.label, strlen(((XfwfGroupWidget)self)->xfwfGroup.label));
}
/* #line 230 "Group.w" */
/*ARGSUSED*/static void insert_child(child)Widget  child;
{ Widget self = XtParent(child); {
    xfwfRowColClassRec.composite_class.insert_child(child);
    if (((XfwfGroupWidget)self)->xfwfGroup.selectionStyle != XfwfNoSelection
	&& XtIsSubclass(child, xfwfToggleWidgetClass)) {
	XtAddCallback(child, XtNonCallback, on_cb, (XtPointer) ((XfwfGroupWidget)self)->xfwfGroup.toggle_ord);
	XtAddCallback(child, XtNoffCallback, off_cb, (XtPointer) ((XfwfGroupWidget)self)->xfwfGroup.toggle_ord);
	switch (((XfwfGroupWidget)self)->xfwfGroup.selectionStyle) {
	case XfwfOneSelection:
	case XfwfSingleSelection: {
	    Boolean state;

	    XtVaGetValues(child, XtNon, &state, NULL);

	    if (state) {
	      XtVaSetValues(self, XtNselection, ((XfwfGroupWidget)self)->xfwfGroup.toggle_ord, NULL);
	    }
	    XtVaSetValues(child, XtNon, ((XfwfGroupWidget)self)->xfwfGroup.toggle_ord == ((XfwfGroupWidget)self)->xfwfGroup.selection, NULL);
	  }
	    break;
	case XfwfMultipleSelection: {
	    Boolean state;

	    XtVaGetValues(child, XtNon, &state, NULL);
	    if (state)
	      ((XfwfGroupWidget)self)->xfwfGroup.selection |=
		(1L << ((XfwfGroupWidget)self)->xfwfGroup.toggle_ord);

	    XtVaSetValues(child, XtNon,
			  (((XfwfGroupWidget)self)->xfwfGroup.selection & (1L<<((XfwfGroupWidget)self)->xfwfGroup.toggle_ord)) != 0, NULL);
	  }
	    break;
	default: ;
	}
	((XfwfGroupWidget)self)->xfwfGroup.toggle_ord++;
    }
}
}
/* #line 127 "Group.w" */