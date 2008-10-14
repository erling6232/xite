
/*

________________________________________________________________

        contour
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
#include <xite/contour.h>
#ifdef HAVE_STDIO_H
#  include <stdio.h>
#endif
#include <stdlib.h>
#include <xite/blab.h>
#include <xite/message.h>
#include <xite/readarg.h>

#ifndef MIN
# define MIN(a,b) ((a)<(b) ? a : b)
#endif

/*P:contour*

________________________________________________________________

		contour
________________________________________________________________

Name:		contour - make a contour map

Syntax:		| contour [-cpv <cpv>] [-bpv <bpv>] <inimage> <outimage>
                |         [<equidistance>]

Description:    Creates a contour map of the input image.
                Default 'equidistance' is 20.

		If the result is noisy, smooth the input image before
		contouring, or use larger equidistance.

Restrictions:   'inimage' must have bands with pixel type unsigned byte.

Options:        &-cpv cpv
                Contour	pixel value, default 0.

		&-bpv bpv
		Background pixel value, default 255.

See also:       contour(3), crossing(1)

Author:		Tor Lønnestad, BLAB, Ifi, UiO

Examples:       contour mona.img monaContour.img 40

Id:             $Id$
________________________________________________________________

*/

int main(int argc, char **argv)
{
   IMAGE i1, i2;
   int bn, eqd, cpv, bpv;
   char *args;

   Iset_message(TRUE);
   Iset_abort(TRUE);
   InitMessage(&argc, argv, xite_app_std_usage_text(
       "Usage: %s  [-cpv <cpv>] [-bpv <bpv>] <input image> \n\
             <output image> [<equidistance>]\n"));

   if (argc == 1) Usage(1, NULL);
   args = argvOptions(argc, argv);

   cpv = read_iswitch(&argc, argv, "-cpv", 0);
   bpv = read_iswitch(&argc, argv, "-bpv", 255);

   if ((argc LT 3) OR (argc GT 4))
     exit(Usage(2, "Illegal number of arguments.\n"));

   if (argc EQ 3) eqd = 20; else eqd = atoi(argv[3]);

   i1 = Iread_image(argv[1]);
   i2 = Icopy_init(i1);

   for (bn=1; bn LE Inbands(i1); ++ bn)
     if (contour(i1[bn], i2[bn], eqd, cpv, bpv))
       Warning(3, "Error in countour() for band %d.\n", bn);

   Ihistory(i2, argv[0], args);
   Iwrite_image(i2, argv[2]);

   return(0);
}
