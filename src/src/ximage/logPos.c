

/*C*

________________________________________________________________

        logPos
        $Id$
        Copyright 1994, Blab, UiO
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
#include <X11/Xos.h>
#include <X11/Intrinsic.h>
#include <X11/StringDefs.h>
#include <X11/Xaw/Dialog.h>
#include <xite/Image.h>
#include <xite/ShellWids.h>
#include <xite/logPos.h>
#include XITE_STDIO_H
#include XITE_STRING_H

/* ---- Switch dependent external variables ---- */

int logpos_e = 0;

#ifndef FUNCPROTO
static char *position_text(x, y, value)
int x, y;
char  *value;
#else /* FUNCPROTO */
static char *position_text(int x, int y, char *value)
#endif /* FUNCPROTO */
{
  static char mess[80];
  static int oldx_s = -1, oldval_s = -1;

  if (value[0] == 0 && oldval_s == 0 && x == oldx_s) return(NULL);

  oldval_s = value[0];
  oldx_s   = x;

  if (oldval_s > 0) sprintf(mess, "x:%3d, y:%3d, f(x,y):%s", x, y, value);
  else if (x == 0) mess[0] = '\0';
  else sprintf(mess, "Width:%d, Height:%d", x, y);

  return(mess);

} /* position_text() */

#ifndef FUNCPROTO
static void logPosition(wid, button)
Widget wid;
ImageCallback button;
#else /* FUNCPROTO */
static void logPosition(Widget wid, ImageCallback button)
#endif /* FUNCPROTO */
{
  char buf[80], *mess;
  Dimension width;
  Boolean dialog;

  /* 'wid' should be a label widget and must have a parent. */

  dialog = XtIsSubclass(XtParent(wid), dialogWidgetClass) ? True : False;

  if (logpos_e) {
    mess = position_text(button->x, button->y,
		    (button->pixtype && button->data) ? 
		    (button->pixtype)->printf(buf, button->data) : "?");

    XtVaGetValues(wid, XtNwidth, &width, NULL);
    XtVaSetValues(wid, XtNlabel, mess, XtNwidth, width, NULL);

    if (dialog) XtPopup(TopLevelWidget(wid), XtGrabNone);
  } else if (dialog) XtPopdown(TopLevelWidget(wid));

  return;

} /* logPosition() */

#ifndef FUNCPROTO
void LogPositionCallback(wid, client_data, call_data)
Widget wid;
XtPointer client_data, call_data;
#else /* FUNCPROTO */
void LogPositionCallback(Widget wid, XtPointer client_data, XtPointer call_data)
#endif /* FUNCPROTO */
{

  if (call_data != NULL) logPosition(client_data, call_data);

} /* LogPositionCallback() */

#ifndef FUNCPROTO
void LogPosition(image_wid, label_wid)
Widget image_wid, label_wid;
#else /* FUNCPROTO */
void LogPosition(Widget image_wid, Widget label_wid)
#endif /* FUNCPROTO */
{

  XtAddCallback(image_wid, XtNcallback, LogPositionCallback,
		(XtPointer) label_wid);

} /* LogPosition() */
