
/*

________________________________________________________________

        histoNorm
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
#include XITE_STDIO_H
#include <xite/blab.h>
#include <xite/histo.h>
#include <xite/message.h>

#ifndef MAIN

/*F:histoNorm*

________________________________________________________________

		histoNorm
________________________________________________________________

Name:		histoNorm - histogram normalization

Syntax:         | #include <xite/histo.h>
                |
                | int histoNorm( IBAND b1, IBAND b2, double my,
                |    double sigma );
Description:    Perform histogram normalization; approximate hi-
                stogram of output image to the normal (gaussian)
                distribution with mean value my and st.dev.=sigma.
                Input and output may be identical. Only the largest
                rectangle common to b1 and b2 is transformed.

                | b1 - input band
                | b2 - output band
                | my - desired mean value
                | sigma - desired standard deviation

Return value:   | 0 => ok
                | 1 => bad input pixel type
                | 2 => bad output pixel type

Author:		Tor L|nnestad

Id:             $Id$
________________________________________________________________
*/

#ifndef FUNCPROTO
int histoNorm(b1, b2, my, sigma)
  IBAND b1, b2;
  double my, sigma;
#else /* FUNCPROTO */
int histoNorm(IBAND b1, IBAND b2, double my, double sigma)
#endif /* FUNCPROTO */
{
  int n;
  histogram normHisto;
  double sigma2_2 = 2*sigma*sigma;

  if (Ipixtyp(b1) != Iu_byte_typ)
    return(Error(1, "histoNorm: Bad input pixel type\n"));
  if (Ipixtyp(b2) != Iu_byte_typ)
    return(Error(2, "histoNorm: Bad output pixel type\n"));

  /* make normal distribution */
  FOR (n=0; n LE 255; INC n)
    normHisto[n] =
      (int)(exp(-((double)n-my)*((double)n-my)/sigma2_2)*100000.0+0.5);
  ENDFOR;
  histoSpecify(b1, b2, normHisto);
  return(0);
}

#endif /* not MAIN */



/*P:histoNorm*

________________________________________________________________

		histoNorm
________________________________________________________________

Name:		histoNorm - histogram normalization

Syntax:		| histoNorm <input image> <output image> [my] [sigma]

Description:    histoNorm makes a new image with the size of the
                input image. Every band in the output image is a
                histogram normalized version of the corresponding
                band  in  the input image, the bands will have
                approximately  normal (gaussian) histogram
                distribution with mean = my and standard
                deviation = sigma. Default values for these are
                my = 127.5 and sigma = 50.0

Files:	        biff.h histo.h 

See also:	
Diagnostics:
Restrictions:
Return value:

Author:		Tor Loennestad, BLAB, ifi, UiO

Examples:       histoNorm mona.img moNorm.img
________________________________________________________________

*/

#ifdef MAIN

#ifndef FUNCPROTO
int main(argc,argv)
int argc;
char *argv[];
#else /* FUNCPROTO */
int main(int argc, char **argv)
#endif /* FUNCPROTO */
{
   IMAGE i;
   int bn;
   double my,sigma;
   char comment[40];

   InitMessage(&argc, argv, xite_app_std_usage_text(
    "Usage: %s <oldfile> <newfile>\n"));
   Iset_message(TRUE);
   if (argc == 1) Usage(1, (char*)0);
   if ((argc < 3) OR (argc > 5)) Usage(2, "Bad number of arguments\n");

   i = Iread_image(argv[1]);
   if (argc GE 4) my = atof(argv[3]); else my=127.5;
   if (argc GE 5) sigma = atof(argv[4]); else sigma = 50.0;

   for (bn=1; bn LE Inbands(i); bn++)
     if (histoNorm(i[bn],i[bn],my,sigma))
       Warning(3, " - in band %d\n", bn);

   sprintf(comment, " with my = %.2f and sigma = %.2f", my, sigma);
   Ihistory(i, argv[0], comment);
   Iwrite_image(i,argv[2]);
   return(0);
}

#endif /* MAIN */
