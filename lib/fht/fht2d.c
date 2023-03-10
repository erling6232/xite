

/*C*

________________________________________________________________

        fht2d
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

#define PRINTA(a,b) printf(" %s   %f %f %f %f\n",a,b[1],b[2],b[3],b[4]); 
#define PRINTAR(a,b) PRINTA(a,b[1]); PRINTA("",b[2]); PRINTA("",b[3]); PRINTA("",b[4]);

#include <stdlib.h>
#include <math.h>
#include <xite/includes.h>
#include <xite/biff.h>
#include <xite/fht.h>
#include XITE_STDIO_H
#include <xite/convert.h>
#include <xite/copy.h>
#include <xite/message.h>
#include <xite/readarg.h>
#include XITE_MALLOC_H

#ifndef PI
# define PI 3.14159265358979323846
#endif

static void transpose(IR_BAND bin, IR_BAND bout)
{
  int x, y, xsize, ysize;
  float buf;

  xsize = Ixsize((IBAND) bin);
  ysize = Iysize((IBAND) bin);

  if (bin == bout) {
    for (y=1; y <= ysize; y++) {
      for (x=y+1; x <= xsize; x++) {
        buf = bout[y][x];
        bout[y][x] = bout[x][y];
        bout[x][y] = buf;
      }
    }
  } else {
    for (y=1; y <= ysize; y++)
      for (x=1; x <= xsize; x++)
        bout[y][x] = bin[x][y];
    for (x=1; x <= xsize; x++) bout[x][x] = bin[x][x];
  }
}

static void specialTransp(IR_BAND bin, IR_BAND bout)
{
  int x, y, xsize, ysize, xsizep2, ysizep2;

  xsize   = Ixsize((IBAND) bin);
  ysize   = Iysize((IBAND) bin);
  xsizep2 = xsize+2;
  ysizep2 = ysize+2;

  for (x=1; x <= xsize; x++) bout[1][x] = bin[x][1];
  for (y=1; y <= ysize; y++) bout[y][1] = bin[1][y];
  for (y=2; y <= ysize; y++)
    for (x=2; x <= xsize; x++)
      bout[x][y] = (bin[y][x] + bin[y][xsizep2-x] 
		    + bin[ysizep2-y][x] - bin[ysizep2-y][xsizep2-x])/2.0;  
}
 

static int permute(int index, int power_index)
{
  int i, j, s;

  j = 0;
  for (i=1; i <= power_index; i++) {
    s = index / 2;
    j = 2*j + index - 2*s;
    index = s;
  }

  return(j);

} /* permute() */




/*F:fhtInitTables*

________________________________________________________________

		fhtInitTables
________________________________________________________________

Name:		fhtInitTables - initialize tables for fast Hartley transform

Syntax:         | #include <xite/fht.h>
		|
                | int fhtInitTables( float* sinTab,
                |    float* cosTab, int* perm, int size );

Description:	Initializes the three tables 'sinTab', 'cosTab' and 'perm'.
                The tables must be allocated by the caller, and have
		'size' elements.

Restrictions:   Size must be >= 2 and power of two

See also:       fht1d(3), fht2d(3)

Return value:   | 0 => ok
                | 1 => bad size

Author:		Tor L?nnestad, BLAB, Ifi, UiO
Revised:        Svein B?e, BLAB, Ifi, UiO

Id: 		$Id$
________________________________________________________________

*/

int fhtInitTables(float *sinTab, float *cosTab, int *perm, int size)
{
  int i, ipi, ipihalf, i3pihalf, pow;
  double angle, omega;
  float f;

  if (size < 2) return(1);
  for (pow=0, i=2; i <= size; i*=2, pow++);
  i /= 2;
  if (i != size) return(1);

  omega            = 2.0*PI/size;
  angle            = omega;
  ipi              = size/2;
  ipihalf          = size/4;
  i3pihalf         = ipi + ipihalf;
  sinTab[0]        = 0.0;
  sinTab[ipihalf]  = 1.0;
  sinTab[ipi]      = 0.0;
  sinTab[i3pihalf] = -1.0;
  cosTab[0]        = 1.0;
  cosTab[ipihalf]  = 0.0;
  cosTab[ipi]      = -1.0;
  cosTab[i3pihalf] = 0.0;

  for (i=1; i < ipihalf; i++) {
    f = (float)sin(angle);
    sinTab[i]          =  f;
    sinTab[ipi-i]      =  f;
    sinTab[ipi+i]      = -f;
    sinTab[size-i]     = -f;
    cosTab[ipihalf-i]  =  f;
    cosTab[ipihalf+i]  = -f;
    cosTab[i3pihalf-i] = -f;
    cosTab[i3pihalf+i] =  f;
    angle = angle + omega;
  }

  for (i=0; i < size; i++) perm[i] = permute(i, pow);

  return(0);

} /* fhtInitTables() */



/*F:fht1d*

________________________________________________________________

		fht1d
________________________________________________________________

Name:		fht1d - 1 dimensional fast Hartley transform

Syntax:         | #include <xite/fht.h>
		|
                | void fht1d( float* data, float* sinTab,
                |    float* cosTab, float* work, int* perm,
                |    int size, int power, int forw );

Description:    'fht1d' performs 1 dimensional fast Hartley transform,
                forward transform if 'forw' is true, inverse otherwise.
		After transformation, the result is placed in 'data'.

Input arguments:
		&data
		The array to be transformed in place.

		&sinTab
		Sine table; 0..2PI with 'size' elems.

		&cosTab
		Cosine table; 0..2PI with 'size' elems.

		&work
		Work space: float array with 'size' elems.

		&perm
		Permutation table with 'size' elems.

		&size
		The size of the arrays.

		&power
		The power of two (log2 of size).

		&forw
		TRUE => forward transform, FALSE => inverse.

		-&
		The tables 'sinTab', 'cosTab' and 'perm' must be initialized 
		before calling this routione. Initialization can be done
		with the 'fhtInitTables' routine.

Restrictions:   For efficiency reasons, input arguments are not tested.

See also:	fht2d(3), haar1d(3), fhtInitTables(3)

Return value:	none

Author:		Tor L?nnestad, BLAB, Ifi, UiO

Id: 		$Id$
________________________________________________________________

*/


void fht1d(float *data, float *sinTab, float *cosTab, float *work, int *perm, int size, int power, int forw)
{
  int pow, i, j, k, trg_ind, trg_inc, tfhSize;
  int i_2, i_3, section, s_start, halfSize;
  float *work2, *work3;

  halfSize = size/2;
  /* permutation */
  for (i=0; i < size; i++)
    work[perm[i]] = data[i];
  work2 = data;

  /* first iteration: 2-transform */
  for (i=0; i < size; i+=2) {
    work2[i]   = work[i] + work[i+1];
    work2[i+1] = work[i] - work[i+1];
  }

  /* second iteration: 4-transform */
  if (size >= 4)
    for (i=0; i < size; i+=4) {
      work[i]   = work2[i]   + work2[i+2];
      work[i+1] = work2[i+1] + work2[i+3];
      work[i+2] = work2[i]   - work2[i+2];
      work[i+3] = work2[i+1] - work2[i+3];
    }

  /* general loop for 8-transform and upwards */
  tfhSize = 4;
  for (pow=3; pow <= power; pow++) {
    j = 0;
    section = 1;
    trg_inc = size / (2*tfhSize);

    while (1) {
      s_start    = section * tfhSize;
      i_2        = j + tfhSize;
      trg_ind    = trg_inc;

      work2[j]   = work[j] + work[i_2];
      work2[i_2] = work[j] - work[i_2];

      j++;
      i_2++;
      i_3 = s_start+s_start + tfhSize-i_2;

      for (k=2; k <= tfhSize; k++) {
        work2[j] = work[j]
                 + work[i_2] * cosTab[trg_ind]
                 + work[i_3] * sinTab[trg_ind];
        work2[i_2] = work[j]
                 + work[i_2] * cosTab[trg_ind + halfSize]
                 + work[i_3] * sinTab[trg_ind + halfSize];

        trg_ind += trg_inc;
        j++;
        i_2++;
        i_3--;
      }

      j += tfhSize;
      section += 2;
      if (j >= size) break;
    }

    tfhSize *= 2;
    work3 = work2; work2 = work; work = work3;
  }

  if (forw) {
    for (i=0; i < size; i++) data[i] = work[i] / size;
  } else {
    for (i=0; i < size; i++) data[i] = work[i];
  }

}   /*  fht1d  */



/*F:fht2d*

________________________________________________________________

		fht2d
________________________________________________________________

Name:		fht2d - 2 dimensional fast Hartley transform

Syntax:         | #include <xite/fht.h>
		|
                | BiffStatus fht2d( IR_BAND inband,
                |    IR_BAND outband, int forward );

Description:	'fht2d' performs 2 dimensional fast Hartley transform,
                forward transform if 'forward' is true, inverse transform
		otherwise.

		The forward and inverse discrete Hartley transforms
		are given respectively as:

		|
		|           M   N
		| H(u,v) = Sum Sum f(x,y) [cas(2*Pi(u*x/M + v*y/N))]
		|          x=0 y=0
		|
		|           M   N
		| f(x,y) = Sum Sum H(u,v) [cas(2*Pi(u*x/M + v*y/N))]
		|          x=0 y=0
		|
		where
		| cas(x) = cos(x) + sin(x)

		The relationships between the Fourier and Hartley transforms
		are described in the documentation of 'ht2ft(3)' and
		'ft2ht(3)'.

		For a description of how to use Hartley transforms to
		accomplish convolution, refer to fht2d(1).

Restrictions:	Only real pixels are accepted as input and output.
                Input and output bands should be of equal size and be
		some power of two.

References:	&[1] 'Mark A. O'Neill'
                "Faster than Fast Fourier",
		BYTE, pp 293-300, April 1988

		&[2] 'R. N. Bracewell'
		"The Hartley Transform",
		Oxford Univ. Press 1986,
		ISBN 0-19-503969-6

See also:	fht2d(1), fft2d(3), haar2d(3), fht1d(3), fhtInitTables(3),
                fhtPower(3), fhtPhase(3), ht2ft(3), ft2ht(3)

Return value:   | 0 => ok
                | 1 => bad input pixel type
		| 2 => bad output pixel type
		| 3 => band size is not power of two
		| 4 => bands are not of equal size
		| 5 => error in copyBand()

Author:		Tor L?nnestad, BLAB, Ifi, UiO
Revised:        Svein B?e, BLAB, Ifi, UiO

Id: 		$Id$
________________________________________________________________

*/

BiffStatus fht2d(IR_BAND inband, IR_BAND outband, int forward)
{
  float *sinTab,*cosTab;
  float *work;
  int *perm;
  int size, line, powsize, pow;
  IR_BAND tmpband;

  if (Ipixtyp((IBAND) inband) != Ireal_typ)
    return(Error(1, "fht2d: Bad input pixel type\n"));
  if (Ipixtyp((IBAND) outband) != Ireal_typ)
    return(Error(2, "fht2d: Bad output pixel type\n"));

  size = Ixsize((IBAND) inband);
  powsize = 2;
  for (pow = 0; powsize <= size; pow++) powsize *= 2;
  powsize = powsize/2;

  if (powsize != size)
    return(Error(3, "fht2d: Band size not power of two\n"));
  if ((size != Iysize((IBAND) inband)) ||
      (size != Ixsize((IBAND) outband)) ||
      (size != Iysize((IBAND) outband)))
    return(Error(4, "fht2d: Bands are not of equal size\n"));

  sinTab = (float*) malloc(size * sizeof(float));
  cosTab = (float*) malloc(size * sizeof(float));
  work   = (float*) malloc(size * sizeof(float));
  perm   = (int*)   malloc(size * sizeof(int));
  fhtInitTables(sinTab, cosTab, perm, size);

  /* Must use copy of inband because fft1d works in-place. Must use
   * an auxiliary band, not outband, for the copy, because of the call to
   * specialTransp() below.
   */
  tmpband = (IR_BAND) Imake_band(Ipixtyp((IBAND) inband),
				 Ixsize((IBAND) inband),
				 Iysize((IBAND) inband));

  if (copyBand((IBAND) inband, (IBAND) tmpband) != 0)
    return(Error(5, "fht2d: Error in copyBand().\n"));

  for (line=1; line <= size; line++)
    fht1d(&(tmpband[line][1]), sinTab, cosTab, work, perm, size, pow, forward);

  transpose(tmpband, tmpband);

  for (line=1; line <= size; line++)
    fht1d(&(tmpband[line][1]), sinTab, cosTab, work, perm, size, pow, forward);

  /* The two arguments must be different bands. */
  specialTransp(tmpband, outband);

  Idel_band((IBAND *) &tmpband);

  return(0);

} /* fht2d() */
