

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

static IPIXTYP bitsToPixtyp(int bits)
{
  IPIXTYP pt;

  if       (bits <= 8) pt = Iu_byte_typ;
  else if (bits <= 16) pt = Iu_short_typ;
  else pt = -1;

  if (pt == -1) return(Error(pt, "bitsToPixtyp: Illegal number of bits.\n"));

  return(pt);

} /* bitsToPixtyp() */

int pnm2biff(xel** pnm, int xsize, int ysize, IMAGE *img, int format, xelval maxval, int lpv, int hpv)
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

int ppm2biff(xel** pnm, int xsize, int ysize, IMAGE *img, xelval maxval)
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

int pgm2biff(xel** pnm, int xsize, int ysize, IMAGE *img, xelval maxval)
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

int pbm2biff(xel** pnm, int xsize, int ysize, IMAGE *img, xelval maxval, int lpv, int hpv)
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
