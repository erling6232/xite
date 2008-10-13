

/*

________________________________________________________________

        write_byteband.c
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

/* #include <xite/biff.h> */
/* #include XITE_STDIO_H 
   #include <math.h>
   #include <xite/blab.h> */

static int write_byteband(IBAND inband, char *filename)
{
  int x, y, xsize, ysize;
  IMAGE tempimage;
  IBAND tempband;

  if (Ipixtyp(inband) != Iu_byte_typ)
    return(Error(1,
		 "write_byteband: Input pixel type must be unsigned byte.\n"));

  xsize = Ixsize(inband);
  ysize = Iysize(inband);

  tempimage = Imake_image(1, filename, Iu_byte_typ, xsize, ysize);
  tempband  = tempimage[1];

  for (x=1; x<=xsize; x++)
    for (y=1; y<=ysize; y++)
      tempband[y][x] = inband[y][x];

  Iwrite_image(tempimage, filename);
  Info(0, "Intermediate image written to file : %s\n", filename);
  Idel_image(&tempimage);

  return 0;
}
