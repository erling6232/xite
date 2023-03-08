/*

________________________________________________________________

        layout.h
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

#ifndef _layout_H_
#define _layout_H_

_XITE_CPLUSPLUS_BEGIN

#include <X11/Intrinsic.h>
#include <xite/biff.h>
#include <xite/callbacks.h>

/****************** core, label ******************/

typedef struct {
  char   *type;
  char   *resource_name;
  char   *shell_resource_name;
  Widget wid;
  int    id;
} layout_core_part;

typedef struct {
  layout_core_part core;
} layout_core_rec;

typedef layout_core_rec *layout_core, *layout, *layout_label;
typedef layout_core_rec layout_rec, layout_label_rec;

#define TypeOfLayout(l_out)              ((l_out)->core.type)
#define ResourceNameOfLayout(l_out)      ((l_out)->core.resource_name)
#define ShellResourceNameOfLayout(l_out) ((l_out)->core.shell_resource_name)
#define WidgetOfLayout(l_out)            ((l_out)->core.wid)
#define IdOfLayout(l_out)                ((l_out)->core.id)
#define CoreOfLayout(l_out)              ((l_out)->core)

/****************** command ******************/

typedef struct {
  XtCallbackList callback_list;
} layout_command_part;

typedef struct {
  layout_core_part    core;
  layout_command_part command;
} layout_command_rec;

typedef layout_command_rec *layout_command;

#define CallbackOfCommandLayout(l_out)   \
    (((layout_command) l_out)->command.callback_list)

/****************** prompt ******************/

typedef struct {
  XtCallbackList callback_list;
  char *text;
} layout_prompt_part;

typedef struct {
  layout_core_part   core;
  layout_prompt_part prompt;
} layout_prompt_rec;

typedef layout_prompt_rec *layout_prompt;

#define CallbackOfPromptLayout(l_out)    \
    (((layout_prompt) l_out)->prompt.callback_list)
#define TextOfPromptLayout(l_out)        \
    (((layout_prompt) l_out)->prompt.text)

/****************** menu ******************/

typedef struct {
  char *resource_name, *label;
  Widget wid;
  XtCallbackList callback_list;
} layout_menu_entry_rec, *layout_menu_entry;

#define ResourceNameOfMenuEntry(entry)   ((entry)->resource_name)
#define LabelOfMenuEntry(entry)          ((entry)->label)
#define WidgetOfMenuEntry(entry)         ((entry)->wid)
#define CallbackOfMenuEntry(entry)       ((entry)->callback_list)

typedef struct {
  int num_entries;
  /* Pointer to first element in array of pointers. */
  layout_menu_entry *menu_entries;
} layout_menu_part;

typedef struct {
  layout_core_part core;
  layout_menu_part menu;
} layout_menu_rec;

typedef layout_menu_rec *layout_menu;

#define NumEntriesOfMenuLayout(l_out)    \
    (((layout_menu) l_out)->menu.num_entries)
#define EntriesOfMenuLayout(l_out)       \
    (((layout_menu) l_out)->menu.menu_entries)

/****************** scroll ******************/

typedef struct {
  XtCallbackList jump_callback_list;
  XtCallbackList scroll_callback_list;
} layout_scroll_part;

typedef struct {
  layout_core_part core;
  layout_scroll_part scroll;
} layout_scroll_rec;

typedef layout_scroll_rec *layout_scroll;

#define JumpCallbackOfScrollLayout(l_out) \
    (((layout_scroll) l_out)->scroll.jump_callback_list)
#define ScrollCallbackOfScrollLayout(l_out) \
    (((layout_scroll) l_out)->scroll.scroll_callback_list)

/****************** list ******************/

typedef struct {
  XtCallbackList callback_list;
  int num_entries;
  char **list_entries;
} layout_list_part;

typedef struct {
  layout_core_part core;
  layout_list_part list;
} layout_list_rec;

typedef layout_list_rec *layout_list;

#define NumEntriesOfListLayout(l_out)    \
    (((layout_list) l_out)->list.num_entries)
#define EntriesOfListLayout(l_out)       \
    (((layout_list) l_out)->list.list_entries)
#define CallbackOfListLayout(l_out)   \
    (((layout_list) l_out)->list.callback_list)

/****************** image ******************/

typedef struct {
  IMAGE img;
  image_callbacks *callbacks;
} layout_image_part;

typedef struct {
  layout_core_part  core;
  layout_image_part image;
} layout_image_rec;

typedef layout_image_rec *layout_image;

#define ImgOfImageLayout(l_out)          (((layout_image) l_out)->image.img)
#define CallbacksOfImageLayout(l_out)    \
    (((layout_image) l_out)->image.callbacks)
#define DestroyCallbackOfImageLayout(l_out) \
    (((layout_image) l_out)->image.callbacks->destroy_cb)
#define CallbackCallbackOfImageLayout(l_out) \
    (((layout_image) l_out)->image.callbacks->callback_cb)
#define ResizeCallbackOfImageLayout(l_out) \
    (((layout_image) l_out)->image.callbacks->resize_cb)
#define DrawCallbackOfImageLayout(l_out) \
    (((layout_image) l_out)->image.callbacks->draw_cb)
#define RoiCallbackOfImageLayout(l_out) \
    (((layout_image) l_out)->image.callbacks->roi_cb)


extern layout_label Make_core_part_layout _XITE_PARAMS(( layout_core lay_out, char *type, char *resource_name, char *shell_resource_name, Widget wid, int id ));
extern layout_label Make_label_layout _XITE_PARAMS(( layout_label lay_out, char *type, char *resource_name, char *shell_resource_name, Widget wid, int id ));
extern layout_image Make_image_layout _XITE_PARAMS(( layout_image lay_out, char *type, char *resource_name, char *shell_resource_name, Widget wid, int id, IMAGE img, image_callbacks *callbacks ));
extern void add_callback_to_image_layout _XITE_PARAMS(( layout_image lay_out, String resource_name, XtCallbackProc callback, XtPointer closure, Boolean first ));
extern layout_command Make_command_layout _XITE_PARAMS(( layout_command lay_out, char *type, char *resource_name, char *shell_resource_name, Widget wid, int id, XtCallbackProc callback, XtPointer closure ));
extern layout_prompt Make_prompt_layout _XITE_PARAMS(( layout_prompt lay_out, char *type, char *resource_name, char *shell_resource_name, Widget wid, int id, XtCallbackProc callback, XtPointer closure, char *text ));
extern layout_scroll Make_scroll_layout _XITE_PARAMS(( layout_scroll lay_out, char *type, char *resource_name, char *shell_resource_name, Widget wid, int id, XtCallbackProc jump_callback, XtPointer jump_closure, XtCallbackProc scroll_callback, XtPointer scroll_closure ));
extern layout_menu Make_menu_layout _XITE_PARAMS(( layout_menu lay_out, char *type, char *resource_name, char *shell_resource_name, Widget wid, int id, int num_entries, layout_menu_entry *menu_entries ));
extern layout_menu_entry Make_entry_layout _XITE_PARAMS(( layout_menu_entry lay_out, char *resource_name, char *label, Widget wid, XtCallbackProc callback, XtPointer closure ));
extern layout_menu_entry *Add_entry_layout _XITE_PARAMS(( layout_menu lay_out, char *resource_name, char *label, Widget wid, XtCallbackProc callback, XtPointer closure ));
extern layout_list Make_list_layout _XITE_PARAMS(( layout_list lay_out, char *type, char *resource_name, char *shell_resource_name, Widget wid, int id, int num_entries, char **list_entries, XtCallbackProc callback, XtPointer closure ));

_XITE_CPLUSPLUS_END

#endif /* _layout_H_ */

