#ifndef IMPLICIT_SHAPE_H
#define IMPLICIT_SHAPE_H

#include <algorithm>
#include <cmath>
#include <memory>
#include<vector>
#include "vec3.h"
#include "color.h"

class ImplicitShape {
  public:
    virtual float getDistance(const point3& from) const = 0;
    virtual ~ImplicitShape() {}
};

class Sphere : public ImplicitShape {
  public:
    color color_;
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
  public:
    color color_;
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


//class Plane : public ImplicitShape


//class CSGShape : public ImplicitShape {
//  protected:
//    const ImplicitShape* shape1_;
//    const ImplicitShape* shape2_;
//    CSGShape(const ImplicitShape& shape1, const ImplicitShape& shape2) :
//      shape1_(&shape1), shape2_(&shape2) {}
//};

class UnionShape : public ImplicitShape {
  private:
   std::shared_ptr<ImplicitShape> shape1_;
   std::shared_ptr<ImplicitShape> shape2_;

  public:
    UnionShape(const std::shared_ptr<ImplicitShape> shape1, const std::shared_ptr<ImplicitShape> shape2) :
      shape1_(shape1), shape2_(shape2) {}

    float getDistance(const point3& from) const {
      return std::min(
          shape1_->getDistance(from),
          shape2_->getDistance(from)
          );
    }
};

class IntersectShape : public ImplicitShape {
  private:
   std::shared_ptr<ImplicitShape> shape1_;
   std::shared_ptr<ImplicitShape> shape2_;
  public:
    IntersectShape(const std::shared_ptr<ImplicitShape> shape1, const std::shared_ptr<ImplicitShape> shape2) :
      shape1_(shape1), shape2_(shape2) {}

    float getDistance(const point3& from) const {
      return std::max(
          shape1_->getDistance(from),
          shape2_->getDistance(from)
          );
    }
};

class SubtractShape : public ImplicitShape {
  private:
   std::shared_ptr<ImplicitShape> shape1_;
   std::shared_ptr<ImplicitShape> shape2_;
  public:
    SubtractShape(const std::shared_ptr<ImplicitShape> shape1, const std::shared_ptr<ImplicitShape> shape2) :
      shape1_(shape1), shape2_(shape2) {}

    float getDistance(const point3& from) const {
      return std::max(
          -shape1_->getDistance(from),
          shape2_->getDistance(from)
          );
    }
};

// TODO blend
// TODO mix


//class ExperimShape : public ImplicitShape {
//  private:
//   std::shared_ptr<ImplicitShape> shape1_;
//   //std::shared_ptr<ImplicitShape> shape2_;
//  public:
//    //ExperimShape(const std::shared_ptr<ImplicitShape> shape1, const std::shared_ptr<ImplicitShape> shape2) :
//    ExperimShape(const std::shared_ptr<ImplicitShape> shape1) :
//      shape1_(shape1) {}
//
//    float getDistance(const point3& from) const {
//      return std::fmod(shape1_->getDistance(from), 10.f);
//    }
//};

#endif
