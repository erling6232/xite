
/*

________________________________________________________________

        callbacks.h
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

#ifndef _callbacks_H_
#define _callbacks_H_

_XITE_CPLUSPLUS_BEGIN

#include <X11/Intrinsic.h>

typedef struct {
  XtCallbackList destroy_cb, callback_cb, resize_cb, draw_cb, roi_cb;
} image_callbacks;


extern image_callbacks *init_image_callbacks _XITE_PARAMS(( void ));
extern XtCallbackList add_callback_first _XITE_PARAMS(( XtCallbackList callback_list, XtCallbackProc new_callback, XtPointer closure ));
extern XtCallbackList add_callback_last _XITE_PARAMS(( XtCallbackList callback_list, XtCallbackProc new_callback, XtPointer closure ));
extern image_callbacks *add_to_image_callbacks _XITE_PARAMS(( image_callbacks *image_cbs, String resource_name_list, XtCallbackProc callback, XtPointer closure, Boolean first ));

_XITE_CPLUSPLUS_END

#endif /* _callbacks_H_ */

