
/*

________________________________________________________________

        threshold
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
#ifdef HAVE_STDIO_H
#  include <stdio.h>
#endif
#include <stdlib.h>
#include <xite/blab.h>
#include <xite/message.h>
#include <xite/readarg.h>



#ifndef MAIN

#ifndef MIN
# define MIN(a,b) (((a) < (b)) ? (a) : (b))
#endif

/*F:thresholdSpOut=threshold*/
/*F:threshold*

________________________________________________________________

		threshold
________________________________________________________________

Name:		threshold, thresholdSpOut - threshold a band with specified
                threshold

Syntax:         | #include <xite/threshold.h>
		|
                | int threshold( IBAND b1, IBAND b2, int t );
                |
                | int thresholdSpOut( IBAND b1, IBAND b2, int t,
                |    int lpv, int hpv );
Description:    'threshold' and 'thresholdSpOut' thresholds 'b1' into
                'b2' with threshold 't'.

                | if (b1[x,y] LE t) b2[x,y] = lpv; else b2[x,y] = hpv;

		'threshold' uses 0 for low pixel value ('lpv') and 255
		for high pixel value ('hpv'), while 'thresholdSpOut'
		uses the supplied arguments.
                Only largest common rectangle of 'b1' and 'b2'
		is thresholded. Accepts only UNS_BYTE pixles.
                Identical in- and out- parameters are legal.
		<lpv> and <hpv> should be in 1..255.

Return value:   | 0 => ok
                | 1 => Bad input pixel type
                | 2 => Bad output pixel type
		| 3 => Bad lpv value (only thresholdSpOut)
		| 4 => Bad hpv value (only thresholdSpOut)

Author:		Tor L|nnestad, BLAB, IfI, UiO 

Id:             $Id$
________________________________________________________________

*/

int thresholdSpOut(IBAND b1, IBAND b2, int t, int lpv, int hpv)
{
  int x, y, xsize, ysize;

  if (Ipixtyp(b1) NE Iu_byte_typ)
    return(Error(1, "thresholdSpOut: Bad input pixel type\n"));
  if (Ipixtyp(b2) NE Iu_byte_typ)
    return(Error(2, "thresholdSpOut: Bad output pixel type\n"));
  if ((lpv LT 0) OR (lpv GT 255))
    return(Error(3, "thresholdSpOut: Bad lpv value\n"));
  if ((hpv LT 0) OR (hpv GT 255))
    return(Error(4, "thresholdSpOut: Bad hpv value\n"));

  xsize = MIN(Ixsize(b1),Ixsize(b2));
  ysize = MIN(Iysize(b1),Iysize(b2));

  for (y=1; y LE ysize; y++)
    for (x=1; x LE xsize; x++)
      b2[y][x] = ((b1[y][x] <= t) ? lpv : hpv);
  return(0);
}


/*
________________________________________________________________
*/

int threshold(IBAND b1, IBAND b2, int t)
{
  return(thresholdSpOut(b1, b2, t, 0, 255));
}

#endif /* not MAIN */



/*P:threshold*

________________________________________________________________

		threshold
________________________________________________________________

Name:		threshold - threshold an image with specified threshold

Syntax:		threshold [-l <lpv>] [-h <hpv>] \
                   <input image> <output image> <threshold>

Description:    'threshold' thresholds an image. Pixels below or
                equal to 'threshold' is set to 'lpv' (low pixel value),
		pixels above 'threshold' are set to 'hpv'. Default values
		0 and 255 can be modified with the -l and -h
		switches.

                | if (old_pixel_value LE threshold) new_pixel_value = lpv
                | else new_pixel_value = hpv;

Restrictions:   Acepts only UNS_BYTE pixels.

Author:		Tor Lønnestad, BLAB, IfI, UiO

Examples:       | threshold mona.img monath.img 110
                | threshold -h 1 mona.img monath.img 150
                | threshold -l 255 -h 0 mona.img monath.img 127

Id:             $Id$
________________________________________________________________

*/

#ifdef MAIN

int main(int argc, char **argv)
{
  IMAGE img;
  int bn, t, lpv, hpv, stat;
  char *options;

  InitMessage(&argc, argv, xite_app_std_usage_text(
    "Usage: %s [-l <lpv>] [-h <hpv>] <oldfile> <newfile> <t>\n"));
  Iset_message(TRUE);
  Iset_abort(TRUE);

  if (argc == 1) Usage(1, NULL);
  options = argvOptions(argc, argv);

  lpv = read_iswitch(&argc, argv, "-l", 0);
  hpv = read_iswitch(&argc, argv, "-h", 255);

  if (argc != 4) Usage(2, "Illegal number of arguments.\n");

  img = Iread_image(argv[1]);
  t   = atoi(argv[3]);

  for (bn=1; bn LE Inbands(img); bn++)
    if ((stat = thresholdSpOut(img[bn], img[bn], t, lpv, hpv)))
      Warning(stat, "Error in band %d.\n", bn);

  Ihistory(img, argv[0], options);
  Iwrite_image(img, argv[2]);

  return(0);
}

#endif /* MAIN */
