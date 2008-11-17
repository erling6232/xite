

/*

________________________________________________________________

        xite_limits.h
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

/* Define limits for XITE pixel types */

#ifndef _XITE_LIMITS_H_
#define _XITE_LIMITS_H_

_XITE_CPLUSPLUS_BEGIN

/* Get limits for integral types. */
#include <limits.h>

/* Get limits for floating types. */
#ifdef HAVE_FLOAT_H
# include <float.h>
#else
# ifdef HAVE_VALUES_H
/* <float.h> does not exist under SunOS. */
#  include <values.h>
# endif /* values.h */
#endif /* float.h */

#define UNS_BYTE_MAX UCHAR_MAX
#define SIGN_BYTE_MAX SCHAR_MAX
#define SIGN_BYTE_MIN SCHAR_MIN
#define UNS_SHORT_MAX USHRT_MAX
#define SIGN_SHORT_MAX SHRT_MAX
#define SIGN_SHORT_MIN SHRT_MIN
#if LONG_MAX > 2147483647
# define INTEGER_MAX INT_MAX
# define INTEGER_MIN INT_MIN
#else
# define INTEGER_MAX LONG_MAX
# define INTEGER_MIN LONG_MIN
#endif
#ifdef HAVE_FLOAT_H
# define REAL_MAX FLT_MAX
# define REAL_MIN FLT_MIN
# define DOUBLE_MAX DBL_MAX
# define DOUBLE_MIN DBL_MIN
#else
/* Don't use MINFLOAT and MINDOUBLE, because they are subnormal numbers under
 * SunOS. Use IEEE values instead, equal to extreme normalized numbers on
 * other platforms.
 */
# define REAL_MAX MAXFLOAT
# define REAL_MIN 1.175494351e-38
# define DOUBLE_MAX MAXDOUBLE
# define DOUBLE_MIN 2.2250738585072014e-308
#endif /* SunOS */

_XITE_CPLUSPLUS_END

#endif /* _XITE_LIMITS_H_ */
