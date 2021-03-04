if [ $1 == "-b" ]; then
  rm -rf build main
  mkdir build
  cd build
  cmake ..
  make
  cd ..
fi

./main
viewnior ./imgs/img.ppm
