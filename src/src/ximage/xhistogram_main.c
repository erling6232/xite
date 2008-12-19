
/*

________________________________________________________________

        Xhistogram.c
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

static char *Id = "$Id$, Otto Milvang, Blab, UiO";



#include <xite/includes.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>
#include <xite/biff.h>
#include <X11/Xos.h>
#include <X11/IntrinsicP.h>
#include <xite/Xhistogram.h>
#include XITE_STDIO_H
#include <X11/Shell.h>
#include <X11/StringDefs.h>
#include <Xfwf/MultiList.h>
#include <X11/Xaw/Form.h>
#include <X11/Xaw/Label.h>
#include <X11/Xaw/Command.h>
#include <X11/Xutil.h>
#include <xite/color.h>
#include <xite/ImageOverlay.h>
#include <xite/ImageP.h> /* For pixtype */
#include <xite/Xslice.h>
#include <xite/histo.h>
#include "Histogram.h"
#include <xite/message.h>
#include <xite/statistics.h>
#include <xite/Visual.h>
#include <xite/Cmap.h>
#include <xite/ShellWids.h>
#include <xite/ximage.h>
#include <xite/ximage_display.h>
#include <xite/Xcolor.h>
#include <xite/Xdialog.h>
#include <xite/readarg.h>
#include "Colormap.h"
#include XITE_MALLOC_H
#include <xite/debug.h>

extern Widget   Control_e;

/*P:xhistogram*

________________________________________________________________

		xhistogram
________________________________________________________________

Name:		xhistogram - Show the histogram of an image in XITE.

Syntax:		xhistogram [-split] <inimage>

Description:    Display the histogram for the BIFF image read from 'inimage'.
                Some of the command buttons will be insensitive. If making
		a histogram from the Tools menu of an image widget in another
		XITE display application, e.g. xshow, these buttons will be
		sensitive.

Options:        'xhistogram' supports all standard X Toolkit command line
                arguments (see X(1)) as well as the XITE 'ximage(3)' toolkit
                command line options.

		Additional options:

		&-split
		If 'inimage' is multiband, the default behaviour is to
		show a total histogram for all the bands. This option
		forces one histogram per band.

Resources:      In order to specify resources, it is useful to know
                the  hierarchy of the widgets which compose 'xhistogram'.
                In the notation below, indentation indicates hierarchical 
                structure.

		&xhistogram                 (class XHistogram)
		| histogramShell            (class TopLevelShell)
		|  histogramForm            (class Form)
		|   histogramLabel          (class Label)
		|   histogram               (class Histogram)
		|   histogramMenuForm       (class Form)
		|    histogramModeForm      (class Form)
		|     histogramModeLabel    (class Label)
		|     histogramModeList     (class XfwfMultiList)
		|    histogramActionsForm   (class Form)
		|     histogramActionsLabel (class Label)
		|     histogramActionsList  (class XfwfMultiList)
		|   colormap                (class Colormap)

See also:       'xhistogram(3)', 'mkHisto(1)', 'ximage(3)', 'Histogram(3)',
                'XfwfMultiList(3)'

Files:          $XITE_HOME/etc/app-defaults/XHistogram

Author:		Otto Milvang

Revised:        Svein Bøe

Doc:            Svein Bøe

Id: 		$Id$
________________________________________________________________

*/

int main(int argc, char **argv)
{
  IMAGE img;
  int i;
  char *local_usage = "Usage: %s [-split] <inimage>\n";
  int split;

  Iset_message(TRUE);
  Iset_abort(TRUE);
  InitMessage(&argc, argv, ximage_app_usage_text(local_usage));

  Control_e = ximage_init(&argc, argv, "XHistogram", NULL, True);

  if (argc == 1) Usage(1, NULL);
  split = read_bswitch(&argc, argv, "-split");
  if (argc != 2) Usage(2, "Illegal number of arguments.\n");

  img = Iread_image(argv[1]);
  if (img == NULL) Error(2, "Can't open file %s\n", argv[1]);

  if (!split) xhistogram(NULL, img, NULL, 0, 0, ximage_display, NULL);
  else {
    for (i=1; i<=Inbands(img); i++) {
      xhistogram(NULL, img, img[i], 0, 0, ximage_display, NULL);
    }
  }

  ximage_mainloop();

  return(0);
}
