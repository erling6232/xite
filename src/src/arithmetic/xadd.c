

/*

________________________________________________________________

        xadd.c
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



#ifdef MAIN

#include <xite/includes.h>
#include <X11/Xos.h>
#include <X11/Intrinsic.h>
#include <X11/StringDefs.h>
#include <xite/biff.h>
#include <stdlib.h>
#include <xite/arithmetic.h>
#include <xite/Image.h>
#include <xite/ximage.h>
#include <xite/Xdialog.h>
#include <xite/ShellWids.h>
#include XITE_STDIO_H
#include <xite/message.h>
#include <xite/mallocp.h>
#include <xite/debug.h>

static IMAGE img1_s, img2_s, addimg_s;
static IBAND addband_s;
static int bandnr_s;
static char *outfile_s;
static float f = 0.5;
static int neg1_s = 0, neg2_s = 0;

static void nextprev(int diff);

typedef enum {
XIMAGE,
SCROLLABEL,
SCROLLBAR,
QUIT,
NEGATE1,
NEGATE2,
PREV,
NEXT,
SAVE
} widget_number;

#define WIDGETS (SAVE + 1)

static char *widgets[WIDGETS] = {
  "Image:image:imageShell",
  "Label:scrollabel:xadd control panel",
  "Scroll:ximagescroll",
  "Command:Quit",
  "Toggle:Negate-1",
  "Toggle:Negate-2",
  "Command:Prev",
  "Command:Next",
  "Command:Save",
};

static Widget rwidgets[WIDGETS];



/*P:xadd*

________________________________________________________________

		xadd
________________________________________________________________

Name:		xadd - add two BIFF images and display under X11

Syntax:		xadd [<option>...] <inimage-1> <inimage-2> [<outimage>]

Description:    'xadd' calculates the pixelwise weighted sum of two images.
                The weight is controlled by a scrollbar. One or both
		images may be negated. 

Options:	'xadd' supports all standard X Toolkit command line
		arguments (see X(1)) as well as the XITE 'ximage(3)' toolkit
		command line arguments.

Restrictions:	All images must be of equal size.

Resources:      In order to specify resources, it is useful to know
		the  hierarchy of the widgets which compose 'xadd'.
		In the notation below, indentation indicates hierarchical 
		structure.

		The widgets of class XfwfButton and XfwfToggle can be replaced
                by class Command and Toggle (from the Athena widget set) by
                using the 'ximage(3)' option '-widgetset' or corresponding
                application resource.

		&xadd                (class XAdd)
		| imageShell         (class TopLevelShell)
		|   imageForm        (class ImageForm)
		|     image          (class Image)
		|     menubar        (class XfwfMenuBar)
                |       colors       (class XfwfPullDown)
                |       tools        (class XfwfPullDown)
                |       options      (class XfwfPullDown)
		| xadd control panel (class TopLevelShell)
		|   topContainer     (class Form)
		|     scrollabel     (class Label)
		|     ximagescroll   (class Scrollbar)
		|     Quit           (class XfwfButton)
		|     Negate-1       (class XfwfToggle)
		|     Negate-2       (class XfwfToggle)
		|     Prev           (class XfwfButton)
		|     Next           (class XfwfButton)
		|     Save           (class XfwfButton)
		| colorsmenu         (class SimpleMenu)
		|   menuLabel        (class SmeBSB)
		|   backgroundcol    (class SmeBSB)
		|   Work-map         (class SmeBSB)
		|   colorsep         (class SmeLine)
		|   White - 256      (class SmeBSB)
		|   Black - 256      (class SmeBSB)
		|   Red - 256        (class SmeBSB)
		|   Green - 256      (class SmeBSB)
		|   Blue - 256       (class SmeBSB)
		|   Spectrum - 256   (class SmeBSB)
		|   Hue - 256        (class SmeBSB)
		|   Rainbow - 256    (class SmeBSB)
		| toolsmenu          (class SimpleMenu)
		|   menuLabel        (class SmeBSB)
		|   imageinfo        (class SmeBSB)
		|   histogram        (class SmeBSB)
		|   slice            (class SmeBSB)
		| optionsmenu        (class SimpleMenu)
		|   menuLabel        (class SmeBSB)
		|   optionsmenusep   (class SmeLine)
		|   optionsmenuentry (class SmeBSB)
		|   optionsmenuentry (class SmeBSB)
		|   optionsmenuentry (class SmeBSB)
		|   optionsmenusep   (class SmeLine)
		|   optionsmenuentry (class SmeBSB)
		|   optionsmenuentry (class SmeBSB)
		|   optionsmenuentry (class SmeBSB)
		
See also:	'addw(1)', 'ximage(3)', 'Image(3)'

Files:          $XITE_HOME/etc/app-defaults/XAdd

Author:		Otto Milvang

Revised:        Svein Bøe

Examples:	| xadd img1 img2 img3

Id: 		$Id$
________________________________________________________________

*/



/*L*

________________________________________________________________

		ximage_work
________________________________________________________________

Name:		ximage_work
Syntax:		
Description:
Return value:
Author:		Otto Milvang
________________________________________________________________

*/


static void ximage_work(Widget wid, XtPointer client_data, XtPointer call_data)
{
  IBAND b1, b2;
  float f1, f2, off;
  char *name, lab[100];
  int i, save;
  short width;
  widget_number wid_num = (widget_number) *((int *) client_data);

  save = 0;
  switch((int) wid_num)
    {
    case XIMAGE:
    case SCROLLABEL:
      return;
      /* break; */
    case SCROLLBAR:
      f = *(float*) call_data;
      break;
    case QUIT:
      ximage_exit(wid, (XtPointer) 0, (XtPointer) 0);
      break;
    case NEGATE1:
      neg1_s = 1 - neg1_s;
      break;
    case NEGATE2: 
      neg2_s = 1 - neg2_s;
      break;
    case PREV:
      nextprev(-1);
      break;
    case NEXT: 
      nextprev(1);
      break;
    case SAVE: 
      save = 1;
      if (outfile_s == NULL)
	save = SelectWriteFile(TopLevelWidget(wid), writeBIFF, &name);
      else name = outfile_s;

      if (save != 1) return;
      break;
    }
  
  f1 = neg1_s ? -f : f;
  f2 = neg2_s ? f-1.0 : 1.0-f;
  off = (neg1_s ? -256.0 * f1 : 0) + (neg2_s ? -256.0 * f2 : 0);
  b1 = img1_s[bandnr_s]; 
  b2 = img2_s[bandnr_s];
  if (save) {
    int status;

    for(i=1; i<Inbands(img1_s); i++)
      addw(img1_s[bandnr_s], img2_s[bandnr_s], addimg_s[bandnr_s],
	   f1, f2, (int) off);
    sprintf(lab, "%5.1f %% / %5.1f %%", 
	    f1 * 100.0, f2 * 100.0);
    Ihistory(addimg_s, "xadd", lab);

    if ((status = Iwrite_image(addimg_s, name)) != Iok)
      InformationBox(wid, Ierr_message(status),
		     "xadd - information", "xadd - info");

    if (outfile_s != NULL)
      ximage_exit(wid, (XtPointer) 0, (XtPointer) 0);
  } else {
    /* Some other action than SAVE. */

    addw(b1, b2, addband_s, f1, f2, (int) off);

    ImageRedisplay((ImageWidget) rwidgets[XIMAGE], UPDATE_ZOOMPAN);
    sprintf(lab,"Band %d: %5.1f %% / %5.1f %%", bandnr_s, 
	    f1 * 100.0, f2 * 100.0);
    XtVaGetValues(rwidgets[SCROLLABEL], XtNwidth, &width,  NULL);
    XtVaSetValues(rwidgets[SCROLLABEL], XtNlabel, lab, XtNwidth, width,  NULL);
  }
}

static void nextprev(int diff)
{
  bandnr_s += diff;
  XtVaSetValues(rwidgets[PREV], XtNsensitive, bandnr_s > 1, NULL);
  XtVaSetValues(rwidgets[NEXT], XtNsensitive, bandnr_s < Inbands(img1_s),
		NULL);
}


/*L*

________________________________________________________________

		main
________________________________________________________________

Name:		main
Syntax:		| main(argc, argv)
                | int argc;
                | char **argv;
Description:    Start Xt, display image, read events
Return value:
Author:		Otto Milvang
________________________________________________________________

*/

static void checkimg(char **argv, IMAGE img1, IMAGE img2)
{
  int i, nbands, xsize, ysize;
  if (img1 == NULL || img2 == NULL)
    {
      fprintf(stderr, "Can't find image: %s\n",argv[img1 == NULL ? 1 : 2]);
      exit(2);
    }
  if (Inbands(img1) != Inbands(img2)) 
    {
      fprintf(stderr, "Input images must be of equal size\n");
      exit(2);
    }
  nbands = Inbands(img1);
  xsize = Ixsize(img1[1]);
  ysize = Iysize(img1[1]);
  for (i=1; i<=nbands; i++)
    if (Ixsize(img1[i]) != xsize || Iysize(img1[i]) != ysize ||
	Ixsize(img2[i]) != xsize || Iysize(img2[i]) != ysize) 
      {
	fprintf(stderr, "All bands must be of equal size\n");
	exit(2);
      }
  for (i=1; i<=nbands; i++)
    if (Ipixtyp(img1[i]) != Iu_byte_typ || Ipixtyp(img2[i]) != Iu_byte_typ)
      {
	fprintf(stderr, "All bands must be of type Unsigned Byte\n");
	exit(2);
      }
}

int main(int argc, char **argv)
{
  IBAND b1, b2;  
  Widget toplevel;
  char *title1, *title2, *title = NULL;
  char *local_usage =
    "Usage:  %s [<option>...] <inimage> <inimage> [<outimage>]\n";
  Dimension width;

  Iset_message(1);
  Iset_abort(0);
  InitMessage(&argc, argv, ximage_app_usage_text(local_usage));

  toplevel = ximage_init(&argc, argv, "XAdd", NULL, TRUE);
  if (argc == 1) Usage(1, NULL);

  if (argc != 3 && argc != 4) Usage(1, "Illegal number of arguments.\n");

  img1_s = Iread_image(argv[1]);
  img2_s = Iread_image(argv[2]);

  outfile_s = argc == 4 ? argv[3] : NULL;
  checkimg(argv, img1_s, img2_s);
  bandnr_s = 1;
  b1 = img1_s[bandnr_s]; 
  b2 = img2_s[bandnr_s];
  title1 = Ititle(img1_s);
  title2 = Ititle(img2_s);
  if (title1 && title2) {
    title = malloc(strlen(title1) + strlen(title2) + 4);
    (void) strcpy(title, title1);
    (void) strcat(title, " + ");
    (void) strcat(title, title2);
  }
  addimg_s = Icopy_init(img1_s);
  Iset_title(addimg_s, title);
  addband_s = Imake_band(Iu_byte_typ, Ixsize(b1), Iysize(b1));
  addimg_s[1] = addband_s;
  Ihistory(addimg_s, argv[0], "End of Image 1 history");
  Icopy_text(img2_s, addimg_s);
  Ihistory(addimg_s, argv[0], "End of Image 2 history");
  addw(b1, b2, addband_s, 0.5, 0.5, 0);

  ximage_layout(toplevel, addband_s, widgets, rwidgets, WIDGETS, ximage_work,
		NULL);
  XtVaGetValues(rwidgets[SCROLLABEL], XtNwidth, &width, NULL);
  XtVaSetValues(rwidgets[SCROLLABEL], XtNlabel, "Band 1:  50.0 % /  50.0 %",
		XtNwidth, width, NULL);
  nextprev(0);
  ximage_mainloop();
  return(0);
}

#endif /* MAIN */
