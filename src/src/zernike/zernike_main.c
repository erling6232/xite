/*

________________________________________________________________

        zernike.c
        $Id$
	Copyright 1996, Blab, UiO
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

static char *Id = "$Id$";

#include <xite/includes.h>
#include XITE_STDIO_H
#include XITE_TIME_H
#include <math.h>
#include <stdlib.h>
#include <signal.h>
#include <xite/blab.h>
#include <xite/biff.h>
#include <xite/histo.h>
#include <xite/readarg.h>
#include <xite/zernike.h>
#include <xite/message.h>

/*P:zernike*

________________________________________________________________

		zernike
________________________________________________________________

Name:		zernike - zernike moment image of a gray scale 
                or binary image

Syntax:		zernike [-n <n>] [-r] [-t <title>] <inimage> <outimage>

Description:	The Zernike moment of order 'n' and repetition 'm' of
                an image f(x,y) is defined as follows:
		|          n+1                             *
                | A(n,m) = ---- Sum Sum f(x,y)[V(n,m, x,y)]  
		|           pi   x   y
		|
		| where x^2+y^2 <= 1
		|
		The image V(n,m, x,y) is the Zernike basis images of
		order 'n' and repetition 'm'. These basis images are
		complex and orthogonal. The Zernike moments are
		essentially the projections of the input image onto
		these basis images. 

		The original image can be reconstructed from the
		Zernike moments. The N-th order approximation is given
		by 
		|  ^         N
		|  f(x,y) = Sum Sum A(n,m) V(n,m, x,y)
		|           n=0  m
		|
		The contribution or information content of the n-th
		order moments is
		|
		|  I(x,y, n) = Sum A(n,m) V(n,m, x,y)
		|              m
		
Restrictions:   'inimage' must be single-band with pixel type unsigned byte.

Options:	&-n n
                Use moment order 'n'. Default 0.

                &-r
                Reconstruct image from moments. Default: Compute the absolute
		value of I(x,y, n).

		&-t title
		Use 'title' for 'outimage'.

See also:       zer_mom(3), zer_con(3), zer_rec(3), zer_pol(3)

References:     &[1] 'A. Khotanzad and Y.H. Hong'
                "Rotation invariant image recognition using features
		selected via a systematic method",
		Pattern Recognition, vol.23, no.10, pp.1089-1101, 1990. 

		&[2] 'Thomas H. Reiss'
		"Recognizing Planar Objects Using Invariant Image Features",
		Lecture Notes in Computer Science, volume 676, pp. 17-20,
		Springer-Verlag, 1993.

Return value:   0 : OK
Files:          | xite/src/zernike/zernike.c
Author:		Øivind Due Trier, Ifi, UiO.
________________________________________________________________

*/

int main(int argc, char *argv[])
{
  int order;
  char *infile, *outfile, *args, *title;
  IBAND inband, outband, tempband;
  IMAGE inimage, outimage;
  int xsize, ysize;
  long start_time, elapsed_time;
  int reconstruct;
  int status;

  Iset_message(TRUE);
  Iset_abort(TRUE);
  InitMessage(&argc, argv, xite_app_std_usage_text(
    "Usage: %s [-n <n>] [-r] [-t <title>] <inimage> <outimage>\n"));

  if (argc == 1) Usage(1, NULL);

  args = argvOptions(argc, argv); /* Save command-line arguments. */

  order       = read_iswitch(&argc, argv, "-n", 0);
  reconstruct = read_bswitch(&argc, argv, "-r");
  title       = read_switch(&argc, argv, "-t", 1,
			    reconstruct ? "zernike reconstructed" : "zernike");

  if (argc < 3) Usage(2, "Illegal number of arguments.\n");

  infile  = argv[argc-2];
  outfile = argv[argc-1];

  /* Start the clock */

  start_time = clock();

  /* Read input image from file */

  inimage = Iread_image(infile);
  inband  = (IBAND)inimage[1]; /* First band only is used. */

  xsize = Ixsize(inband);
  ysize = Iysize(inband);

  /* Make data structure for output image */

  outimage = Icopy_init(inimage);
  Iset_title(outimage, title);
  outband  = (IBAND) outimage[1];

  if (reconstruct) {
    zer_rec (order, inband, outband);
    /* histoEq(tempband, outband, 256);*/
  } else {
    tempband = Imake_band(Iu_byte_typ, xsize, ysize);
    zer_con (order, inband, tempband);
    histoEq(tempband, outband, 256);
    Idel_band (&tempband);
  }

  elapsed_time = clock() - start_time;
  Info(0, "Time used (CPU) : %fs\n", elapsed_time / 1e6);

  Ihistory(outimage, argv[0], args);
  status = Iwrite_image(outimage, outfile);

  return 0; /* Unix commands should return 0 */

} /* main() */
