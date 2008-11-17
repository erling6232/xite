

/*

________________________________________________________________

        xite_strtol.h
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


#ifndef _XITE_STRTOL_H_
#define _XITE_STRTOL_H_

_XITE_CPLUSPLUS_BEGIN

#ifdef HAVE_STDLIB_H
#  include <stdlib.h>
#endif

#ifndef HAVE_STRTOL
  long strtol _XITE_PARAMS(( const char *, char **, int  ));
#endif

#ifndef HAVE_STRTOD
  double strtod _XITE_PARAMS(( const char *, char ** ));
#endif

_XITE_CPLUSPLUS_END

#endif /* _XITE_STRTOL_H_ */
