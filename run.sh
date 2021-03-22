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

rm -fr wip_imgs/*

./main

viewnior wip_imgs/seq_0.ppm
#viewnior ./wip_imgs/img.ppm &

rm -fr wip_imgs/*
