

/*C*

________________________________________________________________

        mkImg
        $Id$
        Copyright 1991, Blab, UiO
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
#include <xite/biff.h>
#include <stdlib.h>
#include <xite/draw.h>
#include <xite/blab.h>
#include <xite/message.h>
#include <xite/readarg.h>
#ifdef HAVE_STRINGS_H
# include <strings.h>
#else
# ifdef HAVE_STRING_H
#  include <string.h>
# endif
#endif



/*P:mkImg*

________________________________________________________________

		mkImg
________________________________________________________________

Name:		mkImg - make an image with specified size, pixel type and value

Syntax:		| mkImg [-xsize <xsize>] [-ysize <ysize>] \\
                |       [-xstart <xstart>] [-ystart <ystart>] \\
                |       [-nbands <nbands>] [-title <title>] \\
		|       [-pt <pt>] [-pv <pv>] [-impv <impv>] <outimage>

Description:	'mkImg' creates an image of any size, pixel type and
		value, specified by the switches. <outimage> is the
		name of the new image.

		|   Switches:           Meaning:        Default value:
		| -----------------------------------------------------
		| -xsize <xsize>     Horizontal size	    512
		| -ysize <ysize>     Vertical size	    512
		| -xstart <xstart>   xstart		    1
		| -ystart <ystart>   ystart		    1
		| -nbands <nbands>   number of bands	    1
		| -title <title>     image title	    "new image"
		| -pt <pt>           pixel type		    unsigned byte
		| -pv <pv>           pixel value	    0
		| -impv <impv>       imaginary pixel value  0

		If the title contains more than one word, surround it
		with "double quotes". The pixel type may be given as
		pixel number or short name, according to the following table:

		|   Pixel number:    Short name:    Pixel type:
		| -------------------------------------------------
		|	 3		 ub	    unsigned byte
		|	 4		 sb	    signed byte
		|	 5		 us	    unsigned short
		|	 6		 ss	    signed short
		|	 7		  i	    integer
		|	 8	     r or f	    real (float)
		|	 9		  c	    complex
		|	10		  d	    double
		|	11		 dc	    double complex

		The -impv switch is only meaningful in case of complex
		and double complex pixels, where the "-pv" switch is
		used to specify real part, and "-impv" the imaginary
		part.

See also:	ascii2biff(1), raw2biff(1)

Diagnostics:	Usage 1: Bad number of arguments
		Error 2: Unknown pixel type
		Error 3: Malloc failed

Return value:	0 if ok, otherwise Error number, see diagnostics.

Author:		Tor L|nnestad, BLAB, ifi, UiO

Examples:	| mkImg black.img
		| mkImg -xsize 256 -ysize 256 -pt c -pv 1.0 -impv 1.0 \\
		|   cmplx.img

Id: 		$Id$
________________________________________________________________

*/

int main(int argc, char **argv)
{
  IMAGE img;
  int xsize, ysize, xstart, ystart, pt, nbands, bn;
  double pv, impv;
  char *ptc, *title;

  Iset_message(TRUE);
  InitMessage(&argc, argv, xite_app_std_usage_text(
    "Usage: %s [-xsize <xsize>] [-ysize <ysize>] [-xstart <xstart>] \n\
             [-ystart <ystart>] [-nbands <nbands>] [-title <title>] \n\
             [-pt <pt>] [-pv <pv>] [-impv <impv>]   <outimage>\n"));

  xsize  = read_iswitch(&argc, argv, "-xsize", 512);
  ysize  = read_iswitch(&argc, argv, "-ysize", 512);
  xstart = read_iswitch(&argc, argv, "-xstart", 1);
  ystart = read_iswitch(&argc, argv, "-ystart", 1);
  nbands = read_iswitch(&argc, argv, "-nbands", 1);
  ptc    = read_switch (&argc, argv, "-pt", 1, "ub");
  title  = read_switch (&argc, argv, "-title", 1, "new image"); 
  pv     = read_dswitch(&argc, argv, "-pv", 0.0);
  impv   = read_dswitch(&argc, argv, "-impv", 0.0);

  if (argc == 1) exit(Usage(1, 0));
  if (argc != 2) exit(Usage(1, "Bad number of arguments\n"));

  pt = IparsePixtyp(ptc);
  if (pt < Iu_byte_typ) exit(Error(2, "main: Unknown pixel type\n"));
  img = Imake_image(nbands, title, pt, xsize, ysize);
  if (!img) exit(Error(3, "main: Malloc failed\n"));
  for (bn=1; bn LE nbands; bn++) Iset_start(img[bn], xstart, ystart);

  for (bn=1; bn LE nbands; bn++) drawBand(img[bn], pv, impv);

  Ihistory(img, argv[0], "");
  Iwrite_image(img, argv[1]);
   return(0);
}
