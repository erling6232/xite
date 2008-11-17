

/*C*

________________________________________________________________

        crossing
        $Id$
        Copyright 1991, Blab, UiO
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
#include <stdlib.h>
#include <xite/convolve.h>
#include XITE_STDIO_H
#include <xite/blab.h>
#include <xite/readarg.h>
#include <xite/message.h>

#ifndef MIN
# define MIN(a,b) (((a)<(b)) ? (a) : (b))
#endif



/*F:crossing*

________________________________________________________________

		crossing
________________________________________________________________

Name:		crossing - find zero crossing or any other crossing

Syntax:         | #include <xite/convolve.h>
		|
                | int crossing( IBAND iband, IBAND oband, int t );

Description:	Can be used to find zero crossing of second derivative,
		laplace. Can also be used to determine any other kind
		of crossing. Pixels below or equal to 't' are set if the pixel
		to the right or below is above 't', pixels above 't' are
		set if the pixel to the right or below is below or equal to
		't'. Pixels that are "set" are set to the maximum absolute
		difference of the two neighbours, to indicate the strength
		of the edge.

		| IF (crossing t)
		|   out(x,y) = MAX(ABS(in(x,y)-in(x+1,y)),
		|                  ABS(in(x,y)-in(x,y+1)))
		| ELSE
		|   out(x,y) = 0

		Identical 'iband' and 'oband' arguments are allowed.

Restrictions:	'iband' and 'oband' must have pixel type unsigned byte.

Return value:	| 0 => ok
		| 1 => bad pixel type input band
		| 2 => bad pixel type output band

See also:       crossing(1), contour(3)

Author:		Tor Lønnestad, BLAB, Ifi, UiO

Id: 		$Id$
________________________________________________________________

*/


BiffStatus crossing(IBAND iband, IBAND oband, int t)
{
  int x, y, xsize, ysize;

  xsize = MIN(Ixsize(iband), Ixsize(oband));
  ysize = MIN(Iysize(iband), Iysize(oband));
  if (Ipixtyp(iband) NE Iu_byte_typ)
    return(Error(1, "crossing: Input pixel type must be unsigned byte.\n"));
  if (Ipixtyp(oband) NE Iu_byte_typ)
    return(Error(2, "crossing: Output pixel type must be unsigned byte.\n"));

  FOR (y=1; y LT ysize; y++)
    FOR (x=1; x LT xsize; x++)
      IF (iband[y][x] LE t)
        if (iband[y+1][x] GT iband[y][x+1])
	  if (iband[y+1][x] GT t) oband[y][x] = iband[y+1][x] - iband[y][x];
          else oband[y][x] = 0;
        else
	  if (iband[y][x+1] GT t) oband[y][x] = iband[y][x+1] - iband[y][x];
          else oband[y][x] = 0;
      ELSE
        if (iband[y+1][x] LT iband[y][x+1])
	  if (iband[y+1][x] LE t) oband[y][x] = iband[y][x] - iband[y+1][x];
          else oband[y][x] = 0;
        else
	  if (iband[y][x+1] LE t) oband[y][x] = iband[y][x] - iband[y][x+1];
          else oband[y][x] = 0;
      ENDIF;
    ENDFOR;
    /* last pixel on line */
    IF (iband[y][xsize] LE t)
      if (iband[y+1][xsize] GT t)
        oband[y][xsize] = iband[y+1][xsize]-iband[y][xsize];
      else oband[y][xsize] = 0;
    ELSE
      if (iband[y+1][xsize] LE t)
        oband[y][xsize] = iband[y][xsize]-iband[y+1][xsize];
      else oband[y][xsize] = 0;
    ENDIF;
  ENDFOR;
  /* last line */
  FOR (x=1; x LT xsize; x++)
    IF (iband[ysize][x] LE t)
      if (iband[y][x+1] GT t)
        oband[ysize][x] = iband[ysize][x+1]-iband[ysize][x];
      else oband[ysize][x] = 0;
    ELSE
      if (iband[y][x+1] LE t)
        oband [ysize][x] = iband[ysize][x]-iband[ysize][x+1];
      else oband[ysize][x] = 0;
    ENDIF;
  ENDFOR;
  /* last pixel */
  oband[ysize][xsize] = 0;

  return(0);
}
