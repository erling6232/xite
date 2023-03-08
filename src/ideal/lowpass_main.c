

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

/*lowpass, highpass, bandpass, bandstop, butterworth, exponential, lowpassIdeal, fftMakeLp, fftMakeHp, fftMakeBp, fftMakeBs*/



/*P:fftMakeLp=lowpass*/
/*P:fftMakeHp=lowpass*/
/*P:fftMakeBp=lowpass*/
/*P:fftMakeBs=lowpass*/
/*P:highpass=lowpass*/
/*P:bandpass=lowpass*/
/*P:bandstop=lowpass*/
/*P:butterworth=lowpass*/
/*P:exponential=lowpass*/
/*P:lowpassIdeal=lowpass*/
/*P:lowpass*

________________________________________________________________

lowpass
________________________________________________________________

Name:		lowpass, highpass, bandpass, bandstop, butterworth,
                exponential, lowpassIdeal, fftMakeLp, fftMakeHp, fftMakeBp,
		fftMakeBs - make an "ideal", truncated,	lowpass filter in
		the spatial or Fourier domain

Syntax:		| lowpass [<option>...] <outimage>
                | highpass [<option>...] <outimage>
                | bandpass [<option>...] <outimage>
                | bandstop [<option>...] <outimage>
		| butterworth [<option>...] <outimage>
		| exponential [<option>...] <outimage>
		| lowpassIdeal [<option>...] <outimage>

Description:    'lowpass' makes a lowpass-filter in the spatial or Fourier
                domain. The filter used is an ideal lowpass filter
		truncated with a circularly symmetric window function
		in the image domain.

		'highpass', 'bandpass' and 'bandstop' behave correspondingly.

		The circular symmetry is achieved using Huang''s method.

		'butterworth' makes a Butterworth lowpass filter in the
		spatial or Fourier domain.

		'exponential' makes an exponential lowpass filter in the
		spatial or Fourier domain.

		'lowpassIdeal' makes an "ideal" lowpass filter in the spatial
		or Fourier domain. This filter is the box-shaped ideal
		frequency response which corresponds to a non-realizable
		spatial-domain filter. The spatial domain result (using
		inverse fft on the frequency-domain result), is an aliased
		version of the non-realizable spatial-domain filter.

		'fftMakeLp' was a program similar (but not identical) to
		'lowpass' in previous XITE versions. It is retained for
		compatibility, but now as a script which invokes 'lowpass'.
		The old 'fftMakeLp' gave results slightly different from the
		results of 'lowpass'. This is due to differences in the
		implementation of the window function. The main difference is
		that 'filter_size' will be reduced by two in 'lowpass',
		compared to the old 'fftMakeLp'. So, the new script version of
		'fftMakeLp' increases 'filter_size' by two before invoking
		'lowpass' with Hamming window. Likewise for 'fftMakeHp',
		'fftMakeBp' and 'fftMakeBs'.
  
Options:	Not all options are accepted by all the programs.

                &-c cut_frequency
		Only for 'lowpass', 'highpass', 'butterworth', 'exponential'
		and 'lowpassIdeal'.

		Specified relative to the Nyquist frequency, i.e. a
		'cut_frequency' of 1.0 equals half the sampling frequency.
		Default: 0.2.

                &-f filter_size, -f filter_order
		For 'lowpass', 'highpass', 'bandpass' and 'bandstop' this
		option gives the diameter of the filter. Default: 41.
		For 'butterworth' and 'exponential', it is the filter order.
		Default: 5. This option can not be used with 'lowpassIdeal'.

                &-h high_cut_frequency
		Only for 'bandpass' and 'bandstop'.

		Specified relative to the Nyquist frequency, i.e. a
		'high_cut_frequency' of 1.0 equals half the sampling frequency.
		Default: 0.7.

                &-l low_cut_frequency
		Only for 'bandpass' and 'bandstop'.

		Specified relative to the Nyquist frequency, i.e. a
		'low_cut_frequency' of 1.0 equals half the sampling frequency.
		Default: 0.2.

		&-pt pixtyp
		Only when result is in the spatial domain.

		Pixeltype of filter.

		  &&"r" or 8
		  Real
		  &&"c" or 9
		  Complex
		  &&"d" or 10
		  Double
		  &&"dc" or 11
		  Double complex

		-&&Default: 8 (real) for spatial-domain result, 9 (complex)
		for Fourier-domain result.

                &-r
		Only for 'highpass' and 'bandpass' when result is in the
		Fourier domain.

                Restore image mean value (without scaling the rest of the
                filter correspondingly).

                &-s
		Return filter in spatial domain, not in Fourier domain.
		Default: Fourier domain.
		
		&-t image_title
		Image title. Default: Lowpass filter.

		&-w win_type
		This option can only be used with 'lowpass', 'highpass',
		'bandpass' and 'bandstop'.

		  &&"r" or 0
		  Circularly symmetric rectangular (box, boxcar) window
		  function
		  &&"b" or 1
		  Circularly symmetric Bartlett (triangular) window
		  &&"hm" or 2
		  Circularly symmetric Hamming window
		  &&"hn" or 3
		  Circularly symmetric Hanning (von Hann) window
		  &&"t" or 4
		  Circularly symmetric triangular window function

		-&&Default: 2.

		&-x xsize
		Horizontal size of the filter image. Default 512.

		&-y ysize
		Vertical size of the filter image. Default 512.

Restrictions:   For a result in the Fourier domain, 'xsize' and 'ysize' must
                be a power of two and 'pixtyp' must be complex.

Return value:	| 0 - OK
                | 1 - Bad pixel type
		| 2 - Unknown window type
		| 3 - Filtersize larger than bandsize
		| 4 - Error in fft2d
	        | 5 - Malloc failed
                | 6 - Bad number of arguments

See also:       bartlett(3), window(1), lowpass(3), lowpassf(3), fft2d(1),
                fftMagPhase(1)

Reference:      &Jae S. Lim
                "Two-dimensional Signal And Image Processing"
                Prentice-Hall, 1990

Author:		Trond Muri Sørensen and Svein Bøe
________________________________________________________________

*/


int main(int argc, char **argv)
{
  IMAGE   img;
  int     filter_size, xsize, ysize, stat, spatial;
  window_type win_type;
  double  cut;
  char    *arg, *title, *pta, *wt;
  IPIXTYP pt;

  InitMessage(&argc, argv, xite_app_std_usage_text(
    "Usage: %s [<option>...] <outimage>\n\
       where <option> is\n\
        -x  <xsize>         : Horizontal size of image\n\
        -y  <ysize>         : Vertical size of image\n\
        -c  <cut_frequency> : Cutoff-frequency (rel. to 0.5*sampl.freq)\n\
	-f  <filter size>   : Diameter of filter in spatial domain\n\
        -t  <image title>   : Title of filter image\n\
        -s                  : Return filter in spatial domain\n\
        -w  <window-type>   : Choice of window (default Hamming)\n\
        -pt <pixtyp>        : Pixeltype for filter (when spatial domain)\n"));

  if (argc == 1) Usage(E_ARG, NULL);
  arg = argvOptions(argc, argv);

  xsize       = read_iswitch(&argc, argv, "-x", 512);
  ysize       = read_iswitch(&argc, argv, "-y", 512);
  cut         = read_dswitch(&argc, argv, "-c", 0.2);
  filter_size = read_iswitch(&argc, argv, "-f", 41);
  title       = read_switch(&argc, argv, "-t", 1, "Windowed ideal LP filter");
  title       = (strcmp(title, "Windowed ideal LP filter") == 0) ?
                read_switch(&argc, argv, "-title", 1,
			    "Windowed ideal LP filter") :
		title;  
  wt          = read_switch(&argc, argv, "-w", 1, "hm");
  spatial     = read_bswitch(&argc, argv, "-s");
  pta         = spatial ? read_switch(&argc, argv, "-pt", 1, "r") :
                          read_switch(&argc, argv, "-pt", 1, "c");

  pt = IparsePixtyp(pta);
  if (pt < Iu_byte_typ) Usage(E_PIXTYP, "Illegal value for option '-pt'.\n");
  win_type = parseWindowName(wt);
  if (win_type < 0) Usage(E_WINDOW, "%s\n", e_s[E_WINDOW]);

  if (!spatial && pt != Icomplex_typ) {
    Usage(E_PIXTYP,
	  "For a Fourier-domain result, pixeltype must be complex.\n");
    exit(1);
  }

  if (pt != Ireal_typ    && pt != Idouble_typ    &&
      pt != Icomplex_typ && pt != Id_complex_typ) {
    Usage(E_PIXTYP, "Illegal value for option '-pt'.\n");
    exit(1);
  }

  if (argc != 2) Usage(E_ARG, "%s\n", e_s[E_ARG]);

  img = Imake_image(1, title, pt, xsize, ysize);
  if (!img) exit(Error(E_MALLOC, "%s\n", e_s[E_MALLOC]));

  if (!spatial) {
    if ( (stat = lowpassf(img[1], cut, filter_size, win_type)) )
      exit(Error(stat, "%s\n", e_s[stat]));
  } else {
    if ( (stat = lowpass(img[1], cut, filter_size, win_type)) )
      exit(Error(stat, "%s\n", e_s[stat]));
  }

  Ihistory(img, argv[0], arg);
  Iwrite_image(img, argv[1]);

  return(0);
}
