#ifndef COLOR_H
#define COLOR_H

//#include <iostream>
//#include <ostream>
#include <fstream>
#include <string>

#include "geometry.h"
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
    Color(Vec3 v) :
      r_(v.x()),
      g_(v.y()),
      b_(v.z()) {}

    float r() const { return r_; }
    float g() const { return g_; }
    float b() const { return b_; }

    int r255() const { return utilities::clamp(static_cast<int>(r_ * 255.999f), 0, 255); }
    int g255() const { return utilities::clamp(static_cast<int>(g_ * 255.999f), 0, 255); }
    int b255() const { return utilities::clamp(static_cast<int>(b_ * 255.999f), 0, 255); }

    Color& operator +=(const Color &c){
      r_ += c.r_;
      g_ += c.g_;
      b_ += c.b_;
      return *this;
    }

    Color& clamp(const float minVal, float maxVal) {
      r_ = utilities::clamp(r_, minVal, maxVal);
      g_ = utilities::clamp(g_, minVal, maxVal);
      b_ = utilities::clamp(b_, minVal, maxVal);
      return *this;
    }

    std::string to_string() const {
      return "Color: " +
        std::to_string(r_) + " " +
        std::to_string(g_) + " " +
        std::to_string(b_);
    }










    // Friends
    friend inline Color operator+(const Color& c, const float& f);
    friend inline Color operator+(const float& f, const Color& c);
    friend inline Color operator+(const Color& c1, const Color& c2);

    friend inline Color operator-(const Color& c1, const Color& c2);

    friend inline Color operator*(const Color& c, const float& f);
    friend inline Color operator*(const float& f, const Color& c);
    friend inline Color operator*(const Color& c1, const Color& c2);

    friend inline Color operator/(const Color& c, const float& f);
    friend inline Color operator/(const float& f, const Color& c);
};


inline Color operator+(const Color& c, const float& f) {
  return Color(
      c.r_ + f,
      c.g_ + f,
      c.b_ + f);
}
inline Color operator+(const float& f, const Color& c) {
  return c+f;
}
inline Color operator+(const Color& c1, const Color& c2) {
  return Color(
      c1.r_ + c2.r_,
      c1.g_ + c2.g_,
      c1.b_ + c2.b_);
}


inline Color operator-(const Color& c1, const Color& c2) {
  return Color(
      c1.r_ - c2.r_,
      c1.g_ - c2.g_,
      c1.b_ - c2.b_);
}


inline Color operator*(const Color& c, const float& f) {
  return Color(
      c.r_ * f,
      c.g_ * f,
      c.b_ * f);
}
inline Color operator*(const float& f, const Color& c) {
  return c*f;
}
inline Color operator*(const Color& c1, const Color& c2) {
  return Color(
      c1.r_ * c2.r_,
      c1.g_ * c2.g_,
      c1.b_ * c2.b_);
}

inline Color operator/(const Color& c, const float& f) {
  return Color(
      c.r_ / f,
      c.g_ / f,
      c.b_ / f);
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

inline Color pow(const Color& base_c, const Color& exp_c) {
  return Color(
      base_c.r() * exp_c.r(),
      base_c.g() * exp_c.g(),
      base_c.b() * exp_c.b()
      );
}

#endif
