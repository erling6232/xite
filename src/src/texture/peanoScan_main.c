/*C*

________________________________________________________________

        peanoScan
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
#include <xite/texture.h>
#include XITE_STDIO_H
#include <xite/blab.h>
#include <xite/message.h>
#include <xite/readarg.h>



/*P:peanoScan*

________________________________________________________________

		peanoScan
________________________________________________________________

Name:		peanoScan - scan an image in "peano" order

Syntax:		| peanoScan [-p <power>] [-x <outxsize>] [-y <outysize>] 
                |           <inimage> <outimage>

Description:    This program will transform an image into a one 
                dimensional signal while conserving the neighborhood
		properties. This is done by using the space filling
		curve called 'Peano Curve'. An input image will be
		scanned according to the Peano curve rules and the
		pixel values will be written in horizontal lines in
		an output image.

		This transformation is well suited for texture analysis
		and for compression, as it utilizes the correlation between
		neighboring pixels in both directions.

		If no '-p power' argument is supplied, scanning is
		continuous over one maximal square window of size some
		power of two.

Options:	&-p power
                Peano scanning over windows of size (2**power)*(2**power).
		Default: one large window.

		&-x outxsize
		Width of the output image. Default = 512.

		&-y outysize
		Height of the output image. Default: High enough to have
		room for the whole transformation.

Examples:       &Size of inimage: 128x128
		The whole band is transformed.

		&Size of inimage: 127x1024
		Only the upper left block of size 64x64 is transformed.

		&power=3
		The image is scanned in sub-windows of size 8x8.

		&&Size of inimage: 128x128
		The whole band is transformed in 16x16 separate blocks, each
		of size 8x8

		&&Size of inimage: 127x1024
		The upper, left block of size 120x1065 is transformed in
		15x128 separate blocks, each of size 8x8.

Restrictions:	Accepts only usigned byte images.

Reference: 	| 1) Joel Quinqueton and Marc Berthod
                |    "A Locally Adaptive Peano Scanning Algorithm"
		|    IEEE Transaction on Pattern Ana. and Machine Inte.
		|    Vol. PAMI-3, No. 4, July 1981
		| 2) P. T. Nguyen and J. Quinqueton
		|    "Space Filling Curves and Texture Analysis"
		|    IEEE Transaction on Pattern Ana. and Machine Inte.
		|    Vol. 2, 1982.

Known bugs:	One happy day there will be a "-i" switch for the inverse
                transform.

Author:		Yogesan and Lønnestad, BLAB, ifi, UiO

Examples:  	| peanoScan mona.img mona_peano.img
		| peanoScan -p 4 mona.img mona_peano.img
                | peanoScan -p 3 -x 128 mona.img mona_peano.img
		| peanoScan -x 64 -y 64 mona.img mona_peano.img 

Id: 		$Id$
________________________________________________________________

*/

int main(int argc, char **argv)
{
  IMAGE inimg, peanoimg;
  int oxsize, oysize, power, bn;
  char *title, arg[100];

  InitMessage(&argc, argv, xite_app_std_usage_text(
    "Usage: %s [-p <power>] [-x <outxsize>] [-y <outysize>] \n\
              <inimage> <outimage> \n"));
  Iset_message(TRUE);
  Iset_abort(TRUE);

  if (argc == 1) Usage(1, NULL);

  oxsize = read_iswitch(&argc, argv, "-x", 512);
  oysize = read_iswitch(&argc, argv, "-y", -1);
  power  = read_iswitch(&argc, argv, "-p", 0);
  title  = read_switch(&argc, argv, "-t", 1, "Peano Scan");
  title  = read_switch(&argc, argv, "-title", 1, title);

  if (argc != 3) Usage(2, "Illegal number of arguments.\n");

  inimg = Iread_image(argv[1]);
  peanoimg = Init_image(Inbands(inimg), title);
  for (bn=1; bn <= Inbands(inimg); bn++) {
    peanoimg[bn] = Imake_band(Iu_byte_typ, oxsize,
      ((oysize == -1) ? ((Ixsize(inimg[bn])*Iysize(inimg[bn])+oxsize-1)/oxsize)
                      : oysize));
    peanoScan(inimg[bn], peanoimg[bn], power);
  }
    
  sprintf(arg, " input image: %s  power:%d", argv[1], power);
  Ihistory(peanoimg, argv[0], arg); 
  Iwrite_image(peanoimg, argv[2]);

  return(0);
}
