#ifndef IMPLICIT_SHAPE_H
#define IMPLICIT_SHAPE_H

#include <algorithm>
#include <cmath>
#include<vector>
#include "vec3.h"
#include "color.h"

class ImplicitShape {
  public:
    color color_;
  public:
    virtual float getDistance(const point3& from) const = 0;
    virtual ~ImplicitShape() {}
};

class Sphere : public ImplicitShape {
  private:
    point3 center;
    float radius;
  public:
    Sphere(const point3& c, const float& r) : center(c), radius(r) {
      color_ = color(0.5);
    }
    Sphere(const point3& c, const float& r, const color& col) : center(c), radius(r) {
      color_ = col;
    }
    float getDistance(const point3& from) const {
      return length(from - center) - radius;
    }
};

class Torus : public ImplicitShape {
  private:
    point3 center_;
    float r0_, r1_;
  public:
    Torus(const float& r0, const float& r1) : center_(point3(0)), r0_(r0), r1_(r1) {
      color_ = color(.5);
    }
    Torus(const point3& c, const float& r0, const float& r1) : center_(c), r0_(r0), r1_(r1) {
      color_ = color(.5);
    }

    float getDistance(const point3& from) const {
      point3 p = from - center_;
      // to 2D plane
      float tmpx = std::sqrt(p.x * p.x + p.z * p.z) - r0_;
      float tmpy = p.y;
      return std::sqrt(tmpx * tmpx + tmpy * tmpy) - r1_;
    }

    //~Torus() {}
};

//class Cube : public ImplicitShape {
//  private:
//    point3 corner_;
//  public:
//    Cube(const point3& corner) : corner_(corner) {}
//    float getDistance(const point3& from) const {
//      //vec3 d = from - corner_;
//      vec3 d = from;
//      point3 dmax = d;
//      dmax.x = std::max(dmax.x, 0.f);
//      dmax.y = std::max(dmax.y, 0.f);
//      dmax.z = std::max(dmax.z, 0.f);
//
//      //float scale = 2;
//      return (std::min(std::max(d.x, std::max(d.y,d.z)), 0.f) + length(dmax));
//    }
//};



#endif
