
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



/*P:tiff2biff*

________________________________________________________________

		tiff2biff
________________________________________________________________

Name:		tiff2biff - Convert an image from TIFF to BIFF format.

Syntax:		tiff2biff <TIFF-image> <BIFF-image> [<colortab filename>]

Description:    Convert a TIFF (Tagged Image File Format) image to a BIFF
                image. The conversion is according to TIFF specification 6.0
		and uses the libtiff library by Sam Leffler. This library
		must exist on your system, it is not supplied with XITE.
		If you get problems with importing TIFF images from other
		applications, make sure those applications are also TIFF
		6.0 compliant.

		Byte-ordering between little-endian and big-endian machines
		is handled automatically.

Restrictions:	'tiff2biff' supports the following conversions
                |
		| For grayscale images:
		|
                |     From TIFF                      To BIFF
                | 1-bit bi-level (b/w)           unsigned byte
                | 2-bit                          unsigned byte
                | 4-bit                          unsigned byte
		| 8-bit unsigned byte            unsigned byte
		| 16-bit unsigned short          unsigned short
		| 32-bit signed integer          signed integer
		| 32-bit floating point          floating point
		| 64-bit floating point          64-bit floating
		|
		| For palette (pseudocolor) images:
		|
                |     From TIFF                      To BIFF
                | 1-bit                          unsigned byte
		| 2-bit                          unsigned byte
		| 4-bit                          unsigned byte
		| 8-bit unsigned byte            unsigned byte
		|
		| For rgb color images:
		|
                |     From TIFF                      To BIFF
		| 3*8-bit unsigned byte          three-band unsigned byte
		| 3*16-bit unsigned short        three-band unsigned short
		| 3*32-bit signed integer        three-band signed integer
		| 3*32-bit floating point        three-band floating point
		| 3*64-bit floating point        three-band 64-bit fl. pnt.
		|
See also:	'tiff2biff(3)', 'biff2tiff(1)', 'biff2tiff(3)'

Author:		Helene Schulerud and Svein Bøe, Blab, Ifi, UiO

Examples:	tiff2biff mona.tif mona.img

Id: 		$Id$
________________________________________________________________

*/

int main(int argc, char **argv)
{
  TIFF* tif;
  IMAGE img;
  Color_cell *coltab;
  int numColors;
  char *args;

  Iset_message(TRUE);
  Iset_abort(TRUE);
  InitMessage(&argc, argv, xite_app_std_usage_text(
    "Usage: %s <TIFF-image> <BIFF-image> [<colortab filename>]\n\
       where output <colortab file> is needed for TIFF palette images.\n"));

  if (argc == 1) Usage(1, NULL);
  args = argvOptions(argc, argv);

  if (argc != 3 && argc != 4) Usage(2, "Illegal number of arguments.\n");

  tif = TIFFOpen(argv[1] , "r");
  if ( tif == NULL )
    TIFFError("tiff2biff", "Error opening TIFF file %s\n", argv[1] ); 

  if ((img = tiff2biff(tif, &coltab, &numColors)) == NULL) {
    TIFFError("tiff2biff",
	      "Error. Function tiff2biff returns NULL-pointer.\n");
    return(1);
  }

  (void) TIFFClose(tif);

  if (coltab != NULL) {
    if (argc != 4)
      Usage(2, "Missing colortab filename for palette image.\n");
    if (color_write(argv[3], coltab, "Colormap from TIFF", numColors) != Iok) {
      Error(2, "Error in writing colortable.\n");
      return(1);
    }
  } else if (argc == 4) {
    Usage(2, "Too many arguments. Input image is not palette.\n");
  }

  Ihistory(img, argv[0], args);
  if (Iwrite_image(img, argv[2]) != Iok) {
    Error(2, "Error in writing image.\n");
    return(1);
  }

  return(0);
}
