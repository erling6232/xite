

/*C*

________________________________________________________________

        highpassf
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
#include <xite/fft.h>
#include <xite/readarg.h>
#include <xite/message.h>
#include <xite/arithmetic.h>
#include <xite/ideal.h>
#include "ideal_L.h"

#ifndef M_PI
#define M_PI    3.14159265358979323846
#endif



int main(int argc, char **argv)
{
  int      filter_size, xsize, ysize, stat, r, spatial;
  window_type win_type;
  double   cut;
  IMAGE    img;
  char     *arg, *title, *pta, *wt;
  IPIXTYP  pt;

  InitMessage(&argc, argv, xite_app_std_usage_text(
    "Usage: %s [<option>...] <outimage>\n\
       where <option> is\n\
        -x <xsize>         : Horizontal size of image\n\
        -y <ysize>         : Vertical size of image\n\
        -c <cut_frequency> : Cutoff-frequency (rel. to 0.5*sampl.freq)\n\
        -f <filter size>   : Diameter of filter in spatial domain\n\
        -t <image title>   : Title of filter image\n\
        -s                 : Return filter in spatial domain\n\
        -r                 : Set DC value equal to 0.0 (if Fourier domain)\n\
        -w <window-type>   : Choice of window (default Hamming)\n\
        -pt <pixtyp>       : Pixeltype for filter (when spatial domain)\n"));

  if (argc == 1) Usage(E_ARG, NULL);
  arg = argvOptions(argc, argv);

  xsize   = read_iswitch (&argc, argv, "-x", 512);
  ysize   = read_iswitch (&argc, argv, "-y", 512);
  cut     = read_dswitch (&argc, argv, "-c",0.2);
  filter_size = read_iswitch (&argc, argv, "-f", 41);
  title   = read_switch (&argc, argv, "-t", 1, "Windowed ideal HP filter");
  if (strcmp(title, "Windowed ideal HP filter") == 0)
    title = read_switch (&argc, argv, "-title", 1, title);
  wt      = read_switch(&argc, argv, "-w", 1, "hm");
  spatial = read_bswitch(&argc, argv, "-s");
  r       = read_bswitch(&argc, argv, "-r");
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

  if (argc != 2) exit(Error(E_ARG, "%s\n", e_s[E_ARG]));

  img = Imake_image(1, title, pt, xsize, ysize);
  if (!img) exit(Error(E_MALLOC, "%s\n", e_s[E_MALLOC]));

  if (!spatial) {
    if ( (stat = highpassf(img[1], cut, &filter_size, win_type)) )
      exit(Error(stat, "%s\n", e_s[stat]));

    if (r && pt == Icomplex_typ) {
      IC_BAND bnd = (IC_BAND) img[1];

      bnd[1][1].re = 1.0;
    }
  } else {
    if ( (stat = highpass(img[1], cut, &filter_size, win_type)) )
      exit(Error(stat, "%s\n", e_s[stat]));
  }

  Ihistory(img, argv[0], arg);
  Iwrite_image(img, argv[1]);

  return(0);
}
