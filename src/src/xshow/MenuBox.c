/***********************************************************************
 *
 * MenuBox.c - MenuBox composite widget
 * Robert Blumen blumen@arisia.xerox.com blumen@cad.berkeley.edu
 *
 * MenuBox.c - code for MenuBox widget
 *
 * Converted to X11 Release 3 by
 *	Win Treese
 *	Digital Equipment Corp.
 *	treese@crl.dec.com
 ***********************************************************************/

/***********************************************************
Copyright 1988 by Digital Equipment Corporation, Maynard, Massachusetts,
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

#include <xite/includes.h>
#include <stdlib.h>
#include <X11/IntrinsicP.h>
#include <X11/StringDefs.h>
#include <X11/Xatom.h>
#include "XawMisc.h"
#include "MenuBoxP.h"
#include "SubMenuCommand.h"
#include "MenuCommand.h"
#include <xite/debug.h>
#include <xite/ShellWids.h>
#include XITE_STDIO_H

#ifndef FUNCPROTO
static void ChangeManaged();
#else /* FUNCPROTO */
static void ChangeManaged(Widget w);
#endif /* FUNCPROTO */

static Dimension defVSpace = 1;

static XtResource resources[] = {
	{ XtNvSpace, XtCVSpace, XtRDimension, sizeof(Dimension), 
		  XtOffsetOf(MenuBoxRec, box.v_space), XtRDimension,
	          (XtPointer) &defVSpace },
};

#ifndef FUNCPROTO
static void ChangeEntry();
#else /* FUNCPROTO */
static void ChangeEntry(Widget wid, XEvent *event, String *params, Cardinal *num_params);
#endif /* FUNCPROTO */

static XtActionsRec actionsList[] = {
  {"change_entry",       ChangeEntry},
};

MenuBoxClassRec menuboxClassRec = {
  {
/* core_class fields      */
    /* superclass         */    (WidgetClass) &boxClassRec,
    /* class_name         */    "MenuBox",
    /* widget_size        */    sizeof(MenuBoxRec),
    /* class_initialize   */    NULL,
    /* class_part_init    */	NULL,
    /* class_inited       */	FALSE,
    /* initialize         */    NULL,
    /* initialize_hook    */	NULL,
    /* realize            */    XtInheritRealize,
    /* actions            */    actionsList,
    /* num_actions	  */	XtNumber(actionsList),
    /* resources          */    resources,
    /* num_resources      */    XtNumber(resources),
    /* xrm_class          */    NULLQUARK,
    /* compress_motion	  */	TRUE,
    /* compress_exposure  */	TRUE,
    /* compress_enterleave*/	TRUE,
    /* visible_interest   */    FALSE,
    /* destroy            */    NULL,
    /* resize             */    XtInheritResize,
    /* expose             */    NULL,
    /* set_values         */    NULL,
    /* set_values_hook    */	NULL,
    /* set_values_almost  */    XtInheritSetValuesAlmost,
    /* get_values_hook    */	NULL,
    /* accept_focus       */    NULL,
    /* version            */	XtVersion,
    /* callback_private   */    NULL,
    /* tm_table           */    NULL,
    /* query_geometry     */	XtInheritQueryGeometry,
    /* display_accelerator*/	XtInheritDisplayAccelerator,
    /* extension	  */	NULL,
  },{
/* composite_class fields */
    /* geometry_manager   */    XtInheritGeometryManager,
    /* change_managed     */    ChangeManaged,
    /* insert_child	  */	XtInheritInsertChild,
    /* delete_child	  */	XtInheritDeleteChild,
  },{
/* Box class fields */
    /* empty		  */	0,
  },{
/* MenuBox class fields */
   /* empty               */    0,
  },
};

WidgetClass menuboxWidgetClass = (WidgetClass)&menuboxClassRec;


/****************************************************************
 *
 * Private Routines
 *
 ****************************************************************/

#ifndef FUNCPROTO
static void ChangeEntry(wid, event, params, num_params)
Widget wid;
XEvent *event;
String *params;
Cardinal *num_params;
#else /* FUNCPROTO */
static void ChangeEntry(Widget wid, XEvent *event, String *params, Cardinal *num_params)
#endif /* FUNCPROTO */
{
  Widget child, menBox, menShell;
  int root_x, root_y, win_x, win_y, child_num, found_child = -1, direction;
  Cardinal num_children;
  unsigned int mask;
  Window root, win;
  WidgetList children;
  Dimension width, height, wid_x, wid_y, box_x, box_y, border_width,
            top_x, top_y, hspace;

  ENTER_FUNCTION_DEBUG("MenuBox.c: ChangeEntry");

  (void) XQueryPointer(XtDisplay(wid), XtWindow(wid), &root, &win,
		       &root_x, &root_y, &win_x, &win_y, &mask);
  /* win == 0 and win_y < 0 if pointer was in window manager title-bar. */

  menShell = TopLevelWidget(wid);
  menBox   = XtNameToWidget(menShell, "menuBox");

  XtVaGetValues(menShell,    XtNx,           &top_x,
		             XtNy,           &top_y, NULL);
  XtVaGetValues(menBox,      XtNx,           &box_x,
		             XtNy,           &box_y,
		             XtNnumChildren, &num_children,
		             XtNchildren,    &children,
		             XtNhSpace,      &hspace, NULL);
  XtVaGetValues(children[0], XtNwidth,       &width,
		             XtNheight,      &height,
		             XtNborderWidth, &border_width, NULL);

  for (child_num = 0; child_num < num_children; child_num++) {
    child = children[child_num];
    XtVaGetValues(child, XtNx, &wid_x, XtNy, &wid_y, NULL);

    if (root_x >= top_x + box_x + wid_x &&
	root_x <= top_x + box_x + wid_x + width &&
	root_y >= top_y + box_y + wid_y &&
	root_y <= top_y + box_y + wid_y + height) {
      /* Found menu entry (or separation label) in which the pointer is
       * positioned. */
      found_child = child_num;
      break;
    }
  }

  /* Found entry where pointer is. If found_child == -1, then pointer must
   * be in window manager decoration (title field).
   */
  
  /* Determine wanted direction.
   * 1: down, -1: up. Default is down.
   */
  if (*num_params == 0) direction = 1;
  else direction = atoi(params[0]);
  
  /* Check direction. */
  if (found_child <= 1 && direction == -1) {
    child_num = num_children - 1;
  } else if (found_child >= (num_children - 1) && direction == 1) {
    child_num = 1;
  } else if (found_child == -1) {
    child_num = 1;
  } else {
    child_num = found_child + direction;
  }

  while (XtClass(children[child_num]) != subMenuCommandWidgetClass &&
	 XtClass(children[child_num]) != menuCommandWidgetClass) {
    if (direction == 1 && child_num + 1 < num_children)
      child_num++;
    else if (direction == 1) child_num = 1;
    else if (direction == -1 && child_num - 1 > 0)
      child_num--;
    else if (direction == -1) child_num = num_children - 1;
  }  

  child = children[child_num];
  XtVaGetValues(child, XtNy, &wid_y, NULL);

  if (XtClass(wid) == menuboxWidgetClass)
    XWarpPointer(XtDisplay(wid), (Window) NULL, XtWindow(menBox), 0, 0, 0, 0,
		 (int) win_x, (int) (wid_y + height / 2));
  else 
    XWarpPointer(XtDisplay(wid), (Window) NULL, XtWindow(menBox), 0, 0, 0, 0,
		 (int) (win_x + hspace + border_width),
		 (int) (wid_y + height / 2));
  
  LEAVE_FUNCTION_DEBUG("MenuBox.c: ChangeEntry");

} /* ChangeEntry() */

#ifndef FUNCPROTO
static void accLabel(wid, lab_width, acc_lab, acc_lab_width, font)
Widget wid;
char **acc_lab;
int *lab_width, *acc_lab_width;
XFontStruct **font;
#else /* FUNCPROTO */
static void accLabel(Widget wid, int *lab_width, char **acc_lab, int *acc_lab_width, XFontStruct **font)
#endif /* FUNCPROTO */
{
  ENTER_FUNCTION_DEBUG("MenuBox.c: accLabel");

  if (XtClass(wid) == menuCommandWidgetClass ||
      XtClass(wid) == subMenuCommandWidgetClass) {
    char *acc, *label;

    XtVaGetValues(wid, XtNlabel, &label,
		       XtNfont,  font, NULL);

    acc = strchr(label, '#');
    if (!acc) {
      *lab_width     = XTextWidth(*font, label, strlen(label));
      *acc_lab       = NULL;
      *acc_lab_width = 0;

      LEAVE_FUNCTION_DEBUG("MenuBox.c: accLabel");
      return;
    }

    /* Found keyboard accelerator. */

    *lab_width     = XTextWidth(*font, label, acc-label);
    *acc_lab       = (acc + 1);
    *acc_lab_width = XTextWidth(*font, *acc_lab, strlen(*acc_lab));
  } else {
    *lab_width     = 0;
    *acc_lab       = NULL;
    *acc_lab_width = 0;
    *font          = (XFontStruct *) NULL;
  }

  LEAVE_FUNCTION_DEBUG("MenuBox.c: accLabel");

} /* accLabel() */

#ifndef FUNCPROTO
static void modLabel(wid, width, max_lab_width, max_acc_width)
Widget wid;
Dimension width;
int max_lab_width, max_acc_width;
#else /* FUNCPROTO */
static void modLabel(Widget wid, Dimension width, int max_lab_width, int max_acc_width)
#endif /* FUNCPROTO */
{
  ENTER_FUNCTION_DEBUG("MenuBox.c: modLabel");

  if (XtClass(wid) == menuCommandWidgetClass ||
      XtClass(wid) == subMenuCommandWidgetClass) {
    char *acc, *label;

    XtVaGetValues(wid, XtNlabel, &label, NULL);
    acc = strchr(label, '#');

    if (acc) {
      /* Found keyboard accelerator. Add intermediate blanks to get
       * accelerator description to appear right-justified.
       */

      char new_label[100], *p;
      Dimension width_int;
      XFontStruct *font;
      int lab_w, acc_w, blank_w, num_blanks;

      *acc++ = '\0';

      XtVaGetValues(wid, XtNfont,          &font,
		         XtNinternalWidth, &width_int, NULL);

      lab_w   = XTextWidth(font, label, strlen(label));
      acc_w   = XTextWidth(font, acc,   strlen(acc));
      blank_w = XTextWidth(font, " ", strlen(" "));

      num_blanks = (width - 2 * width_int - lab_w - max_acc_width) / blank_w;

      strcpy(new_label, label);
      p = new_label + strlen(new_label);

      sprintf(p, "%*c%s", num_blanks, ' ', acc);

      XtVaSetValues(wid, XtNlabel, new_label, NULL);
    }
  }

  LEAVE_FUNCTION_DEBUG("MenuBox.c: modLabel");
} /* modLabel() */

/*----------------------------------------------------------------------

This procedure makes sure that all the boxes are the same width;
Could check first that children are subclass of labelWidget, but
don't bother.
*/

/* ARGSUSED */
static void
#ifndef FUNCPROTO
ChangeManaged(w)
	Widget w;
#else /* FUNCPROTO */
ChangeManaged(Widget w)
#endif /* FUNCPROTO */
{
  int j, lab_width, acc_width, max_lab_width, max_acc_width;
  Dimension width = 0, internal_width;
  MenuBoxWidget mbw = (MenuBoxWidget) w;
  WidgetClass wc;
  XtWidgetProc wpr;
  Widget  child;
  XtWidgetGeometry intended, preferred;
  char *acc_label;
  XFontStruct *font, *keep_font;

  ENTER_FUNCTION_DEBUG("MenuBox.c: ChangeManaged");

  /* get max width of current children */
  intended.request_mode = 0;

  for (j = 0; j < mbw->composite.num_children; j++) {
    child = mbw->composite.children[j];
    if (child->core.managed) {
      XtQueryGeometry(child, &intended, &preferred);
      AssignMax(width, preferred.width);
    }
  }

  /* get max widths of labels and accelerator descriptions */
  max_lab_width = 0; max_acc_width = 0;

  for (j = 0; j < mbw->composite.num_children; j++) {
    child = mbw->composite.children[j];
    if (child->core.managed) {
      accLabel(child, &lab_width, &acc_label, &acc_width, &font);
      if (font) keep_font = font;
      if (lab_width > max_lab_width) max_lab_width = lab_width;
      if (acc_width > max_acc_width) max_acc_width = acc_width;
    }
  }

  /* adjust required width */
  XtVaGetValues(child, XtNinternalWidth, &internal_width, NULL);

  if (max_lab_width + max_acc_width + 2 * internal_width > width)
    width = max_lab_width + max_acc_width + 2 * internal_width;
  if (keep_font) width += XTextWidth(keep_font, "  ", strlen("  "));

  /* make any one smaller than max width larger */
  for (j = 0; j < mbw->composite.num_children; j++) {
    child = mbw->composite.children[j];
    if (XtClass(child) == menuCommandWidgetClass ||
	XtClass(child) == subMenuCommandWidgetClass)
      modLabel(child, width, max_lab_width, max_acc_width);

    if (child->core.width != width)
      XtResizeWidget(child, width, child->core.height,
		     child->core.border_width);

  }

  /* call superclass changed managed to do layout */
  wc = XtSuperclass(w);
  wpr = ((CompositeWidgetClass) wc)->composite_class.change_managed;
  (*wpr)(w);

  LEAVE_FUNCTION_DEBUG("MenuBox.c: ChangeManaged");

  return;
}



