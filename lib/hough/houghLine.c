
/*C*

________________________________________________________________

        houghLine
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
#include <xite/hough.h>
#include XITE_STDIO_H
#include <stdlib.h>
#include <xite/blab.h>
#include <xite/convert.h>
#include <xite/message.h>
#include <xite/minmax.h>
#include <xite/draw.h>
#include <xite/readarg.h>
#include XITE_MALLOC_H



#ifndef MAX
# define MAX(a,b) (((a)>(b)) ? (a) : (b))
#endif

typedef struct _point { int rho, theta, h; struct _point *next; } point;

/*L*
_______________________________________________________________

                distEuclid
_______________________________________________________________
*/

static double distEuclid(int x1, int y1, int x2, int y2)
{
  return(sqrt((double)(((x2-x1)*(x2-x1))+((y2-y1)*(y2-y1)))));
}  

#ifndef MIN
# define MIN(a,b) (((a)<(b)) ? (a) : (b))
#endif
#ifndef M_PI
# define M_PI    3.14159265358979323846
#endif

static float *costab_s, *sintab_s;



/*F:houghLine*

________________________________________________________________

		houghLine
________________________________________________________________

Name:		houghLine - Hough transform for line detection

Syntax:         | #include <xite/hough.h>
		|
                | BiffStatus houghLine( IBAND input,
                |    IUS_BAND output );

Description:    Performs Hough transform for line detection.
                All pixels above zero in the 'input' band are
		transformed to (theta,rho) space in the 'output'
		band. The 'input' band may have any size, while 
		the 'output' band currently must be at least 
		| xsize: 180
		| ysize: 2 * (inputXsize*inputXsize + 
		|             inputYsize*inputYsize + 1)
		This limitation may be removed in future versions.
 
		Notice that band x coordinates 1..180 correspond
		to angles theta = 0 .. 179, and y coordinates
		1..YSIZE correspond to rho = -(YSIZE/2) .. YSIZE/2.

Restrictions:   'input' must have pixel type unsigned byte. 'output' must
                have pixel type unsigned short.

Return value:   | 0 - OK
		| 1 - bad input pixel type
		| 2 - bad output pixel type
       		| 3 - bad band sizes

Author:		Tor Lønnestad, BLAB, Ifi, UiO
________________________________________________________________

*/


BiffStatus houghLine(IBAND input, IUS_BAND output)
{
  int ixsize, iysize, oxsize, oysize, x, y, rho, theta, oyhalf;
  float frho;

  if (Ipixtyp(input) NE Iu_byte_typ)
    return(Error(1, "houghLine: Input pixel type must be unsigned byte.\n"));
  if (Ipixtyp((IBAND) output) NE Iu_short_typ)
    return(Error(2, "houghLine: Output pixel type must be unsigned short.\n"));

  ixsize = Ixsize(input);
  iysize = Iysize(input);
  oxsize = Ixsize((IBAND) output);
  oysize = Iysize((IBAND) output);
  oyhalf = oysize/2+1;

  if (oysize LT 2*(int)(distEuclid(0,0,ixsize,iysize)+0.5)+1)
    return(Error(3, "houghLine: Bad band sizes\n"));
 
  (void) drawBand((IBAND) output, 0.0, 0.0);

  costab_s = (float*)malloc(oxsize*sizeof(float));
  sintab_s = (float*)malloc(oxsize*sizeof(float));

  for (theta=0; theta LT oxsize; theta++) { /* should be optimized! */
    costab_s[theta] = (float)cos(M_PI*theta/oxsize);
    sintab_s[theta] = (float)sin(M_PI*theta/oxsize);
  }

  for (y=1; y LE iysize; y++) {
    for (x=1; x LE ixsize; x++) {
      if (input[y][x]) {
        for (theta=0; theta LT oxsize; theta++) {
          frho = x*costab_s[theta] + y*sintab_s[theta];
          rho = frho >= 0 ? (int) (frho+0.5) : (int) (frho - 0.5);
          output[rho+oyhalf][theta+1] ++;
	}
      }
    }
  }

  return(0);
}

/*F:drawLine*

________________________________________________________________

		drawLine
________________________________________________________________

Name:		drawLine - draw a line in a BIFF image band

Syntax:		| #include <xite/hough.h>
                |
                | int drawLine( IBAND band, int theta, int rho );

Description:    Draw a line in 'band', by increasing the pixel values by one.

Restrictions:   'band' must have pixel type unsigned byte.

See also:	drawCircle(3), houghLine(3), houghLine(1)

Author:		Original function replaced by a version contributed
                by Lucas Parra (lucas@im2.scr.siemens.com).
Id: 		$Id$
________________________________________________________________

*/

static int iround(double x)
{
  return( x > 0 ? (int) (x + 0.5) : (int) (x - 0.5) );
}

int drawLine(IBAND band, int theta, int rho)
{
  int x, y, xsize, ysize, xstart, xstop, ystart, ystop;
  float a, b, sinth;

  if (Ipixtyp(band) != Iu_byte_typ)
    return(Error(1, "drawLine(): Pixel type must be unsigned byte.\n"));

  xsize = Ixsize(band);
  ysize = Iysize(band);

  if (theta == 0) {
    for (y=1; y <= ysize; y++)
      band[y][rho]++;

    return(0);
  } else if (theta == 90) {
    for (x=1; x <= xsize; x++)
      band[rho][x]++;

    return(0);
  }

  sinth = sintab_s[theta];
  a     = -costab_s[theta]/sinth;
  b     = rho/sinth;

  ystart = MAX(1,     MIN(iround(a+b), iround(a*xsize+b)));
  ystop  = MIN(ysize, MAX(iround(a+b), iround(a*xsize+b)));
  xstart = MAX(1,     MIN(iround((1-b)/a), iround((ysize-b)/a)));
  xstop  = MIN(xsize, MAX(iround((1-b)/a), iround((ysize-b)/a)));

  if (45 <= theta && theta <= 135)
    for (x=xstart; x LE xstop; x++) band[iround(a*x + b)][x]++;

  else
    for (y=ystart; y LE ystop; y++) band[y][iround((y-b)/a)]++;

  return(0);
}    
