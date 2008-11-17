

/*

________________________________________________________________

        xite_random.h
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

#ifndef _XITE_RANDOM_H_
# define _XITE_RANDOM_H_

_XITE_CPLUSPLUS_BEGIN

#ifdef HAVE_STDLIB_H
#  include <stdlib.h>
#endif
#ifdef HAVE_MATH_H
#  include <math.h>
#endif

#ifdef RAND_MAX
#   define RANDOM_MAX RAND_MAX
#else
#  define RANDOM_MAX 32767.0
#  define RANDOM_MAX 2147483653.0
#endif

#ifndef HAVE_RANDOM
#  ifdef HAVE_RAND
#    define random  rand
#  else
     long random _XITE_PARAMS(( void ));
#  endif
#endif
#ifndef HAVE_SRANDOM
#  ifdef HAVE_SRAND
#    define srandom srand
#  else
     int srandom _XITE_PARAMS(( int ));
#  endif
#endif

_XITE_CPLUSPLUS_END

#endif /* _XITE_RANDOM_H_ */

