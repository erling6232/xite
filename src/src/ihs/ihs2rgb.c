

/*

________________________________________________________________

        ihs2rgb.c
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

#include <math.h>
#include <xite/includes.h>
#include <xite/biff.h>
#include <stdlib.h>
#ifdef HAVE_STDIO_H
#  include <stdio.h>
#endif
#include <xite/ihs.h>
#include <xite/message.h>



#ifndef MAIN

/*F:ihs2rgb_img*

________________________________________________________________

		ihs2rgb_img
________________________________________________________________

Name:		ihs2rgb_img - Convert image from IHS to RGB
Syntax:         | #include <xite/ihs.h>
		|
                | int ihs2rgb_img( IBAND i, IBAND h, IBAND s,
                |    IBAND r, IBAND g, IBAND b );
Description:	For each pixel convert (i, h, s) to (r, g, b).
Return value:	| 0 - Ok
                | 1 - Bad pixtyp
		| 2 - Bad size
See also:       ihs2rgb(1), rgb2ihs(1), rgb2ihs_img(3), ihs(3)
Author:		Otto Milvang
________________________________________________________________

*/


int ihs2rgb_img(IBAND i, IBAND h, IBAND s, IBAND r, IBAND g, IBAND b)
{
  int xsize, ysize, x, y;
  if (Ipixtyp(r) != Iu_byte_typ ||
      Ipixtyp(g) != Iu_byte_typ ||
      Ipixtyp(b) != Iu_byte_typ ||
      Ipixtyp(i) != Iu_byte_typ ||
      Ipixtyp(h) != Iu_byte_typ ||
      Ipixtyp(s) != Iu_byte_typ) return(1); 
  xsize = Ixsize(r);  
  ysize = Iysize(r);
  if (Ixsize(g) != xsize || Iysize(g) != ysize ||
      Ixsize(b) != xsize || Iysize(b) != ysize ||
      Ixsize(i) != xsize || Iysize(i) != ysize ||
      Ixsize(h) != xsize || Iysize(h) != ysize ||
      Ixsize(s) != xsize || Iysize(s) != ysize) return(2);
  for(y=1; y<=ysize; y++)
    for(x=1; x<=xsize; x++)
      ihs2rgb_byte( i[y][x],  h[y][x],  s[y][x], 
		   &r[y][x], &g[y][x], &b[y][x]);
  return(0);
}

#endif /* not MAIN */



/*P:ihs2rgb*

________________________________________________________________

		ihs2rgb
________________________________________________________________

Name:		ihs2rgb - Convert from IHS to RGB
Syntax:		| ihs2rgb <inimage> <outimage>
Description:	'inimage' must be an 3 band image. Assume
                band 1 as intesity, band 2 as hue and band 3 as saturation.
                For each pixel convert (i, h, s) to (r, g, b).		
Author:		Otto Milvang
See also:       ihs2rgb_img(3), rgb2ihs(1), rgb2ihs_img(3), ihs(3)
Id: 		$Id$
________________________________________________________________

*/


#ifdef MAIN


int main(int argc, char **argv)
{
  IMAGE img;
  int stat;
  Iset_message(TRUE);
  Iset_abort(TRUE);
  InitMessage(&argc, argv, xite_app_std_usage_text(
    "Usage: %s  <inimage> <outimage>\n"));

  if (argc == 1) Usage(1, NULL);
  if (argc != 3) Usage(1, "Illegal number of arguments.\n");

  img = Iread_image(argv[1]);
  if (Inbands(img) != 3)
    Error(2, "Inimage must be a 3-band image.\n");

  stat = ihs2rgb_img(img[1], img[2], img[3], img[1], img[2], img[3]);
  if (stat == 1) Error(2, "All bands must be usigned byte band.\n");
  if (stat == 2) Error(2, "All bands must be of same size.\n");

  Ihistory(img, argv[0], "");
  Iwrite_image(img, argv[2]);
  return(0);
}

#endif /* MAIN */
