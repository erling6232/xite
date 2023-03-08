

/*

________________________________________________________________

        mkHisto
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


#include <stdlib.h>
#include <xite/includes.h>
#include <xite/blab.h>
#include <xite/biff.h>
#include <xite/histo.h>
#include <xite/message.h>
#include <xite/readarg.h>

/*F:mkIhistogram=mkHisto*/
/*F:mkDhistogram=mkHisto*/
/*F:mkHisto*

________________________________________________________________

		mkHisto
________________________________________________________________

Name:		mkHisto, mkIhistogram, mkDhistogram - make a histogram

Syntax:         | #include <xite/histo.h>
                |
                | BiffStatus mkHisto( IBAND band,
                |    histogram histo );
		|
                | BiffStatus mkIhistogram( IBAND band,
                |    int histo[], int limits[], int n );
		|
                | BiffStatus mkDhistogram( IBAND band,
                |    int histo[], double limits[], int n );

Description:    Generate the histogram of a band.
                | band   : input band
                | histo  : output histogram
		| limits : input limits for the accumulator cells in histo
		| n      : input number of limits (n >= 1)

		'limits' should be indexable on 0..'n'-1, and (NOTE!) 'histo'
		should be indexable on 0..'n'. At return, 'histo' contains:
		| histo[0] = #pixels in < -inf .. limits[0] ]
		| histo[i] = #pixels in < limits[i-1] .. limits[i] ]
		| histo[n] = #pixels in < limits[n-1] .. inf. >

		Choose 'mkIhistogram' or 'mkDhistogram', depending on whether
		your limits are integers or real numbers.

Restrictions:   'mkHisto' accepts only unsigned byte pixels, mkIhistogram
                and mkDhistogram accept all pixel types but complex and
		double complex.
		'limits' should be sorted in ascending order.

Return value:   status, see diagnostics

Diagnostics:    | Status 0 => ok
                | Status 2 => Error: Bad pixel type
		| Status 3 => Error: Bad 'n' argument (mkHistogram only)

See also:	histo(3), scale(3)

Author:		Tor Lønnestad, BLAB, Ifi, UiO

Id:             $Id$
________________________________________________________________
*/

BiffStatus mkHisto(IBAND band, int *histo)
{
  int x,y,xsize,ysize;

  if (Ipixtyp(band) != Iu_byte_typ)
    return(Error(2, "mkHisto: Input pixel type must be unsigned byte.\n"));
  xsize = Ixsize(band);
  ysize = Iysize(band);

  for (x=0; x LE 255; histo[x++] = 0) ;
  for (y=1; y LE ysize; y++)
    for (x=1; x LE xsize; x++)
      ++ histo[band[y][x]];

  return(Iok);
}


#define FORALLYX for (y=1; y<=ysize; y++) for (x=1; x<=xsize; x++) {
#define ENDFORALLYX }
#define UPDATE(PIX,LIM) { 				\
           if (PIX <=  LIM[0]) histo[0]++;		\
           else if (PIX > LIM[n-1]) histo[n]++;		\
           else {					\
	     low = 1; high = n-1;			\
             while (low<high) {				\
	       mid = (low+high)/2;			\
	       if (PIX <= LIM[mid]) high = mid;		\
	       else low = mid+1;			\
	     }						\
	     histo[low]++;				\
	     } }


BiffStatus mkIhistogram(IBAND band, int *histo, int *limits, int n)
{
  int low, high, mid, x, y, xsize, ysize, pt, ipix;
  double dpix;
  ISB_BAND sbb;
  IUS_BAND usb;
  ISS_BAND ssb;
  II_BAND  ib;
  IR_BAND  rb;
  ID_BAND  db;
  
  pt = Ipixtyp(band);
  if ((pt == Icomplex_typ) OR (pt == Id_complex_typ))
    return(Error(2, "%s%s\n",
		 "mkIhistogram: Input pixel type must not be complex or ",
		 "double complex."));
  if (n LT 1) return(Error(3, "mkIhistogram: Bad 'n' argument\n"));
  for (x=1; x<n; x++)
    if (limits[x-1] > limits[x])
      return(Error(4, "mkIhistogram: limits not sorted\n"));

  for (x=0; x LE n; histo[x++] = 0) ;

  xsize = Ixsize(band);
  ysize = Iysize(band);
  
  switch (pt) {
  case Iu_byte_typ:
    FORALLYX
      ipix = band[y][x];
      UPDATE(ipix,limits)
    ENDFORALLYX
    break;
    
  case Is_byte_typ:
    sbb = (ISB_BAND) band;
    FORALLYX
      ipix = sbb[y][x];
      UPDATE(ipix,limits)
    ENDFORALLYX
    break;
    
  case Iu_short_typ:
    usb = (IUS_BAND) band;
    FORALLYX
      ipix = usb[y][x];
      UPDATE(ipix,limits)
    ENDFORALLYX
    break;
    
  case Is_short_typ:
    ssb = (ISS_BAND) band;
    FORALLYX
      ipix = ssb[y][x];
      UPDATE(ipix,limits)
    ENDFORALLYX
    break;
    
  case Integer_typ:
    ib = (II_BAND) band;
    FORALLYX
      ipix = ib[y][x];
      UPDATE(ipix,limits)
    ENDFORALLYX
    break;
    
  case Ireal_typ:
    rb = (IR_BAND) band;
    FORALLYX
      dpix = (double) rb[y][x];
      UPDATE(dpix,(double)limits)
    ENDFORALLYX
    break;
    
  case Idouble_typ:
    db = (ID_BAND) band;
    FORALLYX
      dpix = db[y][x];
      UPDATE(dpix,(double)limits)
    ENDFORALLYX
    break;
    
  }
  
  return(Iok);
}



BiffStatus mkDhistogram(IBAND band, int *histo, double *limits, int n)
{
  int low, high, mid, x, y, xsize, ysize, pt;
  double dpix;
  ISB_BAND sbb;
  IUS_BAND usb;
  ISS_BAND ssb;
  II_BAND  ib;
  IR_BAND  rb;
  ID_BAND  db;
  
  pt = Ipixtyp(band);
  if ((pt == Icomplex_typ) OR (pt == Id_complex_typ))
    return(Error(2, "%s%s\n",
		 "mkDhistogram: Input pixel type must not be complex or ",
		 "double complex."));
  if (n LT 1) return(Error(3, "mkDhistogram: Bad 'n' argument\n"));
  for (x=1; x<n; x++)
    if (limits[x-1] > limits[x])
      return(Error(4, "mkDhistogram: limits not sorted\n"));

  for (x=0; x LE n; histo[x++] = 0) ;

  xsize = Ixsize(band);
  ysize = Iysize(band);
  
  switch (pt) {
  case Iu_byte_typ:
    FORALLYX
      dpix = (double) band[y][x];
      UPDATE(dpix,limits)
    ENDFORALLYX
    break;
    
  case Is_byte_typ:
    sbb = (ISB_BAND) band;
    FORALLYX
      dpix = (double) sbb[y][x];
      UPDATE(dpix,limits)
    ENDFORALLYX
    break;
    
  case Iu_short_typ:
    usb = (IUS_BAND) band;
    FORALLYX
      dpix = (double) usb[y][x];
      UPDATE(dpix,limits)
    ENDFORALLYX
    break;
    
  case Is_short_typ:
    ssb = (ISS_BAND) band;
    FORALLYX
      dpix = (double) ssb[y][x];
      UPDATE(dpix,limits)
    ENDFORALLYX
    break;
    
  case Integer_typ:
    ib = (II_BAND) band;
    FORALLYX
      dpix = (double) ib[y][x];
      UPDATE(dpix,limits)
    ENDFORALLYX
    break;
    
  case Ireal_typ:
    rb = (IR_BAND) band;
    FORALLYX
      dpix = (double) rb[y][x];
      UPDATE(dpix,limits)
    ENDFORALLYX
    break;
    
  case Idouble_typ:
    db = (ID_BAND) band;
    FORALLYX
      dpix = db[y][x];
      UPDATE(dpix,limits)
    ENDFORALLYX
    break;
    
  }
  
  return(Iok);
}
