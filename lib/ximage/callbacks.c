

/*C*

________________________________________________________________

        callbacks
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
#include <xite/ImageOverlay.h>
#include XITE_STRING_H
#include XITE_MALLOC_H
#include <xite/callbacks.h>

XtCallbackList add_callback_first(XtCallbackList callback_list, XtCallbackProc new_callback, XtPointer closure)
{
  int num_cbs, i;
  Boolean new = False;
  XtCallbackList new_list, tmp_list = NULL;

  if (!callback_list) {
    new = True;
    tmp_list = XtNew(XtCallbackRec);
    tmp_list[0].callback = NULL;
    tmp_list[0].closure  = NULL;
  } else tmp_list = callback_list;

  for (num_cbs = 0; ; num_cbs++)
    if (tmp_list[num_cbs].callback == NULL) break;

  num_cbs++;

  new_list = (XtCallbackList) malloc((num_cbs + 1) * sizeof(XtCallbackRec));

  new_list[0].callback = new_callback;
  new_list[0].closure  = closure;

  for (i = 0; i < num_cbs; i++)
    new_list[i+1] = tmp_list[i];

  if (new == True) {
    XtFree((char *) tmp_list);
    tmp_list = NULL;
  }

  return(new_list);

} /* add_callback_first() */

XtCallbackList add_callback_last(XtCallbackList callback_list, XtCallbackProc new_callback, XtPointer closure)
{
  int num_cbs, i;
  Boolean new = False;
  XtCallbackList new_list, tmp_list = NULL;

  if (!callback_list) {
    new = True;
    tmp_list = XtNew(XtCallbackRec);
    tmp_list[0].callback = NULL;
    tmp_list[0].closure  = NULL;
  } else tmp_list = callback_list;

  for (num_cbs = 0; ; num_cbs++)
    if (tmp_list[num_cbs].callback == NULL) break;

  num_cbs++;

  new_list = (XtCallbackList) malloc((num_cbs + 1) * sizeof(XtCallbackRec));

  for (i = 0; i < num_cbs - 1; i++)
    new_list[i] = tmp_list[i];

  new_list[i].callback = new_callback;
  new_list[i].closure  = closure;

  i++;
  new_list[i].callback = NULL;
  new_list[i].closure  = NULL;

  if (new == True) {
    XtFree((char *) tmp_list);
    tmp_list = NULL;
  }

  return(new_list);

} /* add_callback_last() */

image_callbacks *init_image_callbacks(void)
{
  image_callbacks *cb;

  cb = XtNew(image_callbacks);

  cb->destroy_cb  = NULL;
  cb->callback_cb = NULL;
  cb->resize_cb   = NULL;
  cb->draw_cb     = NULL;
  cb->roi_cb      = NULL;

  return(cb);

} /* init_image_callback() */

image_callbacks *add_to_image_callbacks(image_callbacks *image_cbs, String resource_name, XtCallbackProc callback, XtPointer closure, Boolean first)
{
  XtCallbackList (*func)(XtCallbackList callback_list, XtCallbackProc new_callback, XtPointer closure);

  if (!image_cbs) image_cbs = init_image_callbacks();

  if (first) func = add_callback_first;
  else func = add_callback_last;

  if      (!strcmp(resource_name, XtNcallback))
    image_cbs->callback_cb =
      func(image_cbs->callback_cb, callback, closure);

  else if (!strcmp(resource_name, XtNdestroyCallback))
    image_cbs->destroy_cb  =
      func(image_cbs->destroy_cb, callback, closure);

  else if (!strcmp(resource_name, XtNresizeCallback))
    image_cbs->resize_cb   =
      func(image_cbs->resize_cb, callback, closure);

  else if (!strcmp(resource_name, XtNdrawProc))
    image_cbs->draw_cb     =
      func(image_cbs->draw_cb, callback, closure);

  else if (!strcmp(resource_name, XtNroiProc))
    image_cbs->roi_cb      =
      func(image_cbs->roi_cb, callback, closure);
  
  return(image_cbs);

} /* add_to_image_callbacks() */
