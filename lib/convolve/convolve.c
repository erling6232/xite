

/*

________________________________________________________________

        convolve.c
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
#include <xite/biff.h>
#include <xite/convolve.h>
#include XITE_STDIO_H
#include <stdlib.h>
#include <xite/convert.h>
#include <xite/readarg.h>
#include <xite/message.h>
#include XITE_STRING_H

static char *convolve_errors[] = {
"Ok.\n",
"Input and output pixeltype must be unsigned byte, signed or unsigned short.\n",
"Kernel pixeltype must be unsigned or signed short.\n",
"Size error.\n",
"Not enough memory.\n",
};

int convolve_overflow;
int convolve_underflow;



/*F:convolve*

________________________________________________________________

		convolve
________________________________________________________________

Name:		convolve - perform an n x m convolution
Syntax:         | #include <xite/convolve.h>
		|
                | BiffStatus convolve( IBAND input, IBAND output,
                |    ISS_BAND convk, double scale, double offset,
                |    int verbose );

Description:    'convolve' performs the convolution between
                a band 'input' and a convolution kernel 'convk'.
		'convk' is a signed short band.
                The result of the convolution is scaled
		|   pix = (conv. result) * scale + offset;
                If 'scale' is zero, an autoscaling is performed.
		If 'verbose' flag is nonzero, the number of underflows and
		overflows are printed. If 'verbose' flag is nonzero
		and autoscaling is performed, the scale and offset
		values are displayed.

		'convolve_overflow' and 'convolve_underflow' are 
		global variabels and may be declared as
		| extern int convolve_overflow, convolve_underflow

See also:	ascii2biff(3), convolve(1), conv3x3(3), laplace(3),
                gradient(3), sobel(3), prewitt(3), roberts(3)

Return value:	| 0 - ok
                | 1 - Bad pixeltype of input or output
		| 2 - Bad pixeltype of convk
		| 3 - Input band and output band must be of equal size
		| 4 - Not enough memory

Restrictions:	Works on unsigned byte and signed short images.
                Unsigned short images will be treated as if they
		were signed short. 

Author:		Otto Milvang

Id: 		$Id$
________________________________________________________________

*/

BiffStatus convolve(IBAND input, IBAND output, ISS_BAND convk, double scale, double offset, int verbose)
{
  UNS_BYTE *ptr;
  int sum, xsize, ysize, ixsize, iysize, oxsize, oysize;
  int min, max, x, y, xx, yy, iscale, ioffset, cxsize, cysize;
  int cx, cy,weight, size, sumn, sump;
  short *sptr, *iptr, w[256];
  ISS_BAND sinput, result;
  IPIXTYP ipixtyp, opixtyp, kpixtyp;
 
  iscale  = scale  * 4096.0;
  ioffset = offset * 4096.0;
  
  ixsize  = Ixsize(input);
  iysize  = Iysize(input);
  ipixtyp = Ipixtyp(input);
  oxsize  = Ixsize(output);
  oysize  = Iysize(output);
  opixtyp = Ipixtyp(output);
  cxsize  = Ixsize((IBAND) convk);
  cysize  = Iysize((IBAND) convk);
  kpixtyp = Ipixtyp((IBAND) convk);
  xsize   = ixsize;
  ysize   = iysize;

  if (ipixtyp != Iu_byte_typ && 
      ipixtyp != Iu_short_typ && 
      ipixtyp != Is_short_typ) return(Error(1, convolve_errors[1]));

  if (opixtyp != Iu_byte_typ && 
      opixtyp != Iu_short_typ && 
      opixtyp != Is_short_typ) return(Error(1, convolve_errors[1]));

  if (kpixtyp != Iu_short_typ &&
      kpixtyp != Is_short_typ)
    return(Error(2, convolve_errors[2]));

  if (ixsize != oxsize || iysize != oysize)
      return(Error(3, convolve_errors[3]));

  if (ipixtyp != Iu_byte_typ) sinput = (ISS_BAND) input;
  if (opixtyp == Iu_byte_typ) 
    {
      result = (ISS_BAND) Imake_band(Is_short_typ, oxsize, oysize);
      if (result == NULL) return(Error(4, convolve_errors[4]));
    } else {
      result = (ISS_BAND) output;
    }

  convolve_overflow = convolve_underflow = 0;
  
  sumn = sump = 0;
  for (y=1; y<=cysize; y++)
    for (x=1; x<=cxsize; x++)
      if (convk[y][x] < 0)
        sumn += convk[y][x];
      else
	sump += convk[y][x];
  if (-sumn > sump) sump = -sumn;
  if (sump<= 128) sump = 0;

  for (y=1; y<=oysize; y++)
    {
      sptr = &result[y][1];
      size = oxsize;
      while(size--) *sptr++ = 0;
    }
 
  for (cy = - (cysize-1)/2; cy <= cysize/2; cy++)
    for (cx = - (cxsize-1)/2; cx <= cxsize/2; cx++)
      {
	weight = convk[cy + (cysize+1) / 2][cx + (cxsize+1) / 2]; 
        if (weight == 0) continue;
	if (sump)
	  for (x=0; x<=255; x++) w[x] = weight*x*128/sump;
	else
	  for (x=0; x<=255; x++) w[x] = weight*x;
	size = (xsize * (cy < 0 ? ysize + cy : ysize - cy)) + 
	  (cx < 0 ? cx : -cx);
	for (y=1; y<=ysize; y++)
	  {
	    sptr = &result[y][1];
	    yy = y+cy;
	    if (yy < 1) yy = 1;
	    if (yy > ysize) yy = ysize;
	    xx = 1+cx;
	    if (xx < 1) xx = 1;
	    if (xx > xsize) xx = xsize;
	    if (ipixtyp != Iu_byte_typ)
	      {
		iptr = &sinput[yy][xx];
		if (cx < 0)
		  {
		    size = -cx;
		    if (weight == 1 && sump == 0)
		      while(size--) *sptr++ += *iptr;
		    else
		      if (weight == -1 && sump == 0)
			while(size--) *sptr++ -= *iptr;
		      else
			while(size--) *sptr++ += w[*iptr];
		  }
		size = xsize + (cx < 0 ? cx : -cx);
		if (weight == 1 && sump == 0)
		  while(size--) *sptr++ += *iptr++;
		else
		  if (weight == -1 && sump == 0)
		    while(size--) *sptr++ -= *iptr++;
		  else
		    while(size--) *sptr++ += w[*iptr++];
		if (cx > 0)
		  {
		    iptr--;
		    size = cx;
		    if (weight == 1 && sump == 0)
		      while(size--) *sptr++ += *iptr;
		    else
		      if (weight == -1 && sump == 0)
			while(size--) *sptr++ -= *iptr;
		      else
			while(size--) *sptr++ += w[*iptr];
		  }
	      } else {
		ptr = &input[yy][xx];
		if (cx < 0)
		  {
		    size = -cx;
		    if (weight == 1 && sump == 0)
		      while(size--) *sptr++ += *ptr;
		    else
		      if (weight == -1 && sump == 0)
			while(size--) *sptr++ -= *ptr;
		      else
			while(size--) *sptr++ += w[*ptr];
		  }
		size = xsize + (cx < 0 ? cx : -cx);
		if (weight == 1 && sump == 0)
		  while(size--) *sptr++ += *ptr++;
		else
		  if (weight == -1 && sump == 0)
		    while(size--) *sptr++ -= *ptr++;
		  else
		    while(size--) *sptr++ += w[*ptr++];
		if (cx > 0)
		  {
		    ptr--;
		    size = cx;
		    if (weight == 1 && sump == 0)
		      while(size--) *sptr++ += *ptr;
		    else
		      if (weight == -1 && sump == 0)
			while(size--) *sptr++ -= *ptr;
		      else
			while(size--) *sptr++ += w[*ptr];
		  }
	      }
	  }
      }
  if (iscale == 0 && opixtyp == Iu_byte_typ)
    { 
      sptr = &result[1][1];
      min = *sptr; max = *sptr;
      for (y=1; y<=ysize; y++)
	{
	  sptr = &result[y][1];
	  size = xsize;
	  while (size--)
	    {
	      if(*sptr < min) min = *sptr;
	      if(*sptr > max) max = *sptr;
	      sptr++;
	    }
	}
      if (max-min)
	{
	  iscale = 0x100000 / (max-min);
	  ioffset = - (min*iscale) + 1;
	  if (sump) iscale = iscale * 128/sump;
	} else {
	  iscale = 0;
	  ioffset = 0;
	}
    }
  if (verbose && sump)
    printf("Weights = %6d\n", sump);

  if (iscale != 0 || opixtyp == Iu_byte_typ)
    {
      if (verbose)
	{
	  printf("Scale   = %.4f\n", (double) iscale / 4096.0);
	  printf("Offset  = %6d\n", ioffset/4096);
	}
      if (sump) iscale = iscale * sump/128;
      for (y=1; y<=ysize; y++)
	{
	  size = xsize;
	  sptr = &result[y][1];
	  if (opixtyp == Iu_byte_typ)
	    {
	      ptr = &output[y][1];
	      while(size--)
		{
		  sum = *sptr++ * iscale + ioffset;       
		  if (sum & ~0xfffff) 
		    {
		      if (sum < 0)
			{
			  convolve_underflow++;
			  *ptr++ = 0;
			} else {
			  convolve_overflow++;
			  *ptr++ = 255;
			}
		    } else
		      *ptr++ = sum >> 12;
		}
	    } else {
	      while(size--)
		{
		  sum = *sptr * iscale + ioffset;
		  *sptr++ = sum >> 12;
		}
	    }
	}
    }
  if (opixtyp == Iu_byte_typ) Idel_band((IBAND *) &result);
  if (verbose) 
    {
      printf("%6d Underflow%c\n",convolve_underflow, 
	      convolve_underflow == 1? ' ':'s');
      printf("%6d Overflow%c\n" ,convolve_overflow, 
	      convolve_overflow == 1? ' ':'s');
    }
  return(0);
}  
