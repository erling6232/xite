
/*

________________________________________________________________

        thresPtile
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
#include <xite/biff.h>
#include XITE_STDIO_H
#include <stdlib.h>
#include <xite/blab.h>
#include <xite/histo.h>
#include <xite/readarg.h>
#include <xite/message.h>
#include <xite/threshold.h>



/*F:thresPtile*

________________________________________________________________

		thresPtile
________________________________________________________________

Name:		thresPtile - threshold at specified percentage

Syntax:         | #include <xite/threshold.h>
		|
                | int thresPtile( histogram h, float pTile );
Description:	Calculates the threshold value needed to perform
                thresholding of the image behind the histogram <h>
		according to the <pTile> argument. This argument
		describes the percentage of the pixels that should
		be below the threshold. The actual thresholding
		is not performed, see the routine "threshold".
		
Restrictions:   pTile should be in 0.0 .. 100.0 

Return value:	threshold value. Pixel values up to, and including,
		the return value, is below the threshold.

Author:		Tor L|nnestad, BLAB, ifi, UiO

Id:             $Id$
________________________________________________________________

*/

int thresPtile ( histogram h, float pTile )
{
  int i, sum, limit ;

  if (pTile LT 0.0) return(-1);
  if (pTile GT 100.0) return(255);
  for (sum=0, i=0; i LE 255;  sum += h[i++]) ;
  limit = (int)((sum*pTile/100.0)+0.5);
  for (sum=0, i=0; sum LT limit; sum += h[i++]) ;
  return(--i);
}  /*  thresPtile  */
