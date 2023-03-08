
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
