
/*

________________________________________________________________

        pseudomedian3x3
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
#include <xite/blab.h>
#include <xite/biff.h>
#include <xite/median.h>
#include <xite/message.h>
#include XITE_MALLOC_H
#include XITE_STDIO_H

#ifndef MAIN

#ifndef MIN
# define MIN(a,b) (((a) < (b)) ? (a) : (b) )
#endif

static int xsize_s;

static void medianLine(UNS_BYTE *input, UNS_BYTE *output)
{
   int gt, x, val1, val2, val3; /* gt <=> val1 GT val2 */
   val1 = input[1]; val2 = input[2]; val3 = input[3];
   gt = (val1 GT val2);
   output[1] = input[1];
   x = 2;
   LOOP
     IF (gt) 
       IF (val3 LT val2)  output[x] = val2;
       ELSE
         output[x] = (val3 LT val1) ? val3 : val1;
         gt = FALSE;
       ENDIF;
     ELSE
       IF (val3 GT val2)  output[x] = val2;
       ELSE
         output[x] = (val3 LT val1) ? val1 : val3; gt = TRUE;
       ENDIF;
     ENDIF;
     EXITIF(++x EQ xsize_s);
     val1 = val2; val2 = val3; val3 = input[x+1];
   ENDLOOP;
   output[xsize_s] = input[xsize_s];
}


static void psmed(UNS_BYTE *b1, UNS_BYTE *b2, UNS_BYTE *b3, UNS_BYTE *output, UNS_BYTE *Gt)
{
   int x, val1, val2, val3;
   FOR (x=1; x LE xsize_s; INC x)
     val1 = b1[x]; val2 = b2[x]; val3 = b3[x];
     IF (Gt[x]) 
       IF (val3 LT val2)  output[x] = val2;
       ELSE
         output[x] = (val3 LT val1) ? val3 : val1;
         Gt[x] = FALSE;
       ENDIF;
     ELSE
       IF (val3 LT val1)  output[x] = val1; Gt[x] = TRUE;
       ELSE
         output[x] = (val3 LT val2) ? val3 : val2;
       ENDIF;
     ENDIF;
   ENDFOR;
}



/*F:pseudomedian3x3*

________________________________________________________________

		pseudomedian3x3
________________________________________________________________

Name:		pseudomedian3x3 - median like noise reduction filtering

Syntax:         | #include <xite/median.h>
		|
                | int pseudomedian3x3( IBAND input,
                |    IBAND output );

Description:    Filters the input band into the output band
                using a median like filter with window size 3 x 3.
                Given the neighborhood a..i around e:

                | a b c
                | d e f
		| g h i

                First the median of a, b and c is computed,
                likewise the median of d, e and f, and the
                median of g, h and i. Finally the median of
                these three median values replaces the value
                e in the output band.

Restrictions:   'input' and 'output' must have pixel type unsigned byte.

Return value:   | 0 => ok
                | 1 => bad input pixel type
		| 2 => bad output pixel type

Author:		Tor Lønnestad, BLAB, Ifi, UiO.

Id:             $Id$
________________________________________________________________

*/

int pseudomedian3x3(IBAND input, IBAND output)
{
   int x,y, ysize;
   UNS_BYTE *b1, *b2, *b3, *tmp, *Gt;

   if (Ipixtyp(input) NE Iu_byte_typ)
     return(Error(1, "pseudomedian3x3: Input pixel type must be unsigend byte.\n"));
   if (Ipixtyp(output) NE Iu_byte_typ)
     return(Error(2, "pseudomedian3x3: Output pixel type must be unsigned byte.\n"));
   xsize_s = MIN(Ixsize(input),Ixsize(output));
   ysize = MIN(Iysize(input),Iysize(output));

   b1 = (UNS_BYTE*)malloc((xsize_s+1)*sizeof(UNS_BYTE));
   b2 = (UNS_BYTE*)malloc((xsize_s+1)*sizeof(UNS_BYTE));
   b3 = (UNS_BYTE*)malloc((xsize_s+1)*sizeof(UNS_BYTE));
   Gt = (UNS_BYTE*)malloc((xsize_s+1)*sizeof(UNS_BYTE));

   /* calc horizontal median from first three lines */
   medianLine(input[1], b1);  
   medianLine(input[2], b2);
   medianLine(input[3], b3);
   for (x=1; x LE xsize_s; INC x) output[1][x] = b1[x];

   /* calc first pseudomedian3x3 */
   for (x=1; x LE xsize_s; INC x) Gt[x] = (b1[x] GT b2[x]);
   psmed(b1, b2, b3, output[2], Gt);

   FOR (y=4; y LE ysize; INC y)
     tmp = b1; b1 = b2; b2 = b3; b3 = tmp;
     medianLine(input[y], b3);
     psmed(b1, b2, b3, output[y-1], Gt);
   ENDFOR;
   for (x=1; x LE xsize_s; INC x) output[ysize][x] = b3[x];
   free(b1); free(b2); free(b3); free(Gt);
   return(0);
}

#endif /* not MAIN */

/*P:pseudomedian3x3*

________________________________________________________________

		pseudomedian3x3
________________________________________________________________

Name:		pseudomedian3x3 - median like noise reduction filtering

Syntax:		pseudomedian3x3 <input> <output>

Description:    Filters the input image into the output image
                band by band, using a median like filter with
                window size 3 x 3.
                Given the neighborhood a..i :

                | a b c
                | d e f
		| g h i

                First the median of a, b and c is computed, likewise the
                median of d, e and f, and the median of g, h and i. Finally
                the median of these three median values replaces the value e
                in the output band. Corner pixels are copied from input image,
                border pixels are set to the median of the border pixel and
                the two neighboring border pixels.

See also:       median(1)

Restrictions:   'input' must have pixeltype unsigned byte.

Author:		Tor Lønnestad, BLAB, Ifi, UiO.

Examples:       pseudomedian3x3 mona.img monamed.img

Id:             $Id$
________________________________________________________________

*/

#ifdef MAIN

int main(int argc, char **argv)
{
   IMAGE i1, i2;
   int bn, stat;

   InitMessage(&argc, argv, xite_app_std_usage_text(
    "Usage: %s <inimage> <outimage> \n"));
   Iset_message(TRUE);
   Iset_abort(TRUE);

   if (argc == 1) Usage(1, NULL);

   if (argc NE 3) Usage(2, "Illegal number of arguments.\n");

   i1 = Iread_image(argv[1]);
   i2 = Icopy_init(i1);

   for (bn=1; bn LE Inbands(i1); bn++)
     if ((stat = pseudomedian3x3(i1[bn],i2[bn])))
       Warning(stat, "Error in band %d\n", bn);

   Ihistory(i2, argv[0], ""); 
   Iwrite_image(i2,argv[2]);

   return(0);
}

#endif /* MAIN */
