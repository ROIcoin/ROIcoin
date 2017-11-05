#!/bin/bash
# create multiresolution windows icon
ICON_SRC=../../src/qt/res/icons/roicoin.png
ICON_DST=../../src/qt/res/icons/roicoin.ico
convert ${ICON_SRC} -resize 16x16 roicoin-16.png
convert ${ICON_SRC} -resize 32x32 roicoin-32.png
convert ${ICON_SRC} -resize 48x48 roicoin-48.png
convert ${ICON_SRC} -resize 64x64 roicoin-64.png
convert ${ICON_SRC} -resize 96x96 roicoin-96.png
convert ${ICON_SRC} -resize 128x128 roicoin-128.png
convert roicoin-16.png roicoin-32.png roicoin-48.png roicoin-64.png roicoin-96.png roicoin-128.png ${ICON_DST}

