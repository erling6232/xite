

/*

________________________________________________________________

        bifftext.c
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
#include <xite/message.h>
#include XITE_STRING_H
#include XITE_STDIO_H

/*P:bifftext*

________________________________________________________________

                bifftext
________________________________________________________________

Name:           bifftext - append a text line or edit text on image

Syntax:         | bifftext <image> [<new line>] ... [<new line>]

Description:    'bifftext' appends new lines to the image text field,
                or starts an editor.

		If the "EDITOR" environment variable is set, that editor will
		be started. Otherwise, "emacs" will be used on a UNIX system,
		and "Notepad" will be used on a Windows NT system.

Author:         Otto Milvang
Examples:       | bifftext mona.img
                | bifftext mona.img "New line 1" "New line 2"
Id:             $Id$
________________________________________________________________

*/

#ifdef MAIN

#ifndef FUNCPROTO
int main(argc, argv)
int argc;
char **argv;
#else /* FUNCPROTO */
int main(int argc, char **argv)
#endif /* FUNCPROTO */
{
   IMAGE img;
   int i;

   Iset_message(1);
   Iset_abort(1);
   InitMessage(&argc, argv, xite_app_std_usage_text(
               "Usage : %s <image> [<new line>] ... [<new line>]\n"));
   if (argc < 2) Usage(1, NULL);
   Iopen_image(&img, argv[1], Ireadwrite);
   Iread_text(img);
   if (argc == 2) Iedit_text(img);
   else {
     for (i=2; i<argc; i++) Iappend_line(img, argv[i]);
   }
   Iwrite_text(img);
   Iclose_image(img);

   return(0);
}

#endif /* MAIN */
