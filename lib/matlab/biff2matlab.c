

/*C*

________________________________________________________________

        biff2matlab
        $Id$
        Copyright 1994, Blab, UiO
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

#include <matlab/mat.h>

#include <xite/includes.h>
#include <xite/matlab.h>
#ifndef mxMAXNAM
# define mxMAXNAM 20
#endif
#include <xite/message.h>
#include XITE_STRING_H

static char *biff2matlab_error_s[] = {
"OK\n",
"Failure in creating MATLAB matrix.\n",
"Failure in converting BIFF band to MATLAB matrix.\n",
"Failure in opening MATLAB matrix file.\n",
"Failure in writing MATLAB matrix to file.\n"
};



/*F:band2matlab*

________________________________________________________________

		band2matlab
________________________________________________________________

Name:		band2matlab - Convert from XITE BIFF band to MATLAB matrix.
Syntax:         | #include <xite/matlab.h>
                |
                | int band2matlab( IBAND band, mxArray* matrix );
		|
Description:    Converts the pixel data in the XITE BIFF band 'band'
                into the MATLAB matrix 'matrix'. 'band' may consist of
                any of the following pixel types:
		UNS_BYTE, SIGN_BYTE, UNS_SHORT, SIGN_SHORT, INTEGER, REAL,
		COMPLEX, DOUBLE, DCOMPLEX
Return value:   0 => ok
Restrictions:   Storage for the MATLAB matrix must be allocated prior to
                calling this routine.
Author:		Svein Bøe, BLAB, Ifi, UiO
Id: 		$Id$
________________________________________________________________

*/

int band2matlab(IBAND band, mxArray *matrix)
{
/*
    MATLAB matrices are column-ordered, while BIFF bands are row-ordered.
*/

    long i, j, xsize, ysize;
    IPIXTYP pixTyp;
    IUB_BAND iub_band;
    ISB_BAND isb_band;
    IUS_BAND ius_band;
    ISS_BAND iss_band;
    II_BAND ii_band;
    IR_BAND ir_band;
    IC_BAND ic_band;
    ID_BAND id_band;
    IDC_BAND idc_band;

    pixTyp = Ipixtyp(band);
    xsize = Ixsize(band);
    ysize = Iysize(band);

#   define CONVERT(INN_BAND_TYPE, INN_BAND) \
    {   INN_BAND = (INN_BAND_TYPE) band; \
        for (i = 1; i <= ysize; i++) \
          for (j = 1; j <= xsize; j++) \
      	    mxGetPr(matrix)[i + (j - 1) * ysize - 1] \
              = (double) INN_BAND[i][j]; \
    }

#   define C_CONVERT(INN_BAND_TYPE, INN_BAND) \
    {   INN_BAND = (INN_BAND_TYPE) band; \
        for (i = 1; i <= ysize; i++) \
        for (j = 1; j <= xsize; j++) { \
  	  mxGetPr(matrix)[i + (j - 1) * ysize - 1] = \
	    (double) INN_BAND[i][j].re; \
	  mxGetPi(matrix)[i + (j - 1) * ysize - 1] = \
	    (double) INN_BAND[i][j].im; \
         } \
    }

    switch((int) pixTyp) {
      case Iu_byte_typ:
	CONVERT(IUB_BAND, iub_band);
	break;
      case Is_byte_typ:
	CONVERT(ISB_BAND, isb_band);
	break;
      case Iu_short_typ:
	CONVERT(IUS_BAND, ius_band);
	break;
      case Is_short_typ:
	CONVERT(ISS_BAND, iss_band);
	break;
      case Integer_typ:
	CONVERT(II_BAND, ii_band);
	break;
      case Ireal_typ:
	CONVERT(IR_BAND, ir_band);
	break;
      case Icomplex_typ:
	C_CONVERT(IC_BAND, ic_band);
	break;
      case Idouble_typ:
	CONVERT(ID_BAND, id_band);
	break;
      case Id_complex_typ:
	C_CONVERT(IDC_BAND, idc_band);
	break;
    }

    return(Iok);

} /* band2matlab */



/*L:matrixName*

________________________________________________________________

		matrixName
________________________________________________________________

Name:		matrixName - Generate name of MATLAB matrix from name of
                XITE BIFF image or MATLAB filename.

Syntax:		| static char *matrixName( char *name, IMAGE img,
                |    char *matFileName, long bandNo );

Description:    Generate name(s) of MATLAB matrix (matrices) from name of
                BIFF image or MATLAB filename.

		If the BIFF image has a non-empty title, the name (names)
		is (are) generated from it (with a filename extension for
		multiband images). If the BIFF image has no non-empty title,
		the name(s) is (are) created from the MATLAB matrix filename
		(and band number). With an empty filename, the name becomes
		the empty string.

		A MATLAB matrix name must start with a letter and only
		consist of letters, digits or underscores (a total of
		maximum 19 characters). An illegal initial character is
		replaced by the letter "a", any other character is replaced
		by an underscore.
Restrictions:   Storage must be allocated for 'name' prior to calling this
                routine.
Return value:   Pointer to MATLAB matrix name
Author:		Svein Bøe, BLAB, Ifi, UiO
Id: 		$Id$
________________________________________________________________

*/

static char *matrixName(char *name, IMAGE img, char *matFileName, long bandNo)
{
    char *imgName, extension[80], *t, *legalChars;
    int len;

    imgName = Ititle(img);
    /* Remove any trailing blanks, otherwise MATLAB will choke on loading
       the file */
    if (strlen(imgName) > strcspn(imgName, " ")) {
	imgName[strcspn(imgName, " ")] = '\0';
    }

    /* Make sure the name is not an ampty string or a stupid stdout filename */
    if (strlen(imgName) == 0 && strlen(matFileName) > 0) {
	imgName = matFileName;
    } else if (strlen(imgName) == 0 && strlen(matFileName) == 0) 
    {
      name[0] = '\0';
      return(name);
    }

    /* Make sure that first character is a letter, and substitute anything but
       letters, digits and underscores with an underscore */

    legalChars =
      "abcdefghijklmnopqrstuvwxyzæøåABCDEFGHIJKLMNOPQRSTUVWXYZÆØÅ";
    if (strspn(imgName, legalChars) < 1) imgName[0] = 'a';

    legalChars =
      "abcdefghijklmnopqrstuvwxyzæøåABCDEFGHIJKLMNOPQRSTUVWXYZÆØÅ0123456789_";
    t = imgName;
    while (1) {
      len = strspn(t, legalChars);
      if (len == strlen(t)) break;
      else {
        t[len] = '_';
	t += len;
      }
    }

    (void) strcpy(name, imgName);

    if (Inbands(img) > 1) {
        /* Add band-extension */
	(void) strcat(name, "_");
	(void) sprintf(extension, "%ld", bandNo);
	(void) strcat(name, extension);
    }

    /* Limit length to maximum MATLAB name-string length */
    if (strlen(name) > mxMAXNAM) name[mxMAXNAM] = '\0';

    return(name);

} /* matrixName */



/*F:biff2matlab*

________________________________________________________________

		biff2matlab
________________________________________________________________

Name:		biff2matlab - Convert all bands of XITE BIFF image to MATLAB
                matrices.
Syntax:         | #include <xite/matlab.h>
                |
                | int biff2matlab( IMAGE img, long* bandNo, long nbands,
                |    mxArray** matrices, char* matFileName );

Description:    Convert the bands of a BIFF image 'img' (band numbers given
                by 'bandNo') to MATLAB matrices. Be aware that this routine
		may require lots of memory. It may be better to convert one
		band at a time if you simply want to store them in a file
		(see 'band2matlab(3)' and 'saveBiffAsMatlab(3)').

		If the BIFF image has a non-empty title, the MATLAB matrix
		(or matrices) is (are) named after it (with an extension for
		multiband images). If the BIFF image has no non-empty title,
		the MATLAB matrix name is created from the MATLAB matrix
		filename (and band number). With filename "", the name
		is not set.

		A MATLAB matrix name must start with a letter and only
		consist of letters, digits and underscores (a total of
		maximum mxMAXNAM characters). An illegal initial character is
		replaced by the letter "a", any other character is replaced
		by an underscore.
Restrictions:   Memory for the matrices will be allocated by the routine,
		but the pointer array 'matrices' must be allocated prior to
		the call.
Return value:   | 0 => OK
                | 1 => Failure in creating MATLAB matrix
		| 2 => Failure in converting BIFF image to MATLAB matrix
See also:	'band2matlab(3)', 'saveBiffAsMatlab(3), 'matlab2biff(3)'
Author:		Svein Bøe, BLAB, Ifi, UiO
Id: 		$Id$
________________________________________________________________

*/


int biff2matlab(IMAGE img, long *bandNo, long nbands, mxArray **matrices, char *matFileName)
{
    long i, n;
    IPIXTYP pixTyp;
    char matName[80];
    mxArray *matrix;

    for (i = 1; i <= nbands; i++)
    {
      n = bandNo[i-1];
      pixTyp = Ipixtyp(img[n]);
      if (pixTyp == Icomplex_typ || pixTyp == Id_complex_typ) {
	if ((matrix =
	     mxCreateDoubleMatrix(Iysize(img[n]), Ixsize(img[n]), mxCOMPLEX))
	    == NULL)
	  return(Error(1, biff2matlab_error_s[1]));
      } else {
	if ((matrix =
	     mxCreateDoubleMatrix(Iysize(img[n]), Ixsize(img[n]), mxREAL))
	    == NULL)
	  return(Error(1, biff2matlab_error_s[1]));
      }

      (void) matrixName(matName, img, matFileName, n);
      if (strlen(matName) > 0) mxSetName(matrix, matName);
      if (band2matlab(img[n], matrix) != Iok)
	return(Error(2, biff2matlab_error_s[2]));

      *matrices++ = matrix;
    }

    return(Iok);

} /* biff2matlab */




/*F:saveBiffAsMatlab*

________________________________________________________________

		saveBiffAsMatlab
________________________________________________________________

Name:		saveBiffAsMatlab - Save XITE BIFF image in MATLAB matrix file.

Syntax:         | #include <xite/matlab.h>
                |
                | int saveBiffAsMatlab( IMAGE img, char* matFileName );

Description:    Convert the XITE BIFF image 'img' to MATLAB matrices and
                save to MAT-file for MATLAB version 5.

		If the BIFF image has a title, the MATLAB matrix (or matrices)
		is (are) named after it (with an extension for multiband
		images). If the BIFF image has no title, the MATLAB matrix
		name is created from the MATLAB matrix filename (and band
		number). With filename "", the name is not set.

		A MATLAB matrix name must start with a letter and only
		consist of letters, digits and underscores (a total of
		maximum 19 characters). An illegal initial character is
		replaced by the letter "a", any other character is replaced
		by an underscore.

		If the MATLAB matrix file exists, the matrix is appended to
		the file (unless a matrix with the same name already exists,
		in which case the new matrix replaces the old).

Return value:   | 0 => OK
		| 1 => Failure in creating MATLAB matrix
		| 2 => Failure in converting BIFF band to MATLAB matrix
                | 3 => Failure in opening MATLAB matrix file
		| 4 => Failure in writing MATLAB matrix to file

Author:		Svein Bøe, BLAB, Ifi, UiO

Id: 		$Id$
________________________________________________________________

*/


int saveBiffAsMatlab(IMAGE img, char *matFileName)
{
    mxArray *matrix;
    MATFile *matFile;
    long nbands, bandNo;
    IPIXTYP pixTyp;
    char matName[80];

    nbands = Inbands(img);

    if ((matFile = matOpen(matFileName, "w")) == NULL)
      return(Error(3, biff2matlab_error_s[3]));

    /* Process each BIFF band */

    for (bandNo = 1; bandNo <= nbands; bandNo++) {
	pixTyp = Ipixtyp(img[bandNo]);
	if (pixTyp == Icomplex_typ || pixTyp == Id_complex_typ) {
	    if ((matrix = mxCreateDoubleMatrix((int) Iysize(img[bandNo]),
					       (int) Ixsize(img[bandNo]),
					       mxCOMPLEX))
		== NULL)
		return(Error(1, biff2matlab_error_s[1]));
	} else {
	    if ((matrix = mxCreateDoubleMatrix((int) Iysize(img[bandNo]),
					       (int) Ixsize(img[bandNo]),
					       mxREAL))
		== NULL)
		return(Error(1, biff2matlab_error_s[1]));
	}

	(void) matrixName(matName, img, matFileName, bandNo);
	if (strlen(matName) > 0) mxSetName(matrix, matName);
	if(band2matlab(img[bandNo], matrix) != Iok)
	  return(Error(2, biff2matlab_error_s[2]));
	if (matPutArray(matFile, matrix) != Iok)
	  return(Error(4, biff2matlab_error_s[4]));

	mxDestroyArray(matrix);
    }
    matClose(matFile);
    return(Iok);

} /* saveBiffAsMatlab */
