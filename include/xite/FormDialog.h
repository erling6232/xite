
/*

________________________________________________________________

        FormDialog.h
        $Id$
        Copyright 1996, Blab, UiO
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

#ifndef _FORM_DIALOG_H_
#define _FORM_DIALOG_H_

_XITE_CPLUSPLUS_BEGIN

#include <X11/Intrinsic.h>

typedef struct _button_element {
  char *name, *label;
  XtCallbackProc callback;
  XtPointer closure;
} button_element, *button_table;


extern int FormDialog _XITE_PARAMS(( Widget parent, char *header, char *dialog_spec, char **return_text, button_table buttons, int nbuttons, char *winName, char *iconName ));
extern button_table init_button_table _XITE_PARAMS(( button_table buttons, int length ));

_XITE_CPLUSPLUS_END

#endif /* _FORM_DIALOG_H_ */
