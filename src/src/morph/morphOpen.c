
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
#include <xite/blab.h>
#include <xite/convert.h>
#include <xite/message.h>
#include <xite/morph.h>
#include <xite/readarg.h>



#ifndef MAIN

#ifndef FUNCPROTO
int morphOpen(inband, outband, B)
IBAND inband, outband;
ISS_BAND B;
#else /* FUNCPROTO */
int morphOpen(IBAND inband, IBAND outband, ISS_BAND B)
#endif /* FUNCPROTO */
{
  int status;
  IBAND tmpband;

  tmpband = Imake_band(Iu_byte_typ, Ixsize(inband), Iysize(inband));
  status = morphErode(inband, tmpband, B);
  if (status) return(Error(status, "morphOpen: Error in morphErode\n"));
  status = morphDilate(tmpband, outband, B);
  if (status) return(Error(status, "morphOpen: Error in morphDilate\n"));
  Idel_band(&tmpband);

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

  if (argc != 3) Usage(2, "Illegal number of arguments.\n");

  inimage  = Iread_image(argv[1]);

  if (a) ascii2biff((IBAND *) &B_band, a, Is_short_typ);
  else if (b) {
    B = (ISS_IMAGE)Iread_image(b);
    B_band = B[1];
  } else Usage(3, "No structuring element.\n");

  outimage = Icopy_init(inimage);

  for (bn=1; bn LE Inbands(inimage); bn++)
    morphOpen(inimage[bn], outimage[bn], B_band);

  Ihistory(outimage, argv[0], args);
  Iwrite_image(outimage, argv[2]);

  return(0);
}

#endif /* MAIN */
