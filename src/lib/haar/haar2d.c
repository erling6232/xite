

/*

________________________________________________________________

        haar2d
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

#include <math.h>
#include <xite/includes.h>
#include <xite/biff.h>
#include <stdlib.h>
#include <xite/haar.h>
#include XITE_STDIO_H
#include <xite/blab.h>
#include <xite/convert.h>
#include <xite/readarg.h>
#include <xite/message.h>

#define SQRT2 1.41421356237309504880
#define SWAP(a,b) { float tmp=(a); a = b; b = tmp;}

#ifndef MIN
# define MIN(a,b) (((a)<(b)) ? (a) : (b))
#endif

#ifndef MAIN

/*________________________________________________________________
*/

/* bit reverse */
static void bitr(int in, int nbits, int *out)
{
   int lout=0;
   WHILE (nbits--)
     lout <<=1;
     if (in BAND 1) lout++;
     in >>=1;
   ENDWHILE;
   *out = lout;
}



/*F:haar2d=haar*/
/*F:haari2d=haar*/
/*F:haar*

________________________________________________________________

		haar2d
________________________________________________________________

Name:		haar, haar2d, haari2d - Two-dimensional Haar transform 

Syntax:		| #include <xite/haar.h>
                |
                | int haar2d(b1, b2)
                | IR_BAND b1, b2;
		|
		| int haari2d(b1, b2)
                | IR_BAND b1, b2;

Description:    The two-dimensional Forward Haar transform, and
                the two-dimensional Inverse Haar transform. 
		Transforms input band <b1> to output band <b2>.
		Identical actual arguments are allowed.

References:	N. Ahmed, T. Natarajan and K.R. Rao, "Cooley-Tukey
		type algorithm for the Haar transform", Electronics
		Letters, Vol. 9, pp. 276-278, 1973.

		N. Ahmed and K.R. Rao, Orthogonal Transforms for 
		Digital Signal Processing, p. 263, Springer-Verlag, 
		1975.

Restrictions:   Bands should have overlapping size some power of two.
                Both bands should have pixel type Ireal_typ.

Return value:   | 0 => ok
                | 1 => bad pixel type band 1
                | 2 => bad pixel type band 2
                | 3 => bad size (not power of two)

Author:		Tor Loennestad, BLAB, ifi, UiO
________________________________________________________________

*/

int haar2d(IR_BAND b1, IR_BAND b2)
{
   int xsize, ysize, x, y, bitrev, xite, yite, nnite, g, po2,
       kl, klb, k1, k2, ia, ms1, ms2, mb, mtep, mtepr, mbg, l,
       ii, ibias, ib;
   float x1, x2, sfn, ha;

   if (Ipixtyp((IBAND) b1) NE Ireal_typ) return(1);
   if (Ipixtyp((IBAND) b2) NE Ireal_typ) return(2);

   xsize = MIN(Ixsize((IBAND) b1), Ixsize((IBAND) b2));
   ysize = MIN(Iysize((IBAND) b1), Iysize((IBAND) b2));
   po2=2; xite = 1;
   while (po2 LT xsize) { po2 *= 2; xite++;}
   if (po2 NE xsize) return(3);
   po2=2; yite = 1;
   while (po2 LT ysize) { po2 *= 2; yite++;}
   if (po2 NE ysize) return(3);

   if (b1 NE b2) Icopy_band((IBAND) b1, (IBAND) b2);

   /*  Bit-reversal permutation for whole data  */
   FOR (y=1; y LE ysize; y++)
     bitr(y-1, yite, &bitrev);
     IF (bitrev GE y)
       g = bitrev+1;
       for (x=1; x LE xsize; x++) SWAP(b2[y][x], b1[g][x]);
     ENDIF;
   ENDFOR;
   FOR (x=1; x LE xsize; x++)
     bitr(x-1, xite, &bitrev);
     IF (bitrev GE x)
       g = bitrev+1;
       for (y=1; y LE ysize; y++) SWAP(b2[y][x], b2[y][g]);
     ENDIF;
   ENDFOR;

   /* Butterfly type additions/subtractions. */
   /* Bit-reversal permutation. */
   /*  -- For rows -- */
   kl = xsize;
   nnite = xite;
   FOR (ia=1; ia LE xite; ia++)
     klb = kl;
     kl >>=1;
     FOR (k1=1; k1 LE kl; k1++)
       k2 = kl + k1;
       FOR (y=1; y LE ysize; y++)
         x1 = b2[y][k1];
         x2 = b2[y][k2];
         b2[y][k1] = x1 + x2;
         b2[y][k2] = x1 - x2;
       ENDFOR;
     ENDFOR;

     if (ia EQ 1) continue;
     nnite--;
     ms1 = klb+1;
     ms2 = 2*klb;
     FOR (mb= ms1; mb LE ms2; mb++)
       mtep = mb-ms1;
       bitr(mtep, nnite, &mtepr);
       IF (mtepr GT mtep)
         mbg = ms1 + mtepr;
         for (y=1; y LE ysize; y++) SWAP(b2[y][mb], b2[y][mbg]);
       ENDIF;
     ENDFOR;
   ENDFOR; /* ia=1.. */

   /*  -- For columns -- */
   kl = ysize;
   nnite = yite;
   FOR (ia=1; ia LE yite; ia++)
     klb = kl;
     kl >>= 1;
     FOR (k1=1; k1 LE kl; k1++)
       k2 = kl+k1;
       FOR (x=1; x LE xsize; x++)
         x1 = b2[k1][x];
         x2 = b2[k2][x];
         b2[k1][x] = x1 + x2;
         b2[k2][x] = x1 - x2;
       ENDFOR;
     ENDFOR;

     if (ia EQ 1) continue;
     nnite--;
     ms1 = klb+1;
     ms2 = 2*klb;
     FOR (mb=ms1; mb LE ms2; mb++)
       mtep = mb-ms1;
       bitr(mtep, nnite, &mtepr);
       IF (mtepr GT mtep)
         mbg = ms1+mtepr;
         for (x=1; x LE xsize; x++) SWAP(b2[mb][x], b2[mbg][x]);
       ENDIF;
     ENDFOR;
   ENDFOR; /* ia=1.. */

   /* Multiplication of Haar coefficients and normalization */
   sfn = (float)sqrt(sqrt((double)(xsize*ysize)));
   FOR (y=1; y LE ysize; y++)
     b2[y][1] /= sfn;
     b2[y][2] /= sfn;
   ENDFOR;
   FOR (x=1; x LE xsize; x++)
     b2[1][x] /= sfn;
     b2[2][x] /= sfn;
   ENDFOR;

   /* -- For rows -- */
   IF (xite NE 1)
     ibias = 1;
     ii = 1;
     ha = 1.0/sfn;
     FOR (ia=2; ia LE xite; ia++)
       ibias += ii;
       ha *= SQRT2;
       ii *= 2;
       FOR (l=1; l LE ii; l++)
         ib = ibias+l;
         for (y=1; y LE ysize; y++) b2[y][ib] *= ha;
       ENDFOR;
     ENDFOR;
   ENDIF;

   /* -- For columns -- */
   IF (yite NE 1)
     ibias = 1;
     ii = 1;
     ha = 1.0/sfn;
     FOR (ia=2; ia LE yite; ia++)
       ibias += ii;
       ha *= SQRT2;
       ii *= 2;
       FOR (l=1; l LE ii; l++)
         ib = ibias+l;
         for (x=1; x LE xsize; x++) b2[ib][x] *= ha;
       ENDFOR;
     ENDFOR;
   ENDIF;
   return(0);
} /* end of haar2d */




int haari2d(IR_BAND b1, IR_BAND b2)
{
   int xsize, ysize, x, y, bitrev, xite, yite, nnite, po2,
       kl, klb, k1, k2, ia, ms1, ms2, mb, mtep, mtepr, mbg, l,
       ii, ibias, ib;
   float x1, x2, sfn, ha;

   if (Ipixtyp((IBAND) b1) NE Ireal_typ) return(1);
   if (Ipixtyp((IBAND) b2) NE Ireal_typ) return(2);

   xsize = MIN(Ixsize((IBAND) b1), Ixsize((IBAND) b2));
   ysize = MIN(Iysize((IBAND) b1), Iysize((IBAND) b2));
   po2=2; xite = 1;
   while (po2 LT xsize) { po2 *= 2; xite++;}
   if (po2 NE xsize) return(3);
   po2=2; yite = 1;
   while (po2 LT ysize) { po2 *= 2; yite++;}
   if (po2 NE ysize) return(3);

   if (b1 NE b2) Icopy_band((IBAND) b1, (IBAND) b2);

   /* Multiplication of Haar coefficients and normalization */
   sfn = (float)sqrt(sqrt((double)(xsize*ysize)));
   FOR (y=1; y LE ysize; y++)
     b2[y][1] /= sfn;
     b2[y][2] /= sfn;
   ENDFOR;
   FOR (x=1; x LE xsize; x++)
     b2[1][x] /= sfn;
     b2[2][x] /= sfn;
   ENDFOR;

   /* -- For rows -- */
   IF (xite NE 1)
     ibias = 1;
     ii = 1;
     ha = 1.0/sfn;
     FOR (ia=2; ia LE xite; ia++)
       ibias += ii;
       ha *= SQRT2;
       ii *= 2;
       FOR (l=1; l LE ii; l++)
         ib = ibias+l;
         for (y=1; y LE ysize; y++) b2[y][ib] *= ha;
       ENDFOR;
     ENDFOR;
   ENDIF;

   /* -- For columns -- */
   IF (yite NE 1)
     ibias = 1;
     ii = 1;
     ha = 1.0/sfn;
     FOR (ia=2; ia LE yite; ia++)
       ibias += ii;
       ha *= SQRT2;
       ii *= 2;
       FOR (l=1; l LE ii; l++)
         ib = ibias+l;
         for (x=1; x LE xsize; x++) b2[ib][x] *= ha;
       ENDFOR;
     ENDFOR;
   ENDIF;

   /* Butterfly type additions/subtractions. */
   /* Bit-reversal permutation. */
   /*  -- For rows -- */
   klb = 1;
   nnite = 0;
   FOR (ia=1; ia LE xite; ia++)
     kl = klb;
     klb = 2*kl;
     FOR (k1=1; k1 LE kl; k1++)
       k2 = k1 + kl;
       FOR (y=1; y LE ysize; y++)
         x1 = b2[y][k1];
         x2 = b2[y][k2];
         b2[y][k1] = x1 + x2;
         b2[y][k2] = x1 - x2;
       ENDFOR;
     ENDFOR;

     if (xite EQ 1) continue;
     if (ia EQ xite) continue;
     nnite++;
     ms1 = klb+1;
     ms2 = 2*klb;
     FOR (mb= ms1; mb LE ms2; mb++)
       mtep = mb-ms1;
       bitr(mtep, nnite, &mtepr);
       IF (mtepr GT mtep)
         mbg = ms1 + mtepr;
         for (y=1; y LE ysize; y++) SWAP(b2[y][mb], b2[y][mbg]);
       ENDIF;
     ENDFOR;
   ENDFOR; /* ia=1.. */

   /*  -- For columns -- */
   klb = 1;
   nnite = 0;
   FOR (ia=1; ia LE yite; ia++)
     kl = klb;
     klb = 2*kl;
     FOR (k1=1; k1 LE kl; k1++)
       k2 = k1+kl;
       FOR (x=1; x LE xsize; x++)
         x1 = b2[k1][x];
         x2 = b2[k2][x];
         b2[k1][x] = x1 + x2;
         b2[k2][x] = x1 - x2;
       ENDFOR;
     ENDFOR;

     if (yite EQ 1) continue;
     if (ia EQ yite) continue;
     nnite++;
     ms1 = klb+1;
     ms2 = 2*klb;
     FOR (mb=ms1; mb LE ms2; mb++)
       mtep = mb-ms1;
       bitr(mtep, nnite, &mtepr);
       IF (mtepr GT mtep)
         mbg = ms1+mtepr;
         for (x=1; x LE xsize; x++) SWAP(b2[mb][x], b2[mbg][x]);
       ENDIF;
     ENDFOR;
   ENDFOR; /* ia=1.. */

   /*  Bit-reversal permutation for whole data */
   FOR (y=1; y LE ysize; y++)
     bitr(y-1, yite, &bitrev);
     IF (bitrev GE y)
       bitrev++;
       for (x=1; x LE xsize; x++) SWAP(b2[bitrev][x], b2[y][x]);
     ENDIF;
   ENDFOR;
   FOR (x=1; x LE xsize; x++)
     bitr(x-1, xite, &bitrev);
     IF (bitrev GE x)
       bitrev++;
       for (y=1; y LE ysize; y++) SWAP(b2[y][x], b2[y][bitrev]);
     ENDIF;
   ENDFOR;
   return(0);
} /* end of haari2d */

#endif /* not MAIN */

/*P:haar2d*

________________________________________________________________

		haar2d
________________________________________________________________

Name:		haar2d - Two dimensional forward or inverse Haar transform

Syntax:		haar2d [-i] [-b] [-r] <inimage> <outimage> [<outimage2>]

Description:    haar2d performs the two dimesional forward or inverse
                haar transform, using all bands of <inimage> as input,
		writing the output to <outimage>. The input image may
		have any pixel type, the bands will be converted to
		REAL (float) before processing.

		Switces:
		| -i : Perform inverse transform (Default: forward)
		| -b : Convert all output bands to byte.
		| -r : Convert all output bands to real. 

		By default, output bands are of type real when doing
		a forward transform, byte when inverse transform.
		If you give BOTH -b and -r, you should also give
		<outimage2>. The byte version will then be written to
		<outimage> and the real version to <outimage2>.
		
Files:	
See also:	
Diagnostics:

Restrictions:   Horizontal and vertical sizes must be some power of two.

Author:		Tor Loennestad, BLAB, ifi, UiO

Examples:       | haar2d mona.img monaHaar.img
		| haar2d -b mona.img monaHaar.img; xshow monaHaar.img
		| haar2d -b -r mona.img monaHaarB.img monaHaarR.img
		| haar2d -i monaHaar.img monaHaari.img

Id: 		$Id$
________________________________________________________________

*/

#ifdef MAIN

int main(int argc, char **argv)
{
  int i, b, r, bn, status;
  IMAGE img;
  IR_IMAGE ir;

  Iset_message(TRUE);
  Iset_abort(TRUE);
  InitMessage(&argc, argv, xite_app_std_usage_text(
    "Usage: %s  [-i] [-b] [-r] <inimage> <outimage> [<outimage2>]\n"));

  if (argc == 1) Usage(1, NULL);

  /* read and test input */
  i = (read_switch(&argc,argv,"-i",FALSE,(char*)0) NE (char*)0);
  b = (read_switch(&argc,argv,"-b",FALSE,(char*)0) NE (char*)0);
  r = (read_switch(&argc,argv,"-r",FALSE,(char*)0) NE (char*)0);

  if (argc < 3) Usage(1, "Illegal number of arguments.\n");

  img = Iread_image(argv[1]);
  ir = (IR_IMAGE) Init_image(Inbands(img), Ititle(img));

  /* perform transformation */
  FOR (bn=1; bn LE Inbands(img); bn++)
    if (Ipixtyp(img[bn]) EQ Ireal_typ) ir[bn] = (IR_BAND)img[bn];
    else ir[bn] = (IR_BAND)mkConvertBand(img[bn], Ireal_typ);

    if (i) status = haari2d(ir[bn], ir[bn]);
    else status = haar2d(ir[bn], ir[bn]);
    if (status) Error(2, "Error while processing band %d\n", bn);
  ENDFOR;

  /* write output */
  if ((NOT b) AND (NOT r)) if (i) b = TRUE; else r = TRUE;
  Icopy_text(img, (IMAGE) ir);
  if (i) Ihistory((IMAGE) ir, argv[0], " Inverse transform");
    else  Ihistory((IMAGE) ir, argv[0], " Forward transform");
  IF (r)
    IF (b)
      if (argc LT 4)
        Usage(1, "No file name for real image.\n");
      else Iwrite_image((IMAGE) ir, argv[3]);
    ELSE
      Iwrite_image((IMAGE) ir, argv[2]);
    ENDIF;
  ENDIF;

  IF (b)
    for (bn=1; bn LE Inbands(img); bn++)
      ir[bn] = (IR_BAND)mkConvertBand((IBAND) ir[bn], Iu_byte_typ);
    Iwrite_image((IMAGE) ir, argv[2]);
  ENDIF;
  return(0);
}

#endif /* MAIN */
