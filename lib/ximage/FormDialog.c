

/*C*

________________________________________________________________

        FormDialog
        $Id$
        Copyright 1996, Blab, UiO
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
#include <X11/Xos.h>
#include <X11/Intrinsic.h>
#include <X11/StringDefs.h>
#include <X11/Shell.h>
#include <X11/Xaw/Command.h>
#include <X11/Xaw/Dialog.h>
#include <X11/Xaw/Text.h>
#include <X11/Xaw/Form.h>
#include <X11/Xaw/Label.h>
#include <Xfwf/FileSel.h>
#include <Xfwf/Prompt.h>
#include <Xfwf/Toggle.h>
#include <Xfwf/Button.h>
#include <Xfwf/Group.h>
#include <xite/ShellWids.h>
#include <xite/Xdialog.h>
#include <xite/FormDialog.h>
#include XITE_MALLOC_H
#include <xite/utils.h>
#include <xite/mallocp.h>
#include <xite/strings.h>
#include <xite/debug.h>
#include <ctype.h>

#define BUTTON1_PUSHED 0
#define BUTTON2_PUSHED 1
#define NOT_DONE (-2)

extern Atom wm_delete_window;
extern Display *dpy_e;

Boolean dialog_defaults_e = False;

#define PromptInt    "promptInt"
#define PromptFloat  "promptFloat"
#define PromptChar   "promptChar"
#define PromptString "promptString"
#define PromptBool   "promptBool"
#define PromptRadio  "promptRadio"
#define PromptMulti  "promptMulti"

#define GroupRadio   "groupRadio"
#define GroupMulti   "groupMulti"

static int form_dialog_return_val_s;

static void Next_event(Widget parent)
{
  XtAppContext appcon;
  XEvent event;

  appcon  = XtWidgetToApplicationContext(parent);
  XtAppNextEvent(appcon, &event);
  XtDispatchEvent(&event);
}

static void quit_form_dialog(Widget widget, XEvent *event, String *params, Cardinal *num_params)
{
  Widget wid = NULL;
  char name[50];

  ENTER_FUNCTION_DEBUG("Xdialog.c: quit_form_dialog");

  wid = XtNameToWidget(widget, strcat(strcpy(name, "*"), "quit"));
  XtCallCallbacks(wid, XtNactivate, NULL);

  LEAVE_FUNCTION_DEBUG("Xdialog.c: quit_form_dialog");

} /* quit_form_dialog() */

static char *nextInterval(char **in)
{
  char *input, *value, *tmp;

  input = *in;

  /* Move up to the first '#'. */
  input = nextNonSpace(input);
  if (input[0] != '#') {
    *in = input;
    return(NULL);
  }


  /* Check for two consecutive '#' (interval is empty). */
  input++;
  if (input[0] == '#') {
    *in = input;
    return(NULL);
  }

  /* Skip leading blanks inside interval. */
  input = nextNonSpace(input);
  if (input[0] == '#') {
    *in = input;
    return(NULL);
  }

  /* Find end of interval. */
  tmp = input;
  while ((*input) && (*input) != '#') input++;

  /* Nothing left on line. */
  if (tmp == input) {
    *in = input;
    return(NULL);
  }

  /* *input is now either '#' or '\0'. */
  input--;

  /* Remove trailing blanks. */
  while (isspace(*input)) input--;

  input++;
  /* *input now refers to the last non-blank. */

  value = (char *) malloc((input - tmp + 1) * sizeof(char));
  (void) strncpy(value, tmp, input - tmp);
  value[input - tmp] = '\0';
  
  *in = input;

  if (strlen(value) == 0) return(NULL);

  return(value);

} /* nextInterval() */

typedef struct _prompt_line{
  Widget label_w, prompt_w;
  char *option, *type, *deflt;
  struct _prompt_line *next_prompt;
} prompt_line;

static prompt_line *first_prompt_s, *last_prompt_s;

static void free_prompt_lines(void)
{
  prompt_line *p = NULL, *prev = NULL;

  for (p = first_prompt_s; p != NULL; p = p->next_prompt) {
    if (prev) { XtFree((char *) prev); prev = NULL; }
    free(p->option); p->option = NULL;
    free(p->type);   p->type   = NULL;
    free(p->deflt);  p->deflt  = NULL;
    prev = p;
  }

  if (prev) { XtFree((char *) prev); prev = NULL; }

  return;

} /* free_prompt_lines() */

static void quit_prompt_dialog(Widget widget, XtPointer client_data, XtPointer call_data)
{
  ENTER_FUNCTION_DEBUG("Xdialog.c: quit_prompt_dialog");

  form_dialog_return_val_s = BUTTON1_PUSHED;

  LEAVE_FUNCTION_DEBUG("Xdialog.c: quit_prompt_dialog");

} /* quit_prompt_dialog() */

static void std_button_callback(Widget widget, XtPointer client_data, XtPointer call_data)
{
  ENTER_FUNCTION_DEBUG("Xdialog.c: std_button_callback");

  form_dialog_return_val_s = (int) *((int *) client_data);

  LEAVE_FUNCTION_DEBUG("Xdialog.c: std_button_callback");

} /* std_button_callback() */

static void accept(Widget widget, XtPointer client_data, XtPointer call_data)
{
  prompt_line *p;
  char *result, *tmp, **cl_data = (char **) client_data;
  int len = 500, len2;

  ENTER_FUNCTION_DEBUG("Xdialog.c: accept");

  result = (char *) malloc(len * sizeof(char));
  result[0] = '\0';
  for (p = first_prompt_s; p != NULL; p = p->next_prompt) {
    char *s = NULL;
    Boolean state = 1;

    if (XtIsSubclass(p->prompt_w, xfwfPromptWidgetClass)) {
      XtVaGetValues(p->prompt_w, XtNtext, &s, NULL);
      if (!s || strlen(nextNonSpace(s)) == 0) {
	p->option = NULL;
	s         = NULL;
      }
    } else {
      XtVaGetValues(p->prompt_w, XtNon, &state, NULL);

      /* For promptRadio (promptMulti), p->deflt will be used only if state
       * is on and p->type == "r0" or "r1" ("m0" or "m1").
       * p->type == "r0-" or "r1-" ("m0-" or "m1-") is used to enable the use
       * of deflt merely as a label and not a value to return.
       */

      if (state && !strcmp(XtName(p->prompt_w), PromptRadio) &&
	  p->type && (!strcmp(p->type, "r0") || !strcmp(p->type, "r1")))
	s = p->deflt;
      else if (state && !strcmp(XtName(p->prompt_w), PromptMulti) &&
	  p->type && (!strcmp(p->type, "m0") || !strcmp(p->type, "m1")))
	s = p->deflt;
    }

    len2 = 0;
    if (p->option) len2 += strlen(p->option) + 1;
    if (s) {
      len2 += strlen(s) + 1;
      if (p->type && !strcmp(p->type, "s")) len2 += 2; /* For quotes */
    }

    if (len2 == 0) tmp = NULL;
    else {
      tmp = realloc_string(result, len, len2, 500);

      if (tmp != result) {
	free(result);
	len += 500;
      }
      result = tmp;
    }

    if (state && p->option) {
      strcat(result, " ");
      strcat(result, p->option);
    }
    if (s) {
      strcat(result, " ");
      if (p->type && !strcmp(p->type, "s")) strcat(result, "\"");
      strcat(result, s);
      if (p->type && !strcmp(p->type, "s")) strcat(result, "\"");
    }
  } /* for */

  *cl_data = result;

  form_dialog_return_val_s = BUTTON2_PUSHED;
  
  LEAVE_FUNCTION_DEBUG("Xdialog.c: accept");

  return;

} /* accept() */

static void help(Widget widget, XtPointer client_data, XtPointer call_data)
{
  char *title = (char *) client_data;
  Widget parent;

  ENTER_FUNCTION_DEBUG("Xdialog.c: help");

  if (!title) title = XtNewString("For help, consult a manual page.");

  parent = TopLevelWidget(widget);
  XtPopup(TextBox(parent, title, "Close", "help", "help"), XtGrabNone);

  LEAVE_FUNCTION_DEBUG("Xdialog.c: help");

  return;

} /* help() */

static void checkInt(Widget widget, XtPointer client_data, XtPointer call_data)
{
  XfwfPromptInfo *c_data = (XfwfPromptInfo *) call_data;
  char *old = c_data->oldstring, *new = c_data->newstring, *np;
  
  ENTER_FUNCTION_DEBUG("Xdialog.c: checkInt");

  if (old && new && !strcmp(old, new)) return;
  if (!new) return;

  for (np = new; *np != '\0'; np++) {
    if (!(isdigit(*np) || *np == '-')) {
      c_data->result = XfwfReject;
      XBell(dpy_e, 50);
      break;
    }
  }

  LEAVE_FUNCTION_DEBUG("Xdialog.c: checkInt");

  return;

} /* checkInt() */

static void checkFloat(Widget widget, XtPointer client_data, XtPointer call_data)
{
  XfwfPromptInfo *c_data = (XfwfPromptInfo *) call_data;
  char *old = c_data->oldstring, *new = c_data->newstring, *np;
  Boolean point_found = 0;
  
  if (old && new && !strcmp(old, new)) return;
  if (!new) return;

  ENTER_FUNCTION_DEBUG("Xdialog.c: checkFloat");

  for (np = new; *np != '\0'; np++) {
    if (*np == '.') point_found = 1;
    else if (!(isdigit(*np) || (!point_found && (*np == '.' || *np == '-')))) {
      c_data->result = XfwfReject;
      XBell(dpy_e, 50);
      break;
    }
  }

  LEAVE_FUNCTION_DEBUG("Xdialog.c: checkFloat");

  return;

} /* checkFloat() */

static void interpret_line(char **dialog_text, char **label, char **option, char **type, char **deflt, char **promptName)
{
  *label  = nextInterval(dialog_text);
  *option = nextInterval(dialog_text);
  *type   = nextInterval(dialog_text);
  *deflt  = nextInterval(dialog_text);

  if (!(*type)) *type = XtNewString("s");
  if (!strcmp(*type, "d")) *promptName = PromptInt;
  else if (!strcmp(*type, "f")) *promptName = PromptFloat;
  else if (!strcmp(*type, "c")) *promptName = PromptChar;
  else if (!strcmp(*type, "s")) *promptName = PromptString;
  else if (!strcmp(*type, "b")) *promptName = PromptBool;
  else if (!strcmp(*type, "r0")  || !strcmp(*type, "r1") ||
	   !strcmp(*type, "r0-") || !strcmp(*type, "r1-"))
    *promptName = PromptRadio;
  else if (!strcmp(*type, "m0")  || !strcmp(*type, "m1") ||
	   !strcmp(*type, "m0-") || !strcmp(*type, "m1-"))
    *promptName = PromptMulti;
  else {
    *promptName = NULL;
    fprintf(stderr, "Prompt type %s not known.\n", *type);
    fprintf(stderr, "Error in dialog specification.\n");
    exit(1);
  }

} /* interpret_line() */

static int CreateBooleanField(Widget *wid_toggle, Widget *wid_lab, Dimension *max_lab_width, Widget top_form, Widget wid_lab_prev, char *label, char *deflt, char *promptName)
{
  Boolean state = 0;
  Dimension lab_width;

  if (!label) Warning(1, "No label text found for boolean input field.\n");

  *wid_lab = XtVaCreateManagedWidget("label", labelWidgetClass, top_form, 
				     XtNlabel, label,
				     XtNfromVert, wid_lab_prev, NULL);
  XtVaGetValues(*wid_lab, XtNwidth, &lab_width, NULL);
  if (lab_width > *max_lab_width) *max_lab_width = lab_width;

  if (!deflt) deflt = XtNewString("yes");

  if      (!strcmp(deflt, "yes")) state = 1;
  else if (!strcmp(deflt, "Yes")) state = 1;
  else if (!strcmp(deflt, "YES")) state = 1;
  else if (!strcmp(deflt, "1")) state = 1;
  else if (!strcmp(deflt, "true")) state = 1;
  else if (!strcmp(deflt, "True")) state = 1;
  else if (!strcmp(deflt, "TRUE")) state = 1;
  *wid_toggle = XtVaCreateManagedWidget(promptName, xfwfToggleWidgetClass,
					top_form,
					XtNfromHoriz, *wid_lab,
					XtNfromVert, wid_lab_prev,
					XtNon, state, NULL);

  return(0);

} /* CreateBooleanField() */

static int CreateRadioMultiField(Widget *wid_toggle, Widget *wid_lab, Widget *wid_group, Dimension *max_lab_width, Widget top_form, Widget wid_lab_prev, char *label, char *type, char *deflt, char *group_name, char *promptName, Boolean is_first_in_group)
{
  Dimension lab_width;
  Boolean state;

  if (!strncmp(type, "m1", 2) || !strncmp(type, "r1", 2)) state = True;
  else state = False;

  if (is_first_in_group) {

    if (!label)
      Warning(1, "No label text found for radio/multi input field.\n");

    *wid_lab = XtVaCreateManagedWidget("label", labelWidgetClass, top_form, 
				       XtNlabel, label,
				       XtNfromVert, wid_lab_prev, NULL);
    XtVaGetValues(*wid_lab, XtNwidth, &lab_width, NULL);
    if (lab_width > *max_lab_width) *max_lab_width = lab_width;

    *wid_group = XtVaCreateManagedWidget(group_name, xfwfGroupWidgetClass,
					 top_form,
					 XtNfromHoriz, *wid_lab,
					 XtNfromVert, wid_lab_prev,
					 NULL);

    if (!deflt)
      Warning(1, "No default text found for radio/multi input field.\n");
    *wid_toggle = XtVaCreateManagedWidget(promptName, xfwfToggleWidgetClass,
					  *wid_group,
					  XtNlabel, deflt,
					  XtNon, state, NULL);
  } else {
    if (!deflt)
      Warning(1, "No default text found for radio/multi input field.\n");
    *wid_toggle = XtVaCreateManagedWidget(promptName, xfwfToggleWidgetClass,
					  *wid_group,
					  XtNlabel, deflt,
					  XtNon, state, NULL);
  }

  return(0);

} /* CreateRadioMultiField() */

static int CreateInputField(Widget *wid_prompt, Widget *wid_lab, Dimension *max_lab_width, Dimension *max_prompt_width, Widget top_form, Widget wid_lab_prev, char *label, char *deflt, char *promptName, char *translations)
{
  Dimension lab_width, prompt_width;
  Dimension height, top_margin, bottom_margin, left_margin,
            right_margin, frame_width, inner_offset, outer_offset;


  if (!label) Warning(1, "No label text found for input field.\n");
  *wid_lab = XtVaCreateManagedWidget("label", labelWidgetClass, top_form, 
				     XtNlabel, label,
				     XtNfromVert, wid_lab_prev, NULL);
  XtVaGetValues(*wid_lab, XtNwidth, &lab_width, NULL);
  if (lab_width > *max_lab_width) *max_lab_width = lab_width;

  *wid_prompt =
    XtVaCreateManagedWidget(promptName, xfwfPromptWidgetClass, top_form,
			    XtNtraversalOn, "off",
			    XtNtranslations,
			    XtParseTranslationTable(translations),
			    XtNfromHoriz,   *wid_lab,
			    XtNfromVert,    wid_lab_prev,
			    XtNtext,        deflt,
			    NULL);

  XtVaGetValues(*wid_prompt, XtNtopMargin,    &top_margin,
		XtNbottomMargin, &bottom_margin,
		XtNleftMargin,   &left_margin,
		XtNrightMargin,  &right_margin,
		XtNframeWidth,   &frame_width,
		XtNinnerOffset,  &inner_offset,
		XtNouterOffset,  &outer_offset,
		XtNwidth,        &prompt_width,
		XtNheight,       &height,        NULL);

  if (prompt_width <= left_margin + right_margin + 2 * frame_width +
	       inner_offset + outer_offset ||
      height <= top_margin + bottom_margin + 2 * frame_width +
      inner_offset + outer_offset) {
    XtDestroyWidget(*wid_prompt);

    *wid_prompt =
      XtVaCreateManagedWidget(promptName, xfwfPromptWidgetClass, top_form,
			      XtNtraversalOn, "off",
			      XtNtranslations,
			      XtParseTranslationTable(translations),
			      XtNfromHoriz,   *wid_lab,
			      XtNfromVert,    wid_lab_prev,
			      XtNtext,        deflt,
			      XtNshrinkToFit, "True", NULL);
  }

  XtVaGetValues(*wid_prompt, XtNwidth, &prompt_width, NULL);
  if (prompt_width > *max_prompt_width) *max_prompt_width = prompt_width;

  return(0);

} /* CreateInputField() */

static prompt_line *attach_prompt_object(Widget wid_lab, Widget wid_prompt, char *option, char *type, char *deflt)
{
  /* Make new prompt-line object and attach at the end of the list. */

  prompt_line *p_line;

  p_line = XtNew(prompt_line);
  if (first_prompt_s == NULL) {
    first_prompt_s = last_prompt_s = p_line;
  } else {
    last_prompt_s->next_prompt = p_line;
    last_prompt_s = p_line;
  }

  p_line->next_prompt = NULL;
  p_line->label_w     = wid_lab;
  p_line->prompt_w    = wid_prompt;
  p_line->option      = option;
  p_line->type        = type;
  p_line->deflt       = deflt;

  return(p_line);

} /* attach_prompt_object() */

static void set_resources_radio_multi(prompt_line *first_prompt, Dimension max_toggle_width, Dimension max_allowed_group_width, Dimension *group_width)
{
  int num_toggles = 0, num_cols = 0;
  prompt_line *p_line, *p_line_old;

  num_toggles  = 0;
  p_line_old   = NULL;
  for (p_line = first_prompt; p_line != NULL; p_line = p_line->next_prompt) {

    if (!strcmp(XtName(p_line->prompt_w), PromptRadio) ||
	!strcmp(XtName(p_line->prompt_w), PromptMulti)) {
      /* Multi-toggle question. */

      if (!p_line_old ||
	  (p_line_old && p_line_old->label_w == p_line->label_w)) {
	/* First or another toggle for the same question. */
	num_toggles++;
      } else if (p_line_old && p_line_old->label_w != p_line->label_w) {
	/* New question and its first toggle. */
	
	break;
      }

      XtVaSetValues(p_line->prompt_w, XtNwidth, max_toggle_width, NULL);
      p_line_old = p_line;
    }
    else break;
  } /* for each input widget. */

  if (num_toggles > 0 && p_line_old != NULL) {
    /* The last question was a multi-toggle one, and it has not been
     * treated. Set number of columns for the last multi-toggle question.
     */

    num_cols = (int) max_allowed_group_width / max_toggle_width;
    if (num_cols < 1) num_cols = 1;
    num_cols = num_toggles <= num_cols ? num_toggles : num_cols;
    XtVaSetValues(XtParent(p_line_old->prompt_w), XtNcolumns, num_cols, NULL);
  }

  *group_width = num_cols * max_toggle_width;

} /* set_resources_radio_multi() */

static void CreateWidgets(Widget parent, char *header, Widget *wid_name, Widget *wid_lab, Widget *wid_group, char *dialog_spec, Boolean *is_first_in_group, Dimension *max_lab_width, Dimension *max_prompt_width, Dimension *max_group_width, char **return_text, char *translations)
{
  Widget wid_lab_prev = NULL, wid_prompt = NULL;
  char *label = NULL, *label_prev = NULL, *option = NULL, *type = NULL;
  char *deflt = NULL, *promptName = NULL;
  Dimension max_toggle_width = 0, toggle_width = 0, group_width = 0;
  Dimension max_allowed_group_width = 0;
  prompt_line *first_in_group;

  /* Create dummy widget to find width resource spec. for string input
   * widgets. */
  wid_lab_prev =
    XtVaCreateWidget(PromptString, xfwfPromptWidgetClass, parent,
		     XtNtraversalOn, "off", XtNtext, "test", NULL);
  XtVaGetValues(wid_lab_prev, XtNwidth, &max_allowed_group_width, NULL);
  if (max_allowed_group_width > 1000) max_allowed_group_width = 1000;
  XtDestroyWidget(wid_lab_prev); wid_lab_prev = NULL;

  if (header) *wid_name = XtVaCreateManagedWidget("header", labelWidgetClass,
						  parent, XtNlabel, header,
						  NULL);
  else *wid_name = NULL;
  
  *wid_lab = *wid_name;
  first_prompt_s = last_prompt_s = NULL;

  while (1) {
    if (*is_first_in_group) wid_lab_prev = *wid_lab;
    else wid_lab_prev = *wid_group;

    if (label) { free(label); label = NULL; }
    interpret_line(&dialog_spec, &label, &option, &type, &deflt, &promptName);

    if (label && strlen(label) >= 1 && label[strlen(label)-1] != ':') {
      int len = strlen(label);

      label          = (char *) realloc(label, len + 2);
      label[len]     = ':';
      label[len + 1] = '\0';
    }

    if (!strcmp(promptName, PromptBool)) {

      if (!(*is_first_in_group) && label && label_prev &&
	  strcmp(label, label_prev)) {
	/* The previous toggle was the last in a group. */

	set_resources_radio_multi(first_in_group, max_toggle_width,
				  max_allowed_group_width, &group_width);
	if (group_width > *max_group_width) *max_group_width = group_width;
	*is_first_in_group = True;
      }

      CreateBooleanField(&wid_prompt, wid_lab, max_lab_width, parent,
			 wid_lab_prev, label, deflt, promptName);

      (void) attach_prompt_object(*wid_lab, wid_prompt, option, type, deflt);

      *is_first_in_group = True;
      max_toggle_width  = 0;

    } else if (!strcmp(promptName, PromptRadio) ||
	       !strcmp(promptName, PromptMulti)) {
      char *group_name;

      if (!strcmp(promptName, PromptRadio))
	group_name = XtNewString(GroupRadio);
      else
	group_name = XtNewString(GroupMulti);

      if (!(*is_first_in_group) && label && label_prev &&
	  strcmp(label, label_prev)) {
	/* The previous toggle was the last in a group. */

	set_resources_radio_multi(first_in_group, max_toggle_width,
				  max_allowed_group_width, &group_width);
	if (group_width > *max_group_width) *max_group_width = group_width;
	*is_first_in_group = True;
      }

      CreateRadioMultiField(&wid_prompt, wid_lab, wid_group, max_lab_width,
			    parent, wid_lab_prev, label, type,
			    deflt, group_name, promptName, *is_first_in_group);

      if (*is_first_in_group) {
	max_toggle_width = 0;
	first_in_group =
	  attach_prompt_object(*wid_lab, wid_prompt, option, type, deflt);
      } else {
	(void) attach_prompt_object(*wid_lab, wid_prompt, option, type, deflt);
      }

      XtVaGetValues(wid_prompt, XtNwidth, &toggle_width, NULL);
      if (toggle_width > max_toggle_width) max_toggle_width = toggle_width;

      if (label_prev) { free(label_prev); label_prev = NULL; }
      label_prev         = XtNewString(label);
      *is_first_in_group = False;

    } else {

      if (!(*is_first_in_group) && label && label_prev &&
	  strcmp(label, label_prev)) {
	/* The previous toggle was the last in a group. */

	set_resources_radio_multi(first_in_group, max_toggle_width,
				  max_allowed_group_width, &group_width);
	if (group_width > *max_group_width) *max_group_width = group_width;
	*is_first_in_group = True;
      }

      CreateInputField(&wid_prompt, wid_lab, max_lab_width, max_prompt_width,
		       parent, wid_lab_prev, label, deflt, promptName,
		       translations);

      XtAddCallback(wid_prompt, XtNactivate, accept, (XtPointer) return_text);

      if (!strcmp(promptName, PromptInt))
	XtAddCallback(wid_prompt, XtNvalidate, checkInt, NULL);
      else if (!strcmp(promptName, PromptFloat))
	XtAddCallback(wid_prompt, XtNvalidate, checkFloat, NULL);

      (void) attach_prompt_object(*wid_lab, wid_prompt, option, type, deflt);

      *is_first_in_group = True;
      max_toggle_width  = 0;
    }

    dialog_spec = nextNonSpace(dialog_spec);
    if (*dialog_spec == '\0') {
      if (!(*is_first_in_group) && label && label_prev &&
	  !strcmp(label, label_prev)) {
	/* The previous toggle was the last in a group. */

	set_resources_radio_multi(first_in_group, max_toggle_width,
				  max_allowed_group_width, &group_width);
	if (group_width > *max_group_width) *max_group_width = group_width;
      }

      break;
    }

  } /* while */

  if (label)      { free(label);      label = NULL; }
  if (label_prev) { free(label_prev); label_prev = NULL; }

} /* CreateWidgets() */

static void set_label_prompt_width(prompt_line *first_prompt, Dimension max_lab_width, Dimension max_prompt_width)
{
  /* Set width of labels and non-toggle prompts. */

  prompt_line *p_line;

  for (p_line = first_prompt; p_line != NULL; p_line = p_line->next_prompt) {
    if (p_line->label_w)
      XtVaSetValues(p_line->label_w, XtNwidth, max_lab_width, NULL);

      if (strcmp(XtName(p_line->prompt_w), PromptBool)  &&
	  strcmp(XtName(p_line->prompt_w), PromptRadio) &&
	  strcmp(XtName(p_line->prompt_w), PromptMulti))
	/* Non-toggle prompt input. */
	XtVaSetValues(p_line->prompt_w, XtNwidth, max_prompt_width, NULL);
  }
} /* set_label_prompt_width() */

static char *button_name(int num)
{
  char *btn_name;

  btn_name = (char *) malloc(strlen("button") + 3);

  (void) strcpy(btn_name, "button");
  sprintf(&(btn_name[strlen(btn_name)]), "%1d", num);

  return(btn_name);

} /* button_name() */

static void add_buttons(Widget parent, Widget from_vert, button_table buttons, int nbuttons, button_table default_buttons)
{
  char *btn_name, *label;
  XtCallbackProc callback;
  XtPointer closure;
  Widget wid = NULL, wid_prev = NULL;
  int btn;
  Dimension width, height, top_margin, bottom_margin, left_margin,
            right_margin, frame_width, inner_offset, outer_offset;

  for (btn = 0; btn < nbuttons; btn++) {

    if (!buttons) {
      if (btn <= 2) {
	btn_name = button_name(btn);
	label    = default_buttons[btn].label;
	callback = default_buttons[btn].callback;
	closure  = (XtPointer) default_buttons[btn].closure;
      } else {
	btn_name = button_name(btn);
	label    = NULL;
	callback = std_button_callback;
	closure  = (XtPointer) MallocIntP(btn);
      }
    } else {
      if (!buttons[btn].name) btn_name = button_name(btn);
      else btn_name = buttons[btn].name;

      if (!buttons[btn].label && btn <= 2)
	label = default_buttons[btn].label;
      else label = buttons[btn].label;

      if (!buttons[btn].callback && btn <= 2)
	callback = default_buttons[btn].callback;
      else if (!buttons[btn].callback)
	callback = std_button_callback;
      else callback = buttons[btn].callback;

      if (!buttons[btn].closure && btn <= 2)
	closure  = default_buttons[btn].closure;
      else if (!buttons[btn].closure)
	closure = (XtPointer) MallocIntP(btn);
      else closure = buttons[btn].closure;
    }

    if (label)
      wid = XtVaCreateManagedWidget(btn_name, xfwfButtonWidgetClass, parent,
				    XtNfromVert,  from_vert,
				    XtNfromHoriz, wid_prev,
				    XtNlabel,     label,    NULL);
    else
      wid = XtVaCreateManagedWidget(btn_name, xfwfButtonWidgetClass, parent,
				    XtNfromVert,  from_vert,
				    XtNfromHoriz, wid_prev, NULL);

    XtVaGetValues(wid, XtNlabel,        &label,
		       XtNtopMargin,    &top_margin,
		       XtNbottomMargin, &bottom_margin,
		       XtNleftMargin,   &left_margin,
		       XtNrightMargin,  &right_margin,
		       XtNframeWidth,   &frame_width,
		       XtNinnerOffset,  &inner_offset,
		       XtNouterOffset,  &outer_offset,
		       XtNwidth,        &width,
		       XtNheight,       &height,        NULL);

    if (width <= left_margin + right_margin + 2 * frame_width +
		 inner_offset + outer_offset ||
	height <= top_margin + bottom_margin + 2 * frame_width +
	inner_offset + outer_offset) {
      XtDestroyWidget(wid);

      if (label)
	wid = XtVaCreateManagedWidget(btn_name, xfwfButtonWidgetClass, parent,
				      XtNfromVert,    from_vert,
				      XtNfromHoriz,   wid_prev,
				      XtNlabel,       label,
				      XtNshrinkToFit, "True", NULL);
      else
	wid = XtVaCreateManagedWidget(btn_name, xfwfButtonWidgetClass, parent,
				      XtNfromVert,    from_vert,
				      XtNfromHoriz,   wid_prev,
				      XtNshrinkToFit, "True",   NULL);
    }

    if (callback) XtAddCallback(wid, XtNactivate, callback, closure);
    XtInstallAccelerators(parent, wid);

    wid_prev = wid;
  } /* for each button */

} /* add_buttons() */

button_table init_button_table(button_table buttons, int length)
{
  button_table btns;
  int i;

  if (!buttons) {
    btns = (button_table) malloc(length * sizeof(button_element));
  } else btns = buttons;

  for (i = 0; i < length; i++) {
    btns[i].name     = NULL;
    btns[i].label    = NULL;
    btns[i].callback = NULL;
    btns[i].closure  = NULL;
  }

  return(btns);

} /* init_button_table() */

int FormDialog(Widget parent, char *header, char *dialog_spec, char **return_text, button_table buttons, int nbuttons, char *winName, char *iconName)
{
  Widget top, top_form, wid_name = NULL, wid_lab = NULL;
  Widget wid_sep = NULL, wid_group = NULL;
  Dimension max_lab_width = 0, max_prompt_width = 0, width = 0;
  Dimension max_group_width = 0, total_width = 0;
  int int_width = 0;
  Boolean is_first_in_group = True;
  static char *prompt_translations_s = "#override \n\
     Ctrl<Key>b:  unselect() left()  \n\
     Ctrl<Key>f:  unselect() right() \n\
     <Key>Delete: cut() backspace()";
  static button_element def_buttons[] = {
    { NULL, "Abort",  quit_prompt_dialog, NULL },
    { NULL, "Accept", accept, NULL },
    { NULL, "Help",   help, (XtPointer) "For help, consult a manual page." },
  };

  ENTER_FUNCTION_DEBUG("Xdialog.c: FormDialog");

  top      = CreatePopTopShell("promptDialog", parent);
  top_form = XtVaCreateManagedWidget("topForm", formWidgetClass, top, NULL);

  AddAction(top, "QuitFormDialog", quit_form_dialog, 0);

  CreateWidgets(top_form, header, &wid_name, &wid_lab, &wid_group,
		dialog_spec, &is_first_in_group, &max_lab_width,
		&max_prompt_width, &max_group_width, return_text,
		prompt_translations_s);

  set_label_prompt_width(first_prompt_s, max_lab_width, max_prompt_width);

  if (max_prompt_width > max_group_width)
    total_width = max_lab_width + max_prompt_width;
  else total_width = max_lab_width + max_group_width;

  if (wid_name) {
    XtVaGetValues(wid_name, XtNwidth, &width, NULL);
    XtVaGetValues(wid_lab, XtNhorizDistance, &int_width, NULL);
    if (total_width + int_width > width) {
      XtVaSetValues(wid_name, XtNwidth, total_width + int_width, NULL);
    }
  }

  wid_sep = XtVaCreateManagedWidget("separator", labelWidgetClass, top_form,
				    XtNfromVert,
				      is_first_in_group ? wid_lab : wid_group,
				    XtNlabel, "  ",
				    XtNborderWidth, 0, NULL);

  def_buttons[1].closure = (XtPointer) return_text;
  add_buttons(top_form, wid_sep, buttons, nbuttons, def_buttons);

  XtRealizeWidget(top);
  XmbSetWMProperties(dpy_e, XtWindow(top),
		     winName, iconName,
		     NULL, 0, NULL, NULL, NULL);
  (void) XSetWMProtocols(dpy_e, XtWindow(top), &wm_delete_window, 1);

  PopUpDialog(top);

  /* Stay in loop so as not to start the program before input has been
   * gathered. */
  form_dialog_return_val_s = NOT_DONE;
  for(;;) {

    if (!dialog_defaults_e) Next_event(parent);
    else {
      Widget wid;
      char name[50];

      wid = XtNameToWidget(top_form, strcat(strcpy(name, "*"), "button1"));
      XtCallCallbacks(wid, XtNactivate, NULL);
    }

    if (form_dialog_return_val_s != NOT_DONE) {
      PopDownDialog(top);

      free_prompt_lines(); first_prompt_s = last_prompt_s = NULL;

      LEAVE_FUNCTION_DEBUG("Xdialog.c: FormDialog");
      return(form_dialog_return_val_s);
    }
  }
  /* for(;;) */

} /* FormDialog() */
