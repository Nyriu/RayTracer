#ifndef COLOR_H
#define COLOR_H

//#include <iostream>
#include <ostream>
#include <fstream>

//#include <glm/vec3.hpp> // glm::vec3

#include "utilities.h"

class Color {
  private:
    int r_,g_,b_;
    // alpha ?

  public:
    Color() = default;

    Color(int c) : r_(c), g_(c), b_(c) {}
    Color(int r, int g, int b) :
      r_(clamp(r,0,255)),
      g_(clamp(g,0,255)),
      b_(clamp(b,0,255)) {}

    Color(double c) {
      r_ = (clamp(c,0,255));
      g_ = (clamp(c,0,255));
      b_ = (clamp(c,0,255));
    }
    Color(double r, double g, double b) :
      r_(clamp(r*255,0,255)),
      g_(clamp(g*255,0,255)),
      b_(clamp(b*255,0,255)) {}

    Color(float c) {
      r_ = (clamp(c,0,255));
      g_ = (clamp(c,0,255));
      b_ = (clamp(c,0,255));
    }
    Color(float r, float g, float b) :
      r_(clamp(r*255,0,255)),
      g_(clamp(g*255,0,255)),
      b_(clamp(b*255,0,255)) {}

    int r() const { return r_; }
    int g() const { return g_; }
    int b() const { return b_; }

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
  out << pixel_color.r() << " "
      << pixel_color.g() << " "
      << pixel_color.b() << "\n";
}

inline std::ostream& operator<<(std::ostream &out, const Color &c) {
  return out
    << c.r() << " "
    << c.g() << " "
    << c.b() << " ";
}

#endif
