

/*C*

________________________________________________________________

        fftMagPhase
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

#include <xite/includes.h>
#include <xite/biff.h>
#include <stdlib.h>
#include <xite/convert.h>
#include <xite/arithmetic.h>
#include <xite/fft.h>
#include <xite/negate.h>
#include <xite/geometry.h>
#include <xite/message.h>
#include <xite/readarg.h>
#include XITE_UNISTD_H


/*P:fftMagPhase*

________________________________________________________________

		fftMagPhase
________________________________________________________________

Name:		fftMagPhase - 2D Fourier transform and conversion into
                magnitude and phase

Syntax:		fftMagPhase [<option>...] <inimage> <outimage> [<outimage>]

Description:	2D Fourier transform of BIFF image followed by conversion to
                linear or logarithmic (decibels) magnitude and/or phase in
		radians. The logarithmic mapping of magnitude is according to

		| out_pix = (log(eps + |in_pix|) + offset) * scale
		| out_pix = MAX(out_pix, minimum)

Options:	&-fftscale scale
                Scaling of FFT. Default is 1/(xsize*ysize) for forward
		transform, 1.0 for inverse transform.

                &-p
                Only phase output. Default: Phase and logarithmic magnitude.

		&-m
		Only magnitude (linear or logarithmic) output.
		Default: Phase and logarithmic magnitude.

		&-lin
		Linear magnitude. Default: Logarithmic.

                &-r
                Allow pixeltype of 'inimage' to be double complex. Convert
                image to pixeltype complex before applying the Fourier
                transform.

                Default: Allow only single-precision complex or non-complex
                pixeltype for 'inimage', not double complex. Convert the
                image to pixeltype complex before applying the Fourier
                transform.

                &-scale scale
		Default value: 20.0.

		&-eps eps
                Default value: 1 / (10 * xsize * ysize)

                &-offset offset
                Default value: -log(eps)

		&-min minimum
		Lower limit for resulting logarithmic magnitude values.
		Values below 'minimum' will be set equal to 'minimum'.
		Default: No lower limit.

See also:	fft2d(1), logarithm(1), phase(1)

Restrictions:	Not pixeltype double complex for bands in 'inimage', unless
                option '-r' is used.

Return value:	| 0 => Ok
                | 1 => Bad number of arguments
		| 2 => Could not read input image
                | 3 => Error in fft routine
		| 4 => Error in absValue routine
                | 5 => Error in logarithm routine
		| 6 => Error in shift_band routine
		| 7 => Error in phase routine

Author:		Svein Bøe, BLAB, Ifi, UiO

Id: 		$Id$
________________________________________________________________

*/

int main(int argc, char **argv)
{
  IMAGE img, img2;
  IBAND band_in, band_mag, band_phase;
  double scale, eps, offset, minim, fftscale;
  int k, r, p, m, lin, nbands;
  char *args;

  /* Automatic error message display in BIFF format routines: on  */
  Iset_message(1);

  /* Automatic job abortion on error in BIFF format routines: off */
  Iset_abort(0);

  /* Install usage message (end multiple lines with \n\) */
  InitMessage(&argc, argv, xite_app_std_usage_text(
    "Usage: %s [<option>...] <inimage> <outimage> [<outimage>]\n"));

  if (argc == 1) Usage(1, NULL);
  args = argvOptions(argc, argv);

  /* Process input options */
  /* Boolean: b=read_bswitch(&argc, argv, "option-name"); */
  /* Integer: i=read_iswitch(&argc, argv, "option-name", default-value); */
  /* Double:  d=read_dswitch(&argc, argv, "option-name", default-value); */
  /* Text:    t=read_switch (&argc, argv, "option-name",1, "default-value"); */

  p        = read_bswitch(&argc, argv, "-p");
  m        = read_bswitch(&argc, argv, "-m");
  lin      = read_bswitch(&argc, argv, "-lin");
  r        = read_bswitch(&argc, argv, "-r");
  scale    = read_dswitch(&argc, argv, "-s", 0.0);
  scale    = scale == 0.0 ? read_dswitch(&argc, argv, "-scale", 0.0) : scale;
  eps      = read_dswitch(&argc, argv, "-eps", DOUBLE_MIN);
  offset   = read_dswitch(&argc, argv, "-offset", DOUBLE_MAX);
  minim    = read_dswitch(&argc, argv, "-min", -DOUBLE_MAX);
  fftscale = read_dswitch(&argc, argv, "-fftscale", 0.0);

  /* Give usage message if arguments are missing */
  if (!m && !p && argc != 4) Usage(1, "Illegal number of arguments.\n");
  else if ((m || p) && argc != 3) Usage(1, "Illegal number of arguments.\n");

  img = Iread_image(argv[1]);
  if (!img) exit(Error(2, "Could not read image from file %s.\n", argv[1]));

  nbands = Inbands(img);
  if (!p && !m) {
    /* Both phase and magnitude output. */

    img2 = Init_image(nbands, Ititle(img));
  }

  for (k=1; k<=nbands; k++) {
    band_in = img[k];

    if (Ipixtyp(band_in) != Icomplex_typ && 
	(Ipixtyp(band_in) != Id_complex_typ || r != 0))
      band_in = (IBAND) mkConvertBand(band_in, Icomplex_typ);

    if (fft2d(band_in, band_in, 0, fftscale))
      exit(Error(3, "Error in fft2d routine\n"));

    if (!p) {
      /* Magnitude output. */

      band_mag = Imake_band(Ireal_typ, Ixsize(band_in), Iysize(band_in));

      if (lin) {
	/* Linear magnitude. */

	if (absValue(band_in, band_mag))
	  exit(Error(4, "Error in absValue routine.\n"));

	if (shift_band(band_mag, band_mag,
		       Ixsize(band_mag)/2, Iysize(band_mag)/2) != Iok)
        return(Error(6, "Error in shift_band routine.\n"));

      } else {
	/* Logarithmic magnitude. */

	if (logarithm(band_in, band_mag, eps, offset, scale, minim))
	  exit(Error(5, "Error in logarithm-routine.\n"));

	if (shift_band(band_mag, band_mag,
		       Ixsize(band_mag)/2, Iysize(band_mag)/2) != Iok)
        return(Error(6, "Error in shift_band routine.\n"));
      }
    }

    if (!m) {
      /* Phase output. */

      band_phase = Imake_band(Ireal_typ, Ixsize(band_in), Iysize(band_in));

      if (phase(band_in, band_phase))
	exit(Error(7, "Error in phase routine.\n"));
      if (shift_band(band_phase, band_phase,
		     Ixsize(band_phase)/2, Iysize(band_phase)/2) != Iok)
        return(Error(6, "Error in shift_band routine.\n"));
    }

    if (!p) {
      img[k] = band_mag;

      if (!m) img2[k] = band_phase;

    } else img[k] = band_phase;
  }

  Ihistory(img, argv[0], args);
  Iwrite_image(img, argv[2]);

  if (!m && !p) {
    /* Both phase and magnitude output. */

    Ihistory(img2, argv[0], NULL);
    Iwrite_image(img2, argv[3]);
  }

  return(0);
}
