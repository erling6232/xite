

/*C*

________________________________________________________________

        ximage
        $Id$
        Copyright 1994, Blab, UiO
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

/*F:ximage_init=ximage*/
/*F:ximageInit=ximage*/
/*F:ximage_layout=ximage*/
/*F:ximageLayout=ximage*/
/*F:ximage_histogram=ximage*/
/*F:ximage_slice=ximage*/
/*F:ximage_info=ximage*/
/*F:ximage_colorbar=ximage*/
/*F:ximage_mainloop=ximage*/
/*F:ximage_exit=ximage*/ 
/*F:ximage_display=ximage*/
/*F:ximage_app_resources=ximage*/
/*F:ximage_app_options=ximage*/
/*F:ximage_usage_text=ximage*/
/*F:ximage_app_usage_text=ximage*/
/*F:set_init_color=ximage*/
/*F:set_init_ovl_color=ximage*/
/*F:LogPosition=ximage*/
/*F:LogPositionCallback=ximage*/
/*F:Make_label_layout=ximage*/
/*F:Make_command_layout=ximage*/
/*F:Make_prompt_layout=ximage*/
/*F:Make_scroll_layout=ximage*/
/*F:Make_menu_layout=ximage*/
/*F:Make_entry_layout=ximage*/
/*F:Add_entry_layout=ximage*/
/*F:Make_list_layout=ximage*/
/*F:Make_image_layout=ximage*/
/*F:add_callback_to_image_layout=ximage*/
/*F:init_image_callbacks=ximage*/
/*F:add_to_image_callbacks=ximage*/
/*F:add_callback_first=ximage*/
/*F:add_callback_last=ximage*/
/*F:ximage*

________________________________________________________________

		ximage
________________________________________________________________

Name:		ximage, ximage_init, ximageInit, ximage_layout, ximageLayout,
                ximage_histogram, ximage_slice, ximage_info, ximage_colorbar,
		ximage_mainloop, ximage_exit, ximage_display,
		ximage_app_resources, ximage_app_options, ximage_usage_text,
		ximage_app_usage_text, set_init_color, set_init_ovl_color,
                LogPosition, LogPositionCallback, Make_label_layout,
                Make_command_layout, Make_prompt_layout, Make_scroll_layout,
                Make_menu_layout, Make_entry_layout, Add_entry_layout,
                Make_list_layout, Make_image_layout, add_callback_to_image,
                init_image_callbacks, add_to_image_callbacks,
		add_callback_to_image_layout, add_callback_first,
		add_callback_last - toolkit for creating X11 applications
		in XITE

Syntax:         | #include <xite/ximage.h>
		|
                | Widget ximage_init( int* argc, char** argv,
                |    char* classname,
                |    String fallback_resources[], int warn );
		|
                | Widget ximageInit( int* argc, char** argv,
                |    char* classname, XtResource* resources,
                |    int num_resources, XtPointer resources_result,
                |    XrmOptionDescRec* optionsTable,
                |    int num_options,
                |    String fallback_resources[], int warn );
                |
                | Widget ximage_layout( Widget app_parent,
                |    IBAND band, char** wids, Widget rwids[],
                |    int nwids, XtCallbackProc ximage_work_fun,
                |    char* res_inst_name );
		|
                | Widget ximageLayout( Widget app_parent,
                |    layout *layouts, int nwids );
		|
		| char *ximage_app_usage_text( char *app_usage );
		| 
		| char *ximage_usage_text( void );
		|
		| int ximage_histogram( Widget wid,
		|    image_disp_cb_data *cb_data );
		|
                | int ximage_slice( Widget wid );
		|
                | int ximage_info( Widget wid );
		|
		| int ximage_colorbar( Widget wid,
		|    image_disp_cb_data *cb_data );
		|
                | void ximage_mainloop( void  );
		|
                | void ximage_exit( Widget wid, XtPointer str,
                |    XtPointer dummy );
		|
                | Widget ximage_display( Widget parent,
                |    char* resource_name, char *shell_resource_name,
		|    IMAGE img, int bandno, WidgetClass widgetClass,
                |    Visual* visual, int depth,
                |    menu_button buttons, ArgList args,
                |    Cardinal num_args, image_callbacks *callbacks );
                |
                | void set_init_color( int color );
                |
                | void set_init_ovl_color( int color );
                |
		| void LogPosition( Widget image_wid, Widget label_wid );
		|
		| void LogPositionCallback( Widget wid,
		|    XtPointer client_data, XtPointer call_data );
		|
		| layout_label Make_label_layout( layout_label lay_out,
		|    char *type, char *resource_name,
		|    char *shell_resource_name, Widget wid, int id );
		|
		| layout_command Make_command_layout(
		|    layout_command lay_out, char *type,
		|    char *resource_name, char *shell_resource_name,
		|    Widget wid, int id, XtCallbackProc callback,
		|    XtPointer closure );
		|
		| layout_prompt Make_prompt_layout( layout_prompt lay_out,
		|    char *type, char *resource_name,
		|    char *shell_resource_name, Widget wid, int id,
		|    XtCallbackProc callback, XtPointer closure,
		|    char *text );
		|
		| layout_scroll Make_scroll_layout(
		|    layout_scroll lay_out, char *type,
		|    char *resource_name, char *shell_resource_name,
		|    Widget wid, int id, XtCallbackProc jump_callback,
		|    XtPointer jump_closure,
		|    XtCallbackProc scroll_callback,
		|    XtPointer scroll_closure );
		|
		| layout_menu Make_menu_layout( layout_menu lay_out,
		|    char *type, char *resource_name,
		|    char *shell_resource_name, Widget wid, int id,
		|    int num_entries, layout_menu_entry *menu_entries );
		|
		| layout_menu_entry Make_entry_layout(
		|    layout_menu_entry lay_out, char *resource_name,
		|    char *label, Widget wid, XtCallbackProc callback,
		|    XtPointer closure );
		|
		| layout_menu_entry *Add_entry_layout(
		|    layout_menu lay_out, char *resource_name,
		|    char *label, Widget wid, XtCallbackProc callback,
		|    XtPointer closure );
		|
		| layout_list Make_list_layout( layout_list lay_out,
		|    char *type, char *resource_name,
		|    char *shell_resource_name, Widget wid, int id,
		|    int num_entries, char **list_entries,
		|    XtCallbackProc callback, XtPointer closure );
		|
		| layout_image Make_image_layout( layout_image lay_out,
		|    char *type, char *resource_name,
		|    char *shell_resource_name, Widget wid, int id,
		|    IMAGE img, image_callbacks *callbacks );
		|
		| void add_callback_to_image_layout( layout_image lay_out,
		|    String resource_name, XtCallbackProc callback,
		|    XtPointer closure, Boolean first );
		|
		| image_callbacks *init_image_callbacks( void );
		|
		| image_callbacks *add_to_image_callbacks(
		|    image_callbacks *image_cbs,
		|    String resource_name_list,
		|    XtCallbackProc callback, XtPointer closure,
		|    Boolean first);
		|
		| XtCallbackList add_callback_first(
		|    XtCallbackList callback_list,
		|    XtCallbackProc new_callback, XtPointer closure );
		|
		| XtCallbackList add_callback_last(
		|    XtCallbackList callback_list,
		|    XtCallbackProc new_callback, XtPointer closure );
		|
		| XtResource *ximage_app_resources(
		|    XtResource *resources,
		|    int *num_resources );
		|
		| XrmOptionDescRec *ximage_app_options(
		|    XrmOptionDescRec *options, int *num_options );
		|

Overview:	'ximage' is a toolkit for design of X11-applications in XITE,
                using the 'Image' and 'ImageOverlay' widgets as well as some
		Athena and Free Widget Foundation (FWF) widgets. This gives
		the programs a common look and feel.

		Prior to XITE version 3.3 the two main ximage functions were
		'ximage_init' and 'ximage_layout'. In version 3.3 each of them
		got a sibling, 'ximageInit' and 'ximageLayout' respectively.
		The former two provide the simplest and easiest programming
		interface. The latter two provide greater flexibility and more
		control at the cost of a somewhat more complex programming
		interface.

		The old functions are described before the new ones below.

		See the section 'Compatibility' below for information on
		how you can increase the functionality of your existing
		'ximage' toolkit based applications just by recompiling and
		relinking them.

Compatibility:  This section describes what changes must be made to function
                calls 'ximage_init' and 'ximage_layout' for old 'ximage'
                based applications.

                &ximage_init
                If your application is based on XITE prior to version 3_01,
		add a 'warn' argument with value zero to the function call.

		Refer to the description of 'ximage_init' below for more
		information.

                &ximage_layout
                If your application is based on XITE prior to version 3.0, add
                the callback function 'ximage_work_fun' (or whatever you
                called the function which responds to events in your
                application) to the list of arguments in the call to
                'ximage_layout'.

		If your application is based on XITE prior to version 3.2,
		also add a NULL 'res_inst_name' argument at the end of the
		argument list in the call to 'ximage_layout'. If you have
		multiple calls to 'ximage_layout' in your application, then
		use a non-NULL 'res_inst_name' argument in every call where
		you have NULL for the 'parent' argument, and replace the NULL
		'parent' argument by the return value from 'ximage_init'.
		A non-NULL 'res_inst_name' argument will lead to the creation
		of a new window with shell resource instance name equal to
		'res_inst_name'.

		Refer to the description of 'ximage_layout' below for more
		information.

		-&

		When you run your recompiled application it should look the
		same as before. However, there may be some more menus or menu
		entries available when you press one of the mouse buttons
		along with the Shift key while the mouse pointer is inside the
		image. Some of these may be familiar from the 'xshow' XITE
		application. One of the useful entries available from the
		Options menu is to log the mouse pointer position.

		Try the option '-layout' (see below) with your recompiled
		application. This should give you a separate window for each
		image. The images can now be resized with the mouse, just like
		what has been possible in 'xshow'.

		You may also want to try the options '-panelcontainer' and
		'-adjustwidth' along with '-layout' to get the desired result.
		The '-widgetset' option can be used to choose between Athena
		(Xaw) and Free Widget Foundation (Xfwf) widgets if they are
		created with 'ximage_layout' or 'ximageLayout'.

		If you get dangling windows containing only a label or single
		buttons and you want to collect them into a larger control
		panel window, refer to the description of 'ximage_layout'
		below for more information.

ximage_init:    'ximage_init' initializes the 'ximage' toolkit (the X11 toolkit
                is also initialized) and assigns 'classname' as the name of
		the application class. The X11 resource file ("app-defaults"
		file) has the same name as the application class.

		&fallback_resources
		'ximage_init' can handle fallback-resources. All XITE
		applications should however use NULL as the value for the
		'fallback_resources' argument. They generally don''t expect to
		find "app-default" files in a standard directory, such as in
		/usr/lib/X11/app-defaults, and we don''t want the fallback
		resources to be used in this event. Resources will be
		specified in files which are stored locally under the XITE
		home-directory, and the XITE applications are able to find
		these files. The 'fallback_resources' argument is provided for
		those who write their own applications which are not included
		in the XITE distribution.

		&Resource and software version correspondence
		Set 'warn' equal to zero unless the application is included in
		the XITE distribution. If 'warn' is non-zero, 'ximage_init'
		checks that the X resource xiteResourceVersion (class
		XiteResourceVersion) is set equal to the XITE version. If not
		equal, a warning is printed. The xiteResourceVersion resource
		is set in all the supplied XITE application resource files.
		This is to ensure correspondence between software version
		and resource file version.
	
		&Window deletion
		'ximage_init' prepares for acceptance of window deletion by
		the window manager. See comment below under 'ximage_layout'.

		&Visuals
		'ximage_init' takes care of initializing the necessary
		information concerning the preferred/requested visual type,
		any other available visual types which the display and XITE
		can support and corresponding XITE-initial colormaps. See also
		the description of the "-iv" option and the "imageVisualClass"
		application resource. For more information, refer to
		'Visual(3)'.

		"reduced-color" mode is chosen as default for display of
		images. This can be overridden with the Image and ImageOverlay
		widget resources XtNdisplayMethod which can be set with the
		option '-full'. See the 'Options' section below.

		&Some options
		'ximage_init' can initialize popup (and menubar pulldown) menus
		for the Image and ImageOverlay widgets, depending on some
		application resources and options. The default behaviour is to
		initialize the Colors, OverlayColors, Tools, Options and
		Visuals menu (but the Visuals menu will not be able to
		do any work unless the '-multivisual' option is used). The
		OverlayColors menu will only be available, however, from
		ImageOverlay widgets. Refer to the Options and Resources
		sections below for more information on how to control the
		presence of these menus.

		&Action functions
		'ximage_init' registers three action functions with the
		application: "PopupMenu", "PopupExit" and "CloseShell". Refer
		to the 'Actions' section below for a description of these
		functions.

		A few other action functions are also registered, but they are
		static (not globally available), and you don''t really need to
		know about them. Their tasks are to prepare the popup menus
		every time they are popped up.

ximageInit:	'ximageInit' is a new version of 'ximage_init' (since XITE
                version 3.3) with more flexibility. 'ximageInit' is actually
		called by 'ximage_init'.

		&resources and options
		An 'ximage' toolkit base application will recognize a
		default set of application resources and command-line options
		automatically.

		Specification of additional application resources can be
		supplied through the 'resources' argument. The
		'resource_result' argument must be a reference to a structure
		for storage of the resulting additional resource values. The
		application must interpret these resource values itself. Also,
		the 'optionsTable' argument should be the concatenation of two
		options tables: One options table for the standard 'ximage'
		options and a table for additional options corresponding to
		new resources. Use the function 'ximage_app_options' to append
		your own options table to the standard 'ximage' options table.

		Refer to the sections 'Options' and 'Resources' below for
		explanation of the standard 'ximage' options and resources.

ximage_app_resource:
                Append 'resources' to the standard 'ximage' toolkit resources.
		Return the total number of resources in	'num_resources'.

ximage_app_options:
                Append 'options' to the standard 'ximage' toolkit options.
                Return the total number of options in 'num_options'.

ximage_usage_text:
                'ximage_usage_text' returns the standard usage text for the
		'ximage' toolkit.

ximage_app_usage_text:
                'ximage_app_usage_text' returns the concatenation of
		'app_usage', the standard usage text for the 'ximage' toolkit
		(refer to 'ximage_usage_text') and the standard usage text for
		XITE (refer to 'xite_standard_options_usage_text(3)').

		Use this function in a call to 'InitMessage(3)' early in the
		'main' function of an application which is based on 'ximage'.

ximage_layout:  'ximage_layout' creates 'nwids' widgets and returns a pointer
		to each through the 'rwids' array. For a more flexible
		method, also refer to the description of 'ximageLayout' below.

		&app_parent
		This argument must not be NULL. It is typically the return
		value from ximageInit.

		&ximage_work_fun
		This is the callback function which handles how
		the widgets will react to events. It is added to callback
		lists for some of the widget types specified below. Refer to
		their individual description for details. The function must be
		supplied by the application which uses the 'ximage' toolkit.
		Its prototype must be
		| void ximage_work_fun(Widget wid,
		|    XtPointer client_data,
		|    XtPointer call_data);

		Unless otherwise stated, the 'client_data' argument to the
		callback function will be a pointer to an integer which
		contains the widget number (the index into the 'wids' array
		for each particular widget, between 0 and 'nwids'-1
		inclusive). The callback function can test this number to
		determine which widget the event occured in.
		See the section 'See also' below for references to
		program examples on how to handle this.

		The 'call_data' argument of the callback function is a
		reference to a chunk with various information from the widget
		referenced by 'wid' and 'client_data'. This information is
		collected in a struct of type 'ImageCallbackRec', defined by
		the 'Image' widget. The definition is as follows
		| typedef struct 
		| {
		|   int reason;
		|   XEvent *event;
		|   struct imagestruct *img;
		|   struct bandstruct *band;
		|   int x, y;
		|   ImageData data;
		|   pixel *pixtype;
		|   float zoom, magnification;
		|   int xpan, ypan;
		| } ImageCallbackRec, *ImageCallback; 

		'reason' is one of two predefined integral constants:
		ACTIVATE or RESIZE.

		'x' and 'y' are the image coordinates where the event took
		place, if the event was one of ButtonPress, ButtonRelease,
		MotionNotify or Expose. In case of a different event type,
		'x' and 'y' refer to present cursor position.

		'data' is a reference to the image pixel at position
		('x','y'). 

		&wids
		'wids' is an array of pointers to strings. Each string is a
		specification for a widget. Each widget specification is
		divided into one or more fields, separated by ":". The first
		field always determines the widget class, the second field
		determines the widget instance resource name (for use in X11
		resource specifications). The simplest widget specification
		is

		| <widget_class>:<r_nam>

		The widgets will be created in the order they are specified
		in the 'wids' array.

		If a third field is present in a widget specification, it
		determines whether a widget should be displayed inside the
		same window as the previous widget or be the first one in a
		new window. A new window will by default be launched if the
		third field is non-empty, like this

		| <widget_class>:<r_nam>:<sh_r_nam>

		'sh_r_nam' will be the resource instance name of the new
		shell widget. Whether or not a new window will be created can
		also be controlled via the option '-layout' and the application
		resource 'layout'.

		The 'widget_class' field can be chosen among

		&&Ascii_page
		Class Text. From the Athena widget set. The widget size will
		be set to hold 80 by 24 characters.

		&&Ascii
		Class Text. From the Athena widget set. Like "Ascii_page", but
		without the fixed size. "Ascii" should be used for
		small Text widgets, such as for small input fields. See the
		"Prompt" entry below for a better alternative.

		&&Command
		Class Command (from the Athena widget set) or XfwfButton (from
		the Xfwf widget set). The widget label will be set equal to
		'r_nam'.

		'ximage_work_fun' will be installed as the XtNcallback (or
		XtNactivate for XfwfButton) callback function, with the widget
		number (the index into the 'wids' array for this widget)
		supplied through the 'client_data' argument.

		Choose between Athena and Xfwf widgets with the option
		'-widgetset' or corresponding application resource.

		&&Image
		Class Image. Defined in XITE. In the absence of
		'sh_r_nam' a new window will as usual not be created for
		an Image widget, i.e. the Image widget will become a child of
		a Box child of 'app_parent'. In this case the Image may be
		inside the same window as buttons and other widgets. It can
		not be resized with the mouse, and no menubar will be available
		above the image. With 'sh_r_nam' present in the widget
		specification, a separate window will be made for the image.
		This is the recommended usage.

		As usual a separate window can be created for the image in
		the absence of the 'sh_r_nam' specification if the option
		'-layout' (or the application resource 'layout') is used.
		In this case, the shell resource name will by default be
		set equal to "imageShell".

		The resource instance name for an Image widget is set to
		"image", regardless of the 'r_nam' string.

                'band' must supply the image data. By default, the display of
                the image will be in "reduced-color" mode. This can be
                overridden with the option '-full'.

		'ximage_work_fun' is installed as the XtNcallback callback
		function for the widget with the widget number (the index into
		the 'wids' array for this widget) supplied through the
		'client_data' argument. 'LogPositionCallback' is also added
		to the XtNcallback callback list, in front of
		'ximage_work_fun'. See the description of 'LogPositionCallback'
		below.

		Additional callback functions may be installed after the call
		to 'ximage_layout' by calling XtAddCallback or one of the
		functions 'add_to_image_callbacks', 'add_callback_first' or
		'add_callback_last'. They are described below.

		An Image widget is actually created by the function
		'ximage_display', which is described below.

		&&Label
		Class Label (from the Athena widget set) or XfwfLabel (from
		the Xfwf widget set). The widget label will equal 'r_nam'.

		Choose between Athena and Xfwf widgets with the option
		'-widgetset' or corresponding application resource.

		&&List
		Class List. From the Athena widget set. The widget
		specification should be on the following form

		| List:r_nam:sh_r_nam:lab1:lab2

		where 'r_nam' is the resource instance name of the widget
		as usual and 'sh_r_nam' is only required if the list
		widget is the first one in a new window. 'lab1' will be the
		label of the first list element, 'lab2' the label of the
		second element etc.

		If a label must contain the character ":", escape it with
		a backslash, like this

		| List:r_nam:sh_r_nam:lab1:lab\\:2

		If a label must contain a backslash, use the following
		kind of specification

		| List:r_nam:sh_r_nam:lab1:l\\ab2

		'ximage_work_fun' will be installed as the XtNcallback
		callback function for the widget with the widget number (the
		index into the 'wids' array for this widget) supplied through
		the 'client_data' argument.

		&&Menu
		Class menuButtonWidget. From the Athena widget set. The
		corresponding string in 'wids' should be on the following
		form

		| Menu:r_nam:sh_r_nam:nm1:lb1:int1:nm2:lb2:int2

		where 'r_nam' is the resource instance name of the widget
		as usual, 'sh_r_nam' is only required if the menubutton
		widget is the first one in a new window, 'nm1' is the
		resource instance name of the first menuentry button, 'lb1'
		is the visible text in the first menuentry button, and 'int1'
		is the client_data returned to the menuentry button callback
		function when this entry is selected (actually a pointer to
		the integer, cast to XtPointer, is returned). 'int1', 'int2',
		..., must be integers.
		
		If a label must contain the character ":", escape it with
		a backslash, like this

		| Menu:r_nam:sh_r_nam:nm1:l\\:b1:int1

		If a label must contain a backslash, use the following
		kind of specification

		| Menu:r_nam:sh_r_nam:nm1:l\\b1:int1

		'ximage_work_fun' will be installed as the XtNcallback
		callback function for the menuentry buttons. It is important
		that 'int1', 'int2' etc. are not in the range zero to
		'nwids'-1, because 'ximage_work_fun' will typically test its
		client_data arument to determine from which widget it was
		called. Integers in the range zero to 'nwids'-1 are probably
		already used as client_data by some of the widgets specified
		in 'wids'.

		&&Overlay
		Class ImageOverlay. Defined in XITE. See the description of
		"Image" above.

		&&Prompt
		Class XfwfPrompt. From the Free Widget Foundation. It is
		suitable to use as an input field. It is recommended to use
		this widget only for the input field itself. Any leading text
		(telling the user what kind of input is expected) should be
		supplied in a Label widget to the left of the Prompt widget.

		The widget specification should be on the following form

		| Prompt:r_nam:sh_r_nam:text

		where 'r_nam' and 'sh_r_nam' have the usual meaning
		and 'text' will be shown initially in the Prompt widget.

		'ximage_work_fun' will be installed as the XtNactivate
		callback function for the widget with the widget number (the
		index into the 'wids' array for this widget) supplied through
		the 'client_data' argument.

		&&Scroll
		Class Scrollbar. From the Athena widget set.

		'ximage_work_fun' will be installed as the XtNjumpProc
		callback function for the widget with the widget number (the
		index into the 'wids' array for this widget) supplied through
		the 'client_data' argument. No callback function is supplied
		for the XtNscrollProc callback list.

		&&Toggle
		Class Toggle (from the Athena widget set) or XfwfToggle (form
		the Xfwf widget set). The widget label will equal 'r_nam'.

		'ximage_work_fun' will be installed as the XtNcallback (or
		XtNonCallback and XtNoffcallback for XfwfToggle) callback
		function for the widget with the widget number (the index into
		the 'wids' array for this widget) supplied through the
		'client_data' argument.

		Choose between Athena and Xfwf widgets with the option
		'-widgetset' or corresponding application resource.

		-&&Examples of widget specifications

		&&A single window
		| String rwids[] = {
		|   "Label:label1",
		|   "Command:command1",
		|   "Image:image",
		|   "Label:label2",
		|   "Command:command2",
		|   };

		&&Several windows
		One window for the first two widgets, one for the image and
		one for the remaining two widgets:
		| String rwids[] = {
		|   "Label:label1",
		|   "Command:command1",
		|   "Image:image:imageShell",
		|   "Label:label2:Control panel",
		|   "Command:command2",
		|   };

		-&&
		If several widgets are to appear inside the same window, they
		will be children of a container widget which itself is the
		child of a shell widget. The default widget class for the
		container widget is Box (Athena widget). Alternatively, the
		class may be forced to be Form (Athena widget), if no image
		widgets are to appear in the container, by using the option
		'-panelcontainer' (or the application resource
		'panelContainer').

		In the event that the container widget class is Form, the
		labels, command buttons, toggle buttons etc. inside the
		container, need resources specified for their relative
		placement (resources fromVert and fromHoriz). With a container
		widget of class Box they will be placed after each other
		according to their order in the 'wids' array.

		-&
		&res_inst_name
		If 'res_inst_name' is not NULL, a new popup shell will be the
		parent of the given widgets. This popup shell will appear as a
		new window. (If the number of specified widgets are zero, the
		window will not be visible on the screen).

		-&'ximage_layout' sets the windowmanager protocol for window
		deletion. Refer to the 'Actions' section below for information
		on how to close a window upon requeste from the window
		manager.

ximageLayout:  'ximageLayout' is a new version of 'ximage_layout' (since XITE
                version 3.3) with more flexibility.

		The differences between 'ximage_layout' and 'ximageLayout' are:

		&Separate image windows
		Image widgets will by default be created in separate windows.
		This can be overridden with the option '-layout' (and
		application resource 'layout').

		&Container widget class
		By default the container widget is of class Form (Athena
		widget). Alternatively, the class may be forced to be class Box
		(Athena widget) by using the option '-panelcontainer' (or the
		application resource 'panelContainer').
		
		&Multiple images
		Several images can be supplied for each call to 'ximageLayout'.

		&Multiple callback functions
		There is no need to use one common callback function
		'ximage_work_fun' for all the widgets. Separate functions
		can be installed for each widget.

		&Widget specifications
		The widget specifications are different. This is explained
		below.

		-&
		The widget specifications are supplied in the 'layouts'
		argument. The number of widget specifications are given by
		'nwids'.

		&layouts
		'layouts' is an array of pointers to 'layout' references.
		Each 'layout' reference in the array refers to one of a set
		of predefined structures. There is a structure type for each
		of the widget types described for the 'ximage_layout' function
		above. The details of the structures are given in the
		file $XITE_HOME/include/layout.h. A number of functions have
		been defined to handle the widget-type specific structures and
		hide the internal definition from the programmer. These
		functions will be described in the following.

		&&Make_label_layout
		This function returns a reference of type 'layout_label' to
		a structure of type 'layout_label_rec'. This can be used for
		widgets of type "Label", "Ascii_page" and "Ascii", as defined
		above in the description of function 'ximage_layout'. 
		The function prototype for 'Make_label_layout' is shown in the
		'Syntax' section above.

		If the function argument 'lay_out' is NULL, a new object which
		can contain "Label" layout data is created. Otherwise, the one
		referenced by 'lay_out' is used. 'type', 'resource_name' and
		'shell_resource_name' should be used in the same way as the
		fields 'widget_class', 'r_nam' and 'sh_r_nam' described for
		the 'ximage_layout' function above. 'wid' is not used. 'id'
		supplies an identification for the widget. It is not used for
		a "Label" widget.

		&&Make_command_layout
		The differences from 'Make_label_layout' are that 'callback'
		will be installed as a callback function, and 'closure' will
		be the 'client_data' argument for the callback function.
		If 'closure' is NULL, 'id' (actually a reference to an integer
		variable containing the 'id' value) will be passed along as the
		'client_data' argument.

		&&Make_prompt_layout
		The difference from 'Make_command_layout' is that 'text'
		is shown initially in the widget.

		&&Make_scroll_layout
		The differences from 'Make_command_layout' are that two
		callback functions may be specified, one for the XtNjumpProc
		callback list, and one for the XtNscrollProc callback list.
		'id' will be used instead of a NULL 'closure' argument.

		&&Make_image_layout
		The differences from 'Make_label_layout' are that 'img' is
		a reference to a BIFF image and that 'callbacks' refers to
		a structure containing callback function data for an Image
		or ImageOverlay widget.

		To construct the 'callbacks' argument, use the functions
		'add_to_image_callbacks' and/or 'init_image_callbacks'.
		Prototypes for these functions are shown in the 'Syntax'
		section above.

		'init_image_callbacks' simply initializes an object of type
		'image_callbacks' and returns a reference to this object.

		'add_to_image_callbacks' adds the function 'callback' with
		client_data given by 'closure' to the callback list given by
		'resource_name'. With 'first' nonzero, 'callback' is added at
		the front of the callback list, otherwise it is added at the
		end of the list.

		Three other convenience functions are also available for
		handling callback lists: 'add_callback_to_image_layout',
		'add_callback_first' and 'add_callback_last'. The latter two
		are independent of the 'layout' definitions, they operate
		completely within the X Toolkit terminology.

		&&Make_menu_layout
		The difference from 'Make_label_layout' are that 'num_entries'
		gives the number of menu entries and that 'menu_entries' is
		a reference to an array of pointers (of type
		'layout_menu_entry') to menu entry objects (of type
		'layout_menu_entry_rec').

		To construct the 'menu_entries' argument, use the functions
		'Make_entry_layout' and/or 'Add_entry_layout'. The function
		prototypes are shown in the 'Syntax' section above.

		The 'resource_name' argument to 'Make_entry_layout' gives the
		X11 resource instance name of the menu entry widget. If
		'lay_out' is NULL, an object of type 'layout_menu_entry_rec'
		will be constructed. The other arguments should be
		self-explanatory.

		'Add_entry_layout' adds a menu entry to the menu referenced
		by 'lay_out' (which must be non-NULL). The new entry is
		added after any previous entries.

		&&Make_list_layout
		The differences from 'Make_command_layout' are that
		'num_entries' gives the number of entries in the list and that
		'list_entries' is an array of strings, one for each list
		entry. 'callback' is the same for all the list entries.

		-&&
		For all the funtions above, the 'wid' argument will refer to
		the actual widget upon return from 'ximageLayout'.

		If you need access to the various members of the layout
		structures, use the following definitions

		&&TypeOfLayout(layout)
		Return type is "char *".

		&&ResourceNameOfLayout(layout)
		Return type is "char *".

		&&ShellResourceNameOfLayout(layout)
		Return type is "char *".

		&&WidgetOfLayout(layout)
		Return type is "Widget".

		&&IdOfLayout(layout)
		Return type is "int".

		&&CallbackOfCommandLayout(layout)
		Return type is "XtCallbackList".

		&&CallbackOfPromptLayout(layout)
		Return type is "XtCallbackList".

		&&TextOfPromptLayout(layout)
		Return type is "char *".

		&&ResourceNameOfMenuEntry(layout_menu_entry)
		Return type is "char *".

		&&LabelOfMenuEntry(layout_menu_entry)
		Return type is "char *".

		&&WidgetOfMenuEntry(layout_menu_entry)
		Return type is "Widget".

		&&CallbackOfMenuEntry(layout_menu_entry)
		Return type is "XtCallbackList".

		&&NumEntriesOfMenuLayout(layout)
		Return type is "int".

		&&EntriesOfMenuLayout(layout)
		Return type is "layout_menu_entry *".

		&&JumpCallbackOfScrollLayout(layout)
		Return type is "XtCallbackList".
		
		&&ScrollCallbackOfScrollLayout(layout)
		Return type is "XtCallbackList".

		&&NumEntriesOfListLayout(layout)
		Return type is "int".

		&&EntriesOfListLayout(layout)
		Return type is "char **".

		&&CallbackOfListLayout(layout)
		Return type is "XtCallbackList".

		&&ImgOfImageLayout(layout)
		Return type is "IMAGE".

		&&CallbacksOfImageLayout(layout)
		Return type is "image_callbacks *".

		&&DestroyCallbackOfImageLayout(layout)
		Return type is "XtCallbackList".

		&&CallbackCallbackOfImageLayout(layout)
		Return type is "XtCallbackList".

		&&ResizeCallbackOfImageLayout(layout)
		Return type is "XtCallbackList".

		&&DrawCallbackOfImageLayout(layout)
		Return type is "XtCallbackList".

		&&RoiCallbackOfImageLayout(layout)
		Return type is "XtCallbackList".

ximage_mainloop:
                'ximage_mainloop' starts the X11 event loop.

ximage_exit:	'ximage_exit' exits the application. The three arguments are
		not used by the function. It used to be added to the
		XtNdestroyCallback callback list for every Image or
		ImageOverlay widget created with 'ximage_layout', but this
		is not the case any more.

ximage_info:	'ximage_info' prints image widget information, specifically
                the image name (title), size, start coordinates, pixel type,
		panning and zoom, minimum and maximum, visual class, depth
		and text (history) field.

		The minimum and maximum may not correspond to actual extrema
		values in the image. For pixeltype unsigned byte
		(ImageBytePixel), the minimum is assumed to be 0, the maximum
		to be 256, regardless of the actual values. The colormap
		values are distributed over the pixel value range given by the
		minimum and the maximum. An image with pixeltype unsigned byte
		may thus not look identical to the same image with pixeltype
		real.

ximage_histogram:
                'ximage_histogram' makes a histogram window. A copy of the
		current image colormap is added to the Colors colortable menu
		and made the current image colormap (in "reduced-color" mode).
		This colormap can be manipulated from the histogram window.

		'cb_data' is a reference to an object of type
		'image_disp_cb_data'

		| typedef struct {
		| int index;
		| ximage_display_func display_func;
		| image_callbacks *callbacks;
		| } image_disp_cb_data

		'display_func' is the function used to send histogram
		transformed images back from the histogram window. This should
		typically be the function 'ximage_display' which is described
		below. The type definition of 'ximage_display_func' is

		| typedef Widget (*ximage_display_func)( Widget parent,
		|    char *resource_name, char *shell_resource_name,
		|    IMAGE img, int bandno, WidgetClass widgetClass,
		|    Visual *visual, int depth, menu_button buttons,
		|    ArgList args, Cardinal num_args,
		|    image_callbacks *callbacks );

		'callbacks' is a collection of callback lists for the
		Image and ImageOverlay widget displayed with 'display_func'.
		'callbacks' should be constructed with the functions
		'init_image_callbacks' and/or 'add_to_image_callbacks'
		described above.

		'index' is not used.

		'ximage_histogram' calls the 'xhistogram' function. Refer to
		'xhistogram(3)' for more information.

ximage_slice:   'ximage_slice' makes a color palette window. A copy of the
                current image colormap is added to the Colors colortable
		menu and made the current image colormap (in "reduced-color"
		mode). This colormap can be manipulated from the palette
		(slice) window by specifying rgb or ihs color values.

		'ximage_slice' calls the 'xslice' function. Refer to
		'xslice(3)' for more information.

ximage_colorbar:
                'ximage_colorbar' makes a colorbar window for the colortable
		currently active in an image window.

ximage_display: 'ximage_display' creates an Image or ImageOverlay widget
                (depending on 'widgetClass') with image contents given by
		the first (or three first) band(s) in 'img'. A new window
		may be created for the widget. See the description of the
		'parent' argument.

		'parent' must be non-NULL. If it refers to a shell widget,
		a new window will be created for the image.

		'resource_name' gives the widget resource instance name
		(default "image" if the argument is NULL). We recommend the
		default value, to make it easier to write X resource files.

		'shell_resource_name' gives the resource instance name of the
		ancestor shell (default "imageShell" if the argument is NULL).
		'shell_resource_name' is only used if a new window is created
		for the image.

		'args' may be a list of 'num_args' widget resource settings.

		'buttons' determines which menus should be available from the
		image widget. The menus can be reached from a menubar above
		the image (unless the menubar has been made invisible, e.g.
		with the use of option '-mb') and as popup menus inside the
		image widget. 'buttons' is a bitwise OR between COLORS_BUTTON,
		OVERLAYS_BUTTON, TOOLS_BUTTON, OPTIONS_BUTTON and
		VISUALS_BUTTON. The value of 'buttons' can be controlled via
		some options, such as '-colorsmenu'. The necessary
		translations for popup menus are added automatically. Refer to
		the 'Options' and 'Actions' sections below. A request for an
		Overlay menu is only honored if 'widgetClass' refers to
		imageOverlayWidgetClass.

		'img' specifies the image data to be displayed. 'bandno'
		specifies the band number from 'img' which will be displayed
		in the Image widget.

		If 'img' has exactly three bands, 'bandno' is equal to one and
		the '-rgb' options is used (or the 'rgbImages' resource is
		set), then 'img' is displayed as an rgb color image where band
		1 represents red, band 2 represents green and band 3
		represents the blue component. For this to work as expected,
		the visual class must be selected appropriately. See the
		descriptions of options '-rgb' and '-iv' below.

		'bandno' equal to zero will give the default value of one.

		An image will be displayed with a default monotonously rising
		colormap. When only one band of an image is used in the
		display, the default colormap is a gray-scale colormap (in
		"reduced-color" mode). For a three-band image the default
		colormap is the same for all the three bands, and the image
		will appear with its expected colors (subject to the
		conditions of an appropriate visual class as described above).
		The choice of colormap can be changed with the function
		'set_init_color', the option '-colortable' or the application
		resource 'colorTable' described below. The initial colortable
		for an image overlay is by default a predefined 32-element
		table. It can be overridden with the function
		'set_init_ovl_color', the option '-overlaytable' or the
		application resource 'overlayTable' described below.

		If the display hardware does not have a changeable colormap,
		the default colormap for the display will be used to display
		the image.

		'callbacks' is a collection of callback lists for the Image
		or ImageOverlay widget. It should be constructed with the
		functions 'add_to_image_callbacks' and/or
		'init_image_callbacks' described above.

		Default value for 'visual' is the requested/preferred visual
		type, and default for 'depth' is the depth of 'visual'.
		To get the default values, use NULL and 0 respectively.
		The option '-iv' can be used to change the requested/preferred
		visual type.

		'args' are overridden by 'img' (and its bands), 'visual' and
		'depth'.

		'ximage_display' is used for the Image and ImageOverlay
		widgets created by 'ximage_layout' and 'ximageLayout'.

		The return value from 'ximage_display' is a reference to the
		created image widget (not its ancestor). This widget is
		realized, but not popped up. It needs a call to XtPopup to
		become visible.
		
set_init_color: 'set_init_color' sets the colortable which will be used
                when new images are created. By default this is a predefined
		gray-scale colortable (in "reduced-color" mode). It can
		also be overridden with option '-colortable' (or the
		application resource 'colorTable').

		'color' is an index into the list of colortables available
		via the Colors menu in the image widget.

set_init_ovl_color:
                'set_init_ovl_color' sets the colortable for new image
		overlays. Default is the built-in standard 32-element table.
		It can also be overridden with option '-overlaytable' (or the
		application resource 'overlayTable').

		'color' is an index into the list of colortables available
		via the OverlayColors menu in the image widget.

LogPositionCallback:
                This function is added at the front of the XtNcallback
		callback list of all images created with 'ximage_layout' or
		'ximageLayout'. 'LogPositionCallback' may also be added to
		the callback lists of images created without the use of
		'ximage_layout' or 'ximageLayout'.

		'LogPositionCallback' will enable the coordinates of the
		cursor position in an image window to be displayed. The
		'client_data' input argument to the callback function must
		refer to the label widget into which the cursor position will
		be written.

		If the label widget referred to by the 'client_data' argument
		of the callback function is the "label" part of an Xaw dialog
		widget, then this dialog widget is assumed to be the child of
		a popup shell which will be popped up/down when needed.
		Otherwise, it is assumed that the label widget is permanently
		visible, and no popping up or down is handled.

		'call_data' must supply an 'ImageCallback' reference which
		contains cursor coordinates etc.

		The functions on the XtNcallback list are called by the
		"notify" Image widget action function. The action function
		supplies the proper 'call_data' argument.

LogPosition:    Add 'LogPositionCallback' to the XtNcallback callback list of
                widget 'image_wid' with client data 'label_wid'.

Options:        'ximage' based programs accept all standard X Toolkit command
		line options in addition to the following. They don''t all
		apply to all the widget types supplied  to 'ximage_layout' or
		'ximageLayout'.

		All the options have their X resource equivalents. This is
		the reason why many of the options have two legal forms,
		one to give a certain effect, the other to cancel the effect.
		This is necessary in order to be able to override X resource
		settings. See also the section 'Resources' below.

		Unique abbreviations for the options will be accepted.

		&-adjustwidth
		Try to automatically adjust the width of "Label", "Scroll"
		and "Ascii_page" widgets to match the widest widget in
		container-widgets of class Box.	'+adjustwidth' has the reverse
		effect.

		&-aspect
		Don''t preserve image aspect ratio when resizing windows.
		Normally an image window will have the same width/height ratio
		as the image data. When a window is resized, it will be forced
		to keep this ratio unchanged. If '-aspect' is specified, it is
		possible to resize the window independently in the width and
		height directions.  This can also be switched on/off
		individually for each image (in the Options menu).
  		
		&-cl filename, -colorlist filename
		The file given by 'filename' contains a list of colortable
		filenames which will be used instead of the default list of
		colortables. Leading "~" and environment variables in
		'filename' will be expanded. If not found, try to find
		'filename' in one of the directories listed in the environment
		variable XSHOWPATH. The colortables will be available from the
		Colors menu of the image windows. See also option
		'-colortable'.

		The default value for 'filename' is "xshow_colortabs".

		&-colmenu, -colorsmenu
		Supply the Image and ImageOverlay widgets with a color menu.
		Default: Supply menu. The form '+colmenu' has the reverse
		effect.

          	&-ct filename, -colortable filename
		The file 'filename' contains a colortable. Leading "~" and
		environment variables in 'filename' will be expanded. If not
		found, try to find 'filename' in one of the directories listed
		int the environment variable XSHOWPATH. The colortable will be
		available from the Colors menu of the image windows.

		Image command-line arguments will initially be displayed with
		this colortable. Default is a monotonously rising colormap,
		grayscale for single band image displays.

		&-full
                Do not use "Reduced color" display of images to reduce
                "technicolor" problems. The default is that only 128 colors
                will be used to display images with PseudoColor visuals and
                only about 2 million colors will be used to display images
                with DirectColor visuals. This can be toggled individually for
                each image, from the Options menu. This option directly sets
		the XtNdisplayMethod Image and ImageOverlay widget resource
		to one.
 
		&-ih height, -imageheight height
		Height of Image and ImageOverlay widgets. '-aspect' can
		be useful with '-ih', along with toggling the fixed aspect
		ratio entry of the Image Options menu.

		&-iv VisualClassName
                Use 'VisualClassName' for all image windows (and certain parts
                of the histogram and slice application windows). Default:
                PseudoColor. If this is not available, try DirectColor. If
                this also fails, try TrueColor.

		Legal choices: PseudoColor, DirectColor and TrueColor. The
		depth of the visual can not be chosen. For PseudoColor, a
		depth of 8 is used, for DirectColor and TrueColor, a depth of
		24 is used. See also the menu entry in the 'Visuals' menu of
		the image widget, the '-visualsmenu', the '-multivisual' and
		the '-rgb' options.
   
		&-iw width, -imagewidth width
		Width of Image and ImageOverlay widgets. '-aspect' can
		be useful with '-iw', along with toggling the fixed aspect
		ratio entry of the Image Options menu.

		&-layout type
		Determines whether images get separate windows or are included
		in container widgets along with buttons etc. Default for
		'ximage_layout' is to obey the widget specifications in the
		'wids' argument. Default for 'ximageLayout' is to separate
		images from other widgets. 'type' can be one of "separate",
		"together" or "check", where the latter means to obey the
		'wids' argument.

		&-logpos
                Turn log position mode on. When the cursor is moved inside an
                image and the 'Log position' option is on, the cursor
                position and pixel value are printed. Default: Off.

		&-mb, -menubar
		Start without menubar above image windows. The menubar will
		be a widget of class XfwfMenuBar. The default height of the
		menubar is 30 pixels. The width will be the same as the image
		width.

		&-multivisual
		Enable display of images for all available visual classes, via
                the Image visuals menu. Implies -visualsmenu.

		&-optionsmenu
		Supply the Image and ImageOverlay widgets with an options
		menu. Default: Supply menu. The form '+optionsmenu' has the
		reverse effect.

		&-ovlmenu, -overlaysmenu
		Supply the ImageOverlay widgets with a color menu for image
		overlays. Default: Supply menu. The form '+ovlmenu' has the
		reverse effect.

          	&-ovt filename, -overlaytable filename
		Similar to option '-colortable', but 'filename' now contains a
		colortable for an image overlay. It will be available from the
		image widget OverlayColors menu for widgets of class
		ImageOverlay.

		Image overlay command-line arguments will initially be
		displayed with this colortable. Default is a built-in
		32-element table.

		&-panelcontainer type
		Determines the widget class of container widgets which will
		hold buttons etc. Default for 'ximage_layout' is Box, for
		'ximageLayout' it is Form. 'type' can be "Box" or "Form"
		(case is not considered).

          	&-protect
		Set 'protect' mode on. It will then not be possible to overdraw
		nonzero pixels in the overlay plane.

		&-rgb
                Assume that all three-band images represent 24-bit color
                images, where the first, second and third bands represent the
                red, green and blue colors respectively. If a three-band image
                is displayed on a 24 bitplane RGB Color screen and the '-rgb'
                switch is specified, the program will combine the three bands
                to a color image. If the image is displayed on a different
                kind of screen, the three bands will be shown separately. For
                this to work, you may need to consider option '-iv' or the
                resource 'imageVisualClass'. Default: No.

		&-share
		Let non-image windows use the same initial colormap as image
		windows. This can reduce colormap flashing on displays with
		a single hardware colormap. It is especially useful in this
		case for applications which involve manipulation of a non-image
		widget with real-time image changes, such as using the slider
		in the XITE applications 'xadd' and 'xfilter'.

		Non-image windows will use a copy of the "Work-map" image
		colormap or the image colormap given by the ximage option
		'-ct'. The non-image windows colormap will not change when the
		image colormap is changed via the Colors image menu.

		Every 'ximage' based application will issue a recommendation
		to use this option if it determines that the option may reduce
		colormap flashing.

		The drawback of using this option is that the colors of the
		non-image windows may not be set equal to the colors requested
		in the X resource file (or with X toolkit options -bg and -fg).
		Warnings will be issued to this effect, from the X toolkit.

		This option only works when the preferred image visual class
		equals the default display visual class.

		&-toolsmenu
		Supply the Image and ImageOverlay widgets with a tools menu.
		Default: Supply menu. The form '+toolsmenu' has the reverse
		effect.

		&-version
                Print XITE version number and exit. Default: Don''t.

		&-visualsmenu
		Supply the Image and ImageOverlay widgets with a visuals menu.
		Default: Supply menu. The form '+visualsmenu' has the
		reverse effect.

		&-widgetset name
		'name' can be Xaw or Xfwf. Choose between the Athena widgets
		Command, Label and Toggle and the Xfwf widgets XfwfButton,
		XfwfLabel and XfwfToggle. This only affects widgets created
		with 'ximage_layout' or 'ximageLayout'.

          	&-zoomall
		Turn 'zoom all' mode on. With this mode set, all images
		will be zoomed/panned with the same parameters.

		-&The two following options are obsolete since XITE version
		3.3.

		&-iow width, -imageoverlaywidth width
		This used to set the width of ImageOverlay widgets only.
		This is now handled by '-iw' as long as the widget resource
		instance name of the ImageOverlay widget equals "image"
		(the default).

		&-ioh height, -imageoverlayheight height
		This used to set the height of ImageOverlay widgets only.
		This is now handled by '-ih' as long as the widget resource
		instance name of the ImageOverlay widget equals "image"
		(the default).

Actions:        Refer to 'Image(3)' and 'ImageOverlay(3)' for descriptions
                of available Image and ImageOverlay widget action functions.

		Applications initialized with 'ximage_init' or 'ximageInit'
		have a few additional action functions available. These are

		&PopupMenu
		Refer to 'PopupMenu(3)'. This is added to the default
		translations described below.

		&PopupExit
		Refer to 'PopupExit(3)'. This is added to the default
		translations described below.

		&CloseShell
		Destroy the shell widget ancestor of a widget. This is not
		added to the default translations. To close a window when
		requested by a window manager operation, add the following
		translation to an application resource file

		|*TopLevelShell.baseTranslations: #override \n\
		|  <ClientMessage>WM_PROTOCOLS: CloseShell()

Default translations:

                Apart from the mouse button and key bindings defined by the
		'Image(3)' and 'ImageOverlay(3)' widgets, the following are
		added by the initialization functions 'ximage_init' and
		'ximageInit'.

		&Menues
		Whether these bindings are in effect depend on the options
		'-colmenu', '-ovlmenu', '-toolsmenu', '-optionsmenu' and
		'-visualsmenu' and the corresponding application resources.
		| Shift <Btn1Down>: ColorsHook()
		|                   PopupMenu(colorsmenu)
		| Shift <Btn2Down>: PopupMenu(toolsmenu)
		| Shift <Btn3DOwn>: OptionsHook()
		|                   PopupMenu(optionsmenu)
		| Shift Mod1 <Btn1Down>: OverlaysHook()
		|                        PopupMenu(overlaysmenu)
		|     (Overlay colortables, only for
		|      ImageOverlay widgets)
		| Shift Mod1 <Btn3Down>: VisualsHook()
		|                        PopupMenu(visualsmenu)

		The Hook functions in the above translations take care of
		adding marks to active menu entries and desensitizing
		illegal/irrelevant entries.

Resources:      'ximage'-based programs accept all standard X Toolkit
                resources.  Some other common resources are mentioned below.
		See also the description of 'ximage_layout' and 'ximageLayout'
		above.

		Options take precedence over resources.

		Application resources. (Application name and class is given
		by the actual application which calls 'ximage_init' or
		'ximageInit'. The application name can also be changed
		with the X Toolkit option -name.):

		&adjustWidth (Class AdjustWidth)
		See option '-adjustwidth'.

		&colorList (Class ColorList)
		See option '-cl'.

		&colorsMenu (Class ImageMenu)
		See option '-colmenu'.

		&colorTable (Class ColorTable)
		See option '-ct'.

		&imageVisualClass (Class VisualClass)
		See option '-iv'.

		&layout (Class Layout)
		See option '-layout'.

		&logpos (Class LogPos)
		See option '-logpos'.

		&optionsMenu (Class ImageMenu)
		See option '-optionsmenu'.

		&overlaysMenu (Class ImageMenu)
		See option '-ovlmenu'.

		&overlayTable (Class ColorTable)
		See option '-overlaytable'.

		&panelContainer (Class PanelContainer)
		See option '-panelcontainer'.

		&rgbImages (Class RgbImages)
		See option '-rgb'.

		&toolsMenu (Class ImageMenu)
		See option '-toolsmenu'.

		&version (Class Version)
		See option '-version'.

		&visualsMenu (Class ImageMenu)
		See option '-visualsmenu'.

		&widgetSet (Class WidgetSet)
		See option '-widgetset'.

		&xiteHelper (Class XiteHelper)
                Determines which program is started when the 'help' button
		is pushed. Colon-separated list of strings, started in the
		order listed until one is successful.

		&xiteHelperOption (Class XiteHelperOption)
                Options for the resource xiteHelper.

		&xiteResourceVersion
		This resource should typically be the same as the XITE version
		(given in response to option '-version'). If the 'warn'
		argument to 'ximage_init' or 'ximageInit' is True, the ximage
		application will check whether 'xiteResourceVersion' is too
		old for the running XITE version.

		&zoomAll (Class ZoomAll)
		See option '-zoomall'.

		-&
		Some of the Image widget resources:

		&displayMethod (Class DisplayMethod)
		See option '-full'.

		&imageHeight (class ImageHeight)
		See option '-ih'.

		&imageWidth (class ImageWidth)
		See option '-iw'.

		&menubar (Class Menubar)
		See option '-mb'.

		&protectOverlay (Class ProtectOverlay)
		See option '-protect'.

		&variableAspect (Class VariableAspect)
		See option '-aspect'.

		-&
		See the section 'Widget hierarchy' on how to specify resources.

Widget hierarchy:
                &ximage_layout
		The top of the hierarchy is always
                | <application name> (Class <application class>)

		The second level depends on how the widgets have been created.
		If the application consists of a single window which contains
		an image as well as buttons etc., the second level will most
		likely be
                |   topContainer (Class Box or Form) (if nwids > 1)

		The class of topContainer is Box by default, but can be set
		to Form with option '-panelcontainer' or the resource
		'panelContainer'.

		With multiple windows or if 'sh_r_nam' was given in the
		widget specification in 'wids', the second level will be
		|   <sh_r_nam> (Class TopLevelShell)

		The level below 'sh_r_nam' will be
                |   topContainer (Class Box or Form)
		if at least one of the widgets "belonging" to 'sh_r_nam'
		is not an image. If there is only one widget which is an image,
		then the level will be
		|   imageForm (Class ImageForm)
		followed by
		|     image (Class Image or ImageOverlay)

		Below a "topContainer" level follows any number of widgets
		according to the widget specifications in the 'wids' argument
		to 'ximage_layout' (described above). The widget
		specifications give both resource instance name and class.

		See the documentation for each specific application to find
		out which widget classes are used and the specific widget
		instance names.

		&ximageLayout
		The hierarchy will be the same as for 'ximage_layout',
		but the widget specifications are of course not given by
		'wids', but when creating the 'layout' structures by calld to
		'Make_label_layout', 'Make_image_layout' etc.

See also:	'Image(3)', 'ImageOverlay(3)', 'xshow(1)', 'xadd(1)',
                'xfft(1)', 'xfilter(1)', 'xhistogram(1)', 'xmovie(1)',
		'xpyramid(1)', 'xregion(1)', 'ximage_example(1)',
		'XfwfButton(3)', 'XfwfMenuBar(3)', 'XfwfPrompt(3)',
		'XfwfPullDown(3)', 'XfwfToggle(3)'.

Author:		Otto Milvang and Svein Bøe
Doc:            Svein Bøe
Id: 		$Id$
________________________________________________________________

*/

#include <xite/includes.h>
#include <X11/Xos.h>
#include <X11/Intrinsic.h>
#include <X11/Shell.h>

#ifdef MOTIF
#include <Xm/ToggleB.h>
#include <Xm/PushB.h>
#include <Xm/Label.h>
#include <Xm/RowColumn.h>
#include <Xm/MainW.h>
#include <Xm/ScrollBar.h>
#include <Xm/Text.h>
#else
#include <X11/StringDefs.h>
#include <X11/Xaw/Toggle.h>
#include <X11/Xaw/Command.h>
#include <X11/Xaw/MenuButton.h>
#include <X11/Xaw/Label.h>
#include <X11/Xaw/Box.h>
#include <X11/Xaw/Form.h>
#include <X11/Xaw/Scrollbar.h>
#include <X11/Xaw/AsciiText.h>
#include <X11/Xaw/Dialog.h>
#include <X11/Xaw/List.h>
#endif

#include <xite/biff.h>
#include <stdlib.h>
#include <xite/color.h>
#include <xite/Image.h>
#include <xite/ImageOverlay.h>
#include <xite/ImageToolsMenu.h>
#include <xite/ImageOptionsMenu.h>
#include <xite/ImageVisualsMenu.h>
#include <xite/ImageForm.h>
#include <xite/message.h>
#include <xite/readarg.h>
#include <xite/statistics.h>
#include <xite/Visual.h>
#include <xite/ShellWids.h>
#include <xite/Cmap.h>
#include <xite/Xcolor.h>
#include <xite/Xdialog.h>
#include <xite/FormDialog.h>
#include <xite/Xhistogram.h>
#include <xite/Xslice.h>
#include <xite/XmenuBar.h>
#include <Xfwf/Prompt.h>
#include <Xfwf/Button.h>
#include <Xfwf/Toggle.h>
#include XITE_MALLOC_H
#include XITE_STDIO_H
#include XITE_STRING_H
#include <xite/strings.h>
#include <xite/utils.h>
#include <xite/layout.h>
#include <xite/logPos.h>
#include <xite/callbacks.h>
#include <xite/mallocp.h>
#include <xite/ximage.h>
#include <xite/ximage_display.h>
#include <xite/debug.h>

#define CVW XtVaCreateManagedWidget

Display     *dpy_e;
Atom        wm_delete_window;    /* For ICCCM deletion of window. */
menu_button image_menus_e;
Boolean     multi_visual_e;
Widget      Control_e;           /* Top application shellwidget,
				  * returned to application. */

Widget      colorsmenu_e, overlaysmenu_e, toolsmenu_e, optionsmenu_e,
            visualsmenu_e;
char        *colortabfile_e, *ovltabfile_e, *colorlistfile_e;
int         InitialColorTab_e = 0, InitialOvlTab_e = 0;
Boolean     share_e;
char        *xiteHelper_e, *xiteHelperOption_e;

extern int  zoomall_e;
extern int  logpos_e;
extern int  rgb_e;


/* Global static variables. */

static Widget       toplevel_s;
static Widget       PosWid_s;   /* Widget to contain image cursor position,
				 * for all image widgets. */
static XtAppContext appcon_s;   /* Can be determined by application by a call
				 * to XtWidgetToApplicationContext. */
static int          screen_s;   /* Can be determined by application by a call
				 * to DefaultScreen. */
static Boolean      adjust_width_s = 0;

typedef enum {
  UNKNOWN_SEPARATE,
  CHECK_PANEL_RES,
  SEPARATE,
  TOGETHER
} separate_type;

typedef enum {
  UNKNOWN_WIDGET_SET,
  XAW_WIDGETS,
  XFWF_WIDGETS
} widget_set_type;

static separate_type   layout_s          = CHECK_PANEL_RES;
static type_container  panel_container_s = BOX_CONTAINER;
static widget_set_type widget_set_s      = XAW_WIDGETS;
static char *imageVisual_s;

typedef enum {
  ASCII_PAGE_WID,
  ASCII_WID,
  COMMAND_WID,
  IMG_WID,
  LABEL_WID,
  LIST_WID,
  MENU_WID,
  OVERLAY_WID,
  PROMPT_WID,
  SCROLL_WID,
  TOGGLE_WID
} widget_type;

typedef struct {
  char *coltab;
  char *ovltab;
  char *colorList;
  char *imageVisual;
  char *xiteHelper;
  char *xiteHelperOption;
  float xiteResourceVersion;
  Boolean zoomall;
  Boolean rgb_images;
  Boolean logpos;
  Boolean version;
  Boolean colorsmenu, overlaysmenu, toolsmenu, optionsmenu, visualsmenu;
  char *layout;
  Boolean adjust_width;
  char *panel_container;
  char *widget_set;
  Boolean share;
  Boolean multivisual;
} AppData, *AppDataPtr;

static AppData appResResult_s;

static XtResource resources_s[] = {
  /* "colorTable" and "overlayTable" must be application resources, not only
   * widget resources. In addition to setting a widget colortable, the
   * application must also add the table to the color menu. */
  {
    "colorTable",
    "ColorTable",
    XtRString,
    sizeof(String),
    XtOffsetOf(AppData, coltab),
    XtRString,
    NULL,
  },
  {
    "overlayTable",
    "ColorTable",
    XtRString,
    sizeof(String),
    XtOffsetOf(AppData, ovltab),
    XtRString,
    NULL,
  },
  {
    "colorsMenu",
    "ImageMenu",
    XtRBoolean,
    sizeof(Boolean),
    XtOffsetOf(AppData, colorsmenu),
    XtRString,
    (XtPointer) "True",
  },
  {
    "overlaysMenu",
    "ImageMenu",
    XtRBoolean,
    sizeof(Boolean),
    XtOffsetOf(AppData, overlaysmenu),
    XtRString,
    (XtPointer) "True",
  },
  {
    "toolsMenu",
    "ImageMenu",
    XtRBoolean,
    sizeof(Boolean),
    XtOffsetOf(AppData, toolsmenu),
    XtRString,
    (XtPointer) "True",
  },
  {
    "optionsMenu",
    "ImageMenu",
    XtRBoolean,
    sizeof(Boolean),
    XtOffsetOf(AppData, optionsmenu),
    XtRString,
    (XtPointer) "True",
  },
  {
    "visualsMenu",
    "ImageMenu",
    XtRBoolean,
    sizeof(Boolean),
    XtOffsetOf(AppData, visualsmenu),
    XtRString,
    (XtPointer) "True",
  },
  {
    "colorList",
    "ColorList",
    XtRString,
    sizeof(String),
    XtOffsetOf(AppData, colorList),
    XtRString,
    NULL,
  },
  { "rgbImages",
    "RgbImages",
    XtRBoolean,
    sizeof(Boolean),
    XtOffsetOf(AppData, rgb_images),
    XtRString,
    (XtPointer) "False",
  },
  { "logPos",
    "LogPos",
    XtRBoolean,
    sizeof(Boolean),
    XtOffsetOf(AppData, logpos),
    XtRString,
    (XtPointer) "False",
  },
  {
    "imageVisualClass",
    "VisualClass",
    XtRString,
    sizeof(String),
    XtOffsetOf(AppData, imageVisual),
    XtRString,
    NULL,
  },
  {
    "multiVisual",
    "MultiVisual",
    XtRBoolean,
    sizeof(Boolean),
    XtOffsetOf(AppData, multivisual),
    XtRString,
    (XtPointer) "False",
  },
  {
    "zoomAll",
    "ZoomAll",
    XtRBoolean,
    sizeof(Boolean),
    XtOffsetOf(AppData, zoomall),
    XtRString,
    (XtPointer) "False",
  },
  {
    "share",
    "Share",
    XtRBoolean,
    sizeof(Boolean),
    XtOffsetOf(AppData, share),
    XtRString,
    (XtPointer) "False",
  },
  {
    "xiteHelper",
    "XiteHelper",
    XtRString,
    sizeof(String),
    XtOffsetOf(AppData, xiteHelper),
    XtRString,
    NULL,
  },
  {
    "xiteHelperOption",
    "XiteHelperOption",
    XtRString,
    sizeof(String),
    XtOffsetOf(AppData, xiteHelperOption),
    XtRString,
    NULL,
  },
  { "version",
    "Version",
    XtRBoolean,
    sizeof(Boolean),
    XtOffsetOf(AppData, version),
    XtRString,
    (XtPointer) "False",
  },
  {
    "xiteResourceVersion",
    "XiteResourceVersion",
    XtRFloat,
    sizeof(float),
    XtOffsetOf(AppData, xiteResourceVersion),
    XtRString,
    NULL,
  },
  {
    "layout",
    "Layout",
    XtRString,
    sizeof(String),
    XtOffsetOf(AppData, layout),
    XtRString,
    NULL,
  },
  {
    "adjustWidth",
    "AdjustWidth",
    XtRBoolean,
    sizeof(Boolean),
    XtOffsetOf(AppData, adjust_width),
    XtRString,
    (XtPointer) "True",
  },
  {
    "panelContainer",
    "PanelContainer",
    XtRString,
    sizeof(String),
    XtOffsetOf(AppData, panel_container),
    XtRString,
    NULL,
  },
  {
    "widgetSet",
    "WidgetSet",
    XtRString,
    sizeof(String),
    XtOffsetOf(AppData, widget_set),
    XtRString,
    NULL,
  },
};

#ifndef FUNCPROTO
XtResource *ximage_app_resources(resources, num_resources)
XtResource *resources;
int *num_resources;
#else /* FUNCPROTO */
XtResource *ximage_app_resources(XtResource *resources, int *num_resources)
#endif /* FUNCPROTO */
{
  XtResource *all_resources;
  int i, num_ximage_resources = XtNumber(resources_s);

  *num_resources += num_ximage_resources;
  all_resources = (XtResource *) malloc(*num_resources * sizeof(XtResource));

  for (i = 0; i < num_ximage_resources; i++) {
    all_resources[i].resource_name   = resources_s[i].resource_name;
    all_resources[i].resource_class  = resources_s[i].resource_class;
    all_resources[i].resource_type   = resources_s[i].resource_type;
    all_resources[i].resource_size   = resources_s[i].resource_size;
    all_resources[i].resource_offset = resources_s[i].resource_offset;
    all_resources[i].default_type    = resources_s[i].default_type;
    all_resources[i].default_addr    = resources_s[i].default_addr;
  }
  for (i = num_ximage_resources; i < (*num_resources); i++) {
    all_resources[i].resource_name   =
      resources[i-num_ximage_resources].resource_name;
    all_resources[i].resource_class  =
      resources[i-num_ximage_resources].resource_class;
    all_resources[i].resource_type   =
      resources[i-num_ximage_resources].resource_type;
    all_resources[i].resource_size   =
      resources[i-num_ximage_resources].resource_size;
    all_resources[i].resource_offset =
      resources[i-num_ximage_resources].resource_offset;
    all_resources[i].default_type    =
      resources[i-num_ximage_resources].default_type;
    all_resources[i].default_addr    =
      resources[i-num_ximage_resources].default_addr;
  }

  return(all_resources);

} /* ximage_app_resources() */

/* Using resource instance name "image", which is always the resource instance
 * name of every image widget created with ximage toolkit functions
 * ximage_layout and ximageLayout. Images created directly with ximage_display
 * may have a different resource instance name.
 */
static XrmOptionDescRec opTable_s[] = {

  /* Application resources. */

  { "-adjustwidth", ".adjustWidth",      XrmoptionNoArg,  (XtPointer) "True"},
  { "+adjustwidth", ".adjustWidth",      XrmoptionNoArg,  (XtPointer) "False"},
  { "-cl", 	    ".colorList",        XrmoptionSepArg, (XtPointer) NULL},
  { "-colorlist",   ".colorList",        XrmoptionSepArg, (XtPointer) NULL},
  { "-ct", 	    ".colorTable",       XrmoptionSepArg, (XtPointer) NULL},
  { "-colortable",  ".colorTable",       XrmoptionSepArg, (XtPointer) NULL},
  { "-iv", 	    ".imageVisualClass", XrmoptionSepArg, (XtPointer) NULL},
  { "-layout",      ".layout",           XrmoptionSepArg, (XtPointer) NULL},
  { "-logpos",      ".logPos",           XrmoptionNoArg,  (XtPointer) "True"},
  { "-multivisual", ".multiVisual",      XrmoptionNoArg,  (XtPointer) "True"},
  { "+multivisual", ".multiVisual",      XrmoptionNoArg,  (XtPointer) "False"},
  { "-ovt", 	    ".overlayTable",     XrmoptionSepArg, (XtPointer) NULL},
  { "-overlaytable",".overlayTable",     XrmoptionSepArg, (XtPointer) NULL},
  { "-panelcontainer",".panelContainer", XrmoptionSepArg, (XtPointer) NULL},
  { "-rgb",         ".rgbImages",        XrmoptionNoArg,  (XtPointer) "True"},
  { "-share",       ".share",            XrmoptionNoArg,  (XtPointer) "True"},
  { "-version",     ".version",          XrmoptionNoArg,  (XtPointer) "True"},
  { "-widgetset",   ".widgetSet",        XrmoptionSepArg, (XtPointer) NULL},
  { "-zoomall",     ".zoomAll",          XrmoptionNoArg,  (XtPointer) "True"},

  { "-colmenu",     ".colorsMenu",       XrmoptionNoArg,  (XtPointer) "True"},
  { "-colorsmenu",  ".colorsMenu",       XrmoptionNoArg,  (XtPointer) "True"},
  { "+colmenu",     ".colorsMenu",       XrmoptionNoArg,  (XtPointer) "False"},
  { "+colorsmenu",  ".colorsMenu",       XrmoptionNoArg,  (XtPointer) "False"},
  { "-optionsmenu", ".optionsMenu",      XrmoptionNoArg,  (XtPointer) "True"},
  { "+optionsmenu", ".optionsMenu",      XrmoptionNoArg,  (XtPointer) "False"},
  { "-ovlmenu",     ".overlaysMenu",     XrmoptionNoArg,  (XtPointer) "True"},
  { "-overlaysmenu",".overlaysMenu",     XrmoptionNoArg,  (XtPointer) "True"},
  { "+ovlmenu",     ".overlaysMenu",     XrmoptionNoArg,  (XtPointer) "False"},
  { "+overlaysmenu",".overlaysMenu",     XrmoptionNoArg,  (XtPointer) "False"},
  { "-toolsmenu",   ".toolsMenu",        XrmoptionNoArg,  (XtPointer) "True"},
  { "+toolsmenu",   ".toolsMenu",        XrmoptionNoArg,  (XtPointer) "False"},
  { "-visualsmenu", ".visualsMenu",      XrmoptionNoArg,  (XtPointer) "True"},
  { "+visualsmenu", ".visualsMenu",      XrmoptionNoArg,  (XtPointer) "False"},


  /* Widget resources. */

  { "-aspect",     "*image.variableAspect",
                                         XrmoptionNoArg,  (XtPointer) "True"},
  { "-full",       "*image.displayMethod",
                                         XrmoptionNoArg,  (XtPointer) "1"},
  { "-ih", 	   "*image.height",      XrmoptionSepArg,
      (XtPointer) NULL},
  { "-imageheight","*image.height",      XrmoptionSepArg, 
      (XtPointer) NULL},
  { "-iw", 	   "*image.width",       XrmoptionSepArg,
      (XtPointer) NULL},
  { "-imagewidth", "*image.width",       XrmoptionSepArg,
      (XtPointer) NULL},

  { "-mb",         "*image.menubar",     XrmoptionNoArg,  (XtPointer) "False"},
  { "-menubar",    "*image.menubar",     XrmoptionNoArg,  (XtPointer) "False"},

  { "-protect",    "*ImageOverlay.protectOverlay",
                                         XrmoptionNoArg,  (XtPointer) "True"},
};

#ifndef FUNCPROTO
XrmOptionDescRec *ximage_app_options(options, num_options)
XrmOptionDescRec *options;
int *num_options;
#else /* FUNCPROTO */
XrmOptionDescRec *ximage_app_options(XrmOptionDescRec *options, int *num_options)
#endif /* FUNCPROTO */
{
  XrmOptionDescRec *all_options;
  int i, num_ximage_options = XtNumber(opTable_s);

  *num_options += num_ximage_options;
  all_options = (XrmOptionDescRec *)
    malloc(*num_options * sizeof(XrmOptionDescRec));

  for (i = 0; i < num_ximage_options; i++) {
    all_options[i].option    = opTable_s[i].option;
    all_options[i].specifier = opTable_s[i].specifier;
    all_options[i].argKind   = opTable_s[i].argKind;
    all_options[i].value     = opTable_s[i].value;
  }
  for (i = num_ximage_options; i < (*num_options); i++) {
    all_options[i].option    = options[i-num_ximage_options].option;
    all_options[i].specifier = options[i-num_ximage_options].specifier;
    all_options[i].argKind   = options[i-num_ximage_options].argKind;
    all_options[i].value     = options[i-num_ximage_options].value;
  }

  return(all_options);

} /* ximage_app_options() */



#ifndef FUNCPROTO
static widget_type parse_widget_type(type)
char *type;
#else /* FUNCPROTO */
static widget_type parse_widget_type(char *type)
#endif /* FUNCPROTO */
{
  /* Allow one-letter (capital) types for some so as not to break old
   * applications. */

  if (! strcasecmp(type, "Ascii_page" ) || ! strcmp(type, "A"))
    return(ASCII_PAGE_WID);
  if (! strcasecmp(type, "Ascii"      )) return(ASCII_WID);
  if (! strcasecmp(type, "Command"    ) || ! strcmp(type, "C"))
    return(COMMAND_WID);
  if (! strcasecmp(type, "Image"      ) || ! strcmp(type, "I"))
    return(IMG_WID);
  if (! strcasecmp(type, "Label"      ) || ! strcmp(type, "L"))
    return(LABEL_WID);
  if (! strcasecmp(type, "List"       )) return(LIST_WID);
  if (! strcasecmp(type, "Menu"       ) || ! strcmp(type, "M"))
    return(MENU_WID);
  if (! strcasecmp(type, "Overlay"    ) || ! strcmp(type, "O"))
    return(OVERLAY_WID);
  if (! strcasecmp(type, "Prompt"     )) return(PROMPT_WID);
  if (! strcasecmp(type, "Scroll"     ) || ! strcmp(type, "S"))
    return(SCROLL_WID);
  if (! strcasecmp(type, "Toggle"     ) || ! strcmp(type, "T"))
    return(TOGGLE_WID);

  return(Error(2, "parse_widget_type: Unknown widget type: %s.\n", type));

} /* parse_widget_type() */



#ifndef FUNCPROTO
char *ximage_usage_text()
#else /* FUNCPROTO */
char *ximage_usage_text(void)
#endif /* FUNCPROTO */
{
  static char t[] =
"  Options from the ximage toolkit: \n\
    -adjustwidth            : Automatically adjust width of control panel \n\
                              widgets. \n\
    +adjustwidth            : Reverse of -adjustwidth. \n\
    -aspect                 : Don't keep image aspect ratio when resizing. \n\
    -ct filename            : Get initial image colortable from filename. \n\
    -colortable filename    : Same as -ct. \n\
    -cl filename            : Get colortable list from filename. \n\
    -colorlist filename     : Same as -cl. \n\
    -colmenu, -colorsmenu   : Include Colors menu in image windows. \n\
    +colmenu, +colorsmenu   : Reverse of -colmenu. \n\
    -full                   : Do not use 'reduced color' display of images. \n\
    -ih height              : Height of image windows. \n\
    -imageheight height     : Same as -ih. \n\
    -iw width               : Width of image windows. \n\
    -imagewidth width       : Same as -iw. \n\
    -iv VisualName          : Use VisualName visual class for all \n\
                              image-related windows. \n\
    -layout <type>          : <type> can be \"separate\", \"together\" or \n\
                              \"check\". Handles whether images are \n\
                              separated from control panel (governs image \n\
                              resizing and existence of menubar). \n\
    -logpos                 : Turn 'log position' mode on. \n\
    -mb, -menubar           : Do not include menubar in image windows.\n\
    -multivisual            : Enable display of images for all available \n\
                              visual classes, via the Image visuals menu. \n\
    +multivisual            : Reverse of -multivisual. \n\
    -optionsmenu            : Include Options menu in image windows. \n\
    +optionsmenu            : Reverse of -optionsmenu. \n\
    -ovt filename           : Get initial overlay colortable from filename. \n\
    -overlaytable filename  : Same as -ovt. \n\
    -ovlmenu, -overlaysmenu : Include OverlayColors menu in image windows. \n\
    +ovlmenu, +overlaysmenu : Reverse of -ovlmenu. \n\
    -panelcontainer <type>  : <type> must be \"Box\" or \"Form\". \n\
    -protect                : Set 'protect' mode on for overlays. \n\
    -rgb                    : Interpret 3-band images as rgb. \n\
    -share                  : Let textual windows share the \"Work-map\" \n\
                              colormap with images. \n\
    -toolsmenu              : Include Tools menu in image windows. \n\
    +toolsmenu              : Reverse of -toolsmenu. \n\
    -version                : Print XITE version number and exit. \n\
    -widgetset <type>       : <type> must be \"Xaw\" or \"Xfwf\". \n\
    -zoomall                : Turn 'zoom all' mode on. \n\
    -visualsmenu            : Include Visuals menu in image windows. \n\
    +visualsmenu            : Reverse of -visualsmenu. \n";

  return(t);
                    
} /* ximage_usage_text() */

#ifndef FUNCPROTO
char *ximage_app_usage_text(app_usage)
char *app_usage;
#else /* FUNCPROTO */
char *ximage_app_usage_text(char *app_usage)
#endif /* FUNCPROTO */
{
  char *ximage_usage, *standard_usage, *full_usage;

  ximage_usage   = ximage_usage_text();
  standard_usage = xite_standard_options_usage_text();
  full_usage     = (char *) malloc(strlen(app_usage) + strlen(ximage_usage) +
				   strlen(standard_usage) + 1);
  (void) strcpy(full_usage, app_usage);
  (void) strcat(full_usage, ximage_usage);
  (void) strcat(full_usage, standard_usage);

  return(full_usage);

} /* ximage_app_usage_text */

#ifndef FUNCPROTO
void set_init_color(color)
int color;
#else /* FUNCPROTO */
void set_init_color(int color)
#endif /* FUNCPROTO */
{
  InitialColorTab_e = color;

} /* set_init_color() */

#ifndef FUNCPROTO
void set_init_ovl_color(color)
int color;
#else /* FUNCPROTO */
void set_init_ovl_color(int color)
#endif /* FUNCPROTO */
{
  InitialOvlTab_e = color;

} /* set_init_ovl_color() */
 


#ifndef FUNCPROTO
int ximage_info(wid)
Widget wid;
#else /* FUNCPROTO */
int ximage_info(Widget wid)
#endif /* FUNCPROTO */
{
  Widget GV, shell;
  int width, height, xstart, ystart, xpan, ypan, depth = -1;
  display_method disp_method;
  _Info *info;
  char *text = NULL, *title, *pixname, *mess, *mess0, visual[30];
  IMAGE img;
  float minval = 0, maxval = 0, zoom;
  Boolean rgb, complex = False;

  GV    = wid ? wid : PopupWidget();
  shell = TopLevelWidget(GV);

  XtVaGetValues(GV, XtNimageImage,    &img,
		    XtNimageTitle,    &title,
		    XtNimageWidth,    &width,
		    XtNimageHeight,   &height,
		    XtNimageXstart,   &xstart,
		    XtNimageYstart,   &ystart,
		    XtNimagePixtype,  &pixname,
		    XtNdisplayMethod, &disp_method,
		    XtNimageRgb,      &rgb, NULL);

  if (pixname && (!strcmp(pixname, ImageComplexPixel) ||
      !strcmp(pixname, ImageDoubleComplexPixel)))
    complex = True;

  if (pixname && !complex)
    XtVaGetValues(GV, XtNminval,        &minval,
		      XtNmaxval,        &maxval, NULL);

  if (!pixname) pixname = XtNewString("Unknown");

  ImageGetZoom((ImageWidget) GV, &zoom, &xpan, &ypan, NULL);

  (void) strcpy(visual, "Not supported by XITE.");
  switch(disp_method) {
  case PSEUDO_COLOR_8_A:
  case PSEUDO_COLOR_8_B:
    (void) strcpy(visual, "PseudoColor");
    depth  = 8;
    break;
  case TRUE_COLOR_24:
    (void) strcpy(visual, "TrueColor");
    depth  = 24;
    break;
  case DIRECT_COLOR_24_A:
  case DIRECT_COLOR_24_B:
    (void) strcpy(visual, "DirectColor");
    depth  = 24;
    break;
  default:
    break;
  } /* switch */

  if (img) {
    info = (_Info *) img[0];
    if (info->text && info->nchars > 0) {
      text = (char *) malloc((info->nchars + 1) * sizeof(char));
      (void) strncpy(text, &((info->text)[1]), info->nchars);
      text[info->nchars] = '\0';
    }
  }
  mess0 = text ? (char *) malloc(sizeof(char) * (strlen(text) + 635)) :
    (char *) malloc(sizeof(char) * 635);
  mess = mess0;

  /* BIFF image-specifics */
  sprintf(mess, "\n%s%s\n%s%s\n%s%d%s%d%s\n%s%d%s%d%s\n",
	  "Image name:      ", title,
	  "Pixeltype:       ", pixname,
	  "Image size:      (", width, " x ", height, ")",
	  "Image start:     (", xstart, ", ", ystart, ")");


  /* Display specifics */
  mess = &(mess[strlen(mess)]);
  sprintf(mess, "%s%d%s%d%s\n%s%f\n",
	  "Panning:         (", xpan, ", ", ypan, ")",
	  "Zoom:            ", zoom);
  mess = &(mess[strlen(mess)]);

  if (!complex)
    sprintf(mess, "%s%f\n%s%f\n",
	    "Assumed minimum: ", minval,
	    "Assumed maximum: ", maxval);

  mess = &(mess[strlen(mess)]);
  sprintf(mess, "%s%s\n%s%d\n",
	  "Visual class:    ", visual,
	  "Visual depth:    ", depth);

  mess = &(mess[strlen(mess)]);
  sprintf(mess, rgb ? "Three-band rgb display.\n" : "Single-band display.\n");

  mess = &(mess[strlen(mess)]);
  if (text) sprintf(mess, "%s\n%s\n%s\n",
		    "----------------------------",
		    "Textfield (history):",
		    text);

  XtPopup(TextBox(shell, mess0, "Close", "Image information", "Image info"),
	  XtGrabNone);

  free(mess0);

  return(0);

} /* ximage_info() */

#ifndef FUNCPROTO
int ximage_histogram(wid, cb_data)
Widget wid;
image_disp_cb_data *cb_data;
#else /* FUNCPROTO */
int ximage_histogram(Widget wid, image_disp_cb_data *cb_data)
#endif /* FUNCPROTO */
{
  IMAGE img;
  IBAND band;
  int planes;
  Colormap newmap;
  Visual *visual;
  ImageWidget GV;
  char *pixname;

  GV = wid ? (ImageWidget) wid : (ImageWidget) PopupWidget();

  XtVaGetValues((Widget) GV, XtNvisual,       &visual,
		             XtNimagePixtype, &pixname, NULL);

  if (pixname && (!strcmp(pixname, ImageComplexPixel) ||
		  !strcmp(pixname, ImageDoubleComplexPixel))) {
    return(Error(2, "ximage_histogram(): Pixeltype can not be complex.\n"));
  }
    
  planes = DepthToReducedPlanes(DepthOfVisual(visual), visual);

  if (ImageHasColormap(GV)) {
    char *title, *entryName;

    XtVaGetValues((Widget) GV, XtNimageTitle, &title, NULL);
    entryName = XtMalloc(strlen("Histogram ") + strlen(title) + 1);
    (void) strcpy(entryName, "Histogram ");
    (void) strcat(entryName, title);

    AddColormapCopy((Widget) GV, visual, entryName);

    /* Set the newly added colormap for the image widget. */
    Color_set(GV, Color_max());

    /* Return the reduced-color version of the newly added colormap. */
    newmap = Color_map2(Color_max(), 128, visual);
  } else {

    /* Something else should probably be done here in the case of
     * a TrueColor visual.
     */
    newmap = cmapOfVisual(visual, planes);
  }

  /* 'newmap' may be zero at this point, if the Color_map2 call above
   * returned zero. Otherwise, 'newmap' and 'planes' are both non-zero.
   */
  XtVaGetValues((Widget) GV, XtNimageImage, &img,
                             XtNimageBand,  &band, NULL);

  xhistogram((Widget) GV, img, band, newmap, planes,
	     cb_data->display_func, cb_data->callbacks);

  return(0);

} /* ximage_histogram() */

#ifndef FUNCPROTO
int ximage_slice(wid)
Widget wid;
#else /* FUNCPROTO */
int ximage_slice(Widget wid)
#endif /* FUNCPROTO */
{
  IMAGE img;
  int planes;
  Colormap newmap;
  Visual *visual;
  ImageWidget GV;
  char *title, *entryName, *pixname;

  GV = wid ? (ImageWidget) wid : (ImageWidget) PopupWidget();

  if (!ImageHasColormap(GV)) {
    Warning(1, "%s\n%s\n%s\n%s\n",
    "ximage_slice: You must display the image with a visual with changeable",
    "    colormap to get access to the slice application.",
    "    Check out the visuals menu, application option or resource for a",
    "    choice of visual.");

    return(1);
  }

  XtVaGetValues((Widget) GV, XtNimageTitle,   &title,
		             XtNimagePixtype, &pixname, NULL);
  if (pixname && (!strcmp(pixname, ImageComplexPixel) ||
		  !strcmp(pixname, ImageDoubleComplexPixel))) {
    return(Error(2, "ximage_slice(): Pixeltype can not be complex.\n"));
  }


  entryName = XtMalloc(strlen("Slice ") + strlen(title) + 1);
  (void) strcpy(entryName, "Slice ");
  (void) strcat(entryName, title);

  XtVaGetValues((Widget) GV, XtNvisual, &visual, NULL);
  AddColormapCopy((Widget) GV, visual, entryName);

  /* Wait until inside the xslice() function to set the newly added
   * colormap for the image widget and its toplevel widget.
   */

  /* Return the reduced-color version of the newly added colormap. */
  newmap = Color_map2(Color_max(), 128, visual);

  /* Set known colors in some cells. */
  ColormapStd(dpy_e, screen_s, newmap, ClassOfVisual(visual), 1);

  planes = DepthToReducedPlanes(DepthOfVisual(visual), visual);
  XtVaGetValues((Widget) GV, XtNimageImage, &img, NULL);

  xslice((Widget) GV, img, newmap, planes); 

  return(0);

} /* ximage_slice() */

#ifndef FUNCPROTO
int ximage_colorbar(wid, cb_data)
Widget wid;
image_disp_cb_data *cb_data;
#else /* FUNCPROTO */
int ximage_colorbar(Widget wid, image_disp_cb_data *cb_data)
#endif /* FUNCPROTO */
{
  Visual *visual;
  Widget GV, bar;
  char *title, *new_title, *return_text;
  int colortab, status, xsize = 512, ysize = 50, planes;

  GV = wid ? wid : PopupWidget();

  XtVaGetValues(GV, XtNvisual,     &visual,
		    XtNcolortab,   &colortab, NULL);

  planes = DepthOfVisual(visual);
  title  = XtNewString(ColortabName(colortab));

  if (title) {
    new_title = (char *) malloc(strlen(title) + 1 + strlen("colorbar") + 1);
    (void) strcpy(new_title, title);
    (void) strcat(new_title, " colorbar");
  } else {
    new_title = (char *) malloc(strlen("Colorbar") + 1);
    (void) strcpy(new_title, "Colorbar");
  }

  status = FormDialog(TopLevelWidget(GV), " ",
  " # Horizontal size # # d # 512 # Vertical size   # # d # 50",
		      &return_text, NULL, 3,
		      "Colorbar dialog", "Colorbar dialog");

  if (status != 1) return(-1);

  if (return_text) {
    (void) sscanf(return_text, "%d%d", &xsize, &ysize);
  }

  bar = colorbar_widget(TopLevelWidget(GV), "colorbar", "colorbarShell",
			colortab, xsize, ysize, 0, 255, visual, planes,
			new_title, cb_data ? cb_data->callbacks : NULL);

  XtPopup(TopLevelWidget(bar), XtGrabNone);

  return(0);

} /* ximage_colorbar() */

/*L:ResourceVersion*

________________________________________________________________

                ResourceVersion
________________________________________________________________

Name:           ResourceVersion
Syntax:         | #include <xite/Xcolor.h>
                |
		| float ResourceVersion( char *version );
		|
Description:    Return minimum required version number of XITE X resources
                for the specified XITE software 'version' number.

                Software 'version' may be changed without a change in X
                resources.

Files:          src/biff.h

Restrictions:   Software version may be one of the following kinds
                | integer (e.g. 3) (type 1)
                | float   (e.g. 3.0, 3.1, 3.01) (type 2)
                | float with trailing letter (3.1a, 3.01a) (type 3)

Return value:   minimum required XITE X resources version number

Author:         Svein Bøe

Id:             $Id$
________________________________________________________________

*/

#ifndef FUNCPROTO
static float ResourceVersion(version)
char *version;
#else /* FUNCPROTO */
static float ResourceVersion(char *version)
#endif /* FUNCPROTO */
{
  double dv;
  float  rv;
  int    iv, sv;
  char   *ptr = "dummy", a;

  /* Disassemble software version number. */

  dv = strtod(version, &ptr);
  if (ptr == version) {
    /* Found no number. */
    Error(2, "ResourceVersion: No version number found.\n");
    return(0);
  } else {
    /* Found version number. */

    iv = (int) dv;

    if (iv == dv) {
      /* Integer version number (type 1). */
      sv  = 0;
      a   = '\0';
    } else if (*ptr == '\0') {
      /* Float version number without trailing letters. */

      sv  = (int) (100 * (dv - iv) + 0.5);
      a   = '\0';
    } else {
      /* Float version number with trailing letter. */

      sv  = (int) (100 * (dv - iv) + 0.5);
      a   = *ptr;
    }
  } /* Version number found. */

  /* Manually set required version for X resources.
   * Software versions may be changed without a change in X resources.
   */

  switch (iv) {
  case 0:
  case 1:
  case 2: rv = 3.0; break;

  case 3:
    if (sv < 9) rv = 3.0;
    else if (sv == 10) rv = 3.1;
    else if (sv == 20) rv = 3.2;
    else if (sv == 30) rv = 3.3;
    else if (sv == 40) rv = 3.4;
    else if (sv == 41) rv = 3.4;
    else if (sv == 42) rv = 3.4;
    else if (sv == 43) rv = 3.4;
    else if (sv == 44) rv = 3.4;
    break;
  } /* switch(v) */

  return(rv);

} /* ResourceVersion() */


#ifndef FUNCPROTO
static void close_shell(wid, event, params, num_params)
Widget wid;
XEvent *event;
String *params;
Cardinal *num_params;
#else /* FUNCPROTO */
static void close_shell(Widget wid, XEvent *event, String *params, Cardinal *num_params)
#endif /* FUNCPROTO */
{
  ENTER_FUNCTION_DEBUG("ximage.c: close_shell");

  XtDestroyWidget(TopLevelWidget(wid));

  LEAVE_FUNCTION_DEBUG("ximage.c: close_shell");

} /* close_shell() */

#ifndef FUNCPROTO
static void processXimageAppResources()
#else /* FUNCPROTO */
static void processXimageAppResources(void)
#endif /* FUNCPROTO */
{
  if (appResResult_s.version) {
    Message(1, "XITE version: %s\n", XITE_VERSION);
    exit(1);
  }
  zoomall_e          = appResResult_s.zoomall;
  rgb_e              = appResResult_s.rgb_images;
  share_e            = appResResult_s.share;
  logpos_e           = appResResult_s.logpos;
  adjust_width_s     = appResResult_s.adjust_width;
  xiteHelper_e       = appResResult_s.xiteHelper;
  xiteHelperOption_e = appResResult_s.xiteHelperOption;
  imageVisual_s      = appResResult_s.imageVisual;
  colortabfile_e     = appResResult_s.coltab;
  ovltabfile_e       = appResResult_s.ovltab;
  colorlistfile_e    = appResResult_s.colorList;
  image_menus_e     |= appResResult_s.colorsmenu   ? COLORS_BUTTON   : 0;
  image_menus_e     |= appResResult_s.overlaysmenu ? OVERLAYS_BUTTON : 0;
  image_menus_e     |= appResResult_s.toolsmenu    ? TOOLS_BUTTON    : 0;
  image_menus_e     |= appResResult_s.optionsmenu  ? OPTIONS_BUTTON  : 0;
  image_menus_e     |= appResResult_s.visualsmenu  ? VISUALS_BUTTON  : 0;
  multi_visual_e     = appResResult_s.multivisual;

  if (multi_visual_e) image_menus_e |= VISUALS_BUTTON;

  if (!appResResult_s.layout) layout_s = UNKNOWN_SEPARATE;
  else if (!strcasecmp(appResResult_s.layout, "separate"))
    layout_s = SEPARATE;
  else if (!strcasecmp(appResResult_s.layout, "together"))
    layout_s = TOGETHER;
  else if (!strcasecmp(appResResult_s.layout, "check"))
    layout_s = CHECK_PANEL_RES;
  else Usage(2, "Unknown value for layout: %s.\n", appResResult_s.layout);

  if (!appResResult_s.panel_container) panel_container_s = UNKNOWN_CONTAINER;
  else if (!strcasecmp(appResResult_s.panel_container, "Box"))
    panel_container_s = BOX_CONTAINER;
  else if (!strcasecmp(appResResult_s.panel_container, "Form"))
    panel_container_s = FORM_CONTAINER;
  else Usage(2, "Unknown value for panelcontainer: %s.\n",
	     appResResult_s.panel_container);

  if (!appResResult_s.widget_set) widget_set_s = UNKNOWN_WIDGET_SET;
  else if (!strcasecmp(appResResult_s.widget_set, "Xfwf"))
    widget_set_s = XFWF_WIDGETS;
  else if (!strcasecmp(appResResult_s.widget_set, "Xaw"))
    widget_set_s = XAW_WIDGETS;
  else Usage(2, "Unknown value for widgetset: %s.\n",
	     appResResult_s.widget_set);

  if (!colorlistfile_e)
    colorlistfile_e = envfindfile("XSHOWPATH", "xshow_colortabs");

} /* processXimageAppResources() */

#ifndef FUNCPROTO
Widget ximageInit(argc, argv, classname, resources, num_resources, resources_result, optionsTable, num_options, fallback_resources, warn)
int *argc;
char **argv, *classname;
XrmOptionDescRec *optionsTable;
XtPointer resources_result;
String fallback_resources[];
XtResource *resources;
int warn, num_resources, num_options;
#else /* FUNCPROTO */
Widget ximageInit(int *argc, char **argv, char *classname, XtResource *resources, int num_resources, XtPointer resources_result, XrmOptionDescRec *optionsTable, int num_options, String *fallback_resources, int warn)
#endif /* FUNCPROTO */
{
  ENTER_FUNCTION_DEBUG("ximage.c: ximageInit");

  if (!optionsTable) {
    optionsTable = opTable_s;
    num_options  = XtNumber(opTable_s);
  }

  /* Start the application and read the app-defaults file and command-line
   * options. */
  Control_e = XtAppInitialize(&appcon_s, classname, 
			      optionsTable, num_options,
			      argc, argv, fallback_resources,
			      NULL, 0);

  dpy_e      = XtDisplay(Control_e);
  screen_s   = DefaultScreen(dpy_e);
  
  /* First read the standard 'ximage' application resources. */
  XtGetApplicationResources(Control_e, &appResResult_s, 
			    resources_s, XtNumber(resources_s),
			    NULL, 0);

  processXimageAppResources();

  if (warn) {
    if ((int) (100 * appResResult_s.xiteResourceVersion + 0.5) <
	(int) (100 * ResourceVersion(XITE_VERSION) + 0.5)) {
      Warning(1, "ximageInit:\n    %s\n    %s\n\n",
      "The version of the X application defaults file is not synchronized",
      "with the version of XITE. This may cause strange behaviour.");
    }
  }

  if (resources) {
    /* Read additional non-ximage specific application resources. */
    XtGetApplicationResources(Control_e, resources_result,
			      resources, num_resources,
			      NULL, 0);
  }
    
  /* Prepare for acceptance of window deletion by window manager. */
  wm_delete_window = XInternAtom(dpy_e, "WM_DELETE_WINDOW", False);
  
#ifndef MOTIF

  AddAction(Control_e, "PopupMenu",   PopupMenu,        1); 
  AddAction(Control_e, "PopupExit",   PopupExit,        0);
  AddAction(Control_e, "CloseShell",  close_shell,      0);

#endif

  InitializeVisual(dpy_e, screen_s, imageVisual_s);

  /* Now everything has been initialized. Destroy the application shell
   * and create a new one with the XITE function CreateAppShell(). This
   * will give the toplevel application shell the correct resources, as
   * determined by the initialization steps just completed.
   */
  XtDestroyWidget(Control_e);
  Control_e = toplevel_s = CreateAppShell(dpy_e, NULL, classname);

  if (zoomall_e) ImageSetZoomAll(toplevel_s, 1);

  LEAVE_FUNCTION_DEBUG("ximage.c: ximageInit");
  return(toplevel_s);

} /* ximageInit() */



#ifndef FUNCPROTO
Widget ximage_init(argc, argv, classname, fallback_resources, warn)
int *argc;
char **argv, *classname;
String fallback_resources[];
int warn;
#else /* FUNCPROTO */
Widget ximage_init(int *argc, char **argv, char *classname, String *fallback_resources, int warn)
#endif /* FUNCPROTO */
{
  Widget wid;

  ENTER_FUNCTION_DEBUG("ximage.c: ximage_init");

  wid = ximageInit(argc, argv, classname,
		   NULL, 0, NULL, /* res., num_res., resources_result */
		   NULL, 0,       /* optionsTable, num_options */
		   fallback_resources, warn);

  LEAVE_FUNCTION_DEBUG("ximage.c: ximage_init");
  return(wid);

} /* ximage_init() */



#ifndef FUNCPROTO
static void prepare_callbacks(callbacks_return, img_callbacks)
image_callbacks *callbacks_return;
image_callbacks *img_callbacks;
#else /* FUNCPROTO */
static void prepare_callbacks(image_callbacks *callbacks_return, image_callbacks *img_callbacks)
#endif /* FUNCPROTO */
{
  image_callbacks *cb = callbacks_return;
  XtCallbackList cb_list;
  char name[80];

  /* Add LogPositionCallback in front of XtNcallback list. */

  if (img_callbacks) {
    cb->draw_cb    = img_callbacks->draw_cb;
    cb->roi_cb     = img_callbacks->roi_cb;
    cb->resize_cb  = img_callbacks->resize_cb;
    cb->destroy_cb = img_callbacks->destroy_cb;
  } else {
    cb->draw_cb    = add_callback_first(cb->draw_cb,    NULL, NULL);
    cb->roi_cb     = add_callback_first(cb->roi_cb,     NULL, NULL);
    cb->resize_cb  = add_callback_first(cb->resize_cb,  NULL, NULL);
    cb->destroy_cb = add_callback_first(cb->destroy_cb, NULL, NULL);
  }

  /*
  cb_list = img_callbacks ? img_callbacks->destroy_cb : NULL;
  cb->destroy_cb = add_callback_first(cb_list, ximage_exit, NULL);
  */

  if (PosWid_s) {
    cb_list = img_callbacks ? img_callbacks->callback_cb : NULL;
    cb->callback_cb =
      add_callback_first(cb_list, LogPositionCallback,
                         (XtPointer) XtNameToWidget(PosWid_s,
                                                    strcat(strcpy(name, "*"),
                                                           "label")));
  }

  return;

} /* prepare_callbacks() */

#ifndef FUNCPROTO
static Widget ximage_image(parent, lout)
Widget parent;
layout lout;
#else /* FUNCPROTO */
static Widget ximage_image(Widget parent, layout lout)
#endif /* FUNCPROTO */
{
  Widget image_wid = NULL;
  WidgetClass widgetClass = NULL;
  char *wid_type, *resource_name, *shell_resource_name = NULL;
  widget_type wid_typ;
  static image_callbacks callbacks;
  Boolean new_window;

  if (!parent) {
    LEAVE_FUNCTION_DEBUG("ximage.c: ximage_image");
    Error(2, "ximage_image: No parent in ximage_image.\n");
  }

  wid_type            = TypeOfLayout(lout);
  resource_name       = ResourceNameOfLayout(lout);
  shell_resource_name = ShellResourceNameOfLayout(lout);

  new_window = XtIsSubclass(parent, shellWidgetClass);

  if (!PosWid_s) {
    char *mess;

    mess = "wwwwwwwwwwwwwwwwwwwwwwwwwwwwww";
    PosWid_s = TextBox(toplevel_s, mess, "Close",
		       "Cursor position", "Curs. pos.");
  }

  prepare_callbacks(&callbacks, CallbacksOfImageLayout(lout));

  wid_typ = parse_widget_type(wid_type);
  switch(wid_typ)
    {
    case IMG_WID:
      widgetClass = imageWidgetClass;
      break;
    case OVERLAY_WID:
      widgetClass = imageOverlayWidgetClass;
      break;
    default:
      break;
    }

  if (widgetClass) {
    if (!new_window && !XtIsSubclass(parent, imageFormWidgetClass) &&
	!XtIsSubclass(parent, boxWidgetClass))
      Warning(2, "%s%s\n",
	      "ximage_image: Image widget parent should be class ImageForm ",
	      "or class Box");

    image_wid = ximage_display(parent, "image", shell_resource_name,
			       ImgOfImageLayout(lout), 0,
			       widgetClass, NULL, 0,
			       image_menus_e, NULL, 0, &callbacks);
  }

  /* image_wid was realized in ximage_display if parent is a shell, but it
   * has not been popped up. */

  if (!image_wid) return(NULL);

  /* Popup if image_wid is a realized shell, i.e. if parent is a shell. */
  if (new_window) XtPopup(TopLevelWidget(image_wid), XtGrabNone);

  LEAVE_FUNCTION_DEBUG("ximage.c: ximage_image");

  return(image_wid);

} /* ximage_image() */

typedef enum {
  FWF_FINE,
  FWF_TOO_NARROW,
  FWF_TOO_WIDE,
  FWF_TOO_TALL,
  FWF_TOO_LOW
} fwf_size_t;

#ifndef FUNCPROTO
static fwf_size_t check_fwf_geometry(wid)
Widget wid;
#else /* FUNCPROTO */
static fwf_size_t check_fwf_geometry(Widget wid)
#endif /* FUNCPROTO */
{
  Dimension width, height, top_margin, bottom_margin, left_margin,
	    right_margin, frame_width, inner_offset, outer_offset;
  fwf_size_t result = FWF_FINE;

  XtVaGetValues(wid, XtNtopMargin,    &top_margin,
		     XtNbottomMargin, &bottom_margin,
		     XtNleftMargin,   &left_margin,
		     XtNrightMargin,  &right_margin,
		     XtNframeWidth,   &frame_width,
		     XtNinnerOffset,  &inner_offset,
		     XtNouterOffset,  &outer_offset,
		     XtNwidth,        &width,
		     XtNheight,       &height,
		     NULL);

  /* 12349 (MAGICNUM) is initial value for width/height attr. of fwf widgets */
  if (width  > 10000) result = FWF_TOO_WIDE;
  else if (height > 10000) result = FWF_TOO_TALL;
  else if (width <= left_margin + right_margin + 2 * frame_width +
      inner_offset + outer_offset) result = FWF_TOO_NARROW;
  else if (height <= top_margin + bottom_margin + 2 * frame_width +
      inner_offset + outer_offset) result = FWF_TOO_TALL;

  return(result);

} /* check_fwf_geometry() */

#ifndef FUNCPROTO
static Widget check_fwf_widget(wid)
Widget wid;
#else /* FUNCPROTO */
static Widget check_fwf_widget(Widget wid)
#endif /* FUNCPROTO */
{
  char *lab;
  XFontStruct *fontinfo;
  fwf_size_t fwf_size;
  Dimension top_margin, bottom_margin, frame_width, inner_offset, outer_offset;

  XtVaGetValues(wid, XtNlabel,        &lab,
		     XtNfont,         &fontinfo,
		     XtNtopMargin,    &top_margin,
		     XtNbottomMargin, &bottom_margin,
		     XtNframeWidth,   &frame_width,
		     XtNinnerOffset,  &inner_offset,
		     XtNouterOffset,  &outer_offset,
		     NULL);


  if ((fwf_size = check_fwf_geometry(wid)) != FWF_FINE) {
    Dimension ch, cw;
    Widget wid2;

    /* Not using the font info yet, perhaps in a later version. */
    cw = fontinfo->max_bounds.width;
    ch = fontinfo->max_bounds.ascent + fontinfo->max_bounds.descent;

    wid2 = wid;

    switch(fwf_size) {
    case FWF_TOO_TALL:
      wid = CVW(XtName(wid2), XtClass(wid2), XtParent(wid2),
		XtNheight, (Dimension) (1.2 * ch) + top_margin +
		           bottom_margin + inner_offset + outer_offset +
		           2 * frame_width, NULL);
      break;
    case FWF_TOO_WIDE:
    case FWF_TOO_NARROW:
    case FWF_TOO_LOW:
      if (lab)
	wid = CVW(XtName(wid2), XtClass(wid2), XtParent(wid2),
		  XtNshrinkToFit, "True", NULL);
      else
	wid = CVW(XtName(wid2), XtClass(wid2), XtParent(wid2),
		  XtNlabel,       XtName(wid2),
		  XtNshrinkToFit, "True",   NULL);
      break;
    } /* switch */

    XtDestroyWidget(wid2);
  } else {
    if (!lab) XtVaSetValues(wid, XtNlabel, XtName(wid), NULL);
    XtManageChild(wid);
  }

  return(wid);

} /* check_fwf_widget() */

static char *prompt_translations =
  "#override \n\
  Ctrl<Key>b: unselect() left()  \n\
  Ctrl<Key>f: unselect() right() \n\
  <Key>Delete: cut() backspace()";

#ifndef FUNCPROTO
static Widget check_fwf_prompt_widget(wid)
Widget wid;
#else /* FUNCPROTO */
static Widget check_fwf_prompt_widget(Widget wid)
#endif /* FUNCPROTO */
{
  XFontStruct *fontinfo;
  fwf_size_t fwf_size;
  char *text;
  Dimension top_margin, bottom_margin, frame_width, inner_offset, outer_offset;

  XtVaGetValues(wid, XtNtext,         &text,
		     XtNfont,         &fontinfo,
		     XtNtopMargin,    &top_margin,
		     XtNbottomMargin, &bottom_margin,
		     XtNframeWidth,   &frame_width,
		     XtNinnerOffset,  &inner_offset,
		     XtNouterOffset,  &outer_offset,
		     NULL);


  if ((fwf_size = check_fwf_geometry(wid)) != FWF_FINE) {
    Dimension ch, cw;
    Widget wid2;

    /* Not using the font info yet, perhaps in a later version. */
    cw = fontinfo->max_bounds.width;
    ch = fontinfo->max_bounds.ascent + fontinfo->max_bounds.descent;

    wid2 = wid;

    switch(fwf_size) {
    case FWF_TOO_TALL:
      wid = CVW(XtName(wid2), XtClass(wid2), XtParent(wid2),
		XtNheight, (Dimension) (1.2 * ch) + top_margin +
		           bottom_margin + inner_offset + outer_offset +
		           2 * frame_width, NULL);
      break;
    case FWF_TOO_WIDE:
    case FWF_TOO_NARROW:
    case FWF_TOO_LOW:
      if (text)
	wid = CVW(XtName(wid2), XtClass(wid2), XtParent(wid2),
		  XtNtext, text,
		  XtNtraversalOn, "off",
		  XtNtranslations,
		  XtParseTranslationTable(prompt_translations),
		  XtNshrinkToFit, "True", NULL);
      else
	wid = CVW(XtName(wid2), XtClass(wid2), XtParent(wid2),
		  XtNtraversalOn, "off",
		  XtNtranslations,
		  XtParseTranslationTable(prompt_translations),
		  XtNshrinkToFit, "True",   NULL);
      break;
    } /* switch */

    XtDestroyWidget(wid2);
  } else {
    XtManageChild(wid);
  }

  return(wid);

} /* check_fwf_prompt_widget() */


#ifndef FUNCPROTO
static Widget ximage_panel(parent, containerWidgetClass, layouts, first_wid, nwids, num_children, widget_set)
Widget parent;
WidgetClass containerWidgetClass;
layout *layouts;
int first_wid, nwids;
Cardinal *num_children;
widget_set_type widget_set;
#else /* FUNCPROTO */
static Widget ximage_panel(Widget parent, WidgetClass containerWidgetClass, layout *layouts, int first_wid, int nwids, Cardinal *num_children, widget_set_type widget_set)
#endif /* FUNCPROTO */
{
  Widget top_form, wid;
  layout lout;
  int i;
  Dimension width = 0, mwidth = 0;
  char *wid_type = NULL, *wid_name = NULL, *resource_name = NULL;
  Boolean do_realize = False;
  widget_type wid_typ;

  /* Remove translations which control multi-line and search
   * behaviour.
   */
  static char *text_box_translations =
    "#override\n\
    <Key>Return: returnok()\n\
    Ctrl<Key>J: no-op()\n\
    Ctrl<Key>M: no-op()\n\
    Ctrl<Key>N: no-op()\n\
    Ctrl<Key>O: no-op()\n\
    Ctrl<Key>P: no-op()\n\
    Ctrl<Key>R: no-op()\n\
    Ctrl<Key>S: no-op()\n\
    Ctrl<Key>V: no-op()\n\
    Ctrl<Key>Z: no-op()\n\
    Meta<Key>I: no-op()\n\
    Meta<Key>Q: no-op()\n\
    Meta<Key>V: no-op()\n\
    Meta<Key>Z: no-op()\n\
    <Key>Down: no-op()\n\
    <Key>Up: no-op()\n\
    <Key>Linefeed: no-op()";

  ENTER_FUNCTION_DEBUG("ximage.c: ximage_panel");

  if (!parent) {
    LEAVE_FUNCTION_DEBUG("ximage.c: ximage_panel");
    Error(2, "ximage_panel: No parent in ximage_panel.\n");
  }

  resource_name = ShellResourceNameOfLayout(layouts[first_wid]);

  if (XtIsSubclass(parent, shellWidgetClass)) {
    if (resource_name) {
      parent = (Widget) CreatePopTopShell(resource_name, parent);
      do_realize = True;
    }

    if (nwids > 1)
      top_form = XtVaCreateManagedWidget("topContainer", containerWidgetClass,
				       parent, NULL);
    else
      top_form = parent;

  } else top_form = parent;

  (*num_children) = 0;
  for (i = first_wid; i < first_wid + nwids; i++) {
    lout = layouts[i];
    wid_type = TypeOfLayout(lout);
    if (!wid_type) {
      Error(2, "%s%s%d%s\n",
	    "ximage_panel: Missing widget type in specification of ",
	    "widget number ", i, ".");
    }

    wid_name = ResourceNameOfLayout(lout);
    if (!wid_name) {
      Error(2, "%s%s%d%s\n",
	    "ximage_panel: Missing widget name in specification of ",
	    "widget number ", i, ".");
    }

    wid_typ = parse_widget_type(wid_type);
    switch(wid_typ)
      {
      case ASCII_PAGE_WID: {
	XFontStruct *fontinfo;
	Dimension ch, cw;

	wid = CVW(wid_name, asciiTextWidgetClass, top_form, 
		  XtNscrollVertical, XawtextScrollAlways,
		  XtNtype, XawAsciiString,
		  XtNeditType, XawtextEdit,
		  NULL);
	XtVaGetValues(wid, XtNfont, &fontinfo, NULL);
	cw = fontinfo->max_bounds.width;
	ch = fontinfo->ascent + fontinfo->descent;
	XtVaSetValues(wid, XtNwidth, 80*cw, XtNheight, 24*ch+4, NULL);

	XtVaGetValues(wid, XtNwidth, &width, NULL);
	if (width > mwidth) mwidth = width;

	WidgetOfLayout(lout) = wid;
      }
	break;
      case ASCII_WID:
	wid = CVW(wid_name, asciiTextWidgetClass, top_form, 
		  XtNtype, XawAsciiString,
		  XtNeditType, XawtextEdit,
		  XtNtranslations,
		  XtParseTranslationTable(text_box_translations),
		  NULL);
	WidgetOfLayout(lout) = wid;
	break;
      case COMMAND_WID: {
	XtCallbackList cb_list = CallbackOfCommandLayout(lout);

	if (widget_set == XAW_WIDGETS)
	  wid = CVW(wid_name, commandWidgetClass, top_form, NULL);
	else {
	  wid = XtVaCreateWidget(wid_name, xfwfButtonWidgetClass, top_form,
				 NULL);
	  wid = check_fwf_widget(wid);
	}

	while (cb_list->callback != NULL) {
	  if (!cb_list->closure) cb_list->closure =
	    (XtPointer) MallocIntP(IdOfLayout(lout));
	  XtAddCallback(wid,
			widget_set == XAW_WIDGETS ? XtNcallback : XtNactivate,
			cb_list->callback, cb_list->closure);
	  cb_list++;
	}

	WidgetOfLayout(lout) = wid;
      }
	break;
      case LABEL_WID: {
	if (widget_set == XAW_WIDGETS) {
	  wid = CVW(wid_name, labelWidgetClass, top_form, NULL);
	} else {
	  wid = XtVaCreateWidget(wid_name, xfwfLabelWidgetClass, top_form,
				 NULL);
	  wid = check_fwf_widget(wid);
	}

	XtVaGetValues(wid, XtNwidth, &width, NULL);
	if (width>mwidth) mwidth = width;

	WidgetOfLayout(lout) = wid;
      }
	break;
      case LIST_WID: {
	layout_list lout_list = (layout_list) lout;
	layout_list_part *list_part = &(lout_list->list);
	XtCallbackList cb_list = CallbackOfListLayout(lout);

	wid = CVW(wid_name, listWidgetClass, top_form,
		  XtNnumberStrings, list_part->num_entries,
		  XtNlist,          list_part->list_entries, NULL);

	while (cb_list->callback != NULL) {
	  if (!cb_list->closure) cb_list->closure =
	    (XtPointer) MallocIntP(IdOfLayout(lout));
	  XtAddCallback(wid, XtNcallback, cb_list->callback, cb_list->closure);
	  cb_list++;
	}

	XtVaGetValues(wid, XtNwidth, &width, NULL);
	if (width > mwidth) mwidth = width;

	WidgetOfLayout(lout) = wid;
      }
	break;
      case MENU_WID: {
	char *name, *menu_name;
	Widget menu_popup;
	int entry_num;
	layout_menu lout_menu = (layout_menu) lout;
	layout_menu_part *menu_part = &(lout_menu->menu);
	layout_menu_entry menu_entry;
	XtCallbackList cb_list;

	menu_name  = XtNewString(wid_name);
	wid        = CVW(wid_name, menuButtonWidgetClass, top_form,
			 XtNmenuName, menu_name, NULL);
	menu_popup = PopupInit(menu_name, wid, NULL);

	/* Find string for the name of the first menubutton widget. */

	if (!menu_part->menu_entries) break;

	for (entry_num = 0; entry_num < menu_part->num_entries; entry_num++) {
	  menu_entry = menu_part->menu_entries[entry_num];
	  cb_list = CallbackOfMenuEntry(menu_entry);
	  name    = ResourceNameOfMenuEntry(menu_entry);
	  if (!name) break;

	  if (!cb_list->closure) cb_list->closure =
	    (XtPointer) MallocIntP(entry_num);

	  WidgetOfMenuEntry(menu_entry) =
	    Popup_add(XtNewString(name), menu_popup,
		      LabelOfMenuEntry(menu_entry),
		      cb_list->callback, cb_list->closure);

	  cb_list++;
	  while (cb_list->callback != NULL) {
	    if (!cb_list->closure) cb_list->closure =
	      (XtPointer) MallocIntP(entry_num);
	    XtAddCallback(WidgetOfMenuEntry(menu_entry),
			  XtNcallback, cb_list->callback, cb_list->closure);
	    cb_list++;
	  }
	}

	WidgetOfLayout(lout) = wid;
      }
	break;
      case PROMPT_WID: {
	XtCallbackList cb_list = CallbackOfPromptLayout(lout);
	fwf_size_t fwf_size;

	if (TextOfPromptLayout(lout))
	  wid = CVW(wid_name, xfwfPromptWidgetClass, top_form,
		    XtNtext, TextOfPromptLayout(lout),
		    XtNtraversalOn, "off",
		    XtNtranslations,
		    XtParseTranslationTable(prompt_translations),
		    NULL);
	else
	  wid = CVW(wid_name, xfwfPromptWidgetClass, top_form,
		    XtNtraversalOn, "off",
		    XtNtranslations,
		    XtParseTranslationTable(prompt_translations),
		    NULL);

	wid = check_fwf_prompt_widget(wid);

	while (cb_list->callback != NULL) {
	  if (!cb_list->closure) cb_list->closure =
	    (XtPointer) MallocIntP(IdOfLayout(lout));
	  XtAddCallback(wid, XtNactivate, cb_list->callback, cb_list->closure);
	  cb_list++;
	}

	WidgetOfLayout(lout) = wid;
      }
	break;
      case SCROLL_WID: {
	XtCallbackList jump_cb_list   = JumpCallbackOfScrollLayout(lout);
	XtCallbackList scroll_cb_list = ScrollCallbackOfScrollLayout(lout);

	wid = CVW(wid_name, scrollbarWidgetClass, top_form, NULL); 

	while (jump_cb_list->callback != NULL) {
	  /* Callback for smooth scrolling. */

	  if (!jump_cb_list->closure) jump_cb_list->closure =
	    (XtPointer) MallocIntP(IdOfLayout(lout));

	  XtAddCallback(wid, XtNjumpProc, jump_cb_list->callback,
			jump_cb_list->closure);
	  jump_cb_list++;
	}
	while (scroll_cb_list->callback) {
	  /* Callback for incremental scrolling. */

	  if (!scroll_cb_list->closure) scroll_cb_list->closure =
	    (XtPointer) MallocIntP(IdOfLayout(lout));

	  XtAddCallback(wid, XtNscrollProc, scroll_cb_list->callback,
			scroll_cb_list->closure);
	  scroll_cb_list++;
	}

	XtVaGetValues(wid, XtNwidth, &width, NULL);
	if (width>mwidth) mwidth = width;

	XawScrollbarSetThumb(wid, 0.5, 1.0);
	WidgetOfLayout(lout) = wid;
      }
	break;
      case TOGGLE_WID: {
	XtCallbackList cb_list = CallbackOfCommandLayout(lout);

	if (widget_set == XAW_WIDGETS)
	  wid = CVW(wid_name, toggleWidgetClass, top_form, NULL);
	else {
	  wid = XtVaCreateWidget(wid_name, xfwfToggleWidgetClass, top_form,
				 NULL);
	  wid = check_fwf_widget(wid);
	}

	while (cb_list->callback) {
	  if (!cb_list->closure)
	    cb_list->closure = (XtPointer) MallocIntP(IdOfLayout(lout));

	  if (widget_set == XAW_WIDGETS)
	    XtAddCallback(wid, XtNcallback,
			  cb_list->callback, cb_list->closure);
	  else {
	    XtAddCallback(wid, XtNonCallback,
			  cb_list->callback, cb_list->closure);
	    XtAddCallback(wid, XtNoffCallback,
			  cb_list->callback, cb_list->closure);
	  }
	  cb_list++;
	} /* while */
	WidgetOfLayout(lout) = wid;
      }
	break;
      default:
	/* Stop if unknown control-panel widget type, e.g. image.
	 * Prepare for return of the number of widgets created from the
	 * list.
	 * Set *num_children one too high (i.e. at least equal to one) in
	 * order to escape the loop.
	 */
	*num_children = i - first_wid + 1;
	break;
      } /* switch */

    if (*num_children) {
      /* The next widget is most locally an image (or unknown). */
      break;
    }
    if (i + 1 < first_wid + nwids &&
	ShellResourceNameOfLayout(layouts[i + 1])) {
      /* The next widget specifies a new control panel. */

      *num_children = i - first_wid + 1;
      break;
    }
  } /* for i */

  /* Fix the value of *num_children. */

  /* First the case where only legal control-panel widgets appeared. */
  if (*num_children == 0) *num_children = nwids;

  /* Then the case where the first specified widget was illegal. */
  else if (*num_children == 1) *num_children = 0;

  /* Then the general case. */
  else *num_children -= 1;

  if (do_realize) {
    /* Realize a new shell. */
    Widget top;

    top = TopLevelWidget(parent);
    XtRealizeWidget(top);
    (void) XSetWMProtocols(dpy_e, XtWindow(top), &wm_delete_window, 1);
  }

  LEAVE_FUNCTION_DEBUG("ximage.c: ximage_panel");

  return(top_form);

} /* ximage_panel() */

#ifndef FUNCPROTO
static void adjust_wid_width(layouts, first, last)
layout *layouts;
int first, last;
#else /* FUNCPROTO */
static void adjust_wid_width(layout *layouts, int first, int last)
#endif /* FUNCPROTO */
{
  Dimension width, mwidth;
  int i;
  Widget wid;
  char *wid_type;
  widget_type wid_typ;

  mwidth = width = 0;
  for (i = first; i <= last; i++) {
    wid      = WidgetOfLayout(layouts[i]);
    wid_type = TypeOfLayout(layouts[i]);

    XtVaGetValues(wid, XtNwidth, &width, NULL);
    if (width > mwidth) mwidth = width;
  } /* for */

  if (mwidth > 0) {
    for (i = first; i <= last; i++) {
      wid      = WidgetOfLayout(layouts[i]);
      wid_type = TypeOfLayout(layouts[i]);
      wid_typ  = parse_widget_type(wid_type);
      switch (wid_typ) {
      case ASCII_PAGE_WID: 
      case LABEL_WID: 
      case SCROLL_WID: 
	XtVaSetValues(wid, XtNwidth, mwidth, NULL);
	break;
      default:
	break;
      } /* switch */
    }
  }

  return;

} /* adjust_wid_width() */

#ifndef FUNCPROTO
static void process_layouts(shells, num_shells, not_shells, num_not_shells, app_parent, containerWidgetClass, layouts, nwids, separate, adjust_width, widget_set)
WidgetList *shells, *not_shells;
Cardinal *num_shells, *num_not_shells;
Widget app_parent;
WidgetClass containerWidgetClass;
layout *layouts;
int nwids;
separate_type separate;
Boolean adjust_width;
widget_set_type widget_set;
#else /* FUNCPROTO */
static void process_layouts(WidgetList *shells, Cardinal *num_shells, WidgetList *not_shells, Cardinal *num_not_shells, Widget app_parent, WidgetClass containerWidgetClass, layout *layouts, int nwids, separate_type separate, Boolean adjust_width, widget_set_type widget_set)
#endif /* FUNCPROTO */
{
  int wid_num;
  layout lout;
  Cardinal num_children;
  int first_in_container = -1, last_in_container = -1;
  char *wid_type = NULL, *next_resource_name = NULL;
  widget_type wid_typ;
  Widget top_container;
  Boolean in_container = False;

  if (num_shells != 0 && shells) {
    *num_shells     = 0;
    *shells         = (WidgetList) malloc(sizeof(Widget) * nwids);
  } else shells = NULL;

  if (num_not_shells != 0 && not_shells) {
    *num_not_shells = 0;
    *not_shells     = (WidgetList) malloc(sizeof(Widget) * nwids);
  } else not_shells = NULL;

  top_container   = app_parent;

  if (adjust_width && nwids == 1) adjust_width = False;

  for (wid_num = 0; wid_num < nwids; wid_num++) {

    /* Check whether this widget is the first one in a new window.
     * In this case, there may be a resource instance name string for the
     * new window appended to the widget name (separated by ":").
     */
    lout = layouts[wid_num];
    wid_type = TypeOfLayout(lout);
    if (wid_num < nwids - 1)
      next_resource_name = ShellResourceNameOfLayout(layouts[wid_num + 1]);
    else next_resource_name = NULL;

    wid_typ  = parse_widget_type(wid_type);
    switch(wid_typ)
      {
      case IMG_WID:
      case OVERLAY_WID: {
	layout_image lout_image;
	layout_image_part *image_part;

	lout_image = (layout_image) lout;
	image_part = &(lout_image->image);

	if (!image_part || !image_part->img) {
	  Error(2, "process_layouts: Missing image.\n");
	}

	if (nwids == 1                                 /* Image is alone     */
	    || separate == SEPARATE                    /* Force separate     */
	    || (separate == CHECK_PANEL_RES            /* Be intelligent     */
		&& (ShellResourceNameOfLayout(lout)    /* Expl. specified  */
		    || (next_resource_name != NULL     /* Next is new window */
			&& !in_container               /* Not already inside */
		     )))) {
	  /* New window for image. */

	  WidgetOfLayout(lout) =
	    ximage_image(TopLevelWidget(app_parent), lout);
	  /* The image widget is a descendent of a new shell, because the
	   * parent argument to ximage_image was a shell. The new shell has
	   * been realized and popped up.
	   */
	  top_container = XtParent(WidgetOfLayout(lout)); /* Class ImageForm */

	  if (adjust_width && first_in_container >= 0 &&
	      last_in_container > first_in_container) {
	    /* Adjust width of widgets in previous container. */
	    adjust_wid_width(layouts, first_in_container, last_in_container);
	  }

	  /* Not in container any more. */
	  in_container = False;
	  first_in_container = last_in_container = -1;

	  if (shells)
	    (*shells)[(*num_shells)++] =
	      TopLevelWidget(WidgetOfLayout(lout));

	} else {
	  /* Not new window for image. */

	  if (top_container == app_parent) {
	    /* This is the first widget encountered. Make sure that it does
	     * not create a window of its own.
	     */
	    top_container = XtVaCreateManagedWidget("topContainer",
						    boxWidgetClass,
						    app_parent, NULL);
	    in_container = True;
	    if ((not_shells &&
		 (num_not_shells == 0 ||
		  (*not_shells)[(*num_not_shells)-1] != top_container)) &&
		(!shells || num_shells == 0 ||
		 (*shells)[*(num_shells-1)] != TopLevelWidget(top_container)))
	      /* top_container not registered as the previous shell or 
	       * not_shell.
	       */
	      (*not_shells)[(*num_not_shells)++] = top_container;
	  }
	  WidgetOfLayout(lout) = ximage_image(top_container, lout);
	  /* The image widget is a direct child of top_container.
	   * The shell parent of top_container must not have been realized
	   * and popped up.
	   */

	  if (first_in_container < 0) first_in_container = wid_num;
	  last_in_container = wid_num;
	}

	/* top_container is class ImageForm or boxWidgetClass. */
      }
	break;
      default:
	if (separate == SEPARATE
	    || wid_num == 0
	    || (separate == CHECK_PANEL_RES
		&& ShellResourceNameOfLayout(lout))) {
	  /* New window for control panel. */

	  if (!ShellResourceNameOfLayout(lout))
	    ShellResourceNameOfLayout(lout) = "Control panel";

	  top_container = ximage_panel(TopLevelWidget(app_parent),
				       containerWidgetClass,
				       layouts, wid_num, nwids - wid_num,
				       &num_children, widget_set);
	  /* The new top_container is different from the previous one,
	   * because ximage_panel created a new shell, and top_container is
	   * its child. The new shell was created because the parent argument
	   * passed to ximage_panel was a shell and the first widget passed
	   * to ximage_panel had a non-NULL panel resource name.
	   *
	   * The shell created by ximage_panel has been realized, but not
	   * popped up.
	   */

	  if (adjust_width && first_in_container >= 0 &&
	      last_in_container > first_in_container) {
	    /* Adjust width of widgets in previous container. */
	    adjust_wid_width(layouts, first_in_container, last_in_container);
	    first_in_container = last_in_container = -1;
	  }

	  in_container = True;
	  if (first_in_container < 0) first_in_container = wid_num;
	  last_in_container = wid_num + num_children - 1;

	  if (shells)
	    (*shells)[(*num_shells)++] = TopLevelWidget(top_container);
	} else {
	  /* Not new window for control panel. */
	  top_container = ximage_panel(top_container, NULL,
				       layouts, wid_num, nwids - wid_num,
				       &num_children, widget_set);
	  /* The new top_container equals the previous one. It is not a
	   * shell. The shell parent of top_container must not have been
	   * realized and popped up. */

	  in_container = True;
	  if (first_in_container < 0) first_in_container = wid_num;
	  last_in_container = wid_num + num_children - 1;

	  if ((not_shells &&
	       (num_not_shells == 0 ||
		(*not_shells)[(*num_not_shells)-1] != top_container)) &&
	      (!shells || num_shells == 0 ||
	       (*shells)[*(num_shells-1)] != TopLevelWidget(top_container)))
	    /* top_container not registered as the previous shell or not_shell.
	     */
	    (*not_shells)[(*num_not_shells)++] = top_container;
	}

	if (num_children) wid_num += num_children - 1;
	break;
      } /* switch */
  } /* for */

  if (adjust_width && first_in_container >= 0 &&
      last_in_container > first_in_container) {
    /* Adjust width of widgets in last container. */
    adjust_wid_width(layouts, first_in_container, last_in_container);
  }

  return;

} /* process_layouts() */

#ifndef FUNCPROTO
static char *next_spec(s)
char *s;
#else /* FUNCPROTO */
static char *next_spec(char *s)
#endif /* FUNCPROTO */
{
  char *t, *t2, *end;
  static char *start_s = NULL, *end_s = NULL, *pos_s = NULL;
  int num_slashes = 0, len;

  if (s) {
    /* Remember end-points of string. */
    start_s = s;
    end_s   = &(s[strlen(s)]); /* Remember the end of the string. */
    pos_s   = start_s;
  }

  if (pos_s == end_s) return(NULL);

  t = pos_s; /* Potential result. */

  /* Find first ':'. */
  while (*pos_s != '\0' && *pos_s != ':') pos_s++;

  /* If ':' is first character, then word is empty. */
  if (*pos_s == ':' && pos_s == t) {
    pos_s++; /* Prepare for next call. */
    return(NULL);
  }

  end = pos_s;
  if (pos_s < end_s) pos_s++; /* First character after ':'. */

  /* Check if ':' is part of string (escaped with leading '\\'). */
  while (t && *(end - 1) == '\\') {
    num_slashes++;
    /* Escaped ':'. Don't interpret this as separator. */
    *(end - num_slashes) = *end; /* Replace '\\' by ':'. */

    if (end < end_s) { /* Have not yet found last token. */
      /* Look for next separator. */
      while (*pos_s != '\0' && *pos_s != ':') {
	*(pos_s - num_slashes) = *pos_s;
	pos_s++;
      }
      end = pos_s;
      if (pos_s < end_s) pos_s++;
    }
  }

  len = end - t - num_slashes;
  t2  = (char *) malloc(sizeof(char) * (len + 1));
  (void) strncpy(t2, t, len);
  t2[len] = '\0';

  return(t2);

} /* next_spec() */

#ifndef FUNCPROTO
Widget ximage_layout(app_parent, band, wids, rwids, nwids, ximage_work_fun, res_inst_name)
Widget app_parent;
IBAND band;
char **wids, *res_inst_name;
Widget rwids[];
int nwids;
XtCallbackProc ximage_work_fun;
#else /* FUNCPROTO */
Widget ximage_layout(Widget app_parent, IBAND band, char **wids, Widget *rwids, int nwids, XtCallbackProc ximage_work_fun, char *res_inst_name)
#endif /* FUNCPROTO */
{
  Widget parent;
  WidgetList shells, not_shells;
  WidgetClass containerWidgetClass;
  Cardinal num_shells, num_not_shells;
  int i;
  char *s = NULL, *wid_name = NULL, *resource_name, *wid_type = NULL;
  widget_type wid_typ;
  layout *layouts;

  ENTER_FUNCTION_DEBUG("ximage.c: ximage_layout");

  if (app_parent == NULL) {
    LEAVE_FUNCTION_DEBUG("ximage.c: ximage_layout");
    Error(2, "ximage_layout: No parent in ximage_layout.\n");
  }
  
  if (res_inst_name)
    parent = (Widget) CreatePopTopShell(res_inst_name, app_parent);
  else
    parent = app_parent;

  layouts = (layout *) malloc(sizeof(layout) * nwids);

  for (i=0; i<nwids; i++) {
    /* Check whether this widget is the first one in a new window.
     * In this case, there may be a resource instance name string for the
     * new window appended to the widget name (separated by ":").
     */
    s = XtNewString(wids[i]);

    wid_type      = next_spec(s);
    wid_name      = next_spec(NULL);
    resource_name = next_spec(NULL);

    wid_typ = parse_widget_type(wid_type);
    switch(wid_typ)
      {
      case ASCII_PAGE_WID:
	layouts[i] = (layout)
	  Make_label_layout(NULL, wid_type, wid_name, resource_name, NULL, i);
	break;
      case ASCII_WID:
	layouts[i] = (layout)
	  Make_label_layout(NULL, wid_type, wid_name, resource_name, NULL, i);
	break;
      case COMMAND_WID:
	layouts[i] = (layout)
	  Make_command_layout(NULL, wid_type, wid_name, resource_name, NULL,
			    i, ximage_work_fun, (XtPointer) MallocIntP(i));
	break;
      case LABEL_WID:
	layouts[i] = (layout)
	  Make_label_layout(NULL, wid_type, wid_name, resource_name, NULL, i);
	break;
      case LIST_WID: {
	char *label, **labels;
	int num_items = 0;

	labels = (char **) malloc(sizeof(char *) * 50);
	/* Find first list label. */
        label = next_spec(NULL);
	if (!label)
	  Error(3, "ximage_layout: Missing label for list item.\n");

        /* "label" now refers to the label of the first list element. */
        while (label) {
	  labels[num_items++] = label;
          label = next_spec(NULL);
        }
	
	layouts[i] = (layout)
	  Make_list_layout(NULL, wid_type, wid_name, resource_name, NULL, i,
			   num_items, labels, ximage_work_fun,
			   (XtPointer) MallocIntP(i));
      }
	break;
      case MENU_WID: {
	char *name, *label, *tvalue;
	int value;

	layouts[i] = (layout)
	  Make_menu_layout(NULL, wid_type, wid_name, resource_name, NULL, i,
			   0, NULL);

        name = next_spec(NULL);
        if (!name) break;

        /* "name" now refers to the resource instance name of the first
	 * menuentry button widget. */

        while (name) {
          label = next_spec(NULL);
          if (!label)
	    Error(3, "ximage_layout: Missing label for menu item.\n");

          tvalue = next_spec(NULL);
          if (!tvalue)
	    Error(3, "ximage_layout: Missing value for menu item.\n");
          if (!sscanf(tvalue, "%d", &value))
            Error(3, "%s%s\n",
		  "ximage_layout: No integer value found for ",
		  "menu item.");

	  (void) Add_entry_layout((layout_menu) layouts[i], name, label, NULL,
				  ximage_work_fun,
				  (XtPointer) MallocIntP(value));
          name = next_spec(NULL);
        }
      }
	break;
      case IMG_WID:
      case OVERLAY_WID: {
	IMAGE img;

	img = Init_image(1, wid_name);
	img[1] = band;
	layouts[i] = (layout)
	  Make_image_layout(NULL, wid_type, wid_name, resource_name, NULL, i,
			    img, init_image_callbacks());

	add_callback_to_image_layout((layout_image) layouts[i], XtNcallback,
				     ximage_work_fun,
				     (XtPointer) MallocIntP(i), False);
      }
	break;
      case PROMPT_WID: {
	char *text;

	/* Find text for prompt widget. */
        text = next_spec(NULL);

        if (text && (&(wid_name[strlen(wid_name) + 1]) == text)) {
          /* The resource name spec was not empty. It may already have been
           * used as the resource instance name for this control panel.
           */

          text = next_spec(NULL);
        }

	layouts[i] = (layout)
	  Make_prompt_layout(NULL, wid_type, wid_name, resource_name, NULL, i,
			   ximage_work_fun, (XtPointer) MallocIntP(i), text);
      }
	break;
      case SCROLL_WID:
	layouts[i] = (layout)
	  Make_scroll_layout(NULL, wid_type, wid_name, resource_name, NULL, i,
			     ximage_work_fun, (XtPointer) MallocIntP(i),
			     NULL, NULL);
	break;
      case TOGGLE_WID:
	layouts[i] = (layout)
	  Make_command_layout(NULL, wid_type, wid_name, resource_name, NULL,
			    i, ximage_work_fun, (XtPointer) MallocIntP(i));
	break;
      } /* switch() */

    XtFree(s); s = NULL;
  } /* for (i) */

  switch(panel_container_s) {
  case UNKNOWN_CONTAINER: containerWidgetClass = boxWidgetClass;
    break;
  case FORM_CONTAINER:    containerWidgetClass = formWidgetClass;
    break;
  case BOX_CONTAINER:     containerWidgetClass = boxWidgetClass;
    break;
  }
  if (layout_s     == UNKNOWN_SEPARATE)   layout_s     = CHECK_PANEL_RES;
  if (widget_set_s == UNKNOWN_WIDGET_SET) widget_set_s = XAW_WIDGETS;

  process_layouts(&shells, &num_shells, &not_shells, &num_not_shells,
		  parent, containerWidgetClass, layouts, nwids, layout_s,
		  adjust_width_s, widget_set_s);
  /* The widgets referred to by shells are shell widgets which may have been
   * realized and popped up.
   */

  for (i = 0; i < nwids; i++) {
    rwids[i] = WidgetOfLayout(layouts[i]);
  }

  {
    Widget top;

    for (i = 0; i < num_shells; i++) {
      top = TopLevelWidget(shells[i]);
      if (!XtIsRealized(top)) XtRealizeWidget(top);
      (void) XSetWMProtocols(dpy_e, XtWindow(top), &wm_delete_window, 1);
      XtPopup(top, XtGrabNone);
    }
  }

  LEAVE_FUNCTION_DEBUG("ximage.c: ximage_layout");

  return(parent);

} /* ximage_layout() */

#ifndef FUNCPROTO
Widget ximageLayout(app_parent, layouts, nwids)
Widget app_parent;
layout *layouts;
int nwids;
#else /* FUNCPROTO */
Widget ximageLayout(Widget app_parent, layout *layouts, int nwids)
#endif /* FUNCPROTO */
{
  int i;
  WidgetClass containerWidgetClass;
  WidgetList shells, not_shells;
  Cardinal num_shells, num_not_shells;

  ENTER_FUNCTION_DEBUG("ximage.c: ximageLayout");

  if (app_parent == NULL) {
    LEAVE_FUNCTION_DEBUG("ximage.c: ximageLayout");
    Error(2, "ximageLayout: No parent in ximageLayout.\n");
  }
  
  switch(panel_container_s) {
  case UNKNOWN_CONTAINER: containerWidgetClass = formWidgetClass;
    break;
  case FORM_CONTAINER:    containerWidgetClass = formWidgetClass;
    break;
  case BOX_CONTAINER:     containerWidgetClass = boxWidgetClass;
    break;
  }
  if (layout_s == UNKNOWN_SEPARATE) layout_s = SEPARATE;
  if (widget_set_s == UNKNOWN_WIDGET_SET) widget_set_s = XAW_WIDGETS;

  process_layouts(&shells, &num_shells, &not_shells, &num_not_shells,
		  app_parent, containerWidgetClass,
		  layouts, nwids, layout_s, adjust_width_s, widget_set_s);

  /* The widgets referred to by shells are shell widgets which may have been
   * realized and popped up.
   */

  {
    Widget top;

    for (i = 0; i < num_shells; i++) {
      top = TopLevelWidget(shells[i]);
      if (!XtIsRealized(top)) XtRealizeWidget(top);
      (void) XSetWMProtocols(dpy_e, XtWindow(top), &wm_delete_window, 1);
      XtPopup(top, XtGrabNone);
    }
  }

  LEAVE_FUNCTION_DEBUG("ximage.c: ximageLayout");

  return(app_parent);

} /* ximageLayout() */


#ifndef FUNCPROTO
void ximage_mainloop()
#else /* FUNCPROTO */
void ximage_mainloop(void)
#endif /* FUNCPROTO */
{
  ENTER_FUNCTION_DEBUG("ximage.c: ximage_mainloop");
  XtAppMainLoop(appcon_s);
  LEAVE_FUNCTION_DEBUG("ximage.c: ximage_mainloop");

} /* ximage_mainloop() */

#ifndef FUNCPROTO
void ximage_exit(wid, str, dummy)
Widget wid;
XtPointer str;
XtPointer dummy;
#else /* FUNCPROTO */
void ximage_exit(Widget wid, XtPointer str, XtPointer dummy)
#endif /* FUNCPROTO */
{
  ENTER_FUNCTION_DEBUG("ximage.c: ximage_exit");
  XtDestroyApplicationContext(appcon_s);
  LEAVE_FUNCTION_DEBUG("ximage.c: ximage_exit");
  exit(0);
}


