#ifndef UTILITIES_H
#define UTILITIES_H

#include <iostream>
#include <algorithm>
#include <iterator>


inline float clamp(float val, float minVal, float maxVal) {
  return std::min(std::max(val, minVal), maxVal);
}

inline void DEBUG_message(const bool& isToShow, const char* message) {
  if (isToShow)
    std::cout << message << std::endl;
}

#endif
