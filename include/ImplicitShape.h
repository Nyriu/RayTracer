#ifndef IMPLICIT_SHAPE_H
#define IMPLICIT_SHAPE_H

#include<vector>
#include "vec3.h"

class ImplicitShape {
  public:
    virtual float getDistance(const point3& from) const = 0;
    virtual ~ImplicitShape() {}
};

class Sphere : public ImplicitShape {
  private:
    point3 center;
    float radius;
  public:
    Sphere(const point3& c, const float& r) : center(c), radius(r) {}
    float getDistance(const vec3& from) const {
      return length(from - center) - radius;
    }
};





inline std::vector<const ImplicitShape *> makeScene() {
  std::vector<const ImplicitShape *> shapes;

  //shapes.push_back(new Sphere(vec3(0), 1));
  //shapes.push_back(new Sphere(vec3(0.5), 1));
  //shapes.push_back(new Sphere(vec3(0,0,-13), 3));

  shapes.push_back(new Sphere(vec3(0,-1,0), .5));
  shapes.push_back(new Sphere(vec3(1,1,0), 1));
  shapes.push_back(new Sphere(vec3(2,2,0), 1));
  shapes.push_back(new Sphere(vec3(3,3,0), 1));

  return shapes;
}

#endif
