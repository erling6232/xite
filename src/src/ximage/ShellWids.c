

/*

________________________________________________________________

        ShellWids.c
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

static char *Id = "$Id$";



#include <xite/includes.h>
#include <stdlib.h>
#include XITE_STDIO_H
#include <X11/Intrinsic.h>
#include <xite/Visual.h>
#include <xite/Cmap.h>
#include <xite/ShellWids.h>
#include <ctype.h>
#include <X11/Xos.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Shell.h>
#include <xite/Image.h>
#include <xite/ximage_display.h>
#include <xite/XmenuBar.h>
#include <xite/Xcolor.h>
#include <xite/mallocp.h>
#include <xite/debug.h>

#ifdef MOTIF
#include <Xm/Xm.h> 
#else
#include <X11/StringDefs.h>
#include <X11/Xaw/AsciiText.h>
#include <X11/Xaw/SimpleMenu.h>
#include <X11/Xaw/Sme.h>
#include <X11/Xaw/SmeBSB.h>
#include <X11/Xaw/SmeLine.h>
#endif



extern Boolean share_e;
extern Widget Control_e;
extern Widget colorsmenu_e;
/* Static variables */

static Visual *imageReqVisual_s;
static Widget menubar_s = NULL;
static Widget GV_s;

#ifdef MOTIF

static Arg ShellArgs_s[] =
{
  { XmNvisual, (XtArgVal) 0 },
  { XmNcolormap, (XtArgVal) 0 },
  { XmNdepth, (XtArgVal) 0 },
  { XmNallowShellResize, (XtArgVal) True}, /* Allow appl. to resize itself */
  { XmNinput, (XtArgVal) True},            /* Windowmgr handles keyb. focus */
  { XmNlabelString, (XtArgVal) 0 },
  { XmNforeground, (XtArgVal) 0 },
  { XmNbackground, (XtArgVal) 0 },
};

#else

static Arg ShellArgs_s[] =
{
  { XtNvisual, (XtArgVal) 0 },             /* ShellPart resource. */
  { XtNcolormap, (XtArgVal) 0 },           /* CorePart resource. */
  { XtNdepth, (XtArgVal) 0 },              /* CorePart resource. */
  { XtNallowShellResize, (XtArgVal) True}, /* ShellPart resource.
					      Allow appl. to resize itself */
  { XtNinput, (XtArgVal) True},            /* Windowmgr handles keyb. focus */
  { XtNlabel, (XtArgVal) 0 },
  { XtNforeground, (XtArgVal) 0 },
  { XtNbackground, (XtArgVal) 0 },
};

#endif



/*F:RequestedImageVisualAndColormap=ShellWids*/
/*F:CreateAppShell=ShellWids*/
/*F:CreateTopLevelShell=ShellWids*/
/*F:CreatePopTopShell=ShellWids*/
/*F:CreatePopupShell=ShellWids*/
/*F:CreatePopOver=ShellWids*/
/*F:AddAction=ShellWids*/
/*F:TopLevelWidget=ShellWids*/
/*F:PopupExit=ShellWids*/
/*F:PopupInit=ShellWids*/
/*F:PopupAdd=ShellWids*/
/*F:Popup_add=ShellWids*/
/*F:PopupLine=ShellWids*/
/*F:PopupWidget=ShellWids*/
/*F:ImageFromMenu=ShellWids*/
/*F:PopupMenu=ShellWids*/
/*F:InitializeVisual=ShellWids*/
/*F:ShellWids*

________________________________________________________________

		ShellWids
________________________________________________________________

Name:		InitializeVisual, RequestedImageVisualAndColormap,
                CreateAppShell, CreateTopLevelShell, CreatePopTopShell, 
		CreatePopupShell, CreatePopOver, AddAction, TopLevelWidget, 
		PopupInit, PopupAdd, Popup_add, PopupLine, PopupMenu,
		PopupExit, PopupWidget, ImageFromMenu - XITE shell widgets.

Syntax:         | #include <xite/ShellWids.h>
                | 
                | void InitializeVisual( Display* dpy,
                |    int screen, char* imageVisualClass );
                |
                | void RequestedImageVisualAndColormap(
                |    Display* dpy, int screen, int* planes,
                |    Visual** visual, Colormap* colormap );
                |
                | Widget CreatePopTopShell( char* name,
                |    Widget parent );
                |
                | Widget CreatePopupShell( char* name,
                |    Widget parent );
                |
                | Widget CreateTopLevelShell( Display* dpy,
                |    char* name );
                |
		| Widget CreatePopOverShell( char *name,
		|    Widget parent );
		|
                | Widget CreateAppShell( Display* dpy,
                |    char* app_name, char* app_class );
                |
                | Widget PopupInit( char* name, Widget parent,
                |    char* label );
                |
                | Widget PopupAdd( char* name, Widget parent,
                |    char* label, XtCallbackProc callback_func,
                |    int arg );
                |
		| Widget Popup_add( char *name, Widget parent,
		|    char *label, XtCallbackProc callback_func,
		|    XtPointer arg );
		|
                | Widget PopupLine( char* name, Widget parent );
                |
                | void PopupMenu( Widget wid, XEvent* event,
                |    String* params, Cardinal* num_params );
                |
                | void PopupExit( Widget wid, XEvent* event,
                |    String* params, Cardinal* num_params );
                |
                | Widget PopupWidget( void  );
                |
		| Widget ImageFromMenu( Widget wid );
		|
                | Widget TopLevelWidget( Widget wid );
                |
                | void AddAction( Widget parent, char* name,
                |    XtActionProc func, int popup );
                |
Overview:       The functions in this module all deal with shell widgets.
                They make sure that the necessary information is present
		prior to creating a shell widget. Some of them deal
		specifically with popup menus.

Description:    'InitializeVisual' initializes XITE X11 applications in terms
		of information about the visuals which 'screen' on 'dpy' is
		capable of supporting and the XITE-initial colormaps for
		these visuals. 'imageVisualClass' is the
		requested/preferred visual for the application. Refer to
		'InitialVisualInfos(3)' for information about visuals and
		the possible values of the 'imageVisualClass' argument.
		Refer to 'GetImageColormap(3)' for information about the
		colormaps.

		'RequestedImageVisualAndColormap' works with the registered
		globally preferred visual (and its depth and colormaps). It
		returns the visual via 'visual', unless 'visual' is NULL or
		'*visual' is non-NULL. In these two cases, don''t change
		'*visual'.

		The depth is returned via 'planes', unless 'planes' is NULL or
		'*planes' != 0. In the latter case, don''t change '*planes'. In
		this case, depth will be 7 for 8-plane PseudoColor and 21 for
		24-plane PseudoColor (reduced-color display).

		The colormap is returned via 'colormap'.

		If no globally preferred visual is registered, then
		call 'InitializeVisual' first.

		'RequestedImageVisualAndColormap' calls 'RequestedImageVisual'
		and 'GetImageColormap' to get its work done.

		The shell creation functions make sure that all the
                information about preferred/requested visuals (class and
		depth) and colormaps has been collected for image-related
		widgets as well as for textual widgets. If necessary, this
		is done by a call to 'InitializeVisual'.

		The shell creation functions create and return a widget of
		class applicationShellWidgetClass ('CreateAppShell'),
		topLevelShellWidgetClass ('CreateTopLevelShell' and
		'CreatePopTopShell'), transientShellWidgetClass
		('CreatePopupShell') or overrideShellWidgetClass
		('CreatePopOverShell'). They get visual class, depth and
		colormap from the default visual, depth and colormap.

		'CreateAppShell' uses 'app_class' and 'app_name' for class and
		instance names respectively. The other shell widget creation
		functions use 'name' as instance name. They all set the
		resource XtNallowShellResize (allow application to resize
		itself). 'CreatePopOverShell', 'CreatePopTopShell' and
		'CreatePopupShell' also set the resource XtNinput (window
		manager handles keyboard focus).

		'CreatePopOverShell' is typically used for non-permanent
		menus.

		'PopupInit' initializes a popup menu (class
		simpleMenuWidgetClass) and returns a reference to it. The
		instance name is given by 'name' (used in resource
		specifications). The visible label is given by 'label' if
		non-NULL, otherwise by 'name'. This string appears at the top
		of the menu.

		'PopupAdd' adds a menu entry (class smeBSBObjectClass) to the
		menu widget given by 'parent'. The instance name of this new
		menu entry is 'name', the label is 'label', and
		'callback_func' is added as a callback function if it is not
		NULL (to the XtNcallback resource). Space is allocated for a
		copy of the parameter 'arg'. This (or actually a reference to
		the copy) arrives in the callback routine as client_data.

		'Popup_add' is very similar to 'PopupAdd', but a reference to
		any chunk of information can be passed along in 'arg'.
		This reference arrives as the client_data argument in
		'callback_func'. It is the responsibility of 'callback_func'
		to interpret the reference correctly.

		'PopupLine' adds a menu line separator (class
		smeLineObjectClass) to the menu widget given by 'parent'.

		'PopupMenu' is an action function which actually pops up a
		menu. It calls XawPositionSimpleMenu and then the X Toolkit
		Intrinsics action function 'MenuPopup'. 'PopupMenu' is
		typically registered as an action function of an application
		and called with the help of a translation table. This is
		the case for all applications which use the function
		'ximageInit' from the 'ximage(3)' toolkit.

		'PopupExit' is also registered as an action function of an
		application and called with the help of a translation table.
		It does NOT call the X Toolkit Intrinsics function
		'MenuPopdown' (which should be invoked prior to 'PopupExit'
		by the translation table). Currently 'PopupExit' is empty.

		'PopupWidget' returns an Image, ImageOverlay or AsciiText
		widget associated with a popup menu. The menu may have been
		popped up in this widget or in an XfwfPullDown button in a
		menubar above an Image or ImageOverlay widget. In this case,
		the Image or ImageOverlay widget is returned.

		'ImageFromMenu' returns Image or ImageOverlay widget
                associated with 'wid'. 'wid' is either an Image or
                ImageOverlay widget in which a menu is popped up or an
                XfwfPullDown button child of an XfwfMenuBar sibling of an
		Image or ImageOverlay widget.

                'TopLevelWidget' returns a reference to the Shell widget
		ancestor of 'wid'.

                'AddAction' adds an action function to the application to
		which 'wid' belongs. 'name' is the name of the aaction
		function and can be used in translation tables. 'func' is
		the action function. 'popup' should be non-zero if the
		'func' involves popping up a menu.

Return value:	Reference to Image, ImageOverlay or AsciiText widget.

See also:       'Visual(3)', 'Cmap(3)', 'Image(3)', 'ImageOverlay(3)',
                'ximage(3)', 'visualFromName(3)'

Author:		Otto Milvang and Svein Bøe
Doc:            Svein Bøe
________________________________________________________________

*/



#ifndef FUNCPROTO
static void SetShellArgs(dpy, screen)
Display *dpy;
int screen;
#else /* FUNCPROTO */
static void SetShellArgs(Display *dpy, int screen)
#endif /* FUNCPROTO */
{
  Visual *visual = NULL;
  Colormap cmap = (Colormap) NULL;
  Widget image_wid;
  IMAGE img;
  Boolean suggest_shared = False;
  int coltab;

  /* Set arguments for creation of shell widgets. Textual (non-image) widget
   * descendents of shell widgets will use the same visual and colormap as the
   * shell ancestor. Image and ImageOverlay descendents can use a different
   * visual and colormap.
   *
   * On displays with multiple hardware colormaps (typically workstations
   * from SGI with PseudoColor visual requested for the image widgets), use
   * the default display colormap when creating shell widgets. Moving the 
   * mouse pointer between non-image and image widgets will generally not
   * lead to much colormap flashing because of the multiple hardware colormaps.
   *
   * On displays with only a single hardware colormap, moving the mouse
   * pointer between non-image and image widgets may lead to colormap flashing.
   * If the requested/preferred reduced-color mode image "Work-map" colormap
   * was inserted into the free colorcells of the default display colormap,
   * there will not be colormap flashing for the "Work-map" in reduced-color
   * mode. If, however, the default display colormap is not the
   * same as the requested/preferred initial image colormap, there will be
   * flashing when the mouse pointer is moved. In this case, an alternative is
   * to use the initial image colormap also for the textual widgets (and thus
   * for the shell widgets). The colors of the text widgets may not correspond
   * to the colors chosen with command-line options or resources, but at least
   * the colormap flashing will cease.
   *
   * When using an image colormap for the shell widgets, first get the
   * "Work-map" image colormap, then create a dummy image and get the initial
   * image colormap (different from "Work-map" if option '-ct' is used).
   */

  if (share_e ||
      MaxCmapsOfScreen(ScreenOfDisplay(dpy, screen)) == 1 ||
      ClassOfVisual(imageReqVisual_s) != PseudoColor) {
    /* Either explicitly requested use of image-preferred colormap for
     * non-image widgets or definitely single hardware colormap or
     * composite-colormap visual class preferred for image widgets.
     * In the latter case (which for XITE means a visual depth of 24),
     * there is most certainly also only a single hardware colormap
     * available.
     */

    if (share_e) {
     /* Try to minimize colormap flashing by using the image-preferred
      * colormap for non-image widgets. Must then also use the image-preferred
      * visual class on non-image widgets.
      */

      if (DefaultVisual(dpy, screen) == imageReqVisual_s) {
	/* Image-preferred visual class is the same as the default visual
	 * class. */
	visual = imageReqVisual_s;
	/* Get the "Work-map". */
	GetImageColormap(dpy, screen,
			 DepthToReducedPlanes(DepthOfVisual(visual), visual),
			 visual, &cmap);
      } else {
	/* Not able to make non-image widgets with visual class different
	 * from the default visual class. This requires modifying the Xfwf
	 * widgets to create GC's with XCreateGC, not XtGetGC, and possibly
	 * other problems (probably the Xaw widgets can't cope either).
	 */
	fprintf(stderr, "\n%s\n",
		"SetShellArgs: Ignoring opion \"-share\".");
	share_e = 0;
      }
    } else {
      if (DefaultVisual(dpy, screen) == imageReqVisual_s) {
	/* It would probably be wise to use the image-preferred colormap
         * for non-image widgets. However, if the default colormap is the
         * same as the image-preferred initial colormap, then it is not
	 * necessary, because the colormaps won't flash anyway. Check this
	 * further down.
	 */
	suggest_shared = True;
      }
    }
  }

  if (!visual) visual = DefaultVisual(dpy, screen);
  XtSetArg(ShellArgs_s[0], XtNvisual, visual);

  if (!cmap) cmap = DefaultColormap(dpy, screen);
  XtSetArg(ShellArgs_s[1], XtNcolormap, cmap);
  cmap = (Colormap) NULL;

  XtSetArg(ShellArgs_s[2], XtNdepth, DepthOfVisual(visual));

  if (ClassOfVisual(visual) == PseudoColor) {
    XtSetArg(ShellArgs_s[6], XtNforeground, 200);
    XtSetArg(ShellArgs_s[7], XtNbackground, 207);
  } else if (ClassOfVisual(visual) == DirectColor) {
    XtSetArg(ShellArgs_s[6], XtNforeground, 0);
    XtSetArg(ShellArgs_s[7], XtNbackground, 0x0ffffff);
  } else {
    XtSetArg(ShellArgs_s[6], XtNforeground, WhitePixel(dpy, screen));
    XtSetArg(ShellArgs_s[7], XtNbackground, BlackPixel(dpy, screen));
  }

  if (share_e || suggest_shared) {
    /* Check if the initial image colortable is not the "Work-map".
     * Create a dummy image widget, get the initial colormap. */
  
    img       = Imake_image(1, "dummy", Iu_byte_typ, 1, 1);
    image_wid = ximage_display(Control_e, "image", "dummy_shell", img, 1,
			       imageWidgetClass, NULL, 0,
			       COLORS_BUTTON, NULL, 0, NULL);
    XtVaGetValues(image_wid, XtNcolortab, &coltab, NULL);

    if (coltab != 1) cmap = Color_map2(coltab, 128, visual);
    else {
      /* Initial image colortab is the "Work-map". The requested/preferred
       * image colormap, with contents equal to "Work-map" may have been
       * installed into the default display colormap.
       */

      GetImageColormap(dpy, screen,
		       DepthToReducedPlanes(DepthOfVisual(visual), visual),
		       visual, &cmap);
      /* The contents of cmap are now probably the same as those of XtNcolormap
       * for the image, but if the image is displayed without "reduced-color"
       * mode, the contents will be different. It is the "reduced-color" mode
       * one we are interested in, because only in this mode is there a hope of
       * avoiding colormap flashing on displays with only a single hardware
       * colormap.
       *
       * Note that cmap will not be the same as XtNcolormap, even if the
       * contents are identical. cmap is the preferred/requested map,
       * XtNcolormap is a copy of it.
       *
       * Don't unset suggest_shared. Option "-ct" must have been used.
       */

      if (suggest_shared && cmap == DefaultColormap(dpy, screen))
	/* The requested/preferred image colormap (with contents equal to
	 * "Work-map") has been installed into the default display colormap. */
	suggest_shared = False;
    }

    if (suggest_shared)
      fprintf(stderr, "\n%s\n%s\n%s\n%s\n",
	      "SetShellArgs: Your display has only one hardware colormap.",
	      "    You may want to use the option \"-share\" to reduce",
	      "    colormap flashing for the chosen initial image colormap",
	      "    in reduced color mode.");

    if (share_e && coltab != 1 && cmap)
      XtSetArg(ShellArgs_s[1], XtNcolormap, cmap);

    /* Clean up. */
  
    XtDestroyWidget(colorsmenu_e); colorsmenu_e = NULL;
    XtDestroyWidget(image_wid);
  }
  
} /* SetShellArgs() */



#ifndef FUNCPROTO
static void ReadDefaults(dpy, screen, imageVisualClass)
Display *dpy;
int screen;
char *imageVisualClass;
#else /* FUNCPROTO */
static void ReadDefaults(Display *dpy, int screen, char *imageVisualClass)
#endif /* FUNCPROTO */
{
  XVisualInfo *visualInfo;

  ENTER_FUNCTION_DEBUG("ShellWids.c: ReadDefaults");

  InitializeVisualInfos(dpy, screen, imageVisualClass, &visualInfo);
  imageReqVisual_s = VisualOfVisualInfo(*visualInfo);

  /* Initialize colormaps. */
  GetImageColormap(dpy, screen, 0, NULL, NULL);

  /* Set arguments for creation of shell widgets. */
  SetShellArgs(dpy, screen);
}

#ifndef FUNCPROTO
void InitializeVisual(dpy, screen, imageVisualClass)
Display *dpy;
int screen;
char *imageVisualClass;
#else /* FUNCPROTO */
void InitializeVisual(Display *dpy, int screen, char *imageVisualClass)

#endif /* FUNCPROTO */
{
  ReadDefaults(dpy, screen, imageVisualClass);
}  

#ifndef FUNCPROTO
void RequestedImageVisualAndColormap(dpy, screen, planes, visual, colormap)
Display *dpy;
int screen;
int *planes;
Visual **visual;
Colormap *colormap;
#else /* FUNCPROTO */
void RequestedImageVisualAndColormap(Display *dpy, int screen, int *planes, Visual **visual, Colormap *colormap)

#endif /* FUNCPROTO */
{

  ENTER_FUNCTION_DEBUG("ShellWids.c: RequestedImageVisualAndColormap");

  RequestedImageVisual(dpy, screen, planes, visual);
  if (planes != 0)
    GetImageColormap(dpy, screen, *planes, *visual, colormap);
  else
    GetImageColormap(dpy, screen, 0, *visual, colormap);

  LEAVE_FUNCTION_DEBUG("ShellWids.c: RequestedImageVisualAndColormap");
}



#ifndef FUNCPROTO
Widget CreateAppShell(dpy, app_name, app_class)
Display *dpy;
char *app_name, *app_class;
#else /* FUNCPROTO */
Widget CreateAppShell(Display *dpy, char *app_name, char *app_class)
#endif /* FUNCPROTO */
{
  ENTER_FUNCTION_DEBUG("ShellWids.c: CreateAppShell");

  if (imageReqVisual_s == NULL)
    ReadDefaults(dpy, DefaultScreen(dpy), NULL);

  LEAVE_FUNCTION_DEBUG("ShellWids.c: CreateAppShell");
  /* Override widget class (when widget is subclass of ApplicationWidget). */
  return(XtAppCreateShell(app_name, app_class, applicationShellWidgetClass,
			      dpy, ShellArgs_s, (Cardinal) 4));
}

#ifndef FUNCPROTO
Widget CreateTopLevelShell(dpy, name)
Display *dpy;
char *name;
#else /* FUNCPROTO */
Widget CreateTopLevelShell(Display *dpy, char *name)
#endif /* FUNCPROTO */
{
  ENTER_FUNCTION_DEBUG("ShellWids.c: CreateTopLevelShell");

  if (imageReqVisual_s == NULL)
    ReadDefaults(dpy, DefaultScreen(dpy), NULL);

  LEAVE_FUNCTION_DEBUG("ShellWids.c: CreateTopLevelShell");
  /* Can not override widget class when widget is not subclass of
   * ApplicationWidget
   */
  return(XtAppCreateShell(name, NULL, topLevelShellWidgetClass,
			      dpy, ShellArgs_s, (Cardinal) 4));
}

#ifndef FUNCPROTO
Widget CreatePopupShell(name, parent)
char *name;
Widget parent;
#else /* FUNCPROTO */
Widget CreatePopupShell(char *name, Widget parent)
#endif /* FUNCPROTO */
{
  Display *dpy;

  ENTER_FUNCTION_DEBUG("ShellWids.c: CreatePopupShell");

  dpy = XtDisplay(parent);
  if (imageReqVisual_s == NULL)
    ReadDefaults(dpy, DefaultScreen(dpy), NULL);

  LEAVE_FUNCTION_DEBUG("ShellWids.c: CreatePopupShell");
  return(XtCreatePopupShell(name, transientShellWidgetClass,
			     parent, ShellArgs_s, (Cardinal) 5));
}

#ifndef FUNCPROTO
Widget CreatePopOverShell(name, parent)
char *name;
Widget parent;
#else /* FUNCPROTO */
Widget CreatePopOverShell(char *name, Widget parent)
#endif /* FUNCPROTO */
{
  Display *dpy;

  ENTER_FUNCTION_DEBUG("ShellWids.c: CreatePopOverShell");

  dpy = XtDisplay(parent);
  if (imageReqVisual_s == NULL)
    ReadDefaults(dpy, DefaultScreen(dpy), NULL);

  LEAVE_FUNCTION_DEBUG("ShellWids.c: CreatePopOverShell");
  return(XtCreatePopupShell(name, overrideShellWidgetClass,
			     parent, ShellArgs_s, (Cardinal) 5));
}

#ifndef FUNCPROTO
Widget CreatePopTopShell(name, parent)
char *name;
Widget parent;
#else /* FUNCPROTO */
Widget CreatePopTopShell(char *name, Widget parent)
#endif /* FUNCPROTO */
{
  Display *dpy;

  ENTER_FUNCTION_DEBUG("ShellWids.c: CreatePopTopShell");

  dpy = XtDisplay(parent);
  if (imageReqVisual_s == NULL)
    ReadDefaults(dpy, DefaultScreen(dpy), NULL);

  LEAVE_FUNCTION_DEBUG("ShellWids.c: CreatePopTopShell");
  return(XtCreatePopupShell(name, topLevelShellWidgetClass,
			     parent, ShellArgs_s, (Cardinal) 5));
}

#ifndef FUNCPROTO
void AddAction(parent, name, func, popup)
Widget parent;
char *name;
XtActionProc func;
int popup;
#else /* FUNCPROTO */
void AddAction(Widget parent, char *name, XtActionProc func, int popup)
#endif /* FUNCPROTO */
{
#ifndef MOTIF 
  XtAppContext appcon;
  XtActionsRec actions[1];

  appcon = XtWidgetToApplicationContext(parent);
  actions[0].string = name;
  actions[0].proc = func;
  XtAppAddActions(appcon, actions, XtNumber(actions));
  if (popup) {

    /* Register the action XawPositionSimpleMenu. */
    XawSimpleMenuAddGlobalActions (appcon);

    XtRegisterGrabAction(func, True,
			 (ButtonPressMask | ButtonReleaseMask),
			 GrabModeAsync, GrabModeAsync);
  }
#endif
}

#ifndef FUNCPROTO
Widget TopLevelWidget(wid)
Widget wid;
#else /* FUNCPROTO */
Widget TopLevelWidget(Widget wid)
#endif /* FUNCPROTO */
{
  while(!XtIsSubclass(wid, shellWidgetClass))
    wid = XtParent(wid);
  return(wid);
}



#ifndef FUNCPROTO
void PopupMenubar(wid)
Widget wid;
#else /* FUNCPROTO */
void PopupMenubar(Widget wid)
#endif /* FUNCPROTO */
{
  menubar_s = wid;
}


#ifndef FUNCPROTO
void PopupExit(wid, event, params, num_params)
Widget wid;
XEvent *event;
String *params;
Cardinal *num_params;
#else /* FUNCPROTO */
void PopupExit(Widget wid, XEvent *event, String *params, Cardinal *num_params)
#endif /* FUNCPROTO */
{
  ENTER_FUNCTION_DEBUG("ShellWids.c: PopupExit");

  LEAVE_FUNCTION_DEBUG("ShellWids.c: PopupExit");
}


#ifndef FUNCPROTO
Widget PopupInit(name, parent, label)
char *name, *label;
Widget parent;
#else /* FUNCPROTO */
Widget PopupInit(char *name, Widget parent, char *label)
#endif /* FUNCPROTO */
{
  Display *dpy;
  Widget menu = NULL;
#ifdef MOTIF
  XmString st;
#endif

  ENTER_FUNCTION_DEBUG("ShellWids.c: PopupInit");

  dpy = XtDisplay(parent);
  if (imageReqVisual_s == NULL)
    ReadDefaults(dpy, DefaultScreen(dpy), NULL);

  /*  Control_e = parent; */

#ifdef MOTIF

  menu =  (Widget) XmCreatePulldownMenu(menubar_s, name, NULL, 0);
  n=0;
  st = XmStringCreate(label, XmSTRING_DEFAULT_CHARSET );
  XtSetArg(args[n], XmNlabelString, st); n++;
  XtSetArg(args[n], XmNsubMenuId, menu); n++; 
  lab = (Widget) XmCreateCascadeButtonGadget(menubar_s, name, args, n);
  XtManageChild(lab);
  XmStringFree(st);
#else
  if (label != NULL) {
    XtSetArg(ShellArgs_s[5], XtNlabel, label);
    menu = XtCreatePopupShell(name, simpleMenuWidgetClass, parent,
			      ShellArgs_s, 6);
  } else {
    menu = XtCreatePopupShell(name, simpleMenuWidgetClass, parent,
			      ShellArgs_s, 5);
  }
#endif

  LEAVE_FUNCTION_DEBUG("ShellWids.c: PopupInit");

  return(menu);
}

#ifndef FUNCPROTO
Widget PopupAdd(name, parent, label, callback_func, arg)
char *name, *label;
Widget parent;
XtCallbackProc callback_func;
int arg;
#else /* FUNCPROTO */
Widget PopupAdd(char *name, Widget parent, char *label, XtCallbackProc callback_func, int arg)
#endif /* FUNCPROTO */
{
  Widget entry;
#ifdef MOTIF
  Arg args[10];
#endif

  ENTER_FUNCTION_DEBUG("ShellWids.c: PopupAdd");
  FPRINTF2("    arg=%d\n", arg);

#ifdef MOTIF
  XmString st;
  n=0;
/*  XtSetArg(args[n], XmNmnemonic, mnemonic); n++; */
  st = XmStringCreate(label, XmSTRING_DEFAULT_CHARSET );
  XtSetArg(args[n], XmNlabelString, st); n++;
  entry = (Widget) XmCreatePushButtonGadget(parent, name, args, n); 
  XtManageChild(entry);
  XmStringFree(st);
  XtAddCallback(entry, XmNactivateCallback, callback_func,
		(XtPointer) MallocIntP(arg));
#else
  if (label) FPRINTF2("    label: %s\n", label);

  if (label) entry =
    XtVaCreateManagedWidget(name, smeBSBObjectClass, parent,
			    XtNlabel, label, NULL);
  else entry =
    XtVaCreateManagedWidget(name, smeBSBObjectClass, parent, NULL);

  if (callback_func)
    XtAddCallback(entry, XtNcallback, callback_func,
		  (XtPointer) MallocIntP(arg));
#endif
  LEAVE_FUNCTION_DEBUG("ShellWids.c: PopupAdd");
  return(entry);

} /* PopupAdd() */

#ifndef FUNCPROTO
Widget Popup_add(name, parent, label, callback_func, arg)
char *name, *label;
Widget parent;
XtCallbackProc callback_func;
int arg;
#else /* FUNCPROTO */
Widget Popup_add(char *name, Widget parent, char *label, XtCallbackProc callback_func, XtPointer arg)
#endif /* FUNCPROTO */
{
  Widget entry;
#ifdef MOTIF
  Arg args[10];
#endif

  ENTER_FUNCTION_DEBUG("ShellWids.c: Popup_add");
  FPRINTF2("    arg=%d\n", arg);

#ifdef MOTIF
  XmString st;
  n=0;
/*  XtSetArg(args[n], XmNmnemonic, mnemonic); n++; */
  st = XmStringCreate(label, XmSTRING_DEFAULT_CHARSET );
  XtSetArg(args[n], XmNlabelString, st); n++;
  entry = (Widget) XmCreatePushButtonGadget(parent, name, args, n); 
  XtManageChild(entry);
  XmStringFree(st);
  XtAddCallback(entry, XmNactivateCallback, callback_func,
		(XtPointer) MallocIntP(arg));
#else
  if (label) FPRINTF2("    label: %s\n", label);

  if (label) entry =
    XtVaCreateManagedWidget(name, smeBSBObjectClass, parent,
			    XtNlabel, label, NULL);
  else entry =
    XtVaCreateManagedWidget(name, smeBSBObjectClass, parent, NULL);

  if (callback_func)
    XtAddCallback(entry, XtNcallback, callback_func, arg);
#endif
  LEAVE_FUNCTION_DEBUG("ShellWids.c: Popup_add");
  return(entry);

} /* Popup_add() */

#ifndef FUNCPROTO
Widget PopupLine(name, parent)
char *name;
Widget parent;
#else /* FUNCPROTO */
Widget PopupLine(char *name, Widget parent)
#endif /* FUNCPROTO */
{
  Widget entry;

  entry = XtVaCreateManagedWidget(name, smeLineObjectClass, parent, NULL);

  return(entry);

} /* PopupLine() */

#ifndef FUNCPROTO
void PopupMenu(wid, event, params, num_params)
Widget wid;
XEvent *event;
String *params;
Cardinal *num_params;
#else /* FUNCPROTO */
void PopupMenu(Widget wid, XEvent *event, String *params, Cardinal *num_params)
#endif /* FUNCPROTO */
{
  Display *dpy;
  int type;

  ENTER_FUNCTION_DEBUG("ShellWids.c: PopupMenu");

  GV_s = wid; /* Image (or ImageOverlay) widget, XfwfPullDown widget or
	       * AsciiText widget. */

#ifndef MOTIF
  dpy = XtDisplay(wid);
  if (imageReqVisual_s == NULL)
    ReadDefaults(dpy, DefaultScreen(dpy), NULL);

  if (XtIsSubclass(wid, asciiTextWidgetClass) ||
      XtIsSubclass(wid, imageWidgetClass)) {
    if (event->type == ButtonPress || event->type == ButtonRelease) {
      FPRINTF2("    ButtonPress||ButtonRelease, event->type=%d\n",
	       event->type);
      XtCallActionProc (Control_e, "XawPositionSimpleMenu", 
			event, params, *num_params);
    } else {
      FPRINTF2("    !(ButtonPress||ButtonRelease),event->type=%d\n",
	       event->type);
      type = event->type;
      event->type = ButtonPress;
      XtCallActionProc (Control_e, "XawPositionSimpleMenu", 
			event, params, *num_params);
      event->type = type;
    }

    XtCallActionProc (Control_e, "MenuPopup", event, params, *num_params);
  }

  
  LEAVE_FUNCTION_DEBUG("ShellWids.c: PopupMenu");
#endif
}



#ifndef FUNCPROTO
Widget ImageFromMenu(wid)
Widget wid;
#else /* FUNCPROTO */
Widget ImageFromMenu(Widget wid)
#endif /* FUNCPROTO */
{
  if (XtIsSubclass(wid, imageWidgetClass)) {
    return(wid);
  }
  else {
    /* XfwfPullDown widget (child of MenuBar which is child of ImageForm). */

    Widget w;

    w = XtParent(XtParent(wid));

    /* ImageForm widget is subclass of Composite. XtNameToWidget will search
     * among its normal children. */
    w = XtNameToWidget(w, "image");
    return(w);
  }
} /* ImageFromMenu() */

#ifndef FUNCPROTO
Widget PopupWidget()
#else /* FUNCPROTO */
Widget PopupWidget(void)
#endif /* FUNCPROTO */
{

  if (XtIsSubclass(GV_s, textWidgetClass)) {
    return(GV_s);
  } else {
    return(ImageFromMenu(GV_s));
  }

} /* PopupWidget() */
