#!/bin/sh

echo=`xiteEcho`

opt=""; outimage=""; help=0; progName=`basename $0`
fsize="41";

usage="Usage: $progName [-x <xsize>] [-y <ysize>] [-l <low_cut_frequency>] \n\
       [-h <high_cut_frequency>] [-f <filter size>] [-t <image title>] [-r] \n\
       <outimage>"

eval `xiteStdOpt 1 14 $0 $@ "$usage"`

while test $# -gt 0; do
    case "$1" in
        -x|-y|-l|-h|-r|-t|-title)
            opt="$opt $1"
            if test $# -gt 1; then
                shift; opt="$opt $1"
            fi;;
        -f) if test $# -gt 1; then
                shift; fsize="$1"
            fi;;
        *)  if test -n "$outimage"; then
                help=1
            else
                outimage=$1
            fi
            ;;
    esac

    shift
done

if test -z "$outimage"; then
    help=1
fi

if test $help -eq 1; then
    $echo "Illegal arguments."
    $echo "$usage"
    exit 1
fi

fsize=`expr $fsize + 2`
opt="$opt -f $fsize -w hm"

bandpass $opt $outimage
