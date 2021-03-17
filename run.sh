#!/bin/sh

#if [ "$1" == "-b" ]; then
  #rm -rf build main imgs/*
  rm -rf build main
  rm -f ./imgs/img.ppm
  mkdir build
  cd build
  cmake ..
  make
  cd ..
#fi

#if [ ! -f "./main" ]; then
#  echo "main not found!"
#  echo "building..."
#  rm -rf build
#  mkdir build
#  cd build
#  cmake ..
#  make
#  cd ..
#fi

./main

#viewnior ./imgs/img.ppm &

#convert ./imgs/img.ppm ./imgs/img.png
#feh ./imgs/img.png
#feh --scale-down --auto-zoom ./imgs/img.png
