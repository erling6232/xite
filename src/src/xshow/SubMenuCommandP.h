

/*C*

________________________________________________________________

        SubMenuCommandP.h
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
 * SubMenuCommandP.h - Private definitions for Command widget
 * 
 */

#ifndef _SUBMENUCOMMANDP_H_
#define _SUBMENUCOMMANDP_H_

_XITE_CPLUSPLUS_BEGIN

#include <xite/includes.h>
#include "SubMenuCommand.h"
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


   /* New fields for the SubMenuCommand widget class record */
typedef struct _SubMenuCommandClass 
  {
    int makes_compiler_happy;  /* not used */
  } SubMenuCommandClassPart;

   /* Full class record declaration */
typedef struct _SubMenuCommandClassRec {
    CoreClassPart	core_class;
    SimpleClassPart	simple_class;
    LabelClassPart	label_class;
    CommandClassPart    command_class;
    SubMenuCommandClassPart subMenuCommand_class;
} SubMenuCommandClassRec;

extern SubMenuCommandClassRec subMenuCommandClassRec;

/***************************************
 *
 *  Instance (widget) structure 
 *
 **************************************/

    /* New fields for the SubMenuCommand widget record */
typedef struct {
    int makes_compiler_happy;  /* not used */
} SubMenuCommandPart;

   /* Full widget declaration */
typedef struct _SubMenuCommandRec {
    CorePart         core;
    SimplePart	     simple;
    LabelPart	     label;
    CommandPart      command;
    SubMenuCommandPart subMenuCommand;
} SubMenuCommandRec;

_XITE_CPLUSPLUS_END

#endif /* _SUBMENUCOMMANDP_H_ */


