#!/bin/sh

if test -f /usr/5bin/echo; then
   echo=/usr/5bin/echo
elif test -f /usr/bin/echo; then
   echo=/usr/bin/echo
else
   echo=echo
fi

opt=""; inimage=""; outimage=""; help=0; progName=`basename $0`

usage="Usage: $progName [-f] [-d] <inimage> <outimage>"

eval `xiteStdOpt 2 4 $0 $@ "$usage"`

while test $# -gt 0; do
    case "$1" in
        -f|-d)
            opt="$opt $1"
            ;;
        *)
            if test -n "$inimage" -a -n "$outimage"; then
                help=1;
                break;
            fi
            if test -z "$inimage"; then
                inimage="$1"
            else
                outimage="$1"
            fi
            ;;
    esac

    shift
done

if test -z "$inimage" -o -z "$outimage"; then
    help=1
fi

if test $help -eq 1; then
    $echo "Illegal number of arguments."
    $echo "$usage"
    exit 1
fi

real $opt $inimage $outimage
