

/*C*

________________________________________________________________

        ImageFormat.c
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

/*F:ImageTitle=ImageFormat*/
/*F:ImageAddress=ImageFormat*/
/*F:ImageWidth=ImageFormat*/
/*F:ImageHeight=ImageFormat*/
/*F:ImageLineLength=ImageFormat*/
/*F:ImageXstart=ImageFormat*/
/*F:ImageYstart=ImageFormat*/
/*F:ImagePixName=ImageFormat*/
/*F:ImagePixPrintFormat=ImageFormat*/
/*F:ImagePixNameToBIFFPixTyp=ImageFormat*/
/*F:ImageFormat*

________________________________________________________________

		ImageFormat
________________________________________________________________

Name:		ImageFormat, ImageTitle, ImageAddress, ImageWidth, ImageHeight,
                ImageLineLength, ImageXstart, ImageYstart, ImagePixName,
		ImagePixPrintFormat, ImagePixNameToBIFFPixTyp - Format
		dependent module for XITE Image	Widget

Syntax:         | #include <xite/ImageFormat.h>
                | 
                | char *ImageTitle( IMAGE img );
		|
                | unsigned char *ImageAddress( IBAND band );
		|
                | int ImageWidth( IBAND band );
		|
                | int ImageHeight( IBAND band );
		|
                | int ImageLineLength( IBAND band );
		|
                | int ImageXstart( IBAND band );
		|
                | int ImageYstart( IBAND band );
		|
                | char *ImagePixName( IBAND band );
		|
		| char *ImagePixPrintFormat( IPIXTYP pixtyp );
		|
		| IPIXTYP ImagePixNameToBIFFPixTyp( char *image_pix_name );

Description:    'ImageAddress' returns a pointer to the least significant byte
                (assuming big-endian byte-ordering) of the first pixel of the
		BIFF band.

		'ImageWidth' returns the number of pixels in a row of the
		image.

		'ImageHeight' returns the number of pixels in a column of the
		image.

		'ImageLineLength' gives the number of bytes needed to
		represent one row of the image.

		'ImageXstart' and 'ImageYstart' return the relative global
		coordinates for the upper left corner of the image.

See also:       'Image(3)', 'ImageOverlay(3)'
________________________________________________________________

*/

#include <xite/includes.h>
#include <xite/biff.h>
#include <X11/Intrinsic.h>
#include <xite/ImageFormat.h>
#include "ImagePixName.h"
#include XITE_STDIO_H

static char *pixname[] =
{
  NULL,
  NULL,
  NULL,
  ImageBytePixel, 
  ImageSignedBytePixel,
  ImageUnsignedShortPixel,
  ImageShortPixel,
  ImageLongPixel,
  ImageFloatPixel,
  ImageComplexPixel,
  ImageDoublePixel,
  ImageDoubleComplexPixel,
  NULL,
  NULL,
};

#ifndef FUNCPROTO
char *ImageTitle(img)
IMAGE img;
#else /* FUNCPROTO */
char *ImageTitle(IMAGE img)
#endif /* FUNCPROTO */
{
  return(Ititle(img));
}

#ifndef FUNCPROTO
unsigned char *ImageAddress(band)
IBAND band;
#else /* FUNCPROTO */
unsigned char *ImageAddress(IBAND band)
#endif /* FUNCPROTO */
{
   return(&band[1][Ipixsize(Ipixtyp(band))/8]);
}

#ifndef FUNCPROTO
int ImageWidth(band)
IBAND band;
#else /* FUNCPROTO */
int ImageWidth(IBAND band)
#endif /* FUNCPROTO */
{
  return(Ixsize(band));
}

#ifndef FUNCPROTO
int ImageHeight(band)
IBAND band;
#else /* FUNCPROTO */
int ImageHeight(IBAND band)
#endif /* FUNCPROTO */
{
  return(Iysize(band));
}

#ifndef FUNCPROTO
int ImageLineLength(band)
IBAND band;
#else /* FUNCPROTO */
int ImageLineLength(IBAND band)
#endif /* FUNCPROTO */
{
  return(Iysize(band) > 1 ? 
	 (&band[2][0] - &band[1][0]) :
	 Ixsize(band));
}

#ifndef FUNCPROTO
int ImageXstart(band)
IBAND band;
#else /* FUNCPROTO */
int ImageXstart(IBAND band)
#endif /* FUNCPROTO */
{
  return(Ixstart(band));
}

#ifndef FUNCPROTO
int ImageYstart(band)
IBAND band;
#else /* FUNCPROTO */
int ImageYstart(IBAND band)
#endif /* FUNCPROTO */
{
  return(Iystart(band));
}

#ifndef FUNCPROTO
char *ImagePixName(band)
IBAND band;
#else /* FUNCPROTO */
char *ImagePixName(IBAND band)
#endif /* FUNCPROTO */
{
  return(pixname[(Ipixtyp(band))]);
}

#ifndef FUNCPROTO
char *ImagePixPrintFormat(pixtyp)
IPIXTYP pixtyp;
#else /* FUNCPROTO */
char *ImagePixPrintFormat(IPIXTYP pixtyp)
#endif /* FUNCPROTO */
{
  return(IpixPrintFormat(pixtyp));
}

#ifndef FUNCPROTO
IPIXTYP ImagePixNameToBIFFPixTyp(image_pix_name)
char *image_pix_name;
#else /* FUNCPROTO */
IPIXTYP ImagePixNameToBIFFPixTyp(char *image_pix_name)
#endif /* FUNCPROTO */
{
  if (!image_pix_name) return(Iunknown_typ);

  if (!strcmp(image_pix_name, ImageBytePixel))          return(Iu_byte_typ);
  if (!strcmp(image_pix_name, ImageSignedBytePixel))    return(Is_byte_typ);
  if (!strcmp(image_pix_name, ImageUnsignedShortPixel)) return(Iu_short_typ);
  if (!strcmp(image_pix_name, ImageShortPixel))         return(Is_short_typ);
  if (!strcmp(image_pix_name, ImageLongPixel))          return(Integer_typ);
  if (!strcmp(image_pix_name, ImageFloatPixel))         return(Ireal_typ);
  if (!strcmp(image_pix_name, ImageComplexPixel))       return(Icomplex_typ);
  if (!strcmp(image_pix_name, ImageDoublePixel))        return(Idouble_typ);
  if (!strcmp(image_pix_name, ImageDoubleComplexPixel)) return(Id_complex_typ);

  return(Iunknown_typ);

} /* ImagePixNameToBIFFPixTyp() */
