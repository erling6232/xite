

/*

________________________________________________________________

        xite_toupper.h
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


#ifndef _XITE_TOUPPER_H_

_XITE_CPLUSPLUS_BEGIN

#ifdef SunOS
# define _XITE_TOUPPER_H_
  int tolower _XITE_PARAMS(( int ));
  int toupper _XITE_PARAMS(( int ));
#endif /* SunOS */

#ifdef SunOS5
# define _XITE_TOUPPER_H_
# include <ctype.h>
#endif /* SunOS5 */

#ifdef ULTRIX
# define _XITE_TOUPPER_H_
# include <ctype.h>
#endif /* ULTRIX */

#ifdef AIX
# define _XITE_TOUPPER_H_
# include <ctype.h>
#endif /* AIX */

#if defined(IRIX) || defined(IRIX64)
# define _XITE_TOUPPER_H_
# include <ctype.h>
#endif /* IRIX || IRIX64 */

#ifdef HPUX
# define _XITE_TOUPPER_H_
# include <ctype.h>
#endif /* HPUX */

#ifdef OSF1
# define _XITE_TOUPPER_H_
# include <ctype.h>
#endif /* OSF1 */

#ifdef CYGWIN32
# define _XITE_TOUPPER_H_
# include <ctype.h>
#endif /* CYGWIN32 */

#if defined(Linux)
# define _XITE_TOUPPER_H_
# include <ctype.h>
#endif /* Linux */

#ifdef _WIN32
# define _XITE_TOUPPER_H_
# include <ctype.h>
#endif /* _WIN32 */

#ifndef _XITE_TOUPPER_H_
# define _XITE_TOUPPER_H_
  int tolower _XITE_PARAMS(( int ));
  int toupper _XITE_PARAMS(( int ));
#endif /* Default case */

_XITE_CPLUSPLUS_END

#endif /* _XITE_TOUPPER_H_ */
