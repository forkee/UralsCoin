#!/bin/bash
# create multiresolution windows icon
ICON_SRC=../../src/qt/res/icons/urals.png
ICON_DST=../../src/qt/res/icons/urals.ico
convert ${ICON_SRC} -resize 16x16 urals-16.png
convert ${ICON_SRC} -resize 32x32 urals-32.png
convert ${ICON_SRC} -resize 48x48 urals-48.png
convert urals-16.png urals-32.png urals-48.png ${ICON_DST}

