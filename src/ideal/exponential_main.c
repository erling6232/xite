

/*C*

________________________________________________________________

        exponential
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
#include <xite/message.h>
#include <xite/readarg.h>
#include <xite/ideal.h>
#include <xite/arithmetic.h>
#include <xite/convert.h>
#include "ideal_L.h"

#ifndef M_PI
#define M_PI    3.14159265358979323846
#endif



int main(int argc, char **argv)
{
  IMAGE   img;
  int     filter_order, xsize, ysize, stat, spatial;
  double  cut;
  char    *arg, *title, *pta;
  IPIXTYP pt;

  InitMessage(&argc, argv, xite_app_std_usage_text(
    "Usage: %s [<option>...] <outimage>\n\
       where <option> is\n\
        -x  <xsize>         : Horizontal size of image\n\
        -y  <ysize>         : Vertical size of image\n\
        -c  <cut_frequency> : Cutoff-frequency (rel. to 0.5*sampl.freq)\n\
        -f  <filter order>  : Filter order\n\
        -t  <image title>   : Title of filter image\n\
        -s                  : Return filter in spatial domain\n\
        -pt <pixtyp>        : Pixeltype for filter\n"));

  if (argc == 1) Usage(E_ARG, NULL);
  arg = argvOptions(argc, argv);

  xsize        = read_iswitch(&argc, argv, "-x", 512);
  ysize        = read_iswitch(&argc, argv, "-y", 512);
  cut          = read_dswitch(&argc, argv, "-c", 0.2);
  filter_order = read_iswitch(&argc, argv, "-f", 5);
  title        = read_switch(&argc, argv, "-t", 1, "Exponential LP filter");
  title        = (strcmp(title, "Lowpass filter") == 0) ?
                 read_switch(&argc, argv, "-title", 1, "Lowpass filter") :
                 title;  
  spatial      = read_bswitch(&argc, argv, "-s");
  pta          = spatial ? read_switch(&argc, argv, "-pt", 1, "r") :
                           read_switch(&argc, argv, "-pt", 1, "c");

  pt = IparsePixtyp(pta);
  if (pt < Iu_byte_typ) Usage(E_PIXTYP, "Illegal value for option '-pt'.\n");

  if (pt != Ireal_typ    && pt != Idouble_typ    &&
      pt != Icomplex_typ && pt != Id_complex_typ) {
    Usage(E_PIXTYP, "Illegal value for option '-pt'.\n");
    exit(1);
  }

  if (argc != 2) Usage(E_ARG, "%s\n", e_s[E_ARG]);

  if (spatial && pt != Icomplex_typ) {
    /* fft2d will be called later, and it will only accept Icomplex_typ. */

    img = Imake_image(1, title, Icomplex_typ, xsize, ysize);
  } else img = Imake_image(1, title, pt, xsize, ysize);

  if (!img) exit(Error(E_MALLOC, "%s\n", e_s[E_MALLOC]));

  if ( (stat = exponentialf(img[1], cut, filter_order)) )
    exit(Error(stat, "%s\n", e_s[stat]));

  if (spatial) {
    IBAND b;

    b = Imake_band(Icomplex_typ, Ixsize(img[1]), Iysize(img[1]));
    fft2d(img[1], b, 1, 0.0);

    if (Ipixtyp(b) != pt) img[1] = mkConvertBand(b, pt);
    else img[1] = b;
  }

  Ihistory(img, argv[0], arg);
  Iwrite_image(img, argv[1]);

  return(0);
}
