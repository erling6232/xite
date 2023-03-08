

/*C*

________________________________________________________________

        phase.c
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
#include <xite/arithmetic.h>
#include XITE_STDIO_H
#include <xite/geometry.h>
#include <xite/readarg.h>
#include <xite/message.h>

#ifndef M_PI
# define M_PI    3.14159265358979323846
#endif

/*P:fftPhase=phase*/
/*P:phase*

________________________________________________________________

		phase
________________________________________________________________

Name:		phase, fftPhase - phase of complex image

Syntax:		| phase [<option>...] <inimage> <outimage> 
                | fftPhase [<option>...] <inimage> <outimage> 
		
Description:	'phase' computes the phase of a complex image.

                'fftPhase' is retained for compatibility with previous XITE
		versions. It is a script which invokes 'phase'.

Options:        &-b
                The result will be written as a byte image, with the interval
		0 - 2*pi scaled to the interval 0 - 255. Default: The result
		will be scaled to the interval 0 - 2pi on a real image.

                &-shift
                The resulting bands are shifted horizontally xsize/2,
                vertically ysize/2. This is convenient to be able to center
                the (0,0) frequency of a Fourier transform image.

Return value:	| 0 - OK
                | 1 - Bands have different horizontal size
                | 2 - Bands have different vertical size
                | 3 - Input band has wrong pixeltype
                | 4 - Output band has wrong pixeltype
		| 5 - Error in shift_band.
		
See also:	phase(3), power(1), square(1), squareRoot(1), absValue(1),
                imag(1), real(1), fftDisplay(1)

Author:		Trond Muri Sørensen (wrote fftPhase)
Modified:       Svein Bøe

Id: 		$Id$
________________________________________________________________

*/

static char *error[]= {
  "OK",
  "Bands have different horizontal size",
  "Bands have different vertical size",
  "Input band has wrong pixeltype",
  "Output band has wrong pixeltype",
  "Error in shift_band",
};

int main(int argc, char **argv)
{
  IMAGE im1, im2;
  IBAND b1, b2;
  int k, stat, byte, shift;
  IPIXTYP pt;
  char *args;

  Iset_message(1);
  Iset_abort(1);
  InitMessage(&argc, argv, xite_app_std_usage_text(
    "Usage: %s [-b] <inimage> <outimage>\n"));

  if (argc == 1) Usage(1, NULL);
  args = argvOptions(argc, argv);

  byte  = read_switch(&argc, argv, "-b", 0, NULL) != NULL;
  shift = read_bswitch(&argc, argv, "-shift");

  if (argc != 3) Usage(1, "Illegal number of arguments.\n");

  im1 = Iread_image(argv[1]);
  im2 = Init_image(Inbands(im1), Ititle(im1));

  for (k=1; k <= Inbands(im1); k++) {
    b1 = im1[k];
    pt = Ipixtyp(b1);

    if (byte)
      b2 = Imake_band(Iu_byte_typ, Ixsize(b1), Iysize(b1));
    else if (pt == Id_complex_typ)
      b2 = Imake_band(Idouble_typ, Ixsize(b1), Iysize(b1));
    else
      b2 = Imake_band(Ireal_typ, Ixsize(b1), Iysize(b1));

    if ((stat = phase(b1, b2)) != Iok) {
      Error(stat, "%s\n", error[stat]);
      exit(stat);
    }

    if (shift) {
      if ((stat = shift_band(b2, b2, Ixsize(b2)/2, Iysize(b2)/2)) != Iok)
        return(Error(5, "%s\n", error[5]));
    }

    im2[k] = b2;
  }

  Ihistory(im2, argv[0], args);
  Iwrite_image(im2, argv[2]);

  return(0);
}
