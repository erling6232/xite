

/*

________________________________________________________________

        binarize.c
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



/*P:binarize*

________________________________________________________________

		binarize
________________________________________________________________

Name:		binarize - binarize a gray scale image

Syntax:		| binarize [-m <method>] 
                |      [<method-specific parameters>]
		|      [-v <val-step-threshold>] 
		|      [iv <inv-val_step_threshold>]
		|      [-iim]
		|      <infile> <outfile>

Description:	Binarize a gray scale image using the selected binarization 
                method. 
		
Options:        &-m method
		'method' is one of the following 2-3 charachter	abbreviations:
		|   Ab        (Abutaleb, 1989)
		|   Be        (Bernsen, 1986)
		|   ETM       (Eikvil, Taxt & Moen, 1991)
		|   KI        (Kittler & Illingworth, 1986)
		|   KSW       (Kapur, Sahoo & Wong 1985)
		|   MH        (Mardia & Hainsworth, 1988)
		|   NR        (Nakagawa & Rosenfeld, 1979)
		|   Ni        (Niblack, 1986)
		|   Ot        (Otsu, 1979)
		|   Pa        (Parker, 1991)
		|   TFJ       (Taxt, Flynn & Jain, 1989)
		|   WR        (White & Rohrer, 1983)
		|   IWR       (Improved White & Rohrer, 1993)
		|   YB        (Yanowitz & Bruckstein, 1989)

		Eikvil, Taxt and Moen`s method is default.

		&-v val-step-threshold
		Specifies that a 'validation step' is to be performed on
		the binarized image. For each connected foreground object in 
		the binarized image  a value 'V' between 0 and 255 is 
		calculated.
		Then the objects are thresholded, removing those objects 
		that vere assigned a value 'V' below 'val-step-threshold'.
		By using a negative value for 
		'val-step-threshold', the individual connected foreground 
		objects will be colored with the gray level 'V'. Then the
		output image can be manually thresholded in 'xshow' to find a 
		suitable value for 'val-step-threshold', i.e. 255-T, where 
		T is the threshold found in 'xshow'.
		
		&-iv inv-val-step-threshold
		Specifies that a similar 'validation step' is to be performed,
	        which tries to remove misclassified background objects.

		&-iim
		Specifies that intermediate images will be produced.

Methods:        A brief description of each method, with its method-specific
		parameters now follows.

		&Ab (Abutaleb)
		No method-specific parameters.

		&Be (Bernsen)
		|    -r <region_size>
		|    -c <contrast_limit>
		|    -low
		For each pixel, the threshold		
		|    T(x,y) = (Z_low + Z_high)/2
		is used, where Z_low and Z_high are the lowest and highest
		gray level pixel values in a square neighborhood of size 
		('region_size' x 'region_size') about the pixel (x,y).
		However, if the contrast measure 
		|    C(x,y) = Z_high - Z_low < 'contrast_limit'
		the pixel is set to 255 if '-low' is specified, else to 0.

		&ETM (Eikvil, Taxt & Moen)
		| -r <region_size>
		| -s <small_window_size>
		| -l <mean_diff_limit>
		| -w <weight>
		| -mi <global_min>
		| -ma <global_max>

		The pixels inside a small window 'S' of size
		('small_window_size' x 'small_window_size') are thresholded on
		basis of the clustering of the pixels inside a larger
		concentric window 'L' of size ('region_size' x 'region_size').
		'S' and 'L' slide across the image in steps equal to the side
		of 'S'. For all pixels inside 'L', a threshold 'T' is
		calculated to divide the pixels into two clusters. If the two
		estimated cluster means mu1 and mu2 are further apart than a
		limit, mu2 - mu1 > 'mean_diff_limit' the pixels inside 'S' are
		binarized using the threshold value 'T'. When mu2 - mu1 <
		'mean_diff_limit', the pixels inside 'S' are all assigned to
		the class with the closest updated mean value, being it print
		or background.

		'bin_method' is used to select the global binarization 
		method which finds 'T'. Use 'bin_method' = 
		OTSU or 'bin_method' = KITTLER_ILLINGWORTH. 

		'weight' is used to calculate the updated mean 
		values MU1 and MU2. For each small window 'S', the
		new values are caluculated as

		| MU1 = 'weight' * MU1 + (1.0-'weight') * mu1
		| MU2 = 'weight' * MU2 + (1.0-'weight') * mu2

		'weight' = 1.0 means that the initial estimates
		for MU1 and MU2 are kept forever, while 
		'weight' = 0.0 means that the mean values
		mu1 and mu2 of the 
		two clusters in 'L' are used in the next step.
		
		For extremely difficult images, a preprocessing 
		step can be used on
		the image.  Replace all pixel values higher than the
		value 'glob_max' by
		' glob_max' itself, and similarly, replace all 
		pixel values lower
		than 'glob_min' by 'glob_min'.

		&KI (Kittler & Illingworth).
		Global method, no method-specific parameters.
		
		&KSW (Kapur, Sahoo & Wong).
		Global method, no method-specific parameters.

		&man (manually selected threshold).
		The image is thresholded using 'threshold'.
		| -t <threshold>

		&MH (Mardia & Hainsworth)
		| -s
		| -d <diff-limit>
		Mardia and Hainsworth`s method first makes an initial
		binarization. Then several steps are iterated until
		convergence is reached. First, the estimated mean values mu1
		and mu2, and the number of pixels n_i in both foreground and
		background of the current binarization are calculated. Then a
		threshold 'T' based on these values is calculated. Then, for
		each pixel, a weighted mean, 'G', of the pixel and its eight
		neighbors is calculated. If 'G'<='T', the pixel is classified
		as ``black'', otherwise ``white''. The weights on the neighbor
		pixels, used to calculate 'G', may be equal, or they may be
		calculated based on a semivariogram approximation. If
		'use_sem_var' >= 1, the semivariogram approach is used. The
		last step of each iteration is to smooth the new binary image
		using a (3 x 3) median filter. 'diff_limit' is used to test if
		convergence has been reached. The number of different pixels
		of two successive binarizations is counted, and compared to
		'diff_limit'.
		
		&Ni (Niblack)
		| -r <region-size>
		| -k <weight>
		The idea of this method is to vary the threshold over the
		image, based on the local mean and local standard deviation. 
		The threshold at pixel (x,y) is calculated as 
		|    T(x,y) = 'weight' * sigma(x,y) + mu(x,y)
		where sigma(x,y) and mu(x,y) are the estimated standard
		deviation and mean value in a local neighborhood of (x,y) of
		size ('region_size' x 'region_size'). The value of 'weight' is
		used to adjust how much of the foreground object edges that
		are taken as a part of the object. A recommended value for
		'weight' is -0.2 for images with black foreground objects, and
		'weight' = +0.2 for images with white foreground objects.

		&NR (Chow & Kaneko / Nakagawa & Rosenfeld)
		| -r <region-size>
		| -m <mean-diff-limit>
		| -mi <min-ratio>
		| -ma <max-ratio>
		| -s <sdev-limit>
		| -vtp <vtp-limit>	     
		The method is a slight modification of Chow and Kaneko`s
		method. The result is returned in 'outband'. This method
		calculates a thresholding surface. The image is divided into
		windows of size ('region_size' x 'region_size'), and the
		histograms for each window are tested for bimodality. Each
		histogram is approximated by a mixture of two Gaussian
		distributions. Let mu1 and mu2 be the estimated mean values,
		and sigma1 and sigma2 be the estimated standard deviations of
		the two distributions. Also, let sigma_tot be the total
		standard deviation, and vtp be the valley-to-peak ratio. The
		bimodality test requires the following five criteria to hold:
		|    (1) sigma_tot > 'sdev_limit'
		|    (2) mu2 - mu1 > 'mean_limit'
		|    (3) sigma1/sigma2 > 'min_ratio'
		|    (4) sigma1/sigma2 > 'min_ratio'
		|    (5) vtp < 'valley_to_peak_limit'
		For each bimodal window, a threshold is calculated based on the
		mu_1, mu_2, sigma_1 and sigma_2	parameters of the mixture
		distribution. 

		Thresholds for the other windows are calculated on basis of
		the thresholds for the bimodal windows. The thresholds are
		first smoothed, and then interpolated to give a threshold
		surface, having a single threshold value for each image pixel.
		The image is binarized using the threshold surface.
		
		&Ot (Otsu). Global method.
		No method-specific parameters.

		&Pa (Parker)
		| -r <region-size>
		| -k <weight>
		| -m <mean-limit>
		| -s <stdev-limit>
		| -abs
		Parker's method should normally not be used with other
		parameters than '-r', if any. The other parameters
		were included to manipulate with the internal tests in
		the algorithm, since some of these were obviously
		wrong in the original paper. More sensible values are
		given as default in this program.

		This method first detects edges, and then the interior
		of objects between edges are filled. First, for each
		pixel (x,y) in the input image Z, calculate 

		|D(x,y) =   min   [Z(x,y)-Z(x_i,y_i)] ,
		|         i=1,..,8
		where (x_i,y_i), i=1,.., 8 are the 8 eight-connected
		neighbors of (x,y). Thus, the negative of the gradient
		in the direction of the brightest neighbor is found.
		Then 'D' is broken up in regions of size 'region-size'
		x 'region-size', and for 
		each region, the sample mean and standard deviations
		are calculated. Both values are smoothed, and then
		bilinearly interpolated to give two new images 'M' and
		'S', originating from the mean values and standard
		deviations, respectively. Then for all pixels (x,y),
		if M(x,y) >= 'mean-limit' or S(x,y) < 'stdev-limit',
		then the pixel is regarded as part of a flat region,
		and remains unlabeled. [In Parker's paper, the test is
		M(x,y) >= -1  or S(x,y) >= -1, which is fulfilled by
		all pixels, since S(x,y) is never negative.]  Else, if
		D(x,y) < M(x,y) + 'weight' S(x,y), then (x,y) is labeled
		print; else, (x,y) remains unlabeled. The resulting
		binary image highlights the edges. This is followed by
		pixel aggregation and region growing steps to locate
		the remaining parts of the print objects. 

		The following default values are used: 'region-size' =
		16, 'mean-limit' = -1.0, 'stdev-limit' = 1.0, and
		'weight' = -1.0.
 
		&TFJ (Taxt, Flynn & Jain)
		| -r <region-size>
		| -s <small-window-size>
		| -t <training-set-size>
		Taxt, Flynn and Jain`s method divides the image into
		non-overlapping windows of size ('small_window_size' x
		'small_window_size'). For each window, a larger concentric
		window of size ('region_size' x 'region_size') pixels. The
		histogram of each of the larger windows is approximated by a
		mixture of two Gaussian distributions. The parameters of the
		mixture are estimated using an expectation-maximization (EM)
		algorithm. In each small window, the pixels are classified
		using the quadratic Bayes'' classifier.

		A training set of 'training_size' pixels are picked on random,
		but edge pixels are avoided in the training set. The EM
		algorithm requires global start values for the estimated class
		means mu1 and mu2, and estimated standard deviations sigma1
		and sigma2. These are obtained by k-means clustering of the
		whole input band. The EM algorithm also needs an initial
		estimate of the mixing weight pi, and pi = 0.5 is used.

		&WR1 (White and Rohrer I)
		| -l <nof-lookahead-pixels>
		In their 1983 paper, White and Rohrer proposed two very
		different binarization methods. The first they called "Dynamic
		threshold algorithm", which we call 'WR1' (White and Rohrer
		method I). The second they called "Integrated function
		algorithm" ('WR2', (White and Rohrer method II)

		The 'lookahead' parameter gives the number of lookahead pixels
		used in the biased running average that is used in deciding
		the threshold at each pixel location. Good values for
		'lookahead' are 8 or (8 + Ixsize(inband)).

		&WR2 (White & Rohrer II)
		| -a <activity-limit>
		| -ma <max-value>
		| -m <mean-filter-size>
		| -s <search-vector-length>
		| -sl <spot-label>
		| -l <min-obj-width>
		| -d
		| -single
		| -double
		White and Rohrer`s method II (Integrated function algorithm)
		uses a gradient-like operator called the activity on the
		image. Pixels with activity below 'activity_limit' are labeled
		'0'. The other pixels are further tested. If the Laplacian
		edge operator of the pixel is positive, the pixel is labeled
		'+', otherwise '-'. That way a three-level label-image is
		constructed, with legal pixel values '+', '0' and '-'. The
		idea is that in a sequence of labels, edges are identified as
		'-+' transitions or '+-' transitions. Object pixels are
		assumed to be '+' and '0' labeled pixels between a '-+' and
		'+-' pair. For each pixel to be classified, a search vector of
		length 'search_vec_len' for each search direction is centered
		on the pixel. The '-+' and '+-' must be inside the seach
		vector to be recognized. By using a low value on
		'search_vec_len', only lines and small filled areas will be
		captured.

		If 'spot_size_limit' > 1, then small '+'-labeled regions
		having less than 'spot_size_limit' pixels are relabeled as
		'0'-regions.
		
		By specifying 'activity_limit' = 0, Kitler and Illingworth`s
		method is used to set the activity threshold. An upper bound
		for this value is specified by 'max_activity'.

		A lower limit on the distance between the '-+' and '+-' for
		pixels to be classified as object pixels is specified by
		'distance_limit'. This can be used to eliminate the thinnest
		lines in 'inband'.

		The first improvement suggested by Trier and Taxt [1] can be
		used in 'bin_wr2':

		(1) The input image is smoothed by a mean filter of size
		('mean_size' x 'mean_size') to remove stochastic 
		noise. To run the original White and Rohrer`s method,
		'mean_size' = 1 must be used.

		Variations on how pixels are classified as 'print' or
		'background' are selected though the 'mode' parameter:

		If 'mode' = WR_TWO_BY_TWO, the original approach is used. Two
		times two pixels are classified at a time, requiring that all
		four pixels are inside either horizontal or vertical object
		pixel sequences.

		The reasons for classifying (2 x 2) pixels at a time in the
		original implementation was partly to save computational time,
		partly to reduce noise. With faster computers, a more
		time-consuming algorithm may be allowed.
		
		If 'mode' = WR_DOUBLE_VECTORS, then the idea is that a single
		pixel being part of an object pixel sequence is not enough. It
		has to have support from at least one of its neighbors. Either
		one of its vertical neighbors and the pixel itself have to be
		inside horizontal object pixel sequences or one of its
		horizontal neighbors and the pixel itself must be inside
		vertical object pixel sequences to be recognized as object
		pixels.

		If 'mode' = WR_SINGLE_VECTOR, only a single search vector is
		used in each direction.

		If 'diagonal' >= 1, then the horizontal and vertical search
		directions are complemented with two diagonal search
		directions. If 'diagonal' == 0, then only the horizontal and
		vertical search directions are used.

		&IWR (Improved White & Rohrer)
		| -a <activity-limit>
		| -ma <max-value>
		| -m <mean-filter-size>
		| -sl <spot-label>
		This is an improved version of 'WR2'. Some of the switches
		allowed for 'WR2' have been removed, since the use of search
		vectors to locate print pixels in the three-level label image
		is abandoned in 'IWR'. The switches still in use have the same
		meaning as for 'WR2'.

		The following approach is used. First, all "'+'"-marked
		regions are labeled 'print' and "'-'"-marked regions are
		marked 'background'. Then, for each "'0'"-marked region, the
		number of "'-'" and "'+'" pixels that are 8-connected to the
		region is counted. If the number of "'+'" pixels is in
		majority, the "'0'" -marked region is labeled 'print',
		otherwise 'background'.

		This approach prodces a lot of noise which can be seen as
		spurious print components, which must be removed by using the
		validation step (the '-v' option described earlier).

		&YB (Yanowitz & Bruckstein)
		| -b <beta>
		| -m <max-nof-iter>
		| -t <threshold>
		A potential surface passing through the image at local maxima
		of the gradient image is used as a thresholding surface. The
		gradient magnitude image is computed, using for example
		Sobel`s edge operators. This image is thresholded and thinned
		to one-pixel wide lines, to identify edge pixels. The original
		image is smoothed by a (3x3) mean filter to remove stochastic
		noise. A potential surface is constructedby an iterative
		interpolation scheme. The interpolation surface is made to
		pass through the edge pixels. An interpolation residual R(x,y)
		is calculated for each non-edge pixel, and the new pixel value
		P(x,y) at iteration n is calculated as
		|  P[n](x,y) = P[n-1](x,y) + (beta * R[n](x,y)) / 4
		Yanowitz and Bruckstein recommended to use 1.0 < 'beta'
		< 2.0 for fast convergence (over-relaxation). However, 
		we found that by using 'beta' > 1.0, P(x,y) started to diverge
		after 5-10 iterations. So 'beta' <= 1.0 must be used, and
		'beat' = 1.0 works well.
		
		Next, the image is binarized using the thresholding surface. 
		In the original algorithm, so-called "ghost" objects are 
		removed by a validation step. This validation step is
		in a separate library function named 'remove_ghost'.
		
Files:          src/binarize/ binarize.c, src/binarize/bin_*.c

See also:       remove_ghost(3), bin_ab(3), bin_be(3), bin_etm(3), bin_ki(3), 
                bin_ksw(3), bin_man(3), bin_mh(3), bin_ni(3), bin_nr(3),
		bin_ot(3), bin_pa(3), bin_tfj(3), bin_wr1(3), bin_wr2(3),
		bin_iwr(3), bin_yb(3).

References:     &[1] 'Ahmed S. Abutaleb'
                "Automatic thresholding of gray-level pictures using
		two-dimensional entropy",
		Computer Vision, Graphics, and Image Processing, vol
		47, pp 22-32, 1989. 

                &[2] 'John Bernsen'
		"Dynamic thresholding of grey-level images", 
		Proc. 8th International Conference on Pattern 
		Recognition (ICPR8), pp 1251-1255, Paris, France, 
		October 1986.

		&[3] 'C.K. Chow & T. Kaneko'
		"Automatic detection of the left ventricle from
		cineangiograms",
		Computers and biomedical research, vol. 5, pp 388-410, 1972.
		
		&[4] 'L.Eikvil, T.Taxt & K.Moen'
		"A Fast Adaptive Method for Binarization of Document Images",
		Proceedings of the First International Conference
		on Document Analysis and Recognition,
		pp 435-443, 1991, Saint-Malo, France.
		
		&[5] 'Kapur, Sahoo & Wong'
		"A new method for Gray-level picture thresholding using the
		entropy of the histogram",
		Computer Vision, Graphics, and Image Processing, vol 29, pp
		273-285, 1985.

		&[6] 'Josef Kittler & John Illingworth'
		"Minimum error thresholding",
		Pattern Recognition, vol 19, pp 41-47, 1986.
		
		&[7] 'K.V. Mardia & T.J. Hainsworth'
		"A spatial thresholding method for image segmentation",
		IEEE Transactions on Pattern Analysis and Machine
		Intelligence, vol T-PAMI 10, no 6, pp 919-927, 1988.
		
		&[8] 'Yasuo Nakagawa & Azriel Rosenfeld'
		"Some experiments on variable thresholding",
		Pattern Recognition, vol 11, no 3, pp 191-204, 1979.
		
		&[9] 'Wayne Niblack'
		"An introduction to Digital image processing",
		pp 115-116, Prentice Hall, 1986.
		
		&[10] 'Nobuyuki Otsu'
		"A threshold selection method from gray-level histograms",
		IEEE transactions on Systems, man and cybernetics,
		vol T-SMC 9, no 1, pp 62-66, 1979.
		
		&[11] 'J.R. Parker'
		"Gray level thresholding in badly illuminated images",
		IEEE Transactions on Pattern Analysis and Machine
		Intelligence, vol T-PAMI 13, no 8, pp 813-819, 1991.
		
		&[12] 'T. Taxt, P.J. Flynn & A.K. Jain'
		"Segmentation of document images",
		IEEE Transactions on Pattern Analysis and Machine
		Intelligence, vol T-PAMI 11, no 12, pp 1322-1329, 1989.

		&[13] 'Oivind Due Trier & Torfinn Taxt' 
		"Improvement of `Integrated Function Algorithm` for
		Binarization of Document Images",
		Pattern Recognition Letters, to appear. 

		&[14] 'Oivind Due Trier & Torfinn Taxt' 
		"Evaluation of binarization methods for utility map images",
		in Proceedings of the first IEEE International Conference on
		Image Processing, vol. 2, Austin, Texas, Nov. 1994.

		&[15] 'Oivind Due Trier & Torfinn Taxt' 
		"Evaluation of binarization methods for document images",
		IEEE Transactions on Pattern Analysis and Machine Intelligence,
		to appear.

		&[16] 'James M. White & Gene D. Rohrer'
		"Image Thresholding for	Optical Character Recognition and
		Other Applications Requiring Character Image Extraction",
		IBM Journal of Research and Development, vol 27, no 4,
		pp 400-411, July 1983. 
		
		&[17] 'S.D. Yanowitz & A.M. Bruckstein'
		"A New Method for Image Segmentation",
		Comput. Vision Graphics Image Proc., vol 46, no 1, pp 82-95,
		April 1989.

Restrictions:   Only single-band images. All images must have bands with
                pixel type unsigned byte.

Bugs:           The intermediate images option '[-iss]' only works for
                a few methods.

Return value:   0   =   OK

Author:		Øivind Due Trier, Ifi.

Examples:	| binarize -m Ni -k 0.18 -v 30 05.img 05.Ni.k.018.v30.img
                | 
		| negate 05.img 05.neg.img
                | binarize -m IWR -d -v 30 05.neg.img 05.neg.IWR.d.v30.img
		| negate 05.neg.IWR.d.v30.img 05.IWR.d.v30.img

Id: 		$Id$
________________________________________________________________

*/

#ifdef MAIN

#define EM_DEBUG 

#include <math.h>
#include <stdlib.h>
#include <xite/includes.h>
#include <xite/biff.h>
#include XITE_STDIO_H
#include XITE_TIME_H
#include XITE_STRING_H
#include <xite/blab.h>
#include <xite/histo.h>
#include <xite/readarg.h>
#include <xite/message.h>
#include <xite/binarize.h>
#include "BinM.h"
#include "write_band.c"

#ifdef FUNCPROTO
static int binarize(int bin_method, int region_size, int step_size, double distance_limit, double weight, int global_min, int global_max, int threshold, int flag, double low_bound, double up_bound, double sdev_limit, double ratio, int intermediate_images, IBAND inband, IBAND outband)
#else
static int binarize(bin_method, region_size, step_size, distance_limit, weight, global_min, global_max, threshold, flag, low_bound, up_bound, sdev_limit, ratio, intermediate_images, inband, outband)
int bin_method, region_size, step_size;
double distance_limit, weight;
int global_min, global_max, threshold, flag;
double low_bound, up_bound, sdev_limit, ratio;
int intermediate_images;
IBAND inband, outband;
#endif
{
  int code;
  switch (bin_method)
    {
    case ABUTALEB:
      {
	code = bin_ab(inband, outband);
	break;
      }
    case BERNSEN :
      {
	code = bin_be(region_size, (int)distance_limit, flag, 
		      inband, outband);
	break;
      }
    case DNM :
    case EIKVIL_TAXT_MOEN :
      {
	code = bin_etm(region_size, step_size, distance_limit, weight,
		       global_min, global_max, OTSU,
		       inband, outband);
	break;
      }
    case DNM_NEW :
      {
	code = bin_etm(region_size, step_size, distance_limit, weight,
		       global_min, global_max, KITTLER_ILLINGWORTH, 
		       inband, outband);
	break;
      }
    case KAPUR_SAHOO_WONG:
      {
	code = bin_ksw(inband, outband);
	break;
      }
    case KITTLER_ILLINGWORTH :
      {
	code = bin_ki(inband, outband);
	break;
      }
    case MARIDA_HAINSWORTH:
      {
	code = bin_mh(inband, outband, flag, distance_limit);
	break;
      }	
    case CHOW_KANEKO :
    case NAKAGAWA_ROSENFELD :
      {
	code = bin_nr(sdev_limit, distance_limit, low_bound, up_bound,
		      ratio, region_size, inband, outband);
	break;
      }
    case NIBLACK :
      {
	code = bin_ni(region_size, weight,
		      inband, outband);	
	break;
      }
    case OTSU :
      {
	code = bin_ot(inband, outband);
	break;
      }
    case PARKER :
      {
	code = bin_pa(inband, outband, region_size, weight,
		      distance_limit, sdev_limit, flag);  
	break;
      }
    case TAXT_FLYNN_JAIN :
      {
	code = bin_tfj(inband, outband, region_size, step_size, flag);
	break;
      }
    case WHITE_ROHRER_1:
      {
	code = bin_wr1(inband, outband, step_size, region_size, flag,
		       threshold, global_min, global_max,
		       intermediate_images);
	break;
      }	

    case WHITE_ROHRER_2:
      {
	code = bin_wr2(WR_OLD, threshold, distance_limit, 
		       region_size, step_size, 
		       flag,  global_max, global_min, inband, outband,
		       intermediate_images); 
	break;
      }	
    case IMPROVED_WHITE_ROHRER:
      {
	code = bin_iwr(flag, threshold,  
		       region_size,
		       global_max, global_min, inband, outband,
		       intermediate_images); 
	break;
      }	
    case WHITE_ROHRER_2_SSV:
      {
	code = bin_wr2(WR_SINGLE_VECTOR, threshold, distance_limit, 
		       region_size, step_size, 
		       flag, global_max, global_min, inband, outband,
		       intermediate_images); 
	break;
      }	

    case WHITE_ROHRER_2_DSV:
      {
	code = bin_wr2(WR_DOUBLE_VECTORS, 
		       threshold, distance_limit, 
		       region_size, step_size, 
		       flag, global_max, global_min, inband, outband,
		       intermediate_images); 
	break;
      }	

    case YANOWITZ_BRUCKSTEIN :
      {
	code = bin_yb(inband, outband, threshold, ratio, 
		      global_max);
	break;
      }
    case MANUAL :
      {
	code = bin_man(threshold, inband, outband);
	break;
      }
    default :
      {
	Error(2, "Method (%i) not implemented.\n", bin_method);
	exit(1);
	break;
      }
    }

  return BIN_OK;
} /* binarize() */

#ifdef FUNCPROTO
static int parse_command_line (int *argc, char *argv[], int *bin_method, int *region_size, int *step_size, double *distance_limit, double *weight, int *glob_min, int *glob_max,	int *threshold, int *flag, double *low_bound, double *up_bound,	double *sdev_limit, double *ratio, int *val_step, int *inv_val_step, int *intermediate_images, char **infile, char **outfile)
#else
static int parse_command_line (argc, argv, bin_method, region_size, step_size, distance_limit, weight, glob_min, glob_max, threshold, flag, low_bound, up_bound, sdev_limit, ratio, val_step, inv_val_step, intermediate_images, infile, outfile)
int *argc;
char *argv[];
int *bin_method;
int *region_size;
int *step_size;
double *distance_limit;
double *weight;
int *glob_min;
int *glob_max;
int *threshold;
int *flag;
double *low_bound;
double *up_bound;
double *sdev_limit; 
double *ratio;
int *val_step;
int *inv_val_step;
int *intermediate_images;
char **infile;
char **outfile;
#endif
{
  int no_of_parameters;
  char method_name[5];

  region_size[0] = -1;
  step_size[0] = -1;
  distance_limit[0] = -1.0;
  weight[0] = -1.0;
  glob_min[0] = -1;
  glob_max[0] = -1;
  threshold[0] = -1;
  flag[0] = -1;
  low_bound[0] = -1.0;
  up_bound[0] = -1.0;
  sdev_limit[0] = -1.0;
  ratio[0] = -1.0;

  infile[0]  = argv[*argc-2];
  outfile[0] = argv[*argc-1];
  strncpy(method_name, read_switch(argc, argv, "-m", 1, "ETM"), 4);
  val_step[0]            = read_iswitch(argc, argv, "-v", 0);
  inv_val_step[0]        = read_iswitch(argc, argv, "-iv", 0);
  intermediate_images[0] = read_bswitch(argc, argv, "-iim");

  if (strcmp(method_name, "Ab") == 0)
    {
      bin_method[0]     = ABUTALEB;
    }
  else if (strcmp (method_name, "Be") == 0)
    {
      bin_method[0]     = BERNSEN;
      region_size[0]    = read_iswitch(argc, argv, "-r", 9);
      distance_limit[0] = atoi(read_switch(argc, argv, "-c", 1, "15.0"));
      flag[0]           = read_bswitch(argc, argv, "-low");
    }
  else if (  (strcmp (method_name, "dnm") == 0) 
	   ||(strcmp (method_name, "ETM") == 0))
    {
      bin_method[0]     = EIKVIL_TAXT_MOEN;
      region_size[0]    = read_iswitch(argc, argv, "-r", 16);
      step_size[0]      = read_iswitch(argc, argv, "-s", 3);
      distance_limit[0] = read_dswitch(argc, argv, "-l", 15.0);
      weight[0]         = read_dswitch(argc, argv, "-w", 0.9);
      glob_min[0]       = read_iswitch(argc, argv, "-mi", 0);
      glob_max[0]       = read_iswitch(argc, argv, "-ma", 255);
    }
  else if (strcmp (method_name, "new") == 0)
    {
      bin_method[0]     = DNM_NEW;
      region_size[0]    = read_iswitch(argc, argv, "-r", 16);
      step_size[0]      = read_iswitch(argc, argv, "-s", 3);
      distance_limit[0] = read_dswitch(argc, argv, "-l", 15.0);
      weight[0]         = read_dswitch(argc, argv, "-w", 0.9);
      glob_min[0]       = read_iswitch(argc, argv, "-mi", 0);
      glob_max[0]       = read_iswitch(argc, argv, "-ma", 255);
    }
  else if (strcmp (method_name, "Ot") == 0)
    {
      bin_method[0]     = OTSU;
    }
  else if (strcmp(method_name, "KSW") == 0)
    {
      bin_method[0]     = KAPUR_SAHOO_WONG;
    }
  else if (strcmp (method_name, "KI") == 0)
    {
      bin_method[0]     = KITTLER_ILLINGWORTH;
    }
  else if (strcmp(method_name ,"MH") == 0)
    {
      bin_method[0]     = MARIDA_HAINSWORTH;
      flag[0]           = read_bswitch(argc, argv, "-s");
      distance_limit[0] = read_dswitch(argc, argv, "-d", 1.0);
      no_of_parameters = 0;
    }
  else if (  (strcmp (method_name, "NR") == 0)
	   ||(strcmp (method_name, "CK") == 0))
    {
      bin_method[0] = NAKAGAWA_ROSENFELD;
      region_size[0]    = read_iswitch(argc, argv, "-r", 32);
      distance_limit[0] = read_dswitch(argc, argv, "-m", 24.0);
      low_bound[0]      = read_dswitch(argc, argv, "-mi", 0.1);
      up_bound[0]       = read_dswitch(argc, argv, "-ma", 10.0);
      sdev_limit[0]     = read_dswitch(argc, argv, "-s", 16.0);
      ratio[0]          = read_dswitch(argc, argv, "-vtp", 0.8);      
    }
  else if (strcmp (method_name, "Ni") == 0)
    {
      bin_method[0] = NIBLACK;
      region_size[0]    = read_iswitch(argc, argv, "-r", 15);
      weight[0]         = read_dswitch(argc, argv, "-k", -0.2);
    }
  else if (strcmp (method_name, "Pa") == 0)
    {
      bin_method[0]     = PARKER;
      region_size[0]    = read_iswitch(argc, argv, "-r", 16);
      distance_limit[0] = read_dswitch(argc, argv, "-m", -1.0);
      sdev_limit[0]     = read_dswitch(argc, argv, "-s", 1.0);
      weight[0]         = read_dswitch(argc, argv, "-k", -1.0); 
      flag[0]           = read_bswitch(argc, argv, "-a");
}
  else if (strcmp (method_name, "Sh") == 0)
    {
      bin_method[0] = SHIO;
    }
  else if (strcmp(method_name, "TFJ") == 0)
    {
      bin_method[0]     = TAXT_FLYNN_JAIN;
      region_size[0]    = read_iswitch(argc, argv, "-r", 32);
      step_size[0]      = read_iswitch(argc, argv, "-s", 32);
      flag[0]           = read_iswitch(argc, argv, "-t", 125);
    }
  else if (strcmp (method_name, "WR1") == 0)
    {
      bin_method[0]     = WHITE_ROHRER_1;
      /* Number of lookahead pixels in x direction: */
      step_size[0]      = read_iswitch(argc, argv, "-lx" , 8);
      /* Number of lookahead lines: */
      region_size[0]    = read_iswitch(argc, argv, "-ly" , 1);
      flag[0]           = read_iswitch(argc, argv, "-m", 0);
      threshold[0]      = read_iswitch(argc, argv, "-b", 100);
      glob_min[0]       = read_iswitch(argc, argv, "-f", 100);
      glob_max[0]       = read_iswitch(argc, argv, "-g", 100);
    }
  else if (strcmp (method_name, "WR2") == 0)
    {
      bin_method[0]     = WHITE_ROHRER_2;
      /* Activity limit. [-a -1] means activity limit will be selected 
	 automatically.  */
      threshold[0] = read_iswitch(argc, argv, "-a", 50);
      /* Maximum value for automatically selected activity limit : */
      glob_max[0]       = read_iswitch(argc, argv, "-ma", 255);
      /* Mean filter size: */
      region_size[0]    = read_iswitch(argc, argv, "-m", 1);
      /* Length of search vector: */
      step_size[0]      = read_iswitch(argc, argv, "-s", 128);
      /* Lower bound on distance between edges: */
      distance_limit[0] = read_dswitch(argc, argv, "-l", 4.0);
      /* Minimum size for spots in the label image : */
      glob_min[0]       = read_iswitch(argc, argv, "-sl", 0);
      flag[0]           = read_bswitch(argc, argv, "-d");
      if ( read_bswitch(argc, argv, "-single") )
	bin_method[0] = WHITE_ROHRER_2_SSV;
      if ( read_bswitch(argc, argv, "-double") )
	bin_method[0] = WHITE_ROHRER_2_DSV;
    }
  else if (strcmp (method_name, "IWR") == 0)
    {
      bin_method[0] = IMPROVED_WHITE_ROHRER;
      /* Activity limit. [-a 0] means activity limit will be selected 
	 automatically.  */
      threshold[0] = read_iswitch(argc, argv, "-a", 20);
      /* Maximum value for automatically selected activity limit : */
      glob_max[0]       = read_iswitch(argc, argv, "-ma", 255);
      /* Mean filter size: */
      region_size[0]    = read_iswitch(argc, argv, "-m", 5);
      /* Minimum size for spots in the label image : */
      glob_min[0]       = read_iswitch(argc, argv, "-sl", 0);
      flag[0] = WR_NULL_MASK;
      if (read_bswitch(argc, argv, "+n"))
	 flag[0] = 0;
      if (read_bswitch(argc, argv, "-p")) 
	 flag[0] += WR_PLUS_MASK;
    }
  else if (strcmp (method_name, "YB") == 0)
    {
      bin_method[0]     = YANOWITZ_BRUCKSTEIN;
      threshold[0]      = read_iswitch(argc, argv, "-t", 0);
      ratio[0]          = read_dswitch(argc, argv, "-b", 1.00);
      glob_max[0]       = read_iswitch(argc, argv, "-m", 50);
      flag[0]           = read_bswitch(argc, argv, "-i");
    }
  else if (strcmp (method_name, "man") == 0)
    {
      bin_method[0] = MANUAL;
      threshold[0] = read_iswitch(argc, argv, "-t", 127);
    }
  else 
    {
      bin_method[0] = DEFAULT;
    }
  return BIN_OK;
} /* parse_command_line() */

#ifdef FUNCPROTO
int main(int argc, char *argv[])
#else
int main (argc, argv)
int argc;
char *argv[];
#endif
{
  char  *infile;
  char  *outfile;
  char  *args;
  int   xsize, ysize;
  long  start_time;
  long  elapsed_time;
  int   bin_method;
  int   error_code;

  int region_size;
  int step_size;
  double weight;
  double distance_limit;
  int global_min;
  int global_max;
  int threshold;
  int flag;
  double sdev_limit;
  double ratio;
  double low_bound;
  double up_bound;
  int validation, inv_validation;
  int intermediate_images, minsize;
  
  IMAGE inimage, outimage;
  IBAND inband, outband;
  IBAND segmented, no_ghosts;

  Iset_message(TRUE);
  Iset_abort(TRUE);

  InitMessage(&argc, argv, xite_app_std_usage_text(
    "Usage: %s [-m <method_name_abbrev>]\n\
       [<method_spesific_parameters>]\n\
       [-v <validation_step_threshold>]\n\
       [-iv <inv_val_step_threshold>]\n\
       [-iim]\n\
       <infile> <outfile>\n\
     \n\
       method_name_abbrev / -name        parameter switches \n\
       ----------------------------------------------------------------\n\
       Ab  (Abutaleb, 1988)               \n\
       Be  (Bernsen, 1986)               -r -d -mi -ma -low\n\
       ETM (Eikvil, Taxt & Moen, 1991)   -r -s -l -w -mi -ma \n\
       KI  (Kittler & Illingworth, 1986)  \n\
       KSW (Kapur, Sahoo & Wong, 1985)    \n\
       MH  (Mardia & Hainsworth, 1988)   -s -d\n\
       NR  (Nakagawa & Rosenfeld, 1979)  -s -m -mi -ma -vtp -r\n\
       Ni  (Niblack, 1986)               -r -k\n\
       Ot  (Otsu, 1979)                   \n\
       Pa  (Parker, 1991)                 \n\
       Sh  (Shio, 1989)                   \n\
       TFJ (Taxt, Flynn & Jain, 1989)    -r -s -t\n\
       WR1 (White & Rohrer I (IBM), 1983)  -l\n\
       WR2 (White & Rohrer II(IBM), 1983)  -a -l -m -s -ma -d -single -2x2\n\
       IWR (Impr. White & Rohrer, 1994)    -a -sl -ma -n +n -p +p\n\
       YB  (Yanowitz & Bruckstein, 1988) -b -t -m \n\
       man (Manual threshold value)      -t \n"));

  /* Parse command line parameters */

  if (argc == 1) Usage(1, NULL);
  args = argvOptions(argc, argv);

  parse_command_line (&argc, argv, &bin_method, 
		      &region_size, &step_size, &distance_limit,
		      &weight, &global_min, &global_max, 
		      &threshold, &flag,
		      &low_bound, &up_bound,
		      &sdev_limit, &ratio,
		      &validation, &inv_validation,
		      &intermediate_images,
		      &infile, &outfile);

  if (argc != 3) Usage(1, "Illegal number of arguments.\n");

  if (bin_method == DEFAULT)
    Usage(2, "%s\n%s\n",
	  "No binarization method or non-existent binarization",
	  "method specified.");

  /* Start the clock */

  start_time = clock();

  /* Read input image from file */

  inimage = Iread_image(infile);
  inband  = (IBAND)inimage[1]; /* First band only is used. */

  xsize = Ixsize(inband);
  ysize = Iysize(inband);
  minsize = MIN(Ixsize(inband),Iysize(inband));

  if (region_size >= minsize) {
    region_size = minsize-2;
    if (Verbose()) {
      printf("%s\n%s\n", "Region size > image size.",
	     "Region size used is minimum of xsize and ysize of image."); 
    }
  }

  /* Make data structure for output image */

  outimage = Imake_image(1, outfile, Ipixtyp(inband), xsize, ysize);
  outband  = (IBAND) outimage[1];

  segmented = Imake_band(Iu_byte_typ, xsize, ysize);

  /* Choose binarization method and put result of binarization
     in output image. */

  binarize(bin_method, 
	   region_size, step_size, distance_limit,
	   weight, global_min, global_max, threshold, flag,
	   low_bound, up_bound, sdev_limit, ratio, intermediate_images,
	   inband, segmented);

  if (validation != 0)
    {
      if (intermediate_images) write_byteband(segmented, "segmented.img");
      if (validation < 0)
	{
	  mark_ghost(segmented, inband, outband, -validation);
	  Idel_band (&segmented);
	}
      else if (inv_validation > 0) 	
	{	
	  no_ghosts = Imake_band(Iu_byte_typ, xsize, ysize);
	  remove_ghost(segmented, inband, no_ghosts, validation);
	  if (intermediate_images) write_byteband(no_ghosts, "no_ghosts.img");
	  Idel_band (&segmented);
	  remove_inv_ghost(no_ghosts, inband, outband, inv_validation);
	  Idel_band (&no_ghosts);
	}
      else
	{
	  remove_ghost(segmented, inband, outband, validation);
	  Idel_band (&segmented);
	}
    }
  else if (inv_validation != 0)
    {
      if (intermediate_images) write_byteband(segmented, "segmented.img");
      if (inv_validation < 0)
	{
	  mark_inv_ghost(segmented, inband, outband, -inv_validation);
	  Idel_band (&segmented);
	}
      else
	{	
	  remove_inv_ghost(segmented, inband, outband, inv_validation);
	  Idel_band (&segmented);
	}
    }
  else
    {
      Icopy_band(segmented, outband);
      Idel_band (&segmented);
    }

  /* Write output image to file */

  Ihistory(outimage, argv[0], args);
  error_code = Iwrite_image(outimage, outfile);

  elapsed_time = clock() - start_time;
  if (Verbose()) printf("Time used (CPU): %fs\n", elapsed_time / 1e6);

  return 0; /* Unix commands should return 0 */
}

#endif /* MAIN */
