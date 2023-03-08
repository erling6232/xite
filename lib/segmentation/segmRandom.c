
/*

________________________________________________________________

        
        $Id$
        Copyright 1992, Blab, UiO
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
#include <xite/segmentation.h>
#include <xite/message.h>
#include <xite/readarg.h>
#include XITE_RANDOM_H
#include XITE_STDIO_H
#include XITE_TIME_H

/*F:segmRandom*

________________________________________________________________

		segmRandom
________________________________________________________________

Name:		segmRandom - draw two regions, random border

Syntax:         | #include <xite/segmentation.h>
		|
                | int segmRandom( IBAND band, double prob[3][2],
                |    int lpv, int rpv );

Description:	'band' is altered into a band with two regions with
                value 'lpv' (left pixel value) on the left part, and
		'rpv' on the right. The border separating the regions
		is made with a kind of random walk, or 2. order markov
		process, starting in the center of the upper line.
		In each step, the border may go down-left (here denoted
		left), down, or down-right (right).

		The probablity of going in each of the three directions
		left, down and right, are given in the table 'prob'.
		The three rows in the table indicate the previous
		direction of the walk. The two columns in the table
		is the probability for going left and right. The
		probability for going down is computed, because the
		total probability always is equal one.

                The table 'prob':
                |   prob[0][0] = p(left | left)
                |   prob[1][0] = p(left | down)
                |   prob[2][0] = p(left | rigth)
		|   prob[0][1] = p(right | left)
		|   prob[1][1] = p(right | down)
		|   prob[2][1] = p(right | right)

Restrictions:	Accepts only UNS_BYTE pixles.

Return value:	| 0 : OK
                | 2 : Bad pixle type
		| 3 : Bad probabilities
		| 4 : Bad pixel values

Author:		Martin Torpe Lie, BLAB, Ifi, UiO

Id: 		$Id$
________________________________________________________________

*/

int segmRandom(IBAND band, double (*prob)[2], int lpv, int rpv)
{
  int x, y, xsize, xval, pre=1;
  double ran2;

  if (Ipixtyp(band) NE Iu_byte_typ)
       return(Error(2,"segmRandom: Bad pixel type.\n"));

  for (y=0; y <=2; y++) for (x=0; x<=1; x++)
    if (prob[y][x] <0.0) return(Error(3,"segmRandom: Bad probabilities\n"));

  for (y=0; y <=2; y++)
    if (prob[y][0] + prob[y][1] > 1.0)
      return(Error(3,"segmRandom: Bad probabilities\n"));

  if ((lpv<0) OR (lpv>255) OR (rpv<0) OR (rpv>255))
    return(Error(4,"segmRandom: Bad pixel values\n"));

  srandom(time(0));
  xsize = Ixsize(band);
  xval  = xsize/2;

  FOR (y=1; y LE Iysize(band); y++)
    for (x=1; x LT xval; x++) band[y][x] = lpv;
    for (x=xval; x LE xsize; x++) band[y][x] = rpv;

    ran2 = (random()%1000000)/1000000.0;
    IF (ran2 LT prob[pre][0])
      xval--;
      pre=0;
    ELSEIF (ran2 GT (1-prob[pre][1]))
      xval++;
      pre=2;
    ELSE
      pre=1;
    ENDIF
    if (xval LT 1) xval=1;
    if (xval GT xsize) xval=xsize;
  ENDFOR
  return(0);
}
