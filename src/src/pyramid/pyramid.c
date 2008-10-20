/*

________________________________________________________________

        pyramid
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
#include <xite/pyramid.h>
#ifdef HAVE_STDIO_H
#  include <stdio.h>
#endif
#include <stdlib.h>
#include <xite/blab.h>
#include <xite/convolve.h>
#include <xite/readarg.h>
#include <xite/message.h>

#ifndef MAIN

#ifndef MIN
# define MIN(a,b) (((a)<(b)) ? (a) : (b))
#endif



/*F:pyramidAver2=pyramid*/
/*F:pyramidRank2=pyramid*/
/*F:pyramidGauss5=pyramid*/
/*F:mkPyramidAver2=pyramid*/
/*F:mkPyramidRank2=pyramid*/
/*F:mkPyramidGauss5=pyramid*/
/*F:pyramid*

________________________________________________________________

		pyramid
________________________________________________________________

Name:		pyramid, pyramidAver2, pyramidRank2, pyramidGauss5, 
                mkPyramidAver2, mkPyramidRank2, mkPyramidGauss5  -
		create a pyramidal raster data structure

Syntax:         | #include <xite/pyramid.h>
		|
                | int pyramidAver2( IBAND b1, IBAND b2 );
		|
                | int pyramidRank2( IBAND b1, IBAND b2,
                |    int rank );
		|
                | int pyramidGauss5( IBAND b1, IBAND b2,
                |    ISS_BAND conv );
		|
                | int mkPyramidAver2( IBAND b, IMAGE* i, int n );
		|
                | int mkPyramidRank2( IBAND b, IMAGE* i,
                |    int rank, int n );
                |
                | int mkPyramidGauss5( IBAND b, IMAGE* i,
                |    ISS_BAND conv, int n );
                |
Description:    'pyramidAver2' performs a 2 by 2 average of pixels in
		'b1' and places the result in 'b2'. The 2 by 2 window 
		is then moved two positions, and the process is 
		repeated over the whole band. 'b2' should thus be half
		the size of 'b1' vertically as well as horizontally.
		If this is not the case, only pixels occurring in
		'b2', and with four "subpixels" in 'b1', are handled.
		The final division by four will always truncate.

		'pyramidRank2' sorts the four pixel values in a 2 by 2
		window in 'b1', looks for the 'rank' ''th value in the
		sorted sequence, and puts this single value into
		'b2'. The 2 by 2 window is then moved two positions,
		and the process is repeated over the whole band.
		'b2' should thus be half the size of 'b1' vertically
		as well as horizontally. If this is not the case,
		only pixels occurring in 'b2', and with four
		"subpixels" in 'b1', are handled.
		
		'pyramidGauss5' performs a 5 by 5 gaussian filtering
                of pixels in 'b1' with the filter 'conv' and subsampels
                the image by 2. The result is placed in 'b2'. 'b2' should 
                thus be half the size of 'b1' vertically as well as 
                horizontally. 

		'mkPyramidAver2' creates a pyramid data structure with 'n'
		levels from the input band 'b' and returns the result as the
		output image 'i'. The bottom layer will have the same size as
		the input band rounded down to nearest even, and each layer
		upwards the pyramid will have half the size of the previous
		(integer division by 2). The routine uses pyramidAver2
		repeatedly. With 'n' equal to zero, use the maximum number
		of levels.

		'mkPyramideRank2' creates a pyramid data structure with 'n'
		levels from the input band 'b' and returns the result as the
		output image 'i'. The bottom layer will have the same size as
		the input band rounded down to nearest even, and ach layer
		upwards the pyramid will have half the size of the previous
		(integer division by 2). From each 2 by 2 window in one layer,
		the smallest (rank=1), second smallest (rank=2), second
		largest (rank=3) or largest (rank=4) pixel value will be
		inserted in the layer above. The 'rank' parameter should thus
		be in 1..4. The routine uses pyramidRank2 repeatedly. With 'n'
		equal to zero, use the maximum number of levels.

		'mkPyramidGauss5' creates a pyramid data structure with 'n'
		levels from the input band 'b' and returns the result as the
		output image 'i'. The bottom layer will have the same size as
		the input band rounded down to nearest even, and each layer
		upwards the pyramid will have half the size of the previous
		(integer division by 2). The routine uses pyramidGauss5
		repeatedly. With 'n' equal to zero, use the maximum number of
		levels.

Restrictions:   Input and output pixel types must be unsigned byte.

Return value:   | 0 => ok
		| 1 => bad pixel type b1
		| 2 => bad pixel type b2
		| 3 => bad rank value (outside 1..4)

Author:		Tor Lønnestad, BLAB, ifi, UiO.

Revised:        Helene Schulerud, BLAB, ifi, UiO.  

Id:		$Id$
________________________________________________________________

*/
 
int pyramidAver2(IBAND b1, IBAND b2)
{
   int x, y, xx, yy, xsize, ysize, sum;

   if (Ipixtyp(b1) NE Iu_byte_typ)
     return(Error(1,
		  "pyramidAver2: Input pixel type must be unsigned byte.\n"));
   if (Ipixtyp(b2) NE Iu_byte_typ)
     return(Error(2,
		  "pyramidAver2: Output pixel type must be unsigned byte.\n"));

   xsize = MIN(Ixsize(b1)/2, Ixsize(b2));
   ysize = MIN(Iysize(b1)/2, Iysize(b2));

   FOR ((y=1, yy=1); y LE ysize; (y++, yy+=2))
     FOR ((x=1, xx=1); x LE xsize; x++)
       sum = b1[yy][xx++];
       sum += b1[yy++][xx--];
       sum += b1[yy][xx++];
       sum += b1[yy--][xx++];
       b2[y][x] = (UNS_BYTE)(sum>>2)
     ENDFOR;
   ENDFOR;
   return 0;
}





int pyramidRank2(IBAND b1, IBAND b2, int rank)
{
   int x, y, xx, yy, xsize, ysize;
   int pv, *arr, sortArr[7];

   if (Ipixtyp(b1) NE Iu_byte_typ)
     return(Error(1,
		  "pyramidRank2: Input pixel type must be unsigned byte.\n"));

   if (Ipixtyp(b2) NE Iu_byte_typ)
     return(Error(2,
		  "pyramidRank2: Output pixel type must be unsigned byte.\n"));

   if ((rank LT 1) OR (rank GT 4))
     return(Error(3, "pyramidRank2: Illegal rank value.\n"));

   xsize = MIN(Ixsize(b1)/2, Ixsize(b2));
   ysize = MIN(Iysize(b1)/2, Iysize(b2));
   rank--;

   FOR ((y=1, yy=1); y LE ysize; (y++, yy+=2))
     FOR ((x=1, xx=1); x LE xsize; x++)
       arr = &(sortArr[3]);
       *arr = b1[yy][xx++];
       pv = b1[yy++][xx--];
       if (pv LT *arr) *(--arr) = pv; else arr[1] = pv;
       pv = b1[yy][xx++];
       IF (pv LT *arr)
         *(--arr) = pv; 
       ELSE
         IF (pv LT arr[1])
           arr[2] = arr[1]; arr[1] = pv;
         ELSE
           arr[2] = pv;
         ENDIF;
       ENDIF;
       pv = b1[yy--][xx++];
       IF (pv LT arr[1])
         IF (pv LT *arr)
           *(--arr) = pv; 
         ELSE
           --arr; *arr = arr[1]; arr[1] = pv;
         ENDIF;
       ELSE
         IF (pv LT arr[2])
           arr[3] = arr[2]; arr[2] = pv;
         ELSE
           arr[3] = pv;
         ENDIF;
       ENDIF;

       b2[y][x] = arr[rank];
     ENDFOR;
   ENDFOR;
   return(0);
}





int pyramidGauss5(IBAND b1, IBAND b2, ISS_BAND conv)
{
   int x, y, xx, yy, xsize, ysize, verb;
   double scale,offset;
   IBAND b3;
   
   scale  = 0.0025;
   offset = 0.0;
   verb   = 0;

   if (Ipixtyp(b1) NE Iu_byte_typ)
     return(Error(1,
	    "pyramidGauss5: Input pixel type must be unsigned byte.\n"));

   if (Ipixtyp(b2) NE Iu_byte_typ)
     return(Error(2,
	    "pyramidGauss5: Output pixel type must be unsigned byte.\n"));

   b3 = Imake_band(Iu_byte_typ, Ixsize(b1), Iysize(b1));

   convolve(b1,b3,conv,scale,offset,verb);

   xsize = MIN(Ixsize(b1)/2, Ixsize(b2));
   ysize = MIN(Iysize(b1)/2, Iysize(b2));

   FOR ((y=1, yy=1); y LE ysize; (y++, yy+=2))
     FOR ((x=1, xx=1); x LE xsize; (x++, xx+=2))
       b2[y][x] =  b3[yy][xx] ;
     ENDFOR;
   ENDFOR;
   return 0;
}




int mkPyramidAver2(IBAND b, IMAGE *i, int n)
{
   int nbands, xsize, ysize, size, nb;

   if (Ipixtyp(b) != Iu_byte_typ)
     return(Error(1,
	    "mkPyramidAver2: Input pixel type must be unsigned byte.\n"));

   xsize = (Ixsize(b)/2)*2;
   ysize = (Iysize(b)/2)*2;
   size  = MIN(xsize, ysize);

   if (n == 0)
     for (nbands=1; size > 1; size /= 2) nbands++;
   else
     nbands = n;

   if ((xsize >> (nbands - 1)) < 1 || (ysize >> (nbands - 1)) < 1) {
     Warning(1,
	     "mkPyramidAver2: Too many levels specified. Neglecting some.\n");
     size   = MIN(xsize, ysize);
     for (nbands=1; size > 1; size /= 2) nbands++;
   }

   *i = Init_image(nbands, "Pyramid");
   nb = 1;

   /* create bottom layer */
   (*i)[nb] = Imake_band(Iu_byte_typ, xsize, ysize);
   Icopy_band(b, (*i)[nb]);

   /* grow upwards */
   FOR (nb=2; nb<=nbands; nb++)
     xsize /= 2;
     ysize /= 2;
     (*i)[nb] = Imake_band(Iu_byte_typ, xsize, ysize);
     pyramidAver2((*i)[nb-1], (*i)[nb]);
   ENDFOR;
   return(0);
}




int mkPyramidRank2(IBAND b, IMAGE *i, int rank, int n)
{
   int nbands, size, xsize, ysize, nb;

   if (Ipixtyp(b) NE Iu_byte_typ)
     return(Error(1,
	    "mkPyramidRank2: Input pixel type must be unsigned byte.\n"));

   xsize = (Ixsize(b)/2)*2;
   ysize = (Iysize(b)/2)*2;
   size=MIN(xsize, ysize);

   if(n == 0)
     for (nbands=1; size GT 1; size /= 2) nbands++;
   else
     nbands = n;

   if ((xsize >> (nbands - 1)) < 1 || (ysize >> (nbands - 1)) < 1) {
     Warning(1,
	     "mkPyramidAver2: Too many levels specified. Neglecting some.\n");
     size   = MIN(xsize, ysize);
     for (nbands=1; size > 1; size /= 2) nbands++;
   }
   *i = Init_image(nbands, "Pyramid");
   nb = 1;

   /* create bottom layer */
   (*i)[nb] = Imake_band(Iu_byte_typ, xsize, ysize);
   Icopy_band(b, (*i)[nb]);

   /* grow upwards */
   FOR (nb=2; nb<=nbands; nb++)
     xsize /= 2;
     ysize /= 2;
     (*i)[nb] = Imake_band(Iu_byte_typ, xsize, ysize);
     pyramidRank2((*i)[nb-1], (*i)[nb], rank);
   ENDFOR;
   return(0);
}




int mkPyramidGauss5(IBAND b, IMAGE *i, ISS_BAND conv, int n)
{
   int nbands, xsize, ysize, size, nb;

   if (Ipixtyp(b) NE Iu_byte_typ)
     return(Error(1,
	    "mkPyramidGauss5: Input pixel type must be unsigned byte.\n"));

   xsize = (Ixsize(b)/2)*2;
   ysize = (Iysize(b)/2)*2;
   size=MIN(xsize, ysize);

   if(n == 0)
     for (nbands=1; size GT 1; size /= 2) nbands++;
   else
     nbands = n;

   if ((xsize >> (nbands - 1)) < 1 || (ysize >> (nbands - 1)) < 1) {
     Warning(1,
	     "mkPyramidAver2: Too many levels specified. Neglecting some.\n");
     size   = MIN(xsize, ysize);
     for (nbands=1; size > 1; size /= 2) nbands++;
   }

   *i = Init_image(nbands, "Pyramid");
   nb = 1;

   /* create bottom layer */
   (*i)[nb] = Imake_band(Iu_byte_typ, xsize, ysize);
   Icopy_band(b, (*i)[nb]);

   /* grow upwards */
   FOR (nb=2; nb<= nbands; nb++)
     xsize /= 2;
     ysize /= 2;
     (*i)[nb] = Imake_band(Iu_byte_typ, xsize, ysize);
     pyramidGauss5((*i)[nb-1], (*i)[nb], conv);
   ENDFOR;

   return(0);
}

#endif /* not MAIN */



/*P:pyramid*

________________________________________________________________

		pyramid
________________________________________________________________

Name:		pyramid - create a pyramid data structure

Syntax:		| pyramid [-l] [-s] [-r <rank>] [-g] [-n <levels>] \\
                |     <inimage> <outimage>

Description:    Create a pyramid data structure from the first band
                of the input image 'inimage', write the resulting
		image to 'outimage'. By default, the bottom layer
		will have the same size as the input image, (-1 if
		odd), and the resampling technique is two by two
		average.

Options:        &-l
		Let the bottom band have the smallest square
		power of two size larger than the input.

		&-s
		Let the bottom band have the largest square
		power of two size smaller than the input.

		&-r rank
		Use rank='rank'; the rank''th sorted pixel value
		in the window as resampling result ('rank' = 1..4).
                
		&-g
		Use Gaussian pyramid, ref. IEEE trans. on Comm.
                1983, no.4, page 532, with a=0.4.
		1983, no.4, page 532 with a=0.4.

		&-n levels
		Spesify the number of 'levels' wanted in 
		the pyramid (default: all possible levels).

		-&Use only one of '-l' and '-s' or none. If none, then 
                integer division by 2 is performed as far as possible. 

                If none of the options are used, mean pyramid is performed.

Restrictions:   Input image must have pixel type unsigned byte.

Author:		Tor Lønnestad, BLAB, Ifi, UiO

Revised:        Helene Schulerud, BLAB, Ifi, UiO,
                Svein Bøe, Blab, Ifi, UiO

Id:             $Id$

Examples:	| pyramid mona.img monaPyr.img
		| pyramid -l mona.img mona.PyrL.img
		| pyramid -s -r 4 mona.img mona.PyrS.img
                | pyramid -g mona.img mona.PyrG.img
________________________________________________________________

*/

#ifdef MAIN

static int initconv(ISS_BAND c)
{
   c[1][1] = 1;
   c[1][2] = 5;
   c[1][3] = 8;
   c[1][4] = 5;
   c[1][5] = 1;
   c[2][1] = 5;
   c[2][2] = 25;
   c[2][3] = 40;
   c[2][4] = 25;
   c[2][5] = 5;
   c[3][1] = 8;
   c[3][2] = 40;
   c[3][3] = 64;
   c[3][4] = 40;
   c[3][5] = 8;
   c[4][1] = 5;
   c[4][2] = 25;
   c[4][3] = 40;
   c[4][4] = 25;
   c[4][5] = 5;
   c[5][1] = 1;
   c[5][2] = 5;
   c[5][3] = 8;
   c[5][4] = 5;
   c[5][5] = 1;

   return 0;
}

int main(int argc, char **argv)
{
  IMAGE ii, oi;
  IBAND band;
  ISS_BAND conv;
  int smaller, larger, rank, gaussian, po2, xsize, ysize, nlevels;
  char arg[50];
  
  Iset_message(TRUE);
  Iset_abort(TRUE);
  InitMessage(&argc, argv, xite_app_std_usage_text(
    "Usage: %s [-s] [-l] [-r <rank>] [-g] [-n <levels>] \n\
       <inimage> <outimage>\n"));

  if (argc == 1) Usage(1, NULL);

  smaller  = read_bswitch(&argc, argv, "-s");
  larger   = read_bswitch(&argc, argv, "-l");
  rank     = read_iswitch(&argc, argv, "-r", 0);
  nlevels  = read_iswitch(&argc, argv, "-n", 0);
  gaussian = read_bswitch(&argc, argv, "-g");

  if (argc != 3) Usage(1, "Illegal number of arguments.\n");

  ii = Iread_image(argv[1]);
  if (Ipixtyp(ii[1]) != Iu_byte_typ)
    Error(2, "Can only handle unsigned byte pixels.\n");

  xsize = Ixsize(ii[1]);
  ysize = Iysize(ii[1]);
  if (smaller) {/* reduce to quadratic size, power of 2 */
    for (po2=1; (po2 < xsize) && (po2 < ysize); po2 *= 2) ;
    po2 /= 2;
    band = Imake_subband(ii[1], 1, 1, po2, po2);
  } else if (larger) {
    for (po2=1; (po2 < xsize) || (po2 < ysize); po2 *= 2) ;
    band = Imake_band(Iu_byte_typ, po2, po2);
    Icopy_band(ii[1], band);
  } else {
    band = ii[1];
  }

  if (rank) {
    mkPyramidRank2(band, &oi, rank, nlevels);
    sprintf(arg, " with rank = %d\n", rank);
  } else if (gaussian) {
    conv = (ISS_BAND)Imake_band(Is_short_typ,5,5);
    initconv(conv);
    mkPyramidGauss5(band, &oi, conv, nlevels);
    sprintf(arg, " using 5x5 Gauss\n");
  } else {
    mkPyramidAver2(band, &oi, nlevels);
    sprintf(arg, " using 2x2 averaging\n");
  }

  Icopy_text(ii, oi);
  Ihistory(oi, argv[0], arg);
  Iwrite_image(oi, argv[2]);  

  return(0);
}

#endif /* MAIN */
