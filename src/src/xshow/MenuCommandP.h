

/*C*

________________________________________________________________

        MenuCommandP.h
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

/*
* $XConsortium: CommandP.h,v 1.30 90/12/01 13:00:10 rws Exp $
*/


/***********************************************************
Copyright 1987, 1988 by Digital Equipment Corporation, Maynard, Massachusetts,
and the Massachusetts Institute of Technology, Cambridge, Massachusetts.

                        All Rights Reserved

Permission to use, copy, modify, and distribute this software and its 
documentation for any purpose and without fee is hereby granted, 
provided that the above copyright notice appear in all copies and that
both that copyright notice and this permission notice appear in 
supporting documentation, and that the names of Digital or MIT not be
used in advertising or publicity pertaining to distribution of the
software without specific, written prior permission.  

DIGITAL DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE, INCLUDING
ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS, IN NO EVENT SHALL
DIGITAL BE LIABLE FOR ANY SPECIAL, INDIRECT OR CONSEQUENTIAL DAMAGES OR
ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS,
WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION,
ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS
SOFTWARE.

******************************************************************/

/* 
 * MenuCommandP.h - Private definitions for Command widget
 * 
 */

#ifndef _MENUCOMMANDP_H_
#define _MENUCOMMANDP_H_

_XITE_CPLUSPLUS_BEGIN

#include <xite/includes.h>
#include "MenuCommand.h"
#include <X11/Xaw/CommandP.h>

/***********************************************************************
 *
 * Command Widget Private Data
 *
 ***********************************************************************/

/************************************
 *
 *  Class structure
 *
 ***********************************/


   /* New fields for the MenuCommand widget class record */
typedef struct _MenuCommandClass 
  {
    int makes_compiler_happy;  /* not used */
  } MenuCommandClassPart;

   /* Full class record declaration */
typedef struct _MenuCommandClassRec {
    CoreClassPart	core_class;
    SimpleClassPart	simple_class;
    LabelClassPart	label_class;
    CommandClassPart    command_class;
    MenuCommandClassPart menuCommand_class;
} MenuCommandClassRec;

extern MenuCommandClassRec menuCommandClassRec;

/***************************************
 *
 *  Instance (widget) structure 
 *
 **************************************/

    /* New fields for the MenuCommand widget record */
typedef struct {
    int makes_compiler_happy;  /* not used */
} MenuCommandPart;

   /* Full widget declaration */
typedef struct _MenuCommandRec {
    CorePart         core;
    SimplePart	     simple;
    LabelPart	     label;
    CommandPart      command;
    MenuCommandPart menuCommand;
} MenuCommandRec;

_XITE_CPLUSPLUS_END

#endif /* _MENUCOMMANDP_H_ */


