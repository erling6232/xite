
/*
  This header-file is produced automatically (at least in part) by
  makeHeader of BLAB, Ifi, UiO.
*/


/*

________________________________________________________________

        compu4.h
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

#ifndef _COMPU4_H_
#define _COMPU4_H_

_XITE_CPLUSPLUS_BEGIN

typedef long INTEG;
typedef struct u_tag
{
      INTEG i_value;
      double d_value;
      char *p_value;
      float f_value;
      int u_type;
} number;

typedef int (*func) _XITE_PARAMS((number *));




extern _XITE_WIN_DLL int compute_line  _XITE_PARAMS(( char *inp_line, ... ));
extern _XITE_WIN_DLL number *compute_answer  _XITE_PARAMS(( void ));
extern _XITE_WIN_DLL int compute_define  _XITE_PARAMS(( char *name, func action ));
extern _XITE_WIN_DLL char *compute_error  _XITE_PARAMS(( int message ));

_XITE_CPLUSPLUS_END

#endif /* _COMPU4_H_ */
