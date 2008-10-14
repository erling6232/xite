
/*

________________________________________________________________

        ascii2biff.c
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
#include <ctype.h>
#include <stdlib.h>
#include <xite/includes.h>
#include <xite/biff.h>
#include <xite/convert.h>
#ifdef HAVE_STDIO_H
#  include <stdio.h>
#endif
#ifdef HAVE_STRINGS_H
#  include <strings.h>
#else
#  ifdef HAVE_STRING_H
#    include <string.h>
#  endif
#endif
#include <xite/blab.h>
#include <xite/utils.h>
#ifdef HAVE_MALLOC_H
# include <malloc.h>
#endif
#ifdef HAVE_LIMITS_H
# include <limits.h>
#endif

static char *ascii2biff_errors_s[] = {
  "ascii2biff: OK.\n",
  "ascii2biff: \n",
  "ascii2biff: Can't find file %s.\n",
  "ascii2biff: Bad pixeltype.\n",
  "ascii2biff: Bad xsize or ysize.\n",
  "ascii2biff: Can't calculate image size.\n",
  "ascii2biff: Not enough memory.\n",
};

/*P:ascii2biff*

________________________________________________________________

		ascii2biff
________________________________________________________________

Name:		ascii2biff - convert an ascii file to BIFF format

Syntax:		| ascii2biff [-title title] [-pt pixel_type]
                |          <ascii-file> <BIFF-file> 

Description:	Converts an ascii file to BIFF format.

Options:        &pixel_type

                One of
		  &&ub, uns_byte
		  Unsigned byte, default.

		  &&us, uns_short
		  Unsigned short, 16 bits.

		  &&ss, sign_short
		  Signed short, 16 bits.

		  &&i, integer
		  Signed integer, 32 bits.

		  &&r, real
		  Float, 32 bits.

		&title
		Set the output image title to 'title'

                &ascii-file
		Name of input file, "-" for stdin.

		&BIFF-file
		Name of output file, "-" for stdout.

File format:	The file is an ordinary ascii file. Comments start with a
                '#' or a ';' and stop at end of line. Blank lines or comment
		lines are ignored. Lines with $ in coloumn 1 are treated as
		special arguments. The format is

                | $<keyword> <1 or 2 arguments>

		where <keyword> is one of "type", "size", "xsize", "ysize",
		"start", "xstart", "ystart", "mag", "xmag" or "ymag".

		The format for each <keyword> is:
                | $type   <uns_byte, uns_short, sign_short, integer, real>
                | $size   <xsize> <ysize>
                | $xsize  <xsize>
                | $ysize  <ysize>
                | $start  <xstart> <ystart>
                | $xstart <xstart>
                | $ystart <ystart>
                | $mag    <xmag> <ymag>
                | $xmag   <xmag>
                | $ymag   <ymag>

                The keywords will be used as image band arguments. If the size
                arguments are omitted, the size is calculated from the
                organization of the file. Default start arguments are
                -(size/2), and default mag arguments are 1.

		Pixels inside a line are separated by blanks or tabs. Lines
		are separated by newline. If pixel type is uns_byte,
		uns_short, sign_short or integer, the special value '-' means
		lowest legal value, and '+' means largest legal value.

		&Example 1

		| #
		| $ size  4 3		; xsize = 4, ysize = 3
		| 3 5 6 4 3 2
		| 7 5 3 2 1 3

		The resulting image:
		| #
		| # 3 5 6 4
		| # 3 2 7 5
		| # 3 2 1 3
		| #

		&Example 2

		| #
		| 3 5 6 4 
		| 3 2 7 5
		| 3 2 1 3

		The resulting image:
		| #
		| # 3 5 6 4
		| # 3 2 7 5
		| # 3 2 1 3

Restrictions:	Accepts one band images only.
Author:		Otto Milvang, BLAB, Ifi, UiO
                

Examples:	| 1. Convert a formatted text file
                |    ascii2biff -v -pt ss mona.txt mona.img
		|
		| 2. Convert an unformatted text file without write access.
		|    First create a file, for example mona.def, containing:
		|    # Definition file for mona.txt
		|    $type  sign_short  # set pixeltype
		|    $size 1666 1230    # set xsize ysize
		|    # ----------------------------------
		|    Then convert the file with:
		|    cat mona.def mona.txt | ascii2biff  -pt ss - mona.img

________________________________________________________________

*/

int main(int argc, char **argv)
{
  
  IMAGE img;
  IBAND band;
  IPIXTYP pix_type;
  char *title, *pt, *args;
  int stat;

  InitMessage(&argc, argv, xite_app_std_usage_text(
    "Usage: %s [-title title] [-pt pixel_type] <ascii-file> <BIFF-file>\n"));

  if (argc == 1) Usage(1, NULL);
  args = argvOptions(argc, argv);

  title = read_switch(&argc, argv, "-title", 1, "Converted from ascii");
  title = read_switch(&argc, argv, "-t", 1, title);
  pt    = read_switch(&argc, argv, "-pt", 1, "ub");

  if (argc != 3) Usage(2, "Illegal number of arguments.\n");

  pix_type = IparsePixtyp(pt);
  if (pix_type < Iu_byte_typ || pix_type > Ireal_typ)
    Error(3, ascii2biff_errors_s[3]);

  img    = Init_image(1, title);
  stat   = ascii2biff(&band, argv[1], pix_type);
  img[1] = band;
  Ihistory(img, argv[0], args);
  Iwrite_image(img, argv[2]);

  return(0);
}	
