#!/bin/sh

if test -f /usr/5bin/echo; then
   echo=/usr/5bin/echo
elif test -f /usr/bin/echo; then
   echo=/usr/bin/echo
else
   echo=echo
fi

opt=""; inimage1=""; inimage2=""; outimage=""; help=0; progName=`basename $0`

usage="Usage: $progName <inimage1> <inimage2> <outimage>"

eval `xiteStdOpt 3 3 $0 $@ "$usage"`

while test $# -gt 0; do
    case "$1" in
        *)
            if test -n "$inimage1" -a -n "$inimage2" -a -n "$outimage"; then
                help=1;
                break;
            fi
            if test -z "$inimage1"; then
                inimage1="$1"
            elif test -z "$inimage2"; then
	        inimage2="$1"
            else
                outimage="$1"
            fi
            ;;
    esac

    shift
done

if test -z "$inimage1" -o -z "$inimage2" -o -z "$outimage"; then
    help=1
fi

if test $help -eq 1; then
    $echo "Illegal number of arguments."
    $echo "$usage"
    exit 1
fi

multiply $inimage1 $inimage2 $outimage
