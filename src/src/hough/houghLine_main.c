
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

/*L*

________________________________________________________________

		findMaxima
________________________________________________________________

Name:		findMaxima

Syntax:		| static int findMaxima(b1, b2, maxima, t)
                | IBAND b1, b2;
		| point** maxima;
		| double t;

Description:    Return list of points representing local maxima,
                points with value equal to max value in window,
		and value greater than 't'.

Restriction:    b1 and b2 should have equal size

Return value:   The number of such points

See also:       houghLine(3), houghLine(1), drawCircle(3)

Author:		Tor Lønnestad, BLAB, Ifi, UiO
________________________________________________________________

*/

static int findMaxima(IBAND b1, IBAND b2, point **max, int t)
{
  int x, y, xsize, ysize, yhalf, count=0;
  point *pt;

  if (Ipixtyp(b1) != Iu_byte_typ || Ipixtyp(b2) != Iu_byte_typ)
    Error(-1, "findMaxima(): Pixel type must be unsigned byte.\n");

  xsize = Ixsize(b1);
  ysize = Iysize(b1);
  yhalf = ysize/2 + 1;

  for (y=1; y LE ysize; y++) {
    for (x=1; x LE xsize; x++) {
      if (b1[y][x] EQ b2[y][x]) {
        if (b1[y][x] GT t) {
          pt = (point*)malloc(sizeof(point));
          pt->theta = x-1;
          pt->rho = y-yhalf;
          pt->h = b1[y][x];
          pt->next = *max;
          *max = pt;
          count++;
	}
      }
    }
  }

  return(count);
}




/*P:houghLine*

________________________________________________________________

		houghLine
________________________________________________________________

Name:		houghLine - Hough transform for line detection

Syntax:		| houghLine [<option>...] <inimage>

Description:    Performs a Hough transform to detect lines. Every band in the
                input image 'inimage' is transformed to a two dimensional
                Hough space, a (theta, rho) space.

		After creating the transform, the Hough space may be searched
		for local maxima. Within each band, only the largest local
		maximum (maxima) within a 'ws'x'ws' area is registered.
		Besides, only maxima with number of updates above a limit
		given by the 'ul' option are used. A list of these maxima may
		be written to stdout, depending on the options given.

Options:        &-list
                Print a list of maxima to stdout. Default: Don''t.

		&-tf transformFileName
		Save the transform to 'transformFileName'. Default: Don''t
		save.
		
		&-rf reconstructionFileName
		The Hough-space	maxima are used to reconstruct the image,
		and the image is written to 'reconstructionFileName'.
		Default: Don''t reconstruct and save.

		&-tt transformTitle
		Use 'transformTitle' as title for transformed image.
		Default: "Hough Line Transform".

		&-rt reconstructionTitle
		Use 'reconstructionTitle' as title for reconstructed image.
		Default: "Hough Line Reconstruction".

		&-ul updateLimit
		This determines the minimum number of updates for a maximum
		to be used. The minimum number is determined from 'updateLimit'
		and the size of the hough space image:
		
		| updateLimit * MAX(horizontal size, vertical size)

		Default: 0.1.

		&-ws windowSize
		Default: 7.

Restrictions:   'inimage' must have bands with pixel type unsigned byte.

See also:	houghLine(3), drawLine(3), houghCircle(1)

Author:		Tor Lønnestad, BLAB, Ifi, UiO

Examples:	| houghLine -tf monaHT.img -rf monaRF.img -ul 0.2 mona.img

Id: 		$Id$
________________________________________________________________

*/

int main(int argc, char **argv)
{
  IMAGE i1, i2, i3;
  IBAND ubb;
  int bn, stat, xsize=180, ysize, yhalf, ws, nMaxima=0;
  double ul;
  point **maxima;
  point *max;
  char *tt, *rt, *tf, *rf;
  char rarg[100];
  int list;

  InitMessage(&argc, argv, xite_app_std_usage_text(
    "Usage: %s [<option>...] <inimage> \n\n\
     where <option> may be:		Defaults:		\n\
       -list                            No                      \n\
       -tf <transformFileName>		No default		\n\
       -rf <reconstructionFileName>	No default		\n\
       -tt <transformTitle>		Hough Line Transform	\n\
       -rt <reconstructionTitle>	Hough Line Reconstruction \n\
       -ul <updateLimit>		0.1			\n\
       -ws <windowSize>			7		        \n"));

  if (argc EQ 1) Usage(1, NULL);

  tf   = read_switch(&argc, argv, "-tf", TRUE, NULL);
  rf   = read_switch(&argc, argv, "-rf", TRUE, NULL);
  tt   = read_switch(&argc, argv, "-tt", TRUE, "Hough Line Transform");
  rt   = read_switch(&argc, argv, "-rt", TRUE, "Hough Line Reconstruction");
  ul   = read_dswitch(&argc, argv, "-ul", 0.1);
  ws   = read_iswitch(&argc, argv, "-ws", 7);
  list = read_bswitch(&argc, argv, "-list");

  if (argc NE 2) Usage(2, "Illegal number of arguments.\n");

  i1 = Iread_image(argv[1]);
  i2 = Init_image(Inbands(i1), tt);
  Icopy_text(i1, i2);
  maxima = (point**) calloc(Inbands(i1) + 1, sizeof(point*));

  Info(0, "%s: Hough Line Transform...\n", argv[0]);
  for (bn=1; bn LE Inbands(i1); bn++) {
    yhalf = (int)(distEuclid(0,0,Ixsize(i1[bn]),Iysize(i1[bn]))+0.5);
    ysize = 2*yhalf+1;
    i2[bn] = Imake_band(Iu_short_typ, xsize, ysize);
    if ((stat = houghLine(i1[bn], (IUS_BAND)i2[bn]))) exit(stat);
    ubb = (IBAND) mkConvertBand(i2[bn], Iu_byte_typ);
    Idel_band(&(i2[bn]));
    i2[bn] = ubb;
    Iset_start(i2[bn], 0, -yhalf);
  }

  if (tf) {
    Info(0, "%s: Saving transform on %s...\n", argv[0], tf);
    Ihistory(i2, argv[0], "");
    Iwrite_image(i2, tf);
  }

  if (rf || list) {
    Info(0, "%s: Searching for maxima...\n", argv[0]);
    ubb = Imake_band(Iu_byte_typ, xsize, ysize);
    for (bn=1; bn LE Inbands(i2); bn++) {
      maxarea(i2[bn], ubb, ws, ws);
      nMaxima += findMaxima(i2[bn], ubb, &(maxima[bn]), 
			    (int)(MAX(xsize, ysize)*ul));
    }
  }

  Idel_image(&i2);

  if (nMaxima && list) {
    Info(0, "\n%s: Found a total of %d maxima\n\n",argv[0], nMaxima);
    for (bn=1; bn LE Inbands(i1); bn++) {
      printf("Band %2d:\n", bn); 
      for (max=maxima[bn]; max; max=max->next)
        printf("  Theta: %4d  Rho: %4d  Updates: %4d\n", 
	       max->theta, max->rho, max->h);
    }
  }

  if (rf) {
    i3 = Icopy_init(i1);
    Iset_title(i3, rt);
    for (bn=1; bn LE Inbands(i3); bn++) {
      (void) drawBand(i3[bn], 0.0, 0.0);
      for (max=maxima[bn]; max; max=max->next)
        drawLine(i3[bn], max->theta, max->rho);
    }

    Info(0, "%s: Saving reconstruction on %s...\n", argv[0], rf);
    sprintf(rarg, " UpdateLimit: %.2f, WindowSize: %d", ul, ws);
    Ihistory(i3, argv[0], rarg);
    Iwrite_image(i3, rf);   
  }

  return(0);
}
