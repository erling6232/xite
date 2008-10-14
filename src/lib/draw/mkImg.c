

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



#ifndef MAIN

/*F:drawBand*

________________________________________________________________

		drawBand
________________________________________________________________

Name:		drawBand - draw constant value into a whole band

Syntax:         | #include <xite/draw.h>
		|
                | int drawBand( IBAND band, double pv,
                |    double impv );
Description:    'drawBand' draws a constant pixel value into a whole
		band. The band may have any pixel type. Notice that
		the pixel value 'pv' is given as double, no matter what
		pixel type the band contains. If the band is complex
		or double complex, the imaginary value should be given
		as argument 'impv'.

Diagnostics:	| Warning 1: band argument nil
		| Error   2: unknown pixel type

Return value:	0 => ok, otherwise error/warning number above.

Author:		Tor L|nnestad, BLAB, ifi, UiO

Id: 		$Id$
________________________________________________________________

*/

#define FORALLYX    for (y=1; y <= ysize; y++) \
      for (x=1; x <= xsize; x++)


int drawBand(IBAND band, double pv, double impv)
{
  ISB_BAND isbb;
  IUS_BAND iusb;
  ISS_BAND issb;
  II_BAND  iib;
  IR_BAND  irb;
  IC_BAND  icb;
  ID_BAND  idb;
  IDC_BAND idcb;
  UNS_BYTE ub;
  SIGN_BYTE sb;
  UNS_SHORT us;
  SIGN_SHORT ss;
  INTEGER  i;
  int x, y, xsize, ysize, pt;

  if (! band) return(Warning(1,"drawBand: band argument nil\n"));
  xsize = Ixsize(band);
  ysize = Iysize(band);
  pt    = Ipixtyp(band);

  switch (pt) {
    case Iu_byte_typ:
      if (pv > 255.0) ub = 255; else if (pv < 0.0) ub = 0; else ub = (UNS_BYTE)(pv+0.5);
      FORALLYX band[y][x] = ub;
      return(0);

    case Is_byte_typ:
      if (pv > 127.0) sb = 127; else if (pv < -128.0) sb = -128; else sb = (SIGN_BYTE)(pv+0.5);
      isbb = (ISB_BAND) band;
      FORALLYX isbb[y][x] = sb;
      return(0);

    case Iu_short_typ:
      if (pv > 65535.0) us = 65535; else if (pv < 0.0) us = 0; else us = (UNS_SHORT)(pv+0.5);
      iusb = (IUS_BAND) band;
      FORALLYX iusb[y][x] = us;
      return(0);

    case Is_short_typ:
      if (pv > 32767.0) ss = 32767; else if (pv < -32768.0) ss = -32768;
        else ss = (SIGN_SHORT)(pv+0.5);
      issb = (ISS_BAND) band;
      FORALLYX issb[y][x] = ss;
      return(0);

    case Integer_typ:
      if (pv >  2147483647.0) i = 2147483647; else if (pv < -2147483647.0) i = -2147483647;
        else i = (INTEGER)(pv+0.5);
      iib = (II_BAND) band;
      FORALLYX iib[y][x] = i;
      return(0);

    case Ireal_typ:
      irb = (IR_BAND) band;
      FORALLYX irb[y][x] = (REAL)pv;
      return(0);

    case Icomplex_typ:
      icb = (IC_BAND) band;
      FORALLYX { icb[y][x].re = (REAL)pv; icb[y][x].im = (REAL)impv; }
      return(0);

    case Idouble_typ:
      idb = (ID_BAND) band;
      FORALLYX idb[y][x] = pv;
      return(0);

    case Id_complex_typ:
      idcb = (IDC_BAND) band;
      FORALLYX { idcb[y][x].re = pv; idcb[y][x].im = impv; }
      return(0);

    default : return(Error(2,"drawBand: unknown pixel type"));
  }
}

#endif /* not MAIN */

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

#ifdef MAIN

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

#endif /* MAIN */
