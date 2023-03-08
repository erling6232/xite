/*C*


________________________________________________________________

        glRunLength
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


#include <math.h>
#include <xite/includes.h>
#include <xite/biff.h>
#include <stdlib.h>
#include <xite/texture.h>
#include XITE_STDIO_H
#include <xite/blab.h>
#include <xite/histo.h>
#include <xite/message.h>
#include <xite/readarg.h>



/*F:glrlSre=glrlParameters*/
/*F:glrlLre=glrlParameters*/
/*F:glrlGln=glrlParameters*/
/*F:glrlRln=glrlParameters*/
/*F:glrlRp=glrlParameters*/
/*F:glrlSgre=glrlParameters*/
/*F:glrlLgre=glrlParameters*/
/*F:glrlParameters*

________________________________________________________________

		glrlParameters
________________________________________________________________

Name:		glrlParameters, glrlSre, glrlLre, glrlGln, glrlRln,
                glrlRp, glrlSgre, glrlLgre - extract grey level
                run length parameters

Syntax:         | #include <xite/texture.h>
		|
                | double glrlSre( II_BAND inband, int maxlength,
                |    int noofruns );
                | double glrlLre( II_BAND inband, int maxlength,
                |    int noofruns );
                | double glrlGln( II_BAND inband, int maxlength,
                |    int noofruns );
                | double glrlRp( II_BAND inband, int maxlength,
                |    int noofruns, int noofpixels );
                | double glrlRln( II_BAND inband, int maxlength,
                |    int noofruns );
                | double glrlSgre( II_BAND inband, int maxlength,
                |    int noofruns );
                | double glrlLgre( II_BAND inband, int maxlength,
                |    int noofruns );

Description:	&glrlSre
                Short Run emphasis

		&glrllre
		Long Run emphasis

		&glrlgln
		Grey Level Nonuniformity

		&glrlrln
		Run Length Nonuniformity

		&glrlrp
		Run Percentage

		&glrlsgre
		Low Grey Level Run emphasis

		&glrllgre
		High Grey Level Run emphasisglrlSre 

Restrictions:   'inband' must have pixel type integer.

Return value:   | -1 => bad pixel value in input band

Reference: 	| A. Chu  et al.
                | "Use of grey value distribution of run lengths for texture
		| analysis"
                | Pattern Recognition Letters - 11
                | June 1990 - 415-420.

See also:       glcmParameter(3), glRunLength(3)

Author:		Yogesan, BLAB, Ifi, UiO

Id: 		$Id$
________________________________________________________________

*/

double glrlSre(II_BAND inband, int maxlength, int noofruns)
{ 
  int x, y, ysize; 
  double sre = 0.0;

  if (Ipixtyp((IBAND) inband) != Integer_typ)
    return(Error(-1, "Input band must have pixel type integer.\n"));

  ysize = Iysize((IBAND) inband);

  for (y=1; y <= ysize; y++)
    for (x=1; x <= maxlength; x++) 
      sre += (double) inband[y][x] / (double)(noofruns * x * x);

  return(sre);  
}

double glrlLre(II_BAND inband, int maxlength, int noofruns)
{
  int x, y, ysize;
  double lre = 0.0; 

  if (Ipixtyp((IBAND) inband) != Integer_typ)
    return(Error(-1, "Input band must have pixel type integer.\n"));

  ysize = Iysize((IBAND) inband);

  for (y=1; y <= ysize; y++)
    for (x=1; x <= maxlength; x++) 
      lre += (double) (inband[y][x] * x * x) / (double)(noofruns);
  
  return(lre);  
}

double glrlGln(II_BAND inband, int maxlength, int noofruns)
{
  int x, y, ysize;
  double gln = 0.0, gln1; 

  if (Ipixtyp((IBAND) inband) != Integer_typ)
    return(Error(-1, "Input band must have pixel type integer.\n"));

  ysize = Iysize((IBAND) inband);

  for (y=1; y <= ysize; y++)
    {
      gln1 = 0.0;
      for (x=1; x <= maxlength; x++) 
	{
	  gln1 += (double) inband[y][x];
	}
      gln += (gln1 * gln1) / (double) noofruns;
    }

  return(gln);  
}

double glrlRp(II_BAND inband, int maxlength, int noofruns, int noofpixels)
{
  int x, y, ysize;
  double rp = 0.0; 

  if (Ipixtyp((IBAND) inband) != Integer_typ)
    return(Error(-1, "Input band must have pixel type integer.\n"));

  ysize = Iysize((IBAND) inband);

  for (y=1; y <= ysize; y++)
    for (x=1; x <= maxlength; x++) 
      rp += (double) inband[y][x] / (double)noofpixels;

  return(rp);  
}

double glrlRln(II_BAND inband, int maxlength, int noofruns)
{
  int x, y, ysize;
  double rln = 0.0, rln1; 

  if (Ipixtyp((IBAND) inband) != Integer_typ)
    return(Error(-1, "Input band must have pixel type integer.\n"));

  ysize = Iysize((IBAND) inband);

  for (y=1; y <= maxlength; y++)
    {
      rln1 = 0.0;
      for (x=1; x <= ysize; x++) 
	{
	  rln1 += (double) inband[x][y];
	}
      rln += (rln1 * rln1) / (double) noofruns;
    }
  
  return(rln);  
}

double glrlSgre(II_BAND inband, int maxlength, int noofruns)
{
  int x, y, ysize;
  double sgre = 0.0; 

  if (Ipixtyp((IBAND) inband) != Integer_typ)
    return(Error(-1, "Input band must have pixel type integer.\n"));

  ysize = Iysize((IBAND) inband);

  for (y=1; y <= ysize; y++)
    for (x=1; x <= maxlength; x++) 
      sgre += (double) inband[y][x] / (double)(noofruns * y * y);
  
  return(sgre);  
}

double glrlLgre(II_BAND inband, int maxlength, int noofruns)
{
  int x, y, ysize;
  double lgre = 0.0; 

  if (Ipixtyp((IBAND) inband) != Integer_typ)
    return(Error(-1, "Input band must have pixel type integer.\n"));

  ysize = Iysize((IBAND) inband);

  for (y=1; y <= ysize; y++)
    for (x=1; x <= maxlength; x++) 
      lgre += (double) (inband[y][x] * y * y) / (double)(noofruns);
  
  return(lgre);  
}




/*F:glRunLength*

________________________________________________________________

		glRunLength
________________________________________________________________

Name:		glRunLength - create the Grey level run length matrix

Syntax:         | #include <xite/texture.h>
		|
                | void glRunLength( IBAND input, II_BAND output,
                |    int hori, int vert, int average, int noofgl,
                |    int* maxlength, int* noofruns );

Description:	The run length statistics can be used to extract 
                texture information of an image from its grey level
	        runs. Consecutive pixels of the same grey value, 
		in a given direction (either horizontal or vertical),
		constitute a run. The numbers of runs of different 
		lengths and grey values, arranged according to the 
		lengths and grey levels, form a 2-D matrix called
		grey level run length matrix.

		There are two possible directions to scan the image, either
		horizontal or vertical directions. The average of these two
		directions can also be chosen. Several features can be
		extracted from this matrix.

Restrictions:   'input' must have pixel type unsigned byte. 'output' must
                have pixel type integer.

See also:	glRunLength(1)

Return value:	| 0 => Ok
                | 1 => bad pixel type in input band
                | 2 => bad pixel type in output band

Author:		Yogesan, BLAB, Ifi, UiO
Modified by:    Tor Lønnestad, BLAB, Ifi, UiO

Id: 		$Id$
________________________________________________________________

*/


int glRunLength(IBAND input, II_BAND output, int hori, int vert, int average, int noofgl, int *maxlength, int *noofruns)
{
  int x, y, xsize, ysize, oxsize, oysize;
  int run_length;
  int level, upperlmt, lowerlmt;

  if (Ipixtyp(input) != Iu_byte_typ)
    return(Error(1, "Input band must have pixel type unsigned byte.\n"));

  if (Ipixtyp((IBAND) output) != Integer_typ)
    return(Error(2, "Output band must have pixel type integer.\n"));

  xsize = Ixsize(input);
  ysize = Iysize(input);

  oxsize = Ixsize((IBAND) output);
  oysize = Iysize((IBAND) output);
  for (y=1; y <= oysize; y++)
    for (x=1; x <= oxsize; x++) 
      output[y][x] = 0;

  noofgl = 256 / noofgl;
  level = 1 + (input[1][1]/noofgl);
  upperlmt = (level * noofgl) - 1;
  lowerlmt = (level-1) * noofgl;
  run_length = *maxlength = *noofruns = 0;
  if (hori) {
    for (y=1; y <= ysize; y++)
      for (x=1; x <= xsize; x++) 
	{
	  if(input[y][x] >= lowerlmt && input[y][x] <= upperlmt) {
	    run_length ++;
	  }
	  else if (input[y][x] < lowerlmt || input[y][x] > upperlmt) {
	    (*noofruns) ++;
	    output[level][run_length]++;
	    if (run_length > *maxlength) *maxlength = run_length;
	    level = 1 + (input[y][x]/noofgl);
	    upperlmt = (level * noofgl) - 1;
	    lowerlmt = (level-1) * noofgl;
	    run_length = 1;
	  }
	  
	  if (x == xsize) { 
	    (*noofruns) ++;
	    output[level][run_length]++;
	    if (run_length > *maxlength) *maxlength = run_length;
	    if (y < ysize) {	    
	      level = 1 + (input[y+1][1]/noofgl);
	      upperlmt = (level * noofgl) - 1;
	      lowerlmt = (level-1) * noofgl;
	      run_length = 0; }
	  }
	}
  }
  else if (vert) {
    for (x=1; x <= xsize; x++)
      for (y=1; y <= ysize; y++) 
	{
	  if(input[y][x] >= lowerlmt && input[y][x] <= upperlmt) {
	    run_length ++;
	  }
	  else if (input[y][x] < lowerlmt || input[y][x] > upperlmt) {
	    (*noofruns) ++;
	    output[level][run_length]++;
	    if (run_length > *maxlength) *maxlength = run_length;
	    level = 1 + (input[y][x]/noofgl);
	    upperlmt = (level * noofgl) - 1;
	    lowerlmt = (level-1) * noofgl;
	    run_length = 1;
	  }
	  
	  if (x == xsize) { 
	    (*noofruns) ++;
	    output[level][run_length]++;
	    if (run_length > *maxlength) *maxlength = run_length;
	    if (y < ysize) {	    
	      level = 1 + (input[y+1][1]/noofgl);
	      upperlmt = (level * noofgl) - 1;
	      lowerlmt = (level-1) * noofgl;
	      run_length = 0; }
	  }
	}
  }
  else if (average) {
    for (y=1; y <= ysize; y++)
      for (x=1; x <= xsize; x++) 
	{
	  if(input[y][x] >= lowerlmt && input[y][x] <= upperlmt) {
	    run_length ++;
	  }
	  else if (input[y][x] < lowerlmt || input[y][x] > upperlmt) {
	    (*noofruns) ++;
	    output[level][run_length]++;
	    if (run_length > *maxlength) *maxlength = run_length;
	    level = 1 + (input[y][x]/noofgl);
	    upperlmt = (level * noofgl) - 1;
	    lowerlmt = (level-1) * noofgl;
	    run_length = 1;
	  }
	  
	  if (x == xsize) { 
	    (*noofruns) ++;
	    output[level][run_length]++;
	    if (run_length > *maxlength) *maxlength = run_length;
	    if (y < ysize) {	    
	      level = 1 + (input[y+1][1]/noofgl);
	      upperlmt = (level * noofgl) - 1;
	      lowerlmt = (level-1) * noofgl;
	      run_length = 0; }
	  }
	}

    level = 1 + (input[1][1]/noofgl);
    upperlmt = (level * noofgl) - 1;
    lowerlmt = (level-1) * noofgl;
    run_length = 0; 
    for (x=1; x <= xsize; x++)
      for (y=1; y <= ysize; y++) 
	{
	  if(input[y][x] >= lowerlmt && input[y][x] <= upperlmt) {
	    run_length ++;
	  }
	  else if (input[y][x] < lowerlmt || input[y][x] > upperlmt) {
	    (*noofruns) ++;
	    output[level][run_length]++;
	    if (run_length > *maxlength) *maxlength = run_length;
	    level = 1 + (input[y][x]/noofgl);
	    upperlmt = (level * noofgl) - 1;
	    lowerlmt = (level-1) * noofgl;
	    run_length = 1;
	  }
	  
	  if (x == xsize) { 
	    (*noofruns) ++;
	    output[level][run_length]++;
	    if (run_length > *maxlength) *maxlength = run_length;
	    if (y < ysize) {	    
	      level = 1 + (input[y+1][1]/noofgl);
	      upperlmt = (level * noofgl) - 1;
	      lowerlmt = (level-1) * noofgl;
	      run_length = 0; }
	  }
	}

    
    for (x=1; x < oxsize; x++)
      for (y=1; y < oysize; y++) 
	output[y][x] = output[y][x] / 2;
    (*noofruns) = (*noofruns) / 2; 

  }

  return(0);
}
