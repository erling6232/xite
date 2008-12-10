
/*

________________________________________________________________

        haarTexture
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
#include <xite/haar.h>
#include XITE_STDIO_H
#include <stdlib.h>
#include <xite/blab.h>
#include <xite/message.h>
#include <xite/readarg.h>


#define ABS(v)    (((v) GT 0.0) ? (v) : -(v))
#define WL        32 /* word length */



/*P:haarTexture*

________________________________________________________________

		haarTexture
________________________________________________________________

Name:		haarTexture - texture extraction from 2D Haar transform

Syntax:		| haarTexture [-t <title>] <input> <output> <xp1> <yp1>
                |   [<xp2> <yp2>....[<xpn> <ypn>]..] 

Description:    Extracts texture information from the input image,
                returns it through the output image. The input is
		assumed to be the result of a forward 2D haar
		transform, 'haar2d'. Only the first band of the input
		image is used.

		'xp' (and 'yp') specifies the kind of textural
		information, and may be varied independently between
		1 (high frequency information) and log2(XSIZE)+1
		(low frequency), where XSIZE is the horisontal size
		of the image. The horisontal size of the texture
		element will be 2^xp except for xp = log2(XSIZE)+1,
                which will use a texture element of size XSIZE.
		Correspondingly for yp/YSIZE/vertical. 

		You can specify any number of 'xp', 'yp' pairs, the
		output image will have as many bands as such pairs.

See also:	haar2d(1)

Restrictions:   Input image must have bands of type IR_BAND, both horizontal
                and vertical size must be some power of two. The image must be
		the result of a forward 2D Haar transform.

		'xp' ('yp') must be between 1 and log2(XSIZE)+1
		(log2(YSIZE)+1).

Return value:   0 when ok

Author:		Tor Lønnestad, BLAB, Ifi, UiO

Examples:       Extraction of four bands with high frequency
                texture information:

		| haarTexture monaHaar.img monaTexture.img \\
		|     1 1  1 2  2 1  2 2

Id: 		$Id$
________________________________________________________________

*/

int main(int argc, char **argv)
{
  IMAGE i1, i2;
  int xp, yp, ac, bn, stat;
  char* t, *args;

  Iset_message(TRUE);
  InitMessage(&argc, argv, xite_app_std_usage_text(
    "Usage: %s [-t <title>] <input> <output> \n\
      <xp1> <yp1> [<xp2> <yp2> ...[<xpn> <ypn>]..]\n"));

  if (argc == 1) Usage(1, NULL);
  args = argvOptions(argc, argv);

  t = read_switch(&argc, argv, "-t", TRUE, "Haar-Texture");

  if ((argc < 5) || (! (argc BAND 1)))
    Usage(2, "Illegal number of arguments.\n");

  i1 = Iread_image(argv[1]);
  i2 = Init_image((argc-3)/2,t);
  ac = 3;
  for (bn=1; bn LE Inbands(i2); bn++) {
    i2[bn] = Imake_band(Ireal_typ, Ixsize(i1[1]), Iysize(i1[1]));
    xp = atoi(argv[ac++]);
    yp = atoi(argv[ac++]);
    if ( (stat = haarTexture((IR_BAND)i1[1], (IR_BAND)i2[bn], xp, yp)) )
      Error(stat, "Error %d in band %d\n", stat, bn);
  }

  Icopy_text(i1, i2);
  Ihistory(i2, argv[0], args);
  Iwrite_image(i2, argv[2]);

  return(0);
}
