
/*

________________________________________________________________

        statistics
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
#include <xite/cdoc.h>
#include <math.h>
#include <xite/biff.h>
#include <stdlib.h>
#include <xite/statistics.h>
#include <xite/histo.h>
#include <xite/message.h>
#include <xite/utl_malloc.h>
#include <xite/readarg.h>
#include XITE_STRING_H
#include XITE_STDIO_H



#define SHORT_LF_FORMAT "%11.6lg"
#define LONG_LF_FORMAT  "%22.16lg"
#define SHORT_LD_FORMAT "%11ld"
#define LONG_LD_FORMAT  "%22ld"
#define SHORT_D_FORMAT "%11d"
#define LONG_D_FORMAT  "%22d"

#ifndef MAX
# define MAX(a, b) ( (a) >= (b) ? (a) : (b) )
#endif /* MAX */

/*P:statistics*

________________________________________________________________

		statistics
________________________________________________________________

Name:		statistics - extract statistical information from an image

Syntax:		| statistics [<option>...] <image> [<image>...]

Description:	Calculates minimum, maximum, median, mean and standard
                deviation for each band in an image. All values
                are written to stdout. For complex-valued image bands, minimum
		and maximum values are calculated as minimum and maximum
		absolute values.
		
		The calculations may be done on the whole image,
                or a mask may be specified.

Options:        &-r file
                Use region mask 'file' which must be a BIFF unsigned byte image
		file. For all the mask pixels with a certain value, calculate
		the statistics for image pixels at the same coordinates.
		Repeat for all mask values.

		&-p value
		Calculate and print statistics only for the mask value 'value'
		(i.e. only for those image pixels with the same location as
		the mask pixels with value equal to 'value'). This option
		requires the '-r' option.

		&-a
		Use long output format. Default is "11.6lg"
		(6 significant digits) for floating-point, "11ld" for
		integers. Long format is "22.16lg" (16 significant digits)
		for floating-point, "22ld" for integers. The exact meaning
		of the format specification is the same as for the C
		function printf(3).

		-&If one of the options [flumns] is used, the
		output is unformatted (unless option '-h' is used), and only
		the indicated statistic is calculated.

		&-h
		Horizontal format information, even if one of the options
		[flumns] is used.
		
		&-f
		Print filename and bandnumber (and masknumber if -r is used).

		&-l
		Print minimum value.

		&-m
		Print mean.

		&-n
		Print median (Unsigned byte image only).

                &-s
		Print standard deviation.

                &-u
		Print maximum value.

Restrictions:	Median works for unsigned byte images only.
		Mask image must have pixel type unsigned byte.

See also:       statistics(3)

Author:		Tor Lønnestad and Otto Milvang, BLAB, Ifi, UiO

Examples:	statistics mona.img

Id:             $Id$
________________________________________________________________

*/

static void readsw(int *argc, char *argv[], char **region, int *format, int *name, int *n, int *ca, int *l, int *u, int *m, int *s, int *p, int *h, int *a)
{
  *a      = read_bswitch(argc, argv, "-a");
  *ca     = read_bswitch(argc, argv, "-c");
  *name   = read_bswitch(argc, argv, "-f");
  *h      = read_bswitch(argc, argv, "-h");
  *l      = read_bswitch(argc, argv, "-l");
  *m      = read_bswitch(argc, argv, "-m");
  *n      = read_bswitch(argc, argv, "-n");
  *s      = read_bswitch(argc, argv, "-s");
  *u      = read_bswitch(argc, argv, "-u");
  *p      = read_iswitch(argc, argv, "-p", -1);
  *region = read_switch (argc, argv, "-r", 1, NULL);

  if ((*ca) || (*name) || (*h) || (*l) || (*m) || (*n) || (*s) || (*u))
    *format = FALSE;

  if (!(*ca || *name || *l || *m || *n || *s || *u))
    *l = *u = *m = *n = *s = *ca = TRUE;
}

static int readfile(char *name, char **filename, IMAGE *img)
{
  if (name[0] == '-') {
    /* Reading from a pipe. */

    *img = Iread_image(name);
    *filename = NULL;
    if (*img == 0) exit(0);
  } else {
    *filename = name;
    *img = Iread_image(name);
    if (*img == 0) return(2);
  } 
  return(Iok);
}
      
int main(int argc, char **argv)
{
  IMAGE img, mask=NULL;
  int i, bn, mn, stat;
  int rstat, ps, pe, room;
  size_t fname_len, sp_len;
  int n=FALSE, l = FALSE, u = FALSE;
  int name = FALSE, format = TRUE, long_format = FALSE;
  int he = FALSE, c=FALSE, m=FALSE, s=FALSE, p = -1;
  char *filename = NULL, *regionfile = NULL, *fname = NULL, *form_lf_string;
  char *form_ld_string, *form_d_string, formStr[512];
  int imin, imax, imedian, argnum;
  long size[256];
  double min[256], max[256], mean[256], stdev[256];
  DCOMPLEX c_mean;
  int compl = 0;

  Iset_message(TRUE);
  Iset_abort(TRUE);

  InitMessage(&argc, argv, xite_app_std_usage_text(
    "Usage: %s [<option>...] <imagefile>... \n\
       where <option> is chosen from\n\
       -r <file>  : Use region mask file.\n\
       -p <value> : Calculate statistics only for the mask value <value>.\n\
       -a         : Use long floating-point and integer output format.\n\
       -f         : Print filename and bandnumber (and masknumber with -r).\n\
       -h         : Horizontal output format (including text), even if an\n\
                    option [flunms] is used.\n\
       -l         : Print min value.\n\
       -m         : Print mean.\n\
       -n         : Print median (unsigned byte image only).\n\
       -s         : Print standard deviation.\n\
       -u         : Print max value.\n"));

  if (argc == 1) Usage(1, NULL);

  readsw(&argc, argv, &regionfile, &format, &name, &n, &c, 
	 &l, &u, &m, &s, &p, &he, &long_format);

  if (he) format = FALSE;
  if (!long_format) {
    form_lf_string = SHORT_LF_FORMAT;
    form_ld_string = SHORT_LD_FORMAT;
    form_d_string  = SHORT_D_FORMAT;
  } else {
    form_lf_string = LONG_LF_FORMAT;
    form_ld_string = LONG_LD_FORMAT;
    form_d_string  = LONG_D_FORMAT;
  }

  if (regionfile) mask = Iread_image(regionfile);
  if (mask && Ipixtyp(mask[1]) != Iu_byte_typ)
    Error(2, "Mask image must have pixeltype unsigned byte.\n");
      
  /* Treat remaining command-line arguments as image filenames. */

  argnum = 1;
  while (argc - argnum > 0) {
    rstat = readfile(argv[argnum++], &filename, &img);
    if (rstat) Error(2, "Error reading file %s\n",argv[--argnum]);

    fname = filename == NULL ? "stdin" : filename;
    fname_len = strlen(fname);

    mn = 1;
    for (bn=1; bn <= Inbands(img);  bn++) {
      imedian = -1; /* Changed only if unsigned-byte and no mask. */
      compl   = FALSE;

      if (mask && (p < 0))
	stat = statisticMasks
	  (img[bn], mask[mn], size, min, max, mean, stdev);
      else if (mask && (p>=0)) {
	stat = statisticMask
	  (img[bn], mask[mn], &size[p], &min[p], &max[p],
	   &mean[p], &stdev[p], p);
      }
      else if (Ipixtyp(img[bn]) == Iu_byte_typ)
	stat = statistics(img[bn], &imin, &imax, &imedian, mean, stdev);
      else if (Ipixtyp(img[bn]) == Icomplex_typ ||
	       Ipixtyp(img[bn]) == Id_complex_typ) {
	compl = TRUE;
	stat = cstatistic(img[bn], size, min, max, &c_mean, stdev);
      }
      else
	stat = statistic(img[bn], size, min, max, mean, stdev);

      if (stat == 2) {
	if (filename)
	  Warning(3, "Bad pixeltype for band number %d in %s.\n",
		  bn, filename);
	else
	  Warning(3, "Bad pixeltype for band number %d.\n", bn);
	continue;
      } else if (stat == 1) {
	if (filename)
	  Warning(3, "Mask and band number %d not same size in %s.\n",
		  bn, filename);
	else
	  Warning(3, "Mask and band number %d not same size.\n", bn);
	continue;
      }
      
      /* size has not been computed in the no-mask unsigned-byte case. */
      if (imedian >=0) size[0] = Ixsize(img[bn]) * Iysize(img[bn]);
      
      ps = p>=0 ? p : 0;
      pe = p>=0 ? p : 255;
      if (!mask) ps = pe = 0;

      if (format) {
	/* Print all statistics, median only if no-mask unsigned-byte. */
	for (i=ps; i<=pe; i++) {
	  if (size[i] == 0) continue; /* No mask pixels with this values. */ 
	  if (mask)
	    printf("\n Band %d, Plane %d : \n\n", bn, i);
	  else
	    printf("\n Band %d : \n\n",bn);
	  
	  sprintf(formStr, "%s%s\n%s%s\n%s%s\n", "   Size  : ",
		  form_ld_string,
		  "   Xsize : ", form_ld_string, "   Ysize : ",
		  form_ld_string);
	  printf(formStr, size[i], Ixsize(img[bn]), Iysize(img[bn]));
	  
	  if (imedian >= 0) {
	    sprintf(formStr, "%s%s\n%s%s\n", "   Min   : ", form_d_string,
		    "   Max   : ", form_d_string);
	    printf(formStr, imin, imax);
	  }
	  else {
	    sprintf(formStr, "%s%s\n%s%s\n", "   Min   : ", form_lf_string,
		    "   Max   : ", form_lf_string);
	    printf(formStr, min[i], max[i]);
	  }
	  if (imedian >= 0) {
	    sprintf(formStr, "%s%s\n", "   Median: ", form_d_string);
	    printf(formStr, imedian);
	  }
	  
	  if (!compl) {
	    sprintf(formStr, "%s%s\n%s%s\n", "   Mean  : ", form_lf_string,
		    "   St.dev: ", form_lf_string);
	    printf(formStr, mean[i], stdev[i]);
	  } else {
	    char *sgn = "+";

	    if (c_mean.im < 0) sgn[0] = '\0';

	    sprintf(formStr, "%s%s%s%sj\n%s%s\n", "   Mean  : ",
		    form_lf_string, sgn, form_lf_string,
		    "   St.dev: ", form_lf_string);
	    printf(formStr, c_mean.re, c_mean.im, stdev[i]);
	  }
	  
	}
      } else if (he) {
	/* Print only selected statistics with headers,
	 * median only if no-mask unsigned-byte. */
	
	if (mask)
	  sp_len = MAX(fname_len, strlen("Filename")) + strlen(" band") +
	    strlen(" mask");
	else
	  sp_len = MAX(fname_len, strlen("Filename")) + strlen(" band");
	
	if (name) {
	  /* Write filename etc. */
	  if (mask) {
	    /* Include mask value. */
	    room = sp_len - strlen("Filename") - strlen(" band") -
	      strlen(" mask");
	    if (room > 0) {
	      printf("%*c%s%s%s", room, ' ',
		     "Filename", " band", " mask");
	    } else {
	      printf("%s%s%s", "Filename", " band", " mask");
	    }
	  } else {
	    /* No mask value. */
	    room = sp_len - strlen("Filename") - strlen(" band");
	    if (room > 0) {
	      printf("%*c%s%s", room, ' ', "Filename", " band");
	    } else {
	      printf("%s%s", "Filename", " band");
	    }
	  }
	} /* name */
	
	if (!long_format) {
	  if (l) printf("         Min");
	  if (u) printf("         Max");
	  if (n && imedian >= 0) printf("      Median");
	  if (m) printf("        Mean");
	  if (s) printf("     St.dev.");
	} else {
	  if (l) printf("                    Min");
	  if (u) printf("                    Max");
	  if (n && imedian >= 0) printf("                 Median");
	  if (m) printf("                   Mean");
	  if (s) printf("                St.dev.");
	}
	if (l || u || (n && imedian >= 0) || m || s || name) printf("\n");
	
	for (i=ps; i<=pe; i++) {
	  if (size[i] == 0) continue;
	  
	  if (name) {
	    if (mask) {
	      /* Include mask value. */
	      room = sp_len - fname_len - strlen(" band") - strlen(" mask");
	      if (room > 0) {
		printf("%*c%s %4d %4d", room, ' ', fname, bn, i);
	      } else {
		printf("%s %4d %4d", fname, bn, i);
	      }
	    } else {
	      /* No mask value. */
	      room = sp_len - fname_len - strlen(" band");
	      if (room > 0) {
		printf("%*c%s %4d", room, ' ', fname, bn);
	      } else {
		printf("%s %4d", fname, bn);
	      }
	    }
	  }
	  
	  if (imedian >= 0) {
	    sprintf(formStr, " %s", form_d_string);
	    if (l) printf(formStr, imin);
	    if (u) printf(formStr, imax);
	    if (n) printf(formStr, imedian); 
	  }
	  else {
	    sprintf(formStr, " %s", form_lf_string);
	    if (l) printf(formStr, min[i]);
	    if (u) printf(formStr, max[i]);
	  }
	  
	  if (!compl) {
	    if (m) {
	      sprintf(formStr, " %s", form_lf_string);
	      printf(formStr, mean[i]);
	    }
	  } else {
	    if (m) {
	      char *sgn = "+";

	      if (c_mean.im < 0) sgn[0] = '\0';

	      sprintf(formStr, " %s%s%sj", form_lf_string, sgn,
		      form_lf_string);
	      printf(formStr, c_mean.re, c_mean.im);
	    }
	  }
	  sprintf(formStr, " %s", form_lf_string);
	  if (s) printf(formStr, stdev[i]);
	  if (l || u || (n && imedian >= 0) || m || s || name) printf("\n");
	}
      }
      else {
	/* Print only selected statistics with no headers,
	 * median only if no-mask unsigned-byte. */
	for (i=ps; i<=pe; i++) {
	  if (size[i] == 0) continue;
	  if (name) { 
	    if (mask) printf("%s %3d %3d", fname, bn, i); 
	    else printf("%s %3d ", fname, bn);
	  }
	  if (imedian >= 0) {
	    sprintf(formStr, " %s", form_d_string);
	    if (l) printf(formStr, imin);
	    if (u) printf(formStr, imax);
	    if (n) printf(formStr, imedian);
	  } else {
	    sprintf(formStr, " %s", form_lf_string);
	    if (l) printf(formStr, min[i]);
	    if (u) printf(formStr, max[i]);
	  }
	  
	  if (!compl) {
	    if (m) {
	      sprintf(formStr, " %s", form_lf_string);
	      printf(formStr, mean[i]);
	    }
	  } else {
	    if (m) {
	      char *sgn = "+";

	      if (c_mean.im < 0) sgn[0] = '\0';

	      sprintf(formStr, " %s%s%sj", form_lf_string, sgn,
		      form_lf_string);
	      printf(formStr, c_mean.re, c_mean.im);
	    }
	  }
	  sprintf(formStr, " %s", form_lf_string);
	  if (s) printf(formStr, stdev[i]);
	  if (l || u || (n && imedian >= 0) || m || s || name) printf("\n");
	}
      } /* else */
      mn++;
      if (mask && (mn > Inbands(mask))) mn = 1;
    } /* for each band */
    if (format) printf("\n");
    Idel_image(&img);
  } /* while arguments */

  return(0);
}  /* main */
