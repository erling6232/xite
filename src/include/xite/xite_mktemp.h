

/*

________________________________________________________________

        xite_mktemp.h
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

#ifndef _XITE_MKTEMP_H_

_XITE_CPLUSPLUS_BEGIN

#ifdef SunOS
# define _XITE_MKTEMP_H_
  char *mktemp _XITE_PARAMS(( char * ));
#endif /* SunOS */

#ifdef SunOS5
# define _XITE_MKTEMP_H_
# include <stdlib.h>
#endif /* SunOS5 */

#ifdef ULTRIX
# define _XITE_MKTEMP_H_
  char *mktemp _XITE_PARAMS(( char * ));
#endif /* ULTRIX */

#ifdef AIX
# define _XITE_MKTEMP_H_
  char *mktemp _XITE_PARAMS(( char * ));
#endif /* AIX */

#if defined (IRIX) || defined(IRIX64)
# define _XITE_MKTEMP_H_
# include <stdlib.h>
#endif /* IRIX || IRIX64 */

#ifdef HPUX
# define _XITE_MKTEMP_H_
# include <unistd.h>
#endif /* HPUX */

#ifdef OSF1
# define _XITE_MKTEMP_H_
  char *mktemp _XITE_PARAMS(( char * ));
#endif /* OSF1 */

#ifdef CYGWIN32
# define _XITE_MKTEMP_H_
  char *tempnam _XITE_PARAMS(( const char *, const char * ));
  char *mktemp _XITE_PARAMS(( char * ));
#endif /* CYGWIN32 */

#ifdef _WIN32
# define _XITE_MKTEMP_H_
#endif /* _WIN32 */

#ifndef _XITE_MKTEMP_H_
# define _XITE_MKTEMP_H_
  char *mktemp _XITE_PARAMS(( char * ));
#endif /* Default case */

_XITE_CPLUSPLUS_END

#endif /* _XITE_MKTEMP_H_ */
