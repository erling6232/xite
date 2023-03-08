
/*
  This header-file is produced automatically (at least in part) by
  makeHeader of BLAB, Ifi, UiO.
*/


/*

________________________________________________________________

        message.h
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

#ifndef _MESSAGE_H_
#define _MESSAGE_H_

_XITE_CPLUSPLUS_BEGIN

#include <xite/includes.h>
#include XITE_STDIO_H

typedef int (*messagefunc) _XITE_PARAMS((int, char *));

extern _XITE_WIN_DLL int InitMessage  _XITE_PARAMS(( int *argc, char *argv[], char *usage ));
extern _XITE_WIN_DLL int PushMessage  _XITE_PARAMS(( messagefunc info, messagefunc warning, messagefunc error, int exitonerror ));
extern _XITE_WIN_DLL int PopMessage  _XITE_PARAMS(( void ));
extern _XITE_WIN_DLL int Info  _XITE_PARAMS((int id, char *format, ... ));
extern _XITE_WIN_DLL int Message  _XITE_PARAMS((int id, char *format, ... ));
extern _XITE_WIN_DLL int Warning  _XITE_PARAMS((int id, char *format, ... ));
extern _XITE_WIN_DLL int Error  _XITE_PARAMS((int id, char *format, ... ));
extern _XITE_WIN_DLL int Usage  _XITE_PARAMS((int id, char *format, ... ));
extern _XITE_WIN_DLL int Verbose  _XITE_PARAMS(( void ));
extern _XITE_WIN_DLL int ExitOnError  _XITE_PARAMS(( void ));
extern _XITE_WIN_DLL char *LastMessage  _XITE_PARAMS(( void ));
extern _XITE_WIN_DLL FILE *MessageStream  _XITE_PARAMS(( void ));
extern _XITE_WIN_DLL int DefaultMessage  _XITE_PARAMS(( int id, char *message ));
extern _XITE_WIN_DLL int DefaultNoMessage  _XITE_PARAMS(( int id, char *message ));
extern _XITE_WIN_DLL char *xite_standard_options_usage_text _XITE_PARAMS(( void ));
extern _XITE_WIN_DLL char *xite_app_std_usage_text _XITE_PARAMS(( char *usage ));

_XITE_CPLUSPLUS_END

#endif /* _MESSAGE_H_ */
