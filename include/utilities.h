#ifndef UTILITIES_H
#define UTILITIES_H

#include <iostream>
#include <algorithm>
#include <iterator>
#include <string>

namespace utilities {

inline float clamp(const float val, const float minVal, const float maxVal) {
  return std::min(std::max(val, minVal), maxVal);
}
inline float clamp_lower(const float val, const float minVal) {
  return std::max(val, minVal);
}

inline float lerp(const float& x, const float& y, const float& s) {
  return x * (1-s) + y * s;
}


inline void DEBUG_message(const bool& isToShow, const char* message) {
  if (isToShow)
    std::cout << message << std::endl;
}
inline void DEBUG_message(const bool& isToShow, const std::string& message) {
  if (isToShow)
    std::cout << message << std::endl;
}


}
#endif
