
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

#ifndef MAIN

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

#ifndef FUNCPROTO
int drawCircle(band, x, y, radius)
IUS_BAND band;
int x, y, radius;
#else /* FUNCPROTO */
int drawCircle(IUS_BAND band, int x, int y, int radius)
#endif /* FUNCPROTO */
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


#ifndef FUNCPROTO
BiffStatus houghCircle(input, output, radius)
IBAND input;
IUS_BAND output;
int radius;
#else /* FUNCPROTO */
BiffStatus houghCircle(IBAND input, IUS_BAND output, int radius)
#endif /* FUNCPROTO */
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



#ifndef FUNCPROTO
BiffStatus houghCircles(input, output, rmin, rmax)
IBAND input;
IUS_IMAGE output;
int rmin, rmax;
#else /* FUNCPROTO */
BiffStatus houghCircles(IBAND input, IUS_IMAGE output, int rmin, int rmax)
#endif /* FUNCPROTO */
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

#endif /* not MAIN */



#ifdef MAIN

/*L*

________________________________________________________________

		findMaxima
________________________________________________________________

Name:		findMaxima

Syntax:		| static int findMaxima(b1, b2, max, t)
                | IBAND b1, b2;
		| point** max;
		| double t;

Description:    Return list of points representing local maxima,
                points with value equal to max value in window,
		and value greater than 't'.

Return value:   The number of such points

Author:		Tor Lønnestad, BLAB, Ifi, UiO
________________________________________________________________

*/

#ifndef FUNCPROTO
static int findMaxima(b1, b2, max, t)
IBAND b1, b2;
point** max;
int t;
#else /* FUNCPROTO */
static int findMaxima(IBAND b1, IBAND b2, point **max, int t)
#endif /* FUNCPROTO */
{
  int x, y, xsize, ysize, count=0;
  point *p;

  if (Ipixtyp(b1) != Iu_byte_typ)
    return(Error(-1, "findMaxima: Pixel type must be unsigned byte.\n"));
  if (Ipixtyp(b2) != Iu_byte_typ)
    return(Error(-1, "findMaxima: Pixel type must be unsigned byte.\n"));

  xsize = Ixsize(b1);
  ysize = Iysize(b1);

  for (y=1; y <= ysize; y++) {
    for (x=1; x <= xsize; x++) {
      if (b1[y][x] == b2[y][x])
        if (b1[y][x] > t) {
          p = (point*)malloc(sizeof(point));
          p->x = x-1;
          p->y = y;
          p->h = b1[y][x];
          p->next = *max;
          *max = p;
          count++;
        }
    }
  }

  return(count);
}


/*P:houghCircle*

________________________________________________________________

		houghCircle
________________________________________________________________

Name:		houghCircle - Hough transform to detect circles

Syntax:		houghCircle [<option>...] <inimage> <minradius> <maxradius>

Description:    Performs a Hough transform to detect circles. The first band
                of the input image 'inimage' is transformed to a three
                dimensional Hough space, a multi band image. The size of each
                output band will be identical to the size of the input band,
                and the number of bands will be 'maxradius'-'minradius'+1.

		The input image should be something like a thresholded edge
		image. All pixels larger than 0 are transformed to Hough
		space.

		After creating the transform, the Hough space may be searched
		for local maxima. Within each band, only the largest local
		maximum within a 'ws'x'ws' area is registered. Besides, only
		maxima with number of updates above 'ul' (on a scale
		0.0 .. 1.0) are used. A list of these maxima may be written
		to stdout, depending on the options given.

Options:        &-list
                Print a list of maxima to stdout. Default: Don''t.

		&-tf transformFileName
		Save the transform to 'transformFileName'. Default: Don''t
		save.

		&-rf reconstructionFileName
		The Hough-space maxima are used to reconstruct the image, and
		the image is written to 'reconstructionFileName'.
		Default: Don''t reconstruct and save.

		&-tt transformTitle
		Use 'transformTitle' as title for transformed image.
		Default: "Hough Circle Transform".

		&-rt reconstructionTitle
		Use 'reconstructionTitle' as title for reconstructed image.
		Default: "Hough Circle Reconstruction".

		&-ul updateLimit
		This determines the minimum number of updates for a maximum
		to be used. The minimum number is determined from 'updateLimit'
		and the radius corresponding to the current band:

		| updateLimit * 2.0 * Pi * radius
		
		Default: 0.5.

		&-ws windowSize
		Default: 7.

See also:	houghLine(1), drawCircle(3), xmovie(1)

Restrictions:   Input image must have pixel type unsigned byte.

Return value:   0 when ok, positive otherwise.

Author:		Tor Lønnestad, BLAB, Ifi, UiO

Examples:       | houghCircle -ws 11 -rf circles.img \\
                |     -rt DetectedCircles input.img 5 25

Id: 		$Id$
________________________________________________________________

*/

#ifndef FUNCPROTO
int main(argc, argv)
int argc;
char *argv[];
#else /* FUNCPROTO */
int main(int argc, char **argv)
#endif /* FUNCPROTO */
{
  IMAGE i1, i2, i3;
  IUS_BAND usb;
  IBAND ubb;
  int bn, r, rmin, rmax, stat, xsize, ysize, ws, nMaxima=0;
  double ul;
  point **maxima;
  point *max;
  char *tt, *rt, *tf, *rf, *options;
  int list;

  InitMessage(&argc, argv, xite_app_std_usage_text(
    "Usage: %s [<option>...] <inimage> <minradius> <maxradius> \n\n\
     where <option> may be:		Defaults:		\n\
       -list                            No                      \n\
       -tf <transformFileName>		No default		\n\
       -rf <reconstructionFileName>	No default		\n\
       -tt <transformTitle>		Hough Transform		\n\
       -rt <reconstructionTitle>	Hough Reconstruction	\n\
       -ul <updateLimit>		0.5	\n\
       -ws <windowSize>			7	\n"));

  if (argc == 1) Usage(1, NULL);
  options = argvOptions(argc, argv);

  tf   = read_switch(&argc, argv, "-tf", TRUE, NULL);
  rf   = read_switch(&argc, argv, "-rf", TRUE, NULL);
  tt   = read_switch(&argc, argv, "-tt", TRUE, "Hough Circle Transform");
  rt   = read_switch(&argc, argv, "-rt", TRUE, "Hough Circle Reconstruction");
  ul   = read_dswitch(&argc, argv, "-ul", 0.5);
  ws   = read_iswitch(&argc, argv, "-ws", 7);
  list = read_bswitch(&argc, argv, "-list");

  if (argc != 4) Usage(2, "Illegal number of arguments.\n");

  i1 = Iread_image(argv[1]);
  if (! i1) exit(Error(3, "Couldn't read file %d\n", argv[1])); 
  xsize = Ixsize(i1[1]);
  ysize = Iysize(i1[1]);
  rmin = atoi(argv[2]);
  rmax = atoi(argv[3]);
  if (rmax < rmin) exit(Error(4, "Must have rmin <= rmax \n"));

  maxima = (point**)calloc(rmax-rmin+2, sizeof(point*));

  Info(0, "%s: Circular Hough transform...\n", argv[0]);
  i2 = Imake_image(rmax-rmin+1, tt, Iu_short_typ, xsize, ysize);
  if ((stat = houghCircles(i1[1], (IUS_IMAGE)i2, rmin, rmax))) exit(5);

  Info(0, "%s: Converting...\n", argv[0]);
  for (r=1; r <= Inbands(i2); r++) {
    ubb = (IBAND)mkConvertBand(i2[r], Iu_byte_typ);
    Idel_band(&(i2[r]));
    i2[r] = ubb;
  }

  if (tf) {
    Info(0, "%s: Saving transform on %s...\n", argv[0], tf);
    Ihistory(i2, argv[0], options);
    Iwrite_image(i2, tf);
  }

  if (rf || list) {
    Info(0, "%s: Searching for maxima...\n", argv[0]);
    ubb = Imake_band(Iu_byte_typ, xsize, ysize);
    for (bn=1; bn <= Inbands(i2); bn++) {
      maxarea(i2[bn], ubb, ws, ws);
      nMaxima += findMaxima(i2[bn], ubb, &(maxima[bn]),
			    (int)(2.0*M_PI*(bn+rmin-1)*ul));
    }
  }
  
  if (rf) {
    usb = (IUS_BAND)Imake_band(Iu_short_typ, xsize, ysize);
    (void) drawBand((IBAND) usb, 0.0, 0.0);
  }

  if (list) {
    Info(0, "\n%s: Found A total of %d maxima\n\n",argv[0], nMaxima);
  }

  if (list || rf) {
    for (bn=1; bn <= Inbands(i2); bn++) {
      for (max=maxima[bn]; max; max=max->next) {
	if (list) printf("Radius:%3d  X:%4d  Y:%4d, H: %4d\n", 
			 bn+rmin-1, max->x, max->y, max->h);
	if (rf) drawCircle(usb, max->x, max->y, bn+rmin-1);
      }
    }
  }

  if (rf) {
    convertBand((IBAND) usb, ubb);
    i3 = Init_image(1, rt);
    i3[1] = ubb;
    Info(0, "%s: Saving reconstruction on %s...\n", argv[0], rf);
    Ihistory(i3, argv[0], options);
    Iwrite_image(i3, rf);
  }

  return(0);
}

#endif /* MAIN */
