/*

________________________________________________________________

        thresMlComCur
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
#include <xite/biff.h>
#include <xite/blab.h>
#include <xite/histo.h>
#include <xite/message.h>
#include <xite/readarg.h>
#include <xite/thresMl.h>



/*P:thresMlComCur*

________________________________________________________________

		thresMlComCur
________________________________________________________________

Name:		thresMlComCur - multi-level thresholding by Reddi using 
                                complexity curve

Syntax:		| thresMlComCur [-mean] [-med] [-scale <scale>]
                |               [-nt <nt>] <inimage> [<outimage>]
                
Description:    'thresMlComCur' performs multi-level thresholding
                according to the method of Reddi et al., here applying 
                complexity curve instead of histogram (see ref).

Options:        &-mean
                Output is mean of pixels between two thresholds.

                &-med
		Output is median of pixels between two thresholds.

                &-scale
		Output is threshold-level * 'scale'. EX: pixels
                between 3rd and 4th threshold get value 3*'scale'.
                If scale is too large, it is adjusted down to make 255
                the maximum output value. Default scale is too large.

 		&-nt no_of_ths
		Desired number of thresholds.

		-&
		Use onlu one of '-mean', '-med' and '-scale'.

See also:	thresMl(1), thresMlAppScale(3), thresMlAppMean(3),
                thresMlAppMedian(3), thresMlApply(3), histoCentroid(3),
                thresMlCentroid(3), thresMlCentroid(3), mkComCur(3),
                thresMlComCur(3), thresMlReddi(3), thresMlReddi(1),
                thresMlWaHa(3), thresMlWaHa(1)

Restrictions:   'inimage' must have bands with pixel type unsigned byte.
                1 <= thresholds <= 255.

Reference:      &'S. S. Reddi, S. F. Rudin and H. R. Keshavan'
                "An Optimal Multiple Threshold Scheme for Image Segmentation"
		IEEE Transactions on Systems, Man and Cybernetics, 
                Vol SMC-14, pp 661-665, 1984.

		&'Sei-ichiro Kamata, Richard O. Eason and Eiji Kawaguchi'
		"Complexity Curves Versus Histograms and Their Application
                to Image Segmentation", 1070-1077

Author:		Hung Buu Huynh, BLAB, Ifi, UiO

Examples:       | # Find 4 thersholds values of mona.img
                | thresMlComCur  -nt 4  mona.img
               
Id: 		$Id$
________________________________________________________________

*/

int main(int argc, char **argv)
{
   IMAGE img;
   histogram t, h;
   int bn, output, nt, i, mean, median;
   double scale;
   char *args;

   InitMessage(&argc, argv, xite_app_std_usage_text(
     "Usage: %s [-nt <nt>] <inimage> [<outimage>]\n"));
   Iset_message(TRUE);
   Iset_abort(TRUE);

   if (argc == 1) Usage(1, NULL);
   args = argvOptions(argc, argv);

   nt     = read_iswitch(&argc, argv, "-nt", 1);
   mean   = read_bswitch(&argc, argv, "-mean");
   median = read_bswitch(&argc, argv, "-med");
   scale  = read_dswitch(&argc, argv, "-scale", 1000.0);

   if (argc > 3) Usage(2, "Illegal number of arguments.\n");

   img    = Iread_image(argv[1]);
   output = (argc GE 3);

   for (bn=1; bn LE Inbands(img); bn++) {
     if (thresMlComCur(img[bn], t, nt))
       Warning(2, "Error in thresMlComCur.\n");
     if (output) {
       if (mean) {
         if (mkHisto(img[bn], h)) Warning(2, "Error in mkHisto.\n");
         if (thresMlAppMean(img[bn], img[bn], t, nt, h))
	   Warning(2, "Error in thresMlAppMean.\n");
       } else if (median) {
         if (mkHisto(img[bn], h)) Warning(2, "Error in mkHisto.\n");
         if (thresMlAppMedian(img[bn], img[bn], t, nt, h))
	   Warning(2, "Error in thresMlAppMedian.\n");
       } else {
         if (thresMlAppScale(img[bn], img[bn], t, nt, scale))
	   Warning(2, "Error in thresMlAppScale.\n");
       }
     } else {
       printf("Threshold value%s in band %d : ", ((nt>1)? ("s") : ("")), bn);
       for (i=0; i<nt; i++) printf(" %d ", t[i]);

       printf("\n");
     }
   }

   if (output) {
     Ihistory(img, argv[0], args);
     Iwrite_image(img, argv[2]);
   }

   return(0);
}
