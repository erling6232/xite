

/*C*

________________________________________________________________

        strings
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

#ifndef _STRINGS_H_
#define _STRINGS_H_

#include <xite/includes.h>

_XITE_CPLUSPLUS_BEGIN

extern _XITE_WIN_DLL char *nextSpace _XITE_PARAMS(( char *input ));
extern _XITE_WIN_DLL char *nextNonSpace _XITE_PARAMS(( char *input ));
extern _XITE_WIN_DLL char *nextWord _XITE_PARAMS(( char *input ));
extern _XITE_WIN_DLL char *iindex _XITE_PARAMS(( char *s, char c ));
extern _XITE_WIN_DLL char *rrindex _XITE_PARAMS(( char *s, char c ));
extern _XITE_WIN_DLL char *remove_quotes_from_word _XITE_PARAMS(( char *word ));
#ifdef DEF_STRCASECMP
extern _XITE_WIN_DLL int strcasecmp _XITE_PARAMS(( char *s1, char *s2 ));
#endif
extern _XITE_WIN_DLL char *center_text _XITE_PARAMS(( char *long_txt, char *short_txt, int width ));

_XITE_CPLUSPLUS_END

#endif /* _STRINGS_H_ */

