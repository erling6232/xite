

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

#ifndef MAIN

/*F:ppm2biff=pnm2biff*/
/*F:pgm2biff=pnm2biff*/
/*F:pbm2biff=pnm2biff*/
/*F:pnm2biff*

________________________________________________________________

		pnm2biff
________________________________________________________________

Name:		pnm2biff - Convert a pnm (ppm, pgm or pbm) image to BIFF format

Syntax:		| #include <xite/xite_pnm.h>
		| 
		| int pnm2biff( xel** pnm, int xsize, int ysize,
		|    IMAGE *img, int format, xelval maxval,
		|    int lpv, int hpv );
		| int ppm2biff( xel** pnm, int xsize, int ysize,
		|    IMAGE *img, xelval maxval );
		| int pgm2biff( xel** pnm, int xsize, int ysize,
		|    IMAGE *img, xelval maxval );
		| int pbm2biff( xel** pnm, int xsize, int ysize,
		|    IMAGE *img, xelval maxval, int lpv, int hpv );

Description:	'pnm2biff' is a wrapper function which calls one of the
                conversion functions 'ppm2biff', 'pgm2biff' or 'pbm2biff',
		depending on the value of 'format' (PPM_FORMAT, PGM_FORMAT
		and PBM_FORMAT respectively).

		The conversion uses the Netpbm or Pbmplus library. One of
		these must exist on your system, they are not supplied with
		XITE.
		
		'pgm2biff' returns a one-band BIFF image of pixel-type
		unsigned byte or unsigned short, depending on whether
		eight or sixteen bits are needed to hold the value of
		'maxval'. The image title will be empty.

		'pbm2biff' works like 'pgm2biff', except that the BIFF
		image will be two-valued. The two values will be 'lpv',
		where the pbm image value is PBM_BLACK, and 'hpv' otherwise.

		'ppm2biff' works like 'pgm2biff', except that a three-band
		BIFF image is returned.

		Storage is allocated for the image 'img' inside the
		functions. It should be released when no longer needed,
		by a call to 'Idel_image':
		| Idel_image(&img);

Return value:	| 0 : OK
                | 1 : Illegal number of bits
		| 2 : Could not allocate memory for image
		| 3 : Cannot handle pixel type
		| 4 : Unknown PNM format

See also:	pnm2biff(1), biff2pnm(3), raw2biff(1), XITE(1)
References:     Documentation for the Pbmplus or Netpbm free software
                packages.
Author:		Svein Bøe, Blab, Ifi, UiO
Id: 		$Id$
________________________________________________________________

*/

#ifndef FUNCPROTO
static IPIXTYP bitsToPixtyp(bits)
int bits;
#else /* FUNCPROTO */
static IPIXTYP bitsToPixtyp(int bits)
#endif /* FUNCPROTO */
{
  IPIXTYP pt;

  if       (bits <= 8) pt = Iu_byte_typ;
  else if (bits <= 16) pt = Iu_short_typ;
  else pt = -1;

  if (pt == -1) return(Error(pt, "bitsToPixtyp: Illegal number of bits.\n"));

  return(pt);

} /* bitsToPixtyp() */

#ifndef FUNCPROTO
int pnm2biff(pnm, xsize, ysize, img, format, maxval, lpv, hpv)
xel** pnm;
IMAGE *img;
int format, xsize, ysize, lpv, hpv;
xelval maxval;
#else /* FUNCPROTO */
int pnm2biff(xel** pnm, int xsize, int ysize, IMAGE *img, int format, xelval maxval, int lpv, int hpv)
#endif /* FUNCPROTO */
{
  int stat;

  switch (PNM_FORMAT_TYPE(format)) {
  case (PPM_FORMAT) :
    stat = ppm2biff(pnm, xsize, ysize, img, maxval);
    break;
    
  case (PGM_FORMAT) :
    stat = pgm2biff(pnm, xsize, ysize, img, maxval);
    break;

  case (PBM_FORMAT) :
    stat = pbm2biff(pnm, xsize, ysize, img, maxval, lpv, hpv);
    break;

  default:
    stat = Error(4, "Unknown PNM format.\n");
    break;
  } /* switch */

  return(stat);

} /* pnm2biff() */

#ifndef FUNCPROTO
int ppm2biff(pnm, xsize, ysize, img, maxval)
xel** pnm;
IMAGE *img;
xelval maxval;
int xsize, ysize;
#else /* FUNCPROTO */
int ppm2biff(xel** pnm, int xsize, int ysize, IMAGE *img, xelval maxval)
#endif /* FUNCPROTO */
{
  int bitsPerSample, x, y;
  IPIXTYP pt;

  bitsPerSample = pm_maxvaltobits(maxval);
  pt = bitsToPixtyp(bitsPerSample);

  if (pt == -1) return(Error(1, "ppm2biff: Illegal number of bits.\n"));

  if (pt == Iu_byte_typ) {
    IMAGE image = Imake_image(3, "", pt, xsize, ysize);
    if (image == NULL)
      return(Error(2, "ppm2biff: Could not allocate memory for image.\n"));

    for (y=1; y<=ysize; y++) {
      for (x=1; x<=xsize; x++) {
	image[1][y][x] = PPM_GETR(pnm[y-1][x-1]);
	image[2][y][x] = PPM_GETG(pnm[y-1][x-1]);
	image[3][y][x] = PPM_GETB(pnm[y-1][x-1]);
      }
    }
    *img = (IMAGE) image;

  } else if (pt == Iu_short_typ) {
    IUS_IMAGE image = (IUS_IMAGE) Imake_image(3, "", pt, xsize, ysize);
    if (image == NULL)
      return(Error(2, "ppm2biff: Could not allocate memory for image.\n"));

    for (y=1; y<=ysize; y++) {
      for (x=1; x<=xsize; x++) {
	image[1][y][x] = PPM_GETR(pnm[y-1][x-1]);
	image[2][y][x] = PPM_GETG(pnm[y-1][x-1]);
	image[3][y][x] = PPM_GETB(pnm[y-1][x-1]);
      }
    }
    *img = (IMAGE) image;
  } else {
    return(Error(3, "ppm2biff: Cannot handle pixel type %d.\n", pt));
  }

  return(0);

} /* ppm2biff() */

#ifndef FUNCPROTO
int pgm2biff(pnm, xsize, ysize, img, maxval)
xel** pnm;
IMAGE *img;
xelval maxval;
int xsize, ysize;
#else /* FUNCPROTO */
int pgm2biff(xel** pnm, int xsize, int ysize, IMAGE *img, xelval maxval)
#endif /* FUNCPROTO */
{
  int bitsPerSample, x, y;
  IPIXTYP pt;

  bitsPerSample = pm_maxvaltobits(maxval);
  pt = bitsToPixtyp(bitsPerSample);
  if (pt == -1) return(Error(1, "pgm2biff: Illegal number of bits.\n"));

  if (pt == Iu_byte_typ) {
    IMAGE image = Imake_image(1, "", pt, xsize, ysize);
    if (image == NULL)
      return(Error(2, "pgm2biff: Could not allocate memory for image.\n"));

    for (y=1; y<=ysize; y++) {
      for (x=1; x<=xsize; x++) {
	image[1][y][x] = PNM_GET1(pnm[y-1][x-1]);
      }
    }
    *img = (IMAGE) image;

  } else if (pt == Iu_short_typ) {
    IUS_IMAGE image = (IUS_IMAGE) Imake_image(1, "", pt, xsize, ysize);
    if (image == NULL)
      return(Error(2, "pgm2biff: Could not allocate memory for image.\n"));

    for (y=1; y<=ysize; y++) {
      for (x=1; x<=xsize; x++) {
	image[1][y][x] = PNM_GET1(pnm[y-1][x-1]);
      }
    }
    *img = (IMAGE) image;
  } else {
    return(Error(3, "pgm2biff: Cannot handle pixel type %d.\n", pt));
  }

  return(0);

} /* pgm2biff() */

#ifndef FUNCPROTO
int pbm2biff(pnm, xsize,  ysize, img, maxval, lpv, hpv)
xel** pnm;
IMAGE *img;
xelval maxval;
int xsize, ysize, lpv, hpv;
#else /* FUNCPROTO */
int pbm2biff(xel** pnm, int xsize, int ysize, IMAGE *img, xelval maxval, int lpv, int hpv)
#endif /* FUNCPROTO */
{
  int bitsPerSample, x, y;
  IPIXTYP pt;

  bitsPerSample = pm_maxvaltobits(maxval);
  pt = bitsToPixtyp(bitsPerSample);
  if (pt == -1) return(Error(1, "pbm2biff: Illegal number of bits.\n"));

  if (pt == Iu_byte_typ) {
    IMAGE image = Imake_image(1, "", pt, xsize, ysize);
    if (image == NULL)
      return(Error(2, "pbm2biff: Could not allocate memory for image.\n"));

    for (y=1; y<=ysize; y++) {
      for (x=1; x<=xsize; x++) {
	if (PNM_GET1(pnm[y-1][x-1]) == PBM_BLACK)
	  image[1][y][x] = lpv;
	else
	  image[1][y][x] = hpv;
      }
    }

    *img = (IMAGE) image;

  } else if (pt == Iu_short_typ) {
    IUS_IMAGE image = (IUS_IMAGE) Imake_image(1, "", pt, xsize, ysize);
    if (image == NULL)
      return(Error(2, "pbm2biff: Could not allocate memory for image.\n"));

    for (y=1; y<=ysize; y++) {
      for (x=1; x<=xsize; x++) {
	if (PNM_GET1(pnm[y-1][x-1]) == PBM_BLACK)
	  image[1][y][x] = lpv;
	else
	  image[1][y][x] = hpv;
      }
    }

    *img = (IMAGE) image;
  } else {
    return(Error(3, "pbm2biff: Cannot handle pixel type %d.\n", pt));
  }

  return(0);

} /* pbm2biff() */

#endif /* not MAIN */

#ifdef MAIN

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

#ifndef FUNCPROTO
int main( argc, argv )
int argc;
char* argv[];
#else /* FUNCPROTO */
int main( int argc, char *argv[])
#endif /* FUNCPROTO */
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

#endif /* MAIN */
