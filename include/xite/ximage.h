
/*
  This header-file is produced automatically (at least in part) by
  makeHeader of BLAB, Ifi, UiO.
*/


/*

________________________________________________________________

        ximage.h
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

#ifndef _ximage_H_
#define _ximage_H_

_XITE_CPLUSPLUS_BEGIN

#include <xite/biff.h>
#include <xite/layout.h>
#include <xite/callbacks.h>
#include <xite/ximage_display.h>

/****************** for image display function ******************/

typedef struct {
  int index;
  ximage_display_func display_func;
  image_callbacks *callbacks;
} image_disp_cb_data;

typedef struct {
  IMAGE img;
  image_callbacks *callbacks;
} image_data;

typedef enum {
  UNKNOWN_CONTAINER,
  FORM_CONTAINER,
  BOX_CONTAINER
} type_container;


extern Widget ximageInit  _XITE_PARAMS(( int *argc, char **argv, char *classname, XtResource *resources, int num_resources, XtPointer resources_result, XrmOptionDescRec *optionsTable, int num_options, String fallback_resources[], int warn ));
extern Widget ximageLayout _XITE_PARAMS(( Widget app_parent, layout *layouts, int nwids ));
extern int ximage_histogram _XITE_PARAMS(( Widget wid, image_disp_cb_data *cb_data ));
extern int ximage_slice  _XITE_PARAMS(( Widget wid ));
extern int ximage_info  _XITE_PARAMS(( Widget wid ));
extern int ximage_colorbar _XITE_PARAMS(( Widget wid, image_disp_cb_data *cb_data ));
extern void ximage_mainloop  _XITE_PARAMS(( void ));
extern void ximage_exit  _XITE_PARAMS(( Widget wid, XtPointer str, XtPointer dummy ));
extern Widget ximage_init _XITE_PARAMS(( int *argc, char **argv, char *classname, String *fallback_resources, int warn ));
extern Widget ximage_layout  _XITE_PARAMS(( Widget app_parent, IBAND band, char **wids, Widget rwids[], int nwids, XtCallbackProc ximage_work_fun, char *res_inst_name ));
extern XtResource *ximage_app_resources  _XITE_PARAMS(( XtResource *resources, int *num_resources ));
extern XrmOptionDescRec *ximage_app_options  _XITE_PARAMS(( XrmOptionDescRec *options, int *num_options ));
extern char *ximage_app_usage_text _XITE_PARAMS(( char *app_usage ));
extern char *ximage_usage_text _XITE_PARAMS(( void ));
extern void set_init_color _XITE_PARAMS(( int color ));
extern void set_init_ovl_color _XITE_PARAMS(( int color ));
extern void xite_usleep _XITE_PARAMS(( unsigned long n ));

_XITE_CPLUSPLUS_END

#endif /* _ximage_H_ */

