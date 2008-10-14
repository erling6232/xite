

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

int main(int argc, char **argv)
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
