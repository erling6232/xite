
/*

________________________________________________________________

        linearTrans
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
static int of=0, uf=0;

#include <xite/includes.h>
#include <xite/biff.h>
#include <xite/principal.h>
#include XITE_STDIO_H
#include <xite/blab.h>
#include <xite/message.h>
#include <xite/readarg.h>
#include XITE_MALLOC_H
#include XITE_TYPES_H

/*P:linearTrans*

________________________________________________________________

		linearTrans
________________________________________________________________

Name:		linearTrans - linear image to image transformation

Syntax:		| linearTrans [-log] -wf <weight_file> <inimage> <outimage>
		|
		| linearTrans [-log] <inimage> <outimage> <weight>...

Description:    Performs a linear transformation from 'inimage'
                to 'outimage' according to specified weights. Every
		pixel (seen as a vector through all bands) becomes
		a linear combination of the elements of input pixel
		vector.

		The total number of weights should equal the square of the
		number of bands in the images.

Options:        &-wf weight_file
		The weights may be given in the command (using the
		'weights' argument), or in a file with the -wf option.

		&-log
		The number of underflows and overflows are written to
		stderr.

Restrictions:   The bands of 'inimage' must have pixel type unsigned byte.
                All the input bands in 'inimage' should have the same size.

Return value:   0 if ok, 1 otherwise

Author:		Tor Lønnestad, BLAB, Ifi, UiO

Examples:       &rotate axes of 3-band image
		|   linearTrans inp.img outp.img 
		|     0.5 0.5 0.0  0.0 0.5 0.5  0.5 0.0 0.5

		&calculate sum and difference of 2-band image
		|   linearTrans inp.img sumdiff.img 
		|     0.5 0.5  1.0 -1.0

Id: 		$Id$
________________________________________________________________

*/

int main(int argc, char **argv)
{
  IMAGE i1, i2;
  IR_BAND rb;
  FILE *wf;
  char *wfn, *args;
  int log, i, j, nbands, wcount;

  InitMessage(&argc, argv, xite_app_std_usage_text(
    "Usage: %s [-log] -wf <weight file> <inimage> <outimage> \n\
   or: %s [-log] <inimage> <outimage> <weights>\n"));

  Iset_message(TRUE);
  Iset_abort(TRUE);

  if (argc == 1) Usage(1, NULL);
  args = argvOptions(argc, argv);

  log = read_bswitch(&argc, argv, "-log");
  wfn = read_switch(&argc, argv, "-wf", TRUE, NULL);

  if (argc < 3)  Usage(2, "Illegal number of arguments.\n");

  i1 = Iread_image(argv[1]);
  i2 = Icopy_init(i1);
  nbands = Inbands(i1);
  rb = (IR_BAND)Imake_band(Ireal_typ, nbands, nbands);

  if (wfn) {
    wf = fopen(wfn, "r");
    for (i=1; i LE nbands; i++)
      for (j=1; j LE nbands; j++)
        fscanf(wf, "%f", &rb[i][j]);
    fclose(wf);
  } else {
    if (argc LT (3+(nbands*nbands))) Usage(3, "Too few weights.\n");
    wcount = 3;
    for (i=1; i LE nbands; i++)
      for (j=1; j LE nbands; j++)
        sscanf(argv[wcount++], "%f", &rb[i][j]);
  }

  if (linearTrans(i1, i2, rb))
    Error(2, "Error in linearTransf function.\n");

  if (log) Info(0, "%s: %d overflows, %d underflows.\n", argv[0], of, uf);

  Ihistory(i2, argv[0], args);
  Iwrite_image(i2, argv[2]);

  return(0);
}
