/*

________________________________________________________________

        subcopy
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
#include <xite/biff.h>
#include <stdlib.h>
#include <xite/copy.h>
#ifdef HAVE_STDIO_H
#  include <stdio.h>
#endif
#include <xite/blab.h>
#include <xite/message.h>
#include <xite/readarg.h>
#ifdef HAVE_STRINGS_H
# include <strings.h>
#else
# ifdef HAVE_STRING_H
#  include <string.h>
# endif
#endif

#ifndef MAX
# define  MAX(a,b) (((a) > (b)) ? (a) : (b))
#endif
#ifndef MIN
# define  MIN(a,b) (((a) < (b)) ? (a) : (b))
#endif

/*
_________________________________________________

        parseBands
_________________________________________________

return 0 when OK with parsing result in bands and nbands.
return 1 when bandspec is NULL
return 2 when bad bandspec
_________________________________________________
*/

static int parseBands(char *bandspec, int *bands, int *nbands)
{
  int bnr=0, bn, bn1, bn2, stat=0;
  if (NOT bandspec) { *nbands = 0; return(1);}

  bandspec = strtok(bandspec,":");
  while (bandspec) {
    if(strchr(bandspec,'-')) {
      if (sscanf(bandspec, "%d-%d%*s", &bn1, &bn2) == 2) {
	if (bn1 <= bn2) 
          for (bn=bn1; bn <= bn2; bn++) bands[++bnr] = bn;
        else for (bn=bn1; bn >= bn2; bn--) bands[++bnr] = bn;
      } else stat=2;
    } else {
      if (sscanf(bandspec, "%d%*s",&bn1) == 1) bands[++bnr] = bn1;
      else stat=2;
    }
    bandspec = strtok(NULL,":");
  }
  *nbands = bnr;
  return(stat);
}


/*P:subcopy*

________________________________________________________________

		subcopy
________________________________________________________________

Name:		subcopy - copy a part of an image into a new image

Syntax:		| subcopy [<option>...] <input> <output>

Description:    Copy a part of an image 'input' into a new image 'output'.
                The part to be copied is specified by the options.
		All size options except '-band' should be
		positive integers. Start and stop coordinates are global
		coordinates, unless the '-l' (local) option is used.
		The band specification is a list of bands or band-intervals
		separated by ":". A band-interval is specified by
		'start-stop'. Default option values give the whole image.
		All pixel types are accepted. 

Options:	&-xstart xstart
		&-xsize  xsize
		&-xstop  xstop
		&-ystart ystart
		&-ysize  ysize
		&-ystop  ystop
		&-band   bandspec
		&-title  title

		&-l
                Start and stop coordinates are local. Default: Global.

See also:	biffcopy(1), biffmerge(1)

Author:		Tor Lønnestad, BLAB, ifi, UiO

Examples:       | subcopy -xstart 200 -xsize 60 -ystart 200 -ysize 30 \\
                |   mona.img smile.img
                | subcopy -xstart 100 -band 1:3:5-7 tm.img part.img

Id:             $Id$
________________________________________________________________

*/

int main(int argc, char **argv)
{
  IMAGE img1,img2;
  int xstart, xsize, xstop, ystart, ysize, ystop, bn, nbands1, nbands2,
      bands[1000];
  int xsta, xsi, ysta, ysi, local;
  char *bandspec, *title, arg[200];
  IBAND band;

  Iset_message(TRUE);
  Iset_abort(TRUE);
  InitMessage(&argc, argv, xite_app_std_usage_text(
"Usage: %s [<option>...] <input> <output>\n\
      where the options are:\n\
	-xstart <xstart>\n\
	-xsize  <xsize>\n\
	-xstop  <xstop>\n\
	-ystart <ystart>\n\
	-ysize  <ysize>\n\
	-ystop  <ystop>\n\
	-bands  <bandspec>\n\
        -title  <title>\n\
        -l      \n"));

  xstart   = read_iswitch(&argc, argv, "-xstart", 1);
  xstop    = read_iswitch(&argc, argv, "-xstop", INT_MAX);
  xsize    = read_iswitch(&argc, argv, "-xsize", xstop-xstart+1);
  ystart   = read_iswitch(&argc, argv, "-ystart", 1);
  ystop    = read_iswitch(&argc, argv, "-ystop", INT_MAX);
  ysize    = read_iswitch(&argc, argv, "-ysize", ystop-ystart+1);
  bandspec = read_switch(&argc, argv, "-b", 1, NULL);
  bandspec = read_switch(&argc, argv, "-band", 1, bandspec);
  bandspec = read_switch(&argc, argv, "-bands", 1, bandspec);
  title    = read_switch(&argc, argv, "-t", 1, (char*)0);
  title    = read_switch(&argc, argv, "-title", 1, title);
  local    = read_bswitch(&argc, argv, "-l");

  if (bandspec)
  {
    sprintf(arg, "bands:%s", bandspec);
    if (parseBands(bandspec, bands, &nbands2))
      exit(Error(2, "Error in bandspec\n"));
  }
  else arg[0] = (char)0;

  if (argc == 1) Usage(1, (char*)0);
  if (argc != 3) Usage(2, "Bad number of arguments\n");

  img1 = Iread_image(argv[1]);
  nbands1 = Inbands(img1);
  if (NOT bandspec) {
    nbands2 = nbands1;
    for (bn=1; bn<=nbands2; bn++) bands[bn]=bn;
  }  
  img2 = Init_image(nbands2, (title) ? title : Ititle(img1));


  for (bn=1; bn LE nbands2; bn++) {
    if (bands[bn] <1 || bands[bn] > nbands1) 
      Warning(3, "Bad band number: %d, no such band\n", bands[bn]);
    band = img1[bands[bn]];

    if (local) {
      xsta = MAX(xstart, 1);
      xsi  = MIN(xsize, Ixsize(band)-xsta+1);
      ysta = MAX(ystart, 1);
      ysi  = MIN(ysize, Iysize(band)-ysta+1);
    } else {
      xsta = MAX(xstart-Ixstart(band)+1, 1);
      xsi  = MIN(xsize, Ixsize(band)-xsta+1);
      ysta = MAX(ystart-Iystart(band)+1, 1);
      ysi  = MIN(ysize, Iysize(band)-ysta+1);
    }
    if (NOT (img2[bn] = Imake_subband(band, xsta, ysta, xsi, ysi)))
      Warning(4, "Couldn't copy from band %d\n", bands[bn]);
  }

  Icopy_text(img1, img2);
  Ihistory(img2, argv[0], arg);
  Iwrite_image(img2, argv[2]);
  return(0);
}
