
/*

________________________________________________________________

        
        $Id$
        Copyright 1992, Blab, UiO
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
#include <xite/morph.h>
#include <xite/blab.h>
#include <xite/convert.h>
#include <xite/message.h>
#include <xite/readarg.h>



#ifndef MAIN

#define TESTVALUE(val) ((val<0)?(0):((val>255)?(255):(val)))
#ifndef MIN
# define MIN(a,b) (((a)<(b)) ? (a) : (b))
#endif

#ifndef FUNCPROTO
int morphErode(inband, outband, B)
IBAND inband, outband;
ISS_BAND B;
#else /* FUNCPROTO */
int morphErode(IBAND inband, IBAND outband, ISS_BAND B)
#endif /* FUNCPROTO */
{
  int x, y, i, j, min, min2, Ixstart_B, Ixsize_B, Iystart_B, Iysize_B,
      Ixstart_inband, Ixsize_inband, Iystart_inband, Iysize_inband;

  if (Ipixtyp(inband) NE Iu_byte_typ)
    return(Error(1, "morphErode: Input pixel type must be unsigned byte.\n"));
  if (Ipixtyp(outband) NE Iu_byte_typ)
    return(Error(2, "morphErode: Output pixel type must be unsigned byte.\n"));
  if (Ipixtyp((IBAND) B) NE Is_short_typ)
    return(Error(3, "%s%s\n",
		 "morphErode: Structuring element pixel type must be ",
		 "signed short."));

  Ixstart_B      = (int)(Ixsize((IBAND) B)/2.0+0.5);
  Ixsize_B       = Ixsize((IBAND) B);
  Iystart_B      = (int)(Iysize((IBAND) B)/2.0+0.5);
  Iysize_B       = Iysize((IBAND) B);
  Ixstart_inband = (int)(Ixsize(inband)/2.0+0.5);
  Ixsize_inband  = MIN(Ixsize(inband), Ixsize(outband));
  Iystart_inband = (int)(Iysize(inband)/2.0+0.5);
  Iysize_inband  = MIN(Iysize(inband), Iysize(outband));

  /* Upper border */
  min = INT_MAX;
  FOR (y=1; y LE Iysize_B-Iystart_B; ++ y)
    FOR (x=1; x LE Ixsize_B-Ixstart_B; ++ x)
      for (j=1; j LE Iystart_B+y-1; ++ j)
        FOR (i=1; i LE Ixstart_B+x-1; ++ i)
          min2=inband[y+(Iystart_B-j)][x+(Ixstart_B-i)]-B[j][i];
          if (min2 < min) min=min2;
        ENDFOR
      outband[y][x] = TESTVALUE(min);
      min=INT_MAX; 
    ENDFOR

    FOR (x=Ixsize_B-Ixstart_B+1; x LE (Ixsize_inband-Ixstart_B+1); ++ x)
      for (j=1; j LE Iystart_B+y-1; ++ j)
        FOR (i=1; i LE Ixsize_B; ++ i)
          min2=inband[y+(Iystart_B-j)][x+(Ixstart_B-i)]-B[j][i];
          if (min2 < min) min=min2;
        ENDFOR
      outband[y][x] = TESTVALUE(min);
      min=INT_MAX;
    ENDFOR

    FOR (x=(Ixsize_inband-Ixstart_B+2); x LE Ixsize_inband; ++ x)
      for (j=1; j LE Iystart_B+y-1; ++ j)
        FOR (i=Ixstart_B-(Ixsize_inband-x); i LE (Ixsize_B); ++ i)
          min2=inband[y+(Iystart_B-j)][x+(Ixstart_B-i)]-B[j][i];
          if (min2 < min) min=min2;
        ENDFOR
      outband[y][x] = TESTVALUE(min);
      min=INT_MAX;
    ENDFOR
  ENDFOR

  /* Middle section */
  FOR (y=Iysize_B-Iystart_B+1;y LE (Iysize_inband-Iystart_B+1); ++ y)
    FOR (x=1;x LE Ixsize_B-Ixstart_B; ++ x)
      for (j=1;j LE Iysize_B; ++ j)
        FOR (i=1; i LE Ixstart_B+x-1; ++ i)
          min2=inband[y+(Iystart_B-j)][x+(Ixstart_B-i)]-B[j][i];
          if (min2 < min) min=min2;
        ENDFOR
      outband[y][x] = TESTVALUE(min);
      min=INT_MAX;
    ENDFOR

    FOR (x=Ixsize_B-Ixstart_B+1; x LE (Ixsize_inband-Ixstart_B+1); ++ x)
      for (j=1; j LE Iysize_B; ++ j)
        FOR (i=1; i LE Ixsize_B; ++ i)
          min2=inband[y+(Iystart_B-j)][x+(Ixstart_B-i)]-B[j][i];
          if (min2 < min) min=min2;
        ENDFOR
      outband[y][x] = TESTVALUE(min);
      min=INT_MAX;
    ENDFOR

    FOR (x=Ixsize_inband-Ixstart_B+2; x LE Ixsize_inband; ++ x)
      for (j=1; j LE Iysize_B; ++ j)
        FOR (i=Ixstart_B-(Ixsize_inband-x); i LE Ixsize_B; ++ i)
          min2=inband[y+(Iystart_B-j)][x+(Ixstart_B-i)]-B[j][i];
          if (min2 < min) min=min2;
       ENDFOR
       outband[y][x] = TESTVALUE(min);
       min=INT_MAX;
    ENDFOR
  ENDFOR

  /* lower border */
  FOR (y=(Iysize_inband-Iystart_B+2); y LE Iysize_inband; ++ y)
    FOR (x=1; x LE Ixsize_B-Ixstart_B; ++ x)
      for (j=Iystart_B-(Iysize_inband-y); j LE Iysize_B; ++ j)
        FOR (i=1; i LE Ixstart_B+x-1; ++ i)
          min2=inband[y+(Iystart_B-j)][x+(Ixstart_B-i)]-B[j][i];
          if (min2 < min) min=min2;
        ENDFOR
      outband[y][x] = TESTVALUE(min);
      min=INT_MAX;
    ENDFOR

    FOR (x=Ixsize_B-Ixstart_B+1; x LE (Ixsize_inband-Ixstart_B+1); ++ x)
      for (j=Iystart_B-(Iysize_inband-y); j LE Iysize_B; ++ j)
        FOR (i=1; i LE Ixsize_B; ++ i)
          min2=inband[y+(Iystart_B-j)][x+(Ixstart_B-i)]-B[j][i];
          if (min2 < min) min=min2;
        ENDFOR
      outband[y][x] = TESTVALUE(min);
      min=INT_MAX;
    ENDFOR

    FOR (x=(Ixsize_inband-Ixstart_B+2); x LE Ixsize_inband; ++ x)
      for (j=Iystart_B-(Iysize_inband-y); j LE Iysize_B; ++ j)
        FOR (i=Ixstart_B-(Ixsize_inband-x); i LE Ixsize_B; ++ i)
          min2=inband[y+(Iystart_B-j)][x+(Ixstart_B-i)]-B[j][i];
          if (min2 < min) min=min2;
        ENDFOR
      outband[y][x] = TESTVALUE(min);
      min=INT_MAX;
    ENDFOR
  ENDFOR

  return(0);
}

#endif /* not MAIN */



#ifdef MAIN

#ifndef FUNCPROTO
int main(argc, argv)
int argc;
char *argv[];
#else /* FUNCPROTO */
int main(int argc, char **argv)
#endif /* FUNCPROTO */
{
  IMAGE inimage, outimage;
  ISS_IMAGE B;
  ISS_BAND B_band;
  int bn;
  char *args;
  char *a,*b;

  InitMessage(&argc,argv, xite_app_std_usage_text(
    "Usage: %s <inimage> <outimage> -a <ascii struct elem>\n\
       or: %s <inimage> <outimage> -b <biff struct elem>\n"));

  Iset_message(TRUE);

  if (argc == 1) Usage(1, NULL);
  args = argvOptions(argc, argv);

  a = read_switch(&argc, argv, "-a", 1, NULL);
  b = read_switch(&argc, argv, "-b", 1, NULL);

  if (argc != 3) Usage(2,"Illegal number of arguments.\n");

  inimage  = Iread_image(argv[1]);

  if (a) ascii2biff((IBAND *) &B_band, a, Is_short_typ);
  else if (b) {
    B = (ISS_IMAGE)Iread_image(b);
    B_band = B[1];
  } else Usage(3, "No structuring element.\n");

  outimage = Icopy_init(inimage);

  for (bn=1; bn <= Inbands(inimage); bn++)
    morphErode(inimage[bn], outimage[bn], B_band);

  Ihistory(outimage, argv[0], args);
  Iwrite_image(outimage,argv[2]);

  return(0);
}

#endif /* MAIN */
