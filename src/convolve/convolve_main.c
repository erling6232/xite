

/*

________________________________________________________________

        convolve.c
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
#include <xite/convolve.h>
#include XITE_STDIO_H
#include <stdlib.h>
#include <xite/convert.h>
#include <xite/readarg.h>
#include <xite/message.h>
#include XITE_STRING_H

static char *convolve_errors[] = {
"Ok.\n",
"Input and output pixeltype must be unsigned byte, signed or unsigned short.\n",
"Kernel pixeltype must be unsigned or signed short.\n",
"Size error.\n",
"Not enough memory.\n",
};

/*P:convolve*

________________________________________________________________

		convolve
________________________________________________________________

Name:		convolve - perform an n x m convolution
Syntax:		| convolve [<option>...] <inimage> <inimage> <outimage>
		| or:
		| convolve [<option>...] <inimage> <outimage>
		|           <xsize> <ysize> <v1> .. <vn>
                |  or: 
		| convolve [<option>...] -conv <asciifile>
		|           <inimage> <outimage>

Description:    'convolve' performs a convolution between a convolution
                kernel and one or more bands. The result is written to
		'outimage'.

		In the first case above, the first band of the first 'inimage'
		argument is considered the convolution kernel. All the bands
		of the second 'inimage' argument will be processed.

		In the second case above, the convolution kernel is specified
		as a list of integers (and 'xsize' and 'ysize' specify the
		size of the kernel).

		In the third case above, the convolution kernel is specified
		by the name of a file containing ascii values.
                
Options:        &-conv filename
                Take convolution kernel from Ascii file given by 'filename'.

		&-scale scale
		Scale convolution result. Default (with no '-scale' option or
		with 'scale' equal to 0.0): Autoscaling.
                | pixel = (conv. result) * scale

		&-offset offset
		Offset (scaled) convolution result.
                | pixel = (conv. result) * scale + offset

		&-title title
		Use 'title' as title for the output image.
		
		&-byte
		Force the output image pixeltype to become unsigned byte.
		Default: Same pixeltype as input band.

		&-word
		Force the output image pixeltype to become signed short.
		Default: Same pixeltype as input band.

		&-verbose
		With this standard XITE option, a message is written if
		the convolution sum is less than 0 or greater than 255.
		If autoscaling is performed, the scale and offset
		factors are displayed.

File format:	See 'ascii2biff(1)'

Restrictions:	Works on unsigned byte and signed short images.
                If the convolution kernel is read from a BIFF image file,
		it must be of type signed short. Unsigned short images will
		be treated as if they were signed short. 

See also:	ascii2biff(1), convolve(3), conv3x3(1), laplace(1),
                gradient(1), sobel(1), prewitt(1), roberts(1)

Example:	| convolve mona.img m.img -scale 0.5 3 3 1 1 1 1 1 1 1 1 1
Author:		Otto Milvang
Revised:        Svein Bøe
Id: 		$Id$
________________________________________________________________

*/



int main(int argc, char **argv)
{
  IMAGE img, img2, kernel = NULL;
  int  n, i, res, wo = 0, bo = 0, stat, x, y;
  char *title = NULL, *conv_file = NULL, txt[1132];
  double scale = 0.0, offset = 0.0;
  int  log = 0, xs = 0, ys = 0;
  ISS_BAND convf = NULL;

  InitMessage(&argc, argv, xite_app_std_usage_text(
      "Usage: %s [<option>...] <inimage> <inimage> <outimage> \n\
or:    %s [<option>...] <inimage> <outimage> <xsize> <ysize> <v1> .. <vn> \n\
or:    %s [<option>...] [-conv <ascii-kernelfile>] <inimage> <outimage> \n\
       where <option> is chosen from \n\
         -scale <scale> \n\
         -offset <offset> \n\
         -title <title> \n\
         -byte \n\
         -word \n"));

  Iset_message(1);
  Iset_abort(1);
  
  strcpy(txt, "");
  for(i=1; i<argc; i++) {
    strcat(txt," ");
    strcat(txt,argv[i]);
  }      

  conv_file    = read_switch(&argc, argv, "-conv", 1, NULL);
  if (!conv_file)
    conv_file  = read_switch(&argc, argv, "-c", 1, conv_file);
  title        = read_switch(&argc, argv, "-title", 1, NULL);
  if (!title)
    title      = read_switch(&argc, argv, "-t", 1, title);
  scale        = read_dswitch(&argc, argv, "-scale", 0.0);
  if (scale == 0.0)
    scale      = read_dswitch(&argc, argv, "-s", scale);
  offset       = read_dswitch(&argc, argv, "-offset", 0.0);
  if (offset == 0.0)
    offset     = read_dswitch(&argc, argv, "-o", offset);
  log          = Verbose();
  bo           = read_bswitch(&argc, argv, "-byte");
  if (bo == 0)
    bo         = bo  || read_bswitch(&argc, argv, "-b");
  wo           = read_bswitch(&argc, argv, "-word");
  if (wo == 0)
    wo         = wo  || read_bswitch(&argc, argv, "-w");

  if (argc >=5) {
    /* <xsize> and <ysize> are given. */
    xs = atoi(argv[3]);
    ys = atoi(argv[4]);
  }
  if ( (argc != 3 && conv_file != NULL) ||
       (argc != 4 && argc != 5 + xs*ys && conv_file == NULL )) {
    /* conv_file is given when it shouldn't be, or the wrong number of
     * listed values are given.
     */
    Usage(2, NULL);
  }

  if (conv_file) {
    /* Kernel is given in ascii-file. */
    stat = ascii2biff((IBAND *) &convf, conv_file, Is_short_typ);
    if (stat) return(stat);
  } else if (argc == 4) {
    /* Kernel is given as BIFF file. */
    kernel = Iread_image(argv[1]);
    if (kernel == NULL)
      Error(2, "Can't read input kernel file %s.\n", argv[2]);

    convf  = (ISS_BAND) kernel[1];
    if (Ipixtyp((IBAND) convf) != Is_short_typ &&
	Ipixtyp((IBAND) convf) != Iu_short_typ)
      Error(3, convolve_errors[2]);
  } else {
    /* Kernel is listed on command-line. */
    convf = (ISS_BAND) Imake_band(Is_short_typ, xs, ys);
    if (convf == NULL)
      Error(5, "Malloc error (Conv matrix %d x %d).\n", xs, ys);
    i = 5;
    for(y=1; y<= ys; y++)
      for(x=1; x<= xs; x++)
	convf[y][x] = atoi(argv[i++]);
  }

  /* Read input image. */
  if (kernel == NULL) img = Iread_image(argv[1]);
  else img = Iread_image(argv[2]);
  if (img == NULL) {
    if (kernel == NULL)
      Error(2, "Can't read input file %s.\n", argv[1]);
    else
      Error(2, "Can't read input file %s.\n", argv[2]);
  }
  n = Inbands(img);
  img2 = Init_image(n, title ? title : Ititle(img));
  if (img2 == NULL) Error(2, "Malloc error (new image).\n");
  for(i=1; i<=n; i++) {
    img2[i] = Imake_band(bo == wo ? Ipixtyp(img[i]) :
			 (bo ? Iu_byte_typ : Is_short_typ),
			 Ixsize(img[i]), Iysize(img[i]));
    Iset_start(img2[i], Ixstart(img[i]), Iystart(img[i]));
    if (img2[i] == NULL) 
      Error(2, "Malloc error (new image, band %d).\n", i);
  }
  Icopy_text(img, img2);
			   
  for (i=1; i <= Inbands(img); i++) {
    res = convolve(img[i], img2[i], convf, scale, offset, log);
    if ((res != 0) || log) {
      if (res) Error(res, "Band 1, %s", convolve_errors[res]);
    }
  }
  Ihistory(img2,argv[0], txt);
  if (kernel == NULL)
    res = Iwrite_image(img2,argv[2]);
  else
    res = Iwrite_image(img2,argv[3]);
  return(res);
}
