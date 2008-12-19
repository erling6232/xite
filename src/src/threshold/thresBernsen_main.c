/*

________________________________________________________________

        thresBernsen
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
#include <xite/threshold.h>
#include XITE_STDIO_H
#include <stdlib.h>
#include <xite/blab.h>
#include <xite/minmax.h>
#include <xite/readarg.h>
#include <xite/message.h>

/*P:thresBernsen*

________________________________________________________________

		thresBernsen
________________________________________________________________

Name:           thresBernsen - local variable treshold by the method of
                Bernsen.

Syntax:		thresBernsen [<option>...] <infile> <outfile> cmin [dx [dy]]

Description:	Local variable treshold by the method of Bernsen.

                &cmin
                Local contrast treshold.
                &dx
		Local window width, default 3.
                &dy
		Local window height, default dx.

Options:        &-lpv lpv
		Low pixel value, default 0.
		&-hpv hpv
		High pixel value, default 255.
		&-low
		Homogenous areas are assumed to be low, below
		the threshold. Default: above the threshold
 
		-&If the difference between the largest and the smallest pixel
		value within the 'dx'*'dy' window is greater than or equal to
		'cmin', the average of the two values is used as threshold.
		Pixels below this threshold are set to 'lpv', and pixels above
		are set to 'hpv'.

		If '-low' is set, pixels in homogenous areas (difference
		below 'cmin') are assumed to be below the threshold and are
		set to 'lpv', otherwise these pixels are set to 'hpv'.

Reference:	Bernsen, J: "Dynamic thresholding of grey-level images"
		Proc. of the 8th ICPR, Paris, Oct 1986, 1251-1255.

Example:        | thresBernsen mona.img monat.img 100
		| thresBernsen -lpv 1 -hpv 0 mona.img monat.img 75 35 55

Author:		Olav Borgli and Tor L|nnestad

Id:		$Id$
________________________________________________________________

*/

int main(int argc, char **argv)
{
  IMAGE img1,img2;
  int bn, dx, dy, cmin, lpv, hpv, low;
  char *args;

  Iset_message(TRUE);
  Iset_abort(TRUE);
  InitMessage(&argc, argv, xite_app_std_usage_text(
    "Usage: %s [-lpv <lpv>] [-hpv <hpv>] [-low] \n\
       <infile> <outfile> <cmin> [dx [dy]] \n"));

  if (argc == 1) Usage(1, NULL);
  args = argvOptions(argc, argv);

  lpv = atoi(read_switch(&argc, argv, "-lpv", 1, "0"));
  hpv = atoi(read_switch(&argc, argv, "-hpv", 1, "255"));
  low = (read_switch(&argc, argv, "-low", 0, NULL) != NULL);

  if (argc < 4) Usage(1, "Illegal number of arguments.\n");
  if (argc == 4) {
    dx = 3; dy = 3;
  } else if (argc == 5) {
    dx = atoi(argv[4]); dy = dx; }
  else {
    dx = atoi(argv[4]); dy = atoi(argv[5]);
  }

  cmin = atoi(argv[3]);

  img1 = Iread_image(argv[1]);
  img2 = Icopy_init(img1);

  for (bn=1; bn LE Inbands(img1); bn++)
    if (thresBernsen(img1[bn], img2[bn], cmin, dx, dy, lpv, hpv, low))
      Error(2, "Error in band %d.\n", bn);

  Ihistory(img2, argv[0], args);
  Iwrite_image(img2, argv[2]);

  return(0);
}
