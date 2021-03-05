if [ $1 == "-b" ]; then
  rm -rf build main imgs/*
  mkdir build
  cd build
  cmake ..
  make
  cd ..
fi

./main
viewnior ./imgs/img.ppm
