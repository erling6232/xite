/* Generated by wbuild from "TextMenu.w"
** (generator version $Revision$ of $Date$)
*/
#include <stdlib.h>
#include <xite/includes.h>
#include <X11/IntrinsicP.h>
#include <X11/StringDefs.h>
#line 634 "TextMenu.w"
#include <X11/Xmu/CharSet.h>
#line 635 "TextMenu.w"
#include <Xfwf/stip4.h>
#line 636 "TextMenu.w"
#include XITE_STDIO_H
#line 637 "TextMenu.w"
#include <Xfwf/TabString.h>
#line 638 "TextMenu.w"
#include <Xfwf/Converters.h>
#line 639 "TextMenu.w"
#include <X11/Xmu/Converters.h>
#line 640 "TextMenu.w"
#include <ctype.h>
#include <Xfwf/TextMenuP.h>
#line 292 "TextMenu.w"
static void init(
Widget,XEvent*,String*,Cardinal*
);
#line 297 "TextMenu.w"
static void add_keyboard_grab(
Widget,XEvent*,String*,Cardinal*
);
#line 302 "TextMenu.w"
static void remove_keyboard_grab(
Widget,XEvent*,String*,Cardinal*
);
#line 313 "TextMenu.w"
static void highlight(
Widget,XEvent*,String*,Cardinal*
);
#line 339 "TextMenu.w"
static void notify(
Widget,XEvent*,String*,Cardinal*
);
#line 355 "TextMenu.w"
static void previous(
Widget,XEvent*,String*,Cardinal*
);
#line 374 "TextMenu.w"
static void next(
Widget,XEvent*,String*,Cardinal*
);
#line 399 "TextMenu.w"
static void shortcut(
Widget,XEvent*,String*,Cardinal*
);

static XtActionsRec actionsList[] = {
{"init", init},
{"add_keyboard_grab", add_keyboard_grab},
{"remove_keyboard_grab", remove_keyboard_grab},
{"highlight", highlight},
{"notify", notify},
{"previous", previous},
{"next", next},
{"shortcut", shortcut},
};

static char defaultTranslations[] = "\
<EnterWindow>: highlight() \n\
<LeaveWindow>: highlight() \n\
<BtnUp>: XtMenuPopdown() notify() \n\
<BtnMotion>: highlight() \n\
<Key>Return: XtMenuPopdown() notify() \n\
<Key>Escape: XtMenuPopdown() \n\
<Key>Up: previous() \n\
<Key>Down: next() \n\
<Key>: shortcut() \n\
<Map>: init() add_keyboard_grab() \n\
<Unmap>: remove_keyboard_grab() \n\
";
static void _resolve_inheritance(
WidgetClass
);
#line 163 "TextMenu.w"
static void class_initialize(
void
);
#line 177 "TextMenu.w"
static void initialize(
Widget ,Widget,ArgList ,Cardinal *
);
#line 198 "TextMenu.w"
static Boolean  set_values(
Widget ,Widget ,Widget,ArgList ,Cardinal *
);
#line 235 "TextMenu.w"
static void expose(
Widget,XEvent *,Region 
);
#line 256 "TextMenu.w"
static void destroy(
Widget
);
#line 433 "TextMenu.w"
#define is_active(n) (n >=8 *sizeof (((XfwfTextMenuWidget)self)->xfwfTextMenu.active )||(((XfwfTextMenuWidget)self)->xfwfTextMenu.active &(1 <<n )))


#line 439 "TextMenu.w"
#define myrealloc(p, size) ((XtPointer )XtRealloc ((char *)p ,size ))


#line 440 "TextMenu.w"
#define mymalloc(size) ((XtPointer )XtMalloc (size ))


#line 441 "TextMenu.w"
#define myfree(p) XtFree ((XtPointer )p )


#line 446 "TextMenu.w"
#define MARGIN 5 


#line 451 "TextMenu.w"
static void draw_label(
Widget,int ,GC 
);
#line 482 "TextMenu.w"
static void draw_normal(
Widget,int 
);
#line 491 "TextMenu.w"
static void draw_highlight(
Widget,int 
);
#line 500 "TextMenu.w"
static void draw_grayed(
Widget,int 
);
#line 520 "TextMenu.w"
#define LINEWD 2 


#line 522 "TextMenu.w"
static void parse_menu(
Widget
);
#line 572 "TextMenu.w"
static void make_gc(
Widget
);
#line 588 "TextMenu.w"
static void make_rvgc(
Widget
);
#line 605 "TextMenu.w"
static void make_graygc(
Widget
);
#line 624 "TextMenu.w"
static void add_cursor_cb(
Widget,XtPointer ,XtPointer 
);
#line 451 "TextMenu.w"
/*ARGSUSED*/static void draw_label(self,n,gc)Widget self;int  n;GC  gc;
{
    Cardinal i, j;
    Dimension w, w1;
    Position y;
    Display *dpy = XtDisplay(self);
    Window win = XtWindow(self);
    String s;

    y = ((XfwfTextMenuWidget)self)->xfwfTextMenu.offset[n] + ((XfwfTextMenuWidget)self)->xfwfTextMenu.font->ascent;
    i = 0;
    s = ((XfwfTextMenuWidget)self)->xfwfTextMenu.item[n];
    for (j = i; s[j] && s[j] != '\n' && s[j] != '_'; j++) ;
    XfwfDrawImageString(dpy, win, gc, MARGIN, y, s, j - i, ((XfwfTextMenuWidget)self)->xfwfTextMenu.tabs);
    if (s[j] == '_') {
	w = XfwfTextWidth(((XfwfTextMenuWidget)self)->xfwfTextMenu.font, s, j - i, ((XfwfTextMenuWidget)self)->xfwfTextMenu.tabs);
	i = j + 1;
	w1 = XTextWidth(((XfwfTextMenuWidget)self)->xfwfTextMenu.font, s + i, 1);
	for (j = i; s[j] && s[j] != '\n'; j++) ;
	XfwfDrawImageString(dpy, win, gc, MARGIN + w, y, s + i, j - i,
			    ((XfwfTextMenuWidget)self)->xfwfTextMenu.tabs);
	XDrawLine(dpy, win, gc, MARGIN + w, y + 2, MARGIN + w + w1 - 1,
		  y + 2);
    }
}
#line 482 "TextMenu.w"
/*ARGSUSED*/static void draw_normal(self,n)Widget self;int  n;
{
    XFillRectangle(XtDisplay(self), XtWindow(self), ((XfwfTextMenuWidget)self)->xfwfTextMenu.rvgc, MARGIN,
		   ((XfwfTextMenuWidget)self)->xfwfTextMenu.offset[n], ((XfwfTextMenuWidget)self)->core.width - 2 * MARGIN, ((XfwfTextMenuWidget)self)->xfwfTextMenu.itemheight);
    draw_label(self, n, ((XfwfTextMenuWidget)self)->xfwfTextMenu.gc);
}
#line 491 "TextMenu.w"
/*ARGSUSED*/static void draw_highlight(self,n)Widget self;int  n;
{
    XFillRectangle(XtDisplay(self), XtWindow(self), ((XfwfTextMenuWidget)self)->xfwfTextMenu.gc, MARGIN,
		   ((XfwfTextMenuWidget)self)->xfwfTextMenu.offset[n], ((XfwfTextMenuWidget)self)->core.width - 2 * MARGIN, ((XfwfTextMenuWidget)self)->xfwfTextMenu.itemheight);
    draw_label(self, n, ((XfwfTextMenuWidget)self)->xfwfTextMenu.rvgc);
}
#line 500 "TextMenu.w"
/*ARGSUSED*/static void draw_grayed(self,n)Widget self;int  n;
{
    draw_label(self, n, ((XfwfTextMenuWidget)self)->xfwfTextMenu.gc);
    XFillRectangle(XtDisplay(self), XtWindow(self), ((XfwfTextMenuWidget)self)->xfwfTextMenu.graygc, MARGIN,
		   ((XfwfTextMenuWidget)self)->xfwfTextMenu.offset[n], ((XfwfTextMenuWidget)self)->core.width - 2 * MARGIN, ((XfwfTextMenuWidget)self)->xfwfTextMenu.itemheight);
}
#line 522 "TextMenu.w"
/*ARGSUSED*/static void parse_menu(self)Widget self;
{
    Cardinal i, j, nlines = 0, p, q, n;
    Boolean is_line;
    char key, h[100];
    Position y = MARGIN;
    Dimension w;

    ((XfwfTextMenuWidget)self)->core.width = 1;
    n = 0;
    i = 0;
    while (((XfwfTextMenuWidget)self)->xfwfTextMenu.menu[i]) {
	is_line = True;
	key = '\0';
	for (; isspace(((XfwfTextMenuWidget)self)->xfwfTextMenu.menu[i]); i++) ;
	for (j = i; ((XfwfTextMenuWidget)self)->xfwfTextMenu.menu[j] != '\0' && ((XfwfTextMenuWidget)self)->xfwfTextMenu.menu[j] != '\n'; j++)
	    if (((XfwfTextMenuWidget)self)->xfwfTextMenu.menu[j] == '_')
		key = ((XfwfTextMenuWidget)self)->xfwfTextMenu.menu[j+1];
	    else if (((XfwfTextMenuWidget)self)->xfwfTextMenu.menu[j] != '-' && ((XfwfTextMenuWidget)self)->xfwfTextMenu.menu[j] != ' ')
		is_line = False;
	if (is_line) {
	    nlines++;
	    ((XfwfTextMenuWidget)self)->xfwfTextMenu.line = (Position *)myrealloc(((XfwfTextMenuWidget)self)->xfwfTextMenu.line, (nlines + 1) * sizeof(*((XfwfTextMenuWidget)self)->xfwfTextMenu.line));
	    ((XfwfTextMenuWidget)self)->xfwfTextMenu.line[nlines-1] = y + MARGIN;
	    y += LINEWD + 2 * MARGIN;
	    ((XfwfTextMenuWidget)self)->xfwfTextMenu.line[nlines] = 0;
	} else {
	    n++;
	    ((XfwfTextMenuWidget)self)->xfwfTextMenu.item = (String *)myrealloc(((XfwfTextMenuWidget)self)->xfwfTextMenu.item, n * sizeof(*((XfwfTextMenuWidget)self)->xfwfTextMenu.item));
	    ((XfwfTextMenuWidget)self)->xfwfTextMenu.item[n-1] = mymalloc((j - i + 1) * sizeof(*((XfwfTextMenuWidget)self)->xfwfTextMenu.item[n-1]));
	    ((XfwfTextMenuWidget)self)->xfwfTextMenu.item[n-1][0] = '\0'; strncat(((XfwfTextMenuWidget)self)->xfwfTextMenu.item[n-1], &((XfwfTextMenuWidget)self)->xfwfTextMenu.menu[i], j - i);
	    w = XfwfTextWidth(((XfwfTextMenuWidget)self)->xfwfTextMenu.font, &((XfwfTextMenuWidget)self)->xfwfTextMenu.menu[i], j - i, ((XfwfTextMenuWidget)self)->xfwfTextMenu.tabs);
	    /* The width includes an extra '_'; is that serious? */
	    if (w > ((XfwfTextMenuWidget)self)->core.width) ((XfwfTextMenuWidget)self)->core.width = w;
	    ((XfwfTextMenuWidget)self)->xfwfTextMenu.offset = (Position *)myrealloc(((XfwfTextMenuWidget)self)->xfwfTextMenu.offset, n * sizeof(*((XfwfTextMenuWidget)self)->xfwfTextMenu.offset));
	    ((XfwfTextMenuWidget)self)->xfwfTextMenu.offset[n-1] = y;
	    y += ((XfwfTextMenuWidget)self)->xfwfTextMenu.itemheight;
	    ((XfwfTextMenuWidget)self)->xfwfTextMenu.shortcut = myrealloc(((XfwfTextMenuWidget)self)->xfwfTextMenu.shortcut, n * sizeof(*((XfwfTextMenuWidget)self)->xfwfTextMenu.shortcut));
	    ((XfwfTextMenuWidget)self)->xfwfTextMenu.shortcut[n-1] = key;
	}
	i = ((XfwfTextMenuWidget)self)->xfwfTextMenu.menu[j] == '\0' ? j : j + 1;
    }
    ((XfwfTextMenuWidget)self)->xfwfTextMenu.nitems = n;
    ((XfwfTextMenuWidget)self)->core.height = y + MARGIN;
    ((XfwfTextMenuWidget)self)->core.width += 2 * MARGIN;
}
#line 572 "TextMenu.w"
/*ARGSUSED*/static void make_gc(self)Widget self;
{
    XtGCMask mask;
    XGCValues values;

    if (((XfwfTextMenuWidget)self)->xfwfTextMenu.gc != NULL) XtReleaseGC(self, ((XfwfTextMenuWidget)self)->xfwfTextMenu.gc);
    values.background = ((XfwfTextMenuWidget)self)->core.background_pixel;
    values.foreground = ((XfwfTextMenuWidget)self)->xfwfTextMenu.foreground;
    values.font = ((XfwfTextMenuWidget)self)->xfwfTextMenu.font->fid;
    values.line_width = LINEWD;
    mask = GCFont | GCBackground | GCForeground | GCLineWidth;
    ((XfwfTextMenuWidget)self)->xfwfTextMenu.gc = XtGetGC(self, mask, &values);
}
#line 588 "TextMenu.w"
/*ARGSUSED*/static void make_rvgc(self)Widget self;
{
    XtGCMask mask;
    XGCValues values;

    if (((XfwfTextMenuWidget)self)->xfwfTextMenu.rvgc != NULL) XtReleaseGC(self, ((XfwfTextMenuWidget)self)->xfwfTextMenu.rvgc);
    values.foreground = ((XfwfTextMenuWidget)self)->core.background_pixel;
    values.background = ((XfwfTextMenuWidget)self)->xfwfTextMenu.foreground;
    values.font = ((XfwfTextMenuWidget)self)->xfwfTextMenu.font->fid;
    mask = GCFont | GCBackground | GCForeground;
    ((XfwfTextMenuWidget)self)->xfwfTextMenu.rvgc = XtGetGC(self, mask, &values);
}
#line 605 "TextMenu.w"
/*ARGSUSED*/static void make_graygc(self)Widget self;
{
    XtGCMask mask;
    XGCValues values;

    if (((XfwfTextMenuWidget)self)->xfwfTextMenu.graygc != NULL) XtReleaseGC(self, ((XfwfTextMenuWidget)self)->xfwfTextMenu.graygc);
    values.foreground = ((XfwfTextMenuWidget)self)->core.background_pixel;
    values.stipple =
	XCreateBitmapFromData(XtDisplay(self),
			      RootWindowOfScreen(XtScreen(self)),
			      stip4_bits, stip4_width, stip4_height);
    values.fill_style = FillStippled;
    mask = GCForeground | GCStipple | GCFillStyle;
    ((XfwfTextMenuWidget)self)->xfwfTextMenu.graygc = XtGetGC(self, mask, &values);
}
#line 624 "TextMenu.w"
/*ARGSUSED*/static void add_cursor_cb(self,client_data,call_data)Widget self;XtPointer  client_data;XtPointer  call_data;
{
    XChangeActivePointerGrab(XtDisplay(self), ButtonPressMask |
			     ButtonReleaseMask | EnterWindowMask |
			     LeaveWindowMask | PointerMotionMask, ((XfwfTextMenuWidget)self)->xfwfTextMenu.cursor,
			     XtLastTimestampProcessed(XtDisplay(self)));
}

static XtResource resources[] = {
#line 21 "TextMenu.w"
{XtNfont,XtCFont,XtRFontStruct,sizeof(((XfwfTextMenuRec*)NULL)->xfwfTextMenu.font),XtOffsetOf(XfwfTextMenuRec,xfwfTextMenu.font),XtRString,(XtPointer)XtDefaultFont },
#line 25 "TextMenu.w"
{XtNforeground,XtCForeground,XtRPixel,sizeof(((XfwfTextMenuRec*)NULL)->xfwfTextMenu.foreground),XtOffsetOf(XfwfTextMenuRec,xfwfTextMenu.foreground),XtRString,(XtPointer)XtDefaultForeground },
#line 32 "TextMenu.w"
{XtNtablist,XtCTablist,XtRString,sizeof(((XfwfTextMenuRec*)NULL)->xfwfTextMenu.tablist),XtOffsetOf(XfwfTextMenuRec,xfwfTextMenu.tablist),XtRImmediate,(XtPointer)NULL },
#line 49 "TextMenu.w"
{XtNmenu,XtCMenu,XtRString,sizeof(((XfwfTextMenuRec*)NULL)->xfwfTextMenu.menu),XtOffsetOf(XfwfTextMenuRec,xfwfTextMenu.menu),XtRImmediate,(XtPointer)"empty"},
#line 56 "TextMenu.w"
{XtNactive,XtCActive,XtRLong,sizeof(((XfwfTextMenuRec*)NULL)->xfwfTextMenu.active),XtOffsetOf(XfwfTextMenuRec,xfwfTextMenu.active),XtRImmediate,(XtPointer)~(0x00) },
#line 70 "TextMenu.w"
{XtNselection,XtCSelection,XtRInt,sizeof(((XfwfTextMenuRec*)NULL)->xfwfTextMenu.selection),XtOffsetOf(XfwfTextMenuRec,xfwfTextMenu.selection),XtRImmediate,(XtPointer)-1 },
#line 76 "TextMenu.w"
{XtNcursor,XtCCursor,XtRCursor,sizeof(((XfwfTextMenuRec*)NULL)->xfwfTextMenu.cursor),XtOffsetOf(XfwfTextMenuRec,xfwfTextMenu.cursor),XtRString,(XtPointer)"arrow"},
#line 86 "TextMenu.w"
{XtNactivate,XtCActivate,XtRCallback,sizeof(((XfwfTextMenuRec*)NULL)->xfwfTextMenu.activate),XtOffsetOf(XfwfTextMenuRec,xfwfTextMenu.activate),XtRImmediate,(XtPointer)NULL },
#line 95 "TextMenu.w"
{XtNchangeSelection,XtCChangeSelection,XtRCallback,sizeof(((XfwfTextMenuRec*)NULL)->xfwfTextMenu.changeSelection),XtOffsetOf(XfwfTextMenuRec,xfwfTextMenu.changeSelection),XtRImmediate,(XtPointer)NULL },
};

XfwfTextMenuClassRec xfwfTextMenuClassRec = {
{ /* core_class part */
(WidgetClass) &overrideShellClassRec,
"XfwfTextMenu",
sizeof(XfwfTextMenuRec),
class_initialize,
_resolve_inheritance,
FALSE,
initialize,
NULL,
XtInheritRealize,
actionsList,
8,
resources,
9,
NULLQUARK,
False ,
FALSE ,
False ,
False ,
destroy,
XtInheritResize,
expose,
set_values,
NULL,
XtInheritSetValuesAlmost,
NULL,
XtInheritAcceptFocus,
XtVersion,
NULL,
defaultTranslations,
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
{ /* Shell_class part */
0
},
{ /* OverrideShell_class part */
0
},
{ /* XfwfTextMenu_class part */
0
},
};
WidgetClass xfwfTextMenuWidgetClass = (WidgetClass) &xfwfTextMenuClassRec;
/*ARGSUSED*/
#line 292 "TextMenu.w"
static void init(self,event,params,num_params)Widget self;XEvent*event;String*params;Cardinal*num_params;
{
    if (((XfwfTextMenuWidget)self)->xfwfTextMenu.selection < 0) XtVaSetValues(self, XtNselection, 0, NULL);
}

/*ARGSUSED*/
#line 297 "TextMenu.w"
static void add_keyboard_grab(self,event,params,num_params)Widget self;XEvent*event;String*params;Cardinal*num_params;
{
    XtGrabKeyboard(self, False, GrabModeAsync, GrabModeAsync, CurrentTime);
}

/*ARGSUSED*/
#line 302 "TextMenu.w"
static void remove_keyboard_grab(self,event,params,num_params)Widget self;XEvent*event;String*params;Cardinal*num_params;
{
    XtUngrabKeyboard(self, CurrentTime);
}

/*ARGSUSED*/
#line 313 "TextMenu.w"
static void highlight(self,event,params,num_params)Widget self;XEvent*event;String*params;Cardinal*num_params;
{
    int newsel, x = event->xbutton.x, y = event->xbutton.y;
    XfwfTextMenuData data;

    if (x < 0 || x >= ((XfwfTextMenuWidget)self)->core.width)			/* Pointer outside menu */
	newsel = -1;
    else					/* Pointer inside menu */
	for (newsel = ((XfwfTextMenuWidget)self)->xfwfTextMenu.nitems - 1; newsel >= 0; newsel--)
	    if (((XfwfTextMenuWidget)self)->xfwfTextMenu.offset[newsel] <= y && y < ((XfwfTextMenuWidget)self)->xfwfTextMenu.offset[newsel] + ((XfwfTextMenuWidget)self)->xfwfTextMenu.itemheight)
		break;
    if (newsel != ((XfwfTextMenuWidget)self)->xfwfTextMenu.selection) {
	if (((XfwfTextMenuWidget)self)->xfwfTextMenu.selection >= 0 && is_active(((XfwfTextMenuWidget)self)->xfwfTextMenu.selection))
	    draw_normal(self, ((XfwfTextMenuWidget)self)->xfwfTextMenu.selection);
	if (newsel >= 0 && is_active(newsel))
	    draw_highlight(self, newsel);
	((XfwfTextMenuWidget)self)->xfwfTextMenu.selection = newsel;
	data.n = ((XfwfTextMenuWidget)self)->xfwfTextMenu.selection;
	data.label = ((XfwfTextMenuWidget)self)->xfwfTextMenu.item[((XfwfTextMenuWidget)self)->xfwfTextMenu.selection];
	XtCallCallbackList(self, ((XfwfTextMenuWidget)self)->xfwfTextMenu.changeSelection, &data);
    }
}

/*ARGSUSED*/
#line 339 "TextMenu.w"
static void notify(self,event,params,num_params)Widget self;XEvent*event;String*params;Cardinal*num_params;
{
    XfwfTextMenuData data;

    if (((XfwfTextMenuWidget)self)->xfwfTextMenu.selection >= 0 && is_active(((XfwfTextMenuWidget)self)->xfwfTextMenu.selection)) {
	data.n = ((XfwfTextMenuWidget)self)->xfwfTextMenu.selection;
	data.label = ((XfwfTextMenuWidget)self)->xfwfTextMenu.item[((XfwfTextMenuWidget)self)->xfwfTextMenu.selection];
	XtCallCallbackList(self, ((XfwfTextMenuWidget)self)->xfwfTextMenu.activate, &data);
    }
}

/*ARGSUSED*/
#line 355 "TextMenu.w"
static void previous(self,event,params,num_params)Widget self;XEvent*event;String*params;Cardinal*num_params;
{
    int newsel;
    XfwfTextMenuData data;

    for (newsel = ((XfwfTextMenuWidget)self)->xfwfTextMenu.selection - 1; newsel >= 0; newsel--)
	if (is_active(newsel)) break;
    if (newsel >= 0) {
	if (is_active(((XfwfTextMenuWidget)self)->xfwfTextMenu.selection)) draw_normal(self, ((XfwfTextMenuWidget)self)->xfwfTextMenu.selection);
	draw_highlight(self, newsel);
	((XfwfTextMenuWidget)self)->xfwfTextMenu.selection = newsel;
	data.n = ((XfwfTextMenuWidget)self)->xfwfTextMenu.selection;
	data.label = ((XfwfTextMenuWidget)self)->xfwfTextMenu.item[((XfwfTextMenuWidget)self)->xfwfTextMenu.selection];
	XtCallCallbackList(self, ((XfwfTextMenuWidget)self)->xfwfTextMenu.changeSelection, &data);
    }
}

/*ARGSUSED*/
#line 374 "TextMenu.w"
static void next(self,event,params,num_params)Widget self;XEvent*event;String*params;Cardinal*num_params;
{
    int newsel;
    XfwfTextMenuData data;

    for (newsel = ((XfwfTextMenuWidget)self)->xfwfTextMenu.selection + 1; newsel < ((XfwfTextMenuWidget)self)->xfwfTextMenu.nitems; newsel++)
	if (is_active(newsel)) break;
    if (newsel < ((XfwfTextMenuWidget)self)->xfwfTextMenu.nitems) {
	if (((XfwfTextMenuWidget)self)->xfwfTextMenu.selection >= 0 && is_active(((XfwfTextMenuWidget)self)->xfwfTextMenu.selection))
	    draw_normal(self, ((XfwfTextMenuWidget)self)->xfwfTextMenu.selection);
	draw_highlight(self, newsel);
	((XfwfTextMenuWidget)self)->xfwfTextMenu.selection = newsel;
	data.n = ((XfwfTextMenuWidget)self)->xfwfTextMenu.selection;
	data.label = ((XfwfTextMenuWidget)self)->xfwfTextMenu.item[((XfwfTextMenuWidget)self)->xfwfTextMenu.selection];
	XtCallCallbackList(self, ((XfwfTextMenuWidget)self)->xfwfTextMenu.changeSelection, &data);
    }
}

/*ARGSUSED*/
#line 399 "TextMenu.w"
static void shortcut(self,event,params,num_params)Widget self;XEvent*event;String*params;Cardinal*num_params;
{
    int newsel, n;
    XfwfTextMenuData data;
    char buf[3];

    if (*num_params == 1) {
	newsel = atol(params[0]);
	if (newsel < 0 || newsel >= ((XfwfTextMenuWidget)self)->xfwfTextMenu.nitems) {
	    XtWarning("Incorrect arguments for shortcut action");
	    return;
	}
	if (newsel == ((XfwfTextMenuWidget)self)->xfwfTextMenu.selection || ! is_active(newsel)) return;
    } else if (*num_params == 0) {
	n = XLookupString(&event->xkey, buf, sizeof(buf), NULL, NULL);
	if (n != 1) return;
	for (newsel = 0; newsel < ((XfwfTextMenuWidget)self)->xfwfTextMenu.nitems; newsel++)
	    if (toupper(((XfwfTextMenuWidget)self)->xfwfTextMenu.shortcut[newsel]) == toupper(buf[0])) break;
	if (newsel == ((XfwfTextMenuWidget)self)->xfwfTextMenu.nitems || ! is_active(newsel)) return;
    } else {
	XtWarning("Incorrect arguments for shortcut action");
	return;
    }
    ((XfwfTextMenuWidget)self)->xfwfTextMenu.selection = newsel;
    XtPopdown(self);
    data.n = ((XfwfTextMenuWidget)self)->xfwfTextMenu.selection;
    data.label = ((XfwfTextMenuWidget)self)->xfwfTextMenu.item[((XfwfTextMenuWidget)self)->xfwfTextMenu.selection];
    XtCallCallbackList(self, ((XfwfTextMenuWidget)self)->xfwfTextMenu.activate, &data);
}

static void _resolve_inheritance(class)
WidgetClass class;
{
  XfwfTextMenuWidgetClass c = (XfwfTextMenuWidgetClass) class;
  XfwfTextMenuWidgetClass super;
  static CompositeClassExtensionRec extension_rec = {
    NULL, NULLQUARK, XtCompositeExtensionVersion,
    sizeof(CompositeClassExtensionRec), True};
  CompositeClassExtensionRec *ext;
  ext = (CompositeClassExtensionRec *)XtMalloc(sizeof(*ext));
  *ext = extension_rec;
  ext->next_extension = c->composite_class.extension;
  c->composite_class.extension = (XtPointer)ext;
  if (class == xfwfTextMenuWidgetClass) return;
  super = (XfwfTextMenuWidgetClass)class->core_class.superclass;
}
#line 163 "TextMenu.w"
/*ARGSUSED*/static void class_initialize()
{
    XtAddConverter(XtRString, XtRLong, XmuCvtStringToLong, NULL, 0);
    XtSetTypeConverter(XtRLong, XtRString, XfwfCvtLongToString,
		       NULL, 0, XtCacheNone, NULL);
}
#line 177 "TextMenu.w"
/*ARGSUSED*/static void initialize(request,self,args,num_args)Widget  request;Widget self;ArgList  args;Cardinal * num_args;
{
    if (((XfwfTextMenuWidget)self)->xfwfTextMenu.menu) ((XfwfTextMenuWidget)self)->xfwfTextMenu.menu = XtNewString(((XfwfTextMenuWidget)self)->xfwfTextMenu.menu);
    ((XfwfTextMenuWidget)self)->xfwfTextMenu.itemheight = ((XfwfTextMenuWidget)self)->xfwfTextMenu.font->ascent + ((XfwfTextMenuWidget)self)->xfwfTextMenu.font->descent;
    ((XfwfTextMenuWidget)self)->xfwfTextMenu.tablist = XtNewString(((XfwfTextMenuWidget)self)->xfwfTextMenu.tablist);
    ((XfwfTextMenuWidget)self)->xfwfTextMenu.tabs = XfwfTablist2Tabs(((XfwfTextMenuWidget)self)->xfwfTextMenu.tablist);
    ((XfwfTextMenuWidget)self)->xfwfTextMenu.offset = NULL;
    ((XfwfTextMenuWidget)self)->xfwfTextMenu.item = NULL;
    ((XfwfTextMenuWidget)self)->xfwfTextMenu.line = NULL;
    ((XfwfTextMenuWidget)self)->xfwfTextMenu.shortcut = NULL;
    parse_menu(self);
    ((XfwfTextMenuWidget)self)->xfwfTextMenu.gc = NULL; make_gc(self);
    ((XfwfTextMenuWidget)self)->xfwfTextMenu.rvgc = NULL; make_rvgc(self);
    ((XfwfTextMenuWidget)self)->xfwfTextMenu.graygc = NULL; make_graygc(self);
    XtAddCallback(self, XtNpopupCallback, add_cursor_cb, NULL);
}
#line 198 "TextMenu.w"
/*ARGSUSED*/static Boolean  set_values(old,request,self,args,num_args)Widget  old;Widget  request;Widget self;ArgList  args;Cardinal * num_args;
{
    Boolean need_redraw = False;

    if (((XfwfTextMenuWidget)old)->xfwfTextMenu.font != ((XfwfTextMenuWidget)self)->xfwfTextMenu.font) {
	((XfwfTextMenuWidget)self)->xfwfTextMenu.itemheight = ((XfwfTextMenuWidget)self)->xfwfTextMenu.font->ascent + ((XfwfTextMenuWidget)self)->xfwfTextMenu.font->descent;
	need_redraw = True;
    }
    if (((XfwfTextMenuWidget)old)->xfwfTextMenu.foreground != ((XfwfTextMenuWidget)self)->xfwfTextMenu.foreground) {
	need_redraw = True;
    }
    if (((XfwfTextMenuWidget)old)->xfwfTextMenu.tablist != ((XfwfTextMenuWidget)self)->xfwfTextMenu.tablist) {
	myfree(((XfwfTextMenuWidget)old)->xfwfTextMenu.tablist);
	((XfwfTextMenuWidget)self)->xfwfTextMenu.tablist = XtNewString(((XfwfTextMenuWidget)self)->xfwfTextMenu.tablist);
	myfree(((XfwfTextMenuWidget)old)->xfwfTextMenu.tabs);
	((XfwfTextMenuWidget)self)->xfwfTextMenu.tabs = XfwfTablist2Tabs(((XfwfTextMenuWidget)self)->xfwfTextMenu.tablist);
	need_redraw = True;
    }
    if (((XfwfTextMenuWidget)old)->xfwfTextMenu.menu != ((XfwfTextMenuWidget)self)->xfwfTextMenu.menu) {
	myfree(((XfwfTextMenuWidget)old)->xfwfTextMenu.menu);
	((XfwfTextMenuWidget)self)->xfwfTextMenu.menu = XtNewString(((XfwfTextMenuWidget)self)->xfwfTextMenu.menu);
	parse_menu(self);
	need_redraw = True;
    }
    if (((XfwfTextMenuWidget)old)->xfwfTextMenu.active != ((XfwfTextMenuWidget)self)->xfwfTextMenu.active) {
	need_redraw = True;
    }
    if (((XfwfTextMenuWidget)old)->xfwfTextMenu.selection != ((XfwfTextMenuWidget)self)->xfwfTextMenu.selection) {
	need_redraw = True;
    }
    return need_redraw;
}
#line 235 "TextMenu.w"
/*ARGSUSED*/static void expose(self,event,region)Widget self;XEvent * event;Region  region;
{
    Cardinal i;

    if (! XtIsRealized(self)) return;
    for (i = 0; i < ((XfwfTextMenuWidget)self)->xfwfTextMenu.nitems; i++)
	if (! is_active(i)) draw_grayed(self, i);
	else if (i == ((XfwfTextMenuWidget)self)->xfwfTextMenu.selection) draw_highlight(self, i);
	else draw_label(self, i, ((XfwfTextMenuWidget)self)->xfwfTextMenu.gc);
    if (((XfwfTextMenuWidget)self)->xfwfTextMenu.line)
	for (i = 0; ((XfwfTextMenuWidget)self)->xfwfTextMenu.line[i]; i++)
	    XDrawLine(XtDisplay(self), XtWindow(self), ((XfwfTextMenuWidget)self)->xfwfTextMenu.gc, 0, ((XfwfTextMenuWidget)self)->xfwfTextMenu.line[i],
		      ((XfwfTextMenuWidget)self)->core.width, ((XfwfTextMenuWidget)self)->xfwfTextMenu.line[i]);
}
#line 256 "TextMenu.w"
/*ARGSUSED*/static void destroy(self)Widget self;
{
    int i;

    for (i = 0; i < ((XfwfTextMenuWidget)self)->xfwfTextMenu.nitems; i++) myfree(((XfwfTextMenuWidget)self)->xfwfTextMenu.item[i]);
    myfree(((XfwfTextMenuWidget)self)->xfwfTextMenu.tablist); myfree(((XfwfTextMenuWidget)self)->xfwfTextMenu.tabs); myfree(((XfwfTextMenuWidget)self)->xfwfTextMenu.menu); myfree(((XfwfTextMenuWidget)self)->xfwfTextMenu.shortcut);
    myfree(((XfwfTextMenuWidget)self)->xfwfTextMenu.offset); myfree(((XfwfTextMenuWidget)self)->xfwfTextMenu.item); myfree(((XfwfTextMenuWidget)self)->xfwfTextMenu.line);
    XtReleaseGC(self, ((XfwfTextMenuWidget)self)->xfwfTextMenu.gc); XtReleaseGC(self, ((XfwfTextMenuWidget)self)->xfwfTextMenu.rvgc); XtReleaseGC(self, ((XfwfTextMenuWidget)self)->xfwfTextMenu.graygc);
}
#line 104 "TextMenu.w"
