
/*
  This header-file is produced automatically (at least in part) by
  makeHeader of BLAB, Ifi, UiO.
*/


/*C*

________________________________________________________________

        Macro.h
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

#ifndef _MACRO_H_
#define _MACRO_H_

_XITE_CPLUSPLUS_BEGIN

typedef struct _macroEntry {
  /* For each complete program command-line. */
  
  char               *entry_name;
  struct _macroEntry *next_entry;
  struct _macroItem  *first_item;
} macroEntry, *macroEntryPtr;

typedef struct _macroItem {
  /* For each <infile> and <outfile> in a program command line. */
  char              *item;
  int               type;
  Widget            wid;
  struct _macroItem *mate;
  struct _macroItem *next_item;
} macroItem, *macroItemPtr;

typedef struct {
  Widget wid;
  Display *dpy;
  Window screen;
} log_macro_data;

#include <X11/Intrinsic.h>


extern void Add_macro_entry  _XITE_PARAMS(( macroEntryPtr entry ));
extern void Add_macro_item  _XITE_PARAMS(( macroEntryPtr entry, macroItemPtr item ));
extern void Add_macro_widget  _XITE_PARAMS(( macroEntryPtr entry, Widget wid ));
extern void FreeMacro  _XITE_PARAMS(( void ));
extern void InitMacroEntry  _XITE_PARAMS(( macroEntryPtr *macro_entry ));
extern void InitMacroItem  _XITE_PARAMS(( macroItemPtr *macro_entry ));
extern void LogMacro  _XITE_PARAMS(( Widget wid, XtPointer client_data, XtPointer call_data ));
extern void Remove_current_entry  _XITE_PARAMS(( void ));

_XITE_CPLUSPLUS_END

#endif /* _MACRO_H_ */
