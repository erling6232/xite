
/*

________________________________________________________________

        addPoisson
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
#include <xite/noise.h>
#include <xite/message.h>
#include XITE_RANDOM_H
#include XITE_STDIO_H
#include XITE_TIME_H



/*F:addPoisson*

________________________________________________________________

		addPoisson
________________________________________________________________

Name:		addPoisson - add Poisson noise to the output band

Syntax:         | #include <xite/noise.h>
                |
                | int addPoisson( IBAND input, IBAND output );

Description:    addPoisson generates Poisson distributed noise in the
                'output' band. For each 'output' pixel, the corresponding
		pixelvalue in 'input' is used as the mean value in the
		distribution. If the resulting random value is larger than
		255, output is set to 255.

Restrictions:   Only unsigned byte pixeltype is accepted for 'input' and
                'output'.

Return value:   | 0 => ok
                | 1 => Bad pixel type
		 
Author:		Helene Schulerud, BLAB, Ifi, UiO

Id:             $Id$
________________________________________________________________
 
*/

int addPoisson(IBAND input, IBAND output)
{
  int x, y, xsize, ysize;
  int n;
  long rl;
  double rl2,p,ek,xl;
 
  if (Ipixtyp(input) != Iu_byte_typ || Ipixtyp(output) != Iu_byte_typ)
    return(Error(1, "addPoisson: Bad pixel type.\n"));

  xsize = Ixsize(input);
  ysize = Iysize(input);

  srandom(time(0)); /* random initialization of random generator */

  for ( y=1; y<=ysize; y++ )
    for ( x=1; x<=xsize; x++ )
      {
	n=0;
	p=1;
	rl=random();
	rl2=(double)rl/RANDOM_MAX;
	xl=(double)input[y][x];
	p=p*rl2;
        ek = (double)(exp(-xl));
	while ( p >= ek )
	  {
	    n++;
	    rl=random();
	    rl2=(double)rl/RANDOM_MAX;
	    xl=(double)input[y][x];
	    p=p*rl2;
	    ek = (exp(-xl));
	  }
	
	if (n<=255)
	  output[y][x] = n ;
	else
	  output[y][x] = 255; 

      }
 
  return(0);
}
