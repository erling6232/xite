#!/bin/sh


#/*P:makepseudo*
#
#________________________________________________________________
#
#		makepseudo
#________________________________________________________________
#
#Name:		makepseudo - Make a pseudocolor image with corresponding
#               colortable from rgb color image
#
#Syntax:	makepseudo <inimage> <outimage> <colortable-file>
#
#Description:	Performs color quantization of an input rgb color image. An
#               output image and a colortable-file are produced.  The values
#               in the output image should be used as indices to the
#               colortable.
#
#		Color quantization is done by 'colorquant(1)', with colorstep
#               equal to 2, while the colortable is built by 'mct(1)'.
#
#               Starting with XITE version 3.2 this can be done in one step by
#               'colorquant(1)'.
#
#Options:	None
#
#See also:	'colorquant(1)', 'mct(1)', color2ascii(1)
#Restrictions:	Input image must be three-band rgb image.
#Author:	Svein Bøe
#Id: 		$Id$
#________________________________________________________________
#
#*/

if test -f /usr/5bin/echo; then
   echo=/usr/5bin/echo
elif test -f /usr/bin/echo; then
   echo=/usr/bin/echo
else
   echo=echo
fi

opt=""; inimage=""; outimage=""; coltab=""; help=0; progName=`basename $0`

$echo "$progName: Use colorquant instead, it is more efficient."

if test -n "$TMPDIR"
then
  tmp=$TMPDIR/$$makepseudo
else
  tmp=/tmp/$$makepseudo
fi

usage="Usage: $progName <inimage> <outimage> <colortable-file>"

eval `xiteStdOpt 3 3 $0 $@ "$usage"`

while test $# -gt 0; do
    case $1 in
        *)  
            if test -n "$inimage" -a -n "$outimage" -a -n "$coltab"; then
                help=1;
                break;
            fi
            if test -z "$inimage"; then
                inimage="$1"
            elif test -z "$outimage"; then
                outimage="$1"
            else
	        coltab="$1"
            fi
            ;;
    esac

    shift
done

if test -z "$inimage" -o -z "$outimage" -o -z "$coltab"; then
    help=1
fi

if test $help -eq 1; then
    $echo "Illegal number of arguments."
    $echo "$usage"
    exit 1
fi

# trap "/bin/rm -f $tmp*; exit" 0 1 2 3 15

colorquant -s 2 $inimage $tmp.map $outimage
if test $? -ne 0; then
    exit 1
fi
mct $tmp.map $coltab
