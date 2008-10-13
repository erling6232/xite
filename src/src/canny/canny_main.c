/* Right is granted to use this software to customers and organizations,
   subject to the terms and conditions set out in their software distribution
   agreement with the University of British Columbia.  No right is granted
   to use or distribute this software in the absence of such an agreement.
*/

/* Program to detect edges in an image using the local
   maxima of the gradient of the Gaussian smoothed image.

   From M.Sc. thesis of John Francis Canny (MIT-TR720) */



/*F:canny_filters=canny*/
/*F:canny_scale=canny*/
/*F:canny*

________________________________________________________________

                canny
________________________________________________________________

Name:           canny, canny_filters, canny_scale -  detect edges in image
                using Canny operator

Syntax:         | #include <xite/canny.h>
		|
                | int canny( IBAND input, IBAND magnitude,
                |    IBAND angle, IBAND laplace, int lngth,
                |    int h1[], int h2[], int h3[], double scale,
                |    int thin, int verbose );
		|
                | int canny_filters( float sigma, int k, int h1[],
                |    int h2[], int h3[], int verbose );
		|
                | double canny_scale( double sigma, int k );

Description:    'canny' performs the canny edge detector on
                the input image 'input'. Output from the function
		are the edge 'magnitude', the edge 'angle' and 
		the 'laplace'. The filter is specified by a
		gaussian function 'h1', the first derivative 'h2',
		and the second derivative 'h3', all of length 'lngth'.
		The outputs are scaled down with a factor 'scale'.
		If 'thin' is true, thinning of the edge is performed.
		The function may be silent or 'verbose'.

		'canny_filters' computes the filters 'h1', 'h2', and 'h3'.
		based on 'sigma' and 'k'. The function returns the filter 
		length. You may try sigma=1.0 .. 7.0 and k=200 .. 500.
		John Canny has used 2.0 and 450 as default values.

		'canny_scale' returns a default scale value based on
		'sigma' and magnification 'k'.

Restrictions:   The current implementation has a maximum filter length 
                of 50, and a maximum input image width of 1024.
		
		All the band parameters in 'canny' must have pixel type
		unsigned byte.

Return value:   &For canny_scale
                The default scale value.

		&For canny_filters
		Filter length.

		&For canny
		| 0 : Ok
		| 1 : Bad pixeltype
		| 2 : Filter length too large
		| 3 : Input image too small compared to filter
		|     length

Reference:      John Canny: A computational Approach to Edge Detection.
                IEEE Transactions on Pattern analysis and Machine Int.
		Vol. PAMI-8 No 6 Nov. 1986

Author:         | Paul Kloschinsky
                | Modified for XITE by Otto Milvang and Svein Bøe.

________________________________________________________________

*/



#include <math.h>
#include <stdlib.h>
#include <xite/includes.h>
#include <xite/biff.h>
#include XITE_STDIO_H
#include <xite/readarg.h>
#include <xite/message.h>
#include <xite/canny.h>

/*P:canny*

________________________________________________________________

                canny
________________________________________________________________

Name:           canny - detect edges in image using Canny operator

Syntax:         | canny [<option>...] <input-image> <output-image>

Description:    'canny' computes the canny edge strength, angle
                and/or laplacian of an input image. 
		The program uses two separable filters to implement the 
		Gaussian operator and therefore is of the order
		of 2*n, where 'n' is the filter length.

Options:	&-s sigma
		The value of 'sigma' to be used for computing the filters, 
		where 'sigma' is a real value (default 2.0).

		&-w diam
		This is an alternative way of specifying sigma in terms 
		of the diameter of pixels in the center region of the 
		gaussian mask. The value of 'sigma' is computed from 'diam'
		by the formula: sigma =  diam/2*sqrt(2).

		&-k K
		This specifies the scale constant 'K' to be used in 
		computing the filters. The default value should do for 
		most applications, but special values may be 
		occasionally required. Increasing 'K' increases filter 
		length and accuracy, but it also increases the chance 
		of overflow. The default value is 450.

		&-m, -magnitude
		Compute edge magnitude.

		&-a, -angle
		Compute edge angle.

		&-l, -laplace
		Compute laplace.

		&-t, -thin
		Perform thinning on the magnitude image. 

Restrictions:   The current implementation has a maximum filter length 
                of 50, and a maximum input image width of 1024.		

		Only UNS_BYTE pixels are supported

Reference:      John Canny: A computational Approach to Edge Detection.
                IEEE Transactions on Pattern analysis and Machine Int.
		Vol. PAMI-8 No 6 Nov. 1986

Examples:       | canny  mona.img edge.img
                | canny -m -s 5 -t mona.img - | threshold - - 50 | xshow

See also:       canny(3)

Author:         | Paul Kloschinsky
                | Modified for XITE by Otto Milvang and Svein Bøe

Id:             $Id$
________________________________________________________________

*/

int main(int argc, char **argv)
{
  IMAGE inimg, outimg;
  int currb, ncols, mag, ang, lap, obands, bnr, thin;
  char *args;
  int h1[CANNY_MAXLN], h2[CANNY_MAXLN], h3[CANNY_MAXLN];
  int lngth, K, stat;
  float sigma, sclefact;
 
  InitMessage(&argc, argv, xite_app_std_usage_text(
    "Usage: %s [<option>...] <infile> <outfile>\n\
        where <option> is one of\n\
        -k K     : Scale constant for computing filters \n\
        -s sigma : Sigma for computing filters \n\
        -w diam  : Diameter of pixels in center region of gaussian mask \n\
        -a       : Compute edge angle \n\
        -m       : Compute edge magnitude \n\
        -l       : Compute edge laplace \n\
        -t       : Thinning the magnitude image \n"));

  if (argc == 1) Usage(1, NULL);
  args = argvOptions(argc, argv);

  K     = read_iswitch(&argc, argv, "-k", 450);
  sigma = read_dswitch(&argc, argv, "-s", 2.0);
  sigma = read_dswitch(&argc, argv, "-w", sigma * 2.0 * sqrt(2.0))/
          (2.0*sqrt(2.0));
  mag   = read_bswitch(&argc, argv, "-m") || 
          read_bswitch(&argc, argv, "-magnitude"); 
  ang   = read_bswitch(&argc, argv, "-a") || 
          read_bswitch(&argc, argv, "-angle"); 
  lap   = read_bswitch(&argc, argv, "-l") || 
          read_bswitch(&argc, argv, "-laplace"); 
  thin  = read_bswitch(&argc, argv, "-t") || 
          read_bswitch(&argc, argv, "-thin"); 
  if (!mag && !ang && !lap) mag = ang = 1; 

  obands = 0;
  if (mag) obands++;
  if (ang) obands++;
  if (lap) obands++;
  
  /* Compute two separable filters */
  
  lngth = canny_filters(sigma, K, h1, h2, h3, Verbose());
  
  if (lngth > CANNY_MAXLN) Error(2, "Filter length too long (%d).\n", lngth);
  
  inimg = Iread_image(argv[1]);
  if (inimg == NULL)
    Error(2, "canny: can't open image file \"%s\".\n", argv[1]);
  
  if (argc != 3) Usage(2, "Illegal number of arguments.\n");

  outimg = Imake_image(obands * Inbands(inimg), Ititle(inimg), Iu_byte_typ,
			Ixsize(inimg[1]), Iysize(inimg[1]));
  
  if (outimg == NULL) Error(2, "Can't allocate memory for result.\n");
  
  Ihistory(outimg, "canny", args);
  
  sclefact = canny_scale(sigma, K);

  bnr = 1;
  for(currb=1; currb <= Inbands(inimg); currb++) {
    ncols = Ixsize(inimg[currb]);
    if (ncols > CANNY_MAXWDTH) 
      Error(2, "image too wide(%d)\n",ncols);
    
    stat = canny(inimg[currb], 
		 mag ? outimg[bnr++] : NULL,
		 ang ? outimg[bnr++] : NULL,
		 lap ? outimg[bnr++] : NULL,
		 lngth, h1, h2, h3, sclefact, thin, Verbose());

    if (stat != 0) {
      Warning(2, "%s%d%s\n", "No result from input band ", currb,
	      " in output.");
      if (mag) bnr--;
      if (ang) bnr--;
      if (lap) bnr--;
    }
  }
  Iset_nbands(&outimg, bnr-1);

  if (Verbose())
    printf("canny: filter-length is %d, sigma is %f.\n", lngth, sigma);

  if (Inbands(outimg) > 0) Iwrite_image(outimg, argv[2]);
  else Error(1, "No output generated, failed for all input bands.\n");

  return(0);
}
