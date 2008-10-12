

/*C*

________________________________________________________________

        readarg.h
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

#ifndef _READARG_H_
#define _READARG_H_ 1

_XITE_CPLUSPLUS_BEGIN

extern _XITE_WIN_DLL char *argvOptions  _XITE_PARAMS(( int argc, char **argv ));
extern _XITE_WIN_DLL char *read_switch  _XITE_PARAMS(( int *argc, char **argv, char *name, int args, char *defreturn ));
extern _XITE_WIN_DLL int read_bswitch  _XITE_PARAMS(( int *argc, char **argv, char *name ));
extern _XITE_WIN_DLL int read_iswitch  _XITE_PARAMS(( int *argc, char **argv, char *name, int defval ));
extern _XITE_WIN_DLL double read_dswitch  _XITE_PARAMS(( int *argc, char **argv, char *name, double defval ));
extern _XITE_WIN_DLL void split_cmd_line _XITE_PARAMS(( char *cmd_line, char **vec ));

_XITE_CPLUSPLUS_END

#endif /* _READARG_H_ */

