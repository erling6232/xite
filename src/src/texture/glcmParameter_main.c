/*C*


________________________________________________________________

        glcmParameter
        $Id$
        Copyright 1991, Blab, UiO
        Image processing lab, Department of Informatics
        University of Oslo
        E-mail: blab@uio.no
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
#include XITE_STDIO_H
#include <stdlib.h>
#include <xite/blab.h>
#include <xite/arithmetic.h>
#include <xite/histo.h>
#include <xite/message.h>
#include <xite/readarg.h>
#include <xite/texture.h>
#include <xite/draw.h>
#include XITE_MALLOC_H



/*P:glcmParameter*

________________________________________________________________

		glcmParameter
________________________________________________________________

Name:		glcmParameter - Computes different features from the 
                Grey Level Co-occurrence Matrix (GLCM)

Syntax:		| glcmParameter [<option>...] <inimage>
                | or
		| glcmParameter -mkglcm [<option>...] <inimage> <dx> <dy>
		|     [<num_greylevels>]

Description:    Computes nine features from the Grey Level Cooccurence 
                Matrix of an image. Input can be a GLC-matrix (image)
                or a normal image. If it is a normal image, then the 
                program will compute the GLC-matrix before computing 
                the features. The user must specify this by giving the
		option '-mkglcm' and values for 'dx', 'dy' and
		'num_greylevels'.

		In the absence of options for what parameters to compute,
		all the parameters will be computed.

		'dx' is horizontal distance (-50 <= dx <= 50), 'dy' is
		vertical distance (-50 <= dy <= 50). Pixels less than
		'num_greylevels' will be considered. The default value
		is 256.

Options:        &-asm
                Angular Second Moment of a GLCM.

                &-cont
		Contrast of a GLCM.

		&-corr
		Correlation of a GLCM.

		&-idm
		Inverse Difference Moment of a GLCM.

		&-entrop
		Entropy of a GLCM. 

                &-var
		Variance of a GLCM.
		
		&-prom
		Cluster Prominence of a GLCM.

		&-shade
		Cluster Shade of a GLCM.

		&-diag
		Diagonal Moment of a GLCM. 

		&-h
		Print a header for the output.

		&-mkglcm
		Input is an image, a GLC-Matrix must be computed.

		&-heq
		Histogram equalization of the input image. The number of
		grey levels will be set to 'num_greylevels'.

See also:	glcm(1)

Author:		Yogesan K., BLAB, Ifi, UiO

Examples:	| glcmParameter -mkglcm mona.img 1 1 100
                | glcmParameter -mkglcm -heq mona.img 1 1 100
		| glcmParameter -mkglcm  mona.img 1 1 
		| glcmParameter -mkglcm -heq mona.img 1 1
                | glcmParameter -prom -inert -corr mona.glcm
                | glcmParameter mona.glcm
		| glcmParameter -h -prom -inert -corr mona.glcm 
		| glcmParameter -mkglcm -h -prom -inert -corr mona.img 1 1
		| glcmParameter -mkglcm -h -heq -prom -mona.img 1 1 100

Id: 		$Id$
________________________________________________________________

*/

int main(int argc, char **argv)
{
  int dx, dy, num_greylevel, nbands, bn, heq;
  int x, y, xsize, ysize;
  ID_IMAGE inimg;
  ID_IMAGE convimg;
  double no;
  int  Asm, cont, corr, idm, entropy, var, shade, prom, diag;
  int makeglcm, head, line, i;

  Iset_message(TRUE);
  InitMessage(&argc, argv, xite_app_std_usage_text(
    "Usage: %s [<option>...] <inimage> \n\
   or: %s -mkglcm [<option>...] <inimage> <dx> <dy> [<num_greylevels>]\n"));

  if (argc == 1) Usage(1, NULL);

  Asm      = read_bswitch(&argc, argv, "-asm");
  cont     = read_bswitch(&argc, argv, "-cont");
  corr     = read_bswitch(&argc, argv, "-corr");
  idm      = read_bswitch(&argc, argv, "-idm");
  entropy  = read_bswitch(&argc, argv, "-entrop");
  var      = read_bswitch(&argc, argv, "-var");
  diag     = read_bswitch(&argc, argv, "-diag");
  shade    = read_bswitch(&argc, argv, "-shade");
  prom     = read_bswitch(&argc, argv, "-prom");
  head 	   = read_bswitch(&argc, argv, "-h");
  heq      = read_bswitch(&argc, argv, "-heq");  
  makeglcm = read_bswitch(&argc, argv, "-mkglcm");

  if (makeglcm) {
    if((argc < 4) || (argc > 5)) Usage(1, "Illegal number of arguments.\n");

    if (abs(dx = atoi(argv[2])) > 50 || abs(dy = atoi(argv[3])) > 50)
      Usage(2, "Must have -50 <= dx, dy <= 50.\n");

    num_greylevel = (argc == 4) ? 256 : atoi(argv[4]);

    if(num_greylevel > 256)
      Usage(3, "Number of grey levels should be <= 256.\n");

  } else if(argc != 2) Usage(1, "Illegal number of arguments.\n");

  if (!Asm && !cont && !corr && !idm && !entropy && !var && !diag &&
      !shade && !prom)
    Asm = cont = corr = idm = entropy = var = diag = shade = prom = TRUE;

  inimg = (ID_IMAGE)Iread_image(argv[1]);
  if (NOT inimg) Usage(4, "Can't find %s\n", argv[1]);

  nbands = Inbands((IMAGE)inimg);
  if (makeglcm) xsize = ysize = num_greylevel;
  else {
    xsize = Ixsize((IBAND)inimg[1]);
    ysize = Iysize((IBAND)inimg[1]);
  }

  convimg = (ID_IMAGE) Imake_image(nbands, "glcm-image",
				   Idouble_typ, xsize, ysize);  

  for (bn=1; bn <= nbands; bn++) {
    if (makeglcm) {
      if (heq) 
	if (histoEq((IBAND)inimg[bn], (IBAND)inimg[bn], num_greylevel))
	  Error(2, "histoEq error.\n");

      if (drawBand((IBAND) convimg[bn], 0.0, 0.0))
	Error(3, "drawBand() error.\n");;
      if (glcm((IBAND)inimg[bn], convimg[bn], dx, dy, num_greylevel) > 1)
	Error(4, "glcm() error.\n");
    }
    else {
      for (y=1; y<=ysize; y++)
	for (x=1; x<=xsize; x++)
	  convimg[bn][y][x] = inimg[bn][y][x];
    }
    
    no = 0.0;
    for (y=1; y<=ysize; y++)
      for (x=1; x<=xsize; x++)
	no += convimg[bn][y][x];

    if (no &&
	scale((IBAND) convimg[bn], (IBAND) convimg[bn], (1.0/no), 0.0) != 0)
      Error(5, "scale() error.\n");
  }
  
  line = 0;
  if(head) {
    printf("\n#");
    if (nbands > 1) fprintf(stdout,"BANDNO ");
    if (Asm)     { printf("     ASM "); line++; }
    if (cont)    { printf("        CONT "); line++; }
    if (corr)    { printf("          CORR "); line++; }
    if (idm)     { printf("          IDM "); line++; }
    if (entropy) { printf("          ENTR "); line++; }
    if (var)     { printf("         VAR "); line++; }
    if (diag)    { printf("        DIAGM "); line++; }
    if (shade)   { printf("         SHADE "); line++; }
    if (prom)    { printf("         PROM "); line++; }
    printf("\n#");
    if (nbands > 1) printf("--------");
    for (i=1; i <= line; i++) printf("-------------");
    printf("\n");
  }
  
  for (bn=1; bn <= nbands; bn++) {
    if (nbands > 1) fprintf(stdout,"%d ", bn);
    if (Asm)     fprintf(stdout,"   %.4E ", glcmAsm(convimg[bn]));
    if (cont)    fprintf(stdout,"   %.4E ", glcmCont(convimg[bn]));
    if (corr)    fprintf(stdout,"   %.4E ", glcmCorr(convimg[bn])); 
    if (idm)     fprintf(stdout,"   %.4E ", glcmIdm(convimg[bn]));
    if (entropy) fprintf(stdout,"   %.4E ", glcmEntropy(convimg[bn]));
    if (var)     fprintf(stdout,"   %.4E ", glcmVar(convimg[bn]));
    if (diag)    fprintf(stdout,"   %.4E ", glcmDiagmoment(convimg[bn]));
    if (shade)   fprintf(stdout,"   %.4E ", glcmShade(convimg[bn]));
    if (prom)    fprintf(stdout,"   %.4E ", glcmProm(convimg[bn]));
    printf("\n");
  }
  
  return(0);
}
