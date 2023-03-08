
/*C*
________________________________________________________________

        biff2tiff
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
#include XITE_MALLOC_H
#include XITE_STRING_H
#include <xite/blab.h>
#include <xite/biff.h>
#include <xite/color.h>
#include <xite/message.h>
#include <xite/readarg.h>
#include <xite/tiff.h>
#include XITE_STDIO_H

#define NIL 0
#ifndef MAX
# define MAX(a,b) ((a)>(b) ? a : b)
#endif
#ifndef MIN
# define MIN(a,b) ((a)<(b) ? a : b)
#endif

#define COLORS8BIT 256

/*F:biff2tiff1=biff2tiff*/
/*F:biff2tiff8=biff2tiff*/
/*F:biff2tiff8p=biff2tiff*/
/*F:biff2tiff8c=biff2tiff*/
/*F:biff2tiff8s=biff2tiff*/
/*F:biff2tiff8sc=biff2tiff*/
/*F:biff2tiff16=biff2tiff*/
/*F:biff2tiff16c=biff2tiff*/
/*F:biff2tiff16s=biff2tiff*/
/*F:biff2tiff16sc=biff2tiff*/
/*F:biff2tiff32i=biff2tiff*/
/*F:biff2tiff32ic=biff2tiff*/
/*F:biff2tiff32fp=biff2tiff*/
/*F:biff2tiff32fpc=biff2tiff*/
/*F:biff2tiff64=biff2tiff*/
/*F:biff2tiff64c=biff2tiff*/
/*F:biff2tiff*

________________________________________________________________

		biff2tiff
________________________________________________________________

Name:	        biff2tiff, biff2tiff1, biff2tiff8, biff2tiff8p, biff2tiff8c,
                biff2tiff8s, biff2tiff8sc, biff2tiff16, biff2tiff16c,
		biff2tiff16s, biff2tiff16sc, biff2tiff32i, biff2tiff32ic,
		biff2tiff32fp, biff2tiff32fpc, biff2tiff64, biff2tiff64c -
		Convert a BIFF image to TIFF

Syntax:         | #include <xite/tiff.h>
                |
                | int biff2tiff( IMAGE img, TIFF* tif,
                |    int format, Color_cell* coltab, int colLen,
                |    uint16 compression );
		|
                | int biff2tiff1( IBAND band, TIFF* tif,
                |    uint16 compression, UNS_BYTE threshold );
		|
                | int biff2tiff8( IBAND band, TIFF* tif,
                |    uint16 compression );
		|
                | int biff2tiff8p( IBAND band, TIFF* tif,
                |    Color_cell* coltab, int colLen,
                |    uint16 compression );
		|
                | int biff2tiff8c( IMAGE img, TIFF* tif,
                |    uint16 compression );
		|
                | int biff2tiff8s( ISB_BAND band, TIFF* tif,
                |    uint16 compression );
		|
                | int biff2tiff8sc( ISB_IMAGE img, TIFF* tif,
                |    uint16 compression );
		|
                | int biff2tiff16( IUS_BAND band, TIFF* tif,
                |    uint16 compression );
		|
                | int biff2tiff16c( IUS_IMAGE img, TIFF* tif,
                |    uint16 compression );
		|
                | int biff2tiff16s( ISS_BAND band, TIFF* tif,
                |    uint16 compression );
		|
                | int biff2tiff16sc( ISS_IMAGE img, TIFF* tif,
                |    uint16 compression );
		|
                | int biff2tiff32i( II_BAND band, TIFF* tif,
                |    uint16 compression );
		|
                | int biff2tiff32ic( II_IMAGE img, TIFF* tif,
                |    uint16 compression );
		|
                | int biff2tiff32fp( IR_BAND band, TIFF* tif,
                |    uint16 compression );
		|
                | int biff2tiff32fpc( IR_IMAGE img, TIFF* tif,
                |    uint16 compression );
		|
                | int biff2tiff64( ID_BAND band, TIFF* tif,
                |    uint16 compression );
		|
                | int biff2tiff64c( ID_IMAGE img, TIFF* tif,
                |    uint16 compression );

Description:    'biff2tiff' is a wrapper function that decides which one of the
		routines below to use, depending on BIFF image pixeltype and
		the value of the parameter 'format' (RGB, PALETTE, GRAYSCALE
		or BW). 'coltab' is the supplied XITE colortable with
		'colLen' entries. 'compression' is one of the legal values for
		the libtiff tag TIFFTAG_COMPRESSION:
		|  COMPRESSION_NONE
		|  COMPRESSION_PACKBITS
		|  COMPRESSION_LZW
		|  COMPRESSION_CCITTRLE
		|  COMPRESSION_CCITTFAX3
		|  COMPRESSION_CCITTFAX4

 		Byte-ordering between little-endian and big-endian machines
		is handled automatically.

		The conversion is according to TIFF specification 6.0
		and uses the libtiff library by Sam Leffler. This library
		must exist on your system, it is not supplied with XITE.
		If you get problems with importing XITE-produced TIFF images
		into other applications, make sure those applications are
		also TIFF 6.0 compliant.

		Grayscale conversion routines:

		'biff2tiff1' converts an unsigned byte BIFF band to a TIFF
		bilevel (1-bit samples) image by thresholding at 'threshold'.

                'biff2tiff8' converts an unsigned byte BIFF band to a TIFF
                grayscale (8-bit unsigned samples) image.

		'biff2tiff8s' converts a signed byte BIFF band to a TIFF
                grayscale (8-bit signed samples) image.

		'biff2tiff16' converts an unsigned short BIFF band to a TIFF
                grayscale (16-bit samples) unsigned short image.

		'biff2tiff16s' converts a signed short BIFF band to a TIFF
                grayscale (16-bit samples) signed short image.

		'biff2tiff32i' converts a signed integer BIFF band to a TIFF
                grayscale (32-bit samples) signed integer image.

		'biff2tiff32fp' converts a 32-bit floating point BIFF band
		to a TIFF grayscale (32-bit samples) floating point image.

		'biff2tiff64' converts a 64-bit floating point BIFF band to a
		TIFF grayscale (64-bit samples) floating point image.

		Palette (pseudocolor) conversion routines:

		'biff2tiff8p' converts an unsigned byte BIFF band with the
		colortable 'coltab' to a TIFF palette image.

		RGB conversion routines:

		'biff2tiff8c' converts a three-band unsigned byte BIFF image
		to a TIFF 3*8-bit unsigned byte rgb image.

		'biff2tiff8sc' converts a three-band signed byte BIFF image
		to a TIFF 3*8-bit signed byte rgb image.

		'biff2tiff16c' converts a three-band unsigned short BIFF image
		to a TIFF 3*16-bit unsigned short rgb image.

		'biff2tiff16sc' converts a three-band signed short BIFF image
		to a TIFF 3*16-bit signed short rgb image.

		'biff2tiff32ic' converts a three-band signed integer BIFF image
		to a TIFF 3*32-bit integer rgb image.

		'biff2tiff32fpc' converts a three-band 32-bit floating point
		BIFF image to a TIFF 3*32-bit floating point rgb image.

		'biff2tiff64c' converts a three-band 64-bit floating point
		BIFF image to a TIFF 3*64-bit floating point rgb image.

Return value:   | 0 => ok
                | other => Bad pixel type

See also:       'biff2tiff(1)', 'tiff2biff(1)', 'tiff2biff(3)'

Author:		Helene Schulerud and Svein Bøe, BLAB, Ifi, UiO.

Id:             $Id$
________________________________________________________________

*/

/* Macro for definition of b/w conversion routines */

# define DECL_BIFF2TIFF_BW(FUNC_NAME, FUNC_STRING, BAND_TYPE, DATA_TYPE, TIFF_PIX_TYPE) \
  int FUNC_NAME(BAND_TYPE band, TIFF *tif, uint16 compression, DATA_TYPE threshold) \
  { \
    uint32 xsize, ysize, x, y, i; \
    DATA_TYPE *buf, *bptr; \
    tsize_t length; \
    int bit; \
 \
    xsize = (uint32) Ixsize((IBAND) band); \
    ysize = (uint32) Iysize((IBAND) band); \
 \
    if (!TIFFSetField(tif, TIFFTAG_IMAGEWIDTH, xsize)) \
      TIFFError(FUNC_STRING, "Could not set field ImageWidth.\n"); \
    if (!TIFFSetField(tif, TIFFTAG_IMAGELENGTH, ysize)) \
      TIFFError(FUNC_STRING, "Could not set field ImageLength.\n"); \
    if (!TIFFSetField(tif, TIFFTAG_BITSPERSAMPLE, 1)) \
      TIFFError(FUNC_STRING, "Could not set field BitsPerSample.\n"); \
    if (!TIFFSetField(tif, TIFFTAG_PHOTOMETRIC, PHOTOMETRIC_MINISBLACK)) \
      TIFFError(FUNC_STRING, "Could not set field Photometric.\n"); \
    if (!TIFFSetField(tif, TIFFTAG_ORIENTATION, ORIENTATION_TOPLEFT)) \
      TIFFError(FUNC_STRING, "Could not set field Orientation.\n"); \
    if (!TIFFSetField(tif, TIFFTAG_PLANARCONFIG, PLANARCONFIG_CONTIG)) \
      TIFFError(FUNC_STRING, "Could not set field Planarconfig.\n"); \
    if (!TIFFSetField(tif, TIFFTAG_SAMPLESPERPIXEL, 1)) \
      TIFFError(FUNC_STRING, "Could not set field SamplesPerPixel.\n"); \
    if (!TIFFSetField(tif, TIFFTAG_SAMPLEFORMAT, TIFF_PIX_TYPE)) \
      TIFFError(FUNC_STRING, "Could not set field Sampleformat.\n"); \
    if (!TIFFSetField(tif, TIFFTAG_COMPRESSION, compression)) \
      TIFFError(FUNC_STRING, "Could not set field Compression.\n"); \
    if (!TIFFSetField(tif, TIFFTAG_FILLORDER, FILLORDER_MSB2LSB)) \
      TIFFError(FUNC_STRING, "Could not set field Fillorder.\n"); \
 \
    length = TIFFScanlineSize(tif); \
    if (length == -1) return(Error(2, "%s%s\n", FUNC_STRING, \
                                   ": Error in determining scanline size.")); \
 \
    buf = (DATA_TYPE *) malloc((size_t) length); \
    if (!buf) return(Error(2, "%s%s\n", FUNC_STRING, \
			   ": Could not allocate memory.")); \
 \
    for (y = 1; y <= ysize;  y++) { \
      bptr = buf; \
      for (x = 1; x <= xsize;  x += 8) { \
        *bptr = 0; \
        bit   = 0x080; \
 \
        for (i = 1; i <= 8; i++) { \
          if (x + i - 1 > xsize) break; \
          if (band[y][x + i - 1] >= threshold) *bptr |= bit; \
          bit >>= 1; \
	} \
 \
        bptr++; \
      } \
 \
      if (TIFFWriteScanline(tif, (unsigned char *) buf, y-1, 0) < 0) \
        break; \
    } \
 \
    TIFFWriteDirectory(tif); \
 \
    free(buf); \
    return(0); \
  }

/* Macro for definition of grayscale conversion routines */

# define DECL_BIFF2TIFF_GRAY(FUNC_NAME, FUNC_STRING, BAND_TYPE, DATA_TYPE, TIFF_PIX_TYPE, BITS_PER_SAMPLE, SAMPLES_PER_PIXEL) \
  int FUNC_NAME(BAND_TYPE band, TIFF *tif, uint16 compression) \
  { \
    uint32 xsize, ysize, x, y; \
    DATA_TYPE *buf; \
    tsize_t length; \
 \
    xsize = (uint32) Ixsize((IBAND) band); \
    ysize = (uint32) Iysize((IBAND) band); \
 \
 \
    if (!TIFFSetField(tif, TIFFTAG_IMAGEWIDTH, xsize)) \
      TIFFError(FUNC_STRING, "Could not set field ImageWidth.\n"); \
    if (!TIFFSetField(tif, TIFFTAG_IMAGELENGTH, ysize)) \
      TIFFError(FUNC_STRING, "Could not set field ImageLength.\n"); \
    if (!TIFFSetField(tif, TIFFTAG_BITSPERSAMPLE, BITS_PER_SAMPLE)) \
      TIFFError(FUNC_STRING, "Could not set field BitsPerSample.\n"); \
    if (!TIFFSetField(tif, TIFFTAG_PHOTOMETRIC, PHOTOMETRIC_MINISBLACK)) \
      TIFFError(FUNC_STRING, "Could not set field Photometric.\n"); \
    if (!TIFFSetField(tif, TIFFTAG_ORIENTATION, ORIENTATION_TOPLEFT)) \
      TIFFError(FUNC_STRING, "Could not set field Orientation.\n"); \
    if (!TIFFSetField(tif, TIFFTAG_PLANARCONFIG, PLANARCONFIG_CONTIG)) \
      TIFFError(FUNC_STRING, "Could not set field Planarconfig.\n"); \
    if (!TIFFSetField(tif, TIFFTAG_SAMPLESPERPIXEL, SAMPLES_PER_PIXEL)) \
      TIFFError(FUNC_STRING, "Could not set field SamplesPerPixel.\n"); \
    if (!TIFFSetField(tif, TIFFTAG_SAMPLEFORMAT, TIFF_PIX_TYPE)) \
      TIFFError(FUNC_STRING, "Could not set field Sampleformat.\n"); \
    if (!TIFFSetField(tif, TIFFTAG_COMPRESSION, compression)) \
      TIFFError(FUNC_STRING, "Could not set field Compression.\n"); \
 \
    length = TIFFScanlineSize(tif); \
    if (length == -1) return(Error(2, "%s%s\n", FUNC_STRING, \
                                   ": Error in determining scanline size.")); \
 \
    buf = (DATA_TYPE *)malloc((size_t) length); \
    if (!buf) return(Error(2, "%s%s\n", FUNC_STRING, \
			   ": Could not allocate memory.")); \
 \
    for (y=1; y <= ysize;  y++) \
      { \
	for (x=1; x <= xsize;  x++) \
	  { \
	    buf[x-1] = band[y][x]; \
	  } \
 \
	if (TIFFWriteScanline(tif, (unsigned char *) buf, y-1, 0) < 0) \
	  break; \
      } \
 \
    TIFFWriteDirectory(tif); \
 \
    free(buf); \
    return(0); \
  }

/* Macro for definition of palette conversion routines */

# define DECL_BIFF2TIFF_PAL(FUNC_NAME, FUNC_STRING, BAND_TYPE, DATA_TYPE, TIFF_PIX_TYPE, BITS_PER_SAMPLE, SAMPLES_PER_PIXEL) \
  int FUNC_NAME(BAND_TYPE band, TIFF *tif, Color_cell *coltab, int colLen, uint16 compression) \
  { \
    uint32 xsize, ysize, x, y, i; \
    DATA_TYPE *buf; \
    uint16 red[COLORS8BIT], green[COLORS8BIT], blue[COLORS8BIT]; \
    unsigned long pix; \
    tsize_t length; \
 \
    xsize = (uint32) Ixsize((IBAND) band); \
    ysize = (uint32) Iysize((IBAND) band); \
 \
 \
    if (!TIFFSetField(tif, TIFFTAG_IMAGEWIDTH, xsize)) \
      TIFFError(FUNC_STRING, "Could not set field ImageWidth.\n"); \
    if (!TIFFSetField(tif, TIFFTAG_IMAGELENGTH, ysize)) \
      TIFFError(FUNC_STRING, "Could not set field ImageLength.\n"); \
    if (!TIFFSetField(tif, TIFFTAG_BITSPERSAMPLE, BITS_PER_SAMPLE)) \
      TIFFError(FUNC_STRING, "Could not set field BitsPerSample.\n"); \
    if (!TIFFSetField(tif, TIFFTAG_PHOTOMETRIC, PHOTOMETRIC_PALETTE)) \
      TIFFError(FUNC_STRING, "Could not set field Photometric.\n"); \
    if (!TIFFSetField(tif, TIFFTAG_ORIENTATION, ORIENTATION_TOPLEFT)) \
      TIFFError(FUNC_STRING, "Could not set field Orientation.\n"); \
    if (!TIFFSetField(tif, TIFFTAG_PLANARCONFIG, PLANARCONFIG_CONTIG)) \
      TIFFError(FUNC_STRING, "Could not set field Planarconfig.\n"); \
    if (!TIFFSetField(tif, TIFFTAG_SAMPLESPERPIXEL, SAMPLES_PER_PIXEL)) \
      TIFFError(FUNC_STRING, "Could not set field SamplesPerPixel.\n"); \
    if (!TIFFSetField(tif, TIFFTAG_SAMPLEFORMAT, TIFF_PIX_TYPE)) \
      TIFFError(FUNC_STRING, "Could not set field Sampleformat.\n"); \
    if (!TIFFSetField(tif, TIFFTAG_COMPRESSION, compression)) \
      TIFFError(FUNC_STRING, "Could not set field Compression.\n"); \
 \
    for (i = 0; i < COLORS8BIT; i++) { \
      red[i]   = 0; \
      green[i] = 0; \
      blue[i]  = 0; \
    } \
    for (i = 0; i < colLen; i++) { \
      pix        = PseudoBasePart(coltab[i].pixel); \
      red[pix]   = coltab[i].red; \
      green[pix] = coltab[i].green; \
      blue[pix]  = coltab[i].blue; \
    } \
 \
    if (!TIFFSetField(tif, TIFFTAG_COLORMAP, red, green, blue)) \
      TIFFError(FUNC_STRING, "Could not set field Colormap.\n"); \
 \
    length = TIFFScanlineSize(tif); \
    if (length == -1) return(Error(2, "%s%s\n", FUNC_STRING, \
                                   ": Error in determining scanline size.")); \
 \
    buf = (DATA_TYPE *)malloc((size_t) length); \
    if (!buf) return(Error(2, "%s%s\n", FUNC_STRING, \
			   ": Could not allocate memory.")); \
 \
    for (y=1; y <= ysize;  y++) \
      { \
	for (x=1; x <= xsize;  x++) \
	  { \
	    buf[x-1] = band[y][x]; \
	  } \
 \
	if (TIFFWriteScanline(tif, (unsigned char *) buf, y-1, 0) < 0) \
	  break; \
      } \
 \
    TIFFWriteDirectory(tif); \
 \
    free(buf); \
    return(0); \
  }

/* Macro for definition of rgb conversion routines */
 
# define DECL_BIFF2TIFF_RGB(FUNC_NAME, FUNC_STRING, IMAGE_TYPE, DATA_TYPE, TIFF_PIX_TYPE, BITS_PER_SAMPLE, SAMPLES_PER_PIXEL) \
  int FUNC_NAME(IMAGE_TYPE img, TIFF *tif, uint16 compression) \
  { \
    uint32 xsize, ysize, x, y, x1; \
    DATA_TYPE *buf; \
    tsize_t length; \
 \
    if (Inbands((IMAGE) img) != 3) \
      return(Error(3, "%s%s\n", FUNC_STRING, \
		   ": Image does not have exactly three bands.\n")); \
 \
    xsize = (uint32) Ixsize((IBAND) img[1]); \
    ysize = (uint32) Iysize((IBAND) img[1]); \
 \
 \
    if (!TIFFSetField(tif, TIFFTAG_IMAGEWIDTH, xsize)) \
      TIFFError(FUNC_STRING, "Could not set field ImageWidth.\n"); \
    if (!TIFFSetField(tif, TIFFTAG_IMAGELENGTH, ysize)) \
      TIFFError(FUNC_STRING, "Could not set field ImageLength.\n"); \
    if (!TIFFSetField(tif, TIFFTAG_BITSPERSAMPLE, BITS_PER_SAMPLE)) \
      TIFFError(FUNC_STRING, "Could not set field BitsPerSample.\n"); \
    if (!TIFFSetField(tif, TIFFTAG_PHOTOMETRIC, PHOTOMETRIC_RGB)) \
      TIFFError(FUNC_STRING, "Could not set field Photometric.\n"); \
    if (!TIFFSetField(tif, TIFFTAG_ORIENTATION, ORIENTATION_TOPLEFT)) \
      TIFFError(FUNC_STRING, "Could not set field Orientation.\n"); \
    if (!TIFFSetField(tif, TIFFTAG_PLANARCONFIG, PLANARCONFIG_CONTIG)) \
      TIFFError(FUNC_STRING, "Could not set field Planarconfig.\n"); \
    if (!TIFFSetField(tif, TIFFTAG_SAMPLESPERPIXEL, SAMPLES_PER_PIXEL)) \
      TIFFError(FUNC_STRING, "Could not set field SamplesPerPixel.\n"); \
    if (!TIFFSetField(tif, TIFFTAG_SAMPLEFORMAT, TIFF_PIX_TYPE)) \
      TIFFError(FUNC_STRING, "Could not set field Sampleformat.\n"); \
    if (!TIFFSetField(tif, TIFFTAG_COMPRESSION, compression)) \
      TIFFError(FUNC_STRING, "Could not set field Compression.\n"); \
 \
    length = TIFFScanlineSize(tif); \
    if (length == -1) return(Error(2, "%s%s\n", FUNC_STRING, \
                                   ": Error in determining scanline size.")); \
 \
    buf = (DATA_TYPE *)malloc((size_t) length); \
    if (!buf) return(Error(2, "%s%s\n", FUNC_STRING, \
			   ": Could not allocate memory.")); \
 \
    for (y=1; y <= ysize;  y++) \
      { \
	x1=0; \
	for (x=1; x <= xsize;  x++) \
	  { \
	    buf[x1++] = img[1][y][x]; \
	    buf[x1++] = img[2][y][x]; \
	    buf[x1++] = img[3][y][x]; \
	  } \
 \
	if (TIFFWriteScanline(tif, (unsigned char *) buf, y-1, 0) < 0) \
	  break; \
      } \
 \
    TIFFWriteDirectory(tif); \
 \
    free(buf); \
    return(0); \
  }

/* Definition of b/w conversion routines */

DECL_BIFF2TIFF_BW(biff2tiff1, "biff2tiff1", IBAND, UNS_BYTE, SAMPLEFORMAT_UINT)

/* Definition of grayscale conversion routines */

DECL_BIFF2TIFF_GRAY(biff2tiff8, "biff2tiff8", IBAND, UNS_BYTE, SAMPLEFORMAT_UINT, 8, 1)
DECL_BIFF2TIFF_GRAY(biff2tiff8s, "biff2tiff8s", ISB_BAND, SIGN_BYTE, SAMPLEFORMAT_INT, 8, 1)
DECL_BIFF2TIFF_GRAY(biff2tiff16, "biff2tiff16", IUS_BAND, UNS_SHORT, SAMPLEFORMAT_UINT, 16, 1)
DECL_BIFF2TIFF_GRAY(biff2tiff16s, "biff2tiff16s", ISS_BAND, SIGN_SHORT, SAMPLEFORMAT_INT, 16, 1)
DECL_BIFF2TIFF_GRAY(biff2tiff32i, "biff2tiff32i", II_BAND, INTEGER, SAMPLEFORMAT_INT, 32, 1)
DECL_BIFF2TIFF_GRAY(biff2tiff32fp, "biff2tiff32fp", IR_BAND, REAL, SAMPLEFORMAT_IEEEFP, 32, 1)
DECL_BIFF2TIFF_GRAY(biff2tiff64, "biff2tiff64", ID_BAND, DOUBLE, SAMPLEFORMAT_IEEEFP, 64, 1)

/* Definition of palette conversion routines */

DECL_BIFF2TIFF_PAL(biff2tiff8p, "biff2tiff8p", IBAND, UNS_BYTE, SAMPLEFORMAT_UINT, 8, 1)

/* Definition of rgb conversion routines */

DECL_BIFF2TIFF_RGB(biff2tiff8c, "biff2tiff8c", IMAGE, UNS_BYTE, SAMPLEFORMAT_UINT, 8, 3)
DECL_BIFF2TIFF_RGB(biff2tiff8sc, "biff2tiff8sc", ISB_IMAGE, SIGN_BYTE, SAMPLEFORMAT_INT, 8, 3)
DECL_BIFF2TIFF_RGB(biff2tiff16c, "biff2tiff16c", IUS_IMAGE, UNS_SHORT, SAMPLEFORMAT_UINT, 16, 3)
DECL_BIFF2TIFF_RGB(biff2tiff16sc, "biff2tiff16sc", ISS_IMAGE, SIGN_SHORT, SAMPLEFORMAT_INT, 16, 3)
DECL_BIFF2TIFF_RGB(biff2tiff32ic, "biff2tiff32ic", II_IMAGE, int, SAMPLEFORMAT_INT, 32, 3)
DECL_BIFF2TIFF_RGB(biff2tiff32fpc, "biff2tiff32fpc", IR_IMAGE, float, SAMPLEFORMAT_IEEEFP, 32, 3)
DECL_BIFF2TIFF_RGB(biff2tiff64c, "biff2tiff64c", ID_IMAGE, double, SAMPLEFORMAT_IEEEFP, 64, 3)




int biff2tiff(IMAGE img, TIFF *tif, int format, Color_cell *coltab, int colLen, uint16 compression)
{
  long pixeltype, bn;
   
   if (format == RGB) {
     pixeltype = Ipixtyp(img[1]);

     if      (pixeltype == Iu_byte_typ)
       return(biff2tiff8c(img, tif, compression));
     else if (pixeltype == Is_byte_typ)
       return(biff2tiff8sc((ISB_IMAGE) img, tif, compression));
     else if (pixeltype == Iu_short_typ)
       return(biff2tiff16c((IUS_IMAGE) img, tif, compression));
     else if (pixeltype == Is_short_typ)
       return(biff2tiff16sc((ISS_IMAGE) img, tif, compression));
     else if (pixeltype == Integer_typ)
       return(biff2tiff32ic((II_IMAGE) img,tif, compression));
     else if (pixeltype == Ireal_typ)
       return(biff2tiff32fpc((IR_IMAGE) img,tif, compression));
     else if (pixeltype == Idouble_typ)
       return(biff2tiff64c((ID_IMAGE) img,tif, compression));
     else return(Error(2, "%s %ld %s\n",
		       "biff2tiff: Cannot convert pixel type ",
		       pixeltype, "to TIFF rgb image."));
   }
   else if (format == BW) {
     for (bn=1; bn <= Inbands(img); bn++) {
       pixeltype = Ipixtyp(img[bn]);

       switch(pixeltype) {
       case Iu_byte_typ: biff2tiff1(img[bn], tif, compression, 128); break;
       default: return(Error(2, "%s %ld %s\n",
			     "biff2tiff: Cannot convert pixel type ",
			     pixeltype, "to TIFF b/w image."));
       }
     }
   }
   else if (format == GRAYSCALE) {
     for (bn=1; bn <= Inbands(img); bn++) { 
       pixeltype = Ipixtyp(img[bn]);
       
       if      (pixeltype == Iu_byte_typ)
	 biff2tiff8(img[bn], tif, compression);
       else if (pixeltype == Is_byte_typ)
	 biff2tiff8s((ISB_BAND) img[bn], tif, compression);
       else if (pixeltype == Iu_short_typ)
	 biff2tiff16((IUS_BAND) img[bn],tif, compression);
       else if (pixeltype == Is_short_typ)
	 biff2tiff16s((ISS_BAND) img[bn],tif, compression);
       else if (pixeltype == Integer_typ)
	 biff2tiff32i((II_BAND) img[bn],tif, compression);
       else if (pixeltype == Ireal_typ)
	 biff2tiff32fp((IR_BAND) img[bn],tif, compression);
       else if (pixeltype == Idouble_typ)
	 biff2tiff64((ID_BAND) img[bn],tif, compression);
       else return(Error(2, "%s %ld %s\n",
			 "biff2tiff: Cannot convert pixel type ",
			 pixeltype, "to TIFF grayscale image."));
     }
   }
   else if (format == PALETTE) {
     for (bn=1; bn <= Inbands(img); bn++) { 
       pixeltype = Ipixtyp(img[bn]);
       
       if (pixeltype == Iu_byte_typ) {
	 if (colLen > COLORS8BIT)
	   Error(2, "%s %s\n",
		 "biff2tiff: Colortab in file is too long for 8 bit"
		 "palette image.\n");
	 else return(biff2tiff8p(img[1], tif, coltab, colLen, compression));
       }
       else  return(Error(2, "%s %ld %s\n",
			  "biff2tiff: Cannot convert pixel type ",
			  pixeltype, "to TIFF palette image."));
     }
   }
  return(0);
}
