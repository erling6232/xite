

/*

________________________________________________________________

        gradient.c
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
#include XITE_STDIO_H
#include <xite/convert.h>
#include <xite/convolve.h>
#include <xite/message.h>
#include <xite/readarg.h>
#include XITE_STRING_H

#ifdef GRADIENT
#define OP gradient
#endif

#ifdef SOBEL
#define OP sobel
#define NAME "sobel"
#define OP1 SOBEL1
#define OPI SOBELI
#define OPN SOBELN
#endif

#ifdef PREWITT
#define OP prewitt
#define NAME "prewitt"
#define OP1 PREWITT1
#define OPI PREWITTI
#define OPN PREWITTN
#endif

#ifdef ROBERTS
#define OP roberts
#define NAME "roberts"
#define OP1 ROBERTS1
#define OPI ROBERTSI
#define OPN ROBERTSN
#endif

#ifdef GRAD
#define OP grad
#define NAME "grad"
#define OP1 GRAD1
#define OPI GRADI
#define OPN GRADN
#endif

/*P:sobel=gradient*/
/*P:prewitt=gradient*/
/*P:roberts=gradient*/
/*P:grad=gradient*/
/*P:gradient*

________________________________________________________________

		gradient
________________________________________________________________

Name:		gradient, sobel, prewitt, roberts, grad - gradient like
                operators

Syntax:         | gradient [-angle] [-magnitude] [-pseudo]
                |          [-xscale <xscale>] [-yscale <yscale>]
                |          [-title  <title>] [-verbose] 
                |          <inimage> <outimage> <xconvfile> <yconvfile>
                | 
		| sobel    [-angle] [-magnitude] [-pseudo]
		|          [-title  <title>] <inimage> <outimage>  
		|
		| prewitt  [-angle] [-magnitude] [-pseudo]
		|          [-title  <title>] <inimage> <outimage>  
		|
		| roberts  [-angle] [-magnitude] [-pseudo]
		|          [-title  <title>] <inimage> <outimage>  
		|
		| grad     [-angle] [-magnitude] [-pseudo]
		|          [-title  <title>] <inimage> <outimage>  

Description:    'gradient' performs a gradient operator on the input image
                'inimage' and writes the result to 'outimage'.
		
		The convolution kernels are specified by 'xconvfile' and
		'yconvfile'.

		'sobel', 'roberts', 'prewitt' and 'grad' use standard
		convolution kernels (see below).

Options:        &-magnitude
                Combine horizontal and vertical contributions by adding
		the absolute values.
		| magnitude = |convx| + |convy|

		&-angel
		Combine horizontal and vertical contributions by calculating
		the gradient angle.
		| angle = atan(convy/convx)

		&-pseudo
		Combine horizontal and vertical contributions by combining
		the magnitude and angle into a pseudo image.
		| pseudo = pseudo combination of magnitude and angle

		The pseudo comination of magnitude and pseudo:
                | mag level = magnitude / 32;
		|  Level  First Length
		|      0      0      4
		|      1      4     12
		|      2     16     20
		|      3     36     28
                |      4     64     36
                |      5    100     44
		|      6    144     52
		|      7    196     60
		| pseudo = first[level] + angle/256*length[level]

		The pseudo-image may be displayed with a "gradient"
		color-table.

		&-xscale xscale
		Scale magnitude data. Default is 'xscale'=0, which gives
		autoscaling.
                | xpix = (xconv. result) * xscale

		&-yscale yscale
		Scale magnitude data. Default is 'yscale'=0, which gives
		autoscaling.
                | ypix = (yconv. result) * yscale;

		&-title title
		Set title of output image to 'title'.

		&-verbose
		Write a message	if the convolution sum is less than 0 or
		greater than 255.

Standard convolution kernels:

		&sobel:
                |
		|            -1 0 1                 -1 -2 -1
		| xconv =    -2 0 2        yconv =   0  0  0  
		|            -1 0 1                  1  2  1
		| 

		&prewitt
                |
		|            -1 0 1                 -1 -1 -1
		| xconv =    -1 0 1        yconv =   0  0  0  
		|            -1 0 1                  1  1  1
		| 

		&roberts
                |
		|            1  0                  0  1
		| xconv =                 yconv =
		|            0 -1                 -1  0
		| 

		&grad
                |
		|           -1  1                 -1  0
		| xconv =                 yconv =
		|            0  0                  1  0
		| 

See also:	convolve(1), conv3x3(1), ascii2biff(1)
Author:		Otto Milvang
Id: 		$Id$
________________________________________________________________

*/

int main(int argc, char **argv)
{
  IMAGE img, img2;
  IBAND bm, ba, bp;
#ifdef GRADIENT
  ISS_BAND c1, c2;
#endif /* GRADIENT */
  int n, stat, bn, angle, mag, pseudo, verbose, num;
  double xscale, yscale;
  char *title, *args;
  int xstart, ystart;

  InitMessage(&argc, argv, xite_app_std_usage_text(
#ifdef GRADIENT
      "Usage: %s [-angle] [-magnitude] [-pseudo]\n\
      [-xscale <xscale>] [-yscale <yscale>]\n\
      [-title  <title>] \n\
      <inimage> <outimage> <xconvfile> <yconvfile>\n"));
#else /* GRADIENT */
      "Usage: %s [-angle] [-magnitude] [-pseudo]\n\
      [-title  <title>] <inimage> <outimage>\n"));
#endif /* GRADIENT */

  Iset_message(1);
  Iset_abort(1);

  if (argc == 1) Usage(1, NULL);
  args = argvOptions(argc, argv);

  angle   = read_bswitch(&argc, argv, "-a") ||  
            read_bswitch(&argc, argv, "-angle");
  mag     = read_bswitch(&argc, argv, "-m") ||  
            read_bswitch(&argc, argv, "-magnitude");
  pseudo  = read_bswitch(&argc, argv, "-p") ||  
            read_bswitch(&argc, argv, "-pseudo");
  title   = read_switch (&argc, argv, "-title", 1,
	    read_switch (&argc, argv, "-t", 1, NULL));
  xscale  = read_dswitch(&argc, argv, "-xscale",
	    read_dswitch(&argc, argv, "-x", 0.0));
  yscale  = read_dswitch(&argc, argv, "-yscale",
	    read_dswitch(&argc, argv, "-y", 0.0));
  verbose = Verbose();

  num = (angle ? 1 : 0) + (mag ? 1 : 0) + (pseudo ? 1 : 0);
  if (num == 0) {
    mag = 1;
    num = 1;
  }

  if (argc > 1) {
    img = Iread_image(argv[1]);
    if (img == NULL) Error(2, "Can't read input file %s.\n", argv[1]);
    n = Inbands(img);

    img2 = Init_image(n*num, title ? title : Ititle(img));
    if (img2 == NULL) Error(2, "Malloc error (new image).\n");

    if (Icopy_text(img, img2) != Iok) {
      Error(2, "Error in Icopy_text().\n");
    }
  }
  n = 1;

#ifdef GRADIENT
  if (argc != 5) Usage(2, NULL);
  stat = ascii2biff((IBAND *) &c1, argv[3], Is_short_typ);
  stat = ascii2biff((IBAND *) &c2, argv[4], Is_short_typ);

  for (bn=1; bn <= Inbands(img); bn++)
    {
      bm = ba = bp = NULL; 
      xstart = Ixstart(img[bn]);
      ystart = Iystart(img[bn]);
      if (mag)
	{
	  bm =  Imake_band(Iu_byte_typ, Ixsize(img[bn]), Iysize(img[bn]));
	  if (bm == NULL) 
	      Error(2, "Malloc error (New image, band %d)\n", bn);
	  Iset_start(bm, xstart, ystart);
	  img2[n++] = bm;
	}
      if (angle)
	{
	  ba =  Imake_band(Iu_byte_typ, Ixsize(img[bn]), Iysize(img[bn]));
	  if (ba == NULL) 
	      Error(2, "Malloc error (New image, band %d)\n", bn);
	  Iset_start(ba, xstart, ystart);
	  img2[n++] = ba;
	}
      if (pseudo)
	{
          bp =  Imake_band(Iu_byte_typ, Ixsize(img[bn]), Iysize(img[bn]));
          if (bp == NULL)
	    Error(2, "Malloc error (New image, band %d)\n", bn);
	  Iset_start(bp, xstart, ystart);
	  img2[n++] = bp;
        }
      stat = gradient(img[bn], c1, c2, xscale, yscale, bm, ba, bp, verbose);
      if (stat) Error(2, "Error in gradient \n");
    }

#else /* GRADIENT */

  if (argc != 3) Usage(2, NULL);
  for (bn=1; bn <= Inbands(img); bn++)
    {
      bm = ba = bp = NULL; 
      xstart = Ixstart(img[bn]);
      ystart = Iystart(img[bn]);
      if (mag)
	{
	  bm =  Imake_band(Iu_byte_typ, Ixsize(img[bn]), Iysize(img[bn]));
	  if (bm == NULL)	  
            Error(2, "Malloc error (New image, band %d)\n", bn);
	  Iset_start(bm, xstart, ystart);
	  img2[n++] = bm;
	}
      if (angle)
	{
	  ba =  Imake_band(Iu_byte_typ, Ixsize(img[bn]), Iysize(img[bn]));
	  if (ba == NULL)
	    Error(2, "Malloc error (New image, band %d)\n", bn);
	  Iset_start(ba, xstart, ystart);
	  img2[n++] = ba;
	}
      if (pseudo)
	{
	  bp =  Imake_band(Iu_byte_typ, Ixsize(img[bn]), Iysize(img[bn]));
	  if (bp == NULL)
	      Error(2, "Malloc error (New image, band %d)\n", bn);
	  Iset_start(bp, xstart, ystart);
	  img2[n++] = bp;
	}
      stat = OP(img[bn], bm, ba, bp, verbose);
      if (stat) Error(2, "Error in gradient \n");
    }

#endif /* GRADIENT */

    Ihistory(img2, argv[0], args); 
    Iwrite_image(img2, argv[2]);

    return(0);
}
