

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
#include XITE_STRING_H

/*F:biff2pnmg=biff2pnm*/
/*F:biff2pnmc=biff2pnm*/
/*F:writepnm=biff2pnm*/
/*F:biff2pnm*

________________________________________________________________

		biff2pnm
________________________________________________________________

Name:		biff2pnm, biff2pnmg, biff2pnmc, writepnm - Convert a BIFF
                image to pnm (ppm, pgm or pbm) format

Syntax:		| #include <xite/xite_pnm.h>
		| 
		| int biff2pnm( IMAGE img, xel*** pnm, xelval *maxval,
		|    int *format );
		|
		| int biff2pnmg( IMAGE img, xel*** pnm,
		|    xelval *maxval, int *binary );
		|
		| int biff2pnmc( IMAGE img, xel*** pnm, xelval *maxval );
		|
		| void writepnm( xel **pnm, char *filename, int xsize,
		|    int ysize, xelval maxval, int format, int forceplain );
		|

Description:	'biff2pnm' is a wrapper function which calls one of the
                conversion functions 'biff2pnmg' or 'biff2pnmc', depending
		on the number of image bands in 'img'. With three bands,
		'biff2pnmc' is called. Otherwise, 'biff2pnmg' is called.

		The conversion uses the Netpbm or Pbmplus library. One of
		these must exist on your system, they are not supplied with
		XITE.

		'biff2pnmg' converts the first band of a BIFF image to
		a pnm image with pixelsize equal to eight or sixteen
		bits (unsigned byte or unsigned short), depending on
		how the Netpbm library on your system was compiled
		(with PGM_BIGGRAYS defined or not).

		'biff2pnmc' converts the three bands of a BIFF image
		to a pnm image with each pixel composed of three rgb
		components. The size of each component depends on how
		the Netpbm or Pbmplus library on your system was compiled.
		With PGM_BIGGRAYS defined and PPM_PACKCOLORS undefined,
		the component size is sixteen bits (unsigned short).
		With both PGM_BIGGRAYS and PPM_PACKCOLORS defined, the
		component size is ten bits. With PGM_BIGGRAYS undefined,
		the component size is eight bits.

		'biff2pnmg' and 'biff2pnmc' return the pnm image via
		the 'pnm' argument. Storage is allocated for 'pnm' inside
		the functions and should be released when no longer needed,
		with the call
                | pnm_freearray(pnm, ysize);
	        'pnm_freearray' is a function from the pnm library.

		'binary' returns nonzero if the image is two-valued (and
		thus can be stored as a pbm).

		'maxval' represents the maximum pixel value of the
		image.

		The 'format' argument of 'biff2pnm' returns the value
		PPM_FORMAT if 'biff2pnmc' was called. Otherwise, it
		returns PBM_FORMAT if 'binary' was nonzero, otherwise
		it returns PGM_FORMAT.

		'writepnm' writes the 'pnm' image to a file. The 'format'
		argument must be one of PBM_FORMAT, PGM_FORMAT or PPM_FORMAT.
		With 'forceplain' nonzero, a plain ascii file will be written,
		as opposed to a more compact binary file.

Restrictions:	Accepts all BIFF pixel types, but the range of the output
                pixelvalues may be smaller than that of the input, depending
		on the input pixel type. Also, the precision may be smaller.
		No scaling is performed when converting to a different
		pixel type.

Return value:	| 0 : OK
		| 1 : Not enough memory for pnm image.
		| 2 : BIFF image does not have three bands.
		| 3 : BIFF image bands do not have equal sizes.

See also:       biff2pnm(1), pnm2biff(3), biff2raw(3), XITE(1)

References:     Documentation for the Pbmplus or Netpbm free software
                packages.
Files:		
Author:		Svein Bøe, Blab, Ifi, UiO
Id: 		$Id$
________________________________________________________________

*/

int biff2pnmg(IMAGE img, xel*** pnm, xelval *maxval, int *binary)
{
  PNM_BIFF_BAND_TYPE bnd = NULL;
  int xsize, ysize, x, y, bin, low, high;
  xel **pnml = NULL;
  xelval maxv;
  IPIXTYP pt;

  xsize = Ixsize(img[1]);
  ysize = Iysize(img[1]);
  pnml  = pnm_allocarray(xsize, ysize);

  if (pnml == NULL)
    return(Error(1, "biff2pnmg: Not enough memory for pnm image.\n"));

  pt = Ipixtyp(img[1]);
  if (pt == PNM_BIFF_PIXVAL_TYPE)
    bnd = (PNM_BIFF_BAND_TYPE) img[1];
  else {
    /* Must convert to different pixel type.
       PNM_BIFF_PIXVAL_TYPE is either Iu_byte_typ or Iu_short_typ.
       The corresponding signed type is
       Is_byte_typ = (Iu_byte_typ + 1) and Is_short_typ = (Iu_short_typ + 1).
       */

    if (pt == PNM_BIFF_PIXVAL_TYPE + 1)
      /* PNM_BIFF_PIXVAL_TYPE == Iu_byte_typ and pt == Is_byte_typ
	 or
	 PNM_BIFF_PIXVAL_TYPE == Iu_short_typ and pt == Is_short_typ
       */
      Warning(0, "%s%s\n%s\n",
	      "biff2pnmg: Converting BIFF image from signed to unsigned ",
	      "pixeltype of same size.",
	      "  May zero negative values.");
    else if (pt == PNM_BIFF_PIXVAL_TYPE - 1)
      /* PNM_BIFF_PIXVAL_TYPE == Iu_short_typ and pt == Is_byte_typ */
      Warning(0, "%s%s\n%s\n",
	      "biff2pnmg: Converting BIFF image from signed to unsigned ",
	      "pixeltype of greater size.",
	      "  May zero negative values.");
    else if (pt > PNM_BIFF_PIXVAL_TYPE)
      Warning(0, "%s%s\n",
              "biff2pnmg: Converting BIFF image to smaller pixeltype. ",
              "May loose range/precision.");

    bnd = (PNM_BIFF_BAND_TYPE) mkConvertBand(img[1], PNM_BIFF_PIXVAL_TYPE);
  }

  maxv = 0;
  bin  = 1;
  high = low = (pixval) bnd[1][1];

  for (y=1; y<=ysize; y++) {
    for (x=1; x<=xsize; x++) {
      PNM_ASSIGN1(pnml[y-1][x-1], (pixval) bnd[y][x]);
      if ((pixval) bnd[y][x] > maxv) maxv = bnd[y][x];

      if (bin) {
	if (low != high && bnd[y][x] != low && bnd[y][x] != high)
	  /* At least three different values have been found. */
	  bin = 0;
	else if (bnd[y][x] != low) high = bnd[y][x];
      }
    }
  }

  *pnm    = pnml;
  *maxval = maxv;
  *binary = bin;

  return(0);

} /* biff2pnmg() */

int biff2pnmc(IMAGE img, xel*** pnm, xelval *maxval)
{
  PNM_BIFF_BAND_TYPE bnd = NULL, bnd2 = NULL, bnd3 = NULL;
  int xsize, ysize, x, y;
  xel **pnml = NULL;
  xelval maxv;
  IPIXTYP pt;

  if (Inbands(img) < 3)
    return(Error(2, "biff2pnmc: BIFF image does not have three bands.\n"));

  xsize = Ixsize(img[1]);
  ysize = Iysize(img[1]);

  if (Ixsize(img[2]) != xsize || Ixsize(img[3]) != xsize ||
      Iysize(img[2]) != ysize || Iysize(img[3]) != ysize)
    return(Error(3, "%s\n",
		 "biff2pnmc: BIFF image bands do not have equal sizes."));

  pnml  = pnm_allocarray(xsize, ysize);
  if (pnml == NULL)
    return(Error(1, "biff2pnmc: Not enough memory for pnm image.\n"));

  pt = Ipixtyp(img[1]);
  if (pt == PNM_BIFF_PIXVAL_TYPE) {
    bnd  = (PNM_BIFF_BAND_TYPE) img[1];
    bnd2 = (PNM_BIFF_BAND_TYPE) img[2];
    bnd3 = (PNM_BIFF_BAND_TYPE) img[3];
  } else {
    if (pt == PNM_BIFF_PIXVAL_TYPE + 1)
      Warning(0, "%s%s\n%s\n",
	      "biff2pnmc: Converting BIFF image from signed to unsigned ",
	      "pixeltype of same size.",
	      "  May zero negative values.");
    else if (pt == PNM_BIFF_PIXVAL_TYPE - 1)
      Warning(0, "%s%s\n%s\n",
	      "biff2pnmc: Converting BIFF image from signed to unsigned ",
	      "pixeltype of greater size.",
	      "  May zero negative values.");
    else if (pt > PNM_BIFF_PIXVAL_TYPE)
      Warning(0, "%s%s\n",
              "biff2pnmc: Converting BIFF image to smaller pixeltype. ",
              "May loose range/precision.");

    bnd  = (PNM_BIFF_BAND_TYPE) mkConvertBand(img[1], PNM_BIFF_PIXVAL_TYPE);
    bnd2 = (PNM_BIFF_BAND_TYPE) mkConvertBand(img[2], PNM_BIFF_PIXVAL_TYPE);
    bnd3 = (PNM_BIFF_BAND_TYPE) mkConvertBand(img[3], PNM_BIFF_PIXVAL_TYPE);
  }

  maxv = 0;

  for (y=1; y<=ysize; y++) {
    for (x=1; x<=xsize; x++) {
      PPM_ASSIGN(pnml[y-1][x-1],
		 (pixval) bnd[y][x],
		 (pixval) bnd2[y][x],
		 (pixval) bnd3[y][x]);
      if ((pixval) bnd[y][x]  > maxv) maxv = bnd[y][x];
      if ((pixval) bnd2[y][x] > maxv) maxv = bnd2[y][x];
      if ((pixval) bnd3[y][x] > maxv) maxv = bnd3[y][x];
    }
  }

  *pnm    = pnml;
  *maxval = maxv;

  return(0);

} /* biff2pnmc() */

int biff2pnm(IMAGE img, xel*** pnm, xelval *maxval, int *format)
{
  if (Inbands(img) == 3) {

    *format = PPM_FORMAT;
    return(biff2pnmc(img, pnm, maxval));

  } else {
    int binary, stat;

    stat = biff2pnmg(img, pnm, maxval, &binary);

    if (binary) *format = PBM_FORMAT;
    else *format = PGM_FORMAT;

    return(stat);

  } /* !rgb */

} /* biff2pnm() */

void writepnm(xel **pnm, char *filename, int xsize, int ysize, xelval maxval, int format, int forceplain)
{
  FILE* ofp;

  if (strcmp(filename, "-") && strcmp(filename, "-1"))
    ofp = pm_openw(filename);
  else ofp = stdout;

  pnm_writepnm(ofp, pnm, xsize, ysize, maxval, format, forceplain);

  if (ofp != stdout) pm_close(ofp);

} /* writepnm() */
