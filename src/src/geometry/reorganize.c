

/*

________________________________________________________________

        reorganize.c
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
#include <xite/geometry.h>
#include XITE_STDIO_H
#include <xite/message.h>
#include <xite/readarg.h>
#include XITE_STRING_H

#ifdef MAIN

static char *txt_s[8] = { "Identity-transform",
			"Vertical mirror",
			"Horizontal mirror",
			"Transpose",
			"Rotate 90 deg",
			"Rotate 180 deg",
			"Rotate 270 deg",
			"Bi-transpose" };

#ifndef FUNCPROTO
static BiffStatus vert(input, output)
IBAND input, output;
#else /* FUNCPROTO */
static BiffStatus vert(IBAND input, IBAND output)
#endif /* FUNCPROTO */
{
  int xsize = Ixsize(output), ysize = Iysize(output);
  int x,y;

  if (Ipixtyp(input) != Iu_byte_typ || Ipixtyp(output) != Iu_byte_typ)
    return(Error(2, "Input and output pixel type must be unsigned byte.\n"));

  for (y=1; y <= ysize; y++)
    for (x=1; x <= xsize; x++)
      output[y][x] = input[y][xsize-x+1];

  return(Iok);
}


#ifndef FUNCPROTO
static BiffStatus hori(input, output)
IBAND input, output;
#else /* FUNCPROTO */
static BiffStatus hori(IBAND input, IBAND output)
#endif /* FUNCPROTO */
{
  int xsize = Ixsize(output), ysize = Iysize(output);
  int x, y;

  if (Ipixtyp(input) != Iu_byte_typ || Ipixtyp(output) != Iu_byte_typ)
    return(Error(2, "Input and output pixel type must be unsigned byte.\n"));

  for (y=1; y <= ysize; y++)
    for (x=1; x <= xsize; x++)
      output[y][x] = input[ysize-y+1][x];

  return(Iok);
}


#ifndef FUNCPROTO
static BiffStatus tran(input, output)
IBAND input, output;
#else /* FUNCPROTO */
static BiffStatus tran(IBAND input, IBAND output)
#endif /* FUNCPROTO */
{
  int xsize = Ixsize(output), ysize = Iysize(output);
  int x, y;

  if (Ipixtyp(input) != Iu_byte_typ || Ipixtyp(output) != Iu_byte_typ)
    return(Error(2, "Input and output pixel type must be unsigned byte.\n"));

  for (y=1; y <= ysize; y++)
    for (x=1; x <= xsize; x++)
      output[y][x] = input[x][y];

  return(Iok);
}


#ifndef FUNCPROTO
static BiffStatus r270(input, output)
IBAND input, output;
#else /* FUNCPROTO */
static BiffStatus r270(IBAND input, IBAND output)
#endif /* FUNCPROTO */
{
  int xsize = Ixsize(output), ysize = Iysize(output);
  int x, y;

  if (Ipixtyp(input) != Iu_byte_typ || Ipixtyp(output) != Iu_byte_typ)
    return(Error(2, "Input and output pixel type must be unsigned byte.\n"));

  for (y=1; y <= ysize; y++)
    for (x=1; x <= xsize; x++)
      output[y][x] = input[xsize-x+1][y];

  return(Iok);
}


#ifndef FUNCPROTO
static BiffStatus r180(input, output)
IBAND input, output;
#else /* FUNCPROTO */
static BiffStatus r180(IBAND input, IBAND output)
#endif /* FUNCPROTO */
{
  int xsize = Ixsize(output), ysize = Iysize(output);
  int x, y;

  if (Ipixtyp(input) != Iu_byte_typ || Ipixtyp(output) != Iu_byte_typ)
    return(Error(2, "Input and output pixel type must be unsigned byte.\n"));

  for (y=1; y <= ysize; y++)
    for (x=1; x <= xsize; x++)
      output[y][x] = input[ysize-y+1][xsize-x+1];

  return(Iok);
}


#ifndef FUNCPROTO
static BiffStatus r090(input, output)
IBAND input, output;
#else /* FUNCPROTO */
static BiffStatus r090(IBAND input, IBAND output)
#endif /* FUNCPROTO */
{
  int xsize = Ixsize(output), ysize = Iysize(output);
  int x, y;

  if (Ipixtyp(input) != Iu_byte_typ || Ipixtyp(output) != Iu_byte_typ)
    return(Error(2, "Input and output pixel type must be unsigned byte.\n"));

  for (y=1; y <= ysize; y++)
    for (x=1; x <= xsize; x++)
      output[y][x] = input[x][ysize-y+1];

  return(Iok);
}


#ifndef FUNCPROTO
static BiffStatus bdia(input, output)
IBAND input, output;
#else /* FUNCPROTO */
static BiffStatus bdia(IBAND input, IBAND output)
#endif /* FUNCPROTO */
{
  int xsize = Ixsize(output), ysize = Iysize(output);
  int x, y;

  if (Ipixtyp(input) != Iu_byte_typ || Ipixtyp(output) != Iu_byte_typ)
    return(Error(2, "Input and output pixel type must be unsigned byte.\n"));

  for (y=1; y <= ysize; y++)
    for (x=1; x <= xsize; x++)
      output[y][x] = input[xsize-x+1][ysize-y+1];

  return(Iok);
}

/*P:reorganize*

________________________________________________________________

		reorganize
________________________________________________________________

Name:		reorganize - rotate, mirror or transpose an image

Syntax:		reorganize [-nvhtlurb] <inimage> <outimage>

  		|   Normal   V-Mirror  H-Mirror Transpose 
  		|     n         v         h         t      
  		|    a b       b a       c d       a c     
  		|    c d       d c       a b       b d     
		|
  		|   Rot-90   Rot-180   Rot-270  B-Transpose   
  		|     l         u         r         b      
  		|    b d       d c       c a       d b     
  		|    a c       b a       d b       c a     

Description:    rotate, mirror or transpose an image.

Restrictions:   'inimage' must have bands with pixel type unsigned byte.

See also:       rotate(1), quadratic(1), affine(1), shift_img(1),
                transpose_img(1), warp(1)

Author:		Otto Milvang

Id: 		$Id$
________________________________________________________________

*/

#ifndef FUNCPROTO
int main(argc,argv)
int argc;
char *argv[];
#else /* FUNCPROTO */
int main(int argc, char **argv)
#endif /* FUNCPROTO */
{
  IMAGE img1,img2;
  int func,i, pt, xsize, ysize;
  char *ptr, *args;
  char *switches = "nvhtlurb";

  Iset_message(TRUE);
  Iset_abort(FALSE);
  InitMessage(&argc, argv, xite_app_std_usage_text(
    "Usage: %s [-nvhtlurb] <inimage> <outimage>\n\
      \n\
       Normal  V-Mirror  H-Mirror  Transpose    \n\
          n         v         h         t        \n\
         a b       b a       c d       a c       \n\
         c d       d c       a b       b d       \n\
      \n\
        Rot-90   Rot-180   Rot-270 B-Transpose   \n\
          l         u         r         b        \n\
         b d       d c       c a       d b       \n\
         a c       b a       d b       c a       \n"));

  if (argc == 1) Usage(1, NULL);
  args = argvOptions(argc, argv);
  if (argc != 4) Usage(1, "Illegal number of arguments.\n");

  if (strlen(argv[1]) != 2 && *argv[1] != '-')
    Usage(1, "First argument must be single option.\n");
  ptr = strchr(switches, argv[1][1]);
  if ( ptr == NULL)
    Usage(1, "Found no legal option as first command-line argument.\n");

  func = ptr - switches;

  if ((img1 = Iread_image(argv[2])) == NULL)
    return(Error(1, "Cannot read input image.\n"));

  img2 = Init_image(Inbands(img1), Ititle(img1));
  
  Info(1, "Reorganizing.\n");
  for (i = 1; i <= Inbands(img1); i++)
    {
      pt    = Ipixtyp(img1[i]);
      xsize = Ixsize(img1[i]);
      ysize = Iysize(img1[i]);
      if (func == 0 || func == 1 || func == 2 || func == 5)
	img2[i] = Imake_band(pt, xsize, ysize);
      else
	img2[i] = Imake_band(pt, ysize, xsize);
      switch(func)
	{
	case 0: img2[i] = img1[i]; break;
	case 1: vert(img1[i], img2[i]); break;
	case 2: hori(img1[i], img2[i]); break;
	case 3: tran(img1[i], img2[i]); break;
	case 4: r090(img1[i], img2[i]); break;
	case 5: r180(img1[i], img2[i]); break; 
	case 6: r270(img1[i], img2[i]); break; 
	case 7: bdia(img1[i], img2[i]); break; 
	default: Usage(1, "Illegal option.\n"); break; 
	}
    }

  Icopy_text(img1,img2);
  Ihistory(img2, argv[0], args);
  Iwrite_image(img2,argv[3]);

  return(0);
}

#endif /* MAIN */
