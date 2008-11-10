
/*

________________________________________________________________

        xshowImageOptionsMenu.c
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

static char *Id = "$Id$, Blab, UiO";



#include <xite/includes.h>
#include <xite/biff.h>
#include <X11/Xos.h>
#include <X11/Intrinsic.h>
#include "xshowImageOptionsMenu.h"
#ifdef HAVE_STDIO_H
#  include <stdio.h>
#endif
#include <ctype.h>
#include <X11/StringDefs.h>
#include <X11/Xaw/SmeBSB.h>
#include <X11/Xaw/Form.h>
#include <xite/Image.h>
#include <xite/ImageOverlay.h>
#include <xite/ShellWids.h>
#include <xite/Xcolor.h>
#include "Xcontrol.h"
#include <xite/XmenuBar.h>
#include <xite/debug.h>

#define MAXENTRIES 15

#define MARKIF(e) (e) ? 'x' : ' '

/* External global variables. */

extern int rgb_e;
extern int logpos_e;
extern menu_button image_menus_e;
extern Widget optionsmenu_e;

/* Internal global variables. */

static Display *dpy_s;
static int     screen_s;
static Widget  Control_s;

static SmeBSBObject entries_s[MAXENTRIES];



/*OptionsHook=OptionsMenuInit*/
/*ProgsMenuInit=OptionsMenuInit*/
/*OptionsMenuInit*

________________________________________________________________

		Options menu
________________________________________________________________

Name:		OptionsMenuInit, OptionsHook, ProgsMenuInit - Popup menus in
                the XITE xshow image widget.
Syntax:		| #include "xshowImageOptionsMenu.h"
                | 
Description:    'OptionsMenuInit' saves 'display', 'screenno' and 'parent' in
                the global variables 'dpy_s', 'screen_s' and 'Control_s'.
		Then the options popupmenu is initialized with parent 'parent',
		and the	entries (given by the static routine 'flags2') are
		added to the menu. This is done in the background by a call to
		'XtAppAddWorkProc(3X)'. The static routine 'OptionsSelect' is
		installed as callback routine for the menu entries.

		'OptionsHook' sets the initial mark (''x'') in each of the
		options popupmenu entries, depending on the status of the
		ImageOverlayWidget 'wid' and the external global variable
		'logpos_e'.

		'ProgsMenuInit' saves its arguments in the same three global
		variables as 'OptionsMenuInit' does. The "Tools" popupmenu is
		initialized with parent 'parent', and the entries (given by
		the static routine 'progs2') are added to the menu. This is
		done in the background by a call to 'XtAppAddWorkProc(3X)'.
		The static routine 'ProgSelect' is installed as callback
		routine for the menu entries. 'ProgsMenuInit' reads the xshow
		menu files given by the filename 'mf', if not NULL, and
		.xshowrc and xshow_menues (the latter two from all the
		directories in the $XSHOWPATH environment variable, only
		if 'envir_also' is non-zero).

Author:		Otto Milvang
Doc:            Svein Bøe
________________________________________________________________

*/

typedef enum {
  REDUCED_ENTRY,
  RGB_ENTRY,
  LOGPOS_ENTRY,
  ZOOMALL_ENTRY,
  ASPECT_ENTRY,
  MENUBAR_ENTRY,
  ROIFILL_ENTRY,
  ROIPERM_ENTRY,
  ROISQUARE_ENTRY,
  ROIZOOMPAN_ENTRY
} entry_type;

static void OptionsHook(Widget wid, XEvent *event, String *params, Cardinal *num_params)
{
  ImageOverlayWidget iow;
  int permanent, fill, xaspect;
  display_method displaymethod;
  Boolean menubar, variable_aspect, roi_zoom_pan, sensitive;
  char *label;

  ENTER_FUNCTION_DEBUG("xshowImageOptionsMenu.c: OptionsHook");

  if (entries_s[0] == NULL) return;

  iow = (ImageOverlayWidget) ImageFromMenu(wid);
  XtVaGetValues((Widget) iow, XtNdisplayMethod, &displaymethod,
		XtNroiFill, &fill,
		XtNroiPermanent, &permanent,
		XtNroiXaspect, &xaspect,
		XtNroiZoomPan, &roi_zoom_pan,
		XtNmenubar, &menubar,
		XtNvariableAspect, &variable_aspect,
		NULL);

  sensitive = ImageHasColormap((ImageWidget) iow) ? 1 : 0;
  XtVaGetValues((Widget) entries_s[REDUCED_ENTRY], XtNlabel, &label, NULL);
  label[0] = MARKIF(((displaymethod & 3) == 0) && sensitive);
  XtVaSetValues((Widget) entries_s[REDUCED_ENTRY],
		XtNlabel,     label,
		XtNsensitive, sensitive, NULL);

  XtVaGetValues((Widget) entries_s[RGB_ENTRY], XtNlabel, &label, NULL);
  label[0] = MARKIF(rgb_e);
  XtVaSetValues((Widget) entries_s[RGB_ENTRY], XtNlabel, label, NULL);

  XtVaGetValues((Widget) entries_s[LOGPOS_ENTRY], XtNlabel, &label, NULL);
  label[0] = MARKIF(logpos_e); 
  XtVaSetValues((Widget) entries_s[LOGPOS_ENTRY], XtNlabel, label, NULL);

  XtVaGetValues((Widget) entries_s[ZOOMALL_ENTRY], XtNlabel, &label, NULL);
  label[0] = MARKIF(ImageGetZoomAll((Widget) iow)); 
  XtVaSetValues((Widget) entries_s[ZOOMALL_ENTRY], XtNlabel, label, NULL);

  XtVaGetValues((Widget) entries_s[ASPECT_ENTRY], XtNlabel, &label, NULL);
  label[0] = MARKIF(!variable_aspect); 
  XtVaSetValues((Widget) entries_s[ASPECT_ENTRY], XtNlabel, label, NULL);

  XtVaGetValues((Widget) entries_s[MENUBAR_ENTRY], XtNlabel, &label, NULL);
  label[0] = MARKIF(menubar); 
  XtVaSetValues((Widget) entries_s[MENUBAR_ENTRY], XtNlabel, label, NULL);

  XtVaGetValues((Widget) entries_s[ROIFILL_ENTRY], XtNlabel, &label, NULL);
  label[0] = MARKIF(fill);
  XtVaSetValues((Widget) entries_s[ROIFILL_ENTRY], XtNlabel, label, NULL);

  XtVaGetValues((Widget) entries_s[ROIPERM_ENTRY], XtNlabel, &label, NULL);
  label[0] = MARKIF(permanent);
  XtVaSetValues((Widget) entries_s[ROIPERM_ENTRY], XtNlabel, label, NULL);

  XtVaGetValues((Widget) entries_s[ROISQUARE_ENTRY], XtNlabel, &label, NULL);
  label[0] = MARKIF(xaspect); 
  XtVaSetValues((Widget) entries_s[ROISQUARE_ENTRY], XtNlabel, label, NULL);

  XtVaGetValues((Widget) entries_s[ROIZOOMPAN_ENTRY], XtNlabel, &label, NULL);
  label[0] = MARKIF(roi_zoom_pan); 
  XtVaSetValues((Widget) entries_s[ROIZOOMPAN_ENTRY], XtNlabel, label, NULL);

  LEAVE_FUNCTION_DEBUG("xshowImageOptionsMenu.c: OptionsHook");
}

static void OptionsSelect(Widget w, XtPointer clos, XtPointer call_data)
{
  Widget image_widget;
  entry_type closure = (entry_type) *((int *) clos);

  ENTER_FUNCTION_DEBUG("xshowImageOptionsMenu.c: OptionsSelect");

  /*  free(clos); */
  if (entries_s[0] == NULL) return;
  if (closure < 1 && DisplayPlanes(dpy_s, screen_s) < 8) return;

  image_widget = PopupWidget(); /* Image or ImageOverlay widget. */

  switch(closure)
    {
    case REDUCED_ENTRY: {
      int colortab;
      display_method displaymethod;

      XtVaGetValues(image_widget, XtNdisplayMethod, &displaymethod,
		    XtNcolortab, &colortab, NULL);

      switch(displaymethod)
	{
	case PSEUDO_COLOR_8_A:  
	  displaymethod = PSEUDO_COLOR_8_B;
	  XtVaSetValues(image_widget, XtNdisplayMethod, displaymethod,
			XtNcolorCells,        256,
			XtNcolorPtr,          NULL,
			XtNoverlayColorCells, 0,
			XtNoverlayColorPtr,   NULL, NULL);
	  break;
	case PSEUDO_COLOR_8_B: 
	  displaymethod = PSEUDO_COLOR_8_A;       
	  XtVaSetValues(image_widget, XtNdisplayMethod, displaymethod,
			XtNcolorCells,        128,
			XtNcolorPtr,          NULL,
			XtNoverlayColorCells, 32,
			XtNoverlayColorPtr,   NULL, NULL);
	  break;
	case DIRECT_COLOR_24_A:  
	  displaymethod = DIRECT_COLOR_24_B;
	  XtVaSetValues(image_widget, XtNdisplayMethod, displaymethod,
			XtNcolorCells,        256,
			XtNcolorPtr,          NULL,
			XtNoverlayColorCells, 0,
			XtNoverlayColorPtr,   NULL, NULL);
	  break;
	case DIRECT_COLOR_24_B: 
	  displaymethod = DIRECT_COLOR_24_A;       
	  XtVaSetValues(image_widget, XtNdisplayMethod, displaymethod,
			XtNcolorCells,        128,
			XtNcolorPtr,          NULL,
			XtNoverlayColorCells, 32,
			XtNoverlayColorPtr,   NULL, NULL);
	  break;
	default:
	  break;
        }

      if (colortab) Color_set((ImageWidget) image_widget, colortab);
    }
      break;
    case RGB_ENTRY: {
      rgb_e = 1 - rgb_e;
    }
      break;
    case LOGPOS_ENTRY: {
      int width, height;

      logpos_e = 1 - logpos_e;
      XtVaGetValues(image_widget, XtNimageWidth, &width,
		                  XtNimageHeight, &height, NULL);
      if (!logpos_e) Message_size(width, height);
    }
      break;
    case ZOOMALL_ENTRY:
      ImageSetZoomAll(image_widget, !ImageGetZoomAll(image_widget));
      break;
    case ASPECT_ENTRY: {
      Boolean variable_aspect;
      
      XtVaGetValues(image_widget, XtNvariableAspect, &variable_aspect, NULL);

      variable_aspect = variable_aspect ? 0 : 1;
      XtVaSetValues(image_widget, XtNvariableAspect, variable_aspect, NULL);
    }
      break;
    case MENUBAR_ENTRY: {
      Boolean menubar;
      Widget MenuBar;
      Dimension core_w, core_h;

      XtVaGetValues(image_widget, XtNmenubar, &menubar,
		                  XtNwidth,   &core_w,
		                  XtNheight,  &core_h,
		                  NULL);
      menubar = menubar ? 0 : 1;

      if (menubar) {
	if (XtIsSubclass(image_widget, imageOverlayWidgetClass))
	  MenuBar = CreateMenuBar(XtParent(image_widget), image_menus_e);
	else
	  MenuBar = CreateMenuBar(XtParent(image_widget),
				  image_menus_e & (~OVERLAYS_BUTTON));

	XtVaSetValues(MenuBar,      XtNwidth,    core_w,  NULL);
	XtVaSetValues(image_widget, XtNfromVert, MenuBar, 
		                    XtNmenubar,  menubar, NULL);
      } else {
	MenuBar = XtNameToWidget(XtParent(image_widget), "menubar");
	XtDestroyWidget(MenuBar);
	XtVaSetValues(image_widget, /* XtNwidth,    core_w,
		                    XtNheight,   core_h, */
		                    XtNfromVert, NULL,
		                    XtNmenubar,  menubar, NULL);

	XawFormDoLayout(XtParent(image_widget), 1);
      }
    }
      break;
    case ROIFILL_ENTRY: {
      int fill;

      XtVaGetValues(image_widget, XtNroiFill, &fill, NULL);
      XtVaSetValues(image_widget, XtNroiFill, 1 - fill, NULL);
    }
      break;
    case ROIPERM_ENTRY: {
      int permanent;

      XtVaGetValues(image_widget, XtNroiPermanent, &permanent, NULL);
      XtVaSetValues(image_widget, XtNroiPermanent, 1 - permanent, NULL);
    }
      break;
    case ROISQUARE_ENTRY: {
      int xaspect, yaspect;

      XtVaGetValues(image_widget, XtNroiXaspect, &xaspect,
		    XtNroiYaspect, &yaspect, NULL);
      if (xaspect)
	XtVaSetValues(image_widget, XtNroiXaspect, 0,
		      XtNroiYaspect, 0, NULL);
      else
	XtVaSetValues(image_widget, XtNroiXaspect, 1,
		      XtNroiYaspect, 1, NULL);
    }
      break;
    case ROIZOOMPAN_ENTRY: {
      Boolean roi_zoom_pan;

      XtVaGetValues(image_widget, XtNroiZoomPan, &roi_zoom_pan, NULL);
      XtVaSetValues(image_widget, XtNroiZoomPan, 1 - roi_zoom_pan, NULL);
    }
      break;
    } /* switch(closure) */

  LEAVE_FUNCTION_DEBUG("xshowImageOptionsMenu.c: OptionsSelect");

  return;
}

void OptionsMenuInit(Display *display, int screenno, Widget parent)
{
  char *label = NULL;

  ENTER_FUNCTION_DEBUG("xshowImageOptionsMenu.c: OptionsMenuInit");
  dpy_s = display;
  screen_s = screenno;
  Control_s = parent;

  optionsmenu_e = PopupInit("optionsmenu", Control_s, NULL);
  XtVaGetValues(optionsmenu_e, XtNlabel, &label, NULL);
  if (!label) {
    XtDestroyWidget(optionsmenu_e);
    optionsmenu_e = PopupInit("optionsmenu", Control_s, "Options menu");
  }

  (void) PopupLine("optionsmenusep", optionsmenu_e);

  entries_s[REDUCED_ENTRY] =
    (SmeBSBObject) PopupAdd("optionsmenuentry", optionsmenu_e,
			    "  Reduced colors ", OptionsSelect, REDUCED_ENTRY);

  entries_s[ASPECT_ENTRY] =
    (SmeBSBObject) PopupAdd("optionsmenuentry", optionsmenu_e,
			    "  Fixed aspect ", OptionsSelect, ASPECT_ENTRY);

  entries_s[MENUBAR_ENTRY] =
    (SmeBSBObject) PopupAdd("optionsmenuentry", optionsmenu_e,
			    "  Menubar  ", OptionsSelect, MENUBAR_ENTRY);

  (void) PopupLine("optionsmenusep", optionsmenu_e);

  entries_s[ROIFILL_ENTRY] =
    (SmeBSBObject) PopupAdd("optionsmenuentry", optionsmenu_e,
			    "  ROI fill   ", OptionsSelect, ROIFILL_ENTRY);

  entries_s[ROIPERM_ENTRY] =
    (SmeBSBObject) PopupAdd("optionsmenuentry", optionsmenu_e,
			    "  ROI permanent ", OptionsSelect, ROIPERM_ENTRY);

  entries_s[ROISQUARE_ENTRY] =
    (SmeBSBObject) PopupAdd("optionsmenuentry", optionsmenu_e,
			    "  ROI square ", OptionsSelect, ROISQUARE_ENTRY);

  entries_s[ROIZOOMPAN_ENTRY] =
    (SmeBSBObject) PopupAdd("optionsmenuentry", optionsmenu_e,
			    "  ROI zoom & pan", OptionsSelect,
			    ROIZOOMPAN_ENTRY);

  (void) PopupLine("optionsmenusep", optionsmenu_e);

  entries_s[RGB_ENTRY] =
    (SmeBSBObject) PopupAdd("optionsmenuentry", optionsmenu_e,
			    "  Interpret next as RGB", OptionsSelect,
			    RGB_ENTRY);

  entries_s[LOGPOS_ENTRY] =
    (SmeBSBObject) PopupAdd("optionsmenuentry", optionsmenu_e,
			    "  Log position ", OptionsSelect, LOGPOS_ENTRY);

  entries_s[ZOOMALL_ENTRY] =
    (SmeBSBObject) PopupAdd("optionsmenuentry", optionsmenu_e,
			    "  Zoom all ", OptionsSelect, ZOOMALL_ENTRY);

  AddAction(parent, "OptionsHook", OptionsHook, 0);

  LEAVE_FUNCTION_DEBUG("xshowImageOptionsMenu.c: OptionsMenuInit");
}
