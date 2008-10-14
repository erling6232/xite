

/*

________________________________________________________________

        isoCluster
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


#include <stdlib.h>
#include <math.h>
#include <xite/includes.h>
#include <xite/biff.h>
#ifdef HAVE_STDIO_H
#  include <stdio.h>
#endif
#ifdef HAVE_SYS_TYPES_H
#  include <sys/types.h>
#endif
#include <xite/blab.h>
#include <xite/histo.h>
#include <xite/classification.h>
#include <xite/message.h>
#include <xite/readarg.h>

#define EUCLID_METRIC 1
#define CITY_METRIC   2
#define CHESS_METRIC  3


/*L* 

________________________________________________________________

		classMeanDist
________________________________________________________________

Name:		classMeanDist
Syntax:		
Description:	calculate distance between two class means
Return value:	the distance
Author:		Tor L|nnestad
Id:             $Id$
________________________________________________________________

*/


static float classMeanDist(float **means, int nbands, int c1, int c2, int metric)
{
   int bn;
   float diff, dist=0.0;
   
   switch (metric) {
   case EUCLID_METRIC:
     for (bn=1; bn LE nbands; INC bn) {
       diff = means[c1][bn] - means[c2][bn];
       if (diff LT 0) diff = -diff;
       dist += diff*diff;
     }
     dist = (float)sqrt((double)dist);
     break;
     
   case CITY_METRIC:
     for (bn=1; bn LE nbands; INC bn) {
       diff = means[c1][bn] - means[c2][bn];
       if (diff LT 0) diff = -diff;
       dist += diff;
     }
     break;
     
   case CHESS_METRIC:
     for (bn=1; bn LE nbands; INC bn) {
       diff = means[c1][bn] - means[c2][bn];
       if (diff LT 0) diff = -diff;
       if (diff GT dist) dist = diff;
     }
     break;
   }
   
   return(dist);
}  /*  classMeanDist  */



/*L*

________________________________________________________________

		mergeTwoClasses
________________________________________________________________

Name:		mergeTwoClasses
Syntax:		
Description:	merge Two classes c1 and c2
Return value:	
Author:		Tor L|nnestad
________________________________________________________________

*/


static void mergeTwoClasses(IBAND output, int c1, int c2)
{
   int x, y, xsize, ysize;
   xsize = Ixsize(output);
   ysize = Iysize(output);
   for (y=1; y LE ysize; INC y)
     for (x=1; x LE xsize; INC x)
       if (output[y][x] GT c2) --output[y][x];
       else if (output[y][x] EQ c2) output[y][x] = c1;
}  /*  mergeTwoClasses  */




/*L*

________________________________________________________________

		mergeClasses
________________________________________________________________

Name:		mergeClasses
Syntax:		
Description:	merge closest pair of classes closer than mindist
Return value:	number of merges (0 or 1)
Author:		Tor L|nnestad
________________________________________________________________

*/

static int mergeClasses(IBAND output, float **means, int *k, int nbands, int metric, float mindist)
{
   int c1, c2, C1, C2;
   float dist, md = 1000.0;
   char c[2];

   for (c1=0; c1 LT (*k)-1; INC c1) {
     for (c2=c1+1; c2 LT *k; INC c2) {
       if ((size_t)means[c1] AND (size_t)means[c2]) {
         dist = classMeanDist(means, nbands, c1, c2, metric);
         if (dist LT md) {md = dist; C1 = c1; C2 = c2;}
       }
     }
   }

   if (md LT mindist) {
     printf("Classes %d and %d have distance %.2f . Merge (y/n) ? ",
	    C1, C2, md);
     scanf("%1s", c);
     if ((*c EQ 'y') OR (*c EQ 'Y')) {
       printf("Merging classes %d and %d.\n", C1, C2);
       mergeTwoClasses(output, C1, C2);
       free(means[*k]);
       (*k)--;
       return(1);
     }
   }
   
   return(0);
}  /*  merge  */





/*L*

________________________________________________________________

		classStdev
________________________________________________________________

Name:		classStdev
Syntax:		
Description:	
Return value:	
Author:		Tor L|nnestad
________________________________________________________________

*/


static void classStdev(IMAGE input, IBAND classif, float **means, int k, float *stdevs, int metric)
{
   int x, y, xsize, ysize, nbands, bn, nk;
   float diff, dist;
   histogram h;

   nbands = Inbands(input);
   xsize = Ixsize(classif);
   ysize = Iysize(classif);

   for (nk=0; nk LT k; INC nk) stdevs[nk] = 0.0;

   for (y=1; y LE ysize; INC y) {
     for (x=1; x LE xsize; INC x) {
       nk = classif[y][x];
       dist = 0.0;
       switch (metric) {
       case EUCLID_METRIC:
	 for (bn=1; bn LE nbands; INC bn) {
	   diff = (float)input[bn][y][x] - means[nk][bn];
	   if (diff LT 0.0) diff = -diff;
	   dist += diff*diff;
	 }
         break;
	 
       case CITY_METRIC:
	 for (bn=1; bn LE nbands; INC bn) {
	   diff = (float)input[bn][y][x] - means[nk][bn];
	   if (diff LT 0.0) diff = -diff;
	   dist += diff;
	 }
	 dist = dist*dist;
         break;
	 
       case CHESS_METRIC:
	 for (bn=1; bn LE nbands; INC bn) {
	   diff = (float)input[bn][y][x] - means[nk][bn];
	   if (diff LT 0.0) diff = -diff;
	   if (diff GT dist) dist = diff;
	 }
	 dist = dist*dist;
         break;
       }
       stdevs[nk] += dist;
     }
   }
   
   mkHisto(classif, h);
   for (nk=0; nk LT k; INC nk) {
     if (h[nk] EQ 0) stdevs[nk] = 0.0;
     else stdevs[nk] = (float)sqrt((double)stdevs[nk]/(double)h[nk]);
   }
}  /*  classStdev  */




/*L*

________________________________________________________________

		splitClass
________________________________________________________________

Name:		splitClass
Syntax:	
Description:	
Return value:	
Author:		Tor L|nnestad
________________________________________________________________

*/


static void splitClass(IMAGE input, IBAND classif, int splitcl, float **means, int *kp)
{
   float splitval, diff, *stdevs;
   int x, y, xsize, ysize, nbands, bn, splitbn;

   nbands = Inbands(input);
   xsize = Ixsize(classif);
   ysize = Iysize(classif);
   means[*kp] = (float*)malloc((nbands+1)*sizeof(float));
   stdevs = (float*)malloc((nbands+1)*sizeof(float));
   for (bn=1; bn LE nbands; INC bn) stdevs[bn] = 0.0;

   FOR (y=1; y LE ysize; INC y)
     FOR (x=1; x LE xsize; INC x)
       IF (classif[y][x] EQ splitcl)
         FOR (bn=1; bn LE nbands; INC bn)
           diff = (float)input[bn][y][x] - means[*kp][bn];
           stdevs[bn] += diff*diff;
         ENDFOR;
       ENDIF;
     ENDFOR;
   ENDFOR;

   splitbn = 1;
   for (bn=2; bn LE nbands; INC bn)
     if (stdevs[bn] GT stdevs[splitbn]) splitbn = bn;

   splitval = means[splitcl][splitbn];
   FOR (y=1; y LE ysize; INC y)
     FOR (x=1; x LE xsize; INC x)
       if (classif[y][x] EQ splitcl)
         if ((float)input[splitbn][y][x] GT splitval)
           classif[y][x] = *kp;
     ENDFOR;
   ENDFOR;
   ++(*kp);
}  /*  splitClass  */






/*L* 

________________________________________________________________

		splitClasses
________________________________________________________________

Name:		splitClasses
Syntax:		
Description:	
Return value:	1 if any classes split, 0 otherwise
Author:		Tor L|nnestad
________________________________________________________________

*/

static int splitClasses(IMAGE input, IBAND classif, float **means, int *kp, float maxstdev, int metric)
{
   int i, maxc;
   float maxstd;
   float stdevs[100];
   char c[2];

   classStdev(input, classif, means, *kp, stdevs, metric);
   maxstd = 0.0;
   for (i=0; i LT *kp; INC i)
     if (stdevs[i] GT maxstd)
       {maxstd = stdevs[i]; maxc = i;}
   if (maxstd GT maxstdev) {
     printf("Class %d has standard deviation %.2f. Split it (y/n) ? ",
       maxc, maxstd);
     scanf("%1s", c);
     if ((*c EQ 'y') OR (*c EQ 'Y')) {
       printf("Splitting class %d into %d and %d\n", maxc, maxc, *kp);
       splitClass(input, classif, maxc, means, kp);
       return(1);
     }
   }

   return(0);
}  /*  splitClasses  */




/*L*

________________________________________________________________

		dropClasses
________________________________________________________________

Name:		dropClasses
Syntax:		
Description:	
Return value:	1 if any classes dropped, 0 otherwise
Author:		Tor L|nnestad
________________________________________________________________

*/

static int dropClasses(float **means, int *kp, int *h, int minpix)
{
   char c[2];
   int nk, mincl;

   mincl = 0;
   for (nk=1; nk LT *kp; INC nk)
     if (h[nk] LT h[mincl]) mincl = nk;
   if (h[mincl] LT minpix) {
     printf("Class %d has only %d occurrences. Drop it (y/n) ? ",
       mincl, h[mincl]);
     scanf("%1s", c);
     if ((*c EQ 'y') OR (*c EQ 'Y')) {
       printf("%s%d%s\n%s\n",
	      "Dropping class ", mincl, ".",
	      "You must classify once more to make it effective.");
       free(means[mincl]);
       for (nk=mincl+1; nk LT *kp; INC nk)
         means[nk-1] = means[nk];
       (*kp)--;
       return(1);
     }
   }

   return(0);
}  /*  dropClasses  */



/*P:isoCluster*

________________________________________________________________

		isoCluster
________________________________________________________________

Name:		isoCluster - ISODATA clustering, unsupervised classification

Syntax:		isoCluster <inimage> <outimage> <k> <mindist>
                   <maxstdev> <minpix> [<metric>]

Description:    Classifies the multi band input image iteratively
                to the nearest cluster mean. The program asks the user
		for initial mean vectors. Algorithm:

                | Ask user for k mean vectors
		| REPEAT
		|   classify input image to nearest mean
		|   Recalculate class means
		|   Report classification result
		|   IF (two classes are closer than <mindist>)
		|   THEN ask if user wants to merge them
		|   ELSEIF (a class has stdev larger than <maxstdev>)
		|   THEN ask if user wants to split it
		|   ELSEIF (a class has fewer than <minpix> pixels)
		|   THEN ask if user wants to remove it
		|   ENDIF
		|   IF (any such canges)
		|     THEN Recalculate class means
		| UNTIL user is satisfied OR no changes
		| save result

Arguments:      &inimage
                Multi band input image.

		&outimage
		Single band classification result.

		&k
		Initial number of classes.

		&mindist
		Minimum distance (using 'metric') between two class means.

		&maxstdev
		Maximum standard deviation within a class (again using
		'metric' as distanse measure).

		&minpix
		Minimum number of pixels in one class.

		&metric
		One of:
		&&1 => Euclidian distance
		&&2 => City block distance
		&&3 => Chess distance
		-&&
		Default value 1

Restrictions:	All bands of input image must have pixel type unsigned byte.
                For every class, a mean vector with n float components must
		be given, where n = number of bands in input image. 'k'
		must be between 2 and 100.

Author:		Tor Lønnestad, BLAB, Ifi, UiO

Examples:       | isoCluster landsat.img classes.img 5 10.0 30.0 500
                | isoCluster landsat.img classes.img 5 10.0 30.0 500 3

Id: 		$Id$
________________________________________________________________

*/

int main(int argc, char **argv)
{
   IMAGE input, output;
   float* means[100];
   float mindist, maxstdev;
   int bn, nk, k, nbands, metric, changes, iteration, more,
       limit, minpix, merges, splits;
   histogram h;
   char *args;

   Iset_message(TRUE);
   Iset_abort(TRUE);
   InitMessage(&argc, argv, xite_app_std_usage_text(
    "Usage: %s <inimage> <outimage> <k> <mindist> \n\
       <maxstdev> <minpix> [<metric>] \n"));

   if (argc == 1) Usage(1, NULL);
   args = argvOptions(argc, argv);
   if (argc != 7 && argc != 8) Usage(1, "Illegal number of arguments.\n");

   k        = atoi(argv[3]);
   mindist  = (float)atof(argv[4]);
   maxstdev = (float)atof(argv[5]);
   minpix   = atoi(argv[6]);
   if (argc GE 8) metric = atoi(argv[7]); else metric = EUCLID_METRIC;
   if ((k LT 2) OR (k GT 100))
     Error(2, "%s: k must be between 2 and 100.\n", argv[0]);


   input = Iread_image(argv[1]);
   nbands = Inbands(input);
   output = Imake_image(1, "Classification result",
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
     printf("\nClassifying, iteration %d...\n",iteration);
     classifyNearest(input, output[1], output[1], means, k,
       metric, &changes);
     printf("Classification finished with %d changes\n", changes);
     EXITIF (changes EQ 0);
     printf("Calculating new class means...\n");
     classMeans(input, output[1], means, k);
     mkHisto(output[1],h);
     printf("New class means and number of occurrences:\n\n");
     printf("            ");
     for (bn=1; bn LE nbands; INC bn) printf("  Band %d  ",bn);
     printf(" # pixels\n");
     FOR (nk=0; nk LT k; INC nk)
       printf("Class %2d : ",nk);
       for (bn=1; bn LE nbands; INC bn) printf(" %8.2f ", means[nk][bn]);
       printf("  %7d\n",h[nk]);
     ENDFOR;
     printf("\n");

     merges = mergeClasses(output[1], means, &k, nbands, metric, mindist);
     IF (NOT merges)
       splits = splitClasses(input, output[1], means, &k, maxstdev, metric);
       if (NOT splits) dropClasses(means, &k, h, minpix);
     ENDIF;

     IF (merges OR splits)
       printf("Recalculating class means...\n");
       classMeans(input, output[1], means, k);
       mkHisto(output[1],h);
       printf("New class means and number of occurrences:\n\n");
       printf("            ");
       for (bn=1; bn LE nbands; INC bn) printf(" Band %d  ",bn);
       printf(" # pixels\n");
       FOR (nk=0; nk LT k; INC nk)
         printf("Class %2d : ",nk);
         for (bn=1; bn LE nbands; INC bn) printf(" %7.2f ",means[nk][bn]);
         printf("  %7d\n",h[nk]);
       ENDFOR;
       printf("\n");
     ENDIF;

     IF (iteration EQ limit)
       printf("How many more iterations ?");
       scanf("%d",&more);
       EXITIF (more LE 0);
       limit = iteration + more;
     ENDIF;
   ENDLOOP;
   Iwrite_image(output, argv[2]);
   return(0);
}
