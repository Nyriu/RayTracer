#ifndef LIGHT_H
#define LIGHT_H

#include<vector>
#include "vec3.h"
#include "color.h"

class Light {
  public:
  //protected:
    point3 position_;
    color color_;
    float intensity_;
  public:
    virtual ~Light() {}
};

class PointLight : public Light {
  public:
    PointLight(const vec3& position, const vec3& color, const float& intensity) {
      position_ = position;
      color_ = color;
      intensity_ = intensity;
    }

};


#endif
