

/*C*

________________________________________________________________

        ft2ht
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


#include <xite/includes.h>
#include <xite/biff.h>
#include <stdlib.h>
#include <xite/fht.h>
#include XITE_STDIO_H
#include <xite/blab.h>
#include <xite/readarg.h>
#include <xite/message.h>



/*P:ft2ht*

________________________________________________________________

		ft2ht
________________________________________________________________

Name:		ft2ht - 2d fourier to hartley conversion

Syntax:		ft2ht [-t <title>] <inimage> <outimage>

Description:	'ft2ht' transforms a (single precision) complex Fourier
                transform to a (single precision) real Hartley transform.
		Note that the Hartley transform is faster than Fourier.
		If you need Fourier it may be faster to compute the
		Hartley transform and then convert to Fourier by means
		of ht2ft.

                The Hartley transform is given by the Fourier transform
		as follows:

		| H(u,v) = Re{F(u,v)} - Im{F(u,v)}
		|        = H_e(u,v) + H_o(u,v)
		where H_e and H_o are the even and odd parts of H, i.e.
		| H_e(u,v) = (H(u,v) + H(-u,-v))/2
		| H_o(u,v) = (H(u,v) - H(-u,-v))/2

		For images with horizontal and vertical even symmetry,
		H_o(u,v) equals zero, so that
		| H(u,v) = F(u,v)

See also:	ft2ht(3), fft2d(1), fht2d(1), ht2ft(1)

Restrictions:	input image should be a sigle precision complex fourier
                transform.

Return value:	| 0 => ok
                | 1 => bad number of arguments
		| 2 => couldn't read input file
		| 3 => couldn't write output file

Author:		Tor L?nnestad, BLAB, Ifi, UiO

Examples:	ft2ht monaf.img monah.img

Id: 		$Id$
________________________________________________________________

*/

int main(int argc, char **argv)
{
  IMAGE img1, img2;
  int bn, nbands, stat;
  char *title, *args;

  Iset_message(TRUE);
  Iset_abort(TRUE);
  InitMessage(&argc, argv, xite_app_std_usage_text(
    "Usage: %s [-t <title>] <input> <output>\n"));

  if (argc == 1) Usage(1, NULL);
  args = argvOptions(argc, argv);

  title = read_switch(&argc, argv, "-t", 1, (char*)0);
  title = read_switch(&argc, argv, "-title", 1, title);

  if (argc != 3) Usage(1, "Illegal number of arguments.\n");

  img1 = Iread_image(argv[1]);
  if (NOT img1) Error(2, "Can't read input %s.\n", argv[1]);

  nbands = Inbands(img1);
  if (NOT title) title = Ititle(img1);
  
  img2 = Init_image(nbands, title);
  Icopy_text(img1, img2);

  FOR (bn=1; bn LE nbands; bn++)
    img2[bn] = Imake_band(Ireal_typ, Ixsize(img1[bn]), Iysize(img1[bn]));
    stat = ft2ht((IC_BAND)img1[bn], (IR_BAND)img2[bn]);
    if (stat EQ 1)
      Error(2, "Bad input pixel type band %d.\n", bn);
    else if(stat EQ 3)
      Error(2, "Bad size in band %d.\n", bn);
  ENDFOR;

  Ihistory(img2, argv[0], args);
  if (Iwrite_image(img2, argv[2])) {
    Error(2, "Can't write output %s\n", argv[2]);
    exit(3);
  }

  return(0);
}
