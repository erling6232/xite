
/*

________________________________________________________________

        sigma
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
#include <xite/sigma.h>
#include XITE_STDIO_H
#include <stdlib.h>
#include <xite/blab.h>
#include <xite/histo.h>
#include <xite/readarg.h>
#include <xite/message.h>

/*P:sigma*

________________________________________________________________

		sigma
________________________________________________________________

Name:		sigma - noise reduction filtering

Syntax:		| sigma -t <t> <input> <output> [<n>] 
                | sigma -s <s> <input> <output> [<n>] 

Description:    'sigma' filters the input image into the output
                image using a sigma filter with window size 'n' x 'n'.
                The center pixel is replaced with the average of all
		pixels in the window whose value is within 't'
		counts from the center pixel, or whose value is
		within 's' standard deviations from the center
		pixel.

		The standard deviation (of the noise) is estimated 
		by measuring the standard deviation within every
		possible 'n' x 'n' window, and averaging over the 10%
		lowest values.

		Uses histogram updating when moving horizontally
		from pixel to pixel.

		Default value of 'n' is 3.

See also:	sigma(3), mean(1), median(1), knn(1), kncn(1), snn(1),
                minarea(1), maxarea(1), mode(1), rank(1)

Restrictions:   The input image must have bands with pixel type unsigned byte.
                If 'n' is even, 'n' is set to 'n'+ 1.

Author:		Tor Lønnestad, BLAB, Ifi, UiO.

Examples:       | sigma -t 20 mona.img monaSig.img 7
                | sigma -s 2.0 mona.img monaSig.img 7

Id:             $Id$
________________________________________________________________

*/

int main(int argc, char **argv)
{

   IMAGE i1, i2;
   int n, windowsize, t;
   float s;
   char *args;

   Iset_message(TRUE);
   Iset_abort(FALSE);
   InitMessage(&argc, argv, xite_app_std_usage_text(
     "Usage: %s -t <t> <oldfile> <newfile> [<window size>]\n\
or:    %s -s <s> <oldfile> <newfile> [<window size>]\n"));

   if (argc == 1) Usage(1, NULL);
   args = argvOptions(argc, argv);
   
   t = atoi(read_switch(&argc, argv, "-t", 1, "-1"));
   s = atof(read_switch(&argc, argv, "-s", 1, "-1.0"));

   if (argc < 3 || argc > 4)  Usage(1, "Illegal number of arguments.\n");

   if (((s<0.0) AND (t<0))) Usage(1, "One option must be given.\n");
   if (((s>=0.0) AND (t>=0))) Usage(1, "Illegal option value.\n");

   if (argc == 4) windowsize = atoi(argv[3]); else windowsize = 3;
   if (windowsize < 3)
     Usage(1, "Window (or mask) size must be greater than 2.\n");

   i1 = Iread_image(argv[1]);
   i2 = Icopy_init(i1);

   if (t GE 0) {
     for (n=1; n LE Inbands(i1); INC n)
       sigmat(i1[n], i2[n], windowsize, t);
   } else if (s GE 0.0) {
     for (n=1; n LE Inbands(i1); INC n)
       sigma(i1[n], i2[n], windowsize, s);
   }

   Ihistory(i2, argv[0], args);
   Iwrite_image(i2,argv[2]);

   return(0);
}
