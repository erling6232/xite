
/*

________________________________________________________________

        histoTransf
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
#include <xite/message.h>

#ifndef MIN
# define MIN(a,b) (((a) LT (b)) ? (a) : (b))
#endif

/*F:histoTransf*

________________________________________________________________

		histoTransf
________________________________________________________________

Name:		histoTransf - perform general histogram transformation

Syntax:         | #include <xite/histo.h>
                |
                | int histoTransf( IBAND inband, IBAND outband,
                |    histogram hist );

Description:    Apply a histogram transformation stored in the histogram
                'hist'. 'Inband' and 'outband' band may be identical.
                If 'inband' and 'outband' have different sizes, the largest
                common rectangle is transformed.

Restrictions:   'inband' and 'outband' must have pixel type unsigned byte.

Return value:   | 0 => ok
                | 1 => Bad input pixel type
                | 2 => Bad output pixel type

Author:		Tor Lønnestad, BLAB, Ifi, UiO

Id:             $Id$
________________________________________________________________
*/

int histoTransf(IBAND inband, IBAND outband, histogram hist)
{
   int x, y, xsize, ysize;
   int xs1, ys1, xs2, ys2;

   if (Ipixtyp(inband) != Iu_byte_typ)
     return(Error(1,
		  "histoTransf: Input pixel type must be unsigned byte.\n"));
   if (Ipixtyp(outband) != Iu_byte_typ)
     return(Error(2,
		  "histoTransf: Output pixel type must be unsigned byte.\n"));

   xs1   =  Ixsize(inband);
   ys1   = Iysize(inband);
   xs2   =  Ixsize(outband);
   ys2   = Iysize(outband);
   xsize = MIN(xs1,xs2);
   ysize = MIN(ys1,ys2);

   for (y=1; y LE ysize; y++)
     for (x=1; x LE xsize; x++)
       outband[y][x] = hist[inband[y][x]];

  return(0);
}
