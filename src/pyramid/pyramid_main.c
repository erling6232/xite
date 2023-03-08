/*

________________________________________________________________

        pyramid
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
#include <xite/pyramid.h>
#include XITE_STDIO_H
#include <stdlib.h>
#include <xite/blab.h>
#include <xite/convolve.h>
#include <xite/readarg.h>
#include <xite/message.h>

/*P:pyramid*

________________________________________________________________

		pyramid
________________________________________________________________

Name:		pyramid - create a pyramid data structure

Syntax:		| pyramid [-l] [-s] [-r <rank>] [-g] [-n <levels>] \\
                |     <inimage> <outimage>

Description:    Create a pyramid data structure from the first band
                of the input image 'inimage', write the resulting
		image to 'outimage'. By default, the bottom layer
		will have the same size as the input image, (-1 if
		odd), and the resampling technique is two by two
		average.

Options:        &-l
		Let the bottom band have the smallest square
		power of two size larger than the input.

		&-s
		Let the bottom band have the largest square
		power of two size smaller than the input.

		&-r rank
		Use rank='rank'; the rank''th sorted pixel value
		in the window as resampling result ('rank' = 1..4).
                
		&-g
		Use Gaussian pyramid, ref. IEEE trans. on Comm.
                1983, no.4, page 532, with a=0.4.
		1983, no.4, page 532 with a=0.4.

		&-n levels
		Spesify the number of 'levels' wanted in 
		the pyramid (default: all possible levels).

		-&Use only one of '-l' and '-s' or none. If none, then 
                integer division by 2 is performed as far as possible. 

                If none of the options are used, mean pyramid is performed.

Restrictions:   Input image must have pixel type unsigned byte.

Author:		Tor Lønnestad, BLAB, Ifi, UiO

Revised:        Helene Schulerud, BLAB, Ifi, UiO,
                Svein Bøe, Blab, Ifi, UiO

Id:             $Id$

Examples:	| pyramid mona.img monaPyr.img
		| pyramid -l mona.img mona.PyrL.img
		| pyramid -s -r 4 mona.img mona.PyrS.img
                | pyramid -g mona.img mona.PyrG.img
________________________________________________________________

*/

static int initconv(ISS_BAND c)
{
   c[1][1] = 1;
   c[1][2] = 5;
   c[1][3] = 8;
   c[1][4] = 5;
   c[1][5] = 1;
   c[2][1] = 5;
   c[2][2] = 25;
   c[2][3] = 40;
   c[2][4] = 25;
   c[2][5] = 5;
   c[3][1] = 8;
   c[3][2] = 40;
   c[3][3] = 64;
   c[3][4] = 40;
   c[3][5] = 8;
   c[4][1] = 5;
   c[4][2] = 25;
   c[4][3] = 40;
   c[4][4] = 25;
   c[4][5] = 5;
   c[5][1] = 1;
   c[5][2] = 5;
   c[5][3] = 8;
   c[5][4] = 5;
   c[5][5] = 1;

   return 0;
}

int main(int argc, char **argv)
{
  IMAGE ii, oi;
  IBAND band;
  ISS_BAND conv;
  int smaller, larger, rank, gaussian, po2, xsize, ysize, nlevels;
  char arg[50];
  
  Iset_message(TRUE);
  Iset_abort(TRUE);
  InitMessage(&argc, argv, xite_app_std_usage_text(
    "Usage: %s [-s] [-l] [-r <rank>] [-g] [-n <levels>] \n\
       <inimage> <outimage>\n"));

  if (argc == 1) Usage(1, NULL);

  smaller  = read_bswitch(&argc, argv, "-s");
  larger   = read_bswitch(&argc, argv, "-l");
  rank     = read_iswitch(&argc, argv, "-r", 0);
  nlevels  = read_iswitch(&argc, argv, "-n", 0);
  gaussian = read_bswitch(&argc, argv, "-g");

  if (argc != 3) Usage(1, "Illegal number of arguments.\n");

  ii = Iread_image(argv[1]);
  if (Ipixtyp(ii[1]) != Iu_byte_typ)
    Error(2, "Can only handle unsigned byte pixels.\n");

  xsize = Ixsize(ii[1]);
  ysize = Iysize(ii[1]);
  if (smaller) {/* reduce to quadratic size, power of 2 */
    for (po2=1; (po2 < xsize) && (po2 < ysize); po2 *= 2) ;
    po2 /= 2;
    band = Imake_subband(ii[1], 1, 1, po2, po2);
  } else if (larger) {
    for (po2=1; (po2 < xsize) || (po2 < ysize); po2 *= 2) ;
    band = Imake_band(Iu_byte_typ, po2, po2);
    Icopy_band(ii[1], band);
  } else {
    band = ii[1];
  }

  if (rank) {
    mkPyramidRank2(band, &oi, rank, nlevels);
    sprintf(arg, " with rank = %d\n", rank);
  } else if (gaussian) {
    conv = (ISS_BAND)Imake_band(Is_short_typ,5,5);
    initconv(conv);
    mkPyramidGauss5(band, &oi, conv, nlevels);
    sprintf(arg, " using 5x5 Gauss\n");
  } else {
    mkPyramidAver2(band, &oi, nlevels);
    sprintf(arg, " using 2x2 averaging\n");
  }

  Icopy_text(ii, oi);
  Ihistory(oi, argv[0], arg);
  Iwrite_image(oi, argv[2]);  

  return(0);
}
