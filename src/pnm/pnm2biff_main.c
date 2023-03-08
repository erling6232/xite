

/*C*

________________________________________________________________

        pnm2biff
        $Id$
        Copyright 1994, Blab, UiO
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

#include <xite/biff.h>
#include <xite/readarg.h>
#include <xite/message.h>
#include <xite/xite_pnm.h>

#define  BIFF_BLACK 0
#define  BIFF_WHITE 255

/*P:pnm2biff*

________________________________________________________________

		pnm2biff
________________________________________________________________

Name:		pnm2biff - Convert a pnm (ppm, pgm or pbm) image to BIFF format

Syntax:		| pnm2biff [<option>...] <pnm-infile> <BIFF-outfile>

Description:    Read an image in one of the formats pbm (portable bitmap),
                pgm (portable graymap) or ppm (portable pixmap) from
		'pnm-file' (pnm: portable anymap).
                Produce a BIFF image as output.

		The conversion uses the Netpbm or Pbmplus library. One of
		these must exist on your system, they are not supplied with
		XITE.
 
		If the pnm image is a pbm image, set white equal to the value
		given by option '-hpv', and black equal to the value given
		by option '-lpv'.

		The pixel type of the resulting BIFF image is determined
		by the maximum pixelvalue of the pnm image, according to
		this table
		| max pnm value in #bits         BIFF pixeltype
		|    8                           unsigned byte
                |   16                           unsigned short

		Larger pnm-values generate an error message.

Options:        &-title title, -t title
                Title of resulting BIFF-image. Default: pnm2biff

                &-lpv lpv
                If pnm image is bitmap: Low pixel value. Default: 0

                &-hpv hpv
                If pnm image is bitmap: High pixel value. Default: 255

See also:       pnm2biff(3), biff2pnm(1), raw2biff(1), XITE(1)

Reference:      Documentation for the Pbmplus or Netpbm software packages.

Author:		Svein Bøe, Blab, Ifi, UiO

Examples:       | pnm2biff monabinary.pnm monabinary.img

Id: 		$Id$
________________________________________________________________

*/

int main( int argc, char *argv[])
{
  IMAGE img = NULL;
  FILE* ifp;
  xel** pnm;
  xelval maxval;
  int rows, cols, format, lpv, hpv, stat;
  char *title, *args;

  InitMessage(&argc, argv, xite_app_std_usage_text(
  "Usage: %s [<option>...] <pnm-infile> <BIFF-outfile>\n\
     where <option> is chosen from\n\
     -t <title>   : Title of resulting BIFF-image.\n\
     -lpv <lpv>   : Low pixel-value of BIFF-image if pnm-image is pbm.\n\
     -hpv <hpv>   : High pixel-value of BIFF-image if pnm-image is pbm.\n"));

  Iset_message(1);		/* Warnings from BIFF-software */
  if (argc == 1) Usage(1, NULL);
  args = argvOptions(argc, argv);
  
  pnm_init(&argc, argv);
  
  title = read_switch(&argc, argv, "-t", 1, "pnm2biff");
  title = read_switch(&argc, argv, "-title", 1, title);
  lpv   = read_iswitch(&argc, argv, "-lpv", BIFF_BLACK);
  hpv   = read_iswitch(&argc, argv, "-hpv", BIFF_WHITE);
  
  if (argc != 3) Usage(2, "Illegal number of arguments.\n");
  
  ifp = pm_openr(argv[1]);
  pnm = pnm_readpnm(ifp, &cols, &rows, &maxval, &format);
  
  if ((stat = pnm2biff(pnm, cols, rows, &img, format, maxval, lpv, hpv)) != 0)
    return(Error(stat, "Failed in conversion from pnm to BIFF.\n"));

  (void) Iset_title(img, title);

  if (img) {
    Ihistory(img, argv[0], args);
    Iwrite_image(img, argv[2]);
  }
  
  return(0);
}
