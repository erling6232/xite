
/*

________________________________________________________________

        
        $Id$
        Copyright 1992, Blab, UiO
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
#include <xite/biff.h>
#include <stdlib.h>
#include <xite/morph.h>
#include <xite/blab.h>
#include <xite/convert.h>
#include <xite/message.h>
#include <xite/readarg.h>



/*F:morphErode=morphDilate*/
/*F:morphClose=morphDilate*/
/*F:morphOpen=morphDilate*/
/*F:morphDilate*

________________________________________________________________

		morphDilate
________________________________________________________________

Name:		morphDilate, morphErode, morphClose, morphOpen - morphological
                grayscale operations

Syntax:         | #include <xite/morph.h>
		|
                | int morphDilate( IBAND inband, IBAND outband,
                |    ISS_BAND B );
                | int morphErode( IBAND inband, IBAND outband,
                |    ISS_BAND B );
                | int morphClose( IBAND inband, IBAND outband,
                |    ISS_BAND B );
                | int morphOpen( IBAND inband, IBAND outband,
                |    ISS_BAND B );

Description:    'morphDilate' calculates the morphological operation
                dilate on 'inband', with a structuring element 'B'.

                'morphErode' calculates the morphological operation erode
		on 'inband'.

		'morphClose' calculates the morphological operation close
                on 'inband'.

		'morphOpen' calculates the morphological operation open
		on 'inband'.

		The result of the operations is sent to the band specified
		in 'outband'.

		The actual calculation for the different operations are
		described by the expressions below (where i and j are inside
		the structuring element 'B', and (0,0) is the center pixel of
		'B').

		&dilate
		| outband(x,y) = max[inband(x+i,y+j)+B(i,j)]

		&erode
                | outband(x,y) = min[inband(x+i,y+j)-B(-i,-j)]

		&close
                | outband = morphErode(morphDilate(inband))

		&open
                | outband = morphDilate(morphErode(inband))

		-&The output values are clipped outside the interval [0, 255]
		to make the result fit in pixels of type unsigned byte.

		Be careful not to use too large values in 'B'. If, e.g. with
		dilation, 'inband' is binary with the two values 0 and 255,
		and all the pixels in 'B' equal 255, then all the pixels in
		'outband' will equal 255, regardless of the distribution of
		zeros in 'inband'.

Restrictions:   'B' must have pixel type signed short, 'inband' and 'outband'
                must have pixel type unsigned byte.

See also:       morphDilate(1), mkMorphForm(3), mkMorphHeight(3), MorphScale(3)

Return value:   | 0 :  OK
                | 1 :  Bad input pixel type
		| 2 :  Bad output pixel type
		| 3 :  Bad pixel type for structuring element

Author:		Martin Torpe Lie, BLAB, Ifi, UiO

Id: 		$Id$
________________________________________________________________

*/

#define TESTVALUE(val) ((val<0)?(0):((val>255)?(255):(val)))
#ifndef MIN
# define MIN(a,b) (((a)<(b)) ? (a) : (b))
#endif

int morphDilate(IBAND inband, IBAND outband, ISS_BAND B)
{
  int x, y, i, j, max, max2, Ixstart_B, Ixsize_B, Iystart_B, Iysize_B,
      Ixstart_inband, Ixsize_inband, Iystart_inband, Iysize_inband;

  if (Ipixtyp(inband) NE Iu_byte_typ)
      return(Error(1, "%s\n",
		   "morphDilate: Input pixel type must be unsigned byte."));
  if (Ipixtyp(outband) NE Iu_byte_typ)
    return(Error(2,"morphDilate: Output pixel type must be unsigned byte.\n"));
  if (Ipixtyp((IBAND) B) NE Is_short_typ)
      return(Error(3, "%s%s\n",
		   "morphDilate: Structuring element pixel type must be ",
		   "signed short."));

  Ixstart_B      = (int)(Ixsize((IBAND) B)/2.0+0.5);
  Ixsize_B       = Ixsize((IBAND) B);
  Iystart_B      = (int)(Iysize((IBAND) B)/2.0+0.5);
  Iysize_B       = Iysize((IBAND) B);
  Ixstart_inband = (int)(Ixsize(inband)/2.0+0.5);
  Ixsize_inband  = MIN(Ixsize(inband), Ixsize(outband));
  Iystart_inband = (int)(Iysize(inband)/2.0+0.5);
  Iysize_inband  = MIN(Iysize(inband), Iysize(outband));

  /* Upper border */
  max = INT_MIN;
  FOR (y=1; y LT Iystart_B; INC y)
    FOR (x=1; x LT Ixstart_B; INC x)
      for (j=Iystart_B-y+1; j LE Iysize_B; INC j)
        FOR (i=Ixstart_B-x+1; i LE Ixsize_B; INC i)
          max2=inband[y+(j-Iystart_B)][x+(i-Ixstart_B)] + B[j][i];
          if (max2 > max)
            max=max2;
        ENDFOR
      outband[y][x]=TESTVALUE(max);
      max=INT_MIN; 
    ENDFOR

    FOR (x=Ixstart_B; x LT (Ixsize_inband-(Ixsize_B-Ixstart_B-1)); INC x)
      for (j=Iystart_B-y+1; j LE Iysize_B; INC j)
        FOR (i=1; i LE Ixsize_B; INC i)
          max2=inband[y+(j-Iystart_B)][x+(i-Ixstart_B)]+B[j][i];
          if (max2 > max)
            max=max2;
        ENDFOR
      outband[y][x]=TESTVALUE(max);
      max=INT_MIN;
    ENDFOR

    FOR (x=(Ixsize_inband-(Ixsize_B-Ixstart_B-1)); x LE Ixsize_inband; INC x)
      for (j=Iystart_B-y+1; j LE Iysize_B; INC j)
        FOR (i=1; i LE (Ixstart_B-x+Ixsize_inband); INC i)
          max2=inband[y+(j-Iystart_B)][x+(i-Ixstart_B)]+B[j][i];
          if (max2 > max)
            max=max2;
        ENDFOR
      outband[y][x]=TESTVALUE(max);
      max=INT_MIN;
    ENDFOR
  ENDFOR

  /* Middle section */
  FOR (y=Iystart_B; y LT (Iysize_inband-(Iysize_B-Iystart_B-1)); INC y)
    FOR (x=1; x LT Ixstart_B; INC x)
      for (j=1; j LE Iysize_B; INC j)
        FOR (i=Ixstart_B-x+1; i LE Ixsize_B; INC i)
          max2=inband[y+(j-Iystart_B)][x+(i-Ixstart_B)]+B[j][i];
          if (max2 > max)
            max=max2;
        ENDFOR
      outband[y][x]=TESTVALUE(max);
      max=INT_MIN;
    ENDFOR

    FOR (x=Ixstart_B; x LT (Ixsize_inband-(Ixsize_B-Ixstart_B-1)); INC x)
      for (j=1; j LE Iysize_B; INC j)
        FOR (i=1; i LE Ixsize_B; INC i)
          max2=inband[y+(j-Iystart_B)][x+(i-Ixstart_B)]+B[j][i];
          if (max2 > max)
            max=max2;
        ENDFOR
      outband[y][x]=TESTVALUE(max);
      max=INT_MIN;
    ENDFOR

    FOR (x=(Ixsize_inband-(Ixsize_B-Ixstart_B-1)); x LE Ixsize_inband; INC x)
      for (j=1; j LE Iysize_B; INC j)
        FOR (i=1; i LE (Ixstart_B+Ixsize_inband-x); INC i)
          max2=inband[y+(j-Iystart_B)][x+(i-Ixstart_B)]+B[j][i];
          if (max2 > max)
            max=max2;
        ENDFOR
        outband[y][x]=TESTVALUE(max);
      max=INT_MIN;
    ENDFOR
  ENDFOR

  /* Lower border */
  FOR (y=(Iysize_inband-(Iysize_B-Iystart_B-1)); y LE Iysize_inband; INC y)
    FOR (x=1; x LT Ixstart_B; INC x)
      for (j=1; j LE Iystart_B-y+Iysize_inband; INC j)
        FOR (i=Ixstart_B-x+1; i LE Ixsize_B; INC i)
          max2=inband[y+(j-Iystart_B)][x+(i-Ixstart_B)]+B[j][i];
          if (max2 > max)
            max=max2;
        ENDFOR
      outband[y][x]=TESTVALUE(max);
      max=INT_MIN;
    ENDFOR

    FOR (x=Ixstart_B; x LT (Ixsize_inband-(Ixsize_B-Ixstart_B-1)); INC x)
      for (j=1; j LE Iystart_B+Iysize_inband-y; INC j)
        FOR (i=1; i LE Ixsize_B; INC i)
          max2=inband[y+(j-Iystart_B)][x+(i-Ixstart_B)]+B[j][i];
          if (max2 > max)
            max=max2;
        ENDFOR
      outband[y][x]=TESTVALUE(max);
      max=INT_MIN;
    ENDFOR

    FOR (x=(Ixsize_inband-(Ixsize_B-Ixstart_B-1)); x LE Ixsize_inband; INC x)
      for (j=1; j LE Iystart_B+Iysize_inband-y; INC j)
        FOR (i=1; i LE (Ixstart_B+Ixsize_inband-x); INC i)
          max2=inband[y+(j-Iystart_B)][x+(i-Ixstart_B)]+B[j][i];
          if (max2 > max)
            max=max2;
        ENDFOR
      outband[y][x]=TESTVALUE(max);
      max=INT_MIN;
    ENDFOR
  ENDFOR

  return(0);
}
