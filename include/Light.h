#ifndef LIGHT_H
#define LIGHT_H

#include<vector>
#include "geometry.h"
#include "Color.h"

class Light {
  //public:
  protected:
  //private:
    Point3 position_;
    Color color_;
    float intensity_;
  public:
    virtual ~Light() {}

    virtual Point3 getPosition() const { return position_; }

    virtual Color getColor() const { return color_; }

    virtual float getIntensity() const { return intensity_; }
};

class PointLight : public Light {
  public:
    PointLight(const Point3& position, const Color& color, const float& intensity) {
      position_ = position;
      color_ = color;
      intensity_ = intensity;
    }
};

class AmbientLight : public Light {
  public:
    AmbientLight(const Color& color, const float& intensity) {
      position_ = Point3(0);
      color_ = color;
      intensity_ = intensity;
    }
};


#endif
