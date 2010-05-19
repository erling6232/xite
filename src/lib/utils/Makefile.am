#
#      Makefile.am for lib/utils
#


@SET_MAKE@

SHELL = /bin/sh

include $(top_srcdir)/Makefile.def

#
#   Makefile definitions
#

noinst_LTLIBRARIES = libxiteutils.la

MANF =  compute_line.3 compute_answer.3 compute_define.3 compute_error.3 \
	compute.3 prepend_debug_blanks.3 leave_function_debug.3 \
	enter_function_debug.3 envfindfilemulti.3 envfindfile.3 \
	realloc_string.3 NextFilename.3 FilenamePart.3 gaussRandom.3 \
	MallocFloatP.3 MallocIntP.3 InitMessage.3 PushMessage.3 \
	PopMessage.3 Info.3 Message.3 Warning.3 Error.3 Usage.3 \
	Verbose.3 ExitOnError.3 LastMessage.3 MessageStream.3 \
	DefaultMessage.3 DefaultNoMessage.3 \
	xite_standard_options_usage_text.3 xite_app_std_usage_text.3 \
	message.3 read_bswitch.3 read_iswitch.3 read_dswitch.3 \
	read_switch.3 argvOptions.3 split_cmd_line.3 iindex.3 \
	xite_pipe_filedes.3 xiteFilenameType.3 tilde_expand.3 \
	Mmatrix_3d.3 Mmatrix_2d.3 Mmatrix_1d.3 Fmatrix_3d.3 \
	Fmatrix_2d.3 Fmatrix_1d.3 utl_malloc.3

man3_MANS=$(MANF)

libxiteutils_la_SOURCES = mallocp.c envfindfile.c readswitch.c \
	tilde_expand.c gaussfunc.c utl_malloc.c message.c compu4.c \
	debug.c strings.c

compute_line.3:	compute.3
compute_answer.3:	compute.3
compute_define.3:	compute.3
compute_error.3:	compute.3
compute.3:	compu4.3
prepend_debug_blanks.3:	enter_function_debug.3
leave_function_debug.3:	enter_function_debug.3
enter_function_debug.3:	debug.3
envfindfilemulti.3:	envfindfile.3
envfindfile.3:	envfindfile.3
realloc_string.3:	envfindfile.3
NextFilename.3:	envfindfile.3
FilenamePart.3:	envfindfile.3
gaussRandom.3:	gaussfunc.3
MallocFloatP.3:	MallocIntP.3
MallocIntP.3:	mallocp.3
InitMessage.3:	message.3
PushMessage.3:	message.3
PopMessage.3:	message.3
Info.3:	message.3
Message.3:	message.3
Warning.3:	message.3
Error.3:	message.3
Usage.3:	message.3
Verbose.3:	message.3
ExitOnError.3:	message.3
LastMessage.3:	message.3
MessageStream.3:	message.3
DefaultMessage.3:	message.3
DefaultNoMessage.3:	message.3
xite_standard_options_usage_text.3:	message.3
xite_app_std_usage_text.3:	message.3
read_bswitch.3:	read_switch.3
read_iswitch.3:	read_switch.3
read_dswitch.3:	read_switch.3
read_switch.3:	readswitch.3
argvOptions.3:	readswitch.3
split_cmd_line.3:	readswitch.3
iindex.3:	strings.3
xite_pipe_filedes.3:	tilde_expand.3
xiteFilenameType.3:	tilde_expand.3
Mmatrix_3d.3:	utl_malloc.3
Mmatrix_2d.3:	utl_malloc.3
Mmatrix_1d.3:	utl_malloc.3
Fmatrix_3d.3:	utl_malloc.3
Fmatrix_2d.3:	utl_malloc.3
Fmatrix_1d.3:	utl_malloc.3
