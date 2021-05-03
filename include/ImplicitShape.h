#ifndef IMPLICIT_SHAPE_H
#define IMPLICIT_SHAPE_H

//#include <algorithm>
//#include <cmath>
//#include <memory>
//#include<vector>

#include "geometry.h"
#include "SceneObject.h"
#include "Color.h"
#include <iterator>

class ImplicitShape : public SceneObject {
  protected:
    Color color_ = Color(0.3);

  private:
    float gradient_delta_ = 10e-6; // delta used to compute gradient (normal)

  public:
    //virtual float getDistance(const Point3& from) const = 0;
    virtual float getDistance(const Point3& from) const { return 0; }
    virtual ~ImplicitShape() {}

    //virtual void setColor(const Color& color) { color_ = color; }
    //virtual void setColor(const float r, const float g, const float b) { color_ = Color(r,g,b); }
    virtual ImplicitShape setColor(const Color& color) {
      color_ = color;
      return *this;
    }
    virtual ImplicitShape setColor(const float r, const float g, const float b) {
      color_ = Color(r,g,b);
      return *this;
    }

    virtual Color getColor() const { return color_; }
    virtual Color getColor(const Point3& p) const { return getColor(); }

    Vec3 getNormalAt(const Point3& p) const {
      return Vec3(
          getDistance(
            p+Vec3(gradient_delta_,0,0)) - getDistance(p + Vec3(-gradient_delta_,0,0)),
          getDistance(
            p+Vec3(0,gradient_delta_,0)) - getDistance(p + Vec3(0,-gradient_delta_,0)),
          getDistance(
            p+Vec3(0,0,gradient_delta_)) - getDistance(p + Vec3(0,0,-gradient_delta_))
          ).normalize();
    }
};

class Sphere : public ImplicitShape {
  private:
    float radius_;
  public:
    Sphere(const float& radius) : radius_(radius) {
      color_ = 0.5;
    }
    Sphere(const Point3& center, const float& radius) : radius_(radius) {
      translate(center.as_Vec3());
      color_ = 0.5;
    }
    Sphere(const Point3& center, const float& radius, const Color& color) : radius_(radius) {
      translate(center.as_Vec3());
      color_ = color;
    }
    Sphere(const float& radius, const Color& color) : radius_(radius) {
      color_ = color;
    }

    float getDistance(const Point3& from) const {
      return (worldToLocal(from)).length() - radius_;
    }
};

class Torus : public ImplicitShape {
  private:
    float r0_, r1_;
  public:
    Torus(const float& r0, const float& r1) : r0_(r0), r1_(r1) {
      color_ = .5;
    }
    Torus(const Point3& center, const float& r0, const float& r1) : r0_(r0), r1_(r1) {
      translate(center.as_Vec3());
      color_ = .5;
    }

    float getDistance(const Point3& from) const {
      Point3 p = worldToLocal(from);
      // to 2D plane
      float tmpx = std::sqrt(p.x() * p.x() + p.z() * p.z()) - r0_;
      float tmpy = p.y();
      return std::sqrt(tmpx * tmpx + tmpy * tmpy) - r1_;
    }

    //~Torus() {}
};

//class Cube : public ImplicitShape {
//  private:
//    //point3 corner_;
//  public:
//    //Cube(const point3& corner) : corner_(corner) {}
//    float getDistance(const Point3& from) const {
//      //vec3 d = from - corner_;
//      Vec3 d = from - Point3(1);
//      Point3 dmax(
//          std::max(d.x(), 0.f),
//          std::max(d.y(), 0.f),
//          std::max(d.z(), 0.f)
//          );
//
//      //float scale = 2;
//      return (std::min(std::max(d.x(), std::max(d.y(),d.z())), 0.f) + dmax.length());
//    }
//};


//class Plane : public ImplicitShape


class CSGShape : public ImplicitShape {
  protected:
   const ImplicitShape* shape1_ = nullptr;
   const ImplicitShape* shape2_ = nullptr;

   void setShapes(const ImplicitShape* shape1, const ImplicitShape* shape2) {
     shape1_ = shape1;
     shape2_ = shape2;
     color_ = (0.5 * shape1_->getColor()) + (0.5 * shape2_->getColor());
   }
   void setShapes(const ImplicitShape& shape1, const ImplicitShape& shape2) {
     setShapes(&shape1, &shape2);
   }

   Color getColor(const Point3& p) const {
     return (shape1_->getDistance(p) < shape2_->getDistance(p)) ? shape1_->getColor() : shape2_->getColor();
   }

};

class UnionShape : public CSGShape {
  public:
    UnionShape(const ImplicitShape* shape1, const ImplicitShape* shape2) { setShapes(shape1,shape2); }
    UnionShape(const ImplicitShape& shape1, const ImplicitShape& shape2) { setShapes(shape1,shape2); }

    float getDistance(const Point3& from) const {
      return std::min(
          shape1_->getDistance(from),
          shape2_->getDistance(from)
          );
    }
};

class IntersectShape : public CSGShape {
  public:
    IntersectShape(const ImplicitShape* shape1, const ImplicitShape* shape2) { setShapes(shape1,shape2); }
    IntersectShape(const ImplicitShape& shape1, const ImplicitShape& shape2) { setShapes(shape1,shape2); }

    float getDistance(const Point3& from) const {
      return std::max(
          shape1_->getDistance(from),
          shape2_->getDistance(from)
          );
    }
};

class SubtractShape : public CSGShape {
  public:
    SubtractShape(const ImplicitShape* shape1, const ImplicitShape* shape2) { setShapes(shape1,shape2); }
    SubtractShape(const ImplicitShape& shape1, const ImplicitShape& shape2) { setShapes(shape1,shape2); }

    float getDistance(const Point3& from) const {
      return std::max(
          shape1_->getDistance(from),
          -shape2_->getDistance(from)
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
