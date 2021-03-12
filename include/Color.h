#ifndef COLOR_H
#define COLOR_H

//#include <iostream>
//#include <ostream>
#include <fstream>

//#include <glm/vec3.hpp> // glm::vec3

#include "utilities.h"

class Color {
  private:
    float r_,g_,b_;
    // alpha ?

  public:
    Color() = default;

    Color(float c) {
      r_ = c;
      g_ = c;
      b_ = c;
    }
    Color(float r, float g, float b) :
      r_(r),
      g_(g),
      b_(b) {}

    float r() const { return r_; }
    float g() const { return g_; }
    float b() const { return b_; }

    int r255() const { return clamp(static_cast<int>(r_ * 255.999f), 0, 255); }
    int g255() const { return clamp(static_cast<int>(g_ * 255.999f), 0, 255); }
    int b255() const { return clamp(static_cast<int>(b_ * 255.999f), 0, 255); }

    Color& operator +=(const Color &c){
      r_ += c.r_;
      g_ += c.g_;
      b_ += c.b_;
      return *this;
    }
};


inline Color operator+(const Color& c, const float& f) {
  return Color(
      c.r() + f,
      c.g() + f,
      c.b() + f);
}
inline Color operator+(const float& f, const Color& c) {
  return c+f;
}

inline Color operator*(const Color& c, const float& f) {
  return Color(
      c.r() * f,
      c.g() * f,
      c.b() * f);
}
inline Color operator*(const float& f, const Color& c) {
  return c*f;
}

inline Color operator/(const Color& c, const float& f) {
  return Color(
      c.r() / f,
      c.g() / f,
      c.b() / f);
}
inline Color operator/(const float& f, const Color& c) {
  return c/f;
}


inline void write_color(std::ofstream &out, const Color& pixel_color) {
  out << pixel_color.r255() << " "
      << pixel_color.g255() << " "
      << pixel_color.b255() << "\n";
}

inline std::ostream& operator<<(std::ostream &out, const Color &c) {
  return out
    << c.r255() << " "
    << c.g255() << " "
    << c.b255() << " ";
}

#endif
