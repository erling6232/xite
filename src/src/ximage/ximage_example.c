
#include <xite/includes.h>
#include <xite/biff.h>
#include <xite/negate.h>
#ifdef HAVE_STDIO_H
#  include <stdio.h>
#endif
#include <xite/Image.h>
#include <xite/ximage.h>     /* For ximage toolkit. */
#include <xite/message.h>    /* For InitMessage(), Usage(), Error() etc. */
#include <xite/Xdialog.h>    /* For ConfirmationBox(). */
#include <xite/ShellWids.h>  /* For TopLevelWidget(). */

#ifdef MAIN

/*P:ximage_example*

________________________________________________________________

		ximage_example
________________________________________________________________

Name:		ximage_example - example application for the XITE ximage
                toolkit

Syntax:		| ximage_example <filename>

Description:    Sample program for the XITE 'ximage' toolkit.

                For this program to operate properly, X11 resources must
		also be set. They can be found in the file
		
		| $XITE_HOME/etc/app-defaults/XImageExample

		'ximage_example' will find its resource file automatically
		if XITE is setup correctly at your site.

Options:        'ximage_example' supports all standard X Toolkit command line
                arguments (see X(1)) as well as the XITE 'ximage(3)' toolkit
                command line options.

Resources:      In order to specify resources, it is useful to know
                the  hierarchy of the widgets which compose 'ximage_example'.
                In the notation below, indentation indicates hierarchical 
                structure.

                The widgets of class XfwfButton can be replaced by class
                Command (from the Athena widget set) by using the 'ximage(3)'
                option '-widgetset' or corresponding application resource.

                &ximage_example      (class XImageExample)
	        | popup              (class TransientShell)
		|   dialog           (class Dialog)
		|     label          (class Label)
		|     command        (class Command)
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
		| overlaysmenu       (class SimpleMenu)
		|   menuLabel        (class SmeBSB)
		|   backgroundovl    (class SmeBSB)
		|   Std-overlay      (class SmeBSB)
		|   colorsep         (class SmeLine)
		|   White - mono     (class SmeBSB)
		|   Black - mono     (class SmeBSB)
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
		| toolsmenu          (class SimpleMenu)
		|   menuLabel        (class SmeBSB)
		|   imageinfo        (class SmeBSB)
		|   histogram        (class SmeBSB)
		|   slice            (class SmeBSB)
		| imageShell         (class TopLevelShell)
		|   imageForm        (class ImageForm)
		|     menubar        (class XfwfMenuBar)
		|       colors       (class XfwfPullDown)
		|       tools        (class XfwfPullDown)
		|       options      (class XfwfPullDown)
		|     image          (class Image)
		| Control panel      (class TopLevelShell)
		|   topContainer     (class Form)
		|     Image Example  (class Label)
		|     menu           (class MenuButton)
		|       menu         (class SimpleMenu)
		|         men1       (class SmeBSB)
		|         men2       (class SmeBSB)
		|     list           (class List)
		|     Quit           (class XfwfButton)
		|     Read file      (class XfwfButton)
		|     Negate         (class XfwfButton)

See also:	'ximage(3)', 'xadd(1)', 'xfft(1)', 'xfilter(1)',
                'xhistogram(1)', 'xmovie(1)', 'xregion(1)', 'xpyramid(1)',
                'xshow(1)', 'Image(3)', 'ImageOverlay(3)'

Files:          $XITE_HOME/etc/app-defaults/XImageExample

Author:		Otto Milvang and Svein Bøe

Example:        ximage_example mona.img

Id: 		$Id$
________________________________________________________________

*/

/* Constants, one of which will be sent to the ximage_work() function via the
 * client_data argument. Each constant represents a widget.
 */
typedef enum {
  XIMAGE,
  HEADER,
  MENU,
  LIST,
  QUIT,
  READ,
  NEGATE
} widget_type;

#define WIDGETS (NEGATE + 1)

/* Array of widget pointers, one element for each widget in the application. */
static Widget rwidgets[WIDGETS];

/* Array which determines the type and instance name of each widget.
 * For label widgets, the name is also the label.
 * With a third string, this
 */
static char *widgets[WIDGETS] = {
  "Image:image:imageShell",
  "Label:Image Example",
  "Menu:menu::men1:men_l\\ab1:20:men2:men_lab2:21",
  "List:list::lis_lab1:lis_l\\:ab2",
  "Command:Quit",
  "Command:Read file",
  "Command:Negate",
};

static IMAGE img;
static IBAND band;




/*L*

________________________________________________________________

		ximage_work
________________________________________________________________

Name:		ximage_work
Syntax:		| static void ximage_work(Widget wid,
                |    XtPointer client_data, XtPointer call_data);
Description:    This function determines which of the widgets that wants
                some work done. The 'client_data' argument is a reference
		to an integer which represents the widget. Take the correct
		action for the widget.
Author:		Otto Milvang
________________________________________________________________

*/

static void ximage_work(Widget wid, XtPointer client_data, XtPointer call_data)
{
  widget_type wid_typ;
  ImageCallback button;

  wid_typ = *((widget_type *) client_data);
  switch(wid_typ)
    {
    case HEADER:
      break;
    case XIMAGE:
      button = (ImageCallback) call_data;
      if (button->reason != ACTIVATE) return;
      if (button->event && button->event->type == ButtonPress)
	switch(button->event->xbutton.button)
	  {
	  case 1: fprintf(stderr, "Left button pressed\n"); break;
	  case 2: fprintf(stderr, "Middle button pressed\n"); break;
	  case 3: fprintf(stderr, "Right button pressed\n"); break;
	  }
      fprintf(stderr, "x=%d, y=%d, value=%d \n",
           button->x, button->y, *( (unsigned char *) button->data) );
      break;
    case QUIT:
      if (ConfirmationBox(TopLevelWidget(wid), "Really quit?", "No", "Yes",
			  "ImageExample - confirmation",
			  "ImageExample - conf."))
	ximage_exit(wid, (XtPointer) 0, (XtPointer) 0);
      break;
    case READ:
      Message(0, "Read file button pushed.\n");
      break;
    case NEGATE: 
      negate(band, band);
      /* Force redisplay, the pixmap must be refilled. */
      ImageRedisplay((ImageWidget) rwidgets[XIMAGE], UPDATE_ZOOMPAN);
      break;
    default:
      break;
    }
}



void main(int argc, char **argv)
{
  Widget toplevel;

  Iset_message(TRUE);
  Iset_abort(TRUE);
  InitMessage(&argc, argv, ximage_app_usage_text("Usage: %s <BIFF-file>\n"));

  toplevel = ximage_init(&argc, argv, "XImageExample",
			 NULL, TRUE);   /* fallback, warn */

  if (argc == 1) Usage(1, NULL);
  if (argc !=2) Usage(2, "Bad number of arguments.\n");

  img = Iread_image(argv[1]);
  if (img == NULL) Error(2, "Can't find %s\n",argv[1]);

  band = img[1];

  ximage_layout(toplevel, band, widgets, rwidgets, WIDGETS, ximage_work,
		NULL);
  ximage_mainloop();
}

#endif /* MAIN */
