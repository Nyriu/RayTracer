#ifndef IMPLICIT_SHAPE_H
#define IMPLICIT_SHAPE_H

#include<vector>
#include "vec3.h"

class ImplicitShape {
  public:
    virtual float getDistance(const vec3& from) const = 0;
    // TODO lookup on scratch pixel

};

std::vector<const ImplicitShape *> makeScene() {
  std::vector<const ImplicitShape *> shapes;

  return shapes;
}

#endif
