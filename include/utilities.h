#ifndef UTILITIES_H
#define UTILITIES_H

#include <algorithm>


inline float clamp(float val, float minVal, float maxVal) {
  return std::min(std::max(val, minVal), maxVal);
}

 #endif
