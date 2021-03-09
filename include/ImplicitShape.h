#ifndef IMPLICIT_SHAPE_H
#define IMPLICIT_SHAPE_H

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
    float getDistance(const vec3& from) const {
      return length(from - center) - radius;
    }
};




// std::vector<std::shared_ptr<ImplicitShape>> makeScene()
// std::vector<std::shared_ptr<ImplicitShape>> shapes;

//inline std::vector<const ImplicitShape *> makeScene() {
//  std::vector<const ImplicitShape *> shapes;
//
//  //shapes.push_back(new Sphere(vec3(0), 1));
//  //shapes.push_back(new Sphere(vec3(0.5), 1));
//  //shapes.push_back(new Sphere(vec3(0,0,-13), 3));
//
//  shapes.push_back(new Sphere(vec3(0,.5,0), .5, color(1,0,1)));
//  shapes.push_back(new Sphere(vec3(1,1,0),  1 , color(1,0,0)));
//  shapes.push_back(new Sphere(vec3(2,2,0),  1 , color(0,1,0)));
//  shapes.push_back(new Sphere(vec3(3,3,0),  1 , color(0,0,1)));
//
//  // terrain
//  shapes.push_back(new Sphere(vec3(0,-100,0), 100, color(.5)));
//
//  return shapes;
//}

#endif
