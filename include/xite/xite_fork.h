

/*

________________________________________________________________

        xite_fork.h
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

#ifndef _XITE_FORK_H_
#define _XITE_FORK_H_

_XITE_CPLUSPLUS_BEGIN

#ifdef HAVE_UNISTD_H
#  include <unistd.h>
#endif

#ifdef HAVE_WORKING_VFORK
#  ifdef HAVE_VFORK_H
#    include <vfork.h>
#  endif
#else
#  ifdef HAVE_WORKING_FORK
#    ifdef HAVE_FORK_H
#      include <fork.h>
#    endif
#    define vfork fork
#  endif
#endif

#ifdef HAVE_PROCESS_H
#  include <process.h>         /* Win32 */
#endif

_XITE_CPLUSPLUS_END

#endif /* _XITE_FORK_H_ */

