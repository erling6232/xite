
/*
  This header-file is produced automatically (at least in part) by
  makeHeader of BLAB, Ifi, UiO.
*/


/*

________________________________________________________________

        ShellWids.h
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

#ifndef _ShellWids_H
#define _ShellWids_H

_XITE_CPLUSPLUS_BEGIN

#include <X11/Intrinsic.h>


extern void InitializeVisual  _XITE_PARAMS(( Display *dpy, int screen, char *imageVisualClass ));
extern void RequestedImageVisualAndColormap  _XITE_PARAMS(( Display *dpy, int screen, int *planes, Visual **visual, Colormap *colormap ));
extern Widget CreateAppShell  _XITE_PARAMS(( Display *dpy, char *app_name, char *app_class ));
extern Widget CreateTopLevelShell  _XITE_PARAMS(( Display *dpy, char *name ));
extern Widget CreatePopupShell  _XITE_PARAMS(( char *name, Widget parent ));
extern Widget CreatePopOverShell _XITE_PARAMS(( char *name, Widget parent ));
extern Widget CreatePopTopShell _XITE_PARAMS(( char *name, Widget parent ));
extern void AddAction  _XITE_PARAMS(( Widget parent, char *name, XtActionProc func, int popup ));
extern Widget TopLevelWidget  _XITE_PARAMS(( Widget wid ));
extern void PopupMenubar  _XITE_PARAMS(( Widget wid ));
extern void PopupExit  _XITE_PARAMS(( Widget wid, XEvent *event, String *params, Cardinal *num_params ));
extern Widget PopupInit  _XITE_PARAMS(( char *name, Widget parent, char *label ));
extern Widget PopupAdd  _XITE_PARAMS(( char *name, Widget parent, char *label, XtCallbackProc callback_func, int arg ));
extern Widget Popup_add  _XITE_PARAMS(( char *name, Widget parent, char *label, XtCallbackProc callback_func, XtPointer arg ));
extern Widget PopupLine  _XITE_PARAMS(( char *name, Widget parent ));
extern void PopupMenu  _XITE_PARAMS(( Widget wid, XEvent *event, String *params, Cardinal *num_params ));
extern Widget PopupWidget  _XITE_PARAMS(( void ));
extern Widget ImageFromMenu _XITE_PARAMS(( Widget wid ));

_XITE_CPLUSPLUS_END

#endif /* _ShellWids_H */
