/*

________________________________________________________________

        debug.h
        debug.h,v 1.2 1995/08/23 14:58:38 svein Exp
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

#ifndef _DEBUG_H_
#define _DEBUG_H_

_XITE_CPLUSPLUS_BEGIN

#ifdef DEBUG

#include <xite/includes.h>
#include XITE_STDIO_H
#include XITE_STRING_H

#define ENTER_FUNCTION_DEBUG(a) enter_function_debug(a)
#define LEAVE_FUNCTION_DEBUG(a) leave_function_debug(a)

#define FPRINTF1(a) {\
  if (strlen(a) >= 4 && strncmp(a, "    ", 4) == 0)\
    fprintf(stderr, prepend_debug_blanks(a)); \
  else fprintf(stderr, a);\
}

#define FPRINTF2(a, b) {\
  if (strlen(a) >= 4 && strncmp(a, "    ", 4) == 0) \
    fprintf(stderr, prepend_debug_blanks(a), b); \
  else fprintf(stderr, a, b);\
}

#define FPRINTF3(a, b, c) {\
  if (strlen(a) >= 4 && strncmp(a, "    ", 4) == 0) \
    fprintf(stderr, prepend_debug_blanks(a), b, c); \
  else fprintf(stderr, a, b, c);\
}

#define FPRINTF4(a, b, c, d) {\
  if (strlen(a) >= 4 && strncmp(a, "    ", 4) == 0) \
    fprintf(stderr, prepend_debug_blanks(a), b, c, d); \
  else \
    fprintf(stderr, a, b, c, d);\
}

#define FPRINTF5(a, b, c, d, e) {\
  if (strlen(a) >= 4 && strncmp(a, "    ", 4) == 0) \
    fprintf(stderr, prepend_debug_blanks(a), b, c, d, e); \
  else \
    fprintf(stderr, a, b, c, d, e);\
}

#define FPRINTF6(a, b, c, d, e, f) {\
  if (strlen(a) >= 4 && strncmp(a, "    ", 4) == 0) \
    fprintf(stderr, prepend_debug_blanks(a), b, c, d, e, f); \
  else \
    fprintf(stderr, a, b, c, d, e, f);\
}

extern _XITE_WIN_DLL char *prepend_debug_blanks _XITE_PARAMS(( char *t ));
extern _XITE_WIN_DLL void enter_function_debug _XITE_PARAMS(( char *func_name ));
extern _XITE_WIN_DLL void leave_function_debug _XITE_PARAMS(( char *func_name ));

#else /* DEBUG */

#define ENTER_FUNCTION_DEBUG(a) ;
#define LEAVE_FUNCTION_DEBUG(a) ;
#define FPRINTF1(a) ;
#define FPRINTF2(a,b) ;
#define FPRINTF3(a,b,c) ;
#define FPRINTF4(a,b,c,d) ;
#define FPRINTF5(a,b,c,d,e) ;
#define FPRINTF6(a,b,c,d,e,f) ;

#endif /* DEBUG */

_XITE_CPLUSPLUS_END

#endif /* _DEBUG_H_ */
