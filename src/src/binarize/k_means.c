

/*

________________________________________________________________

        k_means.c
        $Id$
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

/* static char *Id = "$Id$, Blab, UiO"; */



/*L:k_means*

________________________________________________________________

		k_means
________________________________________________________________

Name:		k_means  -  k-means clustering
Syntax:		int k_means(histogram H, int nof_clusters, int *thresholds)
Description:    Divides the histogram into "natural" clusters.
                The result is an array of thresholds separating the
		clusters. For each cluster, the upper threshold value
		is inside the cluster, but the lower is not. 
Files:		src/binarize/k_means.c
References:	Anil K. Jain and Richard C. Dubes, "Algorithms for
                Clustering Data", Prentice Hall, 1988.
Author:		Oivind Due Trier
Examples:	k_means(H, 2, &threshold);

Id: 		$Id$
________________________________________________________________

*/

/*
#include <xite/includes.h>
#include XITE_STDIO_H 
#include <xite/blab.h>
#include <xite/histo.h>
#include <xite/readarg.h>
#include <math.h>
#include <xite/binarize.h>
*/
#include XITE_MALLOC_H

#ifdef FUNCPROTO
static int sort(int *c, int n)
#else
static int sort(c, n)
     int *c;
     int n;
#endif
{
  int i, j;
  int temp;
  for (i=0; i<n-1; i++)
    {
      for (j=i+1; j<n; j++)
	if (c[i] > c[j])
	  {
	    temp = c[i];
	    c[i] = c[j];
	    c[j] = temp;
	  }
    }
  return BIN_OK;
}



#ifdef FUNCPROTO
static int k_initial_part(histogram H, int nof_clusters, 
		   int* center)
#else
static int k_initial_part(H, nof_clusters, 
		   center)
     histogram H;
     int nof_clusters;
     int* center;
#endif
{
  int i;
  int nof_pix;
  int this_dist, this_min_dist, dist; 
  int sum;
  int d, c;

  nof_pix = 0;
  for (i=0; i<=BIN_BACKGROUND; i++)
    nof_pix += H[i];

  sum = 0;
  for (i=0; i<=BIN_BACKGROUND; i++)
    sum += i*H[i];

  center[0] = (sum/nof_pix);
  for (c=1; c<nof_clusters; c++)
    {
      dist = 0;
      for (i=0; i<=BIN_BACKGROUND; i++)
	{
	  this_min_dist = 256;
	  for (d=0; d<c; d++)
	    {
	      this_dist = abs(center[d] - i);
	      if ((this_min_dist > this_dist) && (H[i] > 0))
		this_min_dist = this_dist;
	    }	
	  if ((dist < this_min_dist) && (H[i] > 0))
	    {	
	      center[c] = i;
	      dist = this_min_dist;
	    }
	}
    }

  return 1;
}



#ifdef FUNCPROTO
static int k_new_part(histogram H, int nof_clusters, 
	       int* center, histogram partition)
#else
static int k_new_part(H, nof_clusters, 
	       center, partition)
     histogram H;
     int nof_clusters;
     int* center;
     histogram partition;
#endif
{	
  int i;
  int tot_dist, this_dist, dist;
  int c;
  int *sum, *n;

  if ((sum = (int*) malloc(nof_clusters * sizeof(int))) == NULL)
    return BIN_NO_MEMORY;
  if ((n = (int*) malloc(nof_clusters * sizeof(int))) == NULL)
    return BIN_NO_MEMORY;

  /* Partition the pixels by assigning each pixel to the cluster with 
     the closest center */
  for (i=0; i<=BIN_BACKGROUND; i++)
    {
      dist = 256;
      partition[i] = -1;
      for (c=0; c<nof_clusters; c++)
	{
	  this_dist = abs(i - center[c]);
	  if (this_dist<dist)
	    {
	      dist = this_dist;
	      partition[i] = c;
	    }	
	  }	
      }	
  
  /* Calculate new cluster centers */
  for (c=0; c<nof_clusters; c++)
    {
      sum[c] = 0;
      n[c] = 0;
    }
  for (i=0; i<=BIN_BACKGROUND; i++)
    {
      if (partition[i] == -1)
	  Warning(1, "Error at histogram position [%i].\n",i);
	sum[partition[i]] += H[i]*i;
	n[partition[i]] += H[i];
      }
  for (c=0; c<nof_clusters; c++)
    if (n[c] > 0)
      center[c] = sum[c] / n[c];
    else 
      Warning(1, "Warning! Empty cluster. Use old center.\n");

  tot_dist = 0;
  for (i=0; i<=BIN_BACKGROUND; i++)
    tot_dist += H[i] * abs(center[partition[i]] - i);

  free(sum);
  free(n);
  return tot_dist;
}	


#ifdef FUNCPROTO
static int k_means(histogram H, int nof_clusters, int *thresholds)
#else
static int k_means(H, nof_clusters, thresholds)
     histogram H;
     int nof_clusters;
     int *thresholds;
#endif
{
  int *center;
  histogram partition;
  int iterations;
  int improvement;
  int old_value, new_value;
  int i;

  if ((center = (int*) malloc (nof_clusters * sizeof(int))) == NULL)
    return BIN_NO_MEMORY;

  k_initial_part(H, nof_clusters, center);

  iterations = 0;
  improvement = 1000;
  old_value = 10000;

  if (Verbose()) {
    printf("k-means pass %i. Centers: ", iterations);
    for (i=0; i<nof_clusters; i++)
      printf(" H[%3i]=%i ", center[i], H[center[i]]);
    printf("\n");
  }

  while ((improvement > 1) && (iterations < 20))
    {
      iterations ++;
      new_value = k_new_part(H, nof_clusters, center, partition);

      if (Verbose()) {
	printf("k-means pass %i. Centers: ", iterations);
	for (i=0; i<nof_clusters; i++)
	  printf("%4i", center[i]);
	printf("\n");
      }

      improvement = abs(old_value - new_value);
      old_value = new_value;
    }

  sort (center, nof_clusters);
  for (i=1; i<nof_clusters; i++)
    thresholds[i-1] = (center[i-1] + center[i]) / 2;
  free(center);
  return 1;
}
