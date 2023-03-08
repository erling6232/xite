
/*
  This header-file is produced automatically (at least in part) by
  makeHeader of BLAB, Ifi, UiO.
*/


/*

________________________________________________________________

        Xdialog.h
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

#ifndef _DIALOG_H_
#define _DIALOG_H_

_XITE_CPLUSPLUS_BEGIN

#include <X11/Intrinsic.h>

/* description of a button */
struct DialogArg {
    char *buttonName;
/* #ifndef FUNCPROTO */
/*    void (*handler) */ /* Widget, XtPointer, XtPointer */
/* #else */ /* FUNCPROTO */ 
/*    void (*handler)( Widget, caddr_t, caddr_t ); */
/* #endif */ /* FUNCPROTO */

    XtCallbackProc handler;
    XtPointer data;
};

#define XTERMOUT        "<xterm>"
#define XSHOWINFILE     "<infile>"
#define XSHOWOUTFILE    "<outfile>"
#define readBIFF        "?readBIFFfile"
#define readColortab    "?readColortabfile"
#define readColormap    "?readColormapfile"
#define readTIFF        "?readTIFFfile"
#define readMAT         "?readMATfile"
#define readPS          "?readPSfile"
#define readfile        "?readfile"
#define writeBIFF       "?writeBIFFfile"
#define writeColortab   "?writeColortabfile"
#define writeColormap   "?writeColormapfile"
#define writeTIFF       "?writeTIFFfile"
#define writeMAT        "?writeMATfile"
#define writePS         "?writePSfile"
#define writefile       "?writefile"
#define writeMacro      "?writeMacrofile"
#define DIALOG          "?dialog:"

extern void xthCenterWidget  _XITE_PARAMS(( Widget widget, int x, int y ));
extern void xthCenterWidgetOverCursor  _XITE_PARAMS(( Widget widget ));
extern Widget CreateDialog  _XITE_PARAMS(( Widget parent, char *title, char *textField, char *winName, char *iconName, struct DialogArg *args, unsigned int count ));
extern void FocusPopUp  _XITE_PARAMS(( Widget popup, XtPointer data, XEvent *event, Boolean* dummy ));
extern void PopUpDialog  _XITE_PARAMS(( Widget popup ));
extern void PopUpDialogNoGrab  _XITE_PARAMS(( Widget popup ));
extern void PopDownDialog  _XITE_PARAMS(( Widget dialog ));
extern void cbPopDownDialog  _XITE_PARAMS(( Widget dialog, XtPointer client_data, XtPointer call_data ));
extern char * GetDialogValue  _XITE_PARAMS(( Widget popup ));
extern int InformationBox  _XITE_PARAMS(( Widget parent, char *message, char *winName, char *iconName ));
extern Widget TextBox  _XITE_PARAMS(( Widget parent, char *message, char *bnt_label, char *winName, char *iconName ));
extern int ConfirmationBox  _XITE_PARAMS(( Widget parent, char *message, char *buttonname1, char *buttonname2, char *winName, char *iconName ));
extern int SimpleDialog  _XITE_PARAMS(( Widget parent, char * title, char *deftext, char **return_text, char *buttonname1, char *buttonname2, char *winName, char *iconName ));
extern int SelectReadFile _XITE_PARAMS(( Widget parent, char *kind, char **return_text ));
extern int SelectWriteFile _XITE_PARAMS(( Widget parent, char *kind, char **return_text ));

_XITE_CPLUSPLUS_END

#endif /* _DIALOG_H_ */
