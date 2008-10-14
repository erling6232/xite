

/*

________________________________________________________________

        classifyNearest
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
#include <xite/classification.h>
#ifdef HAVE_STDIO_H
#  include <stdio.h>
#endif
#include <stdlib.h>
#include <xite/blab.h>
#include <xite/histo.h>
#ifdef HAVE_MALLOC_H
# include <malloc.h>
#endif
#include <xite/message.h>
#include <xite/readarg.h>

#ifndef MIN
# define MIN(a,b) ((a)<(b) ? a : b)
#endif

#define EUCLID_METRIC 1
#define CITY_METRIC   2
#define CHESS_METRIC  3

/*F:classifyNearest*

________________________________________________________________

		classifyNearest
________________________________________________________________

Name:		classifyNearest - classify to nearest class

Syntax:         | #include <xite/classification.h>
		|
                | int classifyNearest( IMAGE input, IBAND oldres,
                |    IBAND newres, float* means[], int k,
                |    int metric, int* changes );

Description:    Classifies the input image to the nearest of the
                specified class means.

		'input' may be multi band image.
		'oldres' is previous classification result.
		'newres' is the classification result.
		'means' refers to the class mean vectors.
		'k' is the number of classes.
		'metric' must be one of
		|   1 => Euclidian distance
		|   2 => City block distance
		|   3 => Chess distance
		'changes' is the number of changes from previous
		classification.

		Identical arguments may be given for 'oldres' and 'newres'.

Restrictions:   'input', 'oldres' and 'newres' must have pixel type unsigned
                byte. 'means' must be an array of pointers to arrays
		of floats, indexable on the interval
		| means[0..k-1][1..Inbands(input)].

Return value:   | 0 => ok
                | 1 => bad pixel type in input image
		| 2 => bad pixel type in oldres
		| 3 => bad pixel type in newres
                | 4 => bad k value (less than 2)
                | 5 => unknown metric

Author:		Tor Lønnestad, BLAB, Ifi, UiO.
Id:             $Id$
________________________________________________________________

*/

int classifyNearest(IMAGE input, IBAND oldres, IBAND newres, float **means, int k, int metric, int *changes)
{
  int x, y, xsize, ysize, nk, bn, nbands, class;
  float dist, mindist, diff;

  nbands = Inbands(input);
  for (bn=1; bn LE nbands; INC bn)
    if (Ipixtyp(input[bn]) NE Iu_byte_typ)
      return(Error(1, "%s%s\n",
		   "classifyNearest: Input pixel type must be ",
		   "unsigned byte."));
  if (Ipixtyp(oldres) NE Iu_byte_typ)
      return(Error(2, "%s%s\n",
		   "classifyNearest: 'oldres' pixel type must be ",
		   "unsigned byte."));
  if (Ipixtyp(newres) NE Iu_byte_typ)
      return(Error(3, "%s%s\n",
		   "classifyNearest: 'newres' pixel type must be ",
		   "unsigned byte."));
  if (k LT 2)
    return(Error(4, "%s\n",
		 "classifyNearest: Bad value for the number of classes."));
  if ((metric LT EUCLID_METRIC) OR (metric GT CHESS_METRIC))
    return(Error(5, "%s\n",
		 "classifyNearest: Unknown metric."));

  xsize = MIN(Ixsize(oldres), Ixsize(newres));
  ysize = MIN(Iysize(oldres), Iysize(newres));
  for (bn=1; bn LE nbands; INC bn) {
    if (Ixsize(input[bn]) LT xsize) xsize = Ixsize(input[bn]);
    if (Iysize(input[bn]) LT ysize) ysize = Iysize(input[bn]);
  }
  *changes = 0;

  for (y=1; y LE ysize; y++) {
    for (x=1; x LE xsize; x++) {
      mindist = 100000000.0;
      for (nk=0; nk LT k; INC nk) {
	dist = 0.0;
	switch (metric) {
	  
	case EUCLID_METRIC:
	  for (bn=1; bn LE nbands; INC bn) {
	    diff = (float)input[bn][y][x] - means[nk][bn];
	    dist += diff*diff;
	  }
	  break;
	  
	case CITY_METRIC:
	  for (bn=1; bn LE nbands; INC bn) {
	    diff = (float)input[bn][y][x] - means[nk][bn];
	    if (diff LT 0) diff = -diff;
	    dist += diff;
	  }
	  break;
	  
	case CHESS_METRIC:
	  for (bn=1; bn LE nbands; INC bn) {
	    diff = (float)input[bn][y][x] - means[nk][bn];
	    if (diff LT 0) diff = -diff;
	    if (diff GT dist) dist = diff;
	  }
	  break;
	  
	} /* switch() */
	if (dist LT mindist) { mindist = dist; class = nk; }
      } /* all k*/
      if (oldres[y][x] NE class) INC *changes;
      newres[y][x] = class;
    }  /* all x */
  }  /* all y */
  return(0);
}  /* classifyNearest */




/*F:classMeans*

________________________________________________________________

		classMeans
________________________________________________________________

Name:		classMeans - calculate class means

Syntax:         | #include <xite/classification.h>
		|
                | int classMeans( IMAGE input,
                |    IBAND classification, float* means[],
                |    int k );

Description:	Calculates new class means.

                'input' may be multi band.
		'classification' is the input classification result.
		'means' refers to the output mean vectors.
		'k' is the number of classes.

		'means' must be indexable on the interval
		| means[0..k-1][1..Inbands(input)]
		Classes that don''t occur at all are given mean values -1000.0.

Restrictions:   'input' and 'classification' must have pixel type unsigned
                byte.

Return value:	| 0 => ok
                | 1 => bad pixel type in input image
		| 2 => bad pixel type in classification
                | 3 => bad k value (less than 1)

Author:		Tor Lønnestad, BLAB, Ifi, UiO
________________________________________________________________

*/

int classMeans(IMAGE input, IBAND classification, float **means, int k)
{
  histogram h;
  int bn, nbands, x, y, nk, xsize, ysize;
  
  nbands = Inbands(input);
  for (bn=1; bn LE nbands; INC bn)
    if (Ipixtyp(input[bn]) NE Iu_byte_typ)
      return(Error(1, "%s\n",
		   "classMeans: Input pixel type must be unsigned byte."));
  if (Ipixtyp(classification) NE Iu_byte_typ)
    return(Error(2, "%s%s\n",
		 "classMeans: Classification pixel type must be ",
		 "unsigned byte."));
  if (k LT 1)
    return(Error(3, "%s\n",
		 "classMeans: Bad value for the number of classes."));
  
  xsize = Ixsize(classification);
  ysize = Iysize(classification);
  for (bn=1; bn LE nbands; INC bn) {
    if (Ixsize(input[bn]) LT xsize) xsize = Ixsize(input[bn]);
    if (Iysize(input[bn]) LT ysize) ysize = Iysize(input[bn]);
  }
  
  mkHisto(classification, h);
  for (nk=0; nk LT k; INC nk)
    for (bn=1; bn LE nbands; INC bn) 
      means[nk][bn] = 0.0;
  for (y=1; y LE ysize; INC y)
    for (x=1; x LE xsize; INC x)
      for (bn=1; bn LE nbands; INC bn)
	means[classification[y][x]][bn] += (float)input[bn][y][x];
  for (nk=0; nk LT k; INC nk)
    for (bn=1; bn LE nbands; INC bn)
      if (h[nk]) means[nk][bn] = means[nk][bn]/(float)h[nk];
      else means[nk][bn] = -1000.0;
  
  return(0);
  
}  /*  classMeans  */
