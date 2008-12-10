

/*C*

________________________________________________________________

        xite_usleep
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

#include <xite/includes.h>
#include <xite/debug.h>
#ifdef _WIN32
# include <winsock.h>
#elif !defined(MSDOS)
# include <sys/time.h>
#endif
#include XITE_UNISTD_H
#include <xite/utils.h>

void xite_usleep(unsigned long n)
{
  struct timeval tv;

  ENTER_FUNCTION_DEBUG("xite_usleep.c: xite_usleep");

  tv.tv_sec  = n / 1000000;
  tv.tv_usec = n % 1000000;
  select(0,0,0,0, &tv);

  LEAVE_FUNCTION_DEBUG("xite_usleep.c: xite_usleep");
}
