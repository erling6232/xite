

/*C*

________________________________________________________________

        lowpass
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


#include <math.h>
#include <stdlib.h>
#include <xite/includes.h>
#include <xite/biff.h>
#include XITE_STDIO_H
#include XITE_STRING_H
#include <xite/strings.h>
#include <xite/fft.h>
#include <xite/message.h>
#include <xite/readarg.h>
#include <xite/ideal.h>
#include <xite/arithmetic.h>
#include "ideal_L.h"

#ifndef M_PI
#define M_PI    3.14159265358979323846
#endif



/*F:highpass=lowpass*/
/*F:bandpass=lowpass*/
/*F:bandstop=lowpass*/
/*F:lowpassu=lowpass*/
/*F:lowpass*

________________________________________________________________

		lowpass
________________________________________________________________

Name:		lowpass, highpass, bandpass, bandstop, lowpassu - "Ideal"
                filters truncated with window function in the spatial domain

Syntax:		| #include <xite/ideal.h>
                | 
		| int lowpass (IBAND band, double cut_frequency,
		|    int filter_size, window_type win_type);
		|
		| int highpass (IBAND band, double cut_frequency,
		|    int *filter_size, window_type win_type);
		|
		| int bandpass (IBAND band, double low_cut_frequency,
		|    double high_cut_frequency, int filter_size,
		|    window_type win_type);
		|
		| int bandstop (IBAND band, double low_cut_frequency,
		|    double high_cut_frequency, int *filter_size,
		|    window_type win_type);
		|
		| int lowpassu (IBAND band, double cut_frequency,
		|    int filter_size, window_type win_type, double *sum);
		|
		
Description:    'lowpass' generates a lowpass filter in the spatial domain.
                The filter is made from an ideal lowpass-filter which is
		truncated with a window function in the spatial domain.

                'filter_size' specifies filter diameter. 'cut_frequency' is
		relative to the Nyquist frequency, i.e. a 'cut_frequency'
		of 1.0 means half the sampling frequency. 'win_type'
		specifies window function. Legal values for 'win_type'
		are the constants
		&BARTLETT
		Circularly symmetric Bartlett window
		&HAMMING
		Circularly symmetric Hamming window
		&HANNING
		Circularly symmetric Hanning (von Hann) window
		&RECTANGLE
		Circularly symmetric rectangular (box, boxcar) window function
		&TRIANGLE
		Circularly symmetric triangular window function

		-&The circular symmetry is achieved using Huang''s method.

		'lowpass' is normalized so that the sum of the filter
		coefficients equals 1.0. 'lowpassu' is not normalized. For
		'lowpassu', 'sum' is returned as the sum of the filter
		coefficients.

		For odd 'filter_size', the symmetry-center of the filter is
		located in pixel (1,1) in 'band'. For even 'filter_size',
		the symmetry-center of the filter is located at the upper-left
		corner of pixel (1,1), i.e. the symmetry-center does not
		fall into the center of a pixel.

		For even 'filter_size' and even horizontal band-size, the
		resulting 'filter_size' may become smaller than you''ld
		expect, because of the location of the symmetry-center of the
		filter, as explained above.

		'highpass', 'bandpass' and 'bandstop' behave correspondingly.
		'highpass' is implemented as delta(n1, n2) - 'lowpass', with
		sum of coefficients equal to 0.0. 'bandpass' is implemented as
		the difference between two lowpass-filters. 'bandstop' is
		implemented as delta(n1, n2) - 'bandpass'.

		For 'highpass' and 'bandstop', 'filter_size' should be odd.
		If it is even, a warning is issued and 'filter_size' will be
		made odd. For these functions, 'filter_size' is a reference.

Restrictions:   'band' must be one of the pixeltypes real, double, complex or
                double complex.

		'filter_size' must not exceed horizontal or vertical size of
		'band'.

		For 'highpass' and 'bandstop', 'filter_size' must be odd.
		If it is even, it will be increased by one (or decreased by
		one if this exceeds the size of 'band').

Return value:   | 0 : Ok
		| 1 : Bad pixeltype
                | 2 : Unknown window type
		| 3 : Filtersize larger than bandsize

See also:       bartlett(3), window(3), lowpassf(3), lowpass(1)

Reference:      &Jae S. Lim
                "Two-dimensional Signal And Image Processing"
                Prentice-Hall, 1990

Author:		Trond Muri Sørensen. Major revision by Svein Bøe.

Doc:            Svein Bøe
________________________________________________________________

*/


int lowpassu(IBAND band, double cut_frequency, int filter_size, window_type win_type, double *sum)
{
  int x, y, xsize, ysize, xhalf, yhalf, feven, xeven, yeven, low;
  int xhalf2, yhalf2;
  double max_rad, distx, disty, dist, fn, PPI;
  IPIXTYP pt;
  double (*window_func)(double n, int size);

  switch (win_type) {
    case BARTLETT:  window_func = bartlett;
      break;
    case HAMMING:   window_func = hamming;
      break;
    case HANNING:   window_func = hanning;
      break;
    case RECTANGLE: window_func = rectangle;
      break;
    case TRIANGLE:  window_func = triangle;
      break;
    default: return(Error(E_WINDOW, "%s\n", e_s[E_WINDOW]));
  }

  *sum    = 0.0;
  xsize   = Ixsize (band);
  ysize   = Iysize (band);

  if (filter_size > xsize || filter_size > ysize)
    return(Error(E_FILTERSIZE, "%s\n", e_s[E_FILTERSIZE]));

  pt      = Ipixtyp(band);
  PPI     = M_PI*M_PI;
  max_rad = (float) (filter_size - 1) / (float) 2.0;
  feven   = 1 - filter_size % 2;
  xeven   = 1 - xsize % 2;
  yeven   = 1 - ysize % 2;
  low     = feven ? 1 : 2;
  xhalf   = (xsize + 1) / 2; /* Odd ysize: Includes center row. */
  yhalf   = (ysize + 1) / 2; /* Odd ysize: Includes center row. */
  xhalf2  = xhalf - feven * (!xeven);
  yhalf2  = yhalf - feven * (!yeven);

  switch ((int) pt) {

  case Ireal_typ: {
    IR_BAND bnd = (IR_BAND) band;

    /* Basic quadrant (upper left). */

    for (y = 1; y <= yhalf; y++) 
      for (x = 1; x <= xhalf; x++) {
	distx = abs(x - 1) + (double) feven / 2;
	disty = abs(y - 1) + (double) feven / 2;
	dist  = sqrt ((double) ((distx * distx) + (disty * disty)));

	if (dist == 0.0)
	  fn = cut_frequency * cut_frequency * PPI / (4.0 * M_PI);
	else if (dist <= max_rad) {
	  fn = ((cut_frequency / ((double) 2.0 * dist)) *
		j1 ((double) (cut_frequency * M_PI * dist)));
	  fn *= window_func(dist + max_rad, filter_size);
	} else fn = 0.0;

	bnd[y][x] = (float) fn;
	*sum = *sum + (float) fn;
      }

    /* Lower left. */

    for (y = low; y <= yhalf2; y++)
      for (x = 1; x <= xhalf; x++) {
	  bnd[ysize + low - y][x] = (float) bnd[y][x];
	  *sum = *sum + (float) bnd[y][x];
	}

    /* Upper right. */

    for (y = 1; y <= yhalf; y++)
      for (x = low; x <= xhalf2; x++) {
	bnd[y][xsize + low - x] = (float) bnd[y][x];
	*sum = *sum + (float) bnd[y][x];
      }

    /* Lower right. */

    for (y = low; y <= yhalf2; y++)
      for (x = low; x <= xhalf2; x++) {
	bnd[ysize + low - y][xsize + low - x] = 
	  (float) bnd[ysize + low - y][x];
	*sum = *sum + (float) bnd[ysize + low - y][x];
      }

    /* Initialize zero-samples in cross-center of band. */

    if ((!feven && yeven) || (feven && !yeven))
      for (x = 1; x <= xsize; x++)
	bnd[yhalf2 + 1][x] = 0.0;
    if ((!feven && xeven) || (feven && !xeven))
      for (y = 1; y <= ysize; y++)
	bnd[y][xhalf2 + 1] = 0.0;

  }
    break;

  case Idouble_typ: {
    ID_BAND bnd = (ID_BAND) band;

    /* Basic quadrant (upper left). */

    for (y = 1; y <= yhalf; y++) 
      for (x = 1; x <= xhalf; x++) {
	distx = abs(x - 1) + (double) feven / 2;
	disty = abs(y - 1) + (double) feven / 2;
	dist  = sqrt ((double) ((distx * distx) + (disty * disty)));

	if (dist == 0.0)
	  fn = cut_frequency * cut_frequency * PPI / (4.0 * M_PI);
	else if (dist <= max_rad) {
	  fn = ((cut_frequency / ((double) 2.0 * dist)) *
		j1 ((double) (cut_frequency * M_PI * dist)));
	  fn *= window_func(dist + max_rad, filter_size);
	} else fn = 0.0;

	bnd[y][x] = fn;
	*sum = *sum + fn;
      }

    /* Lower left. */

    for (y = low; y <= yhalf2; y++)
      for (x = 1; x <= xhalf; x++) {
	  bnd[ysize + low - y][x] = bnd[y][x];
	  *sum = *sum + bnd[y][x];
	}

    /* Upper right. */

    for (y = 1; y <= yhalf; y++)
      for (x = low; x <= xhalf2; x++) {
	bnd[y][xsize + low - x] = bnd[y][x];
	*sum = *sum + bnd[y][x];
      }

    /* Lower right. */

    for (y = low; y <= yhalf2; y++)
      for (x = low; x <= xhalf2; x++) {
	bnd[ysize + low - y][xsize + low - x] = 
	  bnd[ysize + low - y][x];
	*sum = *sum + bnd[ysize + low - y][x];
      }

    /* Initialize zero-samples in cross-center of band. */

    if ((!feven && yeven) || (feven && !yeven))
      for (x = 1; x <= xsize; x++)
	bnd[yhalf2 + 1][x] = 0.0;
    if ((!feven && xeven) || (feven && !xeven))
      for (y = 1; y <= ysize; y++)
	bnd[y][xhalf2 + 1] = 0.0;

  }
    break;

  case Icomplex_typ: {
    IC_BAND bnd = (IC_BAND) band;

    /* Basic quadrant (upper left). */

    for (y = 1; y <= yhalf; y++) 
      for (x = 1; x <= xhalf; x++) {
	distx = abs(x - 1) + (double) feven / 2;
	disty = abs(y - 1) + (double) feven / 2;
	dist  = sqrt ((double) ((distx * distx) + (disty * disty)));

	if (dist == 0.0)
	  fn = cut_frequency * cut_frequency * PPI / (4.0 * M_PI);
	else if (dist <= max_rad) {
	  fn = ((cut_frequency / ((double) 2.0 * dist)) *
		j1 ((double) (cut_frequency * M_PI * dist)));
	  fn *= window_func(dist + max_rad, filter_size);
	} else fn = 0.0;

	bnd[y][x].re = (float) fn;
	bnd[y][x].im = (float) 0.0;
	*sum = *sum + (float) fn;
      }

    /* Lower left. */

    for (y = low; y <= yhalf2; y++)
      for (x = 1; x <= xhalf; x++) {
	  bnd[ysize + low - y][x].re = (float) bnd[y][x].re;
	  bnd[ysize + low - y][x].im = 0.0;
	  *sum = *sum + (float) bnd[y][x].re;
	}

    /* Upper right. */

    for (y = 1; y <= yhalf; y++)
      for (x = low; x <= xhalf2; x++) {
	bnd[y][xsize + low - x].re = (float) bnd[y][x].re;
	bnd[y][xsize + low - x].im = 0.0;
	*sum = *sum + (float) bnd[y][x].re;
      }

    /* Lower right. */

    for (y = low; y <= yhalf2; y++)
      for (x = low; x <= xhalf2; x++) {
	bnd[ysize + low - y][xsize + low - x].re = 
	  (float) bnd[ysize + low - y][x].re;
	bnd[ysize + low - y][xsize + low - x].im = 0.0;
	*sum = *sum + (float) bnd[ysize + low - y][x].re;
      }

    /* Initialize zero-samples in cross-center of band. */

    if ((!feven && yeven) || (feven && !yeven))
      for (x = 1; x <= xsize; x++) {
	bnd[yhalf2 + 1][x].re = 0.0;
	bnd[yhalf2 + 1][x].im = 0.0;
      }
    if ((!feven && xeven) || (feven && !xeven))
      for (y = 1; y <= ysize; y++) {
	bnd[y][xhalf2 + 1].re = 0.0;
	bnd[y][xhalf2 + 1].im = 0.0;
      }

  }
    break;

  case Id_complex_typ: {
    IDC_BAND bnd = (IDC_BAND) band;

    /* Basic quadrant (upper left). */

    for (y = 1; y <= yhalf; y++) 
      for (x = 1; x <= xhalf; x++) {
	distx = abs(x - 1) + (double) feven / 2;
	disty = abs(y - 1) + (double) feven / 2;
	dist  = sqrt ((double) ((distx * distx) + (disty * disty)));

	if (dist == 0.0)
	  fn = cut_frequency * cut_frequency * PPI / (4.0 * M_PI);
	else if (dist <= max_rad) {
	  fn = ((cut_frequency / ((double) 2.0 * dist)) *
		j1 ((double) (cut_frequency * M_PI * dist)));
	  fn *= window_func(dist + max_rad, filter_size);
	} else fn = 0.0;

	bnd[y][x].re = fn;
	bnd[y][x].im = 0.0;
	*sum = *sum + fn;
      }

    /* Lower left. */

    for (y = low; y <= yhalf2; y++)
      for (x = 1; x <= xhalf; x++) {
	  bnd[ysize + low - y][x].re = bnd[y][x].re;
	  bnd[ysize + low - y][x].im = 0.0;
	  *sum = *sum + bnd[y][x].re;
	}

    /* Upper right. */

    for (y = 1; y <= yhalf; y++)
      for (x = low; x <= xhalf2; x++) {
	bnd[y][xsize + low - x].re = bnd[y][x].re;
	bnd[y][xsize + low - x].im = 0.0;
	*sum = *sum + bnd[y][x].re;
      }

    /* Lower right. */

    for (y = low; y <= yhalf2; y++)
      for (x = low; x <= xhalf2; x++) {
	bnd[ysize + low - y][xsize + low - x].re = 
	  bnd[ysize + low - y][x].re;
	bnd[ysize + low - y][xsize + low - x].im = 0.0;
	*sum = *sum + bnd[ysize + low - y][x].re;
      }

    /* Initialize zero-samples in cross-center of band. */

    if ((!feven && yeven) || (feven && !yeven))
      for (x = 1; x <= xsize; x++) {
	bnd[yhalf2 + 1][x].re = 0.0;
	bnd[yhalf2 + 1][x].im = 0.0;
      }
    if ((!feven && xeven) || (feven && !xeven))
      for (y = 1; y <= ysize; y++) {
	bnd[y][xhalf2 + 1].re = 0.0;
	bnd[y][xhalf2 + 1].im = 0.0;
      }

  }
    break;

  default: return(Error(E_PIXTYP, "%s\n", e_s[E_PIXTYP]));
    /* break; */

  } /* switch */

  return(0);

} /* lowpassu() */

int lowpass(IBAND band, double cut_frequency, int filter_size, window_type win_type)
{
  int stat;
  double sum;

  stat = lowpassu(band, cut_frequency, filter_size, win_type, &sum);
  if (stat != 0) return(Error(stat, "%s\n", e_s[stat]));

  /* Normalize to get sum of coefficients (DC-value) equal to 1.0. */

  if (sum != 0.0) {
    stat = scale(band, band, 1 / sum, 0.0);
    if (stat) return(Error(E_PIXTYP, "%s\n", e_s[E_PIXTYP]));
  }

  return(0);

} /* lowpass() */



/*F:fftMakeLp=lowpassf*/
/*F:fftMakeHp=lowpassf*/
/*F:fftMakeBp=lowpassf*/
/*F:fftMakeBs=lowpassf*/
/*F:highpassf=lowpassf*/
/*F:bandpassf=lowpassf*/
/*F:bandstopf=lowpassf*/
/*F:lowpassIdealf=lowpassf*/
/*F:butterworthf=lowpassf*/
/*F:exponentialf=lowpassf*/
/*F:parseFilterTypeName=lowpassf*/
/*F:lowpassf*

________________________________________________________________

lowpassf
________________________________________________________________

Name:		lowpassf, highpassf, bandpassf, bandstopf, lowpassIdealf,
                butterworthf, exponentialf, parseFilterTypeName, fftMakeLp,
		fftMakeHp, fftMakeBp, fftMakeBs - make a filter in the
		Fourier domain

Syntax:         | #include <xite/ideal.h>
		|
                | int lowpassf( IBAND band,
                |    double cut_frequency, int filter_size,
		|    window_type win_type );
		|
                | int highassf( IBAND band,
                |    double cut_frequency, int *filter_size,
		|    window_type win_type );
		|
                | int bandpassf( IBAND band,
                |    double low_cut_frequency, double high_cut_frequency,
		|    int filter_size, window_type win_type );
		|
                | int bandstopf( IBAND band,
                |    double low_cut_frequency, double high_cut_frequency,
		|    int *filter_size, window_type win_type );
		|
                | int lowpassIdealf( IBAND band,
                |    double cut_frequency );
		|
		| int butterworthf( IBAND band, double cut_frequency,
		|    int filter_order );
		|
		| int exponentialf( IBAND band, double cut_frequency,
		|    int filter_order );
		|
		| filter_type parseFilterTypeName( char *name );
		|
		| #include <xite/fft.h>
		|
		| int fftMakeLp( IC_BAND band,
                |    double cut_frequency, int filter_size );
		|
		| int fftMakeHp( IC_BAND band,
                |    double cut_frequency, int filter_size );
		|
		| int fftMakeBp( IC_BAND band,
                |    double low_cut_frequency,
                |    double high_cut_frequency, int filter_size );
		|
		| int fftMakeBs( IC_BAND band,
                |    double low_cut_frequency,
                |    double high_cut_frequency, int filter_size );
		|
Description:    'lowpassf' makes a lowpass-filter in the Fourier domain. The
                filter used is an ideal lowpass-filter truncated with a
                window in the spatial domain. 'lowpassf' calls 'lowpass' to
		generate the filter in the spatial domain, then calls 'fft2d'
		to get the Fourier transform. The scaling in the fft is 1.0,
		although the standard/default scaling in 'fft2d' forward
		transform is 1/(xsize*ysize). The choice of scaling will give
		a DC (frequency (0,0)) value of 1.0.

                'filter_size' specifies filter diameter. 'cut_frequency' is
		relative to the Nyquist frequency, i.e. a 'cut_frequency'
		of 1.0 means half the sampling frequency. 'win_type'
		specifies the type of window function. Legal value are
		the constants
		&RECTANGLE
		Circularly symmetric rectangular (box, boxcar) window function
		&BARTLETT
		Circularly symmetric Bartlett window
		&TRIANGLE
		Circularly symmetric triangular window function
		&HAMMING
		Circularly symmetric Hamming window
		&HANNING
		Circularly symmetric Hanning (von Hann) window

		-&The circular symmetry is achieved using Huang''s method.

		'highpassf', 'bandpassf' and 'bandstopf' all call their
		spatial-domain counterparts and 'fft2d' with scaling equal to
		1.0.

		'lowpassIdealf' makes an ideal lowpass-filter in the Fourier
		domain. Unlike 'lowpassf', 'lowpassIdealf' is not made from
		a window-truncated spatial-domain filter. It is the box-shaped
		ideal frequency response which corresponds to a non-realizable
		spatial-domain filter.

		'butterworthf' makes a Butterworth filter in the Fourier
		domain.

		'exponentialf' makes an exponential filter in the Fourier
		domain.

		'parseFilterTypeName' returns a symbolic integer value for
		various forms of filter-type 'name'. Legal forms of 'name' are

		| Full name    Short name  Number
		| lowpass         lp       LOWPASS
		| highpass        hp       HIGHPASS
		| bandpass        bp       BANDPASS
		| bandstop        bs       BANDSTOP
		| lowpass_ideal   ilp      LOWPASS_IDEAL
		| butterworth_lp  blp      BUTTER_LP
		| exponential_lp  elp      EXP_LP

		'fftMakeLp' was a function similar (but not identical) to
		'lowpassf' in previous XITE versions. It is retained for
		compatibility, but now as a macro call to 'lowpassf' with
		'win_type' equal to HAMMING. The old 'fftMakeLp' gave
		results slightly different from the results of 'lowpassf'.
		This is due to differences in the implementation of the window
		function. The main difference is that 'filter_size' will be
		reduced by two in 'lowpass', compared to 'fftMakeLp'.

Restrictions:   Horizontal and vertical size of 'band' must be power of
                two.

		'filter_size' may not be smaller than horizontal or vertical
		size of 'band'.

		For 'highpassf' and 'bandstopf', 'filter_size' should be
		odd. If it is even, it will be made odd and a warning is
		issued. For these two functions, 'filter_size' is a reference.

		'band' must be of pixeltype complex.

Return value:   | 0 - OK
                | 1 - Bad pixel type
		| 2 - Unknown window type
                | 3 - Filter_size larger than bandsize
		| 4 - Error in fft2d

See also:       bartlett(3), window(3), lowpass(3), lowpass(1), fft2d(3),
                phase(3), absValue(3), power(3), real(3), imag(3)

Reference:      &Jae S. Lim
                "Two-dimensional Signal And Image Processing"
                Prentice-Hall, 1990

Author:		Trond Muri Sørensen
________________________________________________________________

*/

BiffStatus lowpassf (IBAND band, double cut_frequency, int filter_size, window_type win_type)
{
   int stat;

   if (Ipixtyp(band) != Icomplex_typ)
     return(Error(E_PIXTYP, "%d\n", e_s[E_PIXTYP]));

   stat = lowpass(band, cut_frequency, filter_size, win_type);
   if (stat != 0) return(Error(stat, "%s\n", e_s[stat]));

   if (fft2d (band, band, 0, 1.0))
      return(Error(E_FFT, "%s\n", e_s[E_FFT]));

   return(0);

} /* lowpassf() */

filter_type parseFilterTypeName(char *name)
{
  filter_type val;

  if (! strcasecmp(name, "lp"            )) return(LOWPASS);
  if (! strcasecmp(name, "lowpass"       )) return(LOWPASS);
  if (! strcasecmp(name, "hp"            )) return(HIGHPASS);
  if (! strcasecmp(name, "highpass"      )) return(HIGHPASS);
  if (! strcasecmp(name, "bp"            )) return(BANDPASS);
  if (! strcasecmp(name, "bandpass"      )) return(BANDPASS);
  if (! strcasecmp(name, "bs"            )) return(BANDSTOP);
  if (! strcasecmp(name, "bandstop"      )) return(BANDSTOP);
  if (! strcasecmp(name, "ilp"           )) return(LOWPASS_IDEAL);
  if (! strcasecmp(name, "lowpass_ideal" )) return(LOWPASS_IDEAL);
  if (! strcasecmp(name, "blp"           )) return(BUTTER_LP);
  if (! strcasecmp(name, "butterworth_lp")) return(BUTTER_LP);
  if (! strcasecmp(name, "elp"           )) return(EXP_LP);
  if (! strcasecmp(name, "exponential_lp")) return(EXP_LP);

  if ((val = atoi(name)) >= LOWPASS && val <= EXP_LP) return(val);

  return(Error(-1, "Unknown filter type.\n"));
}
