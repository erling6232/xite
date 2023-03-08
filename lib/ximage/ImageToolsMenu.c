
/*

________________________________________________________________

        ImageToolsMenu.c
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
#include <X11/StringDefs.h>
#include <xite/ImageToolsMenu.h>
#include <xite/ShellWids.h>
#include <xite/ximage.h>
#include <xite/debug.h>

/* External global variables. */

extern Widget toolsmenu_e;

typedef enum {
  INFO,
  HISTOGRAM,
  SLICE,
  COLORBAR
} tools_entry;



static void ToolsSelect(Widget wid, XtPointer client_data, XtPointer call_data)
{
  image_disp_cb_data *cl_data = (image_disp_cb_data *) client_data;
  
  ENTER_FUNCTION_DEBUG("ImageToolsMenu.c: ToolsSelect");

  if (!cl_data) return;

  switch(cl_data->index) {
  case INFO: ximage_info(NULL);
    break;
  case HISTOGRAM: ximage_histogram(NULL, cl_data);
    break;
  case SLICE: ximage_slice(NULL);
    break;
  case COLORBAR: ximage_colorbar(NULL, cl_data);
    break;
  } /* switch */

  LEAVE_FUNCTION_DEBUG("ImageToolsMenu.c: ToolsSelect");

  return;
}

void ToolsMenuInit(Display *display, int screenno, Widget parent, ximage_display_func display_func, image_callbacks *callbacks)
{
  image_disp_cb_data *data0, *data1, *data2, *data3;
  char *label = NULL;

  ENTER_FUNCTION_DEBUG("ImageToolsMenu.c: ToolsMenuInit");

  toolsmenu_e = PopupInit("toolsmenu", parent, NULL); 
  XtVaGetValues(toolsmenu_e, XtNlabel, &label, NULL);
  if (!label) {
    XtDestroyWidget(toolsmenu_e);
    toolsmenu_e = PopupInit("toolsmenu", parent, "Tools menu"); 
  }

  data0 = XtNew(image_disp_cb_data);
  data0->index        = INFO;
  data0->display_func = display_func;
  data0->callbacks    = callbacks;
  Popup_add("imageinfo", toolsmenu_e, "  Image info  ",
	    ToolsSelect, (XtPointer) data0);

  data1 = XtNew(image_disp_cb_data);
  data1->index        = HISTOGRAM;
  data1->display_func = display_func;
  data1->callbacks    = callbacks;
  Popup_add("histogram", toolsmenu_e, "  Histogram   ",
	    ToolsSelect, (XtPointer) data1);

  data2 = XtNew(image_disp_cb_data);
  data2->index        = SLICE;
  data2->display_func = display_func;
  data2->callbacks    = callbacks;
  Popup_add("slice",     toolsmenu_e, "  Slice       ",
	    ToolsSelect, (XtPointer) data2);

  data3 = XtNew(image_disp_cb_data);
  data3->index        = COLORBAR;
  data3->display_func = display_func;
  data3->callbacks    = callbacks;
  Popup_add("colorbar",  toolsmenu_e, "  Colorbar    ",
	    ToolsSelect, (XtPointer) data3);

  LEAVE_FUNCTION_DEBUG("ImageToolsMenu.c: ToolsMenuInit");

  return;
}
