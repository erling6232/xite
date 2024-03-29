
/*

________________________________________________________________

        knn
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
#include <xite/knn.h>
#include XITE_STDIO_H
#include <stdlib.h>
#include <xite/blab.h>
#include <xite/histo.h>
#include <xite/readarg.h>
#include <xite/message.h>

#ifndef MAX
# define MAX(a,b) (((a) > (b)) ? (a) : (b))
#endif
#ifndef MIN
# define MIN(a,b) (((a) < (b)) ? (a) : (b))
#endif

/*P:knn*

________________________________________________________________

		knn
________________________________________________________________

Name:		knn - k nearest neighbour noise reduction

Syntax:		knn <input image> <output image> <n> <k>

Description:    Performs k nearest neighbour noise removal.
                n x n is window size.
		Averages over center pixel and k neighbours,
		using histogram updating technique.

Restrictions:   The input image must have pixel type unsigned byte.

Reference:      L. S. Davis & A. Rosenfeld:"Noise Cleaning by
                Iterated Local Averaging" IEEE trans. on syst.,
		man, and cyb., vol smc-8, no 9, 1978.
See also:       knn(3), mbknn(1), kncn(1), mbkncn(1)

Author:		Tor L�nnestad, BLAB, Ifi, UiO

Examples:       knn mona.img monaknn.img 5 9

Id:             $Id$
________________________________________________________________

*/

int main(int argc, char **argv)
{
   IMAGE i1, i2;
   int n, k, bn, stat;
   char *args;

   InitMessage(&argc, argv, xite_app_std_usage_text(
    "Usage: %s <inimage> <outimage> <n> <k>\n"));
   Iset_message(TRUE);
   Iset_abort(TRUE);

   if (argc == 1) Usage(1, NULL);
   args = argvOptions(argc, argv);
   if (argc != 5) Usage(2, "Illegal number of arguments.\n");

   n = atoi(argv[3]);
   k = atoi(argv[4]);

   i1 = Iread_image(argv[1]);
   i2 = Icopy_init(i1);

   for (bn=1; bn LE Inbands(i1); bn++)
     if ((stat = knn(i1[bn], i2[bn], n, k)))
       Warning(stat, "Error in band %d\n", bn);

   Ihistory(i2, argv[0], args);
   Iwrite_image(i2, argv[2]);

   return(0);
}
