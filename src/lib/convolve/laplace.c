
/*

________________________________________________________________

        laplace
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
#ifdef HAVE_STDIO_H
#  include <stdio.h>
#endif
#include <stdlib.h>
#include <xite/convolve.h>
#include <xite/readarg.h>
#ifdef HAVE_STRINGS_H
# include <strings.h>
#else
# ifdef HAVE_STRING_H
#  include <string.h>
# endif
#endif
#include <xite/message.h>

#ifndef MAIN

/*F:laplace*

________________________________________________________________

		laplace
________________________________________________________________

Name:		laplace - edge and line detection
Syntax:         | #include <xite/convolve.h>
		|
                | void laplace( IBAND input, IBAND output, int a,
                |    int b, int c, double scale, double offset,
                |    int verbose );
Description:    'laplace' performs the convolution between
                a band 'input' and a convolution kernel 'conv'.
                |
		|        a          b          a
		|        b          c          b
                |        a          b          a
		|
                The result of the convolution is scaled
		|   pix = (conv. result) * scale + offset;
                If 'scale' is zero an autoscaling is performed.
		If 'verbose' flag is set, the number of underflows and
		overflows are printed. If 'verbose' flag is set
		and autoscale is performed the scale and offset 
		factors are displayed

		'conv3x3_overflow' and 'conv3x3_underflow' are 
		global variabels and may be declared as
		| extern int conv3x3_overflow, conv3x3_underflow
See also:	laplace(1), conv3x3(3), lapZeroCross(3)
Author:		Otto Milvang
Examples:       
Id: 		$Id$
________________________________________________________________

*/



void laplace(IBAND input, IBAND output, int a, int b, int c, double scale, double offset, int verbose)
{
  int conv[9];
  conv[0] = conv[2] = conv[6] = conv[8] = a;
  conv[1] = conv[3] = conv[5] = conv[7] = b;
  conv[4] = c;
  conv3x3(input, output, conv, scale, offset, verbose);
}

#endif /* not MAIN */




/*P:laplace*

________________________________________________________________

		laplace
________________________________________________________________

Name:		laplace - edge and line detection
Syntax:		| laplace [-scale <n>] [-offset <m>]  [-title <title>] 
                |          [-verbose] <inimage> <outimage> a b c 
Description:    'laplace' perform a 3x3 convolution on the input image
                'inimage' and write the result to 'outimage'.
                The convolution kernel is specified by a, b and c
		|
		|     a b a
		|     b c b
		|     a b a
                |
                
                If '-scale' (-s) and '-offset' (-o) is specified all 
		data are scaled:
                | pix = (conv. result) * scale + offset;
                Othewise an autoscaling is performed.

		'-title' (-t) 'title' is used to set a new title on the 
		output image.

                If '-verbose' (-v) is specified 'laplace' writes a message
                if the convolution sum is less than 0 or greater than 255.
                If autoscale is performed the scale and offset factors are
                displayed
Example:	| laplace mona.img m.img -scale 1.5 -1 -1 8
See also:       laplace(3), lapZeroCross(1), conv3x3(1)
Author:		Otto Milvang
Id: 		$Id$
________________________________________________________________

*/



#ifdef MAIN

int main(int argc, char **argv)
{
  IMAGE img, img2;
  int i;
  char *title, txt[132];
  double scale, offset;
  int log, a, b, c;

  Iset_message(TRUE);
  Iset_abort(TRUE);
  InitMessage(&argc, argv, xite_app_std_usage_text(
    "Usage: %s [-s <scale>] [-o <offset>] [-t <title>] [-v]\n\
       <inimage> <outimage> <a> <b> <c> \n"));

  if (argc == 1) Usage(1, NULL);
  title        = read_switch(&argc, argv, "-title", 1, NULL);
  scale        = read_dswitch(&argc, argv, "-scale", 0.0);
  offset       = read_dswitch(&argc, argv, "-offset", 0.0);
  log          = read_bswitch(&argc, argv, "-verbose");
  title        = read_switch(&argc, argv, "-t", 1, title);
  scale        = read_dswitch(&argc, argv, "-s", scale);
  offset       = read_dswitch(&argc, argv, "-o", offset);
  log          = log || read_bswitch(&argc, argv, "-v");

  if (argc != 6) Usage(1, "Illegal number of arguments.\n");

  a = atoi(argv[3]);
  b = atoi(argv[4]);
  c = atoi(argv[5]);

  img = Iread_image(argv[1]);
  if (img == NULL) {
    Error(2, "Can't read input file %s\n", argv[1]);

  }
  img2 = Icopy_init(img);
  for (i=1; i <= Inbands(img); i++)
    laplace(img[i], img2[i], a, b, c, scale, offset, log);
  sprintf(txt,"%d %d %d", a,b,c);
  Ihistory(img2,argv[0],txt);
  if (title) Iset_title(img2, title);
  Iwrite_image(img2,argv[2]);
  return(0);
}

#endif /* MAIN */
