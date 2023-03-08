
/*C*
________________________________________________________________

        tiff2biff
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
#include <xite/xite_limits.h>
#include <xite/biff.h>
#include <xite/blab.h>
#include <xite/color.h>
#include <xite/message.h>
#include <xite/readarg.h>
#include <xite/tiff.h>
#include XITE_MALLOC_H
#include XITE_STDIO_H
#include XITE_TYPES_H



#define getbit(byte,bit)  ((byte & Ibitmask[bit]) ? 255 : 0)
#define getbitw(byte,bit) (255 - getbit(byte, bit))

/*L*

________________________________________________________________

		Byte2Bit
________________________________________________________________

Name:		Byte2Bit - Convert an unsigned byte to 8 bit.
Syntax:		static void Byte2Bit(c,bit,minisblack);
Description:	Convert an unsigned byte to 8 bit.
Return value:   
Author:		Helene Schulerud
________________________________________________________________

*/

static void Byte2Bit(unsigned char c, unsigned char *bit, int minisblack)
{	
  int i;

  if (minisblack)
    for (i=1; i<=8; i++)
      {
	bit[i] = (unsigned char) getbit(c,i);
      }
  else 
    for (i=1; i<=8; i++)
      {
	bit[i] = (unsigned char) getbitw(c,i);
      }

}
   
static void tiffCol2biffCol(uint16 *redmap, uint16 *greenmap, uint16 *bluemap, uint16 bps, Color_cell **coltab, int *numColors)
{
  int i;

  *numColors = (int) pow((double) 2.0, (double) bps);
  if ((*coltab =  (Color_cell *)
       malloc((size_t) (*numColors * sizeof(Color_cell)))) == NULL) {
    Error(2, "tiffCol2biffCol: Could not allocate memory for colortable.\n");
    return;
  }

  for (i = 0; i < *numColors; i++) {
    (*coltab)[i].pixel = RgbPixel(i);
    (*coltab)[i].red   = redmap[i];
    (*coltab)[i].green = greenmap[i];
    (*coltab)[i].blue  = bluemap[i];
  }
}




/*F:tiff2biff1=tiff2biff*/
/*F:tiff2biff1p=tiff2biff*/
/*F:tiff2biff2=tiff2biff*/
/*F:tiff2biff2p=tiff2biff*/
/*F:tiff2biff4=tiff2biff*/
/*F:tiff2biff4p=tiff2biff*/
/*F:tiff2biff8=tiff2biff*/
/*F:tiff2biff8p=tiff2biff*/
/*F:tiff2biff8c=tiff2biff*/
/*F:tiff2biff16=tiff2biff*/
/*F:tiff2biff16c=tiff2biff*/
/*F:tiff2biff32i=tiff2biff*/
/*F:tiff2biff32ic=tiff2biff*/
/*F:tiff2biff32fp=tiff2biff*/
/*F:tiff2biff32fpc=tiff2biff*/
/*F:tiff2biff64=tiff2biff*/
/*F:tiff2biff64c=tiff2biff*/
/*F:tiff2biff*

________________________________________________________________

		tiff2biff
________________________________________________________________

Name:		tiff2biff, tiff2biff1, tiff2biff1p, tiff2biff2, tiff2biff2p, 
                tiff2biff4, tiff2biff4p, tiff2biff8, tiff2biff8p,
		tiff2biff8c, tiff2biff16, tiff2biff16c, tiff2biff32i,
		tiff2biff32ic, tiff2biff32fp, tiff2biff32fpc, 
		tiff2biff64, tiff2biff64c -
		Convert a TIFF image to a BIFF image

Syntax:         | #include <xite/tiff.h>
                | 
                | IMAGE tiff2biff( TIFF* tif,
                |    Color_cell** coltab, int* numColors );
		|
                | void tiff2biff1( IBAND band, TIFF* tif,
                |    uint32 rows, uint32 cols, uint16 fillorder,
                |    int minisblack );
		|
                | void tiff2biff1p( IBAND band, TIFF* tif,
                |    uint32 rows, uint32 cols, uint16 fillorder,
                |    uint16* redmap, uint16* greenmap,
                |    uint16* bluemap, Color_cell** coltab,
                |    uint16 bps, int* numColors );
		|
                | void tiff2biff2( IBAND band, TIFF* tif,
                |    uint32 rows, uint32 cols, int minisblack,
                |    double maxval );
		|
                | void tiff2biff2p( IBAND band, TIFF* tif,
                |    uint32 rows, uint32 cols, uint16* redmap,
                |    uint16* greenmap, uint16* bluemap,
                |    Color_cell** coltab, uint16 bps,
                |    int* numColors );
		|
                | void tiff2biff4( IBAND band, TIFF* tif,
                |    uint32 rows, uint32 cols, int minisblack,
                |    double maxval );
		|
                | void tiff2biff4p( IBAND band, TIFF* tif,
                |    uint32 rows, uint32 cols, uint16* redmap,
                |    uint16* greenmap, uint16* bluemap,
                |    Color_cell** coltab, uint16 bps,
                |    int* numColors );
		|
                | void tiff2biff8( IBAND band, TIFF* tif,
                |    uint32 rows, uint32 cols, int minisblack,
                |    double maxval );
		|
                | void tiff2biff8p( IBAND band, TIFF* tif,
                |    uint32 rows, uint32 cols, uint16* redmap,
                |    uint16* greenmap, uint16* bluemap,
                |    Color_cell** coltab, uint16 bps,
                |    int* numColors );
		|
                | void tiff2biff8c( IMAGE img, TIFF* tif,
                |    uint32 rows, uint32 cols );
		|
                | void tiff2biff16( IUS_BAND band, TIFF* tif,
                |    uint32 rows, uint32 cols );
		|
                | void tiff2biff16c( IUS_IMAGE img, TIFF* tif,
                |    uint32 rows, uint32 cols );
		|
                | void tiff2biff32i( II_BAND band, TIFF* tif,
                |    uint32 rows, uint32 cols );
		|
                | void tiff2biff32ic( II_IMAGE img, TIFF* tif,
                |    uint32 rows, uint32 cols );
		|
                | void tiff2biff32fp( IR_BAND band, TIFF* tif,
                |    uint32 rows, uint32 cols );
		|
                | void tiff2biff32fpc( IR_IMAGE img, TIFF* tif,
                |    uint32 rows, uint32 cols );
		|
                | void tiff2biff64( ID_BAND band, TIFF* tif,
                |    uint32 rows, uint32 cols );
		|
                | void tiff2biff64c( ID_IMAGE img, TIFF* tif,
                |    uint32 rows, uint32 cols );
		|
Description:    'tiff2biff' is a wrapper function that decides which one of
                the routines below to use, depending on what kind the TIFF
		image is (bi-level, grayscale, palette or rgb). In the case
		of a palette image, the parameters 'coltab' and 'numColors'
		reflect the palette colormap.

                Byte-ordering between little-endian and big-endian machines
		is handled automatically.

		The conversions are according to TIFF specification 6.0
		and use the libtiff library by Sam Leffler. This library
		must exist on your system, it is not supplied with XITE.
		If you get problems with importing TIFF images from other
		applications, make sure those applications are also TIFF
		6.0 compliant.

		TIFF grayscale images:

		'tiff2biff1' converts a TIFF bi-level (black-and-white,
                1-bit samples) image to a BIFF unsigned byte band.

		'tiff2biff2' converts a TIFF grayscale (2-bit samples)
		image to a BIFF unsigned byte band.

		'tiff2biff4' converts a TIFF grayscale (4-bit samples)
		image to a BIFF unsigned byte band.

		'tiff2biff8' converts a TIFF grayscale (8-bit samples)
		image to a BIFF unsigned byte band.

		'tiff2biff16' converts an unsigned short TIFF image to an
		unsigned short BIFF band.

		'tiff2biff32i' converts a signed integer TIFF image to a
		signed integer BIFF band

		'tiff2biff32fp' converts a 32-bit floating point TIFF image
		to a REAL BIFF band

		'tiff2biff64' converts a 64-bit floating point TIFF image
		to a DOUBLE BIFF band

		TIFF palette (pseudocolor) images:

		'tiff2biff1p' converts a TIFF bi-level (1-bit samples)
		palette image to a BIFF unsigned byte band and colortable.

		'tiff2biff2p' converts a TIFF palette (2-bit samples)
		image to a BIFF unsigned byte band and colortable.

		'tiff2biff4p' converts a TIFF palette (4-bit samples)
		image to a BIFF unsigned byte band and colortable.

		'tiff2biff8p' converts a TIFF palette (8-bit samples)
		image to a BIFF unsigned byte band and colortable.

		TIFF rgb color images:

		'tiff2biff8c' converts an unsigned byte TIFF 3*8-bit rgb
		image to a BIFF unsigned byte three-band image

		'tiff2biff16c' converts an unsigned short TIFF 3*16-bit rgb
		image to an unsigned short three-band BIFF image

		'tiff2biff32ic' converts a signed integer 3*16-bit rgb
		TIFF image to a signed integer three-band BIFF image

		'tiff2biff32fpc' converts a 32-bit floating point rgb TIFF
		image to a REAL BIFF three-band image

		'tiff2biff64c' converts a 3*64-bit floating point rgb TIFF
		image to a DOUBLE BIFF three-band image

See also:       'tiff2biff(1)', 'biff2tiff(1)',  'biff2tiff(3)'

Author:		Helene Schulerud and Svein Bøe, Blab, Ifi, UiO

Id:             $Id$
________________________________________________________________

*/


void tiff2biff1(IBAND band, TIFF *tif, uint32 rows, uint32 cols, uint16 fillorder, int minisblack)
{
  uint32 row, cc, n, ccn, ccnn, i;	
  unsigned char *tf_buf, *cp, c;
  unsigned char bit[9];
  tsize_t tf_bytesperrow;

  tf_bytesperrow = TIFFScanlineSize(tif);

  tf_buf = (unsigned char *) malloc((size_t) TIFFScanlineSize(tif));
  if (tf_buf == NULL) 
    {
      TIFFError("tiff2biff1", "No space for scanline buffer");
      return;
    }

  if (fillorder == FILLORDER_MSB2LSB)
    for (row = 0; row < (rows); row++) 
      {
	if (TIFFReadScanline(tif, tf_buf, row, (tsample_t) 0) < 0)
	  break;
	n = 0;

	for (cp = tf_buf, cc = 0; cc < tf_bytesperrow; cc++)		
	  {
	    ccn = n*8 ;
	    c = *cp++;
	    Byte2Bit(c,bit,minisblack);

	    for (i = 1; i<=8; i++)
	      if ( (ccn+i) <= cols )
		{	
		  ccnn = ccn+i;
		  band[row+1][ccnn] = bit[9-i];
		}
	    n++;
	}
      }

  if (fillorder == FILLORDER_LSB2MSB)
    for (row = 0; row < (rows); row++) 
      {
	if (TIFFReadScanline(tif, tf_buf, row, (tsample_t) 0) < 0)
	  break;
	n = 0;

	for (cp = tf_buf, cc = 0; cc < tf_bytesperrow; cc++)		
	  {
	    ccn = n*8 ;
	    c = *cp++;
	    Byte2Bit(c,bit,minisblack);

	    for (i = 1; i<=8; i++)
	      if ( (ccn+i) <= cols )
		{	
		  ccnn = ccn+i;
		  band[row+1][ccnn] = bit[i];
		}
	    n++;
	}
      }

  free((char *) tf_buf);
}


void tiff2biff1p(IBAND band, TIFF *tif, uint32 rows, uint32 cols, uint16 fillorder, uint16 *redmap, uint16 *greenmap, uint16 *bluemap, Color_cell **coltab, uint16 bps, int *numColors)
{
  tiff2biff1(band, tif, rows, cols, fillorder, 1);
  tiffCol2biffCol(redmap, greenmap, bluemap, bps, coltab, numColors);
}


void tiff2biff2(IBAND band, TIFF *tif, uint32 rows, uint32 cols, int minisblack, double maxval)
{
  uint32 row, cc, ccn;
  unsigned char *tf_buf, *cp;
  tsize_t tf_bytesperrow;

  tf_bytesperrow = TIFFScanlineSize(tif);

  tf_buf = (unsigned char *) malloc((size_t)TIFFScanlineSize(tif));
  if (tf_buf == NULL) 
    {
      TIFFError("tiff2biff2", "No space for scanline buffer");
      return;
    }

  if (minisblack)
    for (row = 0; row < (rows); row++) 
      {
	if (TIFFReadScanline(tif, tf_buf, row, (tsample_t) 0) < 0)
	  break;

	for (cp = tf_buf, cc = 0, ccn = 1; cc < tf_bytesperrow; cc++, ccn+=4) {
	  band[row+1][ccn]   = (cp[cc] & 0x0c0);
	  band[row+1][ccn+1] = (cp[cc] & 0x030);
	  band[row+1][ccn+2] = (cp[cc] & 0x00c);
	  band[row+1][ccn+3] = (cp[cc] & 0x003);
	}
      }
  else
    for (row = 0; row < (rows); row++) 
      {
	if (TIFFReadScanline(tif, tf_buf, row, (tsample_t) 0) < 0)
	  break;

	for (cp = tf_buf, cc = 0, ccn = 1; cc < tf_bytesperrow; cc++, ccn+=4) {
	  band[row+1][ccn]   = (int) maxval - (cp[cc] & 0x0c0);
	  band[row+1][ccn+1] = (int) maxval - (cp[cc] & 0x030);
	  band[row+1][ccn+2] = (int) maxval - (cp[cc] & 0x00c);
	  band[row+1][ccn+3] = (int) maxval - (cp[cc] & 0x003);
	}
      }

  free((char *) tf_buf);
}

void tiff2biff2p(IBAND band, TIFF *tif, uint32 rows, uint32 cols, uint16 *redmap, uint16 *greenmap, uint16 *bluemap, Color_cell **coltab, uint16 bps, int *numColors)
{
  tiff2biff2(band, tif, rows, cols, 1, 0.0);
  tiffCol2biffCol(redmap, greenmap, bluemap, bps, coltab, numColors);
}


void tiff2biff4(IBAND band, TIFF *tif, uint32 rows, uint32 cols, int minisblack, double maxval)
{
  uint32 row, cc, ccn;
  unsigned char *tf_buf, *cp;
  tsize_t tf_bytesperrow;

  tf_bytesperrow = TIFFScanlineSize(tif);

  tf_buf = (unsigned char *) malloc((size_t)TIFFScanlineSize(tif));
  if (tf_buf == NULL) 
    {
      TIFFError("tiff2biff4", "No space for scanline buffer");
      return;
    }

  if (minisblack)
    for (row = 0; row < (rows); row++) 
      {
	if (TIFFReadScanline(tif, tf_buf, row, (tsample_t) 0) < 0)
	  break;

	for (cp = tf_buf, cc = 0, ccn = 1; cc < tf_bytesperrow; cc++, ccn+=2) {
	  band[row+1][ccn]   = (cp[cc] & 0x0f0);
	  band[row+1][ccn+1] = (cp[cc] & 0x00f);
	}
      }
  else
    for (row = 0; row < (rows); row++) 
      {
	if (TIFFReadScanline(tif, tf_buf, row, (tsample_t) 0) < 0)
	  break;

	for (cp = tf_buf, cc = 0, ccn = 1; cc < tf_bytesperrow; cc++, ccn+=2) {
	  band[row+1][ccn]   = (int) maxval - (cp[cc] & 0x0f0);
	  band[row+1][ccn+1] = (int) maxval - (cp[cc] & 0x00f);
	}
      }


  free((char *) tf_buf);
}

void tiff2biff4p(IBAND band, TIFF *tif, uint32 rows, uint32 cols, uint16 *redmap, uint16 *greenmap, uint16 *bluemap, Color_cell **coltab, uint16 bps, int *numColors)
{
  tiff2biff2(band, tif, rows, cols, 1, 0.0);
  tiffCol2biffCol(redmap, greenmap, bluemap, bps, coltab, numColors);
}




void tiff2biff8(IBAND band, TIFF *tif, uint32 rows, uint32 cols, int minisblack, double maxval)
{
  uint32 row, cc;
  unsigned char *tf_buf, *cp;
  tsize_t tf_bytesperrow;

  tf_bytesperrow = TIFFScanlineSize(tif);

  tf_buf = (unsigned char *) malloc((size_t) TIFFScanlineSize(tif));
  if (tf_buf == NULL) 
    {
      TIFFError("tiff2biff8", "No space for scanline buffer");
      return;
    }

  if (minisblack)
    for (row = 0; row < (rows); row++) 
      {
	if (TIFFReadScanline(tif, tf_buf, row, (tsample_t) 0) < 0)
	  break;
	for (cp = tf_buf, cc = 0; cc < tf_bytesperrow; cc++)		
	    band[row+1][cc+1] = cp[cc];
      }
  else
    for (row = 0; row < (rows); row++) 
      {
	if (TIFFReadScanline(tif, tf_buf, row, (tsample_t) 0) < 0)
	  break;
	for (cp = tf_buf, cc = 0; cc < tf_bytesperrow; cc++)		
	    band[row+1][cc+1] = (int) maxval - cp[cc];
      }	

  free((char *) tf_buf);
}

void tiff2biff8p(IBAND band, TIFF *tif, uint32 rows, uint32 cols, uint16 *redmap, uint16 *greenmap, uint16 *bluemap, Color_cell **coltab, uint16 bps, int *numColors)
{
  tiff2biff8(band, tif, rows, cols, 1, 0.0);
  tiffCol2biffCol(redmap, greenmap, bluemap, bps, coltab, numColors);
}



void tiff2biff8c(IMAGE img, TIFF *tif, uint32 rows, uint32 cols)
{
  uint32 row, cc;
  unsigned char *tf_buf, *cp;
  tsize_t tf_bytesperrow;

  tf_bytesperrow = TIFFScanlineSize(tif);

  tf_buf = (unsigned char *) malloc((size_t) TIFFScanlineSize(tif));
  if (tf_buf == NULL) 
    {
      TIFFError("tiff2biff8c", "No space for scanline buffer");
      return;
    }

  for (row = 0; row < (rows); row++) 
    {
      if (TIFFReadScanline(tif, tf_buf, row, (tsample_t) 0) < 0)
	break;
      for (cp = tf_buf, cc = 0;
	   cc < (tf_bytesperrow/(3 * sizeof(unsigned char))); cc++)		
	{
	  img[1][row+1][cc+1] = *cp++;
	  img[2][row+1][cc+1] = *cp++;
	  img[3][row+1][cc+1] = *cp++;
	}
    }

  free((char *) tf_buf);
}




void tiff2biff16(IUS_BAND band, TIFF *tif, uint32 rows, uint32 cols)
{
  uint32 row, cc;
  unsigned short *tf_buf, *cp;
  tsize_t tf_bytesperrow;

  tf_bytesperrow = TIFFScanlineSize(tif);

  tf_buf = (unsigned short *) malloc((size_t) TIFFScanlineSize(tif));
  if (tf_buf == NULL) 
    {
      TIFFError("tiff2biff16", "No space for scanline buffer");
      return;
    }

  for (row = 0; row < (rows); row++) 
    {
      if (TIFFReadScanline(tif, (unsigned char *) tf_buf, row,
			   (tsample_t) 0) < 0)
	break;
      for (cp = tf_buf, cc = 0;
	   cc < (tf_bytesperrow/(sizeof(unsigned short))); cc++)
	band[row+1][cc+1] = *cp++;
    }
  free((char *) tf_buf);
}




void tiff2biff16c(IUS_IMAGE img, TIFF *tif, uint32 rows, uint32 cols)
{
  uint32 row, cc;
  unsigned short *tf_buf, *cp;
  tsize_t tf_bytesperrow;

  tf_bytesperrow = TIFFScanlineSize(tif);

  tf_buf = (unsigned short *) malloc((size_t) TIFFScanlineSize(tif));
  if (tf_buf == NULL) 
    {
      TIFFError("tiff2biff16c", "No space for scanline buffer");
      return;
    }

  for (row = 0; row < (rows); row++) 
    {
      if (TIFFReadScanline(tif, (unsigned char *) tf_buf, row,
			   (tsample_t) 0) < 0)
	break;
      for (cp = tf_buf, cc = 0;
	   cc < (tf_bytesperrow/(3 * sizeof(unsigned short))); cc++)
	{
	  img[1][row+1][cc+1] = *cp++;
	  img[2][row+1][cc+1] = *cp++;
	  img[3][row+1][cc+1] = *cp++;
	}
    }
  free((char *) tf_buf);
}






void tiff2biff32i(II_BAND band, TIFF *tif, uint32 rows, uint32 cols)
{
  uint32 row, cc;
  int *tf_buf, *cp;
  tsize_t tf_bytesperrow;

  tf_bytesperrow = TIFFScanlineSize(tif);

  tf_buf = (int *) malloc((size_t) TIFFScanlineSize(tif));
  if (tf_buf == NULL) 
    {
      TIFFError("tiff2biff32i", "No space for scanline buffer");
      return;
    }

  for (row = 0; row < (rows); row++) 
    {
      if (TIFFReadScanline(tif, (unsigned char *) tf_buf, row,
			   (tsample_t) 0) < 0)
	break;
      for (cp = tf_buf, cc = 0;
	   cc < (tf_bytesperrow/sizeof(int)); cc++)
	  band[row+1][cc+1] = *cp++;
    }
  free((char *) tf_buf);
}




void tiff2biff32ic(II_IMAGE img, TIFF *tif, uint32 rows, uint32 cols)
{
  uint32 row, cc;
  int *tf_buf, *cp;
  tsize_t tf_bytesperrow;

  tf_bytesperrow = TIFFScanlineSize(tif);

  tf_buf = (int *) malloc((size_t) TIFFScanlineSize(tif));
  if (tf_buf == NULL) 
    {
      TIFFError("tiff2biff32ic", "No space for scanline buffer");
      return;
    }

  for (row = 0; row < (rows); row++) 
    {
      if (TIFFReadScanline(tif, (unsigned char *) tf_buf, row, 
			   (tsample_t) 0) < 0)
	break;
      for (cp = tf_buf, cc = 0;
	   cc < (tf_bytesperrow/(3 * sizeof(int))); cc++)
	{
	  img[1][row+1][cc+1] = *cp++;
	  img[2][row+1][cc+1] = *cp++;
	  img[3][row+1][cc+1] = *cp++;
	}
    }
  free((char *) tf_buf);
}


void tiff2biff32fp(IR_BAND band, TIFF *tif, uint32 rows, uint32 cols)
{
  uint32 row, cc;
  float *tf_buf, *cp;
  tsize_t tf_bytesperrow;

  tf_bytesperrow = TIFFScanlineSize(tif);

  tf_buf = (float *) malloc((size_t) TIFFScanlineSize(tif));
  if (tf_buf == NULL) 
    {
      TIFFError("tiff2biff32fp", "No space for scanline buffer");
      return;
    }

  for (row = 0; row < (rows); row++) 
    {
      if (TIFFReadScanline(tif, (unsigned char *) tf_buf, row, 
			   (tsample_t) 0) < 0)
	break;
      for (cp = tf_buf, cc = 0;
	   cc < (tf_bytesperrow/sizeof(float)); cc++)
	  band[row+1][cc+1] = *cp++;
    }
  free((char *) tf_buf);
}


void tiff2biff32fpc(IR_IMAGE img, TIFF *tif, uint32 rows, uint32 cols)
{
  uint32 row, cc;
  float *tf_buf, *cp;
  tsize_t tf_bytesperrow;

  tf_bytesperrow = TIFFScanlineSize(tif);

  tf_buf = (float *) malloc((size_t) TIFFScanlineSize(tif));
  if (tf_buf == NULL) 
    {
      TIFFError("tiff2biff32fpc", "No space for scanline buffer");
      return;
    }

  for (row = 0; row < (rows); row++) 
    {
      if (TIFFReadScanline(tif, (unsigned char *) tf_buf, row, 
			   (tsample_t) 0) < 0)
	break;
      for (cp = tf_buf, cc = 0;
	   cc < (tf_bytesperrow/(3 * sizeof(float))); cc++)
	{
	  img[1][row+1][cc+1] = *cp++;
	  img[2][row+1][cc+1] = *cp++;
	  img[3][row+1][cc+1] = *cp++;
	}
    }
  free((char *) tf_buf);
}





void tiff2biff64(ID_BAND band, TIFF *tif, uint32 rows, uint32 cols)
{
  uint32 row, cc;
  double *tf_buf, *cp;
  tsize_t tf_bytesperrow;

  tf_bytesperrow = TIFFScanlineSize(tif);

  tf_buf = (double *) malloc((size_t) TIFFScanlineSize(tif));
  if (tf_buf == NULL) 
    {
      TIFFError("tiff2biff64", "No space for scanline buffer");
      return;
    }

  for (row = 0; row < (rows); row++) 
    {
      if (TIFFReadScanline(tif, (unsigned char *) tf_buf, row, 
			   (tsample_t) 0) < 0)
	break;
      for (cp = tf_buf, cc = 0;
	   cc < (tf_bytesperrow/(sizeof(double))); cc++)
	  band[row+1][cc+1] = *cp++;
    }
  free((char *) tf_buf);
}




void tiff2biff64c(ID_IMAGE img, TIFF *tif, uint32 rows, uint32 cols)
{
  uint32 row, cc;
  double *tf_buf, *cp;
  tsize_t tf_bytesperrow;

  tf_bytesperrow = TIFFScanlineSize(tif);

  tf_buf = (double *) malloc((size_t) TIFFScanlineSize(tif));
  if (tf_buf == NULL) 
    {
      TIFFError("tiff2biff64c", "No space for scanline buffer");
      return;
    }

  for (row = 0; row < (rows); row++) 
    {
      if (TIFFReadScanline(tif, (unsigned char *) tf_buf, row,
			   (tsample_t) 0) < 0)
	break;
      for (cp = tf_buf, cc = 0;
	   cc < (tf_bytesperrow/(3 * sizeof(double))); cc++)
	{
	  img[1][row+1][cc+1] = *cp++;
	  img[2][row+1][cc+1] = *cp++;
	  img[3][row+1][cc+1] = *cp++;
	}
    }
  free((char *) tf_buf);
}

static int GetCommonFields(TIFF *tif, uint16 *bps, uint16 *spp, uint16 *photomet, uint16 *fillorder, uint16 *sampleform, uint32 *cols, uint32 *rows, char **title)
{
  if ( ! TIFFGetField( tif, TIFFTAG_BITSPERSAMPLE, bps ) )
    *bps = 1; /* Default from TIFF 6.0 spec. */
  if ( ! TIFFGetField(tif, TIFFTAG_SAMPLESPERPIXEL, spp) )
    *spp = 1; /* Default from TIFF 6.0 spec. */
  if ( ! TIFFGetField( tif, TIFFTAG_PHOTOMETRIC, photomet ) ) {
    /*
    TIFFWarning("tiff2biff", "%s\n%s\n%s\n",
		"could not find tag Photometric.",
		"This violates TIFF revision 6.0.",
		"Assuming PHOTOMETRIC_MINISBLACK.");
    *photomet = PHOTOMETRIC_MINISBLACK;
    */
    TIFFError("tiff2biff",  "Error getting photometric.\n" );
    exit(1);
  }
  if ( ! TIFFGetField( tif, TIFFTAG_FILLORDER, fillorder ) )
    *fillorder = FILLORDER_MSB2LSB; /* Default from TIFF 6.0 spec. */
  if ( ! TIFFGetField( tif, TIFFTAG_SAMPLEFORMAT, sampleform ) )
    *sampleform = SAMPLEFORMAT_UINT; /* Default from TIFF 6.0 spec. */
  if ( ! TIFFGetField( tif, TIFFTAG_IMAGEWIDTH,  cols ) ) {
    TIFFError("tiff2biff", "Could not find tag ImageWidth.\n");
    exit(1);
  }
  if ( !TIFFGetField( tif, TIFFTAG_IMAGELENGTH, rows ) ) {
    TIFFError("tiff2biff", "Could not find tag ImageHeight.\n");
    exit(1);
  }

  (void) TIFFGetField( tif, TIFFTAG_IMAGEDESCRIPTION, title);

  return(0);
}


static int CheckSampleFormat(uint16 sampleFormat, long *pixeltype, double *maxval, uint16 bps)
{
  switch (sampleFormat) {
  case SAMPLEFORMAT_UINT:
  case SAMPLEFORMAT_INT:
  case SAMPLEFORMAT_IEEEFP:
    break;

  default:
      TIFFError("tiff2biff", "Unknown sampleform: %d.\n", sampleFormat); 
      exit(1);
  }

  if      ((sampleFormat == SAMPLEFORMAT_UINT)   && (bps == 1)) {
    *pixeltype = Iu_byte_typ;
    *maxval = 1;
  }
  else if ((sampleFormat == SAMPLEFORMAT_UINT)   && (bps == 2)) {
    *pixeltype = Iu_byte_typ;
    *maxval = (1 << bps) - 1;
  }
  else if ((sampleFormat == SAMPLEFORMAT_INT)    && (bps == 2)) {
    *pixeltype = Is_byte_typ;
    *maxval = (1 << (bps - 1)) - 1;
  }
  else if ((sampleFormat == SAMPLEFORMAT_UINT)   && (bps == 4)) {
    *pixeltype = Iu_byte_typ;
    *maxval = (1 << bps) - 1;
  }
  else if ((sampleFormat == SAMPLEFORMAT_INT)    && (bps == 4)) {
    *pixeltype = Is_byte_typ;
    *maxval = (1 << (bps - 1)) - 1;
  }
  else if ((sampleFormat == SAMPLEFORMAT_UINT)   && (bps == 8)) {
    *pixeltype = Iu_byte_typ;
    *maxval = UNS_BYTE_MAX;
  }
  else if ((sampleFormat == SAMPLEFORMAT_INT)    && (bps == 8)) {
    *pixeltype = Is_byte_typ;
    *maxval = SIGN_BYTE_MAX;
  }
  else if ((sampleFormat == SAMPLEFORMAT_UINT)   && (bps == 16)) {
    *pixeltype = Iu_short_typ;
    *maxval = UNS_SHORT_MAX;
  }
  else if ((sampleFormat == SAMPLEFORMAT_INT)    && (bps == 16)) {
    *pixeltype = Is_short_typ;
    *maxval = SIGN_SHORT_MAX;
  }
  else if ((sampleFormat == SAMPLEFORMAT_INT)    && (bps == 32)) {
    *pixeltype = Integer_typ;
    *maxval = INTEGER_MAX;
  }
  else if ((sampleFormat == SAMPLEFORMAT_IEEEFP) && (bps == 32)) {
    *pixeltype = Ireal_typ;
    *maxval = REAL_MAX;
  }
  else if ((sampleFormat == SAMPLEFORMAT_IEEEFP) && (bps == 64)) {
    *pixeltype = Idouble_typ;
    *maxval = DOUBLE_MAX;
  }
  else {
    TIFFWarning("tiff2biff","%s %d %s %d %s\n%s\n",
		"Pixeltype with", sampleFormat, "SampleForm and",
		bps, "BitsPerSample not known.",
		"Unsigned byte is used.");
    *pixeltype = Iu_byte_typ;
    *maxval = UNS_BYTE_MAX;
  }

  return(0);
}

static int CheckSamplePerPixel(uint16 spp)
{
  switch ( spp ) {
  case 1:
  case 3:
    break;

  default:
    TIFFError("tiff2biff", "%s %s\n", "Can only handle one or multi-channel",
	    "grayscale, palette or 3-channel color." ); 
    exit(1);
  }

  return(0);
}


static int CheckPhotometric(TIFF *tif, uint16 photomet, int *format, int*minisblack, uint16 **redmap, uint16 **greenmap, uint16 **bluemap, uint16 spp)
{
  switch (photomet) {
  case PHOTOMETRIC_MINISBLACK:
    *format = GRAYSCALE;
    *minisblack = 1;
    break;
    
  case PHOTOMETRIC_MINISWHITE:
    *format = GRAYSCALE;
    *minisblack = 0;
    break;

  case PHOTOMETRIC_PALETTE:
    if ( ! TIFFGetField( tif, TIFFTAG_COLORMAP, redmap,
			greenmap, bluemap ) ) {
      TIFFError("tiff2biff", "Error getting colormaps.\n");
      exit(1);
    }

    if (spp != 1) {
      TIFFError("tiff2biff", "%s\n",
		"SamplesPerPixel must be 1 for palette color images.");
      exit(1);
    }

    *format = PALETTE;
    break;

  case PHOTOMETRIC_RGB:
    *format = RGB;
    break;

  default:
    TIFFError("tiff2biff", "Unknown photometric: %d\n", photomet );
    exit(1);
  }

  return(0);
}

IMAGE tiff2biff(TIFF *tif, Color_cell **coltab, int *numColors)
{
  uint16 bps, spp, photomet, fillorder, sampleform,
    *redcolormap, *greencolormap, *bluecolormap;
  uint32 cols, rows;
  int nbands, minisblack, bn, format;
  long pixeltype;
  double maxval;
  char *title;
  IMAGE img;

  *coltab = NULL;
  *numColors = 0;

  (void) GetCommonFields(tif, &bps, &spp, &photomet, &fillorder, &sampleform,
		  &cols, &rows, &title);
  if (title == NULL) title = "Converted from TIFF by tiff2biff.";
  (void) CheckSamplePerPixel(spp);
  (void) CheckSampleFormat(sampleform, &pixeltype, &maxval, bps);

  (void) CheckPhotometric(tif, photomet, &format, &minisblack,
			       &redcolormap, &greencolormap, &bluecolormap,
			       spp);
  if (format == GRAYSCALE && maxval == 1) format = BW;

  if (format == RGB) nbands = 3;
  else nbands = 1;

  img = Imake_image(nbands, title, pixeltype, (long) cols, (long) rows);

  bn = 1;
  do 
    {
      if ((bn != 1) && (nbands == 1)) {
	Iset_nbands(&img, bn);
	img[bn] = Imake_band(pixeltype, (long) cols, (long) rows); 
      }

      if ((format == BW || format == GRAYSCALE) &&
	  pixeltype != Iu_byte_typ && !minisblack)
	TIFFWarning("tiff2biff", "%s %s\n",
		"MinIsWhite is only supported for unsigned byte images.",
		"The image will be inverted.");

      if (format == BW)
	tiff2biff1(img[bn], tif, rows, cols, fillorder, minisblack);
      else if (format == GRAYSCALE) {
	if (pixeltype == Iu_byte_typ) {
	  /* BIFF result is unsigned byte */
	  if      ((int) maxval == 3)             /* 2-bit TIFF samples */
	    tiff2biff2(img[bn], tif, rows, cols, minisblack, maxval);
	  else if ((int) maxval == 15)            /* 4-bit TIFF samples */
	    tiff2biff4(img[bn], tif, rows, cols, minisblack, maxval);
	  else if ((int) maxval == UNS_BYTE_MAX)  /* 8-bit TIFF samples */
	    tiff2biff8(img[bn], tif, rows, cols, minisblack, maxval); 
	} else if (pixeltype == Is_byte_typ) {
	  /* BIFF result is signed byte */
	  if      ((int) maxval == 1)             /* 2-bit TIFF samples */
	    TIFFError("tiff2biff", "%s\n",
		    "Conversion from signed 2-bit samples not implemented.");
	  else if ((int) maxval == 7)             /* 4-bit TIFF samples */
	    TIFFError("tiff2biff", "%s\n",
		    "Conversion from signed 4-bit samples not implemented.");
	  else if ((int) maxval == SIGN_BYTE_MAX) /* 8-bit TIFF samples */
	    TIFFError("tiff2biff", "%s\n",
		    "Conversion from signed 8-bit samples not implemented.");
	} else if (pixeltype == Iu_short_typ)
	  /* BIFF result is unsigned short */
	  tiff2biff16((IUS_BAND) img[bn], tif, rows, cols);
	else if (pixeltype == Is_short_typ)
	  /* BIFF result is signed short */
	    TIFFError("tiff2biff", "%s %s\n",
		    "Conversion from signed 16-bit samples",
		    "not implemented.");
	else if (pixeltype == Integer_typ)
	  /* BIFF result is integer */
	  tiff2biff32i((II_BAND) img[bn], tif, rows, cols); 
	else if (pixeltype == Ireal_typ)
	  /* BIFF result is real */
	  tiff2biff32fp((IR_BAND) img[bn], tif, rows, cols); 
	else if (pixeltype == Idouble_typ)
	  /* BIFF result is double */
	  tiff2biff64((ID_BAND) img[bn], tif, rows, cols);
	else {
	  TIFFError("tiff2biff","%s %ld %s\n",
		    "Error: Unsupported pixeltype", pixeltype,
		    "for grayscale image.");
	  return(NULL);
	}
      }
      else if (format == RGB) {
	if (pixeltype == Iu_byte_typ) 
	  tiff2biff8c(img, tif, rows, cols);
	else if (pixeltype == Iu_short_typ) 
	  tiff2biff16c((IUS_IMAGE) img, tif, rows, cols);
	else if (pixeltype == Integer_typ) 
	  tiff2biff32ic((II_IMAGE) img, tif, rows, cols);
	else if (pixeltype == Ireal_typ) 
	  tiff2biff32fpc((IR_IMAGE) img, tif, rows, cols);
	else if (pixeltype == Idouble_typ) 
	  tiff2biff64c((ID_IMAGE) img, tif, rows, cols);
	else {
	  TIFFError("tiff2biff", "%s %ld %s\n",
		    "Error: Unsupported pixeltype", pixeltype,
		    "for rgb image.");
	  return(NULL);
	}
      }
      else if (format == PALETTE) {
	if (pixeltype == Iu_byte_typ) {
	  /* BIFF result is unsigned byte */
	  if      ((int) maxval == 1)             /* 1-bit TIFF samples */
	    tiff2biff1p(img[bn], tif, rows, cols, fillorder, redcolormap,
			greencolormap, bluecolormap, coltab, bps,
			numColors);
	  else if ((int) maxval == 3)             /* 2-bit TIFF samples */
	    tiff2biff2p(img[bn], tif, rows, cols, redcolormap,
			greencolormap, bluecolormap, coltab, bps,
			numColors);
	  else if ((int) maxval == 15)            /* 4-bit TIFF samples */
	    tiff2biff4p(img[bn], tif, rows, cols, redcolormap,
			greencolormap, bluecolormap, coltab, bps,
			numColors);
	  else if ((int) maxval == UNS_BYTE_MAX)  /* 8-bit TIFF samples */
	    tiff2biff8p(img[bn], tif, rows, cols, redcolormap,
			greencolormap, bluecolormap, coltab, bps,
			numColors);
	} else {
	  TIFFError("tiff2biff", "%s %ld %s\n",
		    "Error: Unsupported pixeltype", pixeltype,
		    "for Palette image.");
	  return(NULL);
	}
      }

      bn++;

    } while (TIFFReadDirectory(tif));

    return(img);
}
