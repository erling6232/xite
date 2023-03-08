/*C*


________________________________________________________________

        glRunLength
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
#include <xite/includes.h>
#include <xite/biff.h>
#include <stdlib.h>
#include <xite/texture.h>
#include XITE_STDIO_H
#include <xite/blab.h>
#include <xite/histo.h>
#include <xite/message.h>
#include <xite/readarg.h>



/*P:glRunLength*

________________________________________________________________

		glRunLength
________________________________________________________________

Name:		glRunLength - Grey value distribution of run lengths

Syntax:		glRunLength [<option>...] <inimage>

Description:	The run length statistics can be used to extract 
                texture information of an image from its grey level
		runs. Consecutive pixels of the same grey value, 
		in a given direction (either horizontal or vertical),
		constitute a run. The number of runs of different 
		lengths and grey values, arranged according to the 
		lengths and grey levels, form a 2-D matrix called
		grey level run length matrix.

		There are two possible directions to scan the image, either
		horizontal or vertical directions. The average of these two
		directions can also be chosen (option '-aver').

		Several features can be extracted from this matrix, using the
		options described below. If you don''t specify any options for
		features, then the program will compute all the parameters.

Options:        The first seven options are for the features which can be
                extracted from the GLRL matrix. 

		&-sre
		Short Run emphasis

		&-lre
		Long Run emphasis

		&-gln
		Grey Level Nonuniformity

		&-rln
		Run Length Nonuniformity

		&-rp
		Run Percentage

		&-sgre
		Low Grey Level Run emphasis

		&-lgre
		High Grey Level Run emphasis

      		&-h
		A header for the output is wanted.

		&-hori
		Horizontal direction 

		&-vert
		Vertical direction

		&-aver
		Consider both directions and take the average of them.
		(Default direction options is -hori.)

		&-heq
		Histogram equalization of the input image.

		&-o filename
		Run length matrix will be saved as an image in 'filename'.

		&-qlevels quantization_levels
		Number of grey levels in the input will be grouped into
		given number of quantization levels. Default is 256.

Restrictions:   'inimage' must have bands with pixel type unsigned byte.

See also:	glcm(1), glcmParameter(1)

Author:		Yogesan, BLAB, Ifi, UiO

Examples:	| glRunLength mona.img 
                | glRunLength -h mona.img
		| glRunLength -h -sre -rp -gln mona.img
		| glRunLength -h -sre -o mona.out mona.img
		| glRunLength -h -sre -o mona.out mona.img -qlevels 16
		| glRunLength -h mona.img -qlevels 32
		| glRunLength mona.img -aver
	       
Id: 		$Id$
________________________________________________________________

*/

int main(int argc, char **argv)
{
  int noofgl, nbands, bn, i, line;
  int hori, vert, head, heq, xsize, ysize, average;
  int sre, lre, gln, rln, rp, sgre, lgre, out = 0;
  int maxlength, noofruns, noofpixels;
  IMAGE inimg;
  II_IMAGE outimg;
  char *args, *outfile;

  Iset_message(TRUE);
  InitMessage(&argc, argv, xite_app_std_usage_text(
    "Usage: %s [<option>...] <inimage> [-qlevels <no_of_greylevels>]\n"));

  if (argc == 1) Usage(1, NULL);
  args = argvOptions(argc, argv);

  sre     = read_bswitch(&argc, argv, "-sre");
  lre     = read_bswitch(&argc, argv, "-lre");
  gln     = read_bswitch(&argc, argv, "-gln");
  rln     = read_bswitch(&argc, argv, "-rln");
  rp      = read_bswitch(&argc, argv, "-rp");
  sgre    = read_bswitch(&argc, argv, "-sgre");
  lgre    = read_bswitch(&argc, argv, "-lgre");
  head    = read_bswitch(&argc, argv, "-h");
  hori    = read_bswitch(&argc, argv, "-hori");
  vert    = read_bswitch(&argc, argv, "-vert");
  heq     = read_bswitch(&argc, argv, "-heq");
  average = read_bswitch(&argc, argv, "-aver");
  noofgl  = read_iswitch(&argc, argv, "-qlevels", 256);
  outfile = read_switch(&argc, argv, "-o", 1, NULL);

  if (outfile != NULL) out = 1;

  if (argc < 2) Usage(1, "Illegal number of arguments.\n");

  if (noofgl > 256)
    exit(Error(2, "Number of grey levels should be <= 256.\n"));
  if (!sre && !lre && !gln && !rln && !rp && !sgre && !lgre)
    sre = lre = gln = rln = rp = sgre = lgre = TRUE;
  if (!hori && !vert && !average) hori = 1;

  inimg = Iread_image(argv[1]);
  if (NOT inimg) exit(Error(3, "Can't find %s\n", argv[1]));

  nbands = Inbands(inimg);
  xsize  = Ixsize(inimg[1]);
  ysize  = Iysize(inimg[1]);

  if (!average) {
    if (vert) 
      outimg = (II_IMAGE) Imake_image(nbands,"glRunLength-image",
				      Integer_typ, ysize+1, noofgl+1);
    else 
      outimg = (II_IMAGE) Imake_image(nbands,"glRunLength-image",
				      Integer_typ, xsize+1, noofgl+1);
  }
  else {
    i = ysize;
    if (xsize > ysize) i = xsize;
    outimg = (II_IMAGE) Imake_image(nbands,"glRunLength-image",
				    Integer_typ, i, noofgl+1);
  }
  
  noofpixels = xsize * ysize;
  for (bn=1; bn <= nbands; bn++) {
    if (heq) 
      if (histoEq(inimg[bn], inimg[bn], noofgl))
        exit(Error(4, "histoEq failed\n"));

    if (glRunLength(inimg[bn], outimg[bn], hori, vert, average, noofgl,
		    &maxlength, &noofruns))
      Error(1, "%s%d\n", "Error in glRunLength() for band number ", bn);
  }

  if(head) {
    printf("#");
    if (nbands > 1) printf(" BANDNO ");
    line = 0;
    if (sre)  { printf("         SRE "); line++; }
    if (lre)  { printf("         LRE "); line++; }
    if (gln)  { printf("         GLN "); line++; }
    if (rln)  { printf("         RLN "); line++; }
    if (rp)   { printf("          RP "); line++; }
    if (sgre) { printf("        SGRE "); line++; }
    if (lgre) { printf("        LGRE "); line++; }
    printf("\n#");
    if (nbands > 1) printf("--------");
    for (i=1; i <= line; i++)
      printf("-------------");
    printf("\n");
  }

  for (bn=1; bn <= nbands; bn++) {
    printf(" ");
    if (nbands > 1) printf("      %d ", bn);
    if (sre)  printf("%12.4E ", glrlSre(outimg[bn],  maxlength, noofruns));
    if (lre)  printf("%12.4E ", glrlLre(outimg[bn],  maxlength, noofruns));
    if (gln)  printf("%12.4E ", glrlGln(outimg[bn],  maxlength, noofruns));
    if (rln)  printf("%12.4E ", glrlRln(outimg[bn],  maxlength, noofruns));
    if (rp)   printf("%12.4E ", glrlRp(outimg[bn],   maxlength, noofruns,
				       noofpixels));
    if (sgre) printf("%12.4E ", glrlSgre(outimg[bn], maxlength, noofruns));
    if (lgre) printf("%12.4E ", glrlLgre(outimg[bn], maxlength, noofruns));
    printf("\n");
  }

  if(out) {
    Ihistory((IMAGE) outimg, argv[0], args);
    Iwrite_image((IMAGE) outimg, outfile);
  } 

  return(0);
}
