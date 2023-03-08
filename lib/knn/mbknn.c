
/*

________________________________________________________________

        mbknn
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
#include <xite/knn.h>
#include XITE_STDIO_H
#include <stdlib.h>
#include <xite/blab.h>
#include <xite/message.h>
#include <xite/readarg.h>

#define MTMAX 100000
#define SQR(a) ((a)*(a))

/*F:mbknn*

________________________________________________________________

		mbknn
________________________________________________________________

Name:		mbknn - multi band k nearest neighbour noise removal

Syntax:         | #include <xite/knn.h>
		|
                | int mbknn( IMAGE i1, IMAGE i2, int n, int k );

Description:    'mbknn' performs multi band 'k' nearest neighbour
                noise removal. All bands are filtered 
                simultaneously. Window size 'n' x 'n', max 31.
                If 'n' is even, 'n'+1 is used. 'k' is the number of
                neighbours to average over.

		Euclidian distance is used as distance measure.

Restrictions:	The bands of 'i1' and 'i2' must have pixel type unsigned byte.
                All bands in both images must have equal size.

Return value:   | 0 => ok
                | 1 => bad input pixel type
                | 2 => bad output pixel type
                | 3 => bad n value (less than 1 or greater than 31)
                | 4 => bad k value (less than 1 or g.t. n*n-1)
                | 5 => different number of bands in i1 and i2.

See also:       mbknn(1), knn(3), kncn(3), mbkncn(3)

Author:		Tor Lønnestad, BLAB, Ifi, UiO

Id:             $Id$
________________________________________________________________

*/


int mbknn(IMAGE i1, IMAGE i2, int n, int k)
{
  int v[961]; /* space for 31*31 local vindow */
  int x, y, nhalf, nsq, i, j, l, ki, min, sum, bn, nbands;
  int xsize, ysize;

  nbands = Inbands(i1);
  if (nbands NE Inbands(i2))
    return(Error(5, "mbknn: different number of bands in images.\n"));

  xsize = Ixsize(i1[1]);
  ysize = Iysize(i1[1]);
  FOR (bn=1; bn LE Inbands(i1); bn++)
    if (Ipixtyp(i1[bn]) NE Iu_byte_typ)
      return(Error(1, "mbknn: Input pixel type must be unsigned byte.\n"));
    if (Ipixtyp(i2[bn]) NE Iu_byte_typ)
      return(Error(2, "mbknn: Output pixel type must be unsigned byte.\n"));

    if (Ixsize(i1[bn]) GT xsize) xsize = Ixsize(i1[bn]);
    if (Iysize(i1[bn]) GT ysize) ysize = Iysize(i1[bn]);
    if (Ixsize(i2[bn]) GT xsize) xsize = Ixsize(i2[bn]);
    if (Iysize(i2[bn]) GT ysize) ysize = Iysize(i2[bn]);
  ENDFOR;
  if ((n LT 1) OR (n GT 31)) return(Error(3, "mbknn: bad n value.\n"));
  if (k LT 1) return(Error(4, "mbknn: bad k value.\n"));

  nhalf = (n-1)/2;  
  n = (2*nhalf)+1;
  nsq = SQR(n);
  if (k GT nsq-1) return(Error(4, "mbknn: bad k value.\n"));

  FOR (y=nhalf+1; y LE ysize-nhalf; y++)
    FOR (x=nhalf+1; x LE xsize-nhalf; x++)

      /* initialize vector v */
      l = -1;
      FOR (j=y-nhalf; j LE y+nhalf; ++ j) 
        FOR (i=x-nhalf; i LE x+nhalf; ++ i)
          v[++ l] = 0;
          for (bn = 1; bn LE Inbands(i1); ++ bn)
            v[l] += SQR(i1[bn][y][x]-i1[bn][j][i]);
          v[l] = (int)sqrt((float)v[l]);
        ENDFOR;
      ENDFOR;

      /* sort:all selected pixels are set to MTMAX, More Than MAX dist. */
      v[((n*n)-1)/2] = MTMAX; /* center pixel */
      FOR (ki=1; ki LE k; ++ ki)
        min = 0;
        for (i=1; i LT nsq; ++ i)
          if (v[i] LT v[min]) min = i;
        v[min] = MTMAX;
      ENDFOR;

      /* In each band, average */
      FOR (bn=1; bn LE Inbands(i1); ++ bn)
        sum = 0;
        for (min=0; min LT nsq; ++ min)
          if (v[min] EQ MTMAX) 
            sum += i1[bn][y-nhalf+(min/n)][x-nhalf+(min MOD n)];
        i2[bn][y][x] = (int)(((float)sum + 0.5)/(k+1));
      ENDFOR;  /* all bn */
    ENDFOR;  /* all x */
  ENDFOR; /* all y */

  return(0);

} /* procedure mbknn */
