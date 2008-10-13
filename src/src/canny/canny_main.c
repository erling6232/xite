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

#ifndef MAIN

#define SIGN(i) (i<0?-1:1)
#define sqr(i) ((double)(i)*(double)(i))
#define flabs(i) ((double) abs(i))

static int intbuff_s[3][CANNY_MAXLN][CANNY_MAXWDTH]; /* Intermediate buffers */
static int gradbuff_s[4][3][CANNY_MAXWDTH]; /* Buffers for d/dx & d/dy d2/dx2 d2/dy2*/

static int pixmax_s;
static unsigned char *inptr_s, *outptr_s, *rptr_s, *lptr_s;

#ifndef FUNCPROTO
static int getpix()
#else /* FUNCPROTO */
static int getpix(void)
#endif /* FUNCPROTO */
{
  return(*inptr_s++);
}

#ifndef FUNCPROTO
static void putpix(val, rval, lval)
int val, rval, lval;
#else /* FUNCPROTO */
static void putpix(int val, int rval, int lval)
#endif /* FUNCPROTO */
{
  unsigned char p;
  if (val < 0) val = 0; else if (val > 255) val=255;
  p = val;
  if (outptr_s) *outptr_s++ = p;
  if (rval < 0) rval = 0; else if (rval > 255) rval=255;
  p = rval;
  if (rptr_s) *rptr_s++ = p;
  if (lval < -120) lval = -120; else if (lval > 120) lval=120;
  p = lval + 128;
  if (lptr_s) *lptr_s++ = p;
}


#ifndef FUNCPROTO
double canny_scale(sig, k)
  int k;
  double sig;
#else /* FUNCPROTO */
double canny_scale(double sig, int k)
#endif /* FUNCPROTO */
{
  return((double) k*k*2.0*3.1415*sqr(sig));
}


#ifdef FUNCPROTO
int canny_filters(float sig, int k, int h1[], int h2[], int h3[], int verbose)
#else
int canny_filters(sig, k, h1, h2, h3, verbose)
float sig;
int k;
int h1[], h2[], h3[];
int verbose;
#endif
{
  int tmp1[CANNY_MAXLN], tmp2[CANNY_MAXLN], tmp3[CANNY_MAXLN];
  int ln;
  register int i,j;
  
  i = 0;
  do {
    float  tmp=2.0;
    
    tmp =  -((i*i)/(tmp*(sig*sig)));
    
    tmp1[i] = (int) (k*exp(tmp));
    tmp2[i] = (int) (k*(i*exp(tmp)));
    tmp3[i] = (int) ((double) tmp1[i]*i*i/sig/sig -tmp1[i]);
  }		 
  while (tmp1[i++] > 0);
  
  i--;
  ln = (2*i--)-1;
  for (j=0;j<=i;j++) {
    h1[i-j] = h1[i+j]   = tmp1[j];
    h2[i+j] = -(h2[i-j] = tmp2[j]);
    h3[i-j] = h3[i+j]   = tmp3[j];
  }
  if (verbose) {
    printf("%s\n%s\n", "canny filters:",
	   "     index         h1         h2         h3");
    for(i=0; i<ln; i++)
      printf("%10d %10d %10d %10d\n", i, h1[i], h2[i], h3[i]);
  }
  
  return(ln);

} /* canny_filters() */

/* Routine to supress all non-maximum gradient values in the
   gradient direction. Uses a 9 pixel neighborhood in 'gradbuff_s'
   for each point. This technique is described in Canny's thesis. */

static unsigned char atg_s[257] = {   0,
     0,  0,  0,  0,  0,  0,  1,  1,  1,  1,  1,  1,  2,  2,  2,  2,
     2,  2,  3,  3,  3,  3,  3,  3,  3,  4,  4,  4,  4,  4,  4,  5,
     5,  5,  5,  5,  5,  6,  6,  6,  6,  6,  6,  6,  7,  7,  7,  7,
     7,  7,  8,  8,  8,  8,  8,  8,  8,  9,  9,  9,  9,  9,  9,  9,
    10, 10, 10, 10, 10, 10, 11, 11, 11, 11, 11, 11, 11, 12, 12, 12,
    12, 12, 12, 12, 13, 13, 13, 13, 13, 13, 13, 14, 14, 14, 14, 14,
    14, 14, 15, 15, 15, 15, 15, 15, 15, 15, 16, 16, 16, 16, 16, 16,
    16, 17, 17, 17, 17, 17, 17, 17, 17, 18, 18, 18, 18, 18, 18, 18,
    19, 19, 19, 19, 19, 19, 19, 19, 20, 20, 20, 20, 20, 20, 20, 20,
    20, 21, 21, 21, 21, 21, 21, 21, 21, 22, 22, 22, 22, 22, 22, 22,
    22, 22, 23, 23, 23, 23, 23, 23, 23, 23, 23, 24, 24, 24, 24, 24,
    24, 24, 24, 24, 25, 25, 25, 25, 25, 25, 25, 25, 25, 26, 26, 26,
    26, 26, 26, 26, 26, 26, 26, 27, 27, 27, 27, 27, 27, 27, 27, 27,
    27, 27, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 29, 29, 29, 29,
    29, 29, 29, 29, 29, 29, 29, 29, 30, 30, 30, 30, 30, 30, 30, 30,
    30, 30, 30, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 32 };

#define IND(a, b ) (a < 65536) ? ((a<<8) /b  )  : (a/(b>>8))

#define ANG(sum, sum1, sum2)\
      if (sum1 > 0 && sum2 >= 0)\
        sum = sum1>sum2 ? atg_s[ IND(sum2, sum1) ] : \
          63 - atg_s[ IND(sum1, sum2) ]; else\
      if (sum1 <= 0 && sum2 > 0)\
        sum = -sum1>sum2 ? 127 - atg_s[ IND(sum2, (-sum1)) ] :\
          64 + atg_s[ IND((-sum1), sum2) ]; else\
      if (sum1 < 0 && sum2 <= 0)\
        sum = -sum1>-sum2 ? 128 + atg_s[ IND((-sum2), (-sum1)) ] :\
          191 - atg_s[ IND((-sum1), (-sum2))]; else\
      if (sum1 >= 0 && sum2 < 0) \
        sum = sum1>-sum2 ? 255 - atg_s[ IND((-sum2), sum1) ] :\
          192 + atg_s[ IND(sum1, (-sum2)) ];

#ifndef FUNCPROTO
static void nonmax_supress(iff,nc,row,fct, thin)
IMAGE iff;
int nc, row, thin;
float fct;
#else /* FUNCPROTO */
static void nonmax_supress(IMAGE iff, int nc, int row, float fct, int thin)
#endif /* FUNCPROTO */
{
  int pix, dir, lap, dx, dy, dx2, dy2;
  double ux, uy, ul, G, G1, G2, Ga, Gb, Gc, Gd;
  int j;
  int midrow, toprow;
  
  pix = 0;
  putpix(pix, pix, pix); 	/* pad */
  /* find top and middle row of 'gradbuff_s' */
  if (row > 0)
    midrow = row-1;
  else
    midrow = 2;
  if (midrow > 0)
    toprow = midrow-1;
  else
    toprow = 2;
  
  
  for (j=1;j<(nc-1);j++) {
    /* get partial derivatives */
    
    dx = gradbuff_s[1][midrow][j];
    dy = gradbuff_s[0][midrow][j];
    dx2 = gradbuff_s[2][midrow][j];
    dy2 = gradbuff_s[3][midrow][j];
    ux = (double) dx;
    uy = (double) dy;
    ux /= fct;
    uy /= fct;
    
    pix = (int) (sqrt(ux*ux+uy*uy));
    dx = -dx;
    ANG(dir, dx, dy);                
    ul =  (double) (dx2 + dy2)/fct;
    lap = ul;
    dx = -dx;
    if (thin && dx !=0 && dy !=0)
      {
	if (abs(dy) > abs(dx)) {	
	  ux = (flabs(dx)/flabs(dy));
	  uy = 1.0;
	  
	  /* get non-diagonal values */ 
	  Gb = sqrt(sqr(gradbuff_s[0][toprow][j])+
		    sqr(gradbuff_s[1][toprow][j]));
	  Gd = sqrt(sqr(gradbuff_s[0][row][j])+
		    sqr(gradbuff_s[1][row][j]));
	  
	  
	  if (SIGN(dx) != SIGN(dy)) { /* get diagonals */
	    Ga = sqrt(sqr(gradbuff_s[0][toprow][j+1])+
		      sqr(gradbuff_s[1][toprow][j+1]));
	    Gc = sqrt(sqr(gradbuff_s[0][row][j-1])+
		      sqr(gradbuff_s[1][row][j-1]));
	  }	
	  else {
	    Ga = sqrt(sqr(gradbuff_s[0][toprow][j-1])+
		      sqr(gradbuff_s[1][toprow][j-1]));
	    Gc = sqrt(sqr(gradbuff_s[0][row][j+1])+
		      sqr(gradbuff_s[1][row][j+1]));
	  }	
	}
	else {
	  ux = (flabs(dy)/flabs(dx));
	  uy = 1.0;
	  
	  /* get non-diagonal values */ 
	  Gb = sqrt(sqr(gradbuff_s[0][midrow][j+1])+
		    sqr(gradbuff_s[1][midrow][j+1]));
	  Gd = sqrt(sqr(gradbuff_s[0][midrow][j-1])+
		    sqr(gradbuff_s[1][midrow][j-1]));
	  
	  if (SIGN(dx) != SIGN(dy)) { /* get diagonals */
	    Ga = sqrt(sqr(gradbuff_s[0][toprow][j+1])+
		      sqr(gradbuff_s[1][toprow][j+1]));
	    Gc = sqrt(sqr(gradbuff_s[0][row][j-1])+
		      sqr(gradbuff_s[1][row][j-1]));
	  }	
	  else {
	    Ga = sqrt(sqr(gradbuff_s[0][row][j+1])+
		      sqr(gradbuff_s[1][row][j+1]));
	    Gc = sqrt(sqr(gradbuff_s[0][toprow][j-1])+
		      sqr(gradbuff_s[1][toprow][j-1]));
	  }
	}
	
	
	
	
	/* Interpolate */ 
	G1 = (ux * Ga)+((uy-ux) * Gb);
	G2 = (ux * Gc)+((uy-ux) * Gd);
	
	G = sqr(dx)+sqr(dy);
	if (G < 0)
	  fprintf(stderr,"canny: overflow.\n");
	G = sqrt(G);
	if (G > G1 && G > G2) 
	  { /* Gradient is at maximum */
	    pix = (long) (G/fct);
	    if (pix < 1)
	      pix = 1;
	    if (pix > pixmax_s) 
	      {
		fprintf(stderr,"canny: pix lowered to %d\n",pixmax_s);
		pix = pixmax_s;
	      }
	} else pix = 0;
      }		      
    putpix(pix, dir, lap);
  } /* for j */
  
  pix = 0;
  putpix(pix, pix, pix);
} /* nonmax_supress() */

#ifndef FUNCPROTO
static void nextrow(n,k)
int *n,k;
#else /* FUNCPROTO */
static void nextrow(int *n, int k)
#endif /* FUNCPROTO */
{
  if (!(++*n < k)) *n = 0;
}

#ifndef FUNCPROTO
static void getrow(iff, nc, bf)
IMAGE iff;
int nc;
int bf[];
#else /* FUNCPROTO */
static void getrow(IMAGE iff, int nc, int *bf)
#endif /* FUNCPROTO */
{
  int i;
  
  for (i=0;i<nc;i++)
    bf[i] = getpix();
}
   
#ifndef FUNCPROTO
static void applyrowfilt(ctr,filt,nc,buff,row,intbf)
int ctr, row, nc, intbf;
int filt[],buff[];
#else /* FUNCPROTO */
static void applyrowfilt(int ctr, int *filt, int nc, int *buff, int row, int intbf)
#endif /* FUNCPROTO */
{
  register int i,j;
  int curval=0;
  
  for (i=0;i<nc;i++) {
    curval = (filt[ctr]*buff[i]);

    for (j=1; j<=i && j<=ctr; j++)
      curval += (filt[ctr-j]*buff[i-j]);

    for (j=1; j<=(nc-i-1) && j<=ctr; j++)
      curval += (filt[ctr+j]*buff[i+j]);
    
    intbuff_s[intbf][row][i] = curval;
  }
  
}

#ifndef FUNCPROTO
static int applycolfilt(ln,filt,col,crstrt,intbf)
int ln, col, crstrt, intbf;
int filt[];
#else /* FUNCPROTO */
static int applycolfilt(int ln, int *filt, int col, int crstrt, int intbf)
#endif /* FUNCPROTO */
{
  register int j;
  int indx;
  int curval=0;
  
  for (indx=crstrt,j=0;j<ln;j++,nextrow(&indx,ln)) 
    curval += (filt[j]*intbuff_s[intbf][indx][col]);
  
  return(curval);
}


#define GAUSS 0
#define GRADIENT 1
#define LAPLACE 2

#ifndef FUNCPROTO
int canny(input, magnitude, angle, laplace, lngth, h1, h2, h3, scale, thin, verbose)
IBAND input, magnitude, angle, laplace;
int lngth;
int h1[], h2[], h3[];
double scale;
int thin, verbose;
#else /* FUNCPROTO */
int canny(IBAND input, IBAND magnitude, IBAND angle, IBAND laplace, int lngth, int *h1, int *h2, int *h3, double scale, int thin, int verbose)
#endif /* FUNCPROTO */
{
  int ncols, nrows;
  int cntr, i, j, curstrt, gradrow;
  double bpp = 8.0;
  int buff[CANNY_MAXWDTH], deriv, pix;

  if (Ipixtyp(input) != Iu_byte_typ)
    return(Warning(1, "Input pixel type must be unsigned byte.\n"));
  if (magnitude && Ipixtyp(magnitude) != Iu_byte_typ)
    return(Warning(1, "Magnitude pixel type must be unsigned byte.\n"));
  if (angle && Ipixtyp(angle) != Iu_byte_typ)
    return(Warning(1, "Angle pixel type must be unsigned byte.\n"));
  if (laplace && Ipixtyp(laplace) != Iu_byte_typ)
    return(Warning(1, "Laplace pixel type must be unsigned byte.\n"));

  inptr_s  = &input[1][1];
  outptr_s = magnitude ? &magnitude[1][1] : NULL;
  rptr_s   = angle     ? &angle[1][1]     : NULL;
  lptr_s   = laplace   ? &laplace[1][1]   : NULL;

  ncols = Ixsize(input);
  nrows = Iysize(input);
  if (ncols > CANNY_MAXWDTH) 
    return(Warning(2, "canny: filter length too long (%d)\n",lngth));
  
  if (nrows < lngth || ncols < lngth)
    return(Warning(3, "%s%d%s%d%s%d\n",
		   "Not enough image rows(", nrows, ") or cols(", ncols,
		   "), filter length is ", lngth));
     
  cntr = (lngth - 1) / 2;

  pixmax_s = (int) (exp(bpp*log(2.0))-1);
  
  for (i = 0; i < lngth; i++) {	                   /* Initialize buffers */
    getrow(NULL,ncols,buff);
    applyrowfilt(cntr,h1,ncols,buff,i,GAUSS);
    applyrowfilt(cntr,h2,ncols,buff,i,GRADIENT);
    applyrowfilt(cntr,h3,ncols,buff,i,LAPLACE); 
  }
  
  pix = 0;			           /* Pad output */ 
  for (i = 0; i <= cntr; i++)
    for (j = 0; j < ncols; j++)
      putpix(pix, pix, pix);
  
  curstrt = 0;      			   /* Main Loop */	
  gradrow = 0; 
  for (i=0;i<=(nrows-lngth);i++) {
    for (j=0;j<ncols;j++) {
      /* compute d/dy */
      deriv = applycolfilt(lngth,h2,j,curstrt,0);
      gradbuff_s[0][gradrow][j] = deriv;
      /* compute d/dx */
      deriv = applycolfilt(lngth,h1,j,curstrt,1);
      gradbuff_s[1][gradrow][j] = deriv;
      /* compute d2/dx2 */
      deriv = applycolfilt(lngth,h1,j,curstrt,2);
      gradbuff_s[2][gradrow][j] = deriv;
      /* compute d2/dx2 */
      deriv = applycolfilt(lngth,h3,j,curstrt,0);
      gradbuff_s[3][gradrow][j] = deriv;
    }
    
    if (i > 1)
      nonmax_supress(NULL,ncols,gradrow,scale, thin);
    
    if (i < (nrows-lngth)) {
      getrow(NULL,ncols,buff);
      applyrowfilt(cntr,h1,ncols,buff,curstrt,0);
      applyrowfilt(cntr,h2,ncols,buff,curstrt,1);
      
      nextrow(&curstrt,lngth);
      nextrow(&gradrow,3);
    }
  }
  
  pix = 0;			           /* Pad output */ 
  for (i = 0; i <= cntr; i++)
    for (j = 0; j < ncols; j++)
      putpix(pix, pix, pix);
  
  return(0);
}

#endif /* not MAIN */

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

#ifdef MAIN

#ifndef FUNCPROTO
int main(argc,argv)
int argc;
char *argv [];
#else /* FUNCPROTO */
int main(int argc, char **argv)
#endif /* FUNCPROTO */
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

#endif /* MAIN */
