

/*

________________________________________________________________

        crossSection
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
#include <xite/arithmetic.h>
#include XITE_STDIO_H
#include XITE_STRING_H
#include XITE_MALLOC_H
#include <xite/draw.h>
#include <xite/message.h>
#include <xite/readarg.h>

#ifndef MAX
# define MAX(a, b) ((a) >= (b) ? (a) : (b))
#endif
#ifndef MIN
# define MIN(a, b) ((a) <= (b) ? (a) : (b))
#endif



#ifndef MAIN

/*F:rowMax=vectorPeak*/
/*F:colMax=vectorPeak*/
/*F:rowMin=vectorPeak*/
/*F:colMin=vectorPeak*/
/*F:rowMaxBand=vectorPeak*/
/*F:colMaxBand=vectorPeak*/
/*F:rowMinBand=vectorPeak*/
/*F:colMinBand=vectorPeak*/
/*F:vectorPeak*

________________________________________________________________

		vectorPeak
________________________________________________________________

Name:		vectorPeak, rowMaxBand, colMaxBand, rowMinBand,
                colMinBand, rowMax, colMax, rowMin, colMin - Maximum and
                minimum of BIFF band row or column

Syntax:         | #include <xite/arithmetic.h>
                |
                | double rowMaxBand( IBAND band, int y );
                | double colMaxBand( IBAND band, int x );
                | double rowMinBand( IBAND band, int y );
                | double colMinBand( IBAND band, int x );
		|
                | double rowMax( IMAGE img, int bandno, int y );
                | double colMax( IMAGE img, int bandno, int x );
                | double rowMin( IMAGE img, int bandno, int y );
                | double colMin( IMAGE img, int bandno, int x );
		|
Description:	'rowMaxBand' returns the maximum value among the pixels in row
                number 'y' of 'band'.

		'rowMax' behaves like 'rowMaxBand', except on band number
		'bandno' in image 'img'.

		The image versions of these functions are macros calling
		the band versions. The image versions are retained for
		compatibility with previous XITE versions.

		All pixeltypes are accepted. If a band is of type
		Icomplex_typ or Id_complex_typ, the absolute values
		are used.

Return value:	The returning value is a double.

See also:       crossSection(3), crossSection(1)

Author:		Svein Bøe, BLAB, Ifi, UiO
________________________________________________________________

*/


double rowMaxBand(IBAND band, int y)
{
  double m = -(DOUBLE_MAX-1), v;
  int i;
  long size;
  
  size = Ixsize(band);

  switch((int) Ipixtyp(band)) {

  case Iu_byte_typ: {
    IUB_BAND bnd = (IUB_BAND) band;

    for (i=1; i<=size; i++) {
      v = (double) bnd[y][i];
      if (v > m) m = v;
    }
  }
    break;

  case Is_byte_typ: {
    ISB_BAND bnd = (ISB_BAND) band;

    for (i=1; i<=size; i++) {
      v = (double) bnd[y][i];
      if (v > m) m = v;
    }
  }
    break;

  case Iu_short_typ: {
    IUS_BAND bnd = (IUS_BAND) band;

    for (i=1; i<=size; i++) {
      v = (double) bnd[y][i];
      if (v > m) m = v;
    }
  }
    break;

  case Is_short_typ: {
    ISS_BAND bnd = (ISS_BAND) band;

    for (i=1; i<=size; i++) {
      v = (double) bnd[y][i];
      if (v > m) m = v;
    }
  }
    break;

  case Integer_typ: {
    II_BAND bnd = (II_BAND) band;

    for (i=1; i<=size; i++) {
      v = (double) bnd[y][i];
      if (v > m) m = v;
    }
  }
    break;

  case Ireal_typ: {
    IR_BAND bnd = (IR_BAND) band;

    for (i=1; i<=size; i++) {
      v = (double) bnd[y][i];
      if (v > m) m = v;
    }
  }
    break;

  case Idouble_typ: {
    ID_BAND bnd = (ID_BAND) band;

    for (i=1; i<=size; i++) {
      v = (double) bnd[y][i];
      if (v > m) m = v;
    }
  }
    break;

  case Icomplex_typ: {
    IC_BAND bnd = (IC_BAND) band;

    for (i=1; i<=size; i++) {
      v = (double) sqrt((double) (bnd[y][i].re * bnd[y][i].re
				  + bnd[y][i].im * bnd[y][i].im));

      if (v > m) m = v;
    }
  }
    break;

  case Id_complex_typ: {
    IDC_BAND bnd = (IDC_BAND) band;

    for (i=1; i<=size; i++) {
      v = (double) sqrt((double) (bnd[y][i].re * bnd[y][i].re
				  + bnd[y][i].im * bnd[y][i].im));

      if (v > m) m = v;
    }
  }
    break;
  }

  return(m);

} /* rowMaxBand() */



double colMaxBand(IBAND band, int x)
{
  double m = -(DOUBLE_MAX-1), v;
  int i;
  long size;
  
  size = Iysize(band);

  switch((int) Ipixtyp(band)) {

  case Iu_byte_typ: {
    IUB_BAND bnd = (IUB_BAND) band;

    for (i=1; i<=size; i++) {
      v = (double) bnd[i][x];
      if (v > m) m = v;
    }
  }
    break;

  case Is_byte_typ: {
    ISB_BAND bnd = (ISB_BAND) band;

    for (i=1; i<=size; i++) {
      v = (double) bnd[i][x];
      if (v > m) m = v;
    }
  }
    break;

  case Iu_short_typ: {
    IUS_BAND bnd = (IUS_BAND) band;

    for (i=1; i<=size; i++) {
      v = (double) bnd[i][x];
      if (v > m) m = v;
    }
  }
    break;

  case Is_short_typ: {
    ISS_BAND bnd = (ISS_BAND) band;

    for (i=1; i<=size; i++) {
      v = (double) bnd[i][x];
      if (v > m) m = v;
    }
  }
    break;

  case Integer_typ: {
    II_BAND bnd = (II_BAND) band;

    for (i=1; i<=size; i++) {
      v = (double) bnd[i][x];
      if (v > m) m = v;
    }
  }
    break;

  case Ireal_typ: {
    IR_BAND bnd = (IR_BAND) band;

    for (i=1; i<=size; i++) {
      v = (double) bnd[i][x];
      if (v > m) m = v;
    }
  }
    break;

  case Idouble_typ: {
    ID_BAND bnd = (ID_BAND) band;

    for (i=1; i<=size; i++) {
      v = (double) bnd[i][x];
      if (v > m) m = v;
    }
  }
    break;

  case Icomplex_typ: {
    IC_BAND bnd = (IC_BAND) band;

    for (i=1; i<=size; i++) {
      v = (double) sqrt((double) (bnd[i][x].re * bnd[i][x].re
				  + bnd[i][x].im * bnd[i][x].im));

      if (v > m) m = v;
    }
  }
    break;

  case Id_complex_typ: {
    IDC_BAND bnd = (IDC_BAND) band;

    for (i=1; i<=size; i++) {
      v = (double) sqrt((double) (bnd[i][x].re * bnd[i][x].re
				  + bnd[i][x].im * bnd[i][x].im));

      if (v > m) m = v;
    }
  }
    break;
  }

  return(m);

} /* colMaxBand() */



double rowMinBand(IBAND band, int y)
{
  double m = DOUBLE_MAX, v;
  int i;
  long size;

  size = Ixsize(band);

  switch((int) Ipixtyp(band)) {

  case Iu_byte_typ: {
    IUB_BAND bnd = (IUB_BAND) band;

    for (i=1; i<=size; i++) {
      v = (double) bnd[y][i];
      if (v < m) m = v;
    }
  }
    break;

  case Is_byte_typ: {
    ISB_BAND bnd = (ISB_BAND) band;

    for (i=1; i<=size; i++) {
      v = (double) bnd[y][i];
      if (v < m) m = v;
    }
  }
    break;

  case Iu_short_typ: {
    IUS_BAND bnd = (IUS_BAND) band;

    for (i=1; i<=size; i++) {
      v = (double) bnd[y][i];
      if (v < m) m = v;
    }
  }
    break;

  case Is_short_typ: {
    ISS_BAND bnd = (ISS_BAND) band;

    for (i=1; i<=size; i++) {
      v = (double) bnd[y][i];
      if (v < m) m = v;
    }
  }
    break;

  case Integer_typ: {
    II_BAND bnd = (II_BAND) band;

    for (i=1; i<=size; i++) {
      v = (double) bnd[y][i];
      if (v < m) m = v;
    }
  }
    break;

  case Ireal_typ: {
    IR_BAND bnd = (IR_BAND) band;

    for (i=1; i<=size; i++) {
      v = (double) bnd[y][i];
      if (v < m) m = v;
    }
  }
    break;

  case Idouble_typ: {
    ID_BAND bnd = (ID_BAND) band;

    for (i=1; i<=size; i++) {
      v = (double) bnd[y][i];
      if (v < m) m = v;
    }
  }
    break;

  case Icomplex_typ: {
    IC_BAND bnd = (IC_BAND) band;

    for (i=1; i<=size; i++) {
      v = (double) sqrt((double) (bnd[y][i].re * bnd[y][i].re
				  + bnd[y][i].im * bnd[y][i].im));

      if (v < m) m = v;
    }
  }
    break;

  case Id_complex_typ: {
    IDC_BAND bnd = (IDC_BAND) band;

    for (i=1; i<=size; i++) {
      v = (double) sqrt((double) (bnd[y][i].re * bnd[y][i].re
				  + bnd[y][i].im * bnd[y][i].im));

      if (v < m) m = v;
    }
  }
    break;
  }

  return(m);

} /* rowMinBand() */



double colMinBand(IBAND band, int x)
{
  double m = DOUBLE_MAX, v;
  int i;
  long size;
  
  size = Iysize(band);

  switch((int) Ipixtyp(band)) {

  case Iu_byte_typ: {
    IUB_BAND bnd = (IUB_BAND) band;

    for (i=1; i<=size; i++) {
      v = (double) bnd[i][x];
      if (v < m) m = v;
    }
  }
    break;

  case Is_byte_typ: {
    ISB_BAND bnd = (ISB_BAND) band;

    for (i=1; i<=size; i++) {
      v = (double) bnd[i][x];
      if (v < m) m = v;
    }
  }
    break;

  case Iu_short_typ: {
    IUS_BAND bnd = (IUS_BAND) band;

    for (i=1; i<=size; i++) {
      v = (double) bnd[i][x];
      if (v < m) m = v;
    }
  }
    break;

  case Is_short_typ: {
    ISS_BAND bnd = (ISS_BAND) band;

    for (i=1; i<=size; i++) {
      v = (double) bnd[i][x];
      if (v < m) m = v;
    }
  }
    break;

  case Integer_typ: {
    II_BAND bnd = (II_BAND) band;

    for (i=1; i<=size; i++) {
      v = (double) bnd[i][x];
      if (v < m) m = v;
    }
  }
    break;

  case Ireal_typ: {
    IR_BAND bnd = (IR_BAND) band;

    for (i=1; i<=size; i++) {
      v = (double) bnd[i][x];
      if (v < m) m = v;
    }
  }
    break;

  case Idouble_typ: {
    ID_BAND bnd = (ID_BAND) band;

    for (i=1; i<=size; i++) {
      v = (double) bnd[i][x];
      if (v < m) m = v;
    }
  }
    break;

  case Icomplex_typ: {
    IC_BAND bnd = (IC_BAND) band;

    for (i=1; i<=size; i++) {
      v = (double) sqrt((double) (bnd[i][x].re * bnd[i][x].re
				  + bnd[i][x].im * bnd[i][x].im));

      if (v < m) m = v;
    }
  }
    break;

  case Id_complex_typ: {
    IDC_BAND bnd = (IDC_BAND) band;

    for (i=1; i<=size; i++) {
      v = (double) sqrt((double) (bnd[i][x].re * bnd[i][x].re
				  + bnd[i][x].im * bnd[i][x].im));

      if (v < m) m = v;
    }
  }
    break;
  }

  return(m);

} /* colMinBand() */

/*F:colMaxArr=crossSection*/
/*F:rowMaxArr=crossSection*/
/*F:colMinArr=crossSection*/
/*F:rowMinArr=crossSection*/
/*F:column=crossSection*/
/*F:row=crossSection*/
/*F:colMaxArrBand=crossSection*/
/*F:rowMaxArrBand=crossSection*/
/*F:colMinArrBand=crossSection*/
/*F:rowMinArrBand=crossSection*/
/*F:columnBand=crossSection*/
/*F:rowBand=crossSection*/
/*F:crossSection*

________________________________________________________________

		crossSection
________________________________________________________________

Name:		crossSection, crossSectionBand, colMaxArrBand, rowMaxArrBand,
		colMinArrBand, rowMinArrBand, rowBand, columnBand, colMaxArr,
		rowMaxArr, colMinArr, rowMinArr, row, column - Horizontal and
		vertical cross-sections, row or column of BIFF band

Syntax:         | #include <xite/arithmetic.h>
                |
                | int colMaxArrBand( IBAND band,
                |    double** vector, long* size, double* min,
                |    double* max );
		|
                | int rowMaxArrBand( IBAND band,,
                |    double** vector, long* size, double* min,
                |    double* max );
		|
                | int colMinArrBand( IBAND band,
                |    double** vector, long* size, double* min,
                |    double* max );
		|
                | int rowMinArrBand( IBAND band,
                |    double** vector, long* size, double* min,
                |    double* max );
		|
                | int columnBand( IBAND band, int col,
                |    double** vector, long* size, double* min,
                |    double* max );
		|
                | int rowBand( IBAND band, int rowno,
                |    double** vector, long* size, double* min,
                |    double* max );
		|
		|
                | int colMaxArr( IMAGE img, int bandno,
                |    double** vector, long* size, double* min,
                |    double* max );
		|
                | int rowMaxArr( IMAGE img, int bandno,
                |    double** vector, long* size, double* min,
                |    double* max );
		|
                | int colMinArr( IMAGE img, int bandno,
                |    double** vector, long* size, double* min,
                |    double* max );
		|
                | int rowMinArr( IMAGE img, int bandno,
                |    double** vector, long* size, double* min,
                |    double* max );
		|
                | int column( IMAGE img, int bandno, int col,
                |    double** vector, long* size, double* min,
                |    double* max );
		|
                | int row( IMAGE img, int bandno, int rowno,
                |    double** vector, long* size, double* min,
                |    double* max );
		|
Description:	'colMaxArrBand' finds the horizontal cross-section of 'band',
                based on column maxima. The cross-section vector/array is
		returned in a vector which has 'size' elements and peak
		values	'min' and 'max'.

		'columnBand' finds column number 'col' from 'band'. The
		remaining parameters have the same meaning as for
		'colMaxArrBand'.

		The image versions of these functions behave like the band
		versions, except that they operate on band number 'bandno'
		in image 'img'.

		The image versions of these functions are macros calling
		the band versions. The image versions are retained for
		compatibility with previous XITE versions.

		All pixel types are accepted. If a band is of a complex
		type, absolute values are returned.

Return value:   | 0 => OK
                | 1 => Band reference is NULL
		| 2 => Illegal row or column number (for 'column' and
		|      'row')
		| 3 => Illegal pixeltype (for 'column' and 'row')

See also:       vectorPeak(3), crossSection(1), profile(3)

Author:		Svein Bøe, BLAB, Ifi, UiO
________________________________________________________________

*/

int colMaxArrBand(IBAND band, double **vector, long int *size, double *min, double *max)
{
  int i;

  if (!band) return(1);

  *max = -(DOUBLE_MAX-1);
  *min = DOUBLE_MAX;
  *size = Ixsize(band);
  *vector = (double *) malloc((unsigned) (sizeof(double) * (*size)));

  for (i=1; i<=(*size); i++) {
    (*vector)[i-1] = colMaxBand(band, i);
    if ((*vector)[i-1] > *max) *max = (*vector)[i-1];
    if ((*vector)[i-1] < *min) *min = (*vector)[i-1];
  }

  return(0);
}



int rowMaxArrBand(IBAND band, double **vector, long int *size, double *min, double *max)
{
  int i;

  if (!band) return(1);

  *max = -(DOUBLE_MAX-1);
  *min = DOUBLE_MAX;
  *size = Iysize(band);  
  *vector = (double *) malloc((unsigned) (sizeof(double) * (*size)));

  for (i=1; i<=(*size); i++) {
    (*vector)[i-1] = rowMaxBand(band, i);
    if ((*vector)[i-1] > *max) *max = (*vector)[i-1];
    if ((*vector)[i-1] < *min) *min = (*vector)[i-1];
  }

  return(0);
}



int colMinArrBand(IBAND band, double **vector, long int *size, double *min, double *max)
{
  int i;

  if (!band) return(1);

  *max = -(DOUBLE_MAX-1);
  *min = DOUBLE_MAX;
  *size = Ixsize(band);  
  *vector = (double *) malloc((unsigned) (sizeof(double) * (*size)));

  for (i=1; i<=(*size); i++) {
    (*vector)[i-1] = colMinBand(band, i);
    if ((*vector)[i-1] > *max) *max = (*vector)[i-1];
    if ((*vector)[i-1] < *min) *min = (*vector)[i-1];
  }

  return(0);
}



int rowMinArrBand(IBAND band, double **vector, long int *size, double *min, double *max)
{
  int i;

  if (!band) return(1);

  *max = -(DOUBLE_MAX-1);
  *min = DOUBLE_MAX;
  *size = Iysize(band);  
  *vector = (double *) malloc((unsigned) (sizeof(double) * (*size)));

  for (i=1; i <= (*size); i++) {
    (*vector)[i-1] = rowMinBand(band, i);
    if ((*vector)[i-1] > *max) *max = (*vector)[i-1];
    if ((*vector)[i-1] < *min) *min = (*vector)[i-1];
  }

  return(0);
}



int columnBand(IBAND band, int col, double **vector, long int *size, double *min, double *max)
{
  int i, status = 0;

  if (!band) return(1);
  if (col < 1 || col > Ixsize(band)) return(2);

  *max = -(DOUBLE_MAX-1);
  *min = DOUBLE_MAX;
  *size = Iysize(band);
  *vector = (double *) malloc((unsigned) (sizeof(double) * (*size)));

  switch((int) Ipixtyp(band)) {

  case Iu_byte_typ: {
    IUB_BAND bnd = (IUB_BAND) band;

    for (i=1; i<=(*size); i++) {
      (*vector)[i-1] = bnd[i][col];
      if ((*vector)[i-1] > *max) *max = (*vector)[i-1];
      if ((*vector)[i-1] < *min) *min = (*vector)[i-1];
    }
  }
    break;

  case Is_byte_typ: {
    ISB_BAND bnd = (ISB_BAND) band;

    for (i=1; i<=(*size); i++) {
      (*vector)[i-1] = bnd[i][col];
      if ((*vector)[i-1] > *max) *max = (*vector)[i-1];
      if ((*vector)[i-1] < *min) *min = (*vector)[i-1];
    }
  }
    break;

  case Iu_short_typ: {
    IUS_BAND bnd = (IUS_BAND) band;

    for (i=1; i<=(*size); i++) {
      (*vector)[i-1] = bnd[i][col];
      if ((*vector)[i-1] > *max) *max = (*vector)[i-1];
      if ((*vector)[i-1] < *min) *min = (*vector)[i-1];
    }
  }
    break;

  case Is_short_typ: {
    ISS_BAND bnd = (ISS_BAND) band;

    for (i=1; i<=(*size); i++) {
      (*vector)[i-1] = bnd[i][col];
      if ((*vector)[i-1] > *max) *max = (*vector)[i-1];
      if ((*vector)[i-1] < *min) *min = (*vector)[i-1];
    }
  }
    break;

  case Integer_typ: {
    II_BAND bnd = (II_BAND) band;

    for (i=1; i<=(*size); i++) {
      (*vector)[i-1] = bnd[i][col];
      if ((*vector)[i-1] > *max) *max = (*vector)[i-1];
      if ((*vector)[i-1] < *min) *min = (*vector)[i-1];
    }
  }
    break;

  case Ireal_typ: {
    IR_BAND bnd = (IR_BAND) band;

    for (i=1; i<=(*size); i++) {
      (*vector)[i-1] = bnd[i][col];
      if ((*vector)[i-1] > *max) *max = (*vector)[i-1];
      if ((*vector)[i-1] < *min) *min = (*vector)[i-1];
    }
  }
    break;

  case Idouble_typ: {
    ID_BAND bnd = (ID_BAND) band;

    for (i=1; i<=(*size); i++) {
      (*vector)[i-1] = bnd[i][col];
      if ((*vector)[i-1] > *max) *max = (*vector)[i-1];
      if ((*vector)[i-1] < *min) *min = (*vector)[i-1];
    }
  }
    break;

  case Icomplex_typ: {
    IC_BAND bnd = (IC_BAND) band;

    for (i=1; i<=(*size); i++) {
      (*vector)[i-1] = (double) sqrt((double) (bnd[i][col].re
					       * bnd[i][col].re 
					       + bnd[i][col].im
					       * bnd[i][col].im));

      if ((*vector)[i-1] > *max) *max = (*vector)[i-1];
      if ((*vector)[i-1] < *min) *min = (*vector)[i-1];
    }
  }
    break;

  case Id_complex_typ: {
    IDC_BAND bnd = (IDC_BAND) band;

    for (i=1; i<=(*size); i++) {
      (*vector)[i-1] = (double) sqrt((double) (bnd[i][col].re
					       * bnd[i][col].re 
					       + bnd[i][col].im
					       * bnd[i][col].im));

      if ((*vector)[i-1] > *max) *max = (*vector)[i-1];
      if ((*vector)[i-1] < *min) *min = (*vector)[i-1];
    }
  }
    break;

  default: status = 3;
  }

  return(status);

}



int rowBand(IBAND band, int rowno, double **vector, long int *size, double *min, double *max)
{
  int i, status = 0;

  if (!band) return(1);
  if (rowno < 1 || rowno > Iysize(band)) return(2);

  *max = -(DOUBLE_MAX-1);
  *min = DOUBLE_MAX;
  *size = Ixsize(band);
  *vector = (double *) malloc((unsigned) (sizeof(double) * (*size)));

  switch((int) Ipixtyp(band)) {

  case Iu_byte_typ: {
    IUB_BAND bnd = (IUB_BAND) band;

    for (i=1; i<=(*size); i++) {
      (*vector)[i-1] = bnd[rowno][i];
      if ((*vector)[i-1] > *max) *max = (*vector)[i-1];
      if ((*vector)[i-1] < *min) *min = (*vector)[i-1];
    }
  }
    break;

  case Is_byte_typ: {
    ISB_BAND bnd = (ISB_BAND) band;

    for (i=1; i<=(*size); i++) {
      (*vector)[i-1] = bnd[rowno][i];
      if ((*vector)[i-1] > *max) *max = (*vector)[i-1];
      if ((*vector)[i-1] < *min) *min = (*vector)[i-1];
    }
  }
    break;

  case Iu_short_typ: {
    IUS_BAND bnd = (IUS_BAND) band;

    for (i=1; i<=(*size); i++) {
      (*vector)[i-1] = bnd[rowno][i];
      if ((*vector)[i-1] > *max) *max = (*vector)[i-1];
      if ((*vector)[i-1] < *min) *min = (*vector)[i-1];
    }
  }
    break;

  case Is_short_typ: {
    ISS_BAND bnd = (ISS_BAND) band;

    for (i=1; i<=(*size); i++) {
      (*vector)[i-1] = bnd[rowno][i];
      if ((*vector)[i-1] > *max) *max = (*vector)[i-1];
      if ((*vector)[i-1] < *min) *min = (*vector)[i-1];
    }
  }
    break;

  case Integer_typ: {
    II_BAND bnd = (II_BAND) band;

    for (i=1; i<=(*size); i++) {
      (*vector)[i-1] = bnd[rowno][i];
      if ((*vector)[i-1] > *max) *max = (*vector)[i-1];
      if ((*vector)[i-1] < *min) *min = (*vector)[i-1];
    }
  }
    break;

  case Ireal_typ: {
    IR_BAND bnd = (IR_BAND) band;

    for (i=1; i<=(*size); i++) {
      (*vector)[i-1] = bnd[rowno][i];
      if ((*vector)[i-1] > *max) *max = (*vector)[i-1];
      if ((*vector)[i-1] < *min) *min = (*vector)[i-1];
    }
  }
    break;

  case Idouble_typ: {
    ID_BAND bnd = (ID_BAND) band;

    for (i=1; i<=(*size); i++) {
      (*vector)[i-1] = bnd[rowno][i];
      if ((*vector)[i-1] > *max) *max = (*vector)[i-1];
      if ((*vector)[i-1] < *min) *min = (*vector)[i-1];
    }
  }
    break;

  case Icomplex_typ: {
    IC_BAND bnd = (IC_BAND) band;

    for (i=1; i<=(*size); i++) {
      (*vector)[i-1] = (double) sqrt((double) (bnd[rowno][i].re
					       * bnd[rowno][i].re 
					       + bnd[rowno][i].im
					       * bnd[rowno][i].im));

      if ((*vector)[i-1] > *max) *max = (*vector)[i-1];
      if ((*vector)[i-1] < *min) *min = (*vector)[i-1];
    }
  }
    break;

  case Id_complex_typ: {
    IDC_BAND bnd = (IDC_BAND) band;

    for (i=1; i<=(*size); i++) {
      (*vector)[i-1] = (double) sqrt((double) (bnd[rowno][i].re
					       * bnd[rowno][i].re 
					       + bnd[rowno][i].im
					       * bnd[rowno][i].im));

      if ((*vector)[i-1] > *max) *max = (*vector)[i-1];
      if ((*vector)[i-1] < *min) *min = (*vector)[i-1];
    }
  }
    break;

  default: status = 3;
  }

  return(status);

}



/*F:curveToBand*

________________________________________________________________

		curveToBand
________________________________________________________________

Name:		curveToBand - Make BIFF image band visualize 1-D curve

Syntax:		| #include <xite/arithmetic.h>
		| 
		| int curveToBand(IBAND band, double *curve,
		|    double minval, double maxval, double scale,
		|    double cur, double fg, double bg);
		| 
Description:	Let 'band' visualize the curve in 'curve', with pixel-values
                below the curve given by 'fg', pixel-values on the curve given
		by 'cur' and pixel-values above the curve given by 'bg'.

		'band' can be any non-complex pixeltype. 'minval' and 'maxval'
		hold the extreme values of 'curve' or values where the curve
		should be clipped (values outside this range will be displayed
		as 'minval' or 'maxval'). 'scale' determines the visual peak of
		'curve' in 'band', relative to the height of 'band'.

See also:	vectorPeak(3), crossSection(3), crossSection(1)

Restrictions:	'band' can not be complex or double complex.

Return value:	| 0 : ok
                | 1 : Band reference is NULL
                | 2 : Illegal pixeltype

Author:		Svein Bøe, BLAB, Ifi, UiO

Id: 		$Id$
________________________________________________________________

*/

int curveToBand(IBAND band, double *curve, double minval, double maxval, double scale, double cur, double fg, double bg)
{
  long x, y, curPix, height, width, val;
  IPIXTYP pt;
  int status = 0;

  if (!band) return(1);

  width  = Ixsize(band);
  height = Iysize(band);
  pt     = Ipixtyp(band);

  /* Fill curve into band. */

  if (minval > maxval) {
    drawBand(band, (double) bg, 0.0);
    return(status);
  }

  switch((int) pt) {

  case Iu_byte_typ: {
    IUB_BAND bnd = (IUB_BAND) band;

    for (x=1; x<=width; x++) {
      curve[x-1] = MAX(curve[x-1], minval);
      curve[x-1] = MIN(curve[x-1], maxval);

      if (minval != maxval) 
	curPix = (long)
	  (height + 1 - (scale * ((long) ( (curve[x-1]-minval) * (height-1) /
					  (maxval-minval) )) + 1));
      else 
	curPix = (long) (height + 1 - scale * height);

      val = MAX(curPix, 1);
      /* Below curve */
      for (y=height; y>val; y--)
	bnd[y][x] = (UNS_BYTE) fg;
      
      if (val >= 1 && val <= height)
	/* Curve pixel */
	bnd[val][x] = (UNS_BYTE) cur;

      val = MIN(curPix, height);
      /* Above curve */
      for (y=val-1; y>=1; y--)
	bnd[y][x] = (UNS_BYTE) bg;
    } /* for x */
  }
    break;

  case Is_byte_typ: {
    ISB_BAND bnd = (ISB_BAND) band;

    for (x=1; x<=width; x++) {
      curve[x-1] = MAX(curve[x-1], minval);
      curve[x-1] = MIN(curve[x-1], maxval);

      if (minval != maxval) 
	curPix = (long)
	  (height + 1 - (scale * ((long) ( (curve[x-1]-minval) * (height-1) /
					  (maxval-minval) )) + 1));
      else 
	curPix = (long) (height + 1 - scale * height);
      
      val = MAX(curPix, 1);
      /* Below curve */
      for (y=height; y>val; y--)
	bnd[y][x] = (SIGN_BYTE) fg;
      
      if (val >= 1 && val <= height)
	/* Curve pixel */
	bnd[val][x] = (SIGN_BYTE) cur;

      val = MIN(curPix, height);      
      /* Above curve */
      for (y=val-1; y>=1; y--)
	bnd[y][x] = (SIGN_BYTE) bg;
    } /* for x */
  }
    break;

  case Iu_short_typ: {
    IUS_BAND bnd = (IUS_BAND) band;

    for (x=1; x<=width; x++) {
      curve[x-1] = MAX(curve[x-1], minval);
      curve[x-1] = MIN(curve[x-1], maxval);

      if (minval != maxval) 
	curPix = (long)
	  (height + 1 - (scale * ((long) ( (curve[x-1]-minval) * (height-1) /
					  (maxval-minval) )) + 1));
      else 
	curPix = (long) (height + 1 - scale * height);

      val = MAX(curPix, 1);      
      /* Below curve */
      for (y=height; y>val; y--)
	bnd[y][x] = (UNS_SHORT) fg;

      if (val >= 1 && val <= height)      
	/* Curve pixel */
	bnd[val][x] = (UNS_SHORT) cur;
      
      val = MIN(curPix, height);
      /* Above curve */
      for (y=val-1; y>=1; y--)
	bnd[y][x] = (UNS_SHORT) bg;
    } /* for x */
  }
    break;

  case Is_short_typ: {
    ISS_BAND bnd = (ISS_BAND) band;

    for (x=1; x<=width; x++) {
      curve[x-1] = MAX(curve[x-1], minval);
      curve[x-1] = MIN(curve[x-1], maxval);

      if (minval != maxval) 
	curPix = (long)
	  (height + 1 - (scale * ((long) ( (curve[x-1]-minval) * (height-1) /
					  (maxval-minval) )) + 1));
      else 
	curPix = (long) (height + 1 - scale * height);
      
      val = MAX(curPix, 1);
      /* Below curve */
      for (y=height; y>val; y--)
	bnd[y][x] = (SIGN_SHORT) fg;

      if (val >= 1 && val <= height)      
	/* Curve pixel */
	bnd[val][x] = (SIGN_SHORT) cur;

      val = MIN(curPix, height);      
      /* Above curve */
      for (y=val-1; y>=1; y--)
	bnd[y][x] = (SIGN_SHORT) bg;
    } /* for x */
  }
    break;

  case Integer_typ: {
    II_BAND bnd = (II_BAND) band;

    for (x=1; x<=width; x++) {
      curve[x-1] = MAX(curve[x-1], minval);
      curve[x-1] = MIN(curve[x-1], maxval);

      if (minval != maxval) 
	curPix = (long)
	  (height + 1 - (scale * ((long) ( (curve[x-1]-minval) * (height-1) /
					  (maxval-minval) )) + 1));
      else 
	curPix = (long) (height + 1 - scale * height);

      val = MAX(curPix, 1);      
      /* Below curve */
      for (y=height; y>val; y--)
	bnd[y][x] = (INTEGER) fg;
      
      if (val >= 1 && val <= height)
	/* Curve pixel */
	bnd[val][x] = (INTEGER) cur;

      val = MIN(curPix, height);      
      /* Above curve */
      for (y=val-1; y>=1; y--)
	bnd[y][x] = (INTEGER) bg;
    } /* for x */
  }
    break;

  case Ireal_typ: {
    IR_BAND bnd = (IR_BAND) band;

    for (x=1; x<=width; x++) {
      curve[x-1] = MAX(curve[x-1], minval);
      curve[x-1] = MIN(curve[x-1], maxval);

      if (minval != maxval) 
	curPix = (long)
	  (height + 1 - (scale * ((long) ( (curve[x-1]-minval) * (height-1) /
					  (maxval-minval) )) + 1));
      else 
	curPix = (long) (height + 1 - scale * height);

      val = MAX(curPix, 1);      
      /* Below curve */
      for (y=height; y>val; y--)
	bnd[y][x] = (REAL) fg;

      if (val >= 1 && val <= height)      
	/* Curve pixel */
	bnd[val][x] = (REAL) cur;

      val = MIN(curPix, height);      
      /* Above curve */
      for (y=val-1; y>=1; y--)
	bnd[y][x] = (REAL) bg;
    } /* for x */
  }
    break;

  case Idouble_typ: {
    ID_BAND bnd = (ID_BAND) band;

    for (x=1; x<=width; x++) {
      curve[x-1] = MAX(curve[x-1], minval);
      curve[x-1] = MIN(curve[x-1], maxval);

      if (minval != maxval) 
	curPix = (long)
	  (height + 1 - (scale * ((long) ( (curve[x-1]-minval) * (height-1) /
					  (maxval-minval) )) + 1));
      else 
	curPix = (long) (height + 1 - scale * height);

      val = MAX(curPix, 1);      
      /* Below curve */
      for (y=height; y>val; y--)
	bnd[y][x] = (DOUBLE) fg;

      if (val >= 1 && val <= height)      
	/* Curve pixel */
	bnd[val][x] = (DOUBLE) cur;
      
      val = MIN(curPix, height);
      /* Above curve */
      for (y=val-1; y>=1; y--)
	bnd[y][x] = (DOUBLE) bg;
    } /* for x */
  }
    break;

  default: status = 2;
  } /* switch */

  return(status);

} /* curveToBand() */

#endif /* not MAIN */

/*P:crossSection*

________________________________________________________________

		crossSection
________________________________________________________________

Name:		crossSection - Find cross sections, row or column of image

Syntax:		crossSection [<option>...] <inimage> <outimage>

Description:    'crossSection' reads a biff image and finds a cross-section,
                row or column. The output is an image with a band for each
		band of	the input image. Each output band displays a curve
		for the	chosen cross-section, row or column. The values of the
		pixels on the curve, below or above the curve can be chosen. 

		All pixeltypes are accepted. The pixeltype of each output
		band is the same as for the corresponding input band, unless
		the option '-pt' is used. For bands of a complex type, real or
		double bands are returned with absolute values of the input
		bands.

		To process only some of the bands in 'inimage', use the
		standard XITE sub-band notation.

Options:	An <option> is one of three kinds: <type>
		(at most one), <dir> (at most one) and <opt>.

		A <type> option may be one of

		&-M
		cross-section maximum (default)

		&-m
		cross-section minimum

		&-n num
		pick row or column number 'num'


		-&A <dir> option may be one of

		&-y
		y (vertical/column) direction (default)

		&-x
		x (horizontal/row) direction

		-&An <opt> option is chosen from

		&-t title
		Title of output image

		&-h height
		Height (ysize) of resulting band

		&-fg fg
		Value of area below curve (double)

		&-bg bg
		Value of area above curve (double)

		&-cur cur
		Value of curve pixels (double)

		&-s scale
		Scale curve height relative to image height (default is 1.0,
		which means that the curve peak will touch the top edge of the
		image)

		&-a
		Print curve values to stdout

		&-pt pixeltype
		Let all output bands be of type 'pixeltype'. Default unsigned
		byte.
                | ub or  3 => unsigned byte
                | sb or  4 => signed byte
                | us or  5 => unsigned short
                | ss or  6 => signed short
                | i  or  7 => integer
                | r  or  8 => real
                | d  or 10 => double

See also:       profile(1), crossSection(3)

Return value:   2 => Illegal arguments.

Examples:       Find the row maxima (maximum for each row) of an image:

                | crossSection -x -M /local/xite/img/mona.img out.img

Author:		Svein Bøe, BLAB, Ifi, UiO

Id: 		$Id$
________________________________________________________________

*/

#ifdef MAIN

int main(int argc, char **argv)
{
  int mini, maxi, xdir, ydir, status, x, num, height, b, ascOut;
  int fieldWidth, nItems;
  long size;
  IMAGE img, out_img;
  IBAND inband, outband;
  double *vector, minPeak, maxPeak, fg, bg, cur, scale;
  char *outFile, *title, *options, *printFormat, fieldType[2], *pta;
  IPIXTYP pt;

  Iset_message(1);
  Iset_abort(1);
  InitMessage(&argc, argv, xite_app_std_usage_text(
    "Usage: %s [<option>...] <inimage> <outimage>\n\
       where <option> is chosen from\n\
         -M          : Cross-section maximum (default) \n\
	 -m          : Cross-section minimum\n\
         -n <num>    : Pick row or column number <num> \n\
         -y          : y (vertical/column) direction (default) \n\
	 -x          : x (horizontal/row) direction.\n\
         -t <title>  : Title of output image\n\
	 -h <height> : Height (ysize) of resulting band\n\
	 -fg <fg>    : Value of area below curve (double)\n\
	 -bg <bg>    : Value of area above curve (double)\n\
	 -cur <cur>  : Value of curve pixels (double)\n\
         -s <scale>  : Scale curve height relative to image height\n\
         -a          : Print curve values to stdout\n\
         -pt <pixtyp>: Pixeltype of all output bands\n"));

  /* Concatenize options in *argv, for use by Ihistory. */

  if (argc == 1) Usage(1, NULL);
  options = argvOptions(argc, argv);
  
  if (argc < 3) Usage(2, "Illegal number of arguments.\n");

  mini   = read_bswitch(&argc, argv, "-m");
  maxi   = read_bswitch(&argc, argv, "-M");
  num    = read_iswitch(&argc, argv, "-n", -1);
  title  = read_switch (&argc, argv, "-t", 1, "");
  xdir   = read_bswitch(&argc, argv, "-x");
  ydir   = read_bswitch(&argc, argv, "-y");
  height = read_iswitch(&argc, argv, "-h", 512);
  fg     = read_dswitch(&argc, argv, "-fg", 255.0);
  bg     = read_dswitch(&argc, argv, "-bg", 0.0);
  cur    = read_dswitch(&argc, argv, "-cur", 255.0);
  scale  = read_dswitch(&argc, argv, "-s", 1.0);
  ascOut = read_bswitch(&argc, argv, "-a");
  pta    = read_switch( &argc, argv, "-pt", 1, "ub");

  pt = IparsePixtyp(pta);
  if (pt < Iu_byte_typ || pt == Icomplex_typ || pt >= Id_complex_typ)
    Usage(2, "Illegal pixeltype.\n");

  if (scale > 1.0 || scale < 0.0)
    Usage(2, "Scale must be between 0.0 and 1.0 inclusive.\n");

  if ( (mini && maxi) || (mini && (num != -1)) || (maxi && (num != -1)) )
    Usage(2, "Conflicting options");
  if (xdir && ydir) Usage(2, "Conflicting options");
  if (!(mini || maxi || (num != -1))) maxi = 1;
  if (!(xdir || ydir)) ydir = 1;
  if (argc != 3) Usage(2, "Illegal number of arguments.\n");

  img = Iread_image(argv[1]);
  outFile = argv[2];
  if (strlen(title) == 0) {
    title = (char *) malloc(sizeof(char) * 81);
    title[0] = '\0';
    if (mini && xdir)
      strcat(title, "Row minima");
    else if (mini)
      strcat(title, "Column minima");
    else if (maxi && xdir)
      strcat(title, "Row maxima");
    else if (maxi)
      strcat(title, "Column maxima");
    else if (xdir) {
      strcat(title, "Row number ");
      sprintf(title + strlen(title), "%d", num);
    } else {
      strcat(title, "Column number ");
      sprintf(title + strlen(title), "%d", num);
    }
  } /* strlen == 0 */
  
  out_img = Init_image(Inbands(img), title);

  for (b=1; b<=Inbands(img); b++) {

    /* Extract information from one band. */

    inband  = img[b];

    if (mini && xdir)
      status = rowMinArrBand(inband, &vector, &size, &minPeak, &maxPeak);
    else if (mini)
      status = colMinArrBand(inband, &vector, &size, &minPeak, &maxPeak);
    else if (maxi && xdir)
      status = rowMaxArrBand(inband, &vector, &size, &minPeak, &maxPeak);
    else if (maxi)
      status = colMaxArrBand(inband, &vector, &size, &minPeak, &maxPeak);
    else if (xdir)
      status = rowBand(inband, num, &vector, &size, &minPeak, &maxPeak);
    else status = columnBand(inband, num, &vector, &size, &minPeak, &maxPeak);
    
    if (status == 1) {
      Error(2, "Illegal band number %d.\n", b);
    }
    else if (status == 2) {
      Error(2, "Illegal row or column number %d.\n", num);
    }

    if (ascOut) printFormat = malloc(80);

    /* Make output band for result. */

    if (pt != Iunknown_typ)
      outband = Imake_band(pt, size, height);
    else if (Ipixtyp(inband) == Icomplex_typ ||
	     Ipixtyp(inband) == Id_complex_typ)
      outband = Imake_band(Idouble_typ, size, height);
    else outband = Imake_band(Ipixtyp(inband), size, height);

    out_img[b] = outband;
    curveToBand(outband, vector, minPeak, maxPeak, scale, cur, fg, bg);

    if (ascOut) {
      /* Format control for printing. */
      switch( (int) Ipixtyp(inband)) {
      case Ibit_typ:
      case Ibit2_typ:
      case Inibble_typ:
      case Iu_byte_typ:
      case Is_byte_typ:
      case Iu_short_typ:
      case Is_short_typ:
      case Integer_typ: {
	int maxi;

	(void) strcpy(fieldType, "d");

	maxi = MAX(abs(minPeak), abs(maxPeak));
	if (maxi == 0) fieldWidth = 2;
	else {
	  fieldWidth = (int) log10((double) maxi) + 2;
	}
      }
	break;
      default:
	(void) strcpy(fieldType, "g");
	fieldWidth = 15;
	break;
      } /* switch */
      
      sprintf(printFormat, "%%%d%s", fieldWidth, fieldType);

      nItems = (int) 120 / (fieldWidth + 1);

      if (Ipixtyp(inband) <= Integer_typ) {
	
	for (x=1; x <= size; x++) {
	  printf(printFormat, (int) vector[x-1]);
	  if (x % (nItems - 1) == 0) printf("\n");
	}
      } else {
	for (x=1; x <= size; x++) {
	  printf(printFormat, vector[x-1]);
	  if (x % (nItems - 1) == 0) printf("\n");
	}
      }
      if ((x - 1) % (nItems - 1) != 0) printf("\n");
    }
  } /* for b */
  
  Icopy_text(img, out_img);
  Ihistory(out_img, argv[0], options);
  Iwrite_image(out_img, outFile);
  
  return(0);
}

#endif /* MAIN */
