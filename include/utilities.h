#ifndef UTILITIES_H
#define UTILITIES_H

#include <iostream>
#include <algorithm>
#include <iterator>
#include <string>
#include "geometry.h"

namespace utilities {

inline float clamp(const float val, const float minVal, const float maxVal) {
  return std::min(std::max(val, minVal), maxVal);
}
inline float clamp_lower(const float val, const float minVal) {
  return std::max(val, minVal);
}

inline Vec3 clamp(const Vec3 v, const float minVal, const float maxVal) {
  return Vec3(
    clamp(v.x(), minVal, maxVal),
    clamp(v.y(), minVal, maxVal),
    clamp(v.z(), minVal, maxVal)
      );
}
inline Vec3 clamp(const Vec3 v, const Vec3 minVal, const Vec3 maxVal) {
  Vec3 clamped(
    clamp(v.x(), minVal.x(), maxVal.x()),
    clamp(v.y(), minVal.y(), maxVal.y()),
    clamp(v.z(), minVal.z(), maxVal.z())
      );
  return clamped;
}

inline Vec3 round(const Vec3 v) {
  return Vec3(
    std::round(v.x()),
    std::round(v.y()),
    std::round(v.z())
      );
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
