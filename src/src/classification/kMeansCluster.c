

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
#include XITE_STDIO_H
#include <stdlib.h>
#include <xite/blab.h>
#include <xite/histo.h>
#include XITE_MALLOC_H
#include <xite/message.h>
#include <xite/readarg.h>

#ifndef MIN
# define MIN(a,b) ((a)<(b) ? a : b)
#endif

#define EUCLID_METRIC 1
#define CITY_METRIC   2
#define CHESS_METRIC  3

#ifndef MAIN

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

#ifndef FUNCPROTO
int classifyNearest(input, oldres, newres, means, k, metric, changes)
   IMAGE input;
   IBAND oldres, newres;
   float *means[];
   int k, metric;
   int *changes;
#else /* FUNCPROTO */
int classifyNearest(IMAGE input, IBAND oldres, IBAND newres, float **means, int k, int metric, int *changes)
#endif /* FUNCPROTO */
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

#ifndef FUNCPROTO
int classMeans(input, classification, means, k)
IMAGE input;
IBAND classification;
float *means[];
int k;
#else /* FUNCPROTO */
int classMeans(IMAGE input, IBAND classification, float **means, int k)
#endif /* FUNCPROTO */
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

#endif /* not MAIN */



/*P:kMeansCluster*

________________________________________________________________

		kMeansCluster
________________________________________________________________

Name:		kMeansCluster - Kmeans clustering

Syntax:		kMeansCluster <inimage> <outimage> <k> [<metric>]

Description:    Classifies the multi band input image iteratively
                to the nearest cluster mean. Algorithm:

                | Ask user for k mean vectors
		| REPEAT
		|   classify input image to nearest mean
		|   Recalculate class means
		|   Report classification result
		| UNTIL user is satisfied OR no changes

Arguments:      &inimage
                Multi band input image.

		&outimage
		Single band classification result.

		&k
		The number of classes.

		&metric
		One of
		&&1
		Euclidian distance.
		&&2
		City block distance.
		&&3
		Chess distance.

		-&&
		Default value 1.

Restrictions:	All bands of 'inimage' must have pixel type unsigned byte.
                For every class, a mean vector with n float
		components must be given, where n = number of
		bands in input image. 'k' must be between 2 and 100.

See also:       classifyNearest(3), classMeans(3)

Author:		Tor Lønnestad, BLAB, Ifi, UiO

Examples:       | kMeansCluster landsat.img classes.img 5
                | kMeansCluster landsat.img classes.img 5 3
________________________________________________________________

*/

#ifdef MAIN

#ifndef FUNCPROTO
int main(argc,argv)
int argc;
char *argv[];
#else /* FUNCPROTO */
int main(int argc, char **argv)
#endif /* FUNCPROTO */
{
   IMAGE input, output;
   int bn, nk, k, nbands, metric, changes, iteration, more, limit;
   float* means[100];
   histogram h;
   char *args;

   Iset_message(TRUE);
   Iset_abort(TRUE);
   InitMessage(&argc, argv, xite_app_std_usage_text(
    "Usage: %s <inimage> <outimage> <k> [<metric>] \n"));

   if (argc == 1) Usage(1, NULL);
   args = argvOptions(argc, argv);
   if (argc != 4 && argc != 5) Usage(1, "Illegal number of arguments.\n");

   k = atoi(argv[3]);
   if (argc GE 5) metric = atoi(argv[4]); else metric = EUCLID_METRIC;
   if ((k LT 2) OR (k GT 100))
     Error(2, "k must be between 2 and 100\n");

   input   = Iread_image(argv[1]);
   nbands  = Inbands(input);

   output  = Imake_image(1, "Classification result",
			 Iu_byte_typ, Ixsize(input[1]), Iysize(input[1]));

   for (nk=0; nk LT k; INC nk) {
     means[nk] = (float*)malloc((nbands+1)*sizeof(float));
     printf("Give mean-vector for class %d : \n", nk);
     for (bn=1; bn LE nbands; INC bn)
       scanf("%f", &means[nk][bn]);
   }

   iteration = 0; limit = 1;
   LOOP
     INC iteration;
     printf("Classifying iteration %d...\n", iteration);
     if (classifyNearest(input, output[1], output[1], means, k,
			 metric, &changes))
       Warning(2, "Error in classifyNearest.\n");

     printf("Classification finished with %d changes\n", changes);
     EXITIF (changes EQ 0);
     printf("Calculating new class means...\n");
     if (classMeans(input, output[1], means, k))
       Warning(2, "Error in classMeans.\n");

     mkHisto(output[1],h);
     printf("New class means and number of occurrences:\n\n");
     printf("            ");
     for (bn=1; bn LE nbands; INC bn) printf(" Band %d  ",bn);
     printf(" # pixels\n");
     for (nk=0; nk LT k; INC nk) {
       printf("Class %2d : ",nk);
       for (bn=1; bn LE nbands; INC bn) printf(" %7.2f ",means[nk][bn]);
       printf("  %7d\n",h[nk]);
     }
     printf("\n\n");

     IF (iteration EQ limit)
       printf("How many more iterations ?");
       scanf("%d",&more);
       EXITIF (more LE 0);
       limit = iteration + more;
     ENDIF;
   ENDLOOP;

   Ihistory(output, argv[0], args);
   Iwrite_image(output, argv[2]);

   return(0);
}

#endif /* MAIN */
