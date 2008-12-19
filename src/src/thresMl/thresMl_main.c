

/*C*

________________________________________________________________

        thresMl
        $Id$
        Copyright 1992, Blab, UiO
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

#include <stdlib.h>
#include <xite/includes.h>
#include <xite/biff.h>
#include <xite/blab.h>
#include <xite/histo.h>
#include <xite/message.h>
#include <xite/readarg.h>
#include <xite/thresMl.h>
#include XITE_MALLOC_H

/*P:thresMl*

________________________________________________________________

		thresMl
________________________________________________________________

Name:		thresMl - multi level supervised thresholding

Syntax:		| thresMl [-mean] [-med] [-scale <scale>] \\
                |     <inimage> <outimage> \\
		|     <thres1> [<thres2> [<thres3>......]    

Description:    Performes multi level thresholding at the specified
                thresholds. You may specify one or more thresholds.
		You should only use one of the options.

Options:        &-mean
                Output is mean of pixels between two thresholds.

		&-med
		Output is median of pixels between two thresholds.

		&-scale scale
		Output is threshold-level * 'scale'. EX: pixels
		between 3rd and 4th threshold get value 3*'scale'.

		If 'scale' is too large, it is adjusted down to make 255
		the maximum output value. Default 'scale' is too large.

See also:	thresMlAppScale(3), thresMlAppMean(3), thresMlAppMedian(3),
                thresMlApply(3), histoCentroid(3), thresMlCentroid(3),
                thresMlCentroid(1), mkComCur(3), thresMlComCur(3),
                thresMlComCur(1), thresMlReddi(3), thresMlReddi(1),
                thresMlWaHa(3), thresMlWaHa(1)

Return value:   | 0 => ok
                | 2 => Bad number of arguments

Author:		Tor Lønnestad, BLAB, Ifi, UiO

Examples:       | thresMl -med mona.img monaThMl.img 60 90 150
                | thresMl mona.img monaThMl.img 60 90 150
                | thresMl -scale 50 mona.img monaThMl.img 60 90 150

Id: 		$Id$
________________________________________________________________

*/

int main(int argc, char **argv)
{
  IMAGE img;
  double scale;
  int mean, median, i, bn, nt;
  int *thress;
  histogram h;
  char *args;

  InitMessage(&argc, argv, xite_app_std_usage_text(
    "Usage: %s [-mean] [-med] [-scale <scale>] \n\
           <inimage> <outimage> <thres1> [<thres2> [<thres3>......]\n"));
  Iset_message(TRUE);
  Iset_abort(TRUE);

  if (argc==1) Usage(1, NULL);
  args = argvOptions(argc, argv);

  mean   = read_bswitch(&argc, argv, "-mean");
  median = read_bswitch(&argc, argv, "-med");
  scale  = read_dswitch(&argc, argv, "-scale", 1000.0);

  if (argc<4) Usage(2, "Illegal number of arguments.\n");

  nt = argc-3;
  thress = (int*)malloc(nt*sizeof(int));
  for (i=0; i LT nt; i++)
    thress[i] = atoi(argv[i+3]);

  img = Iread_image(argv[1]);
  for (bn=1; bn LE Inbands(img); bn++) {
    if (mean) {
      mkHisto(img[bn], h);
      thresMlAppMean(img[bn], img[bn], thress, nt, h);
    } else if (median) {
      mkHisto(img[bn], h);
      thresMlAppMedian(img[bn], img[bn], thress, nt, h);
    } else {
      thresMlAppScale(img[bn], img[bn], thress, nt, scale);
    }
  }

  Ihistory(img, argv[0], args);
  Iwrite_image(img, argv[2]);

  return(0);
}
