/* Generated by wbuild from "Prompt.w"
** (generator version $Revision$ of $Date$)
*/
#ifndef _XfwfPrompt_H_
#define _XfwfPrompt_H_
#include <Xfwf/Label.h>
typedef enum {
		XfwfReject, XfwfAccept, XfwfComplete
	} XfwfPromptInfoResult;

typedef struct {
		String oldstring;
		String newstring;
		Boolean result;
	} XfwfPromptInfo;

#ifndef XtNtext
#define XtNtext "text"
#endif
#ifndef XtCText
#define XtCText "Text"
#endif
#ifndef XtRString
#define XtRString "String"
#endif

#ifndef XtNtemplate
#define XtNtemplate "template"
#endif
#ifndef XtCTemplate
#define XtCTemplate "Template"
#endif
#ifndef XtRString
#define XtRString "String"
#endif

#ifndef XtNpattern
#define XtNpattern "pattern"
#endif
#ifndef XtCPattern
#define XtCPattern "Pattern"
#endif
#ifndef XtRString
#define XtRString "String"
#endif

#ifndef XtNvalidate
#define XtNvalidate "validate"
#endif
#ifndef XtCValidate
#define XtCValidate "Validate"
#endif
#ifndef XtRCallback
#define XtRCallback "Callback"
#endif

#ifndef XtNleave
#define XtNleave "leave"
#endif
#ifndef XtCLeave
#define XtCLeave "Leave"
#endif
#ifndef XtRCallback
#define XtRCallback "Callback"
#endif

#ifndef XtNactivate
#define XtNactivate "activate"
#endif
#ifndef XtCActivate
#define XtCActivate "Activate"
#endif
#ifndef XtRCallback
#define XtRCallback "Callback"
#endif

#ifndef XtNcomplete
#define XtNcomplete "complete"
#endif
#ifndef XtCComplete
#define XtCComplete "Complete"
#endif
#ifndef XtRBoolean
#define XtRBoolean "Boolean"
#endif

#ifndef XtNplaceholder
#define XtNplaceholder "placeholder"
#endif
#ifndef XtCPlaceholder
#define XtCPlaceholder "Placeholder"
#endif
#ifndef XtRChar
#define XtRChar "Char"
#endif

typedef struct _XfwfPromptClassRec *XfwfPromptWidgetClass;
typedef struct _XfwfPromptRec *XfwfPromptWidget;
externalref WidgetClass xfwfPromptWidgetClass;
#endif /*_XfwfPrompt_H_*/
