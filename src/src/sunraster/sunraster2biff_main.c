

/*

________________________________________________________________

        sunraster2biff.c
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
#include <xite/message.h>
#include <xite/readarg.h>
#include <xite/utils.h>
#include XITE_TYPES_H
#include XITE_FILE_H
#include XITE_FCNTL_H
#include XITE_MALLOC_H
#include "rast.h"

#define ERR_NONE   0
#define ERR_BIFF   1
#define ERR_MALLOC 2
#define ERR_OPEN   3
#define ERR_READ   4
#define ERR_MAGIC  5
#define ERR_FMT    6
#define ERR_WRITE  7
#define ERR_ARGV   8



/*P:sunraster2biff*

________________________________________________________________

		sunraster2biff
________________________________________________________________

Name:		sunraster2biff - convert Sun rasterfile to BIFF-file

Syntax:		sunraster2biff <sunrasterfile> <BIFF-file> [<BIFF colorfile>]

Description:	Convert a 'Sun rasterfile' to a 'BIFF file'.
                If raster depth equals 8 and 'BIFF colorfile' is specified,
		a colorfile is written.

		For 24- and 32-bit rasterfiles, the new Sun standard ordering
		of BGR (blue, green, red) is used. The old ordering
		was RGB.

		Accepts the standard XITE filenames "", "-" and "-0" for
		'sunrasterfile' and "", "-" and "-1" for 'BIFF-file' and
		'BIFF colorfile'.

Restriction:	Accepts the following types of Sun rasterfiles: 'standard',
                'old' and 'rgb'. For 24- and 32-bit rasterfiles (i.e. composite
		color), the 'standard' type means that the color order is
		BGR or XBGR (the new Sun color ordering), whereas the 'rgb'
		type is used for color order RGB and XRGB.

Return value:	| 0: Ok
                | 1: BIFF error
		| 2: Memory allocation error
		| 3: Sun rasterfile open error
		| 4: Read error on Sun rasterfile
		| 5: Bad magic number (not a Sun rasterfile)
		| 6: Rasterfile format not supported
		| 7: Write error on BIFF file
		| 8: Illegal command line

Examples:	| sunraster2biff sun-raster8/andy.im8 andy.img andy.col
                | sunraster2biff sun-raster1/andy.im1 andy.img

See also:       read_sunraster(3), biff2sunraster(1)

Author:		Otto Milvang
Revised:        Svein Bøe

Id: 		$Id$
________________________________________________________________

*/

static char *sunraster2biff_error[] =
{
  "Ok",
  "BIFF error",
  "Memory allocation error",
  "Sun rasterfile open error",
  "Read error on Sun rasterfile",
  "Bad magic number (not a Sun rasterfile)",
  "Rasterfile format not supported",
  "Write error on BIFF file",
  "Illegal command line",
};

int main(int argc, char **argv)
{
  IMAGE img;
  Color_tab tab;   /* This is non-NULL (it refers to the first element of a
		    * a vector). */
  int stat, depth; 
  
  InitMessage(&argc, argv, xite_app_std_usage_text(
    "Usage: %s <Sun rasterfile> <BIFF-file> [<BIFF colorfile>]\n"));
  
  if (argc == 1) Usage(0, NULL);
  if (argc != 3 && argc != 4)
    Usage(ERR_ARGV, "Illegal number of arguments.\n");
  
  stat = read_sunraster(&img, tab, argv[1], &depth);
  if (stat) Error(stat, "%s\n", sunraster2biff_error[stat]);
  
  if ((argc == 4) && (depth == 8)) {
    color_write(argv[3], tab, "Converted from Sun Raster", 256);
  }
  
  stat = Iwrite_image(img, argv[2]);
  if (stat) Error(ERR_WRITE, "%s\n", sunraster2biff_error[ERR_WRITE]);
  
  return(ERR_NONE);
}
