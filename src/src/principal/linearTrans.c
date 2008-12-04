
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

#ifndef MAIN

#ifndef MIN
# define MIN(a,b) (((a) < (b)) ? (a) : (b))
#endif



/*F:linearTrans*

________________________________________________________________

		linearTrans
________________________________________________________________

Name:		linearTrans - Linear image to image transformation

Syntax:         | #include <xite/principal.h>
		|
                | BiffStatus linearTrans( IMAGE input,
                |    IMAGE output, IR_BAND weights );

Description:    'linearTrans' performs a linear transformation from
                pixels in the input image to pixels in the output image,
		| Pout = W*Pin
		where W is the weight matrix 'weights', which should
		be of size nbands*nbands, where nbands is the number
		of bands in input and output images.

Restrictions:   All bands in 'input' and 'output' must have pixel type
		unsigned byte.

Return value:   0 => ok
		1 => bad pixel type in input image
		2 => bad pixel type in output image

Author:		Tor Lønnestad, BLAB, Ifi, UiO

Id:             $Id$
________________________________________________________________

*/


BiffStatus linearTrans(IMAGE input, IMAGE output, IR_BAND weights)
{
  int nbands, x, y, bn1, bn2, pv, ival, xsize, ysize;
  float **table, val;

  nbands = MIN(Inbands(input), Inbands(output));

  for (bn1=1; bn1 LE nbands; bn1++)
    if (Ipixtyp(input[bn1]) NE Iu_byte_typ)
      return(Error(1, "Input pixel type must be unsigned byte.\n"));

  for (bn2=1; bn2 LE nbands; bn2++)
    if (Ipixtyp(output[bn2]) NE Iu_byte_typ)
      return(Error(1, "Output pixel type must be unsigned byte.\n"));

  xsize = Ixsize(input[1]);
  ysize = Iysize(input[1]);

  for (bn1=2; bn1 LE nbands; bn1++) {
    if (Ixsize(input[bn1])  LT xsize) xsize = Ixsize(input[bn1]);
    if (Iysize(input[bn1])  LT ysize) ysize = Iysize(input[bn1]);
    if (Ixsize(output[bn1]) LT xsize) xsize = Ixsize(output[bn1]);
    if (Iysize(output[bn1]) LT ysize) ysize = Iysize(output[bn1]);
  }

  /* build table */
  table = (float**) malloc((nbands+1)*sizeof(float*));
  table[1] = (float*) malloc(256*nbands*sizeof(float));
  for (bn1=2; bn1 LE nbands; bn1++)
    table[bn1] = (float*)(table[bn1-1] + 256);

  /* for every output band... */
  for (bn2=1; bn2 LE nbands; bn2++) {
    /* init table */
    for (bn1=1; bn1 LE nbands; bn1++)
      for (pv=0; pv LE 255; pv++)
        table[bn1][pv] = weights[bn2][bn1] * (float)pv;

    /* perform transformation */
    for (y=1; y LE ysize; y++) {
      for (x=1; x LE xsize; x++) {
        val = 0.0;
        for (bn1=1; bn1 LE nbands; bn1++) 
          val += table[bn1][input[bn1][y][x]];
        ival = (int)(val+0.5);
        if (ival GT 255) {of++; output[bn2][y][x] = 255;}
        else if (ival LT 0) {uf++; output[bn2][y][x] = 0;}
        else output[bn2][y][x] = (UNS_BYTE)ival;
      }
    }
  }

  return(0);
}

#endif /* not MAIN */

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

#ifdef MAIN

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

#endif /* MAIN */
