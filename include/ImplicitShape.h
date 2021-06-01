#ifndef IMPLICIT_SHAPE_H
#define IMPLICIT_SHAPE_H

//#include <algorithm>
//#include <cmath>
//#include <memory>
//#include<vector>

#include "geometry.h"
#include "utilities.h"
#include "SceneObject.h"
#include "Color.h"
#include <iostream>
#include <iterator>
#include <ostream>

class ImplicitShape : public SceneObject {
  protected:
    Color cdiff_ = Color(0.5);
    Color cspec_ = Color(0.04);
    float shininess_factor_ = 2;

  private:
    float gradient_delta_ = 10e-6; // delta used to compute gradient (normal)

  public:
    //virtual float getDistance(const Point3& from) const = 0;
    virtual float getDistance(const Point3& from) const { return 0; }
    virtual ~ImplicitShape() {}

    //virtual void setColor(const Color& color) { color_ = color; }
    //virtual void setColor(const float r, const float g, const float b) { color_ = Color(r,g,b); }
    virtual ImplicitShape setAlbedo(const Color& color) {
      cdiff_ = color;
      return *this;
    }
    virtual ImplicitShape setSpecular(const Color& color) {
      cspec_ = color;
      return *this;
    }
    virtual ImplicitShape setShininess(float shininess) {
      shininess_factor_ = shininess;
      return *this;
    }

    virtual Color getAlbedo() const { return cdiff_; }
    virtual Color getSpecular() const { return cspec_; }
    virtual float getShininess() const { return shininess_factor_; }

    virtual Color getAlbedo(const Point3& p) const { return getAlbedo(); }
    virtual Color getSpecular(const Point3& p) const { return getSpecular(); }
    virtual float getShininess(const Point3& p) const { return getShininess(); }

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
    Sphere(const float& radius) : radius_(radius) {}
    Sphere(const Point3& center, const float& radius) : radius_(radius) { translate(center.as_Vec3()); }
    Sphere(const Point3& center, const float& radius, const Color& color) : radius_(radius) {
      translate(center.as_Vec3());
      setAlbedo(color);
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
    Torus(const float& r0, const float& r1) : r0_(r0), r1_(r1) {}
    Torus(const Point3& center, const float& r0, const float& r1) : r0_(r0), r1_(r1) {
      translate(center.as_Vec3());
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
    Cube(const Vec3& dims) : half_dims_(dims*.5) {}
    Cube(const Point3& center) { translate(center.as_Vec3()); }
    Cube(const Point3& center, const Vec3& dims) : half_dims_(dims*.5) {
      translate(center.as_Vec3());
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


/////////////////////////////////////////////////////////////////////////////////////////////////
//// CSG ////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////
class CSGShape : public ImplicitShape {
  protected:
   ImplicitShape* shape1_ = nullptr;
   ImplicitShape* shape2_ = nullptr;

   void setShapes(ImplicitShape* shape1, ImplicitShape* shape2) {
     shape1_ = shape1;
     shape2_ = shape2;
   }
   void setShapes(ImplicitShape& shape1, ImplicitShape& shape2) {
     setShapes(&shape1, &shape2);
   }

   virtual Color getAlbedo()    const { return shape1_->getAlbedo(); }
   virtual Color getSpecular()  const { return shape1_->getSpecular(); }
   virtual float getShininess() const { return shape1_->getShininess(); }

   virtual Color getAlbedo(const Point3& p) const {
     return (shape1_->getDistance(p) < shape2_->getDistance(p)) ?
       shape1_->getAlbedo(p) :
       shape2_->getAlbedo(p);
   }
   virtual Color getSpecular(const Point3& p) const {
     return (shape1_->getDistance(p) < shape2_->getDistance(p)) ?
       shape1_->getSpecular(p) :
       shape2_->getSpecular(p);
   }
   virtual float getShininess(const Point3& p) const {
     return (shape1_->getDistance(p) < shape2_->getDistance(p)) ?
       shape1_->getShininess(p) :
       shape2_->getShininess(p);
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


/////////////////////////////////////////////////////////////////////////////////////////////////
//// OPERATIONS /////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////
class OpShape : public ImplicitShape {
  protected:
   ImplicitShape* shape1_ = nullptr;

   void setShapes(ImplicitShape* shape1) {
     shape1_ = shape1;
   }
   void setShapes(ImplicitShape& shape1) { setShapes(&shape1); }
   virtual Color getAlbedo()    const { return shape1_->getAlbedo(); }
   virtual Color getSpecular()  const { return shape1_->getSpecular(); }
   virtual float getShininess() const { return shape1_->getShininess(); }

   void update() {
     ImplicitShape::update();
     shape1_->update();
   }
};

class DisplacementOpShape : public OpShape {
  private:
   float (*displacement_)(Point3) = nullptr;
  public:
    DisplacementOpShape(ImplicitShape* shape1, float (*displacement)(Point3 p)) : displacement_(displacement) { setShapes(shape1); }

    float getDistance(const Point3& from) const {
      Point3 local(worldToLocal(from));
      return shape1_->getDistance(local) + displacement_(local);
    }
};




class RepeteOpShape : public OpShape {
  private:
    Vec3 rep_period_= Vec3(1); // x,y,z rep periods
    Vec3 num_instances_= Vec3(10); // x,y,z num of instances

  public:
    RepeteOpShape(ImplicitShape* shape1, Vec3 rep_period, Vec3 num_instances) : rep_period_(rep_period), num_instances_(num_instances) {
      setShapes(shape1);
    }

    float getDistance(const Point3& from) const {
      Point3 local(worldToLocal(from));


      //std::cout <<
      //  "\nlocal:         " << local.as_Vec3() <<
      //  "\nrep_period_:   " <<  rep_period_ <<
      //  "\nclamped:       " << utilities::clamp(utilities::round(local.as_Vec3()/rep_period_), -num_instances_, num_instances_) <<
      //  "\nloc/rep:       " << local.as_Vec3()/rep_period_ <<
      //  "\nloc/rep round: " << utilities::round(local.as_Vec3()/rep_period_) <<
      //  std::endl;

      Vec3 q = local.as_Vec3() - rep_period_ * utilities::clamp(
          utilities::round(local.as_Vec3()/rep_period_),
          -num_instances_,
           num_instances_);
      return shape1_->getDistance(q.as_Point());
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
