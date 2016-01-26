#!/bin/bash

IMAGEMAGICK=$(which convert)

if [ -z $IMAGEMAGICK ]; then
	echo "Please install ImageMagick first."
	exit 1
fi

if [ -z $1 ]; then
	echo "usage: $(basename "$0") image.{gif,jpg,png...}"
	exit 1
fi

$IMAGEMAGICK $1 -channel BGR -separate -channel RGB -combine -rotate 90 bgr:${1%.*}.bin
