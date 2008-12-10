
/*

________________________________________________________________

        ImageVisualsMenu.c
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
#include <xite/ImageVisualsMenu.h>
#include XITE_MALLOC_H
#include <ctype.h>
#include <X11/StringDefs.h>
#include <X11/Xaw/SmeBSB.h>
#include <xite/ImageOverlay.h>
#include <xite/Visual.h>
#include <xite/ShellWids.h>
#include <xite/ximage.h>
#include <xite/debug.h>

#define MAXENTRIES 9

#define MARKIF(e) (e) ? 'x' : ' '

/* External global variables. */

extern Widget visualsmenu_e;
extern menu_button image_menus_e;

/* Internal global variables. */

static Display *dpy_s;
static int     screen_s;
static Widget  Control_s;

static SmeBSBObject entries_s[MAXENTRIES];



static void get_widget_resources(Widget iw, ArgList *args, Cardinal *num_args)
{
  ArgList args1, args2, args_tmp;
  Cardinal num_args1 = 0, num_args2 = 0, num = 0;
  int i, depth;

  ImageGetImageResources((ImageWidget) iw, &args1, &num_args1);

  if (XtIsSubclass(iw, imageOverlayWidgetClass))
    ImageOverlayGetImageOverlayResources((ImageOverlayWidget) iw,
					 &args2, &num_args2);

  /* Prepend the core resource XtNdepth to the image resources. Append the
   * imageOverlay resources for widgets of this class. */

  num      = 1 + num_args1 + num_args2;
  args_tmp = (ArgList) malloc(num * sizeof(Arg));

  XtVaGetValues(iw, XtNdepth, &depth, NULL);

  args_tmp[0].name  = XtNdepth;
  args_tmp[0].value = (XtArgVal) depth;

  for (i = 0; i < num_args1; i++) {
    args_tmp[i + 1].name  = args1[i].name;
    args_tmp[i + 1].value = args1[i].value;
  }
  for (i = 0; i < num_args2; i++) {
    args_tmp[i + 1 + num_args1].name  = args2[i].name;
    args_tmp[i + 1 + num_args1].value = args2[i].value;
  }

  *args     = args_tmp;
  *num_args = num;

  return;

} /* get_widget_resources() */

static void fix_widget_resources(ArgList args, Cardinal *num_args, IMAGE img, Visual *visual, int depth, display_method displaymethod)
{
  /* Some resources must be different in the new widget (compared to the
   * original widget). Apart from XtNvisual and XtNdepth, XtNdisplayMethod
   * must also be different because it depends on the visual type.

   * Some other resources are also better left for the widget to determine. */

  int found, i;

  found = 0;
  for (i = 0; i < (*num_args) - found; i++) {

    args[i].value = args[i+found].value;
    args[i].name  = args[i+found].name;

    if (!strcmp(args[i].name, XtNimageImage)) {
      args[i].value = (XtArgVal) img; continue;
    }
    if (!strcmp(args[i].name, XtNvisual)) {
      args[i].value = (XtArgVal) visual; continue;
    }
    if (!strcmp(args[i].name, XtNdepth)) {
      args[i].value = (XtArgVal) depth; continue;
    }
    if (!strcmp(args[i].name, XtNdisplayMethod)) {
      args[i].value = (XtArgVal) displaymethod; continue;
    }
    if (!strcmp(args[i].name, XtNimageData)) {
      found++; i--; continue;
    }
    if (!strcmp(args[i].name, XtNimageRedData)) {
      found++; i--; continue;
    }
    if (!strcmp(args[i].name, XtNimageGreenData)) {
      found++; i--; continue;
    }
    if (!strcmp(args[i].name, XtNimageBlueData)) {
      found++; i--; continue;
    }
    if (!strcmp(args[i].name, XtNimageTitle)) {
      found++; i--; continue;
    }
    if (!strcmp(args[i].name, XtNimagePixtype)) {
      found++; i--; continue;
    }
    if (!strcmp(args[i].name, XtNscaletype)) {
      found++; i--; continue;
    }
    if (!strcmp(args[i].name, XtNscalea)) {
      found++; i--; continue;
    }
    if (!strcmp(args[i].name, XtNscaleb)) {
      found++; i--; continue;
    }
  }
  *num_args = (*num_args) - found;

} /* fix_widget_resources() */

static void VisualsSelect(Widget w, XtPointer client_data, XtPointer call_data)
{
  Widget iw, wid = NULL;
  image_disp_cb_data *cl_data = (image_disp_cb_data *) client_data;
  IMAGE img, img_cpy;
  Visual *visual, *new_visual = NULL;
  int depth, i;
  ArgList args;
  Cardinal num_args;
  display_method displaymethod;
  
  ENTER_FUNCTION_DEBUG("ImageVisualsMenu.c: VisualsSelect");

  iw = PopupWidget(); /* Image widget */

  XtVaGetValues(iw, XtNvisual,        &visual,
		    XtNimageImage,    &img,
		    XtNdisplayMethod, &displaymethod, NULL);

  if (!cl_data) return;

  switch(cl_data->index) {
  case PseudoColor:
    if (ClassOfVisual(visual) != PseudoColor) {
      new_visual = visualFromName("PseudoColor", 0);
      depth      = 8;

      if ((displaymethod & 3) == 0) {
	/* Original is reduced-color display. Keep this also for new. */
	displaymethod = PSEUDO_COLOR_8;
      } else displaymethod = PSEUDO_COLOR_8_B;
    }
    break;
  case DirectColor:
    if (ClassOfVisual(visual) != DirectColor) {
      new_visual = visualFromName("DirectColor", 0);
      depth      = 24;

      if ((displaymethod & 3) == 0) {
	/* Original is reduced-color display. Keep this also for new. */
	displaymethod = DIRECT_COLOR_24;
      } else displaymethod = DIRECT_COLOR_24_B;
    }
    break;
  case TrueColor:
    if (ClassOfVisual(visual) != TrueColor) {
      new_visual    = visualFromName("TrueColor", 0);
      depth         = 24;
      displaymethod = TRUE_COLOR_24;
    }
    break;
  } /* switch */

  if (!new_visual) {
    LEAVE_FUNCTION_DEBUG("ImageVisualsMenu.c: VisualsSelect");
    return;
  }

  /* Make copy of image data, to avoid problems when the old or new image
   * widget is destroyed. (Destruction of a widget also deallocates the
   * BIFF image data.)
   */
  img_cpy = Icopy_init(img);
  for (i = 1; i <= Inbands(img); i++) {
    Icopy_band(img[i], img_cpy[i]);
  }

  /* Prepare to use most of the resources from the original widget in the
   * new widget. */
  get_widget_resources(iw, &args, &num_args);
  fix_widget_resources(args, &num_args, img_cpy, new_visual, depth,
		       displaymethod);

  wid = cl_data->display_func(Control_s, XtName(iw), NULL, img_cpy, 0,
			      XtClass(iw),
			      new_visual, depth, image_menus_e, args, num_args,
			      cl_data->callbacks);

  if (wid) XtPopup(TopLevelWidget(wid), XtGrabNone);

  LEAVE_FUNCTION_DEBUG("ImageVisualsMenu.c: VisualsSelect");

  return;
}

static void VisualsHook(Widget wid, XEvent *event, String *params, Cardinal *num_params)
{
  ImageWidget iw;
  char *label;
  Visual *visual, *visual2;
  Boolean sensitive;

  ENTER_FUNCTION_DEBUG("ImageVisualsMenu.c: VisualsHook");

  if (entries_s[0] == NULL) return;

  iw = (ImageWidget) ImageFromMenu(wid);
  XtVaGetValues((Widget) iw, XtNvisual, &visual, NULL);

  sensitive = visualFromClass(PseudoColor, 0) ? 1 : 0;
  XtVaGetValues((Widget) entries_s[0], XtNlabel, &label, NULL);
  label[0] = MARKIF((ClassOfVisual(visual) == PseudoColor));
  XtVaSetValues((Widget) entries_s[0],
		XtNlabel,     label,
		XtNsensitive, sensitive, NULL);

  sensitive = visualFromClass(DirectColor, 0) ? 1 : 0;
  XtVaGetValues((Widget) entries_s[1], XtNlabel, &label, NULL);
  label[0] = MARKIF((ClassOfVisual(visual) == DirectColor));
  XtVaSetValues((Widget) entries_s[1],
		XtNlabel,     label,
		XtNsensitive, sensitive, NULL);

  visual2   = visualFromClass(TrueColor, 0);
  sensitive = (visual2 && (visual2 == DefaultVisual(dpy_s, screen_s))) ? 1 : 0;
  XtVaGetValues((Widget) entries_s[2], XtNlabel, &label, NULL);
  label[0] = MARKIF((ClassOfVisual(visual) == TrueColor));
  XtVaSetValues((Widget) entries_s[2],
		XtNlabel,     label,
		XtNsensitive, sensitive, NULL);

  LEAVE_FUNCTION_DEBUG("ImageVisualsMenu.c: VisualsHook");
}



void VisualsMenuInit(Display *display, int screenno, Widget parent, ximage_display_func display_func, image_callbacks *callbacks)
{
  image_disp_cb_data *data0, *data1, *data2;
  char *label = NULL;

  ENTER_FUNCTION_DEBUG("ImageVisualsMenu.c: VisualsMenuInit");

  dpy_s = display;
  screen_s = screenno;
  Control_s = parent;
  
  visualsmenu_e = PopupInit("visualsmenu", Control_s, NULL);
  XtVaGetValues(visualsmenu_e, XtNlabel, &label, NULL);
  if (!label) {
    XtDestroyWidget(visualsmenu_e);
    visualsmenu_e = PopupInit("visualsmenu", Control_s, "Visuals menu");
  }

  data0 = XtNew(image_disp_cb_data);
  data0->index        = PseudoColor;
  data0->display_func = display_func;
  data0->callbacks    = callbacks;

  entries_s[0] =
    (SmeBSBObject) Popup_add("pseudocolor", visualsmenu_e,
			    "  PseudoColor 8-plane  ",
			    VisualsSelect, (XtPointer) data0);
  if (!visualFromClass(PseudoColor, 0))
    XtVaSetValues((Widget) entries_s[0], XtNsensitive, 0, NULL);

  data1 = XtNew(image_disp_cb_data);
  data1->index        = DirectColor;
  data1->display_func = display_func;
  data1->callbacks    = callbacks;

  entries_s[1] =
    (SmeBSBObject) Popup_add("directcolor", visualsmenu_e,
			    "  DirectColor 24-plane ",
			    VisualsSelect, (XtPointer) data1);

  if (!visualFromClass(DirectColor, 0))
    XtVaSetValues((Widget) entries_s[1], XtNsensitive, 0, NULL);

  data2 = XtNew(image_disp_cb_data);
  data2->index        = TrueColor;
  data2->display_func = display_func;
  data2->callbacks    = callbacks;

  entries_s[2] =
    (SmeBSBObject) Popup_add("truecolor",   visualsmenu_e,
			    "  TrueColor 24-plane   ", 
			    VisualsSelect, (XtPointer) data2);

  if (!visualFromClass(TrueColor, 0))
    XtVaSetValues((Widget) entries_s[2], XtNsensitive, 0, NULL);

  AddAction(parent, "VisualsHook", VisualsHook, 0); 

  LEAVE_FUNCTION_DEBUG("ImageVisualsMenu.c: VisualsMenuInit");
}
