
/*

________________________________________________________________

        gaussfunc
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
#include <xite/utils.h>
#include XITE_RANDOM_H

#ifndef M_PI
#define M_PI    3.14159265358979323846
#endif

static double tempvalue = 255.0;



/*L:gaussUnitRandom*

________________________________________________________________

		gaussUnitRandom
________________________________________________________________

Name:		gaussUnitRandom - random variable with gaussian unit distribution
Syntax:		| double gaussUnitRandom()
Description:    returns a random variable with gaussian distribution
                of type double. The gaussion distribution has mean 0.0
		and standard deviation 1.0
Return value:   The random variable
Author:		Tor Lønnestad, BLAB, Ifi, UiO
________________________________________________________________

*/
#ifndef FUNCPROTO
static double gaussUnitRandom()
#else /* FUNCPROTO */
static double gaussUnitRandom(void)
#endif /* FUNCPROTO */
{
   double a, b;
   if (tempvalue < 254.0)
   {
      a = tempvalue;
      tempvalue = 255.0;
      return(a);
   }
   else
   {
      a = random() / RANDOM_MAX;
      a = sqrt(-2.0 * log(a));
      b = 2.0 * M_PI * random() / RANDOM_MAX;
      tempvalue = a * sin(b);
      a = a * cos(b);
      return(a);
   }
}




/*F:gaussRandom*

________________________________________________________________

		gaussRandom
________________________________________________________________

Name:		gaussRandom -  random function with gaussian distribution

Syntax:         | #include <xite/utils.h>
		|
                | double gaussRandom( double my, double sigma );

Description:    'gaussRandom' returns a random varable of type double
                with gaussian distribution, mean value 'my', and standard
		deviation 'sigma'. The C library routine 'random' is used
		for random number generation, the user must call the
		initialization routine 'srandom' once before starting to
		call gaussRandom. Using a fixed number (int) as input to
		srandom enables you to reproduce the random sequence,
		otherwise time(0) may be used as argument.

Return value:   The random variable of type double.

Author:		Tor L|nnestad, BLAB, Ifi, UiO

Id: 		$Id$
________________________________________________________________

*/

#ifndef FUNCPROTO
double gaussRandom(my, sigma)
   double my, sigma;
#else /* FUNCPROTO */
double gaussRandom(double my, double sigma)
#endif /* FUNCPROTO */
{
   return(my + (sigma*gaussUnitRandom()));
}


