

/*C*

________________________________________________________________

        biff2pnm
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
#include <xite/convert.h>
#include <xite/xite_pnm.h>

/*P:biff2pnm*

________________________________________________________________

		biff2pnm
________________________________________________________________

Name:		biff2pnm -  Convert a BIFF image to pnm (ppm, pgm or pbm)
                format

Syntax:		| biff2pnm [<option>...] <BIFF-infile> <pnm-outfile>

Description:    'biff2pnm' reads a BIFF image and writes it in one of the
                formats pbm (portable bitmap), pgm (portable graymap) or ppm
		(portable pixmap).

		The conversion uses the Netpbm or Pbmplus library. One of
		these must exist on your system, they are not supplied with
		XITE.

		If the BIFF image has exactly three bands, assume that
		it is an rgb image and write it as ppm. The size of
		each color component depends on how the Netpbm or Pbmplus
		library on your system was compiled. With PGM_BIGGRAYS
		defined and PPM_PACKCOLORS undefined, the component
		size is sixteen bits (unsigned short). With both
		PGM_BIGGRAYS and PPM_PACKCOLORS defined, the component
		size is ten bits. With PGM_BIGGRAYS undefined, the
		component size is eight bits.

		Otherwise, convert only the first band. If it is
		two-valued, write a pbm image. Otherwise, write a pgm
		image. The pgm image will have pixelsize equal to
		eight or sixteen bits (unsigned byte or unsigned
		short), depending on how the Netpbm or Pbmplus library on your
		system was compiled (with PGM_BIGGRAYS undefined or
		defined, respectively).

		'biff2pnm' converts the BIFF band to the correct
		pixeltype before conversion to pbm/pgm/ppm. The
		pixeltype conversion does not imply any scaling.
		Values too large (or too small) to be represented by
		the new pixeltype are set to the maximum (or minimum)
		possible value. A warning is issued to this effect.

		Use "-" as filename arguments for stdin and stdout.

Options:	&-a
		Write a plain (ascii) format file, as opposed to a raw (binary)
		one.

See also:       biff2pnm(3), pnm2biff(1), biff2raw(1), XITE(1)

References:     Documentation for the Pbmplus or Netpbm software packages.

Author:		Svein Bøe, Blab, Ifi, UiO

Examples:       | biff2pnm monabinary.pnm monabinary.img

Id: 		$Id$
________________________________________________________________

*/

int main(int argc, char **argv)
{
  IMAGE img;
  xel** pnm = NULL;
  xelval maxval;
  int rgb, ascii, format, stat;

  InitMessage(&argc, argv, xite_app_std_usage_text(
  "Usage: %s [<option>...] <BIFF-infile> <pnm-outfile>\n\
     where <option> is chosen from\n\
     -a           : Write a plain (ascii) format file, as opposed to a\n\
                    raw (binary) format one.\n"));

  Iset_message(1);		/* Warnings from BIFF-software */
  Iset_abort(1);

  if (argc == 1) Usage(1, NULL);

  pnm_init(&argc, argv);

  ascii = read_bswitch(&argc, argv, "-a");

  if (argc != 3) Usage(2, "Illegal number of arguments.\n");

  img = Iread_image(argv[1]);

  if ((stat = biff2pnm(img, &pnm, &maxval, &format)) != 0) {
    return(Error(stat, "Failed in conversion from BIFF to pnm.\n"));
  }

  writepnm(pnm, argv[2], Ixsize(img[1]), Iysize(img[1]), maxval,
	   format, ascii);

  if (pnm != NULL) pnm_freearray(pnm, Iysize(img[1]));

  return(0);
}
