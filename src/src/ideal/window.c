

/*C*

________________________________________________________________

        window
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

#ifndef M_PI
#define M_PI    3.14159265358979323846
#endif

#ifndef MAIN

/*F:hamming=bartlett*/
/*F:hanning=bartlett*/
/*F:rectangle=bartlett*/
/*F:triangle=bartlett*/
/*F:parseWindowName=bartlett*/
/*F:bartlett*

________________________________________________________________

		bartlett, hamming, hanning, rectangle, triangle
________________________________________________________________

Name:		bartlett, hamming, hanning, rectangle, triangle,
                parseWindowName - circularly symmetric window
		functions for filters and parsing of window function name

Syntax:         | #include <xite/ideal.h>
                |
		| double bartlett (double n, int size);
		| double hamming (double n, int size);
		| double hanning (double n, int size);
		| double rectangle (double n, int size);
		| double triangle (double n, int size);
		| window_type parseWindowName(char *name);

Description:    The 'bartlett', 'hamming', 'hanning' (von Hann),
                'rectangle' (box, boxcar) and 'triangle' functions return
		function values	for the various windows, at a certain
		distance 'n'. 'size' is the diameter of the window
		function.

		Window functions are typically used to multiply with a filter
		in the spatial domain.

		These functions should only be called when
		| 0 <= n < size
		Outside this interval, the functions should be considered
		equal to zero.

		'rectangle' is given by
		| w(n)=1.0

		'bartlett' is given by
		|      2n/(size-1),     n<=(size-1)/2
		| w(n)= 
		|      2-2n/(size-1),   n>(size-1)/2

		'triangle' is given by
		|      2(n+1)/(size+1),   n<=(size-1)/2
		| w(n)=
		|      2-2(n+1)/(size+1), n>(size-1)/2
		'triangle' is almost the same as 'bartlett', but 'triangle'
		does not include zeros at the end-points.

		'hamming' is given by
		| w(n)=0.54-0.46*cos(2 Pi n/(size-1))

		'hanning' is given by
		| w(n)=0.5-0.5*cos(2 Pi (n+1)/(size+1))
		'hanning' is sometimes defined slightly different. This
		version does not include zeros at the end-points.

		'parseWindowName' returns a symbolic integer value for various
		forms of window 'name'. Legal forms of 'name' are

		| Full name  Short name  Number
		| -----------------------------
		| rectangle      r         RECTANGLE
		| bartlett       b         BARTLETT
		| triangle       t         TRIANGLE
		| hamming        hm        HAMMING
		| hanning        hn        HANNING
   
Return value:   Function value.

See also:       window(3), lowpass(3), lowpassf(3)

Author:		Trond Muri Sørensen, Svein Bøe
_______________________________________________________________

*/

#ifndef FUNCPROTO
window_type parseWindowName(name)
char *name;
#else /* FUNCPROTO */
window_type parseWindowName(char *name)
#endif /* FUNCPROTO */
{
  window_type val;

  if (! strcasecmp(name, "r"        )) return(RECTANGLE);
  if (! strcasecmp(name, "rectangle")) return(RECTANGLE);
  if (! strcasecmp(name, "b"        )) return(BARTLETT);
  if (! strcasecmp(name, "bartlett" )) return(BARTLETT);
  if (! strcasecmp(name, "t"        )) return(TRIANGLE);
  if (! strcasecmp(name, "triangle" )) return(TRIANGLE);
  if (! strcasecmp(name, "hm"       )) return(HAMMING);
  if (! strcasecmp(name, "hamming"  )) return(HAMMING);
  if (! strcasecmp(name, "hn"       )) return(HANNING);
  if (! strcasecmp(name, "hanning"  )) return(HANNING);

  if ((val = atoi(name)) >= RECTANGLE && val <= HANNING) return(val);

  return(Error(-1, "Unknown window name.\n"));
}

#ifndef FUNCPROTO
double bartlett(n, size)
double   n;
int     size;
#else /* FUNCPROTO */
double bartlett(double n, int size)
#endif /* FUNCPROTO */
{
   double val, v;

   v = 2.0 * n / (double) (size - 1);
   val = (n <= (double) (size - 1) / 2) ? v : (-v + 2);

   return (val);
}

#ifndef FUNCPROTO
double triangle(n, size)
double   n;
int     size;
#else /* FUNCPROTO */
double triangle(double n, int size)
#endif /* FUNCPROTO */
{
   double val, v;

   v = 2.0 * (n + 1) / (double) (size + 1);
   val = (n <= (double) (size - 1) / 2) ? v : (-v + 2);

   return (val);
}

#ifndef FUNCPROTO
double hamming(n, size)
double   n;
int     size;
#else /* FUNCPROTO */
double hamming(double n, int size)
#endif /* FUNCPROTO */
{
   double  val, pl;

   pl = (double) 2.0 * M_PI * (double) n;
   val = (0.54 - 0.46 * cos (pl / (double) (size - 1)));

   return (val);
}


#ifndef FUNCPROTO
double hanning(n, size)
double   n;
int     size;
#else /* FUNCPROTO */
double hanning(double n, int size)
#endif /* FUNCPROTO */
{
   double  val, pl;

   pl = (double) 2.0 * M_PI * (double) (n + 1);
   val = (0.5 - 0.5 * cos (pl / (double) (size + 1)));

   return (val);
}

#ifndef FUNCPROTO
double rectangle(n, size)
double   n;
int     size;
#else /* FUNCPROTO */
double rectangle(double n, int size)
#endif /* FUNCPROTO */
{
   return (1.0);
}




/*F:window*

________________________________________________________________

		window
________________________________________________________________

Name:		window - Insert a window-function in a BIFF band.

Syntax:		| #include <xite/ideal.h>
		| 
		| int window(IBAND band, int size,
		|    window_type win_type);
		| 
Description:	Produce a circularly symmetric window of diameter 'size'
                in 'band'. Legal values for 'win_type' are
                &BARTLETT
                Circularly symmetric Bartlett (triangular) window
                &HAMMING
                Circularly symmetric Hamming window
                &HANNING
                Circularly symmetric Hanning (von Hann) window
                &RECTANGLE
                Circularly symmetric rectangular (box, boxcar) window function
                &TRIANGLE
                Circularly symmetric triangular window function

		The window is returned with its center in the center of
		'band'. This means that the actual center will be located
		between two pixels in the horizontal direction when the
		horizontal band size is even. When the vertical band size
		is even, the center will be located between two pixels in
		the vertical direction. When both horizontal and vertical
		band size is even, the window center will be located between
		four pixels. With both horizontal and vertical band size
		odd, the window center will be located exactly inside the
		center pixel of the band.

		To be applied as a multiplicative window to an image-domain
		filter, make sure that also the filter-function is centered
		around the band	center, or if the filter-function is centered
		around a band corner, the window must be shifted 
		correspondingly before it can be applied. With center on
		a band corner, the resulting filter may be zero phase (if the
		filter function itself obeys the necessary symmetry
		relations).

Return value:   | 0 : Ok
                | 1 : Unknown window type

See also:	bartlett(3), window(1), lowpass(3), lowpassf(3)

Author:		Svein Bøe, Blab, Ifi, UiO

Id: 		$Id$
________________________________________________________________

*/


#ifndef FUNCPROTO
int window(band, size, win_type)
IBAND band;
int size;
window_type win_type;
#else /* FUNCPROTO */
int window(IBAND band, int size, window_type win_type)
#endif /* FUNCPROTO */
{
  long xsize, ysize, x, y, xhalf, yhalf;
  double radx, rady, rad, max_rad, xcent, ycent;
  IPIXTYP pt;

#ifndef FUNCPROTO
   double (*window_func)();
#else /* FUNCPROTO */
   double (*window_func)(double n, int size);
#endif /* FUNCPROTO */

  xsize = Ixsize(band);
  ysize = Iysize(band);
  pt    = Ipixtyp(band);
  xcent = ((double) xsize + 1) / 2;
  ycent = ((double) ysize + 1) / 2;
  xhalf = (long) xcent;
  yhalf = (long) ycent;

  max_rad = (double) (size - 1) / (float) 2.0;

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
    default: return(1);
  }

  switch(pt) {

  case Iu_byte_typ:
  case Is_byte_typ: {

    /* The top left fourth (the fourth closest to (1,1) in the image). */
    for (y = 1; y <= yhalf; y++)
      for (x = 1; x <= xhalf; x++) {
	radx = fabs(xcent - x);
	rady = fabs(ycent - y);
	rad  = sqrt ((double) ((radx * radx) + (rady * rady)));
	
	if (rad <= max_rad) {
	  band[y][x] = window_func(max_rad - rad, size);
	} else band[y][x] = 0;
      }

    /* The bottom left fourth. */
    for (y = 1; y <= yhalf; y++)
      for (x = 1; x <= xhalf; x++) {
	band[ysize + 1 - y][x] = band[y][x];
      }

    /* The top right and bottom right fourths. */
    for (y = 1; y <= yhalf; y++)
      for (x = 1; x <= xhalf; x++) {
	band[y][xsize + 1 - x] = band[y][x];
	band[ysize + 1 - y][xsize + 1 - x] = band[ysize + 1 - y][x];
      }
  } /* Is_byte_typ */
    break;

  case Iu_short_typ:
  case Is_short_typ: {
    IUS_BAND bnd = (IUS_BAND) band;

    /* The top left fourth (the fourth closest to (1,1) in the image). */
    for (y = 1; y <= yhalf; y++)
      for (x = 1; x <= xhalf; x++) {
	radx = fabs(xcent - x);
	rady = fabs(ycent - y);
	rad  = sqrt ((double) ((radx * radx) + (rady * rady)));
	
	if (rad <= max_rad) {
	  bnd[y][x] = window_func(max_rad - rad, size);
	} else bnd[y][x] = 0;
      }

    /* The bottom left fourth. */
    for (y = 1; y <= yhalf; y++)
      for (x = 1; x <= xhalf; x++) {
	bnd[ysize + 1 - y][x] = bnd[y][x];
      }

    /* The top right and bottom right fourths. */
    for (y = 1; y <= yhalf; y++)
      for (x = 1; x <= xhalf; x++) {
	bnd[y][xsize + 1 - x] = bnd[y][x];
	bnd[ysize + 1 - y][xsize + 1 - x] = bnd[ysize + 1 - y][x];
      }

  }
    break;

  case Integer_typ: {
    II_BAND bnd = (II_BAND) band;

    /* The top left fourth (the fourth closest to (1,1) in the image). */
    for (y = 1; y <= yhalf; y++)
      for (x = 1; x <= xhalf; x++) {
	radx = fabs(xcent - x);
	rady = fabs(ycent - y);
	rad  = sqrt ((double) ((radx * radx) + (rady * rady)));
	
	if (rad <= max_rad) {
	  bnd[y][x] = window_func(max_rad - rad, size);
	} else bnd[y][x] = 0;
      }

    /* The bottom left fourth. */
    for (y = 1; y <= yhalf; y++)
      for (x = 1; x <= xhalf; x++) {
	bnd[ysize + 1 - y][x] = bnd[y][x];
      }

    /* The top right and bottom right fourths. */
    for (y = 1; y <= yhalf; y++)
      for (x = 1; x <= xhalf; x++) {
	bnd[y][xsize + 1 - x] = bnd[y][x];
	bnd[ysize + 1 - y][xsize + 1 - x] = bnd[ysize + 1 - y][x];
      }
  }
    break;

  case Ireal_typ: {
    IR_BAND bnd = (IR_BAND) band;

    /* The top left fourth (the fourth closest to (1,1) in the image). */
    for (y = 1; y <= yhalf; y++)
      for (x = 1; x <= xhalf; x++) {
	radx = fabs(xcent - x);
	rady = fabs(ycent - y);
	rad  = sqrt ((double) ((radx * radx) + (rady * rady)));
	
	if (rad <= max_rad) {
	  bnd[y][x] = window_func(max_rad - rad, size);
	} else bnd[y][x] = 0.0;
      }

    /* The bottom left fourth. */
    for (y = 1; y <= yhalf; y++)
      for (x = 1; x <= xhalf; x++) {
	bnd[ysize + 1 - y][x] = bnd[y][x];
      }

    /* The top right and bottom right fourths. */
    for (y = 1; y <= yhalf; y++)
      for (x = 1; x <= xhalf; x++) {
	bnd[y][xsize + 1 - x] = bnd[y][x];
	bnd[ysize + 1 - y][xsize + 1 - x] = bnd[ysize + 1 - y][x];
      }
  }
    break;

  case Idouble_typ: {
    ID_BAND bnd = (ID_BAND) band;

    /* The top left fourth (the fourth closest to (1,1) in the image). */
    for (y = 1; y <= yhalf; y++)
      for (x = 1; x <= xhalf; x++) {
	radx = fabs(xcent - x);
	rady = fabs(ycent - y);
	rad  = sqrt ((double) ((radx * radx) + (rady * rady)));
	
	if (rad <= max_rad) {
	  bnd[y][x] = window_func(max_rad - rad, size);
	} else bnd[y][x] = 0.0;
      }

    /* The bottom left fourth. */
    for (y = 1; y <= yhalf; y++)
      for (x = 1; x <= xhalf; x++) {
	bnd[ysize + 1 - y][x] = bnd[y][x];
      }

    /* The top right and bottom right fourths. */
    for (y = 1; y <= yhalf; y++)
      for (x = 1; x <= xhalf; x++) {
	bnd[y][xsize + 1 - x] = bnd[y][x];
	bnd[ysize + 1 - y][xsize + 1 - x] = bnd[ysize + 1 - y][x];
      }
  }
    break;
  } /* switch */

  return(0);
}

#endif /* not MAIN */

/*P:window*

________________________________________________________________

		window
________________________________________________________________

Name:		window - Make a BIFF image containing a window function

Syntax:		window [<option>...] <outimage>

Description:	Make a BIFF image containing a circularly symmetric window
                function.

		The window is returned with its center in the center of
		the image. To be applied to an image-domain filter, make sure
		that also the filter-function is centered around the image
		center. If the filter-function is centered around an image
		corner, the window must be shifted before it can be applied.

Options:	&-w type
                Specifies the window type. Legal values are
		&&"r" or 0
		Circularly symmetric rectangular window.
		&&"b" or 1
		Circularly symmetric Bartlett window.
		&&"hm" or 2
		Circularly symmetric Hamming window.
		&&"hn" or 3
		Circularly symmetric Hanning window.
		&&"t" or 4
		Circularly symmetric triangular window.
		-&&Default: Hamming window.
		&-x xsize
		Horizontal size of output image. Default: 512.
		&-y ysize
		Vertical size of output image. Default: 512.
		&-s window_size
		Diameter of window function. Default: xsize/2.
		&-t title
		Title of image. Default: "Window".

See also:	window(3), bartlett(3), lowpass(1)

Author:		Svein Bøe, Blab, Ifi, UiO

Id: 		$Id$
________________________________________________________________

*/

#ifdef MAIN

#ifndef FUNCPROTO
int main(argc, argv)
int argc;
char **argv;
#else /* FUNCPROTO */
int main(int argc, char **argv)
#endif /* FUNCPROTO */
{
  IMAGE img;
  window_type win_type;
  long xsize, ysize, window_size;
  char *title, *wt, def_title[100];

  /* Automatic error message display in BIFF format routines: on  */
  Iset_message(TRUE);

  /* Automatic job abortion on error in BIFF format routines: off */
  Iset_abort(FALSE);

  /* Install usage message (end multiple lines with \n\) */
  InitMessage(&argc, argv, xite_app_std_usage_text(
    "Usage: %s [<option>...] <outimage>\n\
       where <option> is chosen from\n\
         -w type      : Type of window function\n\
         -x xsize     : Horizontal size of output image\n\
         -y ysize     : Vertical size of output image\n\
         -s size      : Diameter of window function\n\
         -t title     : Title of output image\n"));

  /* Process input options */
  /* Boolean: b=read_bswitch(&argc, argv, "option-name"); */
  /* Double:  d=read_dswitch(&argc, argv, "option-name", default-value); */
  /* Text:    t=read_switch (&argc, argv, "option-name",1, "default-value"); */

  if (argc == 1) Usage(1, NULL);

  wt          = read_switch(&argc, argv, "-w", 1, "hm");
  xsize       = read_iswitch(&argc, argv, "-x", 512);
  ysize       = read_iswitch(&argc, argv, "-y", 512);
  window_size = read_iswitch(&argc, argv, "-s", xsize);
  (void) strcpy(def_title, wt);
  (void) strcat(def_title, " window");
  title       = read_switch (&argc, argv, "-t", 1, def_title);

  win_type = parseWindowName(wt);
  if (win_type < 0) Usage(1, "Unknown window type.\n");

  /* Give usage message if arguments are missing */
  if (argc != 2) Usage(2, "Illegal number of arguments.\n");

  img = Imake_image(1, title, Idouble_typ, xsize, ysize);

  if (window(img[1], window_size, win_type)) {
    Error(2, "Unknown window type.\n");
  }

  Iwrite_image(img, argv[1]);

  return(0);
}

#endif /* MAIN */
