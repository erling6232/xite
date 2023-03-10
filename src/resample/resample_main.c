
/*

________________________________________________________________

        resample
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



#include <xite/includes.h>
#include <xite/biff.h>
#include <xite/resample.h>
#include XITE_STDIO_H
#include <stdlib.h>
#include <xite/blab.h>
#include <xite/message.h>

/*P:resample*

________________________________________________________________

		resample
________________________________________________________________

Name:		resample - stretch or compress an image

Syntax:		resample <inimage> <outimage> [<newxsize> <newysize> ...]

Description:    Every band in the old image is stretched or compressed
		to the size specified in the command line. You may
		specify independant sizes for every band. If there are
		more bands than size specifications, the last size
		specification is applied to the rest of the bands.
		Default sizes are 512x512.

		The program uses the value found in the nearest pixel
		in the old image, no interpolation is performed.

Author:		Tor L?nnestad, BLAB, ifi, UiO

Examples:       | resample mona.img largemona.img
		| resample mona.img smallmona.img 256 256
		| resample mona.img:1:1:1 monas.img 128 256 \\
		|     256 128  256 256

Id:             $Id$
________________________________________________________________

*/

int main(int argc, char **argv)
{
  IMAGE i1, i2;
  int bn, xsize, ysize;

  InitMessage(&argc, argv, xite_app_std_usage_text(
     "Usage: %s <infile> <outfile> [<xsize> <ysize>...]\n"));
  Iset_message(TRUE);
  Iset_abort(TRUE);
  if (argc == 1) Usage(1, (char*)0);
  if (argc < 3) Usage(2, "Bad number of arguments\n");

  i1 = Iread_image(argv[1]);
  i2 = Init_image(Inbands(i1), Ititle(i1));
  Icopy_text(i1, i2);
  xsize = 512; ysize = 512;
  FOR (bn=1; bn LE Inbands(i1); bn++)
    if (argc GT 2*bn+1) xsize = atoi(argv[2*bn+1]);
    if (argc GT 2*bn+2) ysize = atoi(argv[2*bn+2]);
    if ((xsize LT 1) OR (ysize LT 1))
      exit(Error(3, "Bad size specification\n"));
    i2[bn] = Imake_band(Iu_byte_typ, xsize, ysize);
    resample(i1[bn], i2[bn]);
  ENDFOR;

  Ihistory(i2, argv[0], "");
  Iwrite_image(i2, argv[2]);
   return(0);
}
