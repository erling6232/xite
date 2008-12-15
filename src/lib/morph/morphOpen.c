
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



int morphOpen(IBAND inband, IBAND outband, ISS_BAND B)
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
