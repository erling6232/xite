
/*

________________________________________________________________

        ximage_display.h
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

#ifndef _ximage_display_H_
#define _ximage_display_H_

_XITE_CPLUSPLUS_BEGIN

#include <X11/Intrinsic.h>
#include <xite/biff.h>
#include <xite/XmenuBar.h>
#include <xite/callbacks.h>

/****************** for image display function ******************/


typedef Widget (*ximage_display_func) _XITE_PARAMS(( Widget parent, char *resource_name, char *shell_resource_name, IMAGE img, int bandno, WidgetClass widgetClass, Visual *visual, int depth, menu_button buttons, ArgList args, Cardinal num_args, image_callbacks *callbacks ));

extern Widget ximage_display _XITE_PARAMS(( Widget parent, char *resource_name, char *shell_resource_name, IMAGE img, int bandno, WidgetClass widgetClass, Visual *visual, int depth, menu_button buttons, ArgList args, Cardinal num_args, image_callbacks *callbacks ));
extern void InitImageMenus _XITE_PARAMS(( char *colorlistfile, char *colortabfile, char *ovltabfile, int *coltabInUse, int *ovltabInUse, menu_button menus, image_callbacks *callbacks ));

_XITE_CPLUSPLUS_END

#endif /* _ximage_display_H_ */

