

/*

________________________________________________________________

        bin_wr1.c
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


/*F:bin_wr1*

________________________________________________________________
 
                bin_wr1
________________________________________________________________


Name:           bin_wr1 - binarize a gray-scale band using White and
                Rohrer's first method.

Syntax:         | #include <xite/binarize.h>
                |
                | int bin_wr1( IBAND inband, IBAND outband,
                |    int x_lookahead, int y_lookahead,
                |    int bias_mode, int bias_factor,
                |    int f_factor, int g_factor,
                |    int intermediate_images );

Description:    The input band 'inband' is binarized using White and Rohrer`s
                first method. The 'x_lookahead' and 'y_lookahead' parameters
		gives the number of lookahead pixels used in the biased
		running average that is used in deciding the threshold at
		each pixel location. Good values for 'x_lookahead' and 
		'y_lookahead' are 8 and 1, respectively.

Restrictions:   'inband' and 'outband' must have pixel type unsigned byte.

See also:       binarize(1)

References:	&'James M. White & Gene D. Rohrer'
                "Image Thresholding for	Optical Character Recognition and
		Other Applications Requiring Character Image Extraction",
		IBM Journal of Research and Development,
		vol 27, no 4, pp  400-411, July 1983. 
		
Return value:   | 0 : ok
                | 1 : Bad input pixel type
		| 2 : Bad output pixel type

Files:          src/binarize/bin_wr1.c
Author:         Øivind Due Trier
Id:             $Id$
________________________________________________________________

*/
#include <math.h>
#include <xite/includes.h>
#include <xite/biff.h>
#ifdef HAVE_STDIO_H
 #include <stdio.h>
#endif
#ifdef HAVE_MALLOC_H
 #include <malloc.h>
#endif
#include <xite/blab.h>
#include <xite/histo.h>
#include <xite/binarize.h>
#include <xite/message.h>
#include "write_band.c"


#define WR1_F_OFFSET 255
#define WR1_G_OFFSET 255
static int wr1_f_tab[512] = 
{
   -62,  -62,  -61,  -61,  -60,  -60,  -59,  -59,
   -58,  -58,  -57,  -57,  -56,  -56,  -54,  -54,
   -53,  -53,  -52,  -52,  -51,  -51,  -50,  -50,
   -49,  -49,  -48,  -48,  -47,  -47,  -46,  -46,
   -45,  -45,  -44,  -44,  -43,  -43,  -42,  -42,
   -41,  -41,  -41,  -41,  -40,  -40,  -39,  -39,
   -38,  -38,  -37,  -37,  -36,  -36,  -36,  -36,
   -35,  -35,  -34,  -34,  -33,  -33,  -33,  -33,
   -32,  -32,  -31,  -31,  -31,  -31,  -30,  -30,
   -29,  -29,  -29,  -29,  -28,  -28,  -27,  -27,
   -27,  -27,  -26,  -26,  -25,  -25,  -25,  -25,
   -24,  -24,  -24,  -24,  -23,  -23,  -23,  -23,
   -22,  -22,  -22,  -22,  -21,  -21,  -21,  -21,
   -20,  -20,  -20,  -20,  -19,  -19,  -19,  -19,
   -18,  -18,  -18,  -18,  -17,  -17,  -17,  -17,
   -16,  -16,  -16,  -16,  -16,  -16,  -15,  -15,
   -15,  -15,  -14,  -14,  -14,  -14,  -14,  -14,
   -13,  -13,  -13,  -13,  -13,  -13,  -12,  -12,
   -12,  -12,  -12,  -12,  -11,  -11,  -11,  -11,
   -11,  -11,  -10,  -10,  -10,  -10,  -10,  -10,
   -9,   -9,   -9,   -9,   -9,   -9,   -8,   -8,
   -8,   -8,   -8,   -8,   -8,   -8,   -7,   -7,
   -7,   -7,   -7,   -7,   -7,   -7,   -6,   -6,
   -6,   -6,   -6,   -6,   -6,   -6,   -5,   -5,
   -5,   -5,   -5,   -5,   -5,   -5,   -4,   -4,
   -3,   -3,   -2,   -2,   -2,   -2,   -2,   -2,
   -2,   -2,   -2,   -2,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,    0,    0,
    1,    1,    2,    2,    2,    2,    2,    2,
    2,    2,    2,    2,    2,    2,    2,    2,
    2,    2,    2,    2,    2,    2,    2,    2,
    2,    2,    2,    2,    2,    2,    2,    2,
    2,    2,    2,    2,    2,    2,    2,    2,
    2,    2,    2,    2,    2,    2,    2,    2,
    2,    2,    2,    2,    2,    2,    2,    2,
    3,    3,    3,    3,    3,    3,    3,    3,
    3,    3,    3,    3,    3,    3,    3,    3,
    3,    3,    3,    3,    3,    3,    4,    4,
    4,    4,    4,    4,    4,    4,    4,    4,
    4,    4,    4,    4,    4,    4,    4,    4,
    4,    4,    4,    4,    4,    4,    4,    4,
    4,    4,    5,    5,    5,    5,    5,    5,
    5,    5,    5,    5,    5,    5,    5,    5,
    5,    5,    6,    6,    6,    6,    6,    6,
    6,    6,    6,    6,    6,    6,    6,    6,
    6,    6,    6,    6,    6,    6,    6,    6,
    6,    6,    6,    6,    6,    6,    6,    6,
    6,    6,    7,    7,    7,    7,    7,    7,
    7,    7,    7,    7,    7,    7,    7,    7,
    7,    7,    7,    7,    7,    7,    7,    7,
    7,    7,    7,    7,    8,    8,    8,    8,
    8,    8,    8,    8,    8,    8,    8,    8,
    8,    8,    8,    8,    8,    8,    8,    8,
    8,    8,    9,    9,    9,    9,    9,    9,
    9,    9,    9,    9,    9,    9,    9,    9,
    9,    9,    9,    9,    9,    9,    9,    9,
    9,    9,    9,    9,    9,    9,    9,    9,
    9,    9,   10,   10,   10,   10,   10,   10,
   10,   10,   10,   10,   10,   10,   10,   10,
   10,   10,   10,   10,   10,   10,   10,    0};

static int wr1_g_tab[512] = 
{
 -126, -126, -125, -125, -124, -124, -123, -123,
 -122, -122, -121, -121, -120, -120, -119, -119,
 -118, -118, -117, -117, -116, -116, -115, -115,
 -114, -114, -113, -113, -112, -112, -111, -111,
 -110, -110, -109, -109, -108, -108, -107, -107,
 -106, -106, -105, -105, -104, -104, -103, -103,
 -102, -102, -101, -101, -100, -100,  -99,  -99,
  -98,  -98,  -97,  -97,  -96,  -96,  -95,  -95,
  -94,  -94,  -93,  -93,  -92,  -92,  -91,  -91,
  -90,  -90,  -89,  -89,  -88,  -88,  -87,  -87,
  -86,  -86,  -85,  -85,  -84,  -84,  -83,  -83,
  -82,  -82,  -81,  -81,  -80,  -80,  -79,  -79,
  -78,  -78,  -77,  -77,  -76,  -76,  -75,  -75,
  -74,  -74,  -73,  -73,  -72,  -72,  -71,  -71,
  -70,  -70,  -69,  -69,  -68,  -68,  -67,  -67,
  -66,  -66,  -65,  -65,  -64,  -64,  -63,  -63,
  -61,  -61,  -59,  -59,  -57,  -57,  -54,  -54,
  -52,  -52,  -50,  -50,  -48,  -48,  -46,  -46,
  -44,  -44,  -42,  -42,  -41,  -41,  -39,  -39,
  -37,  -37,  -36,  -36,  -34,  -34,  -33,  -33,
  -31,  -31,  -30,  -30,  -29,  -29,  -27,  -27,
  -26,  -26,  -25,  -25,  -24,  -24,  -23,  -23,
  -22,  -22,  -21,  -21,  -20,  -20,  -19,  -19,
  -18,  -18,  -17,  -17,  -16,  -16,  -15,  -15,
  -14,  -14,  -14,  -14,  -13,  -13,  -12,  -12,
  -12,  -12,  -11,  -11,  -10,  -10,  -10,  -10,
   -9,   -9,   -8,   -8,   -8,   -8,   -7,   -7,
   -7,   -7,   -6,   -6,   -6,   -6,   -5,   -5,
   -5,   -5,   -4,   -4,   -2,   -2,   -2,   -2,
   -2,   -2,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,    0,    0,
    1,    1,    1,    1,    1,    1,    1,    1,
    1,    1,    1,    1,    1,    1,    1,    1,
    1,    1,    2,    2,    2,    2,    2,    2,
    4,    4,    5,    5,    5,    5,    6,    6,
    6,    6,    7,    7,    7,    7,    8,    8,
    8,    8,    9,    9,   10,   10,   10,   10,
   11,   11,   12,   12,   12,   12,   13,   13,
   14,   14,   14,   14,   15,   15,   16,   16,
   17,   17,   18,   18,   19,   19,   20,   20,
   21,   21,   22,   22,   23,   23,   24,   24,
   25,   25,   26,   26,   27,   27,   29,   29,
   30,   30,   31,   31,   33,   33,   34,   34,
   36,   36,   37,   37,   39,   39,   41,   41,
   42,   42,   44,   44,   46,   46,   48,   48,
   50,   50,   52,   52,   54,   54,   57,   57,
   59,   59,   61,   61,   63,   63,   64,   64,
   65,   65,   66,   66,   67,   67,   68,   68,
   69,   69,   70,   70,   71,   71,   72,   72,
   73,   73,   74,   74,   75,   75,   76,   76,
   77,   77,   78,   78,   79,   79,   80,   80,
   81,   81,   82,   82,   83,   83,   84,   84,
   85,   85,   86,   86,   87,   87,   88,   88,
   89,   89,   90,   90,   91,   91,   92,   92,
   93,   93,   94,   94,   95,   95,   96,   96,
   97,   97,   98,   98,   99,   99,  100,  100,
  101,  101,  102,  102,  103,  103,  104,  104,
  105,  105,  106,  106,  107,  107,  108,  108,
  109,  109,  110,  110,  111,  111,  112,  112,
  113,  113,  114,  114,  115,  115,  116,  116,
  117,  117,  118,  118,  119,  119,  120,  120,
  121,  121,  122,  122,  123,  123,  124,  124,
  125,  125,  126,  126,  127,  127,  127,    0};


/* BIAS: */
/*
static int wr1_bias_tab[256] = 
{
   40,   41,   42,   43,   44,   45,   46,   47,
   48,   49,   50,   51,   52,   53,   54,   55,
   56,   57,   58,   59,   60,   61,   62,   63,
   64,   65,   66,   67,   68,   69,   70,   71,
   72,   73,   74,   75,   76,   77,   78,   79,
   80,   81,   82,   83,   84,   85,   86,   87,
   88,   89,   90,   91,   92,   93,   94,   95,
   96,   97,   98,   99,  100,  101,  102,  103,
  104,  105,  106,  107,  108,  109,  110,  111,
  112,  113,  114,  115,  116,  117,  118,  119,
  120,  121,  122,  123,  124,  125,  126,  127,
  128,  129,  130,  131,  132,  133,  134,  135,
  136,  136,  137,  138,  139,  139,  140,  141,
  142,  142,  143,  144,  145,  145,  146,  147,
  148,  148,  149,  150,  151,  151,  152,  153,
  154,  154,  155,  156,  157,  157,  158,  159,
  160,  160,  161,  162,  163,  163,  164,  165,
  166,  166,  167,  168,  169,  169,  170,  171,
  172,  172,  173,  174,  175,  175,  176,  177,
  178,  178,  179,  180,  181,  181,  182,  183,
  184,  184,  185,  186,  187,  187,  188,  189,
  190,  190,  191,  192,  193,  193,  194,  195,
  196,  196,  197,  198,  199,  199,  200,  201,
  202,  202,  203,  204,  205,  205,  206,  207,
  208,  208,  209,  210,  211,  211,  212,  213,
  214,  214,  215,  216,  217,  217,  218,  219,
  220,  220,  221,  222,  223,  223,  224,  225,
  226,  226,  227,  228,  229,  229,  230,  231,
  232,  232,  233,  234,  235,  235,  236,  237,
  238,  238,  239,  240,  241,  241,  242,  243,
  244,  244,  245,  246,  247,  247,  248,  249,
  250,  250,  251,  252,  253,  253,  254,  255};
*/
/*
  236,  236,  237,  238,  239,  239,  240,  241,
  242,  242,  243,  244,  245,  245,  246,  247};
*/

/*
static int wr1_mak_bias (int x, int bias_crossover, 
	      int black_bias, double black_bias_factor,
	      int white_bias, double white_bias_factor, int *h)
{
   if (x < bias_crossover)
   {
      h[0] = x - black_bias 
	 - (int)(black_bias_factor*(bias_crossover-x));
      if (h[0]<BIN_FOREGROUND)
	 h[0] = BIN_FOREGROUND;
   }
   else if (x > bias_crossover)
   {
      h[0] = x + white_bias 
	 + (int)(white_bias_factor*(x-bias_crossover));
      if (h[0] > BIN_BACKGROUND)
	 h[0] = BIN_BACKGROUND;
   }
   else
      h[0] = x;
   return BIN_OK;
}
*/

static int wr1_stat (IBAND inband, double *mean, double *stdev)
{
   int x, y, xsize, ysize;
   int n;
   double sum_z;
   double sum_zz;
   
   xsize = Ixsize(inband);
   ysize = Iysize(inband);
   
   sum_z = sum_zz = 0;
   n = xsize * ysize;

   for (y=1; y<=ysize; y++)
      for (x=1; x<=xsize; x++)
      {
	 sum_z += inband[y][x];
	 sum_zz += inband[y][x] * inband[y][x];
      }
   mean[0] = sum_z / n;
   stdev[0] = sqrt (sum_zz/n - mean[0]*mean[0]);
   return BIN_OK;
}



#define WR1_BIAS_CROSSOVER 93
#define WR1_BLACK_BIAS -40
#define WR1_WHITE_BIAS 40
#define WR1_BIAS 20
#define WR1_BLACK_BIAS_FACTOR 0.0
#define WR1_WHITE_BIAS_FACTOR -0.25


/*
#define WR1_BIAS_CROSSOVER 64
#define WR1_BLACK_BIAS -20
#define WR1_WHITE_BIAS 20
#define WR1_BLACK_BIAS_FACTOR 0.0
#define WR1_WHITE_BIAS_FACTOR -0.5
*/


static int wr1_bias (int x, int offset)
{
   int result;
   int bias;
   
   x = 256 - x;

   bias = -offset;
   
   if (x < WR1_BIAS_CROSSOVER)
   {
      result = x - bias
	 - (int)(WR1_BLACK_BIAS_FACTOR*(WR1_BIAS_CROSSOVER-x));
   }
   else if (x >= WR1_BIAS_CROSSOVER)
   {
      result = x + bias
	 + (int)(WR1_WHITE_BIAS_FACTOR*(x-WR1_BIAS_CROSSOVER));
   }
   else
      result = x;

/*
   result = x-bias;
*/
   if (result < BIN_FOREGROUND)
      result = BIN_FOREGROUND;
   if (result > BIN_BACKGROUND)
      result = BIN_BACKGROUND;
   
   return  256 - result; 
}

static int wr1_f (int diff, int *f)
{
   if (abs(diff)>WR1_F_OFFSET)
   {
      Warning(2, "wr1_f: Error: diff = %i\n", diff);
      return BIN_ERROR;
   }
   f[0] = -wr1_f_tab[WR1_F_OFFSET - diff];
   return BIN_OK;
}


static int wr1_g (int diff, int *g)
{
   if (abs(diff)>WR1_G_OFFSET)
   {
      Warning(2, "wr1_g: Error: diff = %i\n", diff);   
      return BIN_ERROR;
   }
   g[0] = -wr1_g_tab[WR1_G_OFFSET - diff];
   return BIN_OK;
}

int bin_wr1 (IBAND inband, IBAND outband, int x_lookahead, int y_lookahead,
	     int bias_mode, int bias_factor, int f_factor, int g_factor, 
	     int intermediate_images)
{
  int xsize, ysize;
  int x, y;
  int u;
  int Y, *Z, prevY;
  int f, g;
  int x_ahead, y_ahead;
  int t;
  int offset = WR1_BIAS;
  double mu, s_dev;
/*   int otsu_thr; */
  IBAND thr_surf;

  if (Ipixtyp(inband) != Iu_byte_typ)
    return(Error(1, "bin_wr1: Input pixel type must be unsigned byte.\n"));
  if (Ipixtyp(outband) != Iu_byte_typ)
    return(Error(2, "bin_wr1: Output pixel type must be unsigned byte.\n"));

  xsize = Ixsize(inband);
  ysize = Iysize(inband);

  x_lookahead = x_lookahead % xsize;

   /*
   otsu_thr = otsu_threshold(inband, &mu_1, &mu_2, &mu);
   */

   if (bias_mode == 0) {
     wr1_stat(inband, &mu, &s_dev);
     offset = s_dev - 40 ;
   } else offset = bias_mode;
  
   /*
   Info(0, "[bin_wr1] Otsu thr = %i, mu_1 = %f, mu_2 = %f, mu = %f\n",
	   otsu_thr, mu_1, mu_2, mu);
	   */
   if (Verbose()) printf("[bin_wr1] offset = %i\n", offset);

   if ((Z = (int*)malloc(sizeof(int)*(xsize+1))) == NULL)
      return BIN_NO_MEMORY;

   if (intermediate_images) {
     thr_surf = (IBAND) Imake_band (Iu_byte_typ, xsize, ysize);
     
     if (Verbose()) {
       printf("bin_wr1: Test of bias function wr1_bias:\n");
       for (x=0; x<=255; x++) {
	 printf("%4i %4i\n", x, bias_factor * wr1_bias(x, offset) / 100);
       }
     }
     if (Verbose()) printf("bin_wr1: Test of f function wr1_bias:\n");
     for (x= -255; x<=255; x++) {
       wr1_f(x, &f);
       if (Verbose()) printf("%4i %4i\n", x, f_factor * f / 100);
     }
     
     if (Verbose()) printf("bin_wr1: Test of g function wr1_bias:\n");
     for (x= -255; x<=255; x++) {
       wr1_g (x, &g);
       if (Verbose()) printf("%4i %4i\n", x, g_factor * g / 100);
     }
   }
   
   Z[1] = prevY = mu;
   for (y=1; y<= 1+y_lookahead; y++)
   {
      if (y < 1+y_lookahead)
	 t = xsize;
      else
	 t = x_lookahead;
      for (x=1; x<= t; x++)
      {
	 u = inband[y][x];
	 wr1_f (u-prevY, &f);
	 Y = prevY + f;
	 if (y == 1)
	    Z[x] = mu;
	 else
	 {
	    wr1_g(Y-Z[x], &g);
	    Z[x] = Z[x] + g; 
	 }
      }
      
   }
   x_ahead = 1 + x_lookahead;
   y_ahead = 1 + y_lookahead;
   
   for (y = 1; y <= ysize; y++)
   {
      for (x = 1; x <= xsize; x++)
      {
	 if (inband[y][x] < (bias_factor  
			     * wr1_bias(Z[x_ahead],offset) / 100))
	    outband[y][x] = 0;
	 else	
	    outband[y][x] = 255;
	 if (intermediate_images)
	    thr_surf[y][x] = (bias_factor 
			      * wr1_bias(Z[x_ahead], offset)) / 100;
	 x_ahead++;
	 if (x_ahead > xsize)
	 {
	    x_ahead = 1;
	    y_ahead++;
	 }
	 if (y_ahead <= ysize)
	 {
	    prevY = Y;
	    wr1_f(inband[y_ahead][x_ahead]-prevY, &f);
	    Y = prevY + f_factor * f / 100;
	    wr1_g(Y-Z[x_ahead], &g);
	    Z[x_ahead] = Z[x_ahead] + g_factor * g / 100;
	 }
	 else
	    Z[x_ahead] = Z[x_ahead-1];
      }
   }
   if (intermediate_images)
   {
      write_byteband(thr_surf, "WR1_thr_surf.img");
      Idel_band (&thr_surf);
   }
   else
      if (Verbose()) printf("No intermediate images\n");
     
   return BIN_OK;
} /* END bin_wr1 */



/*
static int bin_wr1_expand (IBAND inband, IBAND outband, 
			   int x_lookahead, int y_lookahead, 
			   int bias_mode, int bias_factor, 
			   int f_factor, int g_factor, 
			   int intermediate_images)
{
   int xsize, ysize;
   int isize, jsize;
   int x, y, i, j;
   int u;
   int Y, *Z, prevY;
   int f, g;
   int x_ahead, y_ahead;
   int t;
   int offset = WR1_BIAS;
   double mu, s_dev;
   IBAND thr_surf;
   IBAND expanded;
   int expand_depth;


   xsize = Ixsize(inband);
   ysize = Iysize(inband);


   x_lookahead = x_lookahead % xsize;

   Info(0, "\n");
   Info(0, "White and Rohrer's method I \n");
   
   Info(0, "Reccognized parameters:\n");
   Info(0, "[-lx]  no. of lookahead pixels      = %i\n", x_lookahead);
   Info(0, "[-ly]  no. of whole lookahead lines = %i\n", y_lookahead);
   Info(0, "[-m]   bias_mode                    = %i\n", bias_mode); 
   Info(0, "[-b]   bias_factor                  = %i\n",  
	   bias_factor);
   Info(0, "[-f]   f_factor                     = %i\n", 
	   f_factor);
   Info(0, "[-b]   g_factor                     = %i\n", 
	   g_factor);
   

   expand_depth = 2*x_lookahead;
   isize = xsize + 2*expand_depth;
   jsize = ysize + 2*expand_depth;
   expanded = (IBAND) Imake_band(Iu_byte_typ, isize, jsize);
   expand_edge (inband, 0, 0, expand_depth, expanded);

   \*
   otsu_thr = otsu_threshold(inband, &mu_1, &mu_2, &mu);
   *\

   if (bias_mode == 0)
   {
      wr1_stat(inband, &mu, &s_dev);
      offset = s_dev / 4 ;
   }
   else
      offset = bias_mode;
  
   \*
   Info(0, "[bin_wr1] Otsu thr = %i, mu_1 = %f, mu_2 = %f, mu = %f\n",
	   otsu_thr, mu_1, mu_2, mu);
	   *\
   Info(0, "[bin_wr1] offset = %i\n", offset);

   if ((Z = (int*)malloc(sizeof(int)*(isize+1))) == NULL)
      return BIN_NO_MEMORY;
   
   if (intermediate_images)
   {
      thr_surf = (IBAND) Imake_band (Iu_byte_typ, xsize, ysize);
      
      Info(0, "bin_wr1: Test of bias function wr1_bias:\n");
      for (x=0; x<=255; x++)
      {
	 Info(0, "%4i %4i\n", x, bias_factor * wr1_bias(x, offset) / 100);
      }
   
      Info(0, "bin_wr1: Test of f function wr1_bias:\n");
      for (x= -255; x<=255; x++)
      {
	 wr1_f(x, &f);
	 Info(0, "%4i %4i\n", x, f_factor * f / 100);
      }
   
      Info(0, "bin_wr1: Test of g function wr1_bias:\n");
      for (x= -255; x<=255; x++)
      {
	 wr1_g (x, &g);
	 Info(0, "%4i %4i\n", x, g_factor * g / 100);
      }
   }
   
   Z[1] = prevY = mu;
   for (j=1; j<= 1+y_lookahead; j++)
   {
      if (j < 1+y_lookahead)
	 t = isize;
      else
	 t = x_lookahead;
      for (i=1; i<= t; i++)
      {
	 u = inband[y][x];
	 wr1_f (u-prevY, &f);
	 Y = prevY + f;
	 if (j == 1)
	    Z[i] = mu;
	 else
	 {
	    wr1_g(Y-Z[i], &g);
	    Z[i] = Z[i] + g; 
	 }
      }
      
   }
   x_ahead = 1 + x_lookahead;
   y_ahead = 1 + y_lookahead;
   
   for (j = 1; j <= jsize; j++)
   {
      y = j-expand_depth;
      for (i = 1; i <= isize; i++)
      {
	 x = i-expand_depth;
	 if ((x >= 1) && ( x <= xsize) && (y >= 1) && (y <= ysize))
	 {
	    if (expanded[j][i] < (bias_factor  
				  * wr1_bias(Z[x_ahead],offset) / 100))
	       outband[y][x] = 0;
	    else	
	       outband[y][x] = 255;
	    if (intermediate_images)
	       thr_surf[y][x] = (bias_factor 
				 * wr1_bias(Z[x_ahead], offset)) / 100;
	 }
	 x_ahead++;
	 if (x_ahead > isize)
	 {
	    x_ahead = 1;
	    y_ahead++;
	 }
	 if (y_ahead <= jsize)
	 {
	    prevY = Y;
	    wr1_f(expanded[y_ahead][x_ahead]-prevY, &f);
	    Y = prevY + f_factor * f / 100;
	    wr1_g(Y-Z[x_ahead], &g);
	    Z[x_ahead] = Z[x_ahead] + g_factor * g / 100;
	 }
	 else
	    Z[x_ahead] = Z[x_ahead-1];
      }
   }
   if (intermediate_images)
   {
      write_byteband(thr_surf, "WR1_thr_surf.img");
      Idel_band (&thr_surf);
   }
   else
      Info(0, "No intermediate images\n");
   
   Idel_band(&expanded);
   return BIN_OK;
}
*/


/*
   z = 127.5;
   for (y = 1; y <= ysize; y++)
   {
      for (x = 1; x <= xsize; x++)
      {
	 if (inband[y][x] < z)
	    outband[y][x] = 0;
	 else
	    outband[y][x] = 255;
	 z = z + 0.01*bias_factor*(inband[y][x]-z);
      }
      / *
      y++;
      if (y <= ysize)
      {
	 for (x = xsize; x >= 1; x--)
	 {
	    if (inband[y][x] < z)
	       outband[y][x] = 0;
	    else
	       outband[y][x] = 255;
	    z = z + 0.1*bias_factor*(inband[y][x]-z);
	 }
      }
      * /
   }
*/
