
/*C*

________________________________________________________________

        matlab2biff
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

#include <mat.h>

#include <xite/includes.h>
#include <xite/matlab.h>
#include <xite/message.h>
#include <xite/readarg.h>
#include XITE_MALLOC_H
#include XITE_STRING_H

static char *matlab2biff_error_s[] = {
  "OK\n",
  "Error\n",
  "Illegal command line.\n",
  "Failure in opening the MAT-file.\n",
  "Failure in reading the MAT-file directory.\n",
  "Did not find all requested matrices in file.\n",
  "Failure in initializing BIFF image.\n",
  "Failure in creating BIFF image band.\n",
  "Failure in converting MATLAB matrix to XITE BIFF band.\n",
  "Failure in opening BIFF image file.\n",
  "Failure in writing BIFF image band to file.\n"
};



/*F:matlab2band*

________________________________________________________________

		matlab2band
________________________________________________________________

Name:		matlab2band - Convert from MATLAB matrix to XITE BIFF band

Syntax:         | #include <xite/matlab.h>
                |
		| int matlab2band( const mxArray *matrix, IBAND band );

Description:    Converts the numeric data in the MATLAB matrix
                'matrix' into the XITE BIFF band 'band'. 'band' will get
                pixel type double or double complex.

Return value:   0 => ok

Restrictions:   Storage for 'band' must be allocated prior to calling this
                routine.

See also:	'matlab2biff(3)', 'saveMatlabAsBiff(3)', 'band2matlab(3)'.

Author:		Svein Bøe, BLAB, Ifi, UiO

Id: 		$Id$
________________________________________________________________

*/

int matlab2band(const mxArray *matrix, IBAND band)
{
/*
    MATLAB matrices are column-ordered, BIFF bands are row-ordered.
*/

    long i, j, xsize, ysize;
    IPIXTYP pixTyp;
    ID_BAND id_band;
    IDC_BAND idc_band;

    pixTyp = (mxIsComplex(matrix) ? Id_complex_typ : Idouble_typ);
    xsize = (long) mxGetN(matrix);
    ysize = (long) mxGetM(matrix);

#   define CONVERT(OUT_BAND_TYPE, OUT_BAND) \
    {   OUT_BAND = (OUT_BAND_TYPE) band; \
        for (i = 1; i <= ysize; i++) \
          for (j = 1; j <= xsize; j++) \
      	    OUT_BAND[i][j] = mxGetPr(matrix)[i + (j - 1) * ysize - 1]; \
    }

#   define C_CONVERT(OUT_BAND_TYPE, OUT_BAND) \
    {   OUT_BAND = (OUT_BAND_TYPE) band; \
        for (i = 1; i <= ysize; i++) \
        for (j = 1; j <= xsize; j++) { \
  	  OUT_BAND[i][j].re = mxGetPr(matrix)[i + (j - 1) * ysize - 1]; \
	  OUT_BAND[i][j].im = mxGetPi(matrix)[i + (j - 1) * ysize - 1]; \
         } \
    }

    switch((int) pixTyp) {
      case Id_complex_typ:
	C_CONVERT(IDC_BAND, idc_band);
	break;
      case Idouble_typ:
	CONVERT(ID_BAND, id_band);
	break;
    }

    return(Iok);

} /* matlab2band */



/*L:biffName*

________________________________________________________________

		biffName
________________________________________________________________

Name:		biffName - Generate title for XITE BIFF image from MATLAB
                matrix names or BIFF filename

Syntax:		| static char *biffName( char *name,
                |    const mxArray **matrices,
                |    char *fileName, long nMat );

Description:    Generate title of XITE BIFF image from MATLAB matrix names or
                BIFF filename.

		The first non-empty matrix name is used, or else the BIFF
		filename (if non-empty), or else the empty string.

Restrictions:   Storage must be allocated for 'name' prior to the call.

Return value:   Pointer to BIFF matrix name

Author:		Svein Bøe, BLAB, Ifi, UiO

Id: 		$Id$
________________________________________________________________

*/

static char *biffName(char *name, const mxArray **matrices, char *fileName, long nMat)
{
  char *matrixName;
  long i;

  matrixName = "\0";
  i = 1;
  while (strlen(matrixName) == 0 && i <= nMat)
  {
    matrixName = (char *) mxGetName(matrices[i-1]);
    i++;
  }

  /* Remove any trailing blanks */
  if (strlen(matrixName) > strcspn(matrixName, " ")) {
      matrixName[strcspn(matrixName, " ")] = '\0';
  }

  /* Check for an empty string and a decent output filename */
  if (strlen(matrixName) == 0 && strlen(fileName) > 0) {
      matrixName = fileName;
  }

  if (strlen(matrixName) == 0)
    name[0] = '\0';
  else (void) strcpy(name, matrixName);

  return(name);

} /* biffName */



/*F:matlab2biff*

________________________________________________________________

		matlab2biff
________________________________________________________________

Name:		matlab2biff - Convert several MATLAB matrices to a multi-band
                XITE BIFF image

Syntax:         | #include <xite/matlab.h>
                |
                | int matlab2biff( IMAGE img,
                |    long* bandNo, long nbands,
                |    const mxArray** matrices, char* biffFileName );

Description:    Convert the 'matrices' given by 'bandNo' to a multi-band BIFF
                image. Memory for the bands will be allocated by the routine.
		The pixeltype of the BIFF bands will be double or double
		complex.

		The BIFF image title will be the same as the first non-empty
		MATLAB matrix name, or else the BIFF filename (if non-empty),
		or else the empty string.

Return value:   | 0 => OK
                | 1 => Failure in creating BIFF band.
		| 2 => Failure in converting MATLAB matrix to BIFF band.

See also:       'biff2matlab(3)', 'saveBiffAsMatlab(3)'.

Author:		Svein Bøe, BLAB, Ifi, UiO

Id: 		$Id$
________________________________________________________________

*/

int matlab2biff(IMAGE img, long *bandNo, long nbands, const mxArray **matrices, char *biffFileName)
{
    long i;
    IPIXTYP pixTyp;
    char imageName[80];

    for (i = 0; i < nbands; i++)
    {
      pixTyp = (mxIsComplex(matrices[i]) ? Id_complex_typ : Idouble_typ);
      if ((img[bandNo[i]] = Imake_band(pixTyp, mxGetN(matrices[i]),
			       mxGetM(matrices[i]))) == NULL)
	return(Error(1, matlab2biff_error_s[7]));
      
      if (matlab2band(matrices[i], img[bandNo[i]]) != Iok)
	return(Error(2, matlab2biff_error_s[8]));
    }

    (void) biffName(imageName, matrices, biffFileName, nbands);
    if (strlen(imageName) > 0) Iset_title(img, imageName);

    return(Iok);

} /* matlab2biff */



/*F:saveMatlabAsBiff*

________________________________________________________________

		saveMatlabAsBiff
________________________________________________________________

Name:		saveMatlabAsBiff - Save MATLAB matrices in XITE BIFF image file

Syntax:         | #include <xite/matlab.h>
                |
                | int saveMatlabAsBiff( const mxArray** matrices,
                |    long nMat, char* biffFileName,
                |    char* historyText );

Description:    Convert the 'nMat' MATLAB matrices 'matrices' to XITE BIFF
                image and save to file. The pixeltype of the BIFF bands will
		be double or double complex.

		The BIFF image title will be the same as the name of the first
		MATLAB matrix with a non-empty name string, or else the BIFF
		filename (if non-empty), or else the empty string.

		The historyText argument is used as the second arguemnt in a
		call to 'Ihistory(5)'.

Return value:   | 0 => OK
                | 1 => Failure in initializing BIFF image.
		| 2 => Failure in creating BIFF image band.
		| 3 => Failure in converting MATLAB matrix to 
		|      XITE BIFF band.

Author:		Svein Bøe, BLAB, Ifi, UiO

Id: 		$Id$
________________________________________________________________

*/

int saveMatlabAsBiff(const mxArray **matrices, long nMat, char *biffFileName, char *historyText)
{
  long bandNo;
  IPIXTYP pixTyp;
  char imageName[80];
  IMAGE img;

  (void) biffName(imageName, matrices, biffFileName, nMat);

  if ((img = Init_image(nMat, imageName)) == NULL)
    return(Error(1, matlab2biff_error_s[6]));

  /* Process each BIFF band */

  for (bandNo = 1; bandNo <= nMat; bandNo++) {
      pixTyp = (mxIsComplex(matrices[bandNo-1]) ? 
		Id_complex_typ : Idouble_typ);
      if ((img[bandNo] = Imake_band(pixTyp, mxGetN(matrices[bandNo-1]),
			       mxGetM(matrices[bandNo-1]))) == NULL)
	return(Error(2, matlab2biff_error_s[7]));

      if (matlab2band(matrices[bandNo-1], img[bandNo]) != Iok)
	return(Error(3, matlab2biff_error_s[8]));
  }

  Ihistory(img, historyText, "");
  Iwrite_image(img, biffFileName);

  return(Iok);

} /* saveMatlabAsBiff */




/*F:readMatrices*

________________________________________________________________

		readMatrices
________________________________________________________________

Name:		readMatrices - Read MATLAB matrices from file

Syntax:         | #include <xite/matlab.h>
                |
                | int readMatrices( mxArray*** ptrMatrices,
                |    long* nMat, char* matNames[], long nNames,
                |    char* fileName );

Description:    Read from file the 'nNames' numeric matrices with names given
                by 'matNames' (or all if 'nNames' == 0). Return the matrices
		in 'matrices' and the number of successfully read matrices in
		'nMat'.

		Will read both MATLAB version 4 and version 5 MAT-files.

Restrictions:	Will not read sparse matrices. Memory will be allocated for
                the pointer array 'ptrMatrices'	as well as for the matrices.

Return value:   0 => OK
                1 => Failure in opening the matrix file.
		2 => Failure in reading the file matrix-directory.
		3 => Did not find all requested matrices in file.

Author:		Svein Bøe, Ifi, UiO
________________________________________________________________

*/

int readMatrices(mxArray ***ptrMatrices, long *nMat, char *matNames[], long nNames, char *fileName)
{
  MATFile *matFile;
  mxArray **matrices;
  mxClassID id;
  long i;

  if ((matFile = matOpen(fileName, "r")) == NULL)
    return(Error(1, matlab2biff_error_s[3]));

  if (nNames == 0) {
    /* Read all numeric matrices */

    matNames = NULL;
    if ((matNames = matGetDir(matFile, (int *) &nNames)) == NULL)
      return(Error(2, matlab2biff_error_s[4]));
  }

  matrices = *ptrMatrices =
    (mxArray **) mxCalloc((size_t) nNames, sizeof(mxArray *));

  *nMat = 0;
  while (*nMat < nNames &&
	 (matrices[*nMat] = matGetArray(matFile, matNames[*nMat]))
	 != NULL) {
    if (!mxIsNumeric(matrices[*nMat]) ||
	((id = mxGetClassID(matrices[*nMat])) == mxCELL_CLASS) ||
	id == mxSTRUCT_CLASS ||
	id == mxOBJECT_CLASS ||
	id == mxCHAR_CLASS   ||
	id == mxSPARSE_CLASS ||
	id == mxUNKNOWN_CLASS)
      mxDestroyArray(matrices[*nMat]);
    else (*nMat)++;
  }

  /* (*nMat) gives the number of matrices read successfully */

  matClose(matFile);

  for (i = *nMat; i < nNames; i++) {
    mxFree(&(matrices[i]));
  }

  if (nNames > 0 && *nMat < nNames) return(Error(3, matlab2biff_error_s[5]));
  else return(Iok);

} /* readMatrices */
