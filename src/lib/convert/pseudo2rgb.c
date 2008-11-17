
/*C*

________________________________________________________________

        pseudo2rgb
        $Id$
        Copyright 1991, Blab, UiO
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
#include <xite/convert.h>
#include <xite/color.h>
#include <xite/message.h>
#include <xite/readarg.h>
#include XITE_STDIO_H



/*F:pseudo2rgb*

________________________________________________________________

		pseudo2rgb
________________________________________________________________

Name:		pseudo2rgb - convert band + colortable to rgb image

Syntax:         | #include <xite/convert.h>
		|
                | int pseudo2rgb( IBAND band, IMAGE img,
                |    Color_tab coltab, int ctlen );

Description:	'pseudo2rgb' converts the input band to a three band 
                rgb output image according to the specified color table.

Restrictions:	Accepts only UNS_BYTE images. All bands of the output
                image should have (at least) the same size as the input
		band.

Return value:   | 0 => ok
		| 1 => bad pixel type input band
		| 2 => less than 3 bands in output image
		| 3 => bad pixel type in output image
		| 4 => output image less than input band
		| 5 => ctlen has bad value (<0 or >256)

Author:		Tor Lønnestad, BLAB, ifi, UiO

Revised:        Svein Bøe

Id:		$Id$
________________________________________________________________

*/

int pseudo2rgb(IBAND band, IMAGE img, Color_cell *coltab, int ctlen)
{
  int x, y, g, c, xsize, ysize;
  int ct[256][3];
  unsigned long pix;

  if (Ipixtyp(band) != Iu_byte_typ)
    return(Error(1, "Input band must have pixeltype unsigend byte.\n"));
  if (Inbands(img) < 3)
    return(Error(2, "Less than 3 bands in output image.\n"));
  for (c=1; c<=3; c++)
    if (Ipixtyp(img[c]) != Iu_byte_typ)
      return(Error(3, "Bad pixeltype in output image.\n"));
  /* test band sizes */
  xsize = Ixsize(band);
  ysize = Iysize(band);
  for (c=1; c<=3; c++)
    if ((Ixsize(img[c]) < xsize) || (Iysize(img[c]) < ysize))
      return(Error(4, "Output image has smaller size than input band.\n"));

  if ((ctlen < 0) || (ctlen > 256))
    return(Error(5, "Illegal length of colortable.\n"));

  for (g=0; g <= 255; g++)
    for (c=0; c <= 2; c++)
      ct[g][c] = 0;

  for (g=0; g < ctlen; g++) {
    pix = PseudoBasePart(coltab[g].pixel);
    ct[pix][0] = coltab[g].red/256;
    ct[pix][1] = coltab[g].green/256;
    ct[pix][2] = coltab[g].blue/256;
  }

  for (y=1; y <= ysize; y++)
    for (x=1; x <= xsize; x++)
      for (c=0; c <= 2; c++)
        img[c+1][y][x] = ct[band[y][x]][c];

  return(0);
}
