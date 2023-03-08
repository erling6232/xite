
/*

________________________________________________________________

        houghCircle
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

#include <math.h>
#include <xite/includes.h>
#include <xite/biff.h>
#include <stdlib.h>
#include <xite/hough.h>
#include XITE_STDIO_H
#include <xite/convert.h>
#include <xite/message.h>
#include <xite/minmax.h>
#include <xite/readarg.h>
#include <xite/draw.h>
#include XITE_MALLOC_H



#ifndef M_PI
#define M_PI    3.14159265358979323846
#endif

typedef struct _point { int x, y, h; struct _point *next; } point;

#define RMAX 10000

/*F:drawCircle*

________________________________________________________________

		drawCircle
________________________________________________________________

Name:		drawCircle - draw a circle in a BIFF image band

Syntax:		| #include <xite/hough.h>
                |
                | int drawCircle( IUS_BAND band, int x, int y,
                |    int radius);
		|

Description:    Draw a circle in 'band' centered at ('x','y')
                by increasing the pixel values by one.

Return value:   | 0 : Ok
                | 1 : Bad input pixel type
		| 2 : Radius is too large (maximum 10000)

See also:       houghCircle(3), houghCircle(1), drawLine(3)

Author:		Tor Lønnestad, BLAB, Ifi, UiO
________________________________________________________________

*/

static int sq_s[RMAX+1];
static int rmx_s = 0;

#define INSIDE(y,x) (((x)>=1) && ((x)<=xsize) && ((y)>=1) && ((y) <= ysize))

int drawCircle(IUS_BAND band, int x, int y, int radius)
{
  int xsize, ysize, x0, y0, x0sq, rsq;

  if (Ipixtyp((IBAND) band) != Iu_short_typ)
    return(Error(1, "Input pixel type must be unsigned short.\n"));

  if (radius > RMAX)
    return(Error(2, "Radius too large.\n"));

  xsize = Ixsize((IBAND) band);
  ysize = Iysize((IBAND) band);
  rsq   = radius*radius;

  if (radius > rmx_s) {
    for (rmx_s++; rmx_s <= radius; rmx_s++) sq_s[rmx_s] = rmx_s * rmx_s;
    rmx_s--;
  }

  x0 = radius;
  if ((x-radius < 1) || (x+radius > xsize)
      || (y-radius < 1) || (y+radius > ysize)) {
    /* the whole circle is NOT within the image */
    /* spec case y0=0 */
    if (INSIDE(y, x+radius)) band[y][x+radius]++;
    if (INSIDE(y, x-radius)) band[y][x-radius]++;
    if (INSIDE(y+radius, x)) band[y+radius][x]++;
    if (INSIDE(y-radius, x)) band[y-radius][x]++;

    for (y0=1; y0 < x0; y0++) {
      x0sq = rsq - sq_s[y0];
      while (sq_s[x0] > x0sq+x0) x0--;

      if (INSIDE(y+y0, x+x0)) band[y+y0][x+x0]++;
      if (INSIDE(y+y0, x-x0)) band[y+y0][x-x0]++;
      if (INSIDE(y-y0, x+x0)) band[y-y0][x+x0]++;
      if (INSIDE(y-y0, x-x0)) band[y-y0][x-x0]++;
      if (x0 != y0) {
        if (INSIDE(y+x0, x+y0)) band[y+x0][x+y0]++;
        if (INSIDE(y+x0, x-y0)) band[y+x0][x-y0]++;
        if (INSIDE(y-x0, x+y0)) band[y-x0][x+y0]++;
        if (INSIDE(y-x0, x-y0)) band[y-x0][x-y0]++;
      }
    }
    return(0);
  }

  /* spec case y0=0 */
  band[y][x+radius]++;
  band[y][x-radius]++;
  band[y+radius][x]++;
  band[y-radius][x]++;

  for (y0=1; y0 < x0; y0++) {
    x0sq = rsq - sq_s[y0];
    while (sq_s[x0] > x0sq+x0) x0--;

    band[y+y0][x+x0]++;
    band[y+y0][x-x0]++;
    band[y-y0][x+x0]++;
    band[y-y0][x-x0]++;
    if (x0 != y0) {
      band[y+x0][x+y0]++;
      band[y+x0][x-y0]++;
      band[y-x0][x+y0]++;
      band[y-x0][x-y0]++;
    }
  }
  return(0);
}



/*F:houghCircles=houghCircle*/
/*F:houghCircle*

________________________________________________________________

		houghCircle
________________________________________________________________

Name:		houghCircle, houghCircles - Hough transform for circle
                detection

Syntax:         | #include <xite/hough.h>
		|
                | BiffStatus houghCircle( IBAND input,
                |    IUS_BAND output, int radius );
		|
                | BiffStatus houghCircles( IBAND input,
                |    IUS_IMAGE output, int rmin, int rmax );

Description:    Performs a Hough transform to detect circles.

                'houghCircle' transforms the input band 'input'
		to Hough space, 'output', looking at circles
		with 'radius' only. The input band should be something
		like a thresholded edge image. All pixels larger than 0
		are transformed to Hough space.

		'houghCircles' transforms for all radii between
		'rmin' and 'rmax'. First band of output will
		correspond to radius 'rmin', second band to
		radius 'rmin'+1 and so on. 'output' should
		thus have a number of bands at least equal to
		'rmax' - 'rmin' + 1.

		Notice that output band(s) are of type unsigned
		short to avoid overflow in accumulation.

Restrictions:   'input' must have pixel type unsigned byte.

Return value:   | 0 - OK
		| 1 - bad input pixel type
		| 2 - bad output pixel type
		| 3 - bad radius value
		| 4 - not enough bands

See also:       houghCircle(1), houghLine(3), drawLine(3)

Author:		Tor Lønnestad, BLAB, Ifi, UiO

Id: 		$Id$
________________________________________________________________

*/


BiffStatus houghCircle(IBAND input, IUS_BAND output, int radius)
{
  int xsize, ysize, x, y;

  if (Ipixtyp(input) != Iu_byte_typ)
    return(Error(1, "houghCircle: Input pixel type must be unsigned byte.\n"));
  if (Ipixtyp((IBAND) output) != Iu_short_typ)
    return(Error(2,
		 "houghCircle: Output pixel type must be unsigned short.\n"));
  if ((radius < 1) || (radius > RMAX))
    return(Error(3, "houghCircle: Radius value illegal.\n"));

  drawBand((IBAND) output, 0.0, 0.0);
  xsize = Ixsize(input);
  ysize = Iysize(input);
  for (y=1; y <= ysize; y++)
    for (x=1; x <= xsize; x++)
      if (input[y][x])
        if (drawCircle(output, x, y, radius))
          Warning(5, "houghCircle: Draw error!\n");

  return(0);
}



BiffStatus houghCircles(IBAND input, IUS_IMAGE output, int rmin, int rmax)
{
  int r, stat;
  if ((rmin < 0) || (rmin > rmax))
    return(Error(3, "houghCircle: Bad radius value\n"));
  if (Inbands((IMAGE) output) < (rmax-rmin+1))
    return(Error(4, "houghCircle: Not enough bands\n"));
  for (r=rmin; r <= rmax; r++) {
    stat = houghCircle(input, output[r-rmin+1], r);
    if (stat) return(stat);
  }

  return(0);
}
