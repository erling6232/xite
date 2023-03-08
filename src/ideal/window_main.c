

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

int main(int argc, char **argv)
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
