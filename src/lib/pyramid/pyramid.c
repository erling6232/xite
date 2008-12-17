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
#include XITE_STDIO_H
#include <stdlib.h>
#include <xite/blab.h>
#include <xite/convolve.h>
#include <xite/readarg.h>
#include <xite/message.h>

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
