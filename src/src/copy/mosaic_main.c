
/*

________________________________________________________________

        mosaic
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



/*P:mosaic*

________________________________________________________________

		mosaic
________________________________________________________________

Name:		mosaic - combine several images to a new one band image

Syntax:		| mosaic -auto [<option>...] <img1> ... <imgn> <outimage>
		| mosaic [<option>...] <img1> <xstart> <ystart> ...
		|    <imgn> <xstart> <ystart> <outimage>

Description:    All bands from all input images are copied into a new
		one-band image or a multi-band image, copied in the sequence
		they are given as arguments.

		'img1', .., 'imgn' are input images.

		'xstart' and 'ystart' give new positions for all bands in the
		new band. Should not be smaller than 1.

Options:        &-auto
                Place the bands according to their individual xstart/ystart
		attributes. Otherwise: Use the xstart/ystart arguments on
		the command line.

		&-multi
		If any of the input images is multiband, then the output
		image will also be multiband. This means that band1 from
		all input images will be combined into band1 of the output,
		band2 of all input images combined into band2 of the output,
		and so on. The input images do not need to have the same
		number of bands.

		&-bg bg
		Use 'bg' as background pixel value. Default: 0

		&-xsize xsize
		Make the new horizontal band size at least as large as 'xsize'

		&-ysize ysize
		Make the new vertical band size at least as large as 'ysize'.
		Default size: Just as large as necessary.

Restrictions:   Works with all pixel types, but all bands of all images
                must have the same pixel type.

Author:		Tor Lønnestad, BLAB, Ifi, UiO.

Examples:       Make frame 10 pixels wide "painted" with pixel value 5:
		| mosaic -bg 5 -xsize 532 -ysize 532 mona.img 11 11 \\
		|   frame.img

		Put two 500x500 images side by side:
		| mosaic i1.img 1 1 i2.img 501 1 both.img

		Put two 500x500 bands (in the same image) side by side:
		| mosaic i1.img:1 1 1 i1.img:2 501 1 both.img

Id:		$Id$
________________________________________________________________

*/

#include <xite/includes.h>
#include <xite/biff.h>
#include <stdlib.h>
#include <xite/copy.h>
#ifdef HAVE_STDIO_H
#  include <stdio.h>
#endif
#include <xite/blab.h>
#include <xite/draw.h>
#include <xite/message.h>
#include <xite/readarg.h>
#ifdef HAVE_MALLOC_H
# include <malloc.h>
#endif

#define MAXIMAGES 100
#ifndef MAX
# define MAX(a,b) (((a)>(b)) ? (a) : (b))
#endif

/*
---------------------------------------------------------
     readAllImages : read all images and startpoints
---------------------------------------------------------
*/
static void readAllImages(IMAGE *images, int aut, int argc, char *argv[],
int *nimages)
{
   IMAGE image;
   int bandnr, ac=1, xstart, ystart;

   WHILE (ac LT argc-1)
     image = Iread_image(argv[ac++]);
     IF (NOT aut)
       IF (sscanf(argv[ac++],"%d%*s",&xstart) NE 1)
         Warning(1, "Bad xstart specification");
         xstart = 1;
       ENDIF;
       IF (sscanf(argv[ac++],"%d%*s",&ystart) NE 1)
         Warning(2, "Bad ystart specification");
         ystart = 1;
       ENDIF;
       for (bandnr=1; bandnr LE Inbands(image); bandnr++)
         Iset_start(image[bandnr], xstart, ystart);
     ENDIF;
     images[++(*nimages)] = image;
   ENDWHILE;
}

/*
---------------------------------------------------------
     testPixtyp : test if all bands have the same pixel type
---------------------------------------------------------
*/
static void testPixtyp(IMAGE *images, int nimages)
{
  int in, bn, pt;
  
  pt = Ipixtyp(images[1][1]);
  for (in=1; in <= nimages; in++)
    for (bn=1; bn <= Inbands(images[in]); bn++)
      if (Ipixtyp(images[in][bn]) != pt)
        exit(Error(3, "Bands have different pixel types\n"));
}

/*
---------------------------------------------------------
     newSize : find the required size of the new band
---------------------------------------------------------
*/

static void newSize(IMAGE *images, int nimages, int *xs, int *ys, int *nbands)
{
   IBAND b;
   int imgnr, bandnr, nb;

   *xs = 0;
   *ys = 0;
   *nbands = 0;
   for (imgnr=1; imgnr LE nimages; imgnr++) {
     nb = Inbands(images[imgnr]);
     if (nb > *nbands) *nbands = nb;

     for (bandnr=1; bandnr LE nb; INC bandnr) {
       b = images[imgnr][bandnr];
       if ((Ixstart(b)+Ixsize(b)-1) GT *xs) *xs = Ixstart(b)+Ixsize(b)-1;
       if ((Iystart(b)+Iysize(b)-1) GT *ys) *ys = Iystart(b)+Iysize(b)-1;
     }
   }
 }

/*
---------------------------------------------------------
     main
---------------------------------------------------------
*/

int main(int argc, char *argv[])
{
   IMAGE *images, Output;
   IBAND b;
   int multi, aut, bg, nimages=0, xsize, ysize, 
       xs, ys, imgnr, bandnr, pt, nbands;

   Iset_message(TRUE);
   Iset_abort(TRUE);
   InitMessage(&argc, argv, xite_app_std_usage_text(
     "Usage: %s -auto [-bg <bg>] [-xsize <xsize>] [-ysize <ysize>]\n\
        <img1> <img2> ... <imgn> <outimage>\n or:\n\
             %s [-bg <bg>] [-xsize <xsize>] [-ysize <ysize>]\n\
        <img1><xstart><ystart> <img2><xstart><ystart>\n\
         ... <imgn><xstart><ystart> <outimage>\n"));

   if (argc == 1) Usage(1, NULL);

   aut    = read_bswitch(&argc, argv, "-auto");
   bg     = read_iswitch(&argc, argv, "-bg", 0);
   multi  = read_bswitch(&argc, argv, "-multi");
   xsize  = read_iswitch(&argc, argv, "-xsize", 0);
   ysize  = read_iswitch(&argc, argv, "-ysize", 0);
   images = (IMAGE*) malloc(MAXIMAGES*sizeof(IMAGE*));

   if ((aut && (argc < 3))
       || ((! aut) && ((argc < 5) || ((argc+1) MOD 3))))
     Usage(2, "Illegal number of arguments.\n");

   readAllImages(images, aut, argc, argv, &nimages);
   newSize(images, nimages, &xs, &ys, &nbands);
   if (!multi) nbands = 1;
   pt = Ipixtyp(images[1][1]);
   xsize = MAX(xsize, xs);
   ysize = MAX(ysize, ys);
   
   Output = Imake_image(nbands, "", pt, xsize, ysize);
   FOR (bandnr=1; bandnr LE nbands; INC bandnr)
     drawBand(Output[bandnr], (double)bg, (double)0.0);
   ENDFOR

   /* Copy bands */
   FOR (imgnr=1; imgnr LE nimages; INC imgnr)
     FOR (bandnr=1; bandnr LE Inbands(images[imgnr]); INC bandnr)
       b = Imake_subband(Output[multi ? bandnr : 1],
             Ixstart(images[imgnr][bandnr]),
             Iystart(images[imgnr][bandnr]),
             Ixsize(images[imgnr][bandnr]),
             Iysize(images[imgnr][bandnr]));
       Icopy_band(images[imgnr][bandnr], b);
       Idel_band(&b);
     ENDFOR
   ENDFOR
   Ihistory(Output, argv[0], "");
   Iwrite_image(Output,argv[argc-1]);
   return(0);
}
