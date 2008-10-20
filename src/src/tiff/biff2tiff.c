
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
#ifdef HAVE_MALLOC_H
# include <malloc.h>
#endif
#ifdef HAVE_STDIO_H
#  include <stdio.h>
#endif
#ifdef HAVE_STRINGS_H
# include <strings.h>
#else
# ifdef HAVE_STRING_H
#  include <string.h>
# endif
#endif
#include <xite/blab.h>
#include <xite/biff.h>
#include <xite/color.h>
#include <xite/message.h>
#include <xite/readarg.h>
#include <xite/tiff.h>

#ifndef MAIN

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

#endif /* not MAIN */



#ifdef MAIN

/*P:biff2tiff*

________________________________________________________________

		biff2tiff
________________________________________________________________

Name:		biff2tiff - Convert an image from BIFF to TIFF format

Syntax:		biff2tiff [-rgb | -p | -bw] [-c <comp>] <BIFF-image>
                   [<colortable filename>] <TIFF-image>   

Description:    Convert all the bands of 'BIFF-image' to a TIFF image.
                Default behaviour is to convert to a grayscale image.

		Byte-ordering between little-endian and big-endian machines
		is handled automatically.

		The conversion is according to TIFF specification 6.0
		and uses the libtiff library by Sam Leffler. This library
		must exist on your system, it is not supplied with XITE.
		If you get problems with importing XITE-produced TIFF images
		into other applications, make sure those applications are
		also TIFF 6.0 compliant.

Options:        &-bw
                Convert the grayscale 'BIFF-image' to a bilevel TIFF image,
		by thresholding	at the value 128.

                &-rgb
                'BIFF-image' must contain three bands (rgb), which are
		converted to a TIFF rgb color image.

		&-p
		A colortable filename must be supplied as a command line
		argument. In this case, the first band of the image is
                converted to a TIFF palette (pseudocolor) image. 

		&-c comp
		Specify compression, where 'comp' may be one of

		  &&none
		  No compression

		  &&lzw
		  Lempel-Ziv & Welch, for grayscale, palette and rgb images.

		  &&packbits
		  Packbits compression, for bilevel images.

		  &&huff
		  CCITT 1D Huffman compression, for bilevel images.

		  &&fax3
		  CCITT Group 3 Facsimile compression, for bilevel images.

		  &&fax4
		  CCITT Group 4 Facsimile compression, for bilevel images.

Restrictions:   'biff2tiff' supports the following conversions
                |
                | For grayscale images:
		|
                |   From BIFF                       To TIFF      
		| unsigned byte               1-bit
		| unsigned byte               8-bit unsigned byte
		| signed byte                 8-bit signed byte
		| unsigned short              16-bit unsigned short
		| signed short                16-bit signed short 
		| signed integer              32-bit signed integer
		| floating point              32-bit floating point
		| 64-bit floating             64-bit floating point
		|
		| For palette (pseudocolor) images:
		|
                |   From BIFF                        To TIFF      
		| unsigned byte               8-bit unsigned byte
		|
		| For rgb color images:
		|
                |   From BIFF                        To TIFF              
		| three-band unsigned byte    3*8-bit unsigned byte (rgb)
		| three-band signed byte      3*8-bit signed byte (rgb)
		| three-band unsigned short   3*16-bit unsigned short (rgb)
		| three-band signed short     3*16-bit signed short (rgb)
		| three-band signed integer   3*32-bit signed integer (rgb)
		| three-band floating point   3*32-bit floating point (rgb)
		| three-band 64-bit fl. pnt.  3*64-bit floating point (rgb)

See also:       'biff2tiff(3)', 'tiff2biff(1)', 'tiff2biff(3)'

Author:		Helene Schulerud and Svein Bøe, BLAB, Ifi, UiO

Examples:       biff2tiff mona.img mona.tif 

Id:             $Id$
________________________________________________________________

*/

int main(int argc, char *argv[])
{
  IMAGE img, colImg;
  int rgb, palette, bw, format, colLen;
  uint16 comp = COMPRESSION_NONE;
  TIFF *tif;
  Color_cell *coltab = NULL;
  char *colName, *compression, *outfileName, *colfileName;

  Iset_message(TRUE);
  Iset_abort(TRUE);
  InitMessage(&argc, argv, xite_app_std_usage_text(
   "Usage: %s [-rgb | -p | -bw] [-c <comp>] <BIFF-image> [<colortable filename>] <TIFF-image>\n\
      where \n\
        -rgb     : Convert a three-band image to an rgb TIFF image\n\
        -p       : Convert a pseudocolor image to a palette TIFF image\n\
        -bw      : Convert a grayscale image to a bilevel TIFF image\n\
                   (threshold=128)\n\
      and <comp> is one of \n\
        lzw      : Lempel-Ziv & Welch compression\n\
        packbits : Packbits compression\n\
        none     : No compression\n\
        huff     : CCITT 1D Huffman compression\n\
        fax3     : CCITT Group 3 Facsimile compression\n\
        fax4     : CCITT Group 4 Facsimile compression\n\
      and <colortable filename> is needed only when '-p' is used.\n"));

  if (argc == 1) Usage(1, NULL);

  rgb         = read_bswitch(&argc, argv, "-rgb");
  palette     = read_bswitch(&argc, argv, "-p");
  bw          = read_bswitch(&argc, argv, "-bw");
  compression = read_switch(&argc,  argv, "-c", 1, "none");

  if (rgb + palette + bw > 1)
    Usage(2, "Choose at most one of '-bw', '-p' and '-rgb'.\n");
  if (argc < 4 && palette)
    Usage(2, "Missing image or colortable argument.\n");
  else if (argc > 4 && palette)
    Usage(2, "Too many arguments.\n");
  else if (!palette && argc < 3)
    Usage(2, "Missing argument.\n");
  else if (!palette && argc > 3)
    Usage(2, "Too many arguments.\n");

  if      (rgb)     format = RGB;
  else if (palette) format = PALETTE;
  else if (!bw)     format = GRAYSCALE;
  else              format = BW;

  if      (!strcmp(compression, "none"))     comp = COMPRESSION_NONE;
  else if (!strcmp(compression, "packbits")) comp = COMPRESSION_PACKBITS;
  else if (!strcmp(compression, "lzw"))      comp = COMPRESSION_LZW;
  else if (!strcmp(compression, "huff"))     comp = COMPRESSION_CCITTRLE;
  else if (!strcmp(compression, "fax3"))     comp = COMPRESSION_CCITTFAX3;
  else if (!strcmp(compression, "fax4"))     comp = COMPRESSION_CCITTFAX4;
/*  else if (!strcmp(compression, "jpeg"))   comp = COMPRESSION_JPEG; */
  else Error(2, "Unknown compression %s.\n", compression);

  img = Iread_image(argv[1]);

  if (format == PALETTE) {
    colfileName = argv[2];
    outfileName = argv[3];
    if ((colImg = Iread_image(colfileName)) == NULL)
      Error(2, "Failed in reading colortable file %s.\n", colfileName);

    coltab = (Color_cell *) malloc((size_t) Iysize(colImg[1])
				   * sizeof(Color_cell));

    if (color_convertfromimg(colImg, coltab, &colName, &colLen) != Iok)
      Error(2, "Failed in reading colortable file %s.\n", colfileName);
    if (colLen != Iysize(colImg[1]))
      Error(2, "Error in colortable file %s.\n", colfileName);
  } else {
    outfileName = argv[2];
  }

  if (strcmp(outfileName, "-") && strcmp(outfileName, "-1"))
    tif = TIFFOpen(outfileName, "w");
  else tif = TIFFFdOpen(1, "Standard Output", "w");

  if (tif == NULL) {
    TIFFError("biff2tiff", "Could not open TIFF file %s\n", outfileName);
    exit(2);
  }

  if (!TIFFSetField(tif, TIFFTAG_IMAGEDESCRIPTION, Ititle(img)))
    TIFFError("biff2tiff", "Could not set TIFF field ImageDescription\n");
  if (!TIFFSetField(tif, TIFFTAG_SOFTWARE, "XITE biff2tiff"))
    TIFFError("biff2tiff", "Could not set TIFF field Software\n");

  if (biff2tiff(img, tif, format, coltab, colLen, comp) != 0) {
    (void) TIFFClose(tif);
    Error(2, "Failed in conversion from BIFF to TIFF.\n");
  } else (void) TIFFClose(tif);

  return(0);
}

#endif /* MAIN */
