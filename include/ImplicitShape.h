#ifndef IMPLICIT_SHAPE_H
#define IMPLICIT_SHAPE_H

//#include <algorithm>
//#include <cmath>
//#include <memory>
//#include<vector>

#include "geometry.h"
#include "SceneObject.h"
#include "Color.h"
#include <iostream>
#include <iterator>
#include <ostream>

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
      //std::cout <<
      //  "\n ----- SPHERE -----" <<
      //  "\nm:     \n" << matrix_ <<
      //  "\nm_inv: \n" << matrix_inverse_ <<
      //  "\nfrom:  " << from <<
      //  "\np:     " << worldToLocal(from) <<
      //  "\n END ----- SPHERE -----" << std::endl;
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

class Cube : public ImplicitShape {
  private:
    Vec3 half_dims_ = Vec3(.5,.5,.5);

  public:
    Cube() = default;
    Cube(const Vec3& dims) : half_dims_(dims*.5) { color_ = .5; }
    Cube(const Point3& center) {
      translate(center.as_Vec3());
      color_ = .5;
    }
    Cube(const Point3& center, const Vec3& dims) : half_dims_(dims*.5) {
      translate(center.as_Vec3());
      color_ = .5;
    }

    float getDistance(const Point3& from) const {
      Point3 p = worldToLocal(from);
      Point3 abs_p = Point3(std::abs(p.x()), std::abs(p.y()), std::abs(p.z()));

      Vec3 v = abs_p - half_dims_.as_Point();
      float d = Vec3(
          std::max(v.x(),.0f),
          std::max(v.y(),.0f),
          std::max(v.z(),.0f)
          ).length();

      //std::cout <<
      //  "\n ----- CUBE -----" <<
      //  //"\nm:     \n" << matrix_ <<
      //  //"\nm_inv: \n" << matrix_inverse_ <<
      //  "\nfrom:  " << from <<
      //  "\np:     " << p <<
      //  "\nabs_p: " << abs_p <<
      //  "\nh_dims:" << half_dims_ <<
      //  "\nv:     " << v <<
      //  "\nd:     " << d <<
      //  "\n END ----- CUBE -----" << std::endl;

      return d;
    }
};


//class Plane : public ImplicitShape


class CSGShape : public ImplicitShape {
  protected:
   ImplicitShape* shape1_ = nullptr;
   ImplicitShape* shape2_ = nullptr;

   void setShapes(ImplicitShape* shape1, ImplicitShape* shape2) {
     shape1_ = shape1;
     shape2_ = shape2;
     color_ = (0.5 * shape1_->getColor()) + (0.5 * shape2_->getColor());
   }
   void setShapes(ImplicitShape& shape1, ImplicitShape& shape2) {
     setShapes(&shape1, &shape2);
   }

   Color getColor(const Point3& p) const {
     return (shape1_->getDistance(p) < shape2_->getDistance(p)) ? shape1_->getColor() : shape2_->getColor();
   }

   void update() {
     ImplicitShape::update();
     shape1_->update();
     shape2_->update();
   }

};

class UnionShape : public CSGShape {
  public:
    UnionShape(ImplicitShape* shape1, ImplicitShape* shape2) { setShapes(shape1,shape2); }
    UnionShape(ImplicitShape& shape1, ImplicitShape& shape2) { setShapes(shape1,shape2); }

    float getDistance(const Point3& from) const {
      Point3 local(worldToLocal(from));
      return std::min(
          shape1_->getDistance(local),
          shape2_->getDistance(local)
          );
    }
};

class IntersectShape : public CSGShape {
  public:
    IntersectShape(ImplicitShape* shape1, ImplicitShape* shape2) { setShapes(shape1,shape2); }
    IntersectShape(ImplicitShape& shape1, ImplicitShape& shape2) { setShapes(shape1,shape2); }

    float getDistance(const Point3& from) const {
      Point3 local(worldToLocal(from));
      return std::max(
          shape1_->getDistance(local),
          shape2_->getDistance(local)
          );
    }
};

class SubtractShape : public CSGShape {
  public:
    SubtractShape(ImplicitShape* shape1, ImplicitShape* shape2) { setShapes(shape1,shape2); }
    SubtractShape(ImplicitShape& shape1, ImplicitShape& shape2) { setShapes(shape1,shape2); }

    float getDistance(const Point3& from) const {
      Point3 local(worldToLocal(from));
      return std::max(
          shape1_ ->getDistance(local),
          -shape2_->getDistance(local)
          );
    }
};

class SmoothUnionShape : public CSGShape {
  private:
    float smooth_factor_ = 0.1f;

    static float smin( float a, float b, float k ) {
      // from https://www.iquilezles.org/www/articles/smin/smin.htm
      float h = std::max( k-std::abs(a-b), 0.0f )/k;
      return std::min( a, b ) - h*h*k*(1.0f/4.0f);
    }

  public:
    SmoothUnionShape(ImplicitShape* shape1, ImplicitShape* shape2) { setShapes(shape1,shape2); }
    SmoothUnionShape(ImplicitShape& shape1, ImplicitShape& shape2) { setShapes(shape1,shape2); }
    SmoothUnionShape(ImplicitShape* shape1, ImplicitShape* shape2, float smooth_factor) : smooth_factor_(smooth_factor) { setShapes(shape1,shape2); }
    SmoothUnionShape(ImplicitShape& shape1, ImplicitShape& shape2, float smooth_factor) : smooth_factor_(smooth_factor) { setShapes(shape1,shape2); }

    float getDistance(const Point3& from) const {
      Point3 local(worldToLocal(from));
      return smin(
          shape1_->getDistance(local),
          shape2_->getDistance(local),
          smooth_factor_
          );
    }
};


class MixShape : public CSGShape {
  private:
    float mix_factor_ = 0.5f;

    void set_mix_factor(float mix_factor){
      mix_factor_ = mix_factor > 1.f ? 1.f : mix_factor < 0.f ? 0.f : mix_factor;
    }
  public:
    MixShape(ImplicitShape* shape1, ImplicitShape* shape2) { setShapes(shape1,shape2); }
    MixShape(ImplicitShape& shape1, ImplicitShape& shape2) { setShapes(shape1,shape2); }
    MixShape(ImplicitShape* shape1, ImplicitShape* shape2, float mix_factor) { setShapes(shape1,shape2); set_mix_factor(mix_factor); }
    MixShape(ImplicitShape& shape1, ImplicitShape& shape2, float mix_factor) { setShapes(shape1,shape2); set_mix_factor(mix_factor); }

    float getDistance(const Point3& from) const {
      Point3 local(worldToLocal(from));
      return
        mix_factor_       * shape1_->getDistance(local) +
        (1.f-mix_factor_) * shape2_->getDistance(local);
    }
};



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
