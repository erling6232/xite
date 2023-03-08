

/*

________________________________________________________________

        band_minmax.c
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
#include <xite/statistics.h>
#include XITE_STDIO_H
#include XITE_STRING_H
#include <xite/debug.h>
#include <xite/readarg.h>
#include <xite/message.h>



/*P:image_minmax*

________________________________________________________________

		image_minmax
________________________________________________________________

Name:		image_minmax - find minimum and maximum in an image

Syntax:	        | image_minmax [-f] [-n <band>] [-min] [-max]
                |    <filename_1> [... <filename_n>]

Description:	'image_minmax' finds the minimum and maximum value of
                each band in one or more images. If no options are 
		given, the result will be written formatted. See example.

		If one or more options are specified, the output
		will be unformatted.

Options:        You may specify more than one option.

                &-f
                Print the filename.

                &-n <band>
		Print min and/or max for 'band'.

		&-min
		Print minimum value only.

		&-max
		Print maximum value only.

Restrictions:   Does not work on complex band. No error message, however,
                and 0.0 is printed for minimum and maximum.

Return value:   0 = success, 2 = failure

Author:		Otto Milvang

Examples:	| xite> image_minmax mona.img
                | mona.img, band 1: Min 27.000000, Max 246.000000
		|
		| xite> image_minmax -min -max mona.img
		| 27.000000 246.000000
		|
		| xite> cat mona | image_minmax - -min -max 
		| 27.000000 246.000000

Id: 		$Id$
________________________________________________________________

*/

int main(int argc, char **argv)
{
  IMAGE img;
  int n=0, l = FALSE, u = FALSE, name = FALSE, format = TRUE, pipe = FALSE;
  int b, num_arg;
  double minval, maxval;
  
  Iset_message(TRUE);
  Iset_abort(TRUE);
  InitMessage(&argc, argv, xite_app_std_usage_text(
   "Usage: %s [-f] [-n <band>] [-min] [-max] \n\
       <filename_1> [... <filename_n>]\n"));

  if (argc == 1) Usage(1, NULL);

  name = read_bswitch(&argc, argv, "-f");
  n    = read_iswitch(&argc, argv, "-n", 0);
  l    = read_bswitch(&argc, argv, "-min");
  u    = read_bswitch(&argc, argv, "-max");
  if (name || n != 0 || l || u) format = FALSE;

  if (argc < 2) Usage(1, "Illegal number of arguments.\n");

  num_arg = 1;
  while (num_arg < argc) {
    pipe =
      (!strcmp(argv[num_arg], "-") || !strcmp(argv[num_arg], "-0")) ? 1 : 0;

    img = Iread_image(argv[num_arg]);
    if (!img) Error(2, "Error reading file %s\n", argv[num_arg]);
    
    for (b = n ? n : 1; b <= (n ? n : Inbands(img)); b++) {
      band_minmax(img[b], &minval, &maxval);
      if (format) {
	printf("%s, band %d: Min %f, Max %f\n",
	       pipe ? "stdin" : argv[num_arg], b, minval, maxval);
      } else {
	if (name) printf("%s %d ", pipe ? "stdin": argv[num_arg], b);
	if (u == l) printf("%f %f\n", minval, maxval);
	else printf("%f\n", u ? maxval : minval);
      }
    }

    Idel_image(&img);
    num_arg++;
  }
  return(0);
}
