
/*

________________________________________________________________

        biffinfo
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



/*P:biffinfo*

________________________________________________________________

		biffinfo
________________________________________________________________

Name:		biffinfo - extract information from BIFF file

Syntax:		| biffinfo [-f] [-t] [-b] [-u] [-h] [-i] [c] \\
                |     [-n <band-spec>] [<filename>...] [<dirname>...]

Description:    'biffinfo' reads the info block of one or more images or all
                images in a directory. If no options are given, the info will
                be written formatted. See examples below.

Options:	If one or more options are specified (exept -i and -c),
		the output will be without explanatory text.
                
                &-f
		Print the filename.

                &-b
		Print the number of bands.

                &-n band-spec
		Print info for bands given by standard XITE band
		specification, i.e. print band number, pixel type,
		horizontal size, vertical size, horizontal start coordinate,
		vertical start coordinate, horizontal magnification factor,
		vertical magnification factor.

                &-u
		Print info for all bands. Overrides '-n'.

		&-t
		Print the image title.

                &-h
		Print text (history)  block.

		&-i
		Print for images only.

		&-c
		Print for colortables only.

		-&You may specify more than one option, but they must be
		space-separated. Without explanatory text, the rules are:

		&When -n or -u is specified
		Combine -f, -b, -n, -u, -t to one output line/band.
		|    <filename> <bands> <bandinfo> <title>
		|    Print text (-h)

		&Otherwise:
		Combine -f, -b, -t, to one line/image
		|    <filename> <bands> <title>
		|    Print text (-h)
		
Return value:   0 = success, 2 = failure

Author:		Tor Lønnestad and Otto Milvang

Examples:       
                &Type out all info about a BIFF file
		| >> biffinfo mona.img
		| 
		| Filename: mona.img
		| Title   : MONA                            
		| Bands   : 3
		| Textlen : 0
		| Band      Pixeltype Xsize Ysize Xstart Ystart Xmag Ymag
		|    1      uns_byte   512   512    1      1      1    1
		|    2      uns_byte   512   768    1      1      1    1
		|    3      uns_byte   256   256    1      1      1    1
		| Textfield :
		| Fri Jan 11 15:54:05 1991 addw -w1 1.00 -w2 1.00

		&Print the number of bands in mona.img
                | >> biffinfo -b mona.img
                | 3

		&Print title of 2 images sent as standard input
                | cat mona1.img mona2.img | biffinfo -t - -
                | MONA 1
                | MONA 2
                |

		&Formated output: filename bands bandinfo
		| >> biffinfo -f -b -u mona.img
		| mona.img 3 1 3 512 512 1 1 1 1
		| mona.img 3 2 3 512 768 1 1 1 1
		| mona.img 3 3 3 256 256 1 1 1 1

		&List all images in a directory
		| >> biffinfo -i -f /local/blab/img
		| /local/blab/mona.img
		| /local/blab/mona1.img
		| /local/blab/mona2.img

		&Use band specification on file name
		| >> biffinfo reine_rgb.img:2
		| Filename : reine_rgb.img
		| Title    : Reine, Lofoten, Norway
		| Bands    : 1
		| Textlen  : 0
		| Band       Pixeltype Xsize Ysize Xstart Ystart Xmag Ymag
		| 1           uns_byte  512   512    1      1      1   1

		&Combine band spec with option -n
		| >> biffinfo -n 2 reine_rgb.img:1:3:2
		| 2  3 512  512   1   1   1   1
		The band number here is 2, i.e. the second band of the
		image which contains bands from reine_rgb.img in the order
		1, 3, 2.

Id: 		$Id$
________________________________________________________________

*/

#include <xite/includes.h>
#include <xite/cdoc.h>
#include <xite/biff.h>
#include <stdlib.h>
#include <errno.h>
#include <xite/blab.h>
#include <xite/utils.h>
#include XITE_TYPES_H

#ifdef HAVE_DIRENT_H
# include <dirent.h>
# define NAMLEN(dirent) strlen ((dirent)->d_name)
#else
# define dirent direct
# define NAMLEN(dirent) ((dirent)->d_namlen)
# ifdef HAVE_SYS_NDIR_H
#  include <sys/ndir.h>
# endif
# ifdef HAVE_SYS_DIR_H
#  include <sys/dir.h>
# endif
# ifdef HAVE_NDIR_H
#  include <ndir.h>
# endif
#endif

#include XITE_STDIO_H
#include XITE_STRING_H
#include XITE_UNISTD_H
#include XITE_TYPES_H
#include XITE_STAT_H
#include XITE_PARAM_H
#include <xite/debug.h>

#ifdef HAVE_DIR
  static DIR *dir_s;
  static struct dirent *dirEnt_s;
# define dname(a)    (a->d_name)
# define openDir(a)  opendir(a)
# define closeDir(a) closedir(a)

    static struct dirent *readDir(DIR *dir)
    {
      return(readdir(dir));
    }

#else
#ifdef _WIN32

  static long dir_s;
  static struct _finddata_t dirent_s;
  static struct _finddata_t *dirEnt_s = &dirent_s;
# define dname(a)       (a->name)
# define closeDir(a)    _findclose(a)

    static long openDir(char *dir_name)
    {
      static char filespec[MAXPATHLEN];

      (void) strcat(strcpy(filespec, dir_name), "\\*");
      return(_findfirst(filespec, dirEnt_s));
    }

    static struct _finddata_t *readDir(long dir_handle)
    {
      int status;

      status = _findnext(dir_handle, dirEnt_s);

      if (!status) return(dirEnt_s);
      else return(NULL);
    }
#endif /* _WIN32 */
#endif /* HAVE_DIR */


static struct stat buf_s;
#define readstat(a, b) stat(a, b)
#define testdir(buf)   ((buf.st_mode & S_IFMT) == S_IFDIR) 

static char pathname_s[1025];
static int item_printed_s;

static void readsw(int *argc, char **argv, int *format, int *b, int *name, int *title, char **n, int *u, int *hist, int *img, int *col)
{
  *b     = read_bswitch(argc, argv, "-b");
  *name  = read_bswitch(argc, argv, "-f");
  *title = read_bswitch(argc, argv, "-t");
  *n     = read_switch (argc, argv, "-n", 1, NULL);
  *u     = read_bswitch(argc, argv, "-u");
  *hist  = read_bswitch(argc, argv, "-h");
  *img   = read_bswitch(argc, argv, "-i");
  *col   = read_bswitch(argc, argv, "-c");

  if (*b || *name || *title || *n || *u || *hist) *format = FALSE;

} /* readsw() */

static int readnm(char *name, char **filename, char **dirname, int *pipe, IMAGE *i, int *isdir)
{
  int rstat;

  *pipe  = 0;
  *isdir = 0;

  if (name[0] == '-') {
    *pipe = 1;
    *i = Iread_image(name);
    if (*i EQ 0) exit(1);
    rstat  = Iok;
    *isdir = 0;
  } else {
    if (readstat(Ifilename_part(name), &buf_s) != 0) {
      /* Failed in getting status information for file. */
      *filename = name;
      return(-1);
    }

    *isdir = testdir(buf_s);
    if (*isdir == 0) {
      rstat     = Iopen_image(i, name, Ireadonly);
      *filename = name;
    } else {
      *dirname = name;
      dir_s    = openDir(*dirname);
      dirEnt_s = readDir(dir_s);
      rstat    = -1;
    }
  }

  return(rstat);
}

static BiffStatus readdr(char **filename, IMAGE *i)
{
  *filename = dname(dirEnt_s);

  while (strcmp(*filename, ".") == 0 || strcmp(*filename, "..") == 0) {
    dirEnt_s  = readDir(dir_s);
    if (dirEnt_s == NULL) return(-1);
    *filename = dname(dirEnt_s);
  }
  return(Iopen_image(i, *filename, Ireadonly));
}

static void print_name(int isdir, char *dirname, char *filename)
{
  if (item_printed_s++) printf(" ");
  if (isdir) printf("%s/%s", dirname, filename);
  else printf("%s", filename);
}

static void print_title(IMAGE i)
{
  if (item_printed_s++) printf(" ");
  printf("%s", Ititle(i));
} 

static void print_nbands(IMAGE i)
{
  if (item_printed_s++) printf(" ");
  printf("%ld", Inbands(i));
} 


static void print_bandinfo(IMAGE i, IBAND band, int bandno)
{
  if (item_printed_s++) printf(" ");
  printf("%4d %4ld %6ld %6ld %6ld %6ld %6ld %6ld",
	 bandno, Ipixtyp(band), Ixsize(band), Iysize(band),
	 Ixstart(band),Iystart(band),Ixmag(band),Iymag(band));
} 

static void print_history(IMAGE i, int pipe)
{
  if (! pipe) Iread_text(i);
  Itype_text(i, stdout);
}

static void print_format(IMAGE i, int *bands, int nbands, int nbands_img, int pipe, int argc, int arg_num, int isdir, char *dirname, char *filename)
{
  int j, bn;

  printf("\nFilename  : ");
  if (!pipe) {
    print_name(isdir, dirname, filename);
    printf("\n");
  } else printf("stdin\n");

  printf("Title     : %s\n",  Ititle(i));
  printf("Bands     : %d\n",  nbands_img);
  printf("Textlen   : %ld\n", Inchars(i));
  if (nbands) {
    printf("Band        Pixeltype   Xsize  Ysize");
    printf(" Xstart Ystart   Xmag   Ymag\n");
  }

  for (j=1; j <= nbands;  j++) {
    bn = bands[j];
    printf("  %2d        %-10s %6ld %6ld %6ld %6ld %6ld %6ld\n",
	   j, Ipixname(Ipixtyp(i[bn])),
	   Ixsize(i[bn]),Iysize(i[bn]),
	   Ixstart(i[bn]),Iystart(i[bn]),Ixmag(i[bn]),Iymag(i[bn]));
  }
  if (! pipe) Iread_text(i);
  if (! Iend_of_text(i)) { 
    printf("Textfield :\n");
    Itype_text(i, stdout);
  }
  if (argc - arg_num > 0) printf("\n\n");
} 

int main(int argc, char **argv)
{
  IMAGE img;
  int rstat, j, isdir, isimg, iscol;
  int all_bands = FALSE, name = FALSE, img_only = FALSE;
  int col_only = FALSE, title = FALSE, num_bands = FALSE, format = TRUE;
  int hist = FALSE, pipe = FALSE, argnum;
  int *bands_img = NULL, *bands_opt = NULL, nbands_img = 0, nbands_opt = 0;
  int *bands = NULL, nbands = 0, bn;
  char *dirname = NULL, *filename = NULL, *band_spec = NULL;

  ENTER_FUNCTION_DEBUG("biffinfo.c: main()");

  Iset_message(TRUE);
  InitMessage(&argc, argv, xite_app_std_usage_text(
    "Usage: %s [<option>...] [<filename>...] [<dirname>]\n\
       where <option> is chosen from\n\
         -f             : Print the filename\n\
         -b             : Print number of bands\n\
         -n <band-spec> : Print info for bands given by standard XITE band\n\
                          specification\n\
                          <band number> <pixtyp> <xsize> <ysize>\n\
                                 <xstart> <ystart> <xmag> <ymag>\n\
         -u             : Print info for all bands\n\
         -t             : Print the image title\n\
         -h             : Print text (history)  block\n\
         -i             : Print images only\n\
         -c             : Print colortables only\n"));

  if (argc == 1) Usage(1, NULL);

  readsw(&argc, argv, &format, &num_bands, &name, &title, &band_spec, 
	 &all_bands, &hist, &img_only, &col_only);
  if (band_spec)
    rstat = Iparse_band_spec(band_spec, &bands_opt, &nbands_opt);
  else rstat = 0;

  if (rstat) {
    Error(1, "Bad band specification.\n");
    return(2);
  }

  /* Treat remaining command-line arguments as image filenames. */

  argnum = 1;
  while (argc - argnum > 0) {
    rstat = readnm(tilde_expand(argv[argnum++]), &filename, &dirname,
		   &pipe, &img, &isdir);

    if (isdir) {

      errno = 0;
      if (!getcwd(pathname_s, MAXPATHLEN)) Error(2, "%s\n", strerror(errno));

      errno = 0;
      if (chdir(dirname) == -1) Error(2, "%s\n", strerror(errno));
    }

    do { /* For 'filename', stdin or each of the files in 'dirname'. */
      if (isdir) {
	rstat = readdr(&filename, &img);
	if (rstat == -1) break;
      }

      if (rstat != Iok) {
	if (format && (isdir == 0)) 
	  printf("biffinfo: %s is not a BIFF-file\n\n", filename);
	continue;
      }

      nbands_img = 0;
      if (bands_img) {
	free(bands_img); bands_img = NULL;
      }

      if (!pipe) {
	if (Iparse_filename(&filename, &bands_img, &nbands_img) != Iok) {
	  return(Error(2, "Could not parse band specification.\n"));
	}
      }

      nbands = 0;
      if (bands) {
	free(bands); bands = NULL;
      }

      if (all_bands) {
	if (nbands_img == 0) nbands_img = Inbands(img);
	nbands_opt = 0;
	nbands     = nbands_img;
      } else if (nbands_img == 0 && nbands_opt == 0) {
	nbands_img = Inbands(img);
	nbands     = nbands_img;
      } else if (nbands_img == 0) nbands = nbands_opt;
      else if (nbands_opt == 0) nbands = nbands_img;
      else nbands = nbands_opt;
      
      bands = (int *) malloc((nbands + 1) * sizeof(int));
      
      for (bn = 1; bn <= nbands; bn++) {
	if (all_bands) {
	  if (nbands_img == 0 || bands_img == NULL) bands[bn] = bn;
	  else {
	    if (bands_img[bn] < 1 || bands_img[bn] > Inbands(img)) {
	      return(Error(2, "Non-existent band.\n"));
	    }
	    bands[bn] = bands_img[bn];
	  }
	}
	else if (nbands_img == 0) {
	  if (bands_opt) {
	    if (bands_opt[bn] < 1 || bands_opt[bn] > Inbands(img)) {
	      return(Error(2, "Non-existent band.\n"));
	    }
	    bands[bn] = bands_opt[bn];
	  } else bands[bn] = bn;
	}
	else if (nbands_opt == 0) {
	  if (bands_img) {
	    if (bands_img[bn] < 1 || bands_img[bn] > Inbands(img)) {
	      return(Error(2, "Non-existent band.\n"));
	    }
	    bands[bn] = bands_img[bn];
	  }
	  else bands[bn] = bn;
	}
	else {
	  if (bands_opt[bn] < 1 || bands_opt[bn] > nbands_img) {
	    return(Error(2, "Non-existent band.\n"));
	  }
	  bands[bn] = bands_img[bands_opt[bn]];
	}
      }
      
      if (all_bands) 
	for(j = 1; j <= nbands; j++) {
	  isimg = Ipixtyp(img[bands[j]]) < 256;
	  iscol = !isimg;

	  if ((img_only && isimg) ||
	      (col_only && iscol) ||
	      (img_only == col_only)) {
	    item_printed_s = 0;
	    if (name && !pipe)  print_name(isdir, dirname, filename);
	    if (num_bands)      print_nbands(img);
	    if (nbands_img)
	      print_bandinfo(img, img[bands[j]], j);
	    else
	      print_bandinfo(img, img[bands[j]], bands_img[j]);
	    if (title)          print_title(img);
	    if (item_printed_s) printf("\n");
	  }
      } else {
	isimg = 1;
	iscol = 1;
	for (j = 1; j <= nbands; j++) {
	  isimg = isimg && Ipixtyp(img[bands[j]]) < 256;
	  iscol = iscol && Ipixtyp(img[bands[j]]) >= 256;
	}

	if ((img_only && isimg) ||
	    (col_only && iscol) ||
	    (img_only == col_only)) {
	  item_printed_s = 0;
	  if (name && !pipe)  print_name(isdir, dirname, filename);
	  if (num_bands)      print_nbands(img);
	  if (nbands_opt) {
	    for (j = 1; j <= nbands; j++) {
	      print_bandinfo(img, img[bands[j]], bands_opt[j]);

	      if (item_printed_s) {
		printf("\n");
		item_printed_s = 0;
	      }
	    }
	  }
	  if (title)          print_title(img);
	  if (item_printed_s) printf("\n");
	}
      } /* Not all_bands. */

      if (hist) print_history(img, pipe);

      if (format) {
	isimg = 1;
	iscol = 1;
	for (j=1; j<= nbands; j++) {
	  isimg = isimg && (Ipixtyp(img[bands[j]]) < 256);
	  iscol = iscol && (Ipixtyp(img[bands[j]]) >= 256);
	}
	if ((img_only && isimg) ||
	    (col_only && iscol) ||
	    (img_only == col_only))
	  print_format(img, bands, nbands, nbands_img, pipe, argc, argnum,
		       isdir, dirname, filename);
      }
    } /* do */
    while(isdir && (dirEnt_s = readDir(dir_s)));

    if (isdir) chdir(pathname_s);
    if ((! pipe) && (rstat == Iok)) Iclose_image(img);
  } /* More command-line arguments. */

  if (dir_s) closeDir(dir_s);

  LEAVE_FUNCTION_DEBUG("biffinfo.c: main()")

  return(0);
}
