#!/bin/sh

if [ "$1" == "-b" ]; then
  rm -rf ./build ./main
fi

if [ -d "./build" ]; then
  cd build
  make
  cd ..
fi

if [ ! -f "./main" ]; then
  echo "Main not found!"
  echo "Building..."
  rm -rf build main
  mkdir build
  cd build
  cmake ..
  make
  cd ..
fi

rm -fr wip_imgs
mkdir wip_imgs

./main

viewnior wip_imgs/seq_0.ppm
