

/*

________________________________________________________________

        xpyramid.c
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



/*P:xpyramid*

________________________________________________________________

		xpyramid
________________________________________________________________

Name:		xpyramid - pyramid representation of image

Syntax:		xpyramid [<option>...] <filename>

Description:    Display an image with variable resolution and number of
                graylevels.

Options:        'xpyramid' supports all standard X Toolkit command line
                options (see X(1)) as well as the XITE 'ximage(3)' toolkit
                command line arguments.

Resources:      In order to specify resources, it is useful to know
                the  hierarchy of the widgets which compose 'xpyramid'.
                In the notation below, indentation indicates hierarchical 
                structure.

                The following hierarchy is constructed when the default
                resources/options for widget layout is used.

                The widgets of class XfwfButton can be replaced by class
                Command (from the Athena widget set) by using the 'ximage(3)'
                option '-widgetset' or corresponding application resource.

		&xpyramid                (class XPyramid)
		| popup                  (class TransientShell)
		|   dialog               (class Dialog)
		|     label              (class Label)
		|     command            (class Command)
		| colorsmenu             (class SimpleMenu)
		|   menuLabel            (class SmeBSB)
		|   backgroundcol        (class SmeBSB)
		|   Work-map             (class SmeBSB)
		| imageShell             (class TopLevelShell)
		|   imageForm            (class ImageForm)
		|     image              (class Image)
		| xpyramid control panel (class TopLevelShell)
		|   topContainer         (class Form)
		|     label              (class Label)
		|     labelspace         (class Label)
		|     listspace          (class List)
		|     labelcolor         (class Label)
		|     listcolor          (class List)
		|     kill               (class XfwfButton)

See also:       'ximage(3)', 'pyramid(1)', 'Image(3)'

Files:          $XITE_HOME/etc/app-defaults/XPyramid

Author:		Otto Milvang and Svein Bøe

Revised:        Svein Bøe

Doc:            Svein Bøe

Id: 		$Id$
________________________________________________________________

*/



#include <xite/includes.h>
#include <X11/Xos.h>
#include <X11/Intrinsic.h>
#include <ctype.h>
#include <stdlib.h>
#include <X11/Shell.h>
#include <X11/StringDefs.h>
#include <X11/Xaw/List.h>
#include <xite/biff.h>
#include <xite/pyramid.h>
#include <xite/Image.h>
#include <xite/ShellWids.h>
#include <xite/Xcolor.h>
#include <xite/message.h>
#include <xite/ximage.h>
#include <xite/Cmap.h>
#include <xite/Visual.h>
#include XITE_MALLOC_H
#include XITE_STDIO_H
#include <xite/mallocp.h>
#include <xite/debug.h>



extern Display *dpy_e;
static int cells_s;
static IMAGE img_s;
static int current_s = 0, max_s = 0, colors_s = 2;

typedef enum {
  XIMAGE,
  LABEL,
  LABEL_SPACE,
  LIST_SPACE,
  LABEL_COLOR,
  LIST_COLOR,
  QUIT
} widget_number;

static int num_widgets_s = QUIT + 1;
static layout *layouts_s;

#define wid_XIMAGE (layouts_s ? WidgetOfLayout(layouts_s[XIMAGE]) : NULL)
#define wid_LABEL (layouts_s ? WidgetOfLayout(layouts_s[LABEL]) : NULL)
#define wid_LABEL_SPACE (layouts_s ? \
			 WidgetOfLayout(layouts_s[LABEL_SPACE]) : NULL)
#define wid_LIST_SPACE (layouts_s ? \
			WidgetOfLayout(layouts_s[LIST_SPACE]) : NULL)
#define wid_LABEL_COLOR (layouts_s ? \
			 WidgetOfLayout(layouts_s[LABEL_COLOR]) : NULL)
#define wid_LIST_COLOR (layouts_s ? \
			WidgetOfLayout(layouts_s[LIST_COLOR]) : NULL)
#define wid_QUIT (layouts_s ? WidgetOfLayout(layouts_s[QUIT]) : NULL)

static char labelText_s[81] = "Pyramid";

static char *graylevels_s[] =
{
  "              2              ",
  "              4              ",
  "              8              ",
  "             16              ",
  "             32              ",
  "             64              ",
  "            128              ",
};

static char *spacelevels_s[16];



static void Spatial(Widget iw, XtPointer client_data, XtPointer call_data)
{
  int choice;
  XawListReturnStruct *list = (XawListReturnStruct *) call_data;

  if (list) choice = list->list_index;
  else choice = max_s - 1;

  current_s = max_s - choice;
  
  XtVaSetValues(wid_XIMAGE, XtNimageBand, img_s[current_s], NULL);
  sprintf(labelText_s, "Pyramid %3ld x %3ld pixels,  %3d colors",
	  Ixsize(img_s[current_s]), Iysize(img_s[current_s]), colors_s);
  XtVaSetValues(wid_LABEL, XtNlabel, labelText_s, NULL);

} /* Spatial() */




static void Colors(Widget iw, XtPointer client_data, XtPointer call_data)
{
  XColor col[256];
  Visual *visual = NULL;
  Colormap cmap = NULL;
  int choice;

  XawListReturnStruct *list = (XawListReturnStruct *) call_data;

  if (cells_s == 2) return;

  if (list) choice = list->list_index;
  else choice = 0;

  colors_s = 2 << choice;

  XtVaGetValues(wid_XIMAGE, XtNvisual, &visual, XtNcolormap, &cmap, NULL);

  /* Change the central part of the colormap (pixels 64..191).
   * It is assumed that the image is shown with 'reduced-color' display.
   */
  InitColortablePixels(col, 192 /* len */, 0 /* offset */,
		       (ClassOfVisual(visual) == DirectColor));
  XQueryColors(dpy_e, cmap, &col[64], 128);

  SpreadGray(col, colors_s /* num_colors */,
	     64 /* pix_min */, 128 /* num_cells */);

  XStoreColors(dpy_e, cmap, &col[64], 128);
  XFlush(dpy_e);

  sprintf(labelText_s,"Pyramid %3ld x %3ld pixels,  %3d colors",
	  Ixsize(img_s[current_s]), Iysize(img_s[current_s]), colors_s);
  XtVaSetValues(wid_LABEL, XtNlabel, labelText_s, NULL);

} /* Colors() */


 
static void make_labels(void)
{
  int i = 1;

  for (i = 1; i <= max_s; i++) {
    spacelevels_s[max_s - i] = (char *) malloc(30);
    sprintf(spacelevels_s[max_s - i], "        %4ld x %4ld         ",
	    Ixsize(img_s[i]), Iysize(img_s[i]));
  }

} /* make_labels() */



static void prepare_layout(void)
{
  layouts_s = (layout *) malloc(num_widgets_s * sizeof(layout));

  layouts_s[XIMAGE] = (layout)
    Make_image_layout(NULL, "Image", "imageShell", NULL, NULL, XIMAGE, img_s,
		      add_to_image_callbacks(NULL, XtNdestroyCallback,
					     ximage_exit, NULL, True));
  layouts_s[LABEL] = (layout)
    Make_label_layout(NULL, "Label", "label", "xpyramid control panel", NULL,
		      LABEL);
  layouts_s[LABEL_SPACE] = (layout)
    Make_label_layout(NULL, "Label", "labelspace", NULL, NULL, LABEL_SPACE);
  layouts_s[LIST_SPACE]  = (layout)
    Make_list_layout(NULL, "List", "listspace", NULL, NULL, LIST_SPACE,
		     max_s, spacelevels_s, Spatial, NULL);
  layouts_s[LABEL_COLOR] = (layout)
    Make_label_layout(NULL, "Label", "labelcolor", NULL, NULL, LABEL_COLOR);
  layouts_s[LIST_COLOR] = (layout)
    Make_list_layout(NULL, "List", "listcolor", NULL, NULL, LIST_COLOR,
		     XtNumber(graylevels_s), graylevels_s, Colors, NULL);
  layouts_s[QUIT] = (layout)
    Make_command_layout(NULL, "Command", "kill", NULL, NULL, QUIT,
			ximage_exit, NULL);

} /* prepare_layout() */



int main(int argc, char **argv)
{
  Widget toplevel;
  IMAGE inimg;
  int status;

  Iset_message(TRUE);
  Iset_abort(FALSE);
  InitMessage(&argc, argv,
	      ximage_app_usage_text("Usage: %s [<option>...] <filename>\n"));

  toplevel = ximage_init(&argc, argv, "XPyramid", NULL, True);

  if (argc == 1) Usage(1, NULL);
  if (argc != 2) Usage(2, "Illegal number of arguments.\n");

  if ((cells_s = DisplayCells(dpy_e, DefaultScreen(dpy_e))) > 2) colors_s = 1;
  
  inimg = Iread_image(argv[1]);
  if (inimg == NULL) {
    fprintf(stderr,"Can't open file %s\n", argv[1]);
    exit(2);
  } 

  if ((status = mkPyramidAver2(inimg[1], &img_s, 0)) != 0)
    Error(2, "Error %d in mkPyramidAver2().\n", status);

  max_s     = Inbands(img_s);
  current_s = max_s;
  make_labels();

  prepare_layout();
  ximageLayout(toplevel, layouts_s, num_widgets_s);

  XtVaSetValues(wid_XIMAGE, XtNimageBand, img_s[current_s], NULL);
  sprintf(labelText_s,"Pyramid %3ld x %3ld pixels,  %3d colors",
	  Ixsize(img_s[current_s]), Iysize(img_s[current_s]), 2);
  XtVaSetValues(wid_LABEL, XtNlabel, labelText_s, NULL);
  XtVaSetValues(wid_LIST_COLOR, XtNsensitive, cells_s > 2, NULL);

  XawListHighlight(wid_LIST_SPACE, 0);
  if (cells_s > 2) XawListHighlight(wid_LIST_COLOR, 0);

  Colors(NULL, (XtPointer) MallocIntP(1), NULL);
  ximage_mainloop();

  return(0);
}
