

/*C*

________________________________________________________________

        layout
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

#include <xite/includes.h>
#include <X11/Xos.h>
#include <X11/Intrinsic.h>
#include <X11/StringDefs.h>
#include <xite/layout.h>
#include <xite/callbacks.h>
#include XITE_MALLOC_H
#include <xite/message.h>
#include <xite/debug.h>

layout_label Make_core_part_layout(layout_core lay_out, char *type, char *resource_name, char *shell_resource_name, Widget wid, int id)
{
  layout_core_part *core_part;

  if (!lay_out) lay_out = (layout_core) XtNew(layout_core_rec);

  core_part = &(lay_out->core);

  core_part->type                = XtNewString(type);
  core_part->resource_name       = XtNewString(resource_name);
  core_part->shell_resource_name = XtNewString(shell_resource_name);
  core_part->wid                 = wid;
  core_part->id                  = id;

  return(lay_out);

} /* Make_core_part_layout() */

layout_label Make_label_layout(layout_label lay_out, char *type, char *resource_name, char *shell_resource_name, Widget wid, int id)
{
  if (!lay_out) lay_out = (layout_label) XtNew(layout_label_rec);
  (void) Make_core_part_layout((layout_core) lay_out, type, resource_name,
			       shell_resource_name, wid, id);

  return(lay_out);
  
} /* Make_label_layout() */

layout_image Make_image_layout(layout_image lay_out, char *type, char *resource_name, char *shell_resource_name, Widget wid, int id, IMAGE img, image_callbacks *callbacks)
{
  layout_image_part *image_part;

  if (!lay_out) lay_out = (layout_image) XtNew(layout_image_rec);
  (void) Make_core_part_layout((layout_core) lay_out, type, resource_name,
			       shell_resource_name, wid, id);

  image_part            = &(lay_out->image);
  image_part->img       = img;
  image_part->callbacks = callbacks;

  return(lay_out);

} /* Make_image_layout() */

void add_callback_to_image_layout(layout_image lay_out, String resource_name, XtCallbackProc callback, XtPointer closure, Boolean first)
{
  if (!lay_out) Error(2, "lay_out is NULL.\n");
  
  CallbacksOfImageLayout(lay_out) =
    add_to_image_callbacks(CallbacksOfImageLayout(lay_out), resource_name,
			   callback, closure, first);

  return;

} /* add_callback_to_image_layout() */

layout_command Make_command_layout(layout_command lay_out, char *type, char *resource_name, char *shell_resource_name, Widget wid, int id, XtCallbackProc callback, XtPointer closure)
{
  layout_command_part *command_part;

  if (!lay_out) lay_out = (layout_command) XtNew(layout_command_rec);
  (void) Make_core_part_layout((layout_core) lay_out, type, resource_name,
			       shell_resource_name, wid, id);
  
  command_part                = &(lay_out->command);
  command_part->callback_list = add_callback_first(NULL, callback, closure);

  return(lay_out);

} /* Make_command_layout() */

layout_prompt Make_prompt_layout(layout_prompt lay_out, char *type, char *resource_name, char *shell_resource_name, Widget wid, int id, XtCallbackProc callback, XtPointer closure, char *text)
{
  layout_prompt_part *prompt_part;

  if (!lay_out) lay_out = (layout_prompt) XtNew(layout_prompt_rec);
  (void) Make_core_part_layout((layout_core) lay_out, type, resource_name,
			       shell_resource_name, wid, id);
  
  prompt_part                = &(lay_out->prompt);
  prompt_part->text          = XtNewString(text);
  prompt_part->callback_list = add_callback_first(NULL, callback, closure);

  return(lay_out);

} /* Make_prompt_layout() */

layout_scroll Make_scroll_layout(layout_scroll lay_out, char *type, char *resource_name, char *shell_resource_name, Widget wid, int id, XtCallbackProc jump_callback, XtPointer jump_closure, XtCallbackProc scroll_callback, XtPointer scroll_closure)
{
  layout_scroll_part *scroll_part;

  if (!lay_out) lay_out = (layout_scroll) XtNew(layout_scroll_rec);
  (void) Make_core_part_layout((layout_core) lay_out, type, resource_name,
			       shell_resource_name, wid, id);
  
  scroll_part                       = &(lay_out->scroll);
  scroll_part->jump_callback_list   =
    add_callback_first(NULL, jump_callback, jump_closure);
  scroll_part->scroll_callback_list =
    add_callback_first(NULL, scroll_callback, scroll_closure);

  return(lay_out);

} /* Make_scroll_layout() */

layout_menu Make_menu_layout(layout_menu lay_out, char *type, char *resource_name, char *shell_resource_name, Widget wid, int id, int num_entries, layout_menu_entry *menu_entries)
{
  layout_menu_part *menu_part;

  if (!lay_out) lay_out = (layout_menu) XtNew(layout_menu_rec);
  (void) Make_core_part_layout((layout_core) lay_out, type, resource_name,
			       shell_resource_name, wid, id);
  
  menu_part               = &(lay_out->menu);
  menu_part->num_entries  = num_entries;
  menu_part->menu_entries = menu_entries;

  return(lay_out);

} /* Make_menu_layout() */

layout_menu_entry Make_entry_layout(layout_menu_entry lay_out, char *resource_name, char *label, Widget wid, XtCallbackProc callback, XtPointer closure)
{
  if (!lay_out) lay_out = (layout_menu_entry) XtNew(layout_menu_entry_rec);

  lay_out->resource_name = XtNewString(resource_name);
  lay_out->label         = XtNewString(label);
  lay_out->wid           = wid;
  lay_out->callback_list = add_callback_first(NULL, callback, closure);

  return(lay_out);

} /* Make_entry_layout() */

layout_menu_entry *Add_entry_layout(layout_menu lay_out, char *resource_name, char *label, Widget wid, XtCallbackProc callback, XtPointer closure)
{
  layout_menu_part  *menu_part;
  layout_menu_entry *new_entries;
  int i;

  menu_part = &(lay_out->menu);
  (menu_part->num_entries)++;

  /* Array of pointers. */
  if (menu_part->num_entries == 1 && menu_part->menu_entries == NULL)
    new_entries = (layout_menu_entry *)
      malloc(sizeof(layout_menu_entry) * menu_part->num_entries);
  else
    new_entries = (layout_menu_entry *)
      realloc(menu_part->menu_entries,
	      sizeof(layout_menu_entry) * menu_part->num_entries);

  for (i = 0; i < menu_part->num_entries - 1; i++) {
    new_entries[i] = menu_part->menu_entries[i];
  }

  new_entries[i] = NULL;
  new_entries[i] = Make_entry_layout(new_entries[i], resource_name,
				     label, wid, callback, closure);

  menu_part->menu_entries = new_entries;

  return(new_entries);

} /* Add_entry_layout() */

layout_list Make_list_layout(layout_list lay_out, char *type, char *resource_name, char *shell_resource_name, Widget wid, int id, int num_entries, char **list_entries, XtCallbackProc callback, XtPointer closure)
{
  layout_list_part *list_part;

  if (!lay_out) lay_out = (layout_list) XtNew(layout_list_rec);
  (void) Make_core_part_layout((layout_core) lay_out, type, resource_name,
			       shell_resource_name, wid, id);
  
  list_part                = &(lay_out->list);
  list_part->num_entries   = num_entries;
  list_part->list_entries  = list_entries;
  list_part->callback_list = add_callback_first(NULL, callback, closure);

  return(lay_out);

} /* Make_list_layout() */
