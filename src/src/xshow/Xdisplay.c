/*

________________________________________________________________

        Xdisplay.c
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

static char *Id = "$Id$, Otto Milvang, Blab, UiO";



#include <xite/includes.h>
#include <X11/Xos.h>
#include <X11/Intrinsic.h>
#include <X11/StringDefs.h>
#include <ctype.h>
#include <xite/biff.h>
#include "Xdisplay.h"
#include <X11/Shell.h>
#include <X11/Xutil.h>
#include <xite/ImageOverlay.h>
#include <xite/arithmetic.h>   /* For logical_xor() */
#include "Xcontrol.h"
#include "Xprog.h"
#include <xite/ShellWids.h>
#include <xite/XmenuBar.h>
#include <xite/ximage.h>
#include <xite/logPos.h>
#include "Macro.h"
#include XITE_STDIO_H
#include <xite/debug.h>

/* External global variables. */

extern Widget Control_e;
extern macroEntryPtr current_macro_e;
extern menu_button image_menus_e;

/* Internal global variables. */
static Widget Image_s, Pos_s;
static IBAND lastband_s = NULL;



#ifndef FUNCPROTO
static void Button_prog(wid, client_data, btn)
Widget wid;
XtPointer client_data, btn;
#else /* FUNCPROTO */
static void Button_prog(Widget wid, XtPointer client_data, XtPointer btn)
#endif /* FUNCPROTO */
{
  ImageCallback button = (ImageCallback) btn;

  if (button->event && button->event->xbutton.type == ButtonRelease) {
    Prog_button(wid, (IMAGE) button-> img, (IBAND) button->band,
		button-> event->xbutton.button, 0, 0, 0, 0);
  }

  return;

} /* Button_prog() */

#ifndef FUNCPROTO
static void Roi(wid, client_data, btn)
Widget wid;
XtPointer client_data, btn;
#else /* FUNCPROTO */
static void Roi(Widget wid, XtPointer client_data, XtPointer btn)
#endif /* FUNCPROTO */
{
  int w, h;
  ImageOverlayCallback button = (ImageOverlayCallback) btn;

  if (button->value != ROI_STOPDRAG) return;

  Prog_button(wid, 
	      (IMAGE) button-> img,
	      (IBAND) button->band,
	      1,
	      button-> x - Ixstart((IBAND) button->band) + 1,
	      button-> y - Iystart((IBAND) button->band) + 1,
	      button-> width,
	      button-> height);
  XtVaGetValues(wid, XtNimageWidth, &w, XtNimageHeight, &h, NULL);
  Message_size(w, h);

  return;

} /* Roi() */

#ifndef FUNCPROTO
static void MRoi(wid, client_data, btn)
Widget wid;
XtPointer client_data, btn;
#else /* FUNCPROTO */
static void MRoi(Widget wid, XtPointer client_data, XtPointer btn)
#endif /* FUNCPROTO */
{
  ImageOverlayCallback button = (ImageOverlayCallback) btn;

  if (button->value == ROI_STOPDRAG) return;

  Message_roi(button-> x,  button-> y, 
	      button-> width, button-> height);

  return;

} /* MRoi() */

#ifndef FUNCPROTO
static void decrease_image_num(wid, client_data, call_data)
Widget wid;
XtPointer client_data, call_data;
#else /* FUNCPROTO */
static void decrease_image_num(Widget wid, XtPointer client_data, XtPointer call_data)
#endif /* FUNCPROTO */
{

  Message_images(-1);

} /* decrease_image_num() */



#ifndef FUNCPROTO
int Add_overlay(img)
IMAGE img;
#else /* FUNCPROTO */
int Add_overlay(IMAGE img)
#endif /* FUNCPROTO */
{
  IBAND band;

  ENTER_FUNCTION_DEBUG("Xdisplay.c: Add_overlay");

  if (lastband_s == NULL) return(1);

  if (img == NULL) {
    band = Imake_band(3, Ixsize(lastband_s), Iysize(lastband_s));
    logical_xor(band, band, band);
  } else band = img[1];

  if (Ixsize(band) != Ixsize(lastband_s) || Iysize(band) != Iysize(lastband_s))
    return(2);

  XtVaSetValues(Image_s, XtNoverlay, band, NULL);
  ImageOverlayRedisplay(Image_s, UPDATE_NONE);

  LEAVE_FUNCTION_DEBUG("Xdisplay.c: Add_overlay");
  return(0);

} /* Add_overlay() */

#ifndef FUNCPROTO
int Display_picture(img, visual, depth)
IMAGE img;
Visual *visual;
int depth;
#else /* FUNCPROTO */
int Display_picture(IMAGE img, Visual *visual, int depth)
#endif /* FUNCPROTO */
{
  Widget imageShell;
  IBAND rband = NULL;
  int bandno;
  char name[80];
  image_callbacks *callbacks = NULL;

  ENTER_FUNCTION_DEBUG("Xdisplay.c: Display_picture");

  if (!Pos_s)
    Pos_s = XtNameToWidget(Control_e, strcat(strcpy(name, "*"), "dataLabel"));

  callbacks = add_to_image_callbacks(callbacks, XtNcallback,
				     LogPositionCallback, (XtPointer) Pos_s,
				     True);
  (void) add_to_image_callbacks(callbacks, XtNcallback,
				Button_prog, NULL, False);
  (void) add_to_image_callbacks(callbacks, XtNdestroyCallback,
                                decrease_image_num, NULL, True);
  (void) add_to_image_callbacks(callbacks, XtNroiProc,
				Roi, NULL, True);
  (void) add_to_image_callbacks(callbacks, XtNroiProc,
				MRoi, NULL, False);

  for (bandno = 1; bandno <= Inbands(img); bandno++) {

    Image_s = ximage_display(Control_e, NULL, NULL, img, bandno,
			     imageOverlayWidgetClass, visual, depth,
			     image_menus_e, NULL, 0, callbacks);

    lastband_s = img[bandno];
    imageShell = TopLevelWidget(Image_s);

    XtVaGetValues(Image_s, XtNimageRedBand, &rband, NULL);

    Add_macro_widget(current_macro_e, imageShell);
    Message_images(1);

    XtPopup(imageShell, XtGrabNone);

    if (rband) break;
  } /* for each band (or after three bands if rgb_e==1 and Inbands(img)==3. */

  if (rband) bandno = 1;
  else bandno = Inbands(img);

  LEAVE_FUNCTION_DEBUG("Xdisplay.c: Display_picture");

  return(bandno);

} /* Display_picture() */
