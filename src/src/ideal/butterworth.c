

/*C*

________________________________________________________________

        butterworth
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
#ifdef HAVE_STDIO_H
#  include <stdio.h>
#endif
#ifdef HAVE_STRINGS_H
#  include <strings.h>
#else
#  ifdef HAVE_STRING_H
#    include <string.h>
#  endif
#endif
#ifdef HAVE_MALLOC_H
#  include <malloc.h>
#endif
#include <xite/fft.h>
#include <xite/message.h>
#include <xite/readarg.h>
#include <xite/ideal.h>
#include <xite/arithmetic.h>
#include <xite/convert.h>
#include "ideal_L.h"

#ifndef M_PI
# define M_PI    3.14159265358979323846
#endif
#ifndef XITE_ABS
# define XITE_ABS(i) ((i) >= 0 ? (i) : (-(i)))
#endif



#ifndef MAIN

int butterworthf(IBAND band, double cut_frequency, int filter_order)
{
  int x, y, xsize, ysize, xhalf, yhalf, feven, xeven, yeven, low, filter_size;
  int xhalf2, yhalf2, status = 0;
  double max_rad, *distx, *disty, dist, fn, PPI, sum = 0.0;
  IPIXTYP pt;

  sum         = 0.0;
  xsize       = Ixsize (band);
  ysize       = Iysize (band);
  filter_size = xsize;

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
  cut_frequency = cut_frequency * max_rad;

  if (xsize != ysize) {
    disty = (double *) malloc(sizeof(double) * (yhalf + xhalf));
    distx = &disty[yhalf];
  } else {
    disty = (double *) malloc(sizeof(double) * yhalf);
    distx = disty;
  }

  for (y = 1; y <= yhalf; y++) 
    disty[y-1] = XITE_ABS(y - 1) + (double) feven / 2.0;

  if (xsize != ysize) {
    for (x = 1; x <= xhalf; x++)
      distx[x-1] = XITE_ABS(x - 1) + (double) feven / 2.0;
  }

  switch ((int) pt) {

  case Ireal_typ: {
    IR_BAND bnd = (IR_BAND) band;

    /* Basic quadrant (upper left). */

    for (y = 1; y <= yhalf; y++) 
      for (x = 1; x <= xhalf; x++) {
	dist  = sqrt ((double) ((distx[x-1] * distx[x-1]) +
				(disty[y-1] * disty[y-1])));

	if (cut_frequency == 0.0) fn = 0.0;
	else if (dist <= max_rad) {
	  fn = 1 / (1 + pow((double) dist / cut_frequency,
			    (double) 2.0 * filter_order));
	} else fn = 0.0;

	bnd[y][x] = (float) fn;
	sum = sum + (float) fn;
      }

    /* Lower left. */

    for (y = low; y <= yhalf2; y++)
      for (x = 1; x <= xhalf; x++) {
	  bnd[ysize + low - y][x] = (float) bnd[y][x];
	  sum = sum + (float) bnd[y][x];
	}

    /* Upper right. */

    for (y = 1; y <= yhalf; y++)
      for (x = low; x <= xhalf2; x++) {
	bnd[y][xsize + low - x] = (float) bnd[y][x];
	sum = sum + (float) bnd[y][x];
      }

    /* Lower right. */

    for (y = low; y <= yhalf2; y++)
      for (x = low; x <= xhalf2; x++) {
	bnd[ysize + low - y][xsize + low - x] = 
	  (float) bnd[ysize + low - y][x];
	sum = sum + (float) bnd[ysize + low - y][x];
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
        dist  = sqrt ((double) ((distx[x-1] * distx[x-1]) +
				(disty[y-1] * disty[y-1])));

	if (cut_frequency == 0.0) fn = 0.0;
	else if (dist <= max_rad) {
	  fn = 1 / (1 + pow((double) dist / cut_frequency,
			    (double) 2.0 * filter_order));
	} else fn = 0.0;

        bnd[y][x] = fn;
        sum = sum + fn;
      }

    /* Lower left. */

    for (y = low; y <= yhalf2; y++)
      for (x = 1; x <= xhalf; x++) {
          bnd[ysize + low - y][x] = bnd[y][x];
          sum = sum + bnd[y][x];
        }

    /* Upper right. */

    for (y = 1; y <= yhalf; y++)
      for (x = low; x <= xhalf2; x++) {
        bnd[y][xsize + low - x] = bnd[y][x];
        sum = sum + bnd[y][x];
      }

    /* Lower right. */

    for (y = low; y <= yhalf2; y++)
      for (x = low; x <= xhalf2; x++) {
        bnd[ysize + low - y][xsize + low - x] = 
          bnd[ysize + low - y][x];
        sum = sum + bnd[ysize + low - y][x];
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
        dist  = sqrt ((double) ((distx[x-1] * distx[x-1]) +
				(disty[y-1] * disty[y-1])));

	if (cut_frequency == 0.0) fn = 0.0;
	else if (dist <= max_rad) {
	  fn = 1 / (1 + pow((double) dist / cut_frequency,
			    (double) 2.0 * filter_order));
	} else fn = 0.0;

        bnd[y][x].re = (float) fn;
        bnd[y][x].im = (float) 0.0;
        sum = sum + (float) fn;
      }

    /* Lower left. */

    for (y = low; y <= yhalf2; y++)
      for (x = 1; x <= xhalf; x++) {
          bnd[ysize + low - y][x].re = (float) bnd[y][x].re;
          bnd[ysize + low - y][x].im = 0.0;
          sum = sum + (float) bnd[y][x].re;
        }

    /* Upper right. */

    for (y = 1; y <= yhalf; y++)
      for (x = low; x <= xhalf2; x++) {
        bnd[y][xsize + low - x].re = (float) bnd[y][x].re;
        bnd[y][xsize + low - x].im = 0.0;
        sum = sum + (float) bnd[y][x].re;
      }

    /* Lower right. */

    for (y = low; y <= yhalf2; y++)
      for (x = low; x <= xhalf2; x++) {
        bnd[ysize + low - y][xsize + low - x].re = 
          (float) bnd[ysize + low - y][x].re;
        bnd[ysize + low - y][xsize + low - x].im = 0.0;
        sum = sum + (float) bnd[ysize + low - y][x].re;
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
        dist  = sqrt ((double) ((distx[x-1] * distx[x-1]) +
				(disty[y-1] * disty[y-1])));

	if (cut_frequency == 0.0) fn = 0.0;
	else if (dist <= max_rad) {
	  fn = 1 / (1 + pow((double) dist / cut_frequency,
			    (double) 2.0 * filter_order));
	} else fn = 0.0;

        bnd[y][x].re = fn;
        bnd[y][x].im = 0.0;
        sum = sum + fn;
      }

    /* Lower left. */

    for (y = low; y <= yhalf2; y++)
      for (x = 1; x <= xhalf; x++) {
          bnd[ysize + low - y][x].re = bnd[y][x].re;
          bnd[ysize + low - y][x].im = 0.0;
          sum = sum + bnd[y][x].re;
        }

    /* Upper right. */

    for (y = 1; y <= yhalf; y++)
      for (x = low; x <= xhalf2; x++) {
        bnd[y][xsize + low - x].re = bnd[y][x].re;
        bnd[y][xsize + low - x].im = 0.0;
        sum = sum + bnd[y][x].re;
      }

    /* Lower right. */

    for (y = low; y <= yhalf2; y++)
      for (x = low; x <= xhalf2; x++) {
        bnd[ysize + low - y][xsize + low - x].re = 
          bnd[ysize + low - y][x].re;
        bnd[ysize + low - y][xsize + low - x].im = 0.0;
        sum = sum + bnd[ysize + low - y][x].re;
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

  default: status = 1;
    break;
    /* break; */

  } /* switch */

  free((char *) disty); disty = NULL;

  if (status) return(Error(E_PIXTYP, "%s\n", e_s[E_PIXTYP]));
  else return(0);

} /* butterworthf() */

#endif /* not MAIN */

#ifdef MAIN

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
  title        = read_switch(&argc, argv, "-t", 1, "Butterworth LP filter");
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

  if ( (stat = butterworthf(img[1], cut, filter_order)) )
    exit(Error(stat, "%s\n", e_s[stat]));

  if (spatial) {
    IBAND b;

    b = Imake_band(Icomplex_typ, Ixsize(img[1]), Iysize(img[1]));
    fft2d(img[1], b, 1, 0.0);

    if (Ipixtyp(b) != pt) {
      img[1] = mkConvertBand(b, pt);
    } else img[1] = b;
  }

  Ihistory(img, argv[0], arg);
  Iwrite_image(img, argv[1]);

  return(0);
}

#endif /* MAIN */
