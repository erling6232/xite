#!/bin/sh


#/*P:xiteStdOpt*
#
#________________________________________________________________
#
#		xiteStdOpt
#________________________________________________________________
#
#Name:		xiteStdOpt - process standard XITE options
#
#Syntax:	xiteStdOpt <min_opts> <max_opts> <program-name>
#                   [<option>...] <usage>
#
#Description:	Process any standard XITE options, given by 'option'.
#
#               Standard XITE options are those treated by 'InitMessage'.
#               'usage' is the program usage text.
#
#               'xiteStdOpt' is a shell script, only to be used by programs
#               which themselves are shell scripts.
#
#               The calling shell should use 'eval' on the output from
#               'xiteStdOpt'.
#
#               'min_opts' is the minimum number of words required in the
#               command calling 'program-name' (not counting 'program-name'
#               itself).
#
#               'max_opts' is the maximum number of words allowed in the
#               command calling 'program-name' (not counting 'program-name'
#               itself). A value of -1 means no limit.
#
#Restrictions:  The shell (/bin/sh) command 'set' must accept the argument
#               '--'.
#
#Output:	An sh command which should be evaluated by the calling shell.
#               Help/usage/man output is printed to stderr.
#
#Return value:  &0
#               No action because of standard options. Remaining options
#               prepended with the text "set args " is printed to stdout.
#
#               &1
#               One of the options '-help', '-usage', '-man' or '-whatis'
#               found. Result is printed to stderr, "exit 1" is printed to
#               stdout.
#
#               &2
#               Illegal use of xiteStdOpt. Error message is printed to stderr,
#               "exit 1" is printed to stdout.
#
#See also:	InitMessage(3)
#
#Example:       From inside an XITE script:
#               | eval `xiteStdOpt 2 4 $0 $@ "$usage"`
#               where usage is a variable containing the usage text.
#
#Author:	Svein Bøe
#Id: 		$Id$
#________________________________________________________________
#
#*/

echo=`xiteEcho`

if test $# -lt 4; then
    $echo "xiteStdOpt: Missing arguments." 1>&2
    $echo "exit 1"
    exit 2
fi

num_args=$#;
min_opts=$1; shift; max_opts=$1; shift; progName=`basename $1`; shift
if test $max_opts -eq -1; then
    max_opts=$num_args;
fi

opts=""; numOpts=0; help=0; man=0; whatis=0;

while test $# -gt 1; do
    case "$1" in
        -help|-usage)
            help=1
            ;;
        -man)
            man=1
            ;;
        -whatis)
            whatis=1
            ;;
        *)  
            opts="$opts $1"
            numOpts=`expr $numOpts + 1`
            ;;
    esac

    shift
done

if test $man -eq 1; then
    man $progName 1>&2
    $echo "exit 1"
    exit 1
fi

if test $whatis -eq 1; then
    grep "^${progName} (1)" $XITE_MAN/whatis 1>&2
    $echo "exit 1"
    exit 1
fi

if test $numOpts -eq 0 -a $min_opts -gt 0; then
    help=1
fi

std_xite_opts="  Standard XITE options: \n\
    -help    : Print usage text for program and exit.\n\
    -usage   : Print usage text for program and exit.\n\
    -man     : Print man page for program and exit.\n\
    -whatis  : Print one-line man page description for program and exit.\n\
    -verbose : Set the verbose flag.\n";

usage="$1\n${std_xite_opts}"

if test $help -eq 0 -a \
  \( $numOpts -lt $min_opts -o $numOpts -gt $max_opts \); then
    help=1
    usage="Illegal number of arguments.\n$usage"
fi

if test $help -eq 1; then
    if test -n "$usage"; then
        $echo "$usage" 1>&2
    fi
    $echo "exit 1"
    exit 1
fi

if test -n "$opts"; then
    $echo "set -- $opts"
fi

exit 0
