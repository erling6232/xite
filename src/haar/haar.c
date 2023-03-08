

/*

________________________________________________________________

        haar
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

/*F:haar1d=haar*/
/*F:haari1d=haar*/
/*F:haar2d=haar*/
/*F:haari2d=haar*/
/*F:haar*

________________________________________________________________

		haar
________________________________________________________________

Name:		haar, haar1d, haari1d, haar2d, haari2d - 1D and 2D
		forward and inverse Haar Transform

Syntax:         | #include <xite/haar.h>
		|
                | int haar1d( float* a1, float* a2, int size );
		|
                | int haari1d( float* a1, float* a2, int size );
		|
                | int haar2d( IR_BAND b1, IR_BAND b2 );
		|
                | int haari2d( IR_BAND b1, IR_BAND b2 );
Description:    | `haar1d` - 1D forward Haar Transform (HT).
                | `haari1d` - 1D inverse HT
                | `haar2d` - 2D forward HT
                | `haari2d` - 2D inverse HT
		|
                | a1 : input vector
		| a2 : output vector
		| size : size of a1 and a2.
		| b1 : input band
		| b2 : output band

		Identical actual arguments are allowed for b1 and b2.

References:     N. Ahmed and K.R. Rao, Orthogonal Transforms for 
		Digital Signal Processing, p. 263, Springer-Verlag, 1975. 

		H.C. Andrews and K.L. Caspari, "A generalized
		technique for spectral analysis", IEEE Trans., Comput.,
		Vol. C-19, pp. 16-25, 1970.

		N. Ahmed, T. Natarajan and K.R. Rao, "Cooley-Tukey
		type algorithm for the Haar transform", Electronics Letters,
		Vol. 9, pp. 276-278, 1973. 

Restrictions:   Vectors a1 and a2 should have size some power of two.
		Bands b1 and b2 should have overlapping size some power
		of two. Both bands should have pixel type Ireal_typ.
 
Return value:   haar1d and haari1d:
		| 0 => ok
		| 1 => negative size
		| 2 => size not power of 2

		haar2d and haari2d:
		| 0 => ok
                | 1 => bad pixel type band 1
                | 2 => bad pixel type band 2
                | 3 => bad size (not power of two)

See also:       fft2d(3), fft2d(1), fht2d(3), fht2d(1), haarTexture(3),
                haarTexture(1)
Author:		Tor Lønnestad, BLAB, ifi, UiO

Id: 		$Id$
________________________________________________________________

*/

/*P:haar2d=haar*/
/*P:haar*

________________________________________________________________

		haar2d
________________________________________________________________

Name:		haar, haar2d - Two dimensional forward or inverse Haar transform

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
