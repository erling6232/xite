
/*

________________________________________________________________

        median
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
#include XITE_STDIO_H
#include <stdlib.h>
#include <xite/blab.h>
#include <xite/convert.h>
#include <xite/histo.h>
#include <xite/median.h>
#include <xite/message.h>
#include <xite/minmax.h>
#include <xite/readarg.h>

/*P:median*

________________________________________________________________

		median
________________________________________________________________

Name:		median - noise reduction filtering

Syntax:		| median [-frac <fractile>] [-mask <maskfile>] [-mirror] 
                |         <inimage> <outimage> [<dx> [<dy>]]

Description:    Median/fractal noise filtering with window 'dx' * 'dy'.
                For images with pixel type unsigned byte, the histogram
		update technique is used.

		Default values for 'dx' is 3, for 'dy' it is 'dx'.

Options:        &-frac fractile
		'fractile' determines which output pixel is chosen from the
		'dx' * 'dy' window. 'fractile' equal to 0.5 gives the median,
		'fractile' equal to 0.0 gives the minimum, 'fractile' equal
		to 1.0 gives the maximum. Default: 0.5 (median).

		&-mask maskfile
		Read a mask from an ascii file rather than using a rectangle
		of size 'dx' * 'dy'.

		&-mirror
		Mirror the mask before using it.

Reference:      Huang, Yang and Tang:
		"A Fast Two-Dimensional Median Filtering Algorithm"
		IEEE Trans. Ac., Speech, and Signal Proc, Vol ASSP-27,
		No.1, Feb.1979

See also:	pseudomedian3x3(1)

Author:		Tor Lønnestad and Otto Milvang, BLAB, Ifi, Uio

Examples:       | median mona.img monaMed.img
                | median mona.img monaMed.img 7
		| median mona.img monaMed.img 7 5
		| median -f 0.25 -m filter.msk mona.img mona.25.img
		|
		| filter.msk:
		| #
		| # Filter for mona
		| $size 5 5
		| 0 1 1 1 0
		| 1 1 1 1 1
		| 1 1 2 1 1	; count the center pixel twice
		| 1 1 1 1 1
		| 0 1 1 1 0

Id:             $Id$
________________________________________________________________

*/

int main(int argc, char **argv)
{
   IMAGE i1,i2;
   int bn, dx, dy, x, y, mirror;
   char *mask, *frac;
   double dfractile;
   char *args;
   ISS_BAND mband;

   InitMessage(&argc, argv, xite_app_std_usage_text(
     "Usage: %s [-frac <fractile>] [-mask <maskfile>] [-mirror]\n\
      <inimage> <outimage> [<dx> [<dy>]] \n"));
   Iset_message(TRUE); 
   Iset_abort(TRUE);

   if (argc == 1) Usage(1, NULL);
   args = argvOptions(argc, argv);

   mask   = read_switch(&argc, argv, "-mask", 1, NULL);
   frac   = read_switch(&argc, argv, "-frac", 1, NULL);
   mirror = read_bswitch(&argc, argv, "-mirror");
   if (frac != NULL) dfractile = atof(frac); else dfractile = 0.5;

   if ((argc < 3) OR (argc GT 5) OR (mask != NULL && argc>3))
     Usage(1, "Illegal number of arguments.\n");

   if (argc GE 4) dx = atoi(argv[3]); else dx = 3;
   if (argc GE 5) dy = atoi(argv[4]); else dy = dx;
   if ((dx LT 1) || (dy LT 1))
     Error(2, "Window size should be greater than 0.\n");
 
   i1 = Iread_image(argv[1]);
   i2 = Icopy_init(i1);

   if (mask) {
     ascii2biff((IBAND *) &mband, mask, 6);
   } else {
     mband = (ISS_BAND) Imake_band(Is_short_typ, dx, dy);
     Iset_start((IBAND) mband, -(dx/2), -(dy/2));
     for (y=1; y<=dy; y++) 
       for(x=1; x<=dx; x++) 
	 mband[y][x] = 1;
   }

   for (bn=1; bn LE Inbands(i1); INC bn) {
     if (Ipixtyp(i1[bn]) == Iu_byte_typ && mask == NULL) {
       if (frac == NULL)
	 median(i1[bn], i2[bn], dx, dy);
       else if (dfractile <= 0.0)
	 minarea(i1[bn], i2[bn], dx, dy);
       else if (dfractile >= 1.0)
	 maxarea(i1[bn], i2[bn], dx, dy); 
       else
	 fractile(i1[bn], i2[bn], dx, dy, dfractile);
     } else {
       fractile_mask(i1[bn], i2[bn], mband, mirror, dfractile);
     }
   }
   
   Ihistory(i2, argv[0], args);
   Iwrite_image(i2, argv[2]);
   
   return(0);
}
