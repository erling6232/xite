
/*

________________________________________________________________

        combineTransf
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

static char *Id = "$Id$, Blab, UiO";



#include <xite/includes.h>
#include <xite/blab.h>
#include <xite/biff.h>
#include <xite/histo.h>

/*F:combineTransf*

________________________________________________________________

		combineTransf
________________________________________________________________

Name:		combineTransf - combine two histo. transformations to one

Syntax:         | #include <xite/histo.h>
                |
                | void combineTransf( histogram h1, histogram h2,
                |    histogram h3 );
Description:    combine two histogram transformations into one.
                Any two, or all three parameters may be identical.

                | h1 - histogram that was to be applied first
                | h2 - histogram that was to be applied secondly
                | h3 - output combined histogram transformation.

Return value:   none

Author:		Tor L|nnestad

Id:             $Id$
________________________________________________________________
*/

void combineTransf(int *h1, int *h2, int *h3)
{
  int n;
  for (n=0; n <= 255; n++)
    h3[n] = h2[h1[n]];
}
