

/*

________________________________________________________________

        haar1d
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
#include <stdlib.h>
#include <xite/includes.h>
#include <xite/blab.h>
#include <xite/biff.h>
#include <xite/haar.h>

#define SQRT2 1.4142136
#define SWAP(a,b) { float tmp=(a); a = b; b = tmp;}



/*F:haar1d=haar*/
/*F:haari1d=haar*/
/*
________________________________________________________________

		haar1d
________________________________________________________________

Name:		haar, haar1d, haari1d - One dimensional forward and
                inverse Haar transform

Syntax:		| #include <xite/haar.h>
                |
                | int haar1d(a1, a2, size)
                | float *a1, *a2;
		| int size;
		|
		| int haari1d(a1, a2, size)
                | float *a1, *a2;
		| int size;

Description:    The one-dimensional Forward and Inverse Haar
		Transform (HT).

                | a1 : input vector
		| a2 : output vector
		| size : size of a1 and a2.

References:     N. Ahmed and K.R. Rao, Orthogonal Transforms for 
		Digital Signal Processing, p. 263, Springer-Verlag, 1975. 

		H.C. Andrews and K.L. Caspari, "A generalized
		technique for spectral analysis", IEEE Trans., Comput.,
		Vol. C-19, pp. 16-25, 1970.

		N. Ahmed, T. Natarajan and K.R. Rao, "Cooley-Tukey
		type algorithm for the Haar transform", Electronics Letters,
		Vol. 9, pp. 276-278, 1973. 
 
Return value:   | 0 => ok
		| 1 => negative size
		| 2 => size not power of 2

Author:		Tor Lønnestad, BLAB, ifi, UiO
________________________________________________________________

*/


/*
________________________________________________________________

                bitr - bit reverse
________________________________________________________________
*/
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

/*
________________________________________________________________

                   haar1d
________________________________________________________________
*/

int haar1d(float *a1, float *a2, int size)
{
   int po2, x, nite, nnite, irg, k, kl, klb, k2,
   ia, mb, mbg, ms1, ms2, mtep, mtepr;
   float fn, x1, x2, sfa;

   if (size LT 2) return(1);
   po2=2; nite = 1;
   while (po2 LT size) { po2 *= 2; nite++;}
   if (po2 NE size) return(2);

   /* copy data to output vector */
   for (x=0; x LT size; x++) a2[x] = a1[x];

   /* Bit-reversal permutation of whole data */
   FOR (x=0; x LT size; x++)
     bitr(x, nite, &irg);
     IF (irg GT x) SWAP(a2[irg], a2[x]); ENDIF;
   ENDFOR;

   /* Main loop -- Butterfly type additions/subtractions */
   /* and partial bit-reversal permutation */
   fn = (float)sqrt((double)size);
   kl = size;
   nnite = nite;

   FOR (ia=1; ia LE nite; ia++)
     klb = kl;
     kl >>=1;
     FOR (k=0; k LT kl; k++)
       k2 = kl+k;
       x1 = a2[k];
       x2 = a2[k2];
       a2[k] = x1+x2;
       a2[k2] = x1-x2;
     ENDFOR;
     if (ia EQ 1) continue;
     nnite--;
     ms1 = klb;
     ms2 = (klb<<1) - 1;

     /* Partial bit-reversal permutation */
     FOR (mb=ms1; mb LE ms2; mb++)
       mtep = mb-ms1;
       bitr(mtep,nnite,&mtepr);
       IF (mtepr GT mtep)
         mbg = ms1+mtepr;
         SWAP(a2[mb], a2[mbg]);
       ENDIF;
     ENDFOR;

     /* Multiplication of Haar coefficients and normalization */
     sfa = (float)pow((double)SQRT2, (double)((float)nnite))/fn; 
     for (mb=ms1; mb LE ms2; mb++) a2[mb] *= sfa;
   ENDFOR;
   
   a2[0] /= fn;
   a2[1] /= fn;
   return(0);
}   /* end of haar1d */




/*
________________________________________________________________

		haari1d
________________________________________________________________

*/

int haari1d(float *a1, float *a2, int size)
{
   int po2, x, nite, nnite, irg, k, kl, klb, k2,
   ia, mb, mbg, ms1, ms2, mtep, mtepr;
   float fn, x1, x2, sfa;

   if (size LT 2) return(1);
   po2=2; nite = 1;
   while (po2 LT size) { po2 *= 2; nite++;}
   if (po2 NE size) return(2);

   /* copy data to output vector */
   for (x=0; x LT size; x++) a2[x] = a1[x];

   fn = (float)sqrt((double)size);
   a2[0] /= fn;
   a2[1] /= fn;
   klb = 1;
   nnite = 0;

   /* Main loop -- Butterfly type additions/subtractions */
   /* and partial bit-reversal permutation */
   FOR (ia=1; ia LE nite; ia++)
     kl = klb;
     klb =2*kl;
     FOR (k=0; k LT kl; k++)
       k2 = kl+k;
       x1 = a2[k];
       x2 = a2[k2];
       a2[k] = x1+x2;
       a2[k2] = x1-x2;
     ENDFOR;
     if (nite EQ 1) return(0);
     if (ia EQ nite) continue;
     nnite++;
     ms1 = klb;
     ms2 = (klb<<1) - 1;

     /* Multiplication of Haar coefficients and normalization */
     sfa = (float)pow((double)SQRT2, (double)((float)nnite))/fn; 
     for (mb=ms1; mb LE ms2; mb++) a2[mb] *= sfa;

     /* Partial bit-reversal permutation */
     FOR (mb=ms1; mb LE ms2; mb++)
       mtep = mb-ms1;
       bitr(mtep,nnite,&mtepr);
       IF (mtepr GT mtep)
         mbg = ms1+mtepr;
         SWAP(a2[mb], a2[mbg]);
       ENDIF;
     ENDFOR;

   ENDFOR;


   /* Bit-reversal permutation of whole data */
   FOR (x=0; x LT size; x++)
     bitr(x, nite, &irg);
     IF (irg GT x) SWAP(a2[irg], a2[x]); ENDIF;
   ENDFOR;

   return(0);
}  /* end of haari1d */
