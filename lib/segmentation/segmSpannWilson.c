
/*C*

________________________________________________________________

        segmSpannWilson
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
#include <xite/segmentation.h>
#include <xite/blab.h>
#include <xite/histo.h>
#include <xite/message.h>
#include <xite/minmax.h>
#include <xite/pyramid.h>
#include <xite/readarg.h>
#include <xite/thresMl.h>

#ifndef MIN
# define MIN(a,b) (((a)<(b)) ? (a) : (b))
#endif

/*S:thres2map*

________________________________________________________________

		thres2map
________________________________________________________________

Name:		thres2map - convert thresholds to trandformation table
Syntax:		| static void thres2map(thres, map, nt)
                | histogram thres, map;
                | int nt;
Description:	convert vector 'thres' with 'nt' thresholds to 'map',
                a transformation table.
Restrictions:
Return value:
Author:		Tor L|nnestad, BLAB, ifi, UiO
________________________________________________________________

*/

static void thres2map(int *thres, int *map, int nt)
{
  int i, j, low, high;
  low = 0;
  FOR (i=0; i LT nt; i++)
    high = thres[i];
    for (j=low; j LE high; j++) map[j] = (low+high)/2;
    low = high+1;
  ENDFOR;
  high = 255;
  for (j=low; j LE high; j++) map[j] = (low+high)/2;
}


/*S:sameAsFather*

________________________________________________________________

		sameAsFather
________________________________________________________________

Name:		sameAsFather - copy pixel values from 'father' to 'son'
Syntax:		| static BiffStatus sameAsFather(father, son)
                | IBAND father, son;
Description:	copy pixel values from 'father' to 'son', where the 'son'
                is twice as large in x/ysize
Restrictions:
Return value:
Author:		Tor L|nnestad, BLAB, ifi, UiO
________________________________________________________________

*/

static BiffStatus sameAsFather(IBAND father, IBAND son)
{
  int x, y, xx, yy=1, xsize, ysize, sonXsize, sonYsize, pixelval;

  xsize = Ixsize(father);
  ysize = Iysize(father);
  sonXsize = Ixsize(son);
  sonYsize = Iysize(son);
  if (sonXsize LT 2*xsize) return(Error(2,"Internal fatal error\n"));
  if (sonYsize LT 2*ysize) return(Error(2,"Internal fatal error\n"));
  FOR (y=1; y LE ysize; y++)
    xx=1;
    FOR (x=1; x LE xsize; x++)
      pixelval = father[y][x];
      son[yy++][xx] = pixelval;
      son[yy][xx++] = pixelval;
      son[yy--][xx] = pixelval;
      son[yy][xx++] = pixelval;
    ENDFOR;
    if (xx LE sonXsize) { son[yy++][xx] = pixelval; son[yy++][xx] = pixelval; }
    else yy += 2;
  ENDFOR;
  if (yy LE sonYsize)
    for (xx=1; xx LE sonXsize; xx++)
      son[yy][xx] = son[yy-1][xx];

  return(Iok);
}



/*S:createBoundary*

________________________________________________________________

		createBoundary
________________________________________________________________

Name:		createBoundary - create band describing the boundary in input
Syntax:		| static BiffStatus createBoundary(inband, outband)
                | IBAND inband, outband;
Description:	in the output band, 0 means not boundary, 1 means boundary
Restrictions:
Return value:
Author:		Tor L|nnestad, BLAB, ifi, UiO
________________________________________________________________

*/
#define EQUAL(x1,y1,x2,y2) (inband[y1][x1] == inband[y2][x2])

static BiffStatus createBoundary(IBAND inband, IBAND outband)
{
  int x, y, xsize, ysize;

  xsize = Ixsize(inband);
  ysize = Iysize(inband);
  if (Ixsize(outband) NE xsize) return(Error(2,"Internal fatal error\n"));
  if (Iysize(outband) NE ysize) return(Error(2,"Internal fatal error\n"));

  /* upper, left corner */
  outband[1][1] = 0;
  /* upper line */
  for (x=2; x LT xsize; x++)
    outband[1][x] = (EQUAL(x,1,x-1,1) AND EQUAL(x,1,x+1,1)) ? (0) : (1);
  /* upper, right corner */
  outband[1][xsize] = 0;

  /* for most lines.. */
  FOR (y=2; y LT ysize; y++)
    /* leftmost point */
    outband[y][1] = (EQUAL(1,y,1,y-1) AND EQUAL(1,y,1,y+1)) ? (0) : (1);
    /* general point */
    for (x=2; x LT xsize; x++)
      outband[y][x] = (EQUAL(x,y,x-1,y-1) AND EQUAL(x,y,x,y-1)   AND
		       EQUAL(x,y,x+1,y-1) AND EQUAL(x,y,x-1,y)   AND
		       EQUAL(x,y,x+1,y)   AND EQUAL(x,y,x-1,y+1) AND
		       EQUAL(x,y,x,y+1)   AND EQUAL(x,y,x+1,y+1))
	? (0) : (1);
    /* rightmost point */
    outband[y][xsize] = (EQUAL(xsize,y,xsize,y-1) AND EQUAL(xsize,y,xsize,y+1))
      ? (0) : (1);
  ENDFOR

  /* bottom, left corner */
  outband[ysize][1] = 0;
  /* bottom line */
  for (x=2; x LT xsize; x++)
    outband[ysize][x] = (EQUAL(x,ysize,x-1,ysize) AND EQUAL(x,ysize,x+1,ysize))
      ? (0) : (1);
  /* bottom, right corner */
  outband[ysize][xsize] = 0;

  return(Iok);
}





/*S:enlargeBoundary*

________________________________________________________________

		enlargeBoundary
________________________________________________________________

Name:		enlargeBoundary - enlarge the boundary created by createBoundary
Syntax:		| static void enlargeBoundary(inband, outband)
                | IBAND inband, outband;
Description:	enlarge the input boundary by all its 8-neighbors
Restrictions:
Return value:
Author:		Tor L|nnestad, BLAB, ifi, UiO
________________________________________________________________

*/

static void enlargeBoundary(IBAND inband, IBAND outband)
{
  maxarea(inband, outband, 3, 3);
}




/*F:reclassifyBoundary*

________________________________________________________________

		reclassifyBoundary
________________________________________________________________

Name:		reclassifyBoundary - reclassify the boundary pixels

Syntax:         | #include <xite/segmentation.h>
		|
                | BiffStatus reclassifyBoundary( IBAND orig,
                |    IBAND result, IBAND boundary,
                |    histogram map );

Description:    reclassify the boundary pixels. 'boundary' describes
                what is boundary pixels, 'orig' have the original pixel
		values, map is the classification map, and the result
		goes into 'result'.

Restrictions:   'orig', 'result' and 'boundary' must have pixel type
                unsigned byte.

Return value:   | 0 : Ok
                | 1 : Bad input pixel type
		| 2 : Bad result pixel type
		| 3 : Bad boundary pixel type

Author:		Tor Lønnestad, BLAB, Ifi, UiO
________________________________________________________________

*/

BiffStatus reclassifyBoundary(IBAND orig, IBAND result, IBAND boundary, int *map)
{
  int x, y, xsize, ysize;

  if (Ipixtyp(orig) != Iu_byte_typ)
    return(Error(1, "%s%s\n",
		 "reclassifyBoundary: Input pixel type must be ",
		 "unsigned byte.\n"));
  if (Ipixtyp(orig) != Iu_byte_typ)
    return(Error(2, "%s%s\n",
		 "reclassifyBoundary: Result pixel type must be ",
		 "unsigned byte.\n"));
  if (Ipixtyp(orig) != Iu_byte_typ)
    return(Error(3, "%s%s\n",
		 "reclassifyBoundary: Boundary pixel type must be ",
		 "unsigned byte.\n"));

  xsize = Ixsize(orig);
  ysize = Iysize(orig);

  if (Ixsize(result) NE xsize)
    return(Error(4, "%s%s\n",
		 "reclassifyBoundary: orig and result bands have unequal ",
		 "horizontal size."));
  if (Iysize(result) NE ysize)
    return(Error(4, "%s%s\n",
		 "reclassifyBoundary: orig and result bands have unequal ",
		 "vertical size."));

  if (Ixsize(boundary) NE xsize)
    return(Error(4, "%s%s\n",
		 "reclassifyBoundary: orig and boundary bands have unequal ",
		 "horizontal size."));

  if (Iysize(boundary) NE ysize)
    return(Error(4, "%s%s\n",
		 "reclassifyBoundary: orig and boundary bands have unequal ",
		 "vertical size."));

  for (y=1; y LE ysize; y++)
    for (x=1; x LE xsize; x++)
      if (boundary[y][x]) result[y][x] = map[orig[y][x]];

  return(Iok);
}



/*F:segmSpannWilson*

________________________________________________________________

		segmSpannWilson
________________________________________________________________

Name:		segmSpannWilson - segmentation by the Spann and Wilson method

Syntax:         | #include <xite/segmentation.h>
		|
                | BiffStatus segmSpannWilson( IBAND inband,
                |    IBAND outband, int n, int m, int method );

Description:    Performs segmentation of the input band into the
		output band. The algorithm has three steps:

		&Quad-tree smoothing

		A quad-tree is created, and pixels are averaged 4 by 4
		up in the quad-tree. 'n' quad-tree levels are created,
		including the bottom level, which is 'inband'.

		&Local centroid clustering

		The topmost quad-tree level is clustered by moving histogram
		mass to the centroid (mean or median) of a local window moved
		over the histogram. Applied iteratively, until stable. See
		'thresMlCentroid'.
		
		&Boundary estimation

		The classification result is copied down the quad-tree, but at
		every level, all pixels close to a differently classified pixel
		are reclassified to determine the border more precisely.

Arguments: 	&inband
                Input band to be segmented.

		&outband
		The result of the segmentation.

		&n
		Levels in the quad-tree (must be at least two).

		&m
		Size of histogram window, like 10-20.

		&method
		Local centriod clustering: 0 -> mean, 1 -> median.

		-&
		Note that if you specify histogram window size 'm'=0, the 
		algorithm will try to find an 'm'-value where the clustering
		result is relatively stable over a range of 'm'-values.

Restrictions:   'inband' and 'outband' must have pixel type unsigned byte.
                'outband' should have the same size as 'inband'. Only the
		largest common rectangle of 'inband' and 'outband' is handled.

See also:	segmSpannWilson(1), thresMlCentroid(3), segmRandom(3)

Reference:      &M. Spann and R. Wilson,
                "A Quad-Tree Approach to Image Segmentation which Combines
		Statistical and Spatial Information"
		Patteren Recognition, Vol 18, Nos 3/4, pp 257-269, 1985

Return value:   | 0 => ok
		| 2 => error: bad inband pixel type
		| 3 => error: bad outband pixel type
		| 4 => error: bad n value
		| 5 => error: bad m value
		| 6 => error: bad method value

Author:		Tor Lønnestad, BLAB, Ifi, UiO

Id: 		$Id$
________________________________________________________________

*/


BiffStatus segmSpannWilson(IBAND inband, IBAND outband, int n, int m, int method)
{
  IMAGE img1, img2, img3, img4;
  histogram histo, thres;
  int i, xsize, ysize, nt;

  if (Ipixtyp(inband) NE Iu_byte_typ)
    return(Error(2,
	"segmSpannWilson: Input pixel type must be unsigned byte.\n"));
  if (Ipixtyp(outband) NE Iu_byte_typ)
    return(Error(3,
	 "segmSpannWilson: Output pixel type must be unsigned byte.\n"));

  if (n LT 2) return(Error(4, "segmSpannWilson: bad 'n' argument\n"));
  if ((m LT 0) OR (m GT 127))
    return(Error(5, "segmSpannWilson: bad 'm' argument\n"));
  if ((method LT 0) OR (method GT 1))
    return(Error(6, "segmSpannWilson: bad 'method' argument\n"));

  img1    = Init_image(n, "Temporary pyramid image");
  img1[1] = inband;
  xsize   = MIN(Ixsize(inband), Ixsize(outband));
  ysize   = MIN(Iysize(inband), Iysize(outband));

  /* Part one: quad-tree smoothing */
  Info(0, "\nQuad-tree smoothing...\n");
  FOR (i=2; i LE n; i++) /* up the pyramid.. */
    Info(0, "  level %d...\n", i);
    xsize /= 2;
    ysize /= 2;
    if ((xsize == 0) OR (ysize == 0))
      return(Error(4, "segmSpannWilson: bad 'n' argument\n"));
    img1[i] = Imake_band(Iu_byte_typ, xsize, ysize);
    pyramidAver2(img1[i-1], img1[i]);
  ENDFOR;

  /* Part two: Local centriod clustering */
  Info(0, "Local centriod clustering...\n");
  img2 = Init_image(n, "temporary pyramid image");
  img2[1] = outband;
  for (i=2; i LE n; i++)
    img2[i] = Imake_band(Iu_byte_typ, Ixsize(img1[i]), Iysize(img1[i]));
  mkHisto(img1[n], histo);
  thresMlCentroid(histo, thres, m, method, &nt);
  thres2map(thres, histo, nt);
  histoTransf(img1[n], img2[n], histo);

  /* Part three: Boundary estimation */ 
  Info(0, "Boundary estimation...\n");
  img3 = Icopy_init(img1);
  img4 = Icopy_init(img1);
  FOR (i=n-1; i GE 1; i--)  /* down the pyramid.. */
    Info(0, "  level %d...\n", i);
    sameAsFather(img2[i+1], img2[i]);
    createBoundary(img2[i], img3[i]);
    enlargeBoundary(img3[i], img4[i]);
    reclassifyBoundary(img1[i], img2[i], img4[i], histo);
  ENDFOR;
  /* free */
  img1[1] = NULL;
  Idel_image(&img1);
  img2[1] = NULL;
  Idel_image(&img2);
  Idel_image(&img3);
  Idel_image(&img4);

  return(Iok);
}
