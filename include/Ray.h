#ifndef RAY_H
#define RAY_H

#include "geometry.h"

class Ray {
  private:
    Point3 orig;
    Vec3 dir;

  public:
    Ray() {}
    Ray(const Point3& origin, const Vec3& direction) : orig(origin), dir(direction) {}

    Point3 origin() const { return orig; }
    Vec3 direction() const { return dir; }

    const Point3 at(float t) const {
      return orig + t*dir;
      // TODO what if the operator changes the state of orig, i.e. moving it?
    }

};

#endif
