
/*

________________________________________________________________

        bandpassf
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


#include <stdlib.h>
#include <math.h>
#include <xite/includes.h>
#include <xite/biff.h>
#include XITE_STDIO_H
#include XITE_STRING_H
#include <xite/fft.h>
#include <xite/message.h>
#include <xite/readarg.h>
#include <xite/arithmetic.h>
#include <xite/ideal.h>
#include "ideal_L.h"

#ifndef M_PI
#define M_PI    3.14159265358979323846
#endif



#ifndef MAIN

#ifndef FUNCPROTO
int bandpass(band, low_cut_frequency, high_cut_frequency, filter_size, win_type)
IBAND band;
int filter_size;
double low_cut_frequency, high_cut_frequency;
window_type win_type;
#else /* FUNCPROTO */
int bandpass(IBAND band, double low_cut_frequency, double high_cut_frequency, int filter_size, window_type win_type)
#endif /* FUNCPROTO */
{
  int stat;
  IBAND b2;

  stat = lowpass(band, high_cut_frequency, filter_size, win_type);
  if (stat != 0) return(Error(stat, "%s\n", e_s[stat]));

  b2 = Imake_band(Ipixtyp(band), Ixsize(band), Iysize(band));

  stat = lowpass(b2, low_cut_frequency, filter_size, win_type);
  if (stat != 0) return(Error(stat, "%s\n", e_s[stat]));

  stat = signDiff(band, b2, band, 1.0, 0.0);
  if (stat != 0) return(1);

  Idel_band(&b2);

  return(0);

} /* bandpass() */



#ifndef FUNCPROTO
BiffStatus bandpassf (band, low_cut_frequency, high_cut_frequency, filter_size, win_type)
IBAND band;
double low_cut_frequency, high_cut_frequency;
int filter_size;
window_type win_type;
#else /* FUNCPROTO */
BiffStatus bandpassf (IBAND band, double low_cut_frequency, double high_cut_frequency, int filter_size, window_type win_type)
#endif /* FUNCPROTO */
{
  int stat;

  if (Ipixtyp(band) != Icomplex_typ)
    return(Error(E_PIXTYP, "%d\n", e_s[E_PIXTYP]));

  stat = bandpass(band, low_cut_frequency, high_cut_frequency, filter_size,
		  win_type);
  if (stat != 0) return(Error(stat, "%s\n", e_s[stat]));

  if (fft2d(band, band, 0, 1.0))
    return(Error(E_FFT, "%s\n", e_s[E_FFT]));

  return(0);
}

#endif /* not MAIN */



#ifdef MAIN

#ifndef FUNCPROTO
int main(argc, argv)
int     argc;
char   **argv;
#else /* FUNCPROTO */
int main(int argc, char **argv)
#endif /* FUNCPROTO */
{
  int     filter_size, xsize, ysize, spatial;
  window_type win_type;
  double  lcut, hcut;
  IMAGE   img;
  int     stat, r;
  char    *arg, *title, *pta, *wt;
  IPIXTYP pt;

  InitMessage(&argc, argv, xite_app_std_usage_text(
    "Usage: %s [<option>...] <outimage>\n\
       where <option> is\n\
        -x <xsize>         : Horizontal size of image\n\
        -y <ysize>         : Vertical size of image\n\
        -l <cut_frequency> : Low cutoff-frequency (rel. to 0.5*sampl.freq)\n\
        -h <cut_frequency> : High Cutoff-frequency (rel. to 0.5*sampl.freq)\n\
        -f <filter size>   : Diameter of filter in spatial domain\n\
        -t <image title>   : Title of filter image\n\
        -s                 : Return filter in spatial domain\n\
        -r                 : Set DC value equal to 0.0 (if Fourier domain)\n\
        -w <window-type>   : Choice of window (default Hamming)\n\
        -pt <pixtyp>       : Pixeltype for filter (when spatial domain)\n"));

  if (argc == 1) Usage(E_ARG, NULL);
  arg = argvOptions(argc, argv);

  xsize       = read_iswitch (&argc, argv, "-x", 512);
  ysize       = read_iswitch (&argc, argv, "-y", 512);
  lcut        = read_dswitch (&argc, argv, "-l", 0.2);
  hcut        = read_dswitch (&argc, argv, "-h", 0.7);
  filter_size = read_iswitch (&argc, argv, "-f", 41);
  spatial     = read_bswitch (&argc, argv, "-s");
  r           = read_bswitch(&argc, argv, "-r");
  title       = read_switch (&argc, argv, "-t", 1, "Windowed ideal BP filter");
  if (strcmp(title, "Windowed ideal BP filter") == 0)
    title     = read_switch (&argc, argv, "-title", 1,
			     "Windowed ideal BP filter");
  wt          = read_switch(&argc, argv, "-w", 1, "hm");
  if (spatial) pta = read_switch(&argc, argv, "-pt", 1, "r");
  else pta = read_switch(&argc, argv, "-pt", 1, "c");

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

  if (argc != 2) Usage(E_ARG, "%d\n", e_s[E_ARG]);

  img = Imake_image (1, title, pt, xsize, ysize);
  if (!img) exit(Error(E_MALLOC, "%s\n", e_s[E_MALLOC]));

  if (!spatial) {
    if ( (stat = bandpassf(img[1], lcut, hcut, filter_size, win_type)) )
      exit(Error(stat, "%s\n", e_s[stat]));

    if (r && pt == Icomplex_typ) {
      IC_BAND bnd = (IC_BAND) img[1];

      bnd[1][1].re = 1.0;
    }
  } else {
    if ( (stat = bandpass(img[1], lcut, hcut, filter_size, win_type)) )
      exit(Error(stat, "%s\n", e_s[stat]));
  }

  Ihistory(img, argv[0], arg);
  Iwrite_image(img, argv[1]);

  return(0);
}

#endif  /* MAIN */
