

/*

________________________________________________________________

        biff2sunraster.c
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

#ifndef lint

static char *Id = "$Id$, Blab, UiO";

#endif



#include <xite/includes.h>
#include <xite/biff.h>
#include <xite/sunraster.h>
#include XITE_STDIO_H
#include XITE_UNISTD_H
#include <xite/color.h>
#include <xite/readarg.h>
#include <xite/utils.h>
#include <xite/convert.h>
#include XITE_TYPES_H
#include XITE_FCNTL_H
#include XITE_FILE_H
#include XITE_MALLOC_H
/*#include "rast.h"*/

#define ERR_NONE   0
#define ERR_BIFF   1
#define ERR_OPEN   2
#define ERR_WRITE  3
#define ERR_BANDS  4
#define ERR_PIXTYP 5
#define ERR_MALLOC 6
#define ERR_DIM    7
#define ERR_DEPTH  8
#define ERR_ARGV   9



/*P:biff2sunraster*

________________________________________________________________

		biff2sunraster
________________________________________________________________

Name:		biff2sunraster - convert BIFF image to Sun rasterfile

Syntax:		| biff2sunraster [-t <v>] <BIFF image> <Sun rasterfile>
		| biff2sunraster <BIFF image> <Sun rasterfile> \
                |    [<BIFF coltab>]

Description:	Convert a 'BIFF image' to a 'Sun rasterfile'. Use the BIFF
                colortable in 'BIFF coltab' if the 'BIFF coltab' argument is
		supplied.

                If the pixeltype of 'BIFF image' is not unsigned byte, the
		image will be converted to unsigned byte before conversion to
		Sun rasterfile. The pixeltype conversion does not imply any
		scaling. Values too large (or to small) to be represented by
		the new pixeltype are set to the maximum (or minimum) possible
		value. A warning is issued to this effect.

		One-band BIFF images are by default converted to rasters of
		depth 8. Three-band BIFF images are by default converted to
		rasters of depth 24.

		Accepts the standard XITE filenames "", "-" and "-0" for
		'BIFF image' and 'BIFF coltab', and "", "-" and "-1" for
		'Sun rasterfile'.

Options:        &-t v
                Threshold the input image at the value 'v' and write a Sun
		rasterfile of depth 1. This applies only to one-band BIFF
		images. A bit in the Sun rasterfile will be set if the
		corresponding BIFF pixel value is <= 'v'.

Restrictions:	Image bands with pixeltype different from unsigned byte will be
                converted to unsigned byte, without any prior scaling.

		Only one-band and three-band images are allowed as
		'BIFF image'.

Return value:	| 0: Ok
                | 1: Error in handling BIFF file
		| 2: Open error on Sun rasterfile
		| 3: Write error on Sun rasterfile
		| 4: BIFF file must have 1 or 3 band(s)
                | 5: Illegal BIFF pixel type
		| 6: Memory allocation error
		| 7: Image bands must have the same dimensions
		| 8: Illegal combination of depth and number of bands
		| 9: Illegal command line

See also:       write_sunraster(3), sunraster2biff(1)
Author:		Otto Milvang

Examples:	| biff2sunraster sun-raster8/andy.im8 andy.img andy.col
                | biff2sunraster -t 120 sun-raster1/andy.im1 andy.img

Id: 		$Id$
________________________________________________________________

*/

static char *biff2sunraster_error[] =
{
  "Ok",
  "Error in handling BIFF file",
  "Open error on Sun rasterfile",
  "Write error on Sun rasterfile",
  "BIFF file must have 1 or 3 band(s)",
  "Illegal BIFF pixel type",
  "Memory allocation error",
  "Image bands must have the same dimensions",
  "Illegal combination of depth and number of bands",
  "Illegal command line",
};

int main(int argc, char **argv)
{
  IMAGE img;
  Color_tab tab;
  int stat, thres, depth, len = 0, nbands, i;
  char *name;
  
  Iset_message(1);
  Iset_abort(1);
  InitMessage(&argc, argv, xite_app_std_usage_text(
"Usage: %s  [-t <v>] <BIFF image> <Sun rasterfile>\n\
   or: %s <BIFF image> <Sun rasterfile> [<BIFF coltab>]\n"));
  
  if (argc == 1) Usage(0, NULL);
  
  thres = read_iswitch(&argc, argv, "-t", -1);
  if ((thres != -1 && argc != 3) || (thres == -1 && (argc != 3 && argc != 4))){
    Usage(ERR_ARGV, "Illegal number of arguments.\n");
  }
  
  img = Iread_image(argv[1]);
  if ((nbands = Inbands(img)) != 1 && nbands != 3) {
    Usage(ERR_ARGV, "BIFF image must be one-band or three-band.\n");
  }

  if (thres != -1 && nbands == 3) {
    Usage(ERR_ARGV, "Option -t is only allowed with one-band BIFF image.\n");
  }
  
  if (nbands == 3) depth = 24;
  else if (thres == -1) depth = 8;
  else depth = 1;
  
  if ((argc == 4) && (depth == 8)) {
    if ((stat = color_read(argv[3], tab, &name, &len)) != Iok)
      Ierr_action(stat);
  }

  for (i = 1; i <= nbands; i++) {
    if (Ipixtyp(img[i]) > Iu_byte_typ) {
      IBAND b;
      
      Warning(0, "%s%s\n",
	      "Converting BIFF image band to smaller pixeltype. ",
	      "May loose range.");
      
      b      = (IBAND) img[i];
      img[i] = mkConvertBand(img[i], Iu_byte_typ);
      Idel_band(&b);
    }
  } /* for */
  
  stat = write_sunraster(img, argc == 4 ? tab : NULL, len, argv[2], depth,
			 thres);
  if (stat) Error(stat, "%s\n", biff2sunraster_error[stat]);
  
  return(ERR_NONE);
}
