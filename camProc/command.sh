#! /bin/sh

INPUT_NUMBER = 0

./cameraProcessMain "v4l2src device=/dev/video$INPUT_NUMBER use-fixed-fps=false ! ffmpegcolorspace ! capsfilter caps=video/x-raw-rgb,bpp=24,width=320,height=240 ! identity name=artoolkit ! fakesink"