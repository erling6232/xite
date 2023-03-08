
/*
  This header-file is produced automatically (at least in part) by
  makeHeader of BLAB, Ifi, UiO.
*/


/*C*

________________________________________________________________

        Xcontrol.h
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

#ifndef _XCONTROL_H_
#define _XCONTROL_H_

_XITE_CPLUSPLUS_BEGIN

#include <X11/Intrinsic.h>


extern void HelpXshow _XITE_PARAMS((Widget wid, XtPointer c_data, XtPointer call_data));
extern void Init_control  _XITE_PARAMS(( void ));
extern void Message_images  _XITE_PARAMS(( int delta ));
extern void Message_jobs  _XITE_PARAMS(( int delta ));
extern void Message_mouse  _XITE_PARAMS(( char *state ));
extern void Message_name  _XITE_PARAMS(( char *name ));
extern void Message_size  _XITE_PARAMS(( int x, int y ));
extern void Message_roi  _XITE_PARAMS(( int x, int y, int w, int h ));
extern void Message_zoom  _XITE_PARAMS(( double f, double zoom ));
extern int  MessageToWidget  _XITE_PARAMS(( int id, char *fmt ));
extern int  MessageToStdout  _XITE_PARAMS(( int id, char *fmt ));

_XITE_CPLUSPLUS_END

#endif /* _XCONTROL_H_ */
