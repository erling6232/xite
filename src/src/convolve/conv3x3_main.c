

/*

________________________________________________________________

        conv3x3
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
#include <stdlib.h>
#include <xite/convolve.h>
#include XITE_STDIO_H
#include <xite/utils.h>
#include XITE_MALLOC_H
#include XITE_STRING_H

/*P:conv3x3*

________________________________________________________________

		conv3x3
________________________________________________________________

Name:		conv3x3 - convolution between image and 3x3 matrix

Syntax:		| conv3x3 [-scale <f>] [-offset <m>] [-title <title>] 
                |       [-verbose] <inimage> <outimage> <v1> .. <v9>
                |  or: 
		| conv3x3 [-scale <f>] [-offset <m>] [-title <title>]  
                |        [-verbose] -conv <datafile> <inimage> <outimage>

Description:    'conv3x3' performs a 3x3 convolution on the input image
                'inimage' and writes the result to 'outimage'.

		The convolution kernel is specified as a list of
                nine integers, on the command line or in an ascii file.

Options:        &-conv filename
                'filename' specifies an Ascii file with the nine integers
		for the convolution kernel.
           
		&-scale scale
		Scale convolution result. Default: Autoscaling.
		| pix = (conv. result) * scale

		&-offset offset
		Offset (scaled) convolution result.
		| pix = (conv. result) * scale + offset

		&-title title
		Use 'title' as title for the output image.

                &-verbose
		With this standard XITE option,	a message is written if the
		convolution sum is less than 0 or greater than 255.
                If autoscaling is in effect, the scale and offset factors are
                displayed.

See also:       conv3x3(3), convolve(1), laplace(1), gradient(1), sobel(1),
                prewitt(1), roberts(1)

Example:	| conv3x3 mona.img m.img -scale 1.5 1 1 1 1 1 1 1 1 1
                | conv3x3 mona.img m.img - s 2.0 -o 33 -c conv.fil

Author:		Otto Milvang & Tor Lønnestad

Id: 		$Id$
________________________________________________________________

*/



#ifdef MAIN

int main(int argc, char **argv)
{
  IMAGE img, img2;
  int i;
  char *title, *conv3x3_file, *args;
  double scale, offset;
  int log;
  int conv[9];
  FILE *cf;

  InitMessage(&argc, argv, xite_app_std_usage_text(
    "Usage: %s [-scale <scale>] [-offset <offset>] [-t <title>] \n\
              <inimage> <outimage> <v1> .. <v9> \n\
   or: %s [-scale <scale>] [-offset <offset>] [-t <title>] \n\
                 -conv <convfile> <inimage> <outimage>\n"));
  Iset_message(1);
  Iset_abort(1);

  if (argc == 1) Usage(1, NULL);
  args = argvOptions(argc, argv);

  conv3x3_file = read_switch(&argc, argv, "-conv", 1, NULL);
  title        = read_switch(&argc, argv, "-title", 1, NULL);
  scale        = read_dswitch(&argc, argv, "-scale", 0.0);
  offset       = read_dswitch(&argc, argv, "-offset", 0.0);
  log          = Verbose();
  conv3x3_file = read_switch(&argc, argv, "-c", 1, conv3x3_file);
  title        = read_switch(&argc, argv, "-t", 1, title);
  scale        = read_dswitch(&argc, argv, "-s", scale);
  offset       = read_dswitch(&argc, argv, "-o", offset);

  if ( (argc != 3 && conv3x3_file != NULL) ||
       (argc != 12 && conv3x3_file == NULL ))
    Usage(2, "Bad number of arguments\n");

  if (conv3x3_file)
    {
      cf = fopen(tilde_expand(conv3x3_file),"r");
      for (i=0; i < 9; i++)
	fscanf(cf," %d",&conv[i]);
      fclose(cf);
    } else {
      for (i=0; i < 9; i++)
	conv[i] = atoi(argv[i+3]);
    }  

  img = Iread_image(argv[1]);
  if (img == NULL)
    exit(Error(2, "Can't read input file %s\n", argv[1]));

  img2 = Icopy_init(img);
  for (i=1; i <= Inbands(img); i++)
    if (conv3x3(img[i], img2[i], conv, scale, offset, log))
      Warning(3, "bad pixel type in band %d\n");

  Ihistory(img2,argv[0], args);
  if (title) Iset_title(img2, title);
  Iwrite_image(img2,argv[2]);

  return(0);
}
