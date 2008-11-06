/*

________________________________________________________________

        XmenuBar.c
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

static char *Id = "$Id";



/*I*

________________________________________________________________

		Include files and definitions
________________________________________________________________

*/

#include <xite/includes.h>
#include <X11/Xos.h>
#include <X11/Intrinsic.h>
#include <X11/StringDefs.h>
#include <Xfwf/PullDown.h>
#include <Xfwf/MenuBar.h>
#include <xite/debug.h>
#include <xite/XmenuBar.h>

extern Widget colorsmenu_e, overlaysmenu_e, toolsmenu_e, optionsmenu_e,
              visualsmenu_e;

static void CreateMenuButtons(Widget parent, menu_button buttons)
{
  Widget ColorMenuButton, OverlayMenuButton, ProgMenuButton, FlagMenuButton,
         VisualMenuButton;
  char *label = NULL;

  if (buttons & COLORS_BUTTON) {
    ColorMenuButton   = XtVaCreateManagedWidget("colors",
						xfwfPullDownWidgetClass,
						parent,
						XtNpopup, colorsmenu_e,
						NULL);
    XtVaGetValues(ColorMenuButton, XtNlabel, &label, NULL);
    if (!label) XtVaSetValues(ColorMenuButton, XtNlabel, "Colors", NULL);
  }

  if (buttons & OVERLAYS_BUTTON) {
    OverlayMenuButton = XtVaCreateManagedWidget("overlays",
						xfwfPullDownWidgetClass,
						parent,
						XtNpopup, overlaysmenu_e,
						NULL);

    XtVaGetValues(OverlayMenuButton, XtNlabel, &label, NULL);
    if (!label) XtVaSetValues(OverlayMenuButton, XtNlabel, "OverlayColors",
			      NULL);
  }


  if (buttons & TOOLS_BUTTON) {
    ProgMenuButton    = XtVaCreateManagedWidget("tools",
						xfwfPullDownWidgetClass,
						parent,
						XtNpopup, toolsmenu_e,
						NULL);

    XtVaGetValues(ProgMenuButton, XtNlabel, &label, NULL);
    if (!label) XtVaSetValues(ProgMenuButton, XtNlabel, "Tools", NULL);
  }

  if (buttons & OPTIONS_BUTTON) {
    FlagMenuButton    = XtVaCreateManagedWidget("options",
						xfwfPullDownWidgetClass,
						parent,
						XtNpopup, optionsmenu_e,
						NULL);

    XtVaGetValues(FlagMenuButton, XtNlabel, &label, NULL);
    if (!label) XtVaSetValues(FlagMenuButton, XtNlabel, "Options", NULL);
  }

  if (buttons & VISUALS_BUTTON) {
    VisualMenuButton  = XtVaCreateManagedWidget("visuals",
						xfwfPullDownWidgetClass,
						parent,
						XtNpopup, visualsmenu_e,
						NULL);

    XtVaGetValues(VisualMenuButton, XtNlabel, &label, NULL);
    if (!label) XtVaSetValues(VisualMenuButton, XtNlabel, "Visuals", NULL);
  }

} /* CreateMenuButtons() */
    
Widget CreateMenuBar(Widget parent, menu_button buttons)
{
  Widget menu_bar;
  Dimension width;

  XtVaGetValues(parent, XtNwidth, &width, NULL);
  menu_bar = XtVaCreateManagedWidget("menubar",
				     xfwfMenuBarWidgetClass,
				     parent,
				     XtNwidth, width,
				     XtNheight, 30,
				     NULL);
  CreateMenuButtons(menu_bar, buttons);

  return(menu_bar);

} /* CreateMenuBar() */
