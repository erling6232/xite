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



#ifndef MAIN

/*F:glcmAsm=glcmParameter*/
/*F:glcmCont=glcmParameter*/
/*F:glcmCorr=glcmParameter*/
/*F:glcmIdm=glcmParameter*/
/*F:glcmEntropy=glcmParameter*/
/*F:glcmVar=glcmParameter*/
/*F:glcmProm=glcmParameter*/
/*F:glcmShade=glcmParameter*/
/*F:glcmInertia=glcmParameter*/
/*F:glcmDiagmoment=glcmParameter*/
/*F:glcmParameter*

________________________________________________________________

		glcmParameter
________________________________________________________________

Name:		glcmParameter, glcmAsm, glcmCont, glcmCorr, glcmIdm,
                glcmEntropy, glcmVar, glcmProm, glcmShade, glcmInertia,
                glcmDiagmoment - Computes features from the Grey Level
                Co-occurrence Matrix (GLCM)

Syntax:         | #include <xite/texture.h>
		|
                | double glcmAsm( ID_BAND inband );
		|
                | double glcmCont( ID_BAND inband );
		|
                | double glcmCorr( ID_BAND inband );
		|
                | double glcmIdm( ID_BAND inband );
		|
                | double glcmEntropy( ID_BAND inband );
		|
                | double glcmVar( ID_BAND inband );
		|
                | double glcmProm( ID_BAND inband );
		|
                | double glcmShade( ID_BAND inband );
		|
                | double glcmInertia( ID_BAND inband );
		|
                | double glcmDiagmoment( ID_BAND inband );

Description:    The input parameters are normalized bands.

                &glcmAsm
		Calculates Angular Second Moment of a GLCM. See reference 1.

                &glcmCont
		Calculates Contrast of a GLCM. See reference 3.

		&glcmCorr
		Calculates Correlation of a GLCM. See reference 1.

		&glcmIdm
		Calculates Inverse Difference Moment of a GLCM. See
		reference 1.

		&glcmEntropy
		Calculates Entropy of a GLCM. See reference 1.

                &glcmVar
		Calculates Variance of a GLCM. See reference 1.

		&glcmProm
		Calculates Cluster Prominence of a GLCM. See reference 2.

		&glcmShade
		Calculates Cluster Shade of a GLCM. See reference 2.

		&glcmInertia
		Calculates Inertia of a GLCM. See reference 2.

                &glcmDiagmoment
		Calculates Diagonal moment of a GLCM.

Restrictions:   'inband' must have pixel type double.

See also:       glcm(3)

Reference: 	&R. M. Haralick et al.
                "Textural Features for Image Classification",
                IEEE Trans. on Systems, Man and Cybernetics,
                Vol. SMC-3, pp. 610-621, 1973.

                &R. W. Conners et al.
		"Segmentation of a High-Resolution Urban Scene Using 
                Texture Operators",
		Computer Vision, Graphics and Image Processing,
		Vol. 25, pp. 273-310, 1984.

		&C. C. Gotlieb et al.
		"Texture Descriptors Based on Co-ooccurrence Matrices",
		Computer Vision, Graphics and Image Processing,
		Vol. 51, pp. 70-86, 1990.

Return value:	GLCM-Parameter value (double)

Author:		Yogesan, BLAB, Ifi, UiO

Id: 		$Id$
________________________________________________________________

*/

#ifndef FUNCPROTO
double glcmAsm(inband)
ID_BAND inband;
#else /* FUNCPROTO */
double glcmAsm(ID_BAND inband)
#endif /* FUNCPROTO */
{
  int x, y, xsize, ysize;
  double Asm = 0.0;

  if (Ipixtyp((IBAND) inband) != Idouble_typ)
    return(Error(-1, "Input band must have pixel type double.\n"));

  xsize = Ixsize((IBAND) inband);
  ysize = Iysize((IBAND) inband);

  for (y=1; y<=ysize; y++)
    for (x=1; x<=xsize; x++)
      Asm += (inband[y][x] * inband[y][x]);

  return(Asm);
}


#ifndef FUNCPROTO
double glcmCont(inband)
ID_BAND inband;
#else /* FUNCPROTO */
double glcmCont(ID_BAND inband)
#endif /* FUNCPROTO */
{
  int x, y, xsize, ysize, n;
  double cont=0.0, *sum;

  if (Ipixtyp((IBAND) inband) != Idouble_typ)
    return(Error(-1, "Input band must have pixel type double.\n"));

  xsize = Ixsize((IBAND) inband);
  ysize = Iysize((IBAND) inband);
  sum = (double*)calloc(xsize, sizeof(double));
 
  for (n=0; n<xsize; n++) sum[n] = 0.0;

  for (y=1 ; y <= ysize;y++)
    for (x=1; x <= xsize;x++)
      sum[abs(y-x)] += inband[y][x]; 

  for (n=0; n<xsize; n++) cont += n * n * sum[n];

  free(sum);

  return(cont);
}

#ifndef FUNCPROTO
double glcmCorr(inband)
ID_BAND inband;
#else /* FUNCPROTO */
double glcmCorr(ID_BAND inband)
#endif /* FUNCPROTO */
{
  int x, y, xsize, ysize;
  double corr = 0.0, mux, muy, muxx, muyy, sigmax, sigmay;

  if (Ipixtyp((IBAND) inband) != Idouble_typ)
    return(Error(-1, "Input band must have pixel type double.\n"));

  xsize = Ixsize((IBAND) inband);
  ysize = Iysize((IBAND) inband);

  muy =  mux = 0.0;
  for (y=1 ; y <= ysize; y++)
    { 
      muyy = muxx = 0.0;
      for (x=1; x <= xsize; x++)
	{
	  muyy += inband[y][x];
	  muxx += inband[x][y];
	}
      muy += muyy * (y-1);
      mux += muxx * (y-1);
    }

  sigmay  = sigmax = 0.0;
  for (y=1; y <= ysize; y++)
    { 
      muxx = muyy = 0.0;
      for (x=1; x <= xsize; x++) 
	{
	  muyy += inband[y][x];
          muxx += inband[x][y];
         }
      sigmay += (y - 1 - muy) * (y - 1 - muy) * muyy;
      sigmax += (y - 1 - mux) * (y - 1 - mux) * muxx;
    }

  for (y= 1; y <= ysize; y++)
    for (x=1; x <= xsize; x++)
      corr += ((x-1)*(y-1))*inband[y][x] ;

  sigmax=pow(sigmax,0.5);
  sigmay=pow(sigmay,0.5);
  corr  = ( corr - (mux*muy) ) / (sigmax * sigmay);


  return(corr);
}

#ifndef FUNCPROTO
double glcmIdm(inband)
ID_BAND inband;
#else /* FUNCPROTO */
double glcmIdm(ID_BAND inband)
#endif /* FUNCPROTO */
{
  int x, y, xsize, ysize;
  double idm = 0.0;

  if (Ipixtyp((IBAND) inband) != Idouble_typ)
    return(Error(-1, "Input band must have pixel type double.\n"));

  xsize = Ixsize((IBAND) inband);
  ysize = Iysize((IBAND) inband);

  for (y= 1; y <= ysize; y++)
    for (x=1; x <= xsize; x++) 
      idm += inband[y][x] / (1 + pow((double)(y - x),2.0)) ;
  
  return(idm);
}

#ifndef FUNCPROTO
double glcmEntropy(inband)
ID_BAND inband;
#else /* FUNCPROTO */
double glcmEntropy(ID_BAND inband)
#endif /* FUNCPROTO */
{
  int x, y, xsize, ysize;
  double entropy = 0.0;

  if (Ipixtyp((IBAND) inband) != Idouble_typ)
    return(Error(-1, "Input band must have pixel type double.\n"));

  xsize = Ixsize((IBAND) inband);
  ysize = Iysize((IBAND) inband);

  for (y=1; y <=ysize; y++)
    for (x=1; x <= xsize; x++) 
      if (inband[y][x] != 0.0)
        entropy += inband[y][x] * ( log10(inband[y][x]) / log10(2.0) );

  entropy = -1.0 * entropy;

  return(entropy);
}

#ifndef FUNCPROTO
double glcmVar(inband)
ID_BAND inband;
#else /* FUNCPROTO */
double glcmVar(ID_BAND inband)
#endif /* FUNCPROTO */
{
    int x, y, xsize, ysize;
    double var = 0.0, mean = 0.0;
    double mux=0.0, muy=0.0, muyy = 0.0;

    if (Ipixtyp((IBAND) inband) != Idouble_typ)
    return(Error(-1, "Input band must have pixel type double.\n"));

    xsize = Ixsize((IBAND) inband);
    ysize = Iysize((IBAND) inband);

    for (y=1; y <= ysize; y++)
      for (x=1; x <= xsize; x++) 
        mean += ( (double)(x-1) * (double)(y-1) * inband[y][x]);
    
    for (y=1; y<=ysize; y++)
      for (x=1; x<=xsize; x++)
	 var += ((double)(y-1) - mean) * ((double)(y-1) - mean) * inband[y][x];

    return(var);
}


#ifndef FUNCPROTO
double glcmShade(inband)
ID_BAND inband;
#else /* FUNCPROTO */
double glcmShade(ID_BAND inband)
#endif /* FUNCPROTO */
{
  int x, y, xsize, ysize;
  double shade = 0.0, mux = 0.0, muy = 0.0;
  double muxy = 0.0, muyy = 0.0, muxx = 0.0;

  if (Ipixtyp((IBAND) inband) != Idouble_typ)
    return(Error(-1, "Input band must have pixel type double.\n"));

  xsize = Ixsize((IBAND) inband);
  ysize = Iysize((IBAND) inband);

  for (y=1 ; y <= ysize; y++)
    {
      muyy = muxx = 0.0;
      for (x=1; x <= xsize; x++)
	{
	  muyy += inband[y][x];
	  muxx += inband[x][y];
	}
      muy += muyy * (y-1);
      mux += muxx * (y-1);
    }

  muxy = mux + muy;
  for (y=1; y <= ysize; y++)
    for (x=1; x <= xsize; x++) 
      shade += inband[y][x] * pow(((double)x + (double)y - 2.0 - muxy),3.0);
      
  return(shade);
}

#ifndef FUNCPROTO
double glcmProm(inband)
ID_BAND inband;
#else /* FUNCPROTO */
double glcmProm(ID_BAND inband)
#endif /* FUNCPROTO */
{
  int x, y, xsize, ysize;
  double prom = 0.0, mux = 0.0, muy = 0.0;
  double muxy = 0.0, muyy = 0.0;

  if (Ipixtyp((IBAND) inband) != Idouble_typ)
    return(Error(-1, "Input band must have pixel type double.\n"));

  xsize = Ixsize((IBAND) inband);
  ysize = Iysize((IBAND) inband);

  for (y=1; y <= ysize; y++)
    {
      muyy = 0.0;
      for (x=1; x <= xsize; x++) 
	{
	  muyy += inband[y][x];
	  mux += (double)(x-1) * inband[y][x]; 
	}
      muy += (double)(y-1) * muyy; 
    }

  muxy = mux + muy;
  for (y=1; y <= ysize; y++)
    for (x=1; x <= xsize; x++) 
      prom += inband[y][x] * pow(((double)x + (double)y - 2.0 - muxy),4.0);
      
  return(prom);
}

#ifndef FUNCPROTO
double glcmInertia(inband)
ID_BAND inband;
#else /* FUNCPROTO */
double glcmInertia(ID_BAND inband)
#endif /* FUNCPROTO */
{
  int x, y, xsize, ysize;
  double inert = 0.0;

  if (Ipixtyp((IBAND) inband) != Idouble_typ)
    return(Error(-1, "Input band must have pixel type double.\n"));

  xsize = Ixsize((IBAND) inband);
  ysize = Iysize((IBAND) inband);

  for (y=1; y <= ysize; y++)
    for (x=1; x <= xsize; x++) 
      inert += inband[y][x] * ((y-x) * (y-x));
 
  return(inert);
}



#ifndef FUNCPROTO
double glcmDiagmoment(inband)
ID_BAND inband;
#else /* FUNCPROTO */
double glcmDiagmoment(ID_BAND inband)
#endif /* FUNCPROTO */
{	
   int x, y, xsize, ysize;
   double diagm = 0.0, mean= 0.0;
   double muy=0.0, mux=0.0, muyy=0.0;

   if (Ipixtyp((IBAND) inband) != Idouble_typ)
    return(Error(-1, "Input band must have pixel type double.\n"));

   xsize = Ixsize((IBAND) inband);
   ysize = Iysize((IBAND) inband);
    
   for (y=1; y <= ysize; y++)
     {
       muyy = 0.0;
       for (x=1; x <= xsize; x++)
         {
	   muyy += inband[y][x];
	   mux += (double)(x-1) * inband[y][x];
         }
       muy += (double)(y-1) * muyy;
     }
    
   /* mean (below) is a better approximation for real mean of the
      original image */
   /* mean = (mux + muy) / 2; */
    
   diagm = 0.0;
   for (y=1; y <= ysize; y++)
     for (x=1; x <= xsize; x++)
       diagm  += (double) (abs(y-x)*( (y-1) + (x-1) - mux - muy )*inband[y][x]);
    
   return(diagm);
}

#endif /* not MAIN */

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

#ifdef MAIN

#ifndef FUNCPROTO
int main(argc, argv)
int argc;
char *argv[];
#else /* FUNCPROTO */
int main(int argc, char **argv)
#endif /* FUNCPROTO */
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

#endif /* MAIN */
