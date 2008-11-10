

/*C*

________________________________________________________________

        xshow.c
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



/*P:Xite=xite*/
/*P:XITE=xite*/
/*P:xite*

________________________________________________________________

		XITE
________________________________________________________________

Name:           xite, Xite, XITE - X-based Image Processing Tools and
                Environment
Description:    XITE is developed at the Image Processing Laboratory,
                Department of Informatics, University of Oslo, Norway.
		XITE is	software written in C for use under flavors of
		the UNIX operating system. It consists of
		
		&File format BIFF (Blab Image File Format).
		&X11-based displayprogram with Image widget and GUI.
		&More than 170 programs for image processing.
		&More than 300 subroutines for image processing.
		&On-line documentation of routines and programs.
		&Some images in BIFF format.

                -&'xshow(1)' gives access to just about all the algorithms
		through a menu interface.

		The XITE WWW home page is
		'http://www.ifi.uio.no/~blab/Software/Xite/',
		and the ftp repository is '/pub/blab/xite' at 'ftp.ifi.uio.no'.

See also:       'xshow(1)', 'ximage(3)'

________________________________________________________________

*/


/*P:xshow*

________________________________________________________________

		xshow
________________________________________________________________

Name:		xshow - XITE X11 displayprogram for images and GUI for image
                processing

Syntax:		'xshow' [<option>...] [<BIFF-file>...] [+] [+<BIFF-file>...]

Description:    'xshow' works under the X Window System and displays a
                control window, a menu window and optionally one or more
		image windows. The image data are read from BIFF
		(Blab Image File Format) files. The menu-interface gives
		access to a large number of image processing algorithms.

		Several popular image formats may be imported to and
		exported from 'xshow'.

Display requirements:
		'xshow' was originally developed for color workstations
		with 8-bit PseudoColor displays, capable of showing 256
		different colors at the same time. Some work has been done
		also for other visual types, especially for
		24-bit DirectColor and to a certain extent 24-bit TrueColor.
		'xshow' will give a warning message if it does not like
		the kind of visual you are using.

		You can choose the visual type for the windows used to display
		images. Refer to 'ximage(3)' and 'Visual(3)' for information
		on how to choose the visual type. The default behaviour is to
		use the PseudoColor visual type for image windows and the
		default visual type for menus, buttons etc.

Options:	'xshow' accepts all of the standard X Toolkit command line
                options as well as the options defined by the 'ximage' toolkit.
		Refer to 'ximage(3)' for information on the 'ximage' options.

		Additionally, 'xshow' supports the options listed below.

		&-dialog_defaults
		    Automatically accept dialog defaults, without popping
		    up any dialog window.
		&-i chan 
		    Use input channel 'chan'. This option may be used when
		    'xshow' is forked out of other programs. Refer to
		    'start_xshow(3)' and 'fork_xshow(1)'.
		&-m filename
		    This file contains the menu layout. No other menu files
		    are read. Default: $Dir/xshow_menues and $Dir/.xshowrc,
		    where $Dir represents each directory in XSHOWPATH.
		    See also the section 'Program menus' below.

Arguments:	Image files or colortable files in BIFF format.

		A + in front of an image filename indicates that the
		image is an overlay. A space-delimited + indicates an empty
		overlay. The overlay will be written to the image listed
		in front of it on the command-line. A + sign in front of a
		colortable filename indicates an overlay colortable.

		All colortables given as arguments will be available from the
		Colors popup menu within the images and from the menubar above
		each image (see below). This resembles the '-ct' option from
		the 'ximage' toolkit. However, an image will be displayed with
		the last non-option colortable argument given to the left of
		the image argument on the command-line. With no such
		non-option colortable argument, the initial colortable for all
		command-line arguments is determined by the '-ct' option from
		the 'ximage' toolkit. Without this option, the images are
		displayed initially with a gray-scale colortable.

		Arguments which are overlay colortables, are treated the
		same way as ordinary colortables, except that they become
		available from the OverlayColors menu and that the resembling
		'ximage' toolkit option is '-ovt'.

		See the section 'Examples' further down.

X toolkit options:
                The X Toolkit Intrinsics automatically accepts the following
		options.

		&-display display
		This option specifies the name of the X server to use.

		&-geometry geometry
		This option specifies the initial size and location of the
		window.

		&-bg color, -background color
		Either option specifies the color to use for the window
		background.

		&-bd color, -bordercolor color
		Either option specifies the color to use for the window border.

		&-bw number, -borderwidth number
		Either option specifies the width in pixels of the window
		border.

		&-fg color, -foreground color
		Either option specifies the color to use for text or graphics.

		&-fn font, -font font
		Either option specifies the font to use for displaying text.

		&-iconic
		This option indicates that the user would prefer that the
		application''s windows initially are not visible, as if the
		windows had been immediately iconified by the user. Window
		managers may choose not to honor the application''s request.

		&-name
		This option specifies the name under which resources for the
		application should be found. This option is useful in shell
		aliases to distinguish between invocations of an application,
		without resorting to creating links to alter the executable
		file name.

		&-rv, -reverse
		Either option indicates that the program should simulate
		reverse video if possible, often by swapping the foreground
		and background colors. Not all programs honor this or
		implement it correctly. It is usually only used on monochrome
		displays.

		&+rv
		This option indicates that the program should not simulate
		reverse video. This is used to override any defaults since
		reverse video doesn''t always work properly.

		&-selectionTimeout
		This option specifies the timeout in milliseconds within which
		two communicating applications must respond to one another for
		a selection request.

		&-synchronous
		This option indicates that requests to the X server should be
		sent synchronously, instead of asynchronously. Since Xlib
		normally buffers requests to the server, errors do not
		necessarily get reported immediately after they occur. This
		option turns off the buffering so that the application can be
		debugged. It should never be used with a working program.

		&-title string
		This option specifies the title to be used for this window.
		This information is sometimes used by a window manager to
		provide some sort of header identifying the window.

		&-xnllanguage language[_territory][.codeset]
		This option specifies the language, territory, and codeset for
		use in resolving resource and other filenames.

		&-xrm resourcestring
		This option specifies a resource name and value to override
		any defaults. It is also very useful for setting resources
		that don''t have explicit command line arguments.

Run a program from xshow:
		From the menu window it is possible to start programs/jobs.
		Parameters to these jobs may be existing images, and
		the result may appear as new images.

		&1.
		Select your choice of menu entry in the menu window. A menu
		entry may pop up a submenu. If the selected program expects
		input image(s),	the Mouse state field in the Control window
		will change to JC.
		&2.
		Select an Image (<Btn3>), a band (<Btn2>) or an area
		(<Btn1>) as input to the program.

		-&If the program reads from stdin or writes to stdout,
		a text window for the job will be created.
		You may press 'Shift <Btn2>' to save/delete the
		text-window.

		If the program writes to stderr, this will be
		written in the message area of the Control window.

		To abort a program: Press the left mouse button in the mouse
		state field.

		The menu hierarchy may be manipulated with the mouse only,
		or with a combination of mouse button activity and keyboard
		accelerators. The keyboard accelerators are displayed in the
		menu entry labels. The mouse pointer must be inside the
		Control window for the accelerators to work. There are
		accelerators for must submenus and command entries.

		In addition to the accelerators displayed, one may use
		the submenu popup-accelerators without the Ctrl key to
		get permanent versions of the submenus. These permanent
		versions are extracts of the originals, not containing any
		submenu buttons. They may be moved and iconified just as
		other windows (unless your windowmanager has been told
		differently).

		Inside the permanent submenu extracts, one may navigate
		with arrow keys or Ctrl-n (next entry) or Ctrl-p (previous
		entry). A menu entry may be activated by hitting the
		Return key (or clicking the left mouse-button).

		A permanent submenu may be removed by hitting the "q" key
		while the mouse pointer is somewhere inside the submenu.

Control window: 'xshow' may display several images. Each image band has its
                own window (for an exception, see option '-rgb' above).
		Histogram windows and slice windows (color palette) may be
		created from the image windows. The 'Control window' gives
		information about existing images and running jobs.

		The various fields of the Control window are:
                &Menu
		From the control menu you can press (using the left mouse
		button, <Btn1>):

		    &&Macro
		    This toggle button indicates whether or not the
		    macro-generating facility is active. It
		    provides a mechanism for running a sequence of
		    algorithms on several images, with a minimum of mouse
		    button activity (and with the same arguments to the
		    algorithms for each run).

		    Press this macro button and you will be asked to give a
		    name for a macro. All activity generated via the menu
		    window will be logged to a file with the macro name. The
		    macroname can optionally be added to the menu hierarchy as
		    the last entry in a menu file. You will be asked for the
		    location of such a file. (See below, in the section
		    'Program menus'.) The menu hierarchy of 'xshow' is updated
		    immediately to reflect the change. The macro file can
		    alternatively be executed from the UNIX command-line.

		    When running a macro from within the 'xshow' menu
		    hierarchy, you must give the input images in the
		    same order as when the macro was generated (by
		    clicking a mouse button as described in the section
		    'Run a program from xshow').

		    A macro run may leave fewer images displayed than
		    when the macro was generated. The rule here is that
		    only those images which were not used as input to
		    other commands (within the macro), are displayed.
		    If you want to display also intermediate results,
		    use the menu entry 'Copy image' in the 'File' menu
		    to display an extra copy while generating the macro.

		    A current limitation of the macro facility is that
		    any dialog input during macro generation is fixed in
		    the resulting macro (which of course can be manually
		    edited).

		    &&Help
		    Start a help session. One of the following will
		    happen:
		    | 1) The man page for xshow is displayed by
		    |    starting the "man" program.
		    | 2) "xman" is started.
		    | 3) The WWW browser "netscape" is started on
		    |    the front page of the online hypertext
		    |    reference manual.
		    | 4) The WWW browser "Mosaic" is started on the
		    |    front page of the online hypertext
		    |    reference manual.
		    | 5) The WWW browser "lynx" is started on the
		    |    front page of the online hypertext
		    |    reference manual.
		    | 6) The formatted manual page for xshow or some
		    other specified file is displayed with "more",
		    "less" or "cat".
		    Which one of the above alternatives that is chosen,
		    depends on the XShow X11 resource xiteHelper.
		    This resource should be a colon-separated list of
		    names of help programs. An example of setting this
		    resource in a resource file, is:

		    XShow*XiteHelper: netscape:Mosaic:lynx:xman:man

		    In this case, netscape is started. If it fails,
		    Mosaic will be started. If this also fails, lynx will
		    be started, and so on.
		    
		    Options to the help programs may be set with the
		    resource xiteHelperOption. The setting is not used
		    for Mosaic.
		    
		    The resource setting for help programs may be
		    overridden by corresponding environment variables,
		    called XITE_HELPER and XITE_HELPER_OPTION.
		    
		    The environment variable XITE_DOC determines
		    where the reference manual is found, in
		    $XITE_DOC/ReferenceManual/Contents.html.

		    The environment variable XITE_MAN determines where
		    the formatted manual page is found by "more", "less"
		    and "cat".
		    
		    &&Quit
		    Quit 'xshow'.
		&Info
		Gives some information about the state

		    &&Images
		    Number of images.

		    &&Jobs
		    Number of running jobs started by 'xshow'. Press left mouse
		    button to get a list of all running jobs. The list is
		    written in the message area.

		    &&Mouse
		    Mouse state. N (Normal), JC (Job Control). Jobs started
		    with the special argument '<infile>' (in the menu file),
		    will read input image from one of the existing images. JC
		    indicates that the program expects to read an image. The
		    job may be aborted by pressing the left mouse button in
		    the mouse field.

		&Active image
		When the cursor is on an image, 'xshow' will display
		    &&Name
		    Image name. The n''th band of an image will have the name
		    'name:n'. If more than one image has the same name, they
		    will be numbered 'name #n' for some integer 'n'.

		    &&Zoom factors
		    Magnification and zoom. This field displays the ratio
		    between the pixel size and the dot size. A zoom factor of
		    4 indicates that an area of 4x4 dots on the screen
		    corresponds to one pixel.

		    &&Size or pos
		    Depending on the 'Log position' option (which is also
		    available from the 'Options' menu), this field will either
		    print the image size or it will log cursor position
		    (x, y, pixel value).

		&Message area
		The message area displays additional information.

Image windows:  The following actions may be invoked in the image window.

                &resize
		Images may be resized from window manager functions.
		If fixed aspect ratio is off, the window will get the size
		indicated by the window manager during the resize operation.
		If fixed aspect ratio is on, the window manager will not
		show the correct size during the resize operation. The
		final window size is determined by the following rule:
		If only the window width or the window height is changed,
		this will determine the new window size. If both window width
		and window height are changed, the new window size is
		determined by the new window width.

		&zoom
		| Ctrl  <Btn1>:       Zoom In
                | Ctrl  <Btn2>:       Zoom Normal
		| Ctrl  <Btn3>:       Zoom Out
		| Shift Ctrl  <Btn1>: Zoom 8 x In
                | Shift Ctrl  <Btn2>: Zoom = 32
		| Shift Ctrl  <Btn3>: Zoom 8 x Out

		'Zoom normal' means zoom factor equal to one, i.e. that one
		dot on the screen corresponds to one image pixel. This does
		not necessarily correspond to the zoom factor used initially
		for an image if 'xshow' was started with the option '-iw' or
		'-ih'. If the screen window is larger than the image size,
		'Zoom normal' means to use the smallest possible zoom factor.

		&pan 
		| <Key>Left:        Pan 4 pixels left
		| <Key>Right:       Pan 4 pixels right
		| <Key>Up:          Pan 4 pixels up
		| <Key>Down:        Pan 4 pixels down
		| Shift <Key>Left:  Pan 16 pixels left
		| Shift <Key>Right: Pan 16 pixels right
		| Shift <Key>Up:    Pan 16 pixels up
		| Shift <Key>Down:  Pan 16 pixels down
		| Ctrl <Key>Left:   Pan 256 pixels left
		| Ctrl <Key>Right:  Pan 256 pixels right
		| Ctrl <Key>Up:     Pan 256 pixels up
		| Ctrl <Key>Down:   Pan 256 pixels down

		&menus
		By default, there is a menubar above each image. This can be
		removed (see the section 'Options menu' below and the
		'ximage(3)' toolkit option '-mb'). Additionally, the menus can
		always be accessed with the following key combinations:

                    &&Shift <Btn1>
		    Colors menu (see the section 'Colortables' below). This
		    menu is available by default, but see also the 'ximage'
		    toolkit option '-colorsmenu'.

                    &&Shift Alt <Btn1>
		    OverlayColors menu (see the section 'Colortables' below).
		    This menu is available by default, but see also the
		    'ximage' toolkit option '-overlaysmenu'.

                    &&Shift <Btn2>
		    Tools menu (Built-in programs, see the sections 'Image
		    info', 'Histogram' and 'Slice' below). This menu is
		    available by default, but see also the 'ximage' toolkit
		    option '-toolsmenu'.

		    &&Shift Alt <Btn2>
		    Visuals menu (to display the image with a different
		    visual, see the description of the 'ximage' toolkit
		    option '-iv'). This menu is not available by default,
		    but see also the 'ximage' toolkit option '-visualsmenu'.

		    &&Shift <Btn3>
		    Options menu (see the section 'Options menu' below).
		    This menu is available by default, but see also the
		    'ximage' toolkit option '-optionsmenu'.
		  
		&Mouse-bindings when Mouse State = JC (Job control)
		    &&None <Btn1>
		    Drag region of interest.
		    &&None <Btn2>
		    Select band.
		    &&None <Btn3>
		    Select image.

		&Other key-bindings
		    &&q
		    Kill the image window
		    &&c
		    Increase current overlay draw color
		    &&p
		    Protect current overlay (toggle on/off)
		    &&g
		    Toggle overlay graphics on/off

		-&&
		-&The above operations are defined in terms of translation
		tables, which refer to action functions. These action
		functions are defined by the 'Image(3)' and 'ImageOverlay(3)'
		widgets. Refer to their descriptions for more information on
		the action functions. Examples of the use of action functions
		and translation tables can be found in the X11 application
		resources file for 'xshow'.

Environment:    'xshow' will search for the files xshow_colortabs and
                xshow_menues in the directories given by the environment
		variable XSHOWPATH. The colortable filenames listed in
		xshow_colortabs are also searched for in these directories.
		See the sections 'Colortables' and 'Program menus' as well
		as the options '-m' and the 'ximage' toolkit options '-cl',
		'-ct' and '-ovl'.

		Under the csh shell you may set this environment variable
		with the command
		| setenv XSHOWPATH \
		|   $XITE_HOME/data/xshow
		Under the Bourne shell sh, use the corresponding commands
		| XSHOWPATH=\
		|   $XITE_HOME/data/xshow
		| export XSHOWPATH

		The program started when pushing the 'help'-button is
		determined by the environment variable XITE_HELPER
		(with options determined by XITE_HELPER_OPTION).
		If these variables are not set, the program is determined
		by corresponding X11 resource settings. See the section
		'Control window' above.

		The online hypertext manual which is read for some settings
		of XITE_HELPER, is assumed to be located in
		$XITE_DOC/ReferenceManual/Contents.html.

Colortables:    'xshow' will search for a file 'xshow_colortabs' in the
                directories listed in XSHOWPATH. This may be overridden with
		the '-cl' option. The file should contain a list of
		colortable filenames. These files are expected to be found
		in the same directory as the file 'xshow_colortabs'.

		&Example
		| ! 
		| ! Sample colortables file.
		| ! Lines beginning with ;, ! or # are comment lines
		| ! Lines beginning with :S start standard colortabs
		| ! Lines beginning with :O start overlay colortabs
		| ! Lines beginning with @ read an include file
		| !
		| ! First read all standard colortables:
		| !
		| @$XSHOWPATH/xshow_colortabs
		| !
		| ! Add some private colortables
		| !
		| :Standard color tables
		| mywhite.col
		| myblack.col
		| !
		| ! Add an overlay table
		| !
		| :Overlay color tables
		| myoverlay.col
		|

		-&These colortables will be available from the image window
		Colors and OverlayColors popup menus and menubar.

		One of the entries in the Colors menu will be marked by an
		"x". This is the colortable in use for that particular image.

		One of the entries in the OverlayColors menu will also be
		marked by an "x". This is the colortable in use for the
		overlay of that particular image (most images don''t have
		overlays, but you can supply an overlay on the command-line
		as described in the section 'Arguments' above).

		See the section 'Options menu' below for more information
		on the relationship between the colortable for an image and
		the colortable for the overlay of that image.

Program menus:	'xshow' will search for files with name 'xshow_menues' in
                all the directories specified by XSHOWPATH. This may be
		overridden by using the '-m' option, in which case only
		the file given by this option will be read.

		All menu files consist of menu entries. If more than one
		file named 'xshow_menues' is found among the directories
		given by XSHOWPATH, all the entries are concatenated to
		form a larger menu hierarchy. Two conditions must be met
		for this to work.
		&1.
		One of the menu files must have a top-level menu
		with the name 'Xshow'. This file is called a master menu file.
		&2.
		The names of the top-level menu of each of the other menu files
		must exist as entries in the top-level menu of the master
		menu file.

		-&The menu file supplied with the XITE distribution has a
		top-level menu with the name 'Xshow'. Two of the other
		top-level menu-entries are 'Site-specific programs' and
		'My programs'. So, if you
		&1.
		have a file called 'xshow_menues' in your home directory
		&2.
		you call the top-level menu in this file 'My programs'
		&3.
		set the environment variable XSHOWPATH to include your home
		directory (in addition to the directory where 'xshow'''s own
		menu file is located)

		-&your menu hierarchy will pop up under the menu entry
		'My programs'.

		In the same way, a menu file specific to the local site
		or project can be created and included in the menu
		hierarchy.

		The menu file supplied with the XITE distribution can
		probably be found on your local system in the directory
		$XITE_HOME/data/xshow, where $XITE_HOME is the name of the
		directory into which XITE was installed.

		The structure of a menu file is as follows.
		| ! 
		| ! Sample menu file.
		| ! Lines beginning with ;, ! or # are comment lines
		| !
		| ! @ in first column reads an include file
		| ! : in first column creates a new menu
		| ! + in first column means that the entry is a new menu
		| ! - in first column creates a menu separator
		| ! ? in first column defines a dialog
		| !   (blank) in first column means a simple entry
		| !
		| ! ONE MENU IN ONE OF THE MENU FILES MUST HAVE THE NAME
		| ! "Xshow" (root menu) !!
		| :Xshow
		| +Arithmetic/logical
		| +Others
		| !
		| !  
		| :Arithmetic/logical
		| +Generate noisy images
		| -One image/band
		|  Negate          ; negate <infile> <outfile>
		|  Scale           ; scale <infile> <outfile> ?dialog:scale
		|  Absolute value  ; absValue <infile> <outfile>
		| -Two images/bands
		|  Absolute difference; absDiff <infile> <infile> <outfile>
		|  Signed difference ; signDiff <infile> <infile> <outfile>
		| !
		| ?scale
		|  scale the input image according to the formula \
		|  \
		|         output(x,y) = scale*input(x,y) + offset
		|  # scale:  # -scale  # f # 1.0 \
		|  # offset: # -offset # f # 0.0
		| !
		| ! This creates eight rows in the menu
		| ! "Arithmetic/logical":
		| !   - "Generate noisy images" (refers to a new menu)
		| !   - "One image/band" (menu separator, i.e.
		| !                       inactive entry)
		| !   - "Negate"
		| !   - "Scale"
		| !   - "Absolute value"
		| !   - "Two images/bands" (menu separator)
		| !   - "Absolute difference"
		| !   - "Signed difference"
		| !
		| ! Text before ; is the entry name.
		| ! Text after ; is the command line (ordinary Unix
		| ! commands).
		| ! Some special arguments:
		| !   <infile>       - Read input from mouse command.
		| !   <outfile>      - Send output to xshow (image or
		| !                    colortable).
		| !   <xterm>        - Send textual output to a separate
		| !                    text window. The command must be
		| !                    able to interpret "-1" for stdout.
		| !   ?writeBIFFfile - Standard dialog (ask for BIFF output
		| !                    filename).
		| !   ?readBIFFfile  - Standard dialog (ask for BIFF input
		| !                    filename).
		| !   ?dialog:dialogname - Fetch argument(s) from a dialog.
		| ! 
		| ! The dialog is defined over 3 lines, and the dialogname
		| ! is global across all the menu files in XSHOWPATH.
		| line 1: ?dialogname
		| line 2:  Info text for the program.
		| line 3:  Description of expected input arguments.
		| ! Refer to the standard menu file $XSHOWPATH/xshow_menues
		| ! and FormDialog(3) for more information, especially
		| ! on the third line of a menu dialog.
		| !
		| ! Lines may be continued if the last character on a
		| ! line is a backslash.
		| !
		| ! There are standard filename dialogs also for other
		| ! file types than BIFF files. See the section
		| ! 'Widget hierarchy' below.

Image info:	The 'Image info' entry under the 'Tools' menubar button, or the
                key/mouse combination 'Shift <Btn2>' + 'Image info' entry
		(while the mouse pointer is in the image window) will print
		some information about the image.

Histogram:	The 'Histogram' entry under the 'Tools' menubar button, or the
                key/mouse combination 'Shift <Btn2>' + 'Histogram' entry
		will create a histogram-window.

                Press 'Ctrl <Btn1>' in the histogram window to toggle between
		histogram and cumulative histogram. The header displays
		pixel value and histogram value in absolute value and 
		in percent of the total band.

		The LUT may be manipulated by invoking the action Piecewise
		linear, Threshold, Linear, Logarithmic or Exponential. The LUT
		may be treated as a grayscale or separate red, green and blue
		LUT by choosing one of the options in the Modes menu.

		&Piecewise linear
		Specify a piecewise linear polynomial.
		    &&<Btn1>
		    Insert new breakpoint
		    &&<Btn2>
		    Move breakpoint vertical
		    &&<Btn3>
		    Delete breakpoint

		&Threshold
		Set threshold.
		    &&<Btn1> horizontal
		    Set threshold

		&Linear
		Specify a linear transformation, p = ax + b.
		    &&<Btn1> vertical
		    Adjust a.
		    &&<Btn1> horizontal
		    Adbust b.

		&Logarithmic or Exponential
		<Btn1> horizontal - Adjust log/exp factor.

		&Reset
		Set initial values for the selected mode.

		&Histogram EQ
		Make a new histogram equalized image.

		&Send min/max
		Apply a linear transformation.

		&Send colortab
		Send colortable back to 'xshow' (add it to the list of
		available colortables) or to file.

		&Send image
		Transform the image according to the chosen
		LUT (for the selected mode only), and send image back to
		'xshow'.

Slice (color palette):
                The 'Slice' entry under the 'Tools' menubar button, or the
                key/mouse combination 'Shift <Btn2>' + 'Slice' entry will
		make a piecewise constant lookup table (LUT) of pseudocolors.
		Colors may be mixed in a palette by specifying rgb or ihs
		values.

		&Buttons:
		    &&Send colortable to xshow
		      Send LUT back to xshow
		    &&Send colortable to file
		      Save LUT in a file
		    &&Load original colortable
		      Load the colortable which the corresponding image used
		      when the slice application was started
		    &&Load colortable from file
		      Read LUT from file
		    &&Set patch in color range
		      Select a color index-range to fill with the color in the
		      palette

		-&&
                &Actions
		The following actions may be invoked in the colorfield in
                the bottom of the slice window.
		    &&<Btn1Down>
		      Fill LUT (and colorfield) at cursor position with the
		      palette color. This will most likely influence the image.
		    &&<Btn2Down>
		      Set the palette color equal to the LUT value at the
		      cursor position. This will influence the color palette.
		    &&<Btn3Down>
		      Replace a constant part of LUT values around the cursor
		      position with the palette color.
		    &&Drag <Btn>
		      Same as <Btn1Down>

Colorbar:       The 'Colorbar' entry under the 'Tools' menubar button, or the
                key/mouse combination 'Shift <Btn2>' + 'Colorbar' entry will
		display this image''s current colormap in a separate image
		window.

Options menu:   &Reduced colors
                For 8-plane pseudocolor displays, this switch toggles between
                128 and 256 colors. For 24-plane DirectColor displays, it
                toggles between approximately 16 million and 2 million
                colors.

		The decision of using 'Reduced colors' or not has two
		implications
		&&Technicolor effects
		    How the colors on the display outside the image appear
		&&Overlays possible
		    Whether one can use overlay images on top of other images
		-&& These two topics are treated in some more detail in the
		remaining description of this menu entry.

		With 'Reduced colors' set on an 8-plane pseudocolor display,
		all pixel values are transformed to the range 64-191, and
		this value is used to look up the color from the colortable.
		This means that from a 256-element colortable, only the entries
		with indices 64-191 are used.

		One advantage of the above transformation is that the rest of
		the display is more likely to keep its original colors, even
		when the mouse cursor is inside the image. At least the window
		manager colors and background colors are likely to stay
		unchanged. This is because the color entries for indices 0-63
		and 224-255 are copied from the default colortable of the
		display. Window managers, display background, cursor colors
		etc. are often chosen among these color entries. Displays with
		multiple hardware colormaps tend to avoid this problem and
		thus don''t benefit from this to the same degree. (Silicon
		Graphics is one vendor with such displays even on low-end
		workstations.)

		The other advantage of using a 'Reduced colors' scheme is that
		another image may be put on top of the first one. This other
		image may use a limited set of color-entries. This is exactly
		the case with overlays in 'xshow' (and in 'xregion'). The
		overlay uses the color entries with indices 192-223 (32
		colors).

		With the Colors menu, an image colortable is chosen. With the
		OverlayColors menu, a colortable for the overlay is chosen. Be
		aware, though, that the overlay colors are always installed in
		the color entries with indices 192-223 in the current
		colortable for the image. Since all images use the same list
		of colortables (in the Colors menu), no two images with
		overlays can use the same colortable and at the same time
		different overlay colortables. (Of course, one may work around
		this limitation by adding an extra copy of the image
		colortable and choose a different overlay colortable to be
		installed into this copy.)

		The default behavior is to use reduced colors.

		&Fixed aspect
		Toggle whether this image should maintain a fixed aspect ratio.

		&Menubar
		Toggle the appearance of a menubar for this image.

		&ROI fill
		If set, ROI (region of interest) will be inverted when you
		drag <Btn1>.

		&ROI permanent
		If set, display last ROI.

		&ROI square
		If set, force ROI to be a square area.

		&ROI zoom & pan
		If set, the ROI size and position will remain constant relative
		to the image when an image is zoomed and panned. Otherwise,
		the ROI will have a fixed screen size in a fixed screen
		position.

		&Interpret next as RGB
		If set, the next image created will be interpreted as a
		three-band RGB image.

		&Log position
		If set, display position and pixel value in the control window.
		Otherwise, display image size.

		&Zoom all
		If set, force all images to be zoomed with the same parameters.

Visuals menu:   Whether the entries in this menu are sensitive or not, depends
                on the visual capabilities of the display, which visual/depth
		is the default on this display, and the options '-multivisual'
		and '-iv'.

                &PseudoColor 8-plane
                Create a copy of this image which can be displayed on a
		PseudoColor visual with depth 8.

                &DirectColor 24-plane
                Create a copy of this image which can be displayed on a
		DirectColor visual with depth 24.

		&TrueColor 24-plane
                Create a copy of this image which can be displayed on a
		TrueColor visual with depth 24.

Widget hierarchy:
                The widget hierarchy is listed here, to ease the setting
		of X resources. See also the section 'Resources'.

		For the 'xshow' control window.

		&xshow (class XShow)
		| mainForm           (class Form)
		|   menuForm         (class Form)
		|     menuLabel      (class Label)
		|     action1Button  (class Toggle)
		|     action2Button  (class Command)
		|     action3Button  (class Command)
		|   infoForm         (class Form)
		|     infoLabel      (class Label)
		|     imagesLabel    (class Label)
		|     jobsButton     (class Command)
		|     mouseButton    (class Toggle)
		|   activeForm       (class Form)
		|     activeLabel    (class Label)
		|     imageNameLabel (class Label)
		|     zoomLabel      (class Label)
		|     dataLabel      (class Label)
		|   stderr           (class Text)

		-&For the menu window (popup child of Control window)

		&mainMenu                 (class TopLevelShell)
		|                          (':' in the  menu file)
		|   menuBox                (class MenuBox)
		|     menuTopLabel         (class Label)
		|     <Command entry name> (class MenuCommand)
		|     menuSepLabel         (class Label)
		|     <Submenu label>      (class SubMenuCommand)

		-&For non-permanent submenus (popup children of mainMenu)

		&<Submenu label>     (class OverrideShell)
		The children of the submenu are the same as those of the
		main menu.

		-&For permanent submenus (popup children of mainMenu)

		&<Submenu label>          (class TransientShell
		|   menuBox                (class MenuBox)
		|     menuTopLabel         (class Label)
		|     <Command entry name> (class MenuCommand)
		|     menuSepLabel         (class Label)

		-&For the input file-selector window (popup child of Control
		window or of Slice window).

		&fileSelectorShell    (Class TransientShell)
		| <readFileWidget>    (Class XfwfFileSelector)
		|   title             (Class Label)
		|   cur_dir_text      (Class Text)
		|   cur_file_text     (Class Text)
		|   path_list_title   (Class Label)
		|   path_list         (Class XfwfScrolledList)
		|     viewport        (Class Viewport)
		|	clip          (Class Core)
		|	vertical      (Class Scrollbar)
		|	multilist     (Class XfwfMultiList)
		|   file_list_title   (Class Label)
		|   file_list         (Class XfwfScrolledList)
		|     viewport        (Class Viewport)
		|	clip          (Class Core)
		|	vertical      (Class Scrollbar)
		|	multilist     (Class XfwfMultiList)
		|   goto_button       (Class Command)
		|   select_button     (Class Command)
		|   up_button         (Class Command)
		|   ok_button         (Class Command)
		|   cancel_button     (Class Command)

		<readFileWidget> above is one of
		|   readBIFFfile      (for BIFF images)        
		|   readColortabfile  (for BIFF colortables)   
		|   readColormapfile  (for ascii colormaps)    
		|   readTIFFfile      (for TIFF files)         
		|   readMATfile       (for MATLAB files)       
		|   readfile          (for any other file type)

		-&For the output file-selector window (popup child of Control
		window or of Slice window).

		&fileSelectorShell    (Class TransientShell)
		| <writeFileWidget>   (Class XfwfFileSelector)
		|   title             (Class Label)
		|   cur_dir_text      (Class Text)
		|   cur_file_text     (Class Text)
		|   path_list_title   (Class Label)
		|   path_list         (Class XfwfScrolledList)
		|     viewport        (Class Viewport)
		|	clip          (Class Core)
		|	vertical      (Class Scrollbar)
		|	multilist     (Class XfwfMultiList)
		|   file_list_title   (Class Label)
		|   file_list         (Class XfwfScrolledList)
		|     viewport        (Class Viewport)
		|	clip          (Class Core)
		|	vertical      (Class Scrollbar)
		|	multilist     (Class XfwfMultiList)
		|   goto_button       (Class Command)
		|   select_button     (Class Command)
		|   up_button         (Class Command)
		|   ok_button         (Class Command)
		|   cancel_button     (Class Command)

		<writeFileWidget> above is one of
		|   writeBIFFfile     (for BIFF images)        
		|   writeColortabfile (for BIFF colortables)   
		|   writeColormapfile (for ascii colormaps)    
		|   writeTIFFfile     (for TIFF files)         
		|   writeMATfile      (for MATLAB files)       
		|   writePSfile	      (for PostScript files)
		|   writeMacrofile    (for xshow macro files)
		|   writefile         (for any other file type)

                -&For the image window (popup child of Control window).

		&imageShell                (class TopLevelShell)
		| imageForm                (class ImageForm)
		|   image                  (class ImageOverlay)
		|   menubar                (class XfwfMenuBar)
		|     colors               (class XfwfPullDown)
		|     tools                (class XfwfPullDown)
		|	toolsmenu          (class SimpleMenu)
		|     options              (class XfwfPullDown)
		|	optionsmenu        (class SimpleMenu)

		-&For the colorsmenu (popup child of application shell).

		&colorsmenu      (class SimpleMenu)
		| menuLabel      (class SmeBSB)
		| backgroundcol  (class SmeBSB)
		| Work-map       (class SmeBSB)
		| colorsep       (class SmeLine)
		| White - 256    (class SmeBSB)
		| Black - 256    (class SmeBSB)
		| Red - 256      (class SmeBSB)
		| Green - 256    (class SmeBSB)
		| Blue - 256     (class SmeBSB)
		| Spectrum - 256 (class SmeBSB)
		| Hue 1 - 256    (class SmeBSB)
		| Rainbow - 256  (class SmeBSB)

		-&The toolsmenu, optionsmenu, overlaysmenu and visualsmenu
		have a hierarchy similar to the colorsmenu.

		-&For the histogram window (popup child of imageShell).

		&histogramShell               (class TopLevelShell)
		| histogramForm               (class Form)
		|   histogramLabel            (class Label)
		|   histogram                 (class Histogram)
		|   histogramMenuForm         (class Form)
		|     histogramModeForm       (class Form)
		|	histogramModeLabel    (class Label)
		|	histogramModeList     (class XfwfMultiList)
		|     histogramActionsForm    (class Form)
		|	histogramActionsLabel (class Label)
		|	histogramActionsList  (class XfwfMultiList)
		|   colormap                  (class Colormap)

		-&For the slice window (popup child of imageShell).

		&sliceShell                   (Class TopLevelShell)
		| sliceForm                   (Class Form)
		|   topLabel                  (Class Label)
		|   scrollbarIntensity        (Class Scrollbar)
		|   scrollbarHue              (Class Scrollbar)
		|   scrollbarSaturation       (Class Scrollbar)
		|   scrollbarRed              (Class Scrollbar)
		|   scrollbarGreen            (Class Scrollbar)
		|   scrollbarBlue             (Class Scrollbar)
		|   labelIntensity            (Class Label)
		|   labelHue                  (Class Label)
		|   labelSaturation           (Class Label)
		|   labelRed                  (Class Label)
		|   labelGreen                (Class Label)
		|   labelBlue                 (Class Label)
		|   colorPatch                (Class Label)
		|   labelIhs                  (Class Label)
		|   labelRgb                  (Class Label)
		|   sendColtab                (Class Command)
		|   range                     (Class Command)
		|   quit                      (Class Command)
		|   color                     (Class Label)
		|   colorIndex                (Class Label)
		|   colorStripe               (Class Image)

Resources:      'xshow' uses the 'ImageOverlay' widget (subclass of the
                'Image' widget). It understands all of the core X Toolkit
		and Athena resource names and classes as well.

		The 'xshow' application resources are defined by the 'ximage'
		toolkit. Refer to 'ximage(3)' for a description.

		For information about resources for the ImageOverlay widget,
		refer to 'ImageOverlay(3)' and 'Image(3)'. 

		The resources that may be specified for the various menus
		of class SimpleMenu above, are described in the documentation
		for the Athena SimpleMenu widget.

		You may find some of the resources for the file-selector widget
		particularly useful:

		&<widget class or instance>*currentDirectory:
		The directory path used.

		Substitute <widget class or instance> by
		XfwfFileSelector (class) or one of the instance
		names mentioned in the 'Widget hierarchy' section
		(such as readBIFFfile, writeTIFFfile).

		Examples
		| XShow*XfwfFileSelector*currentDirectory: .
		| XShow*XfwfFileSelector*currentDirectory: $HOME/img

		&<widget class or instance>*pattern:
		A pattern which the filenames, displayed in the	file list,
		must match.

		Example
		|  XShow*readBIFFfile*pattern: *.img

		&<widget class or instance>*checkExistence:
		Whether or not to demand existence of the specified file.

		Examples
		|  XShow*readColortabfile*checkExistence:  True
		|  XShow*writeMacrofile*checkExistence: False

		-&See the application defaults resource file for a list of
		more available resources, e.g. keyboard accelerators/shortcuts.
		This file is found in the directory
		$XITE_HOME/etc/app-defaults. The filename is XShow
		(capital ''S'').

		See also the above section 'Widget hierarchy'.

Files:		$XSHOWPATH/xshow_menues, $XSHOWPATH/xshow_colortabs,
                $XITE_HOME/etc/app-defaults/XShow.

See also:	'xite(1)', 'ximage(3)', 'mct(1)', 'xhistogram(3)',
                'xhistogram(1)', 'xslice(3)', 'xregion(1)', 'xadd(1)',
		'xfft(1)', 'xfilter(1)', 'xpyramid(1)', 'xmovie(1)',
		'Image(3)', 'ImageOverlay(3)', 'Histogram(3)', 'FormDialog(3)',
		'start_xshow(3)', 'fork_xshow(1)', 'XfwfButton(3)',
		'XfwfFileSelector(3)', 'XfwfFrame(3)', 'XfwfLabel(3)',
		'XfwfToggle(3)'

Diagnostics:	Please report bugs to blab@ifi.uio.no

Examples:
                Display an image with standard grayscale colortable
		|  xshow mona.img

		Display an image with standard grayscale colortable and
		an extra given colortable available from the Colors menu.
		|  xshow mona.img mona.col

		Display an image with a given colortable
		|  xshow mona.col mona.img

		Display two images, both with the same given colortable.
		|  xshow mona.col mona.img lena.img

		Display two images, with different given colortables
		|  xshow mona.col mona.img lena.col lena.img

		Display two images, mona.img with the colortable mona.col,
		lena.img with the colortable lena.col, and make green.col
		available from the Colors menu.
		|  xshow green.col mona.col mona.img lena.col lena.img
		|  xshow -ct green.col mona.col mona.img lena.col lena.img

		Display an image with a given overlay image.
                |  xshow mona.img +lena.img

		Display an image (mona.img) with a given colortable (mona.col)
		and a given overlay (lena.img) which has the given overlay
		colortable (black.ovl.col).
		and overlay colortable
		|  xshow -ovt black.ovl.col mona.col mona.img +lena.img

		Use xshow at the end of a pipe
		|  median mona.img - 5 | xshow -

		Display an rgb image in a single window
		|  xshow -rgb -iv DirectColor reine_rgb.img

Author:		Otto Milvang and Svein Bøe
Doc:            Otto Milvang and Svein Bøe
________________________________________________________________

*/

#include <xite/includes.h>
#include <ctype.h>
#include <xite/biff.h>
#include <X11/Xos.h>
#include <X11/Intrinsic.h>
#include <X11/StringDefs.h>
#include "xshow.h"
#include <signal.h>
#ifdef HAVE_STDIO_H
#  include <stdio.h>
#endif
#include <stdlib.h>
#ifdef HAVE_UNISTD_H
#  include <unistd.h>
#endif
#include <xite/color.h>
#include <xite/message.h>
#include <xite/readarg.h>
#include <xite/Xcolor.h>
#include <xite/ximage.h>
#include <xite/ImageVisualsMenu.h>
#include "xshowImageOptionsMenu.h"
#include <xite/XmenuBar.h>
#include "Xcontrol.h"
#include "Xdisplay.h"
#include "Xprog.h"
#include <xite/Xpty.h>
#ifdef HAVE_STRINGS_H
# include <strings.h>
#else
# ifdef HAVE_STRING_H
#  include <string.h>
# endif
#endif
#ifdef HAVE_SYS_FILE_H
# include <sys/file.h>
#else
# ifdef HAVE_SYS_IO_H
#  include <sys/io.h>
# endif
#endif
#include <xite/debug.h>

#define FALSE 0
#define TRUE 1

extern menu_button image_menus_e;
extern Widget colorsmenu_e, overlaysmenu_e, Control_e;
extern char *colortabfile_e, *ovltabfile_e, *colorlistfile_e;

extern Boolean dialog_defaults_e, dump_commands_e;

static XtAppContext appcon_s;



static void ReadImage2(XtPointer client_data, int *fid, XtInputId *id)
{
  int status, mess;

  ENTER_FUNCTION_DEBUG("xshow.c: ReadImage2");

  mess   = Iset_message(FALSE);
  status = Read_ipipe(*fid, NULL, NULL);
  Iset_message(mess);

  FPRINTF2("    Read_ipipe return: %d.\n", status);

  if (!status) {
    Message(1, "Input job finished - %s\n", "stdin");
    XtRemoveInput(*id);  
    close(*fid);
  }

  LEAVE_FUNCTION_DEBUG("xshow.c: ReadImage2");

} /* ReadImage2() */

static void Open_pipe(int pipe, int val)
{
  ENTER_FUNCTION_DEBUG("xshow.c: Open_pipe");

  if (!appcon_s) appcon_s = XtWidgetToApplicationContext(Control_e);
  XtAppAddInput(appcon_s, pipe, (XtPointer) XtInputReadMask, ReadImage2, NULL);

  LEAVE_FUNCTION_DEBUG("xshow.c: Open_pipe");

} /* Open_pipe() */

static void Main_loop(void)
{
  ENTER_FUNCTION_DEBUG("xshow.c: Main_loop");

  if (!appcon_s) appcon_s = XtWidgetToApplicationContext(Control_e);
  XtAppMainLoop(appcon_s);

  LEAVE_FUNCTION_DEBUG("xshow.c: Main_loop");
} /* Main_loop() */



/*L*

________________________________________________________________

		Read_ipipe
________________________________________________________________

Name:		Read_ipipe
Syntax:		| #include <xite/xshow.h>
                |
                | int Read_ipipe(int ipipe, char *name);
		|
Description:    Read image from ipipe. Use preferred visual class and depth.
Return value:
Author:		Otto Milvang
________________________________________________________________

*/

int Read_ipipe(int ipipe, char *name, program *prog)
{
  IMAGE img;
  Color_tab ctab;
  int overlay, return_value = 0;
  char pipenr[20], *cname;
  int len;
  IPIXTYP pt;

  ENTER_FUNCTION_DEBUG("xshow.c: Read_ipipe");

  if (prog && prog->outimages == 0) {
    /* No more images expected. */
    LEAVE_FUNCTION_DEBUG("xshow.c: Read_ipipe");
    return(return_value);
  }

  overlay = 0;

  if (name == NULL) { 
    sprintf(pipenr,"%c%d", '-', ipipe);
    name = pipenr;
  } else if (name[0] == '+') {
    name++;
    overlay = 1;
  }
  FPRINTF2("    name/pipenr: %s\n", name);
  
  if (overlay && (strlen(name) == 0)) {
    img = NULL;
    pt  = 0;
  }
  else {
    img = Iread_image(name);
    if (prog) prog->outimages--;
    if (img == 0) {
      return(return_value);
    }
    pt = Ipixtyp(img[1]);
  }

  if (img)
    FPRINTF3("    Ipixtyp : %ld %ld\n", ((_Iband_info *) img[1][0])->pixtyp,
	     Ipixtyp(img[1]));
  
  switch ((int) pt / 256) {
  case 0: /* Image or overlay image, not colortable. */
    if (overlay) {
      Add_overlay(img);
      return_value = 3;
    }
    else {
      int nbands;

      nbands = Display_picture(img, NULL, 0);
      return_value = 4 + nbands - 1;
    }
    break; 
  case 1: /* Colortable or overlay colortable. */
    FPRINTF1("    color table.\n");
    color_convertfromimg(img, ctab, &cname, &len);
    if (overlay) {
      if (!overlaysmenu_e) {
	InitImageMenus(colorlistfile_e, colortabfile_e, ovltabfile_e, NULL,
		       NULL, OVERLAYS_BUTTON, NULL);
      }
      Color_ovl_add((XColor *) ctab, cname, len);
      return_value = 2;
      set_init_ovl_color(Color_ovl_max());
    }
    else {
      if (!colorsmenu_e) {
	InitImageMenus(colorlistfile_e, colortabfile_e, ovltabfile_e, NULL,
		       NULL, COLORS_BUTTON, NULL);
      }
      Color_add((XColor *) ctab, cname, len);
      return_value = 1;
      set_init_color(Color_max());
    }
    
    break;
  }
  
  LEAVE_FUNCTION_DEBUG("xshow.c: Read_ipipe");

  return(return_value);

}  /* Read_ipipe() */



#ifdef MAIN

extern Display *dpy_e;
extern Widget  Control_e;
extern Widget  optionsmenu_e;

char *menufilename_e;

/* Local external variables. */

static int ipipe_s  = -1;
/* static int pipeopen_s; */

/*L*

________________________________________________________________

		Handler
________________________________________________________________

Name:		Handler, Setup_handler
Syntax:		void Handler(int signl)
                void Setup_handler(void)
Description:    Handler prints a message if the program is
                interrupted.  
Author:		Otto Milvang
________________________________________________________________

*/



static void Handler(int signl)
{
  PopMessage();
  Message(2, "xshow aborted ...\n");
  XptyKill(NULL);
  exit(2);
}


static void Setup_handler(void)
{
  ENTER_FUNCTION_DEBUG("xshow.c: Setup_handler");
  signal(SIGHUP,Handler);
  signal(SIGINT,Handler);
  signal(SIGQUIT,Handler);
  signal(SIGCLD, SIG_IGN);
  LEAVE_FUNCTION_DEBUG("xshow.c: Setup_handler");
}



/*L*

________________________________________________________________

		Read_args
________________________________________________________________

Name:		Read_args
Syntax:		void read_args(argc, argv)
                int argc,
                char *argv[];
Description:    Read images described on command line. Use preferred
                visual class and depth.
Externals:  	int ipipe_s;
Author:		Otto Milvang
________________________________________________________________

*/

static void Read_args(int argc, char **argv)
{
  int inp, arg, status = 0;
  char *arga;

  ENTER_FUNCTION_DEBUG("xshow.c: Read_args");

  arg = 1;
  while (argc > arg || ipipe_s != -1) {
    arga = argv[arg];
    FPRINTF2("    argc=%d\n", argc);

    if (ipipe_s != -1 ||
	(*arga == '-' && ((arga[1] == 0) || isdigit(arga[1])))) {
      /* Read from pipe ('-', '-0', '-1', '-2' etc). */

      if (ipipe_s != -1 ) {
	inp     = ipipe_s;
	ipipe_s = -1;
      }
      else inp = -atoi(arga);

      FPRINTF2("    Read from pipe %d.\n", inp);

      Open_pipe(inp, -1);
      if (inp) Message_jobs(1);
      status = Read_ipipe(inp, NULL, NULL);
      FPRINTF2("    Read_ipipe return: %d.\n", status);
      if (!status) {
	Error(2, "Pipe %d is not in BIFF-format.\n", inp);
	/* Close_pipe(inp); */
      }
    } else {
      FPRINTF2("    Read from file %s.\n", arga);
      if (! Read_ipipe(0, arga, NULL)) {
	if (access(arga, 0) == -1 ) {
	  Error(2, "%s not found.\n", arga);
	} else if (access(arga, 4) == -1) {
	  Error(2, "%s is read protected.\n", arga);
	} else {
	  Error(2,"%s is not a BIFF-file.\n", arga);
	}
      }
    }
    arg++;
  }
  LEAVE_FUNCTION_DEBUG("xshow.c: Read_args");
}



int main(int argc, char **argv)
{
  int i;

  ENTER_FUNCTION_DEBUG("xshow.c: main");

#ifdef DEBUG
  for (i=0; i<argc; i++) FPRINTF3("    arg %d = |%s| \n", i, argv[i]);
#endif /*  */

  Iset_message(TRUE);
  InitMessage(&argc, argv, ximage_app_usage_text(
    "Usage: %s [<option>...] [<BIFF-file>] [+] [+<BIFF-file>...]\n\
       where <option> can be \n\
         -dialog_defaults : Automatically accept dialog defaults.\n\
         -i <chan>        : Use input channel <chan>.\n\
         -m <filename>    : This file contains the menu layout.\n\
                            Don't read any other menu files.\n\
    \n\
       <BIFF-file>:\n\
         - Image files in BIFF format\n\
         - Colortable files in BIFF format\n\
       Leading '+' indicates overlay image or overlay colortable\n\
       Separate '+' indicates empty overlay\n"));

  Setup_handler();

  Control_e = ximage_init(&argc, argv, "XShow",
			  NULL, True);   /* fallback, warn */

  /* Additional options connected to application resources, are treated by
   * XtGetApplicationResources.
   */
  menufilename_e    = read_switch(&argc, argv, "-m", TRUE, NULL);
  dump_commands_e   = read_bswitch(&argc, argv, "-dump_commands");
  dialog_defaults_e = read_bswitch(&argc, argv, "-dialog_defaults");
  if (dump_commands_e) dialog_defaults_e = True;


  PushMessage(MessageToWidget, MessageToWidget, MessageToWidget, 0);  
  Init_control();

  if (menufilename_e != NULL)
    /* Do not read standard menu files. */
    Init_prog(menufilename_e, FALSE);
  else
    /* Also read standard menu files. */
    Init_prog(menufilename_e, TRUE);

  /* Prepare for an AsciiText widget to receive messages. */
  XptyInit(dpy_e, DefaultScreen(dpy_e), Control_e);

  ipipe_s = read_iswitch(&argc, argv, "-i", -1);

  Read_args(argc, argv);

/*  pipeopen_s = ! isatty(ipipe_s);
  if (pipeopen_s) Open_pipe(ipipe_s, -1);
*/

  i = 1;
  while (i < argc) {
    /* Check whether any remaining argument is an option (and not <stdin> or
       a channel number). */
    if (argv[i][0] == '-' &&
	(! ((argv[i][1] == 0) || isdigit(argv[i][1])) )
	)
      Usage(2, "Unknown argument: %s.\n", argv[i]);
    i++;
  }

  Main_loop();

  LEAVE_FUNCTION_DEBUG("xshow.c: main");

  return(0);
}       

#endif /* MAIN */
