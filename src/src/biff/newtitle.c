
/*

________________________________________________________________

        newtitle.c
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
#include <xite/blab.h>
#include <xite/biff.h>
#include <stdlib.h>
#include <xite/message.h>
#include XITE_MALLOC_H
#include XITE_STDIO_H
#include XITE_STRING_H

/*P:newtitle*

________________________________________________________________

		newtitle
________________________________________________________________

Name:		newtitle - insert new title into an image

Syntax:		newtitle <image> [<new_title>]

Description:	'newtitle' changes the title of an image to 
		'new_ title'. 'new_title' may consist of several
		words, with a total maximum length of 32 characters.

		In the absence of 'new_title', the image title will
		be cleared.

Author:		Tor Lønnestad, BLAB, ifi UiO.

Examples:	newtitle mona.img Mona Lisa

Id: 		$Id$
________________________________________________________________

*/

#ifdef MAIN

#ifndef FUNCPROTO
int main(argc,argv)
int argc;
char *argv[];
#else /* FUNCPROTO */
int main(int argc, char **argv)
#endif /* FUNCPROTO */
{
   IMAGE i;
   int l, n;
   char* title;

   Iset_message(TRUE);
   Iset_abort(TRUE);
   InitMessage(&argc, argv, xite_app_std_usage_text(
    "Usage: %s <image> <new title>\n"));

   if (argc == 1) Usage(1, NULL);
   if (argc <  2) Usage(1, "Illegal number of arguments.\n");

   l = 1;
   for (n=2; n LT argc; INC n) l += strlen(argv[n])+1;
   title = (char*)malloc(l);
   title[0] = '\0';

   for (n=2; n < argc; ++ n) {
     strcat(title, argv[n]);
     if (n <argc-1) strcat(title, " ");
   }
   Iopen_image(&i, argv[1], Ireadwrite);
   Iset_title(i, title);
   Iclose_image(i);
   return(0);
}	/* main */

#endif /* MAIN */
