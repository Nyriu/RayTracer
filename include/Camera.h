#ifndef CAMERA_H
#define CAMERA_H

#include "SceneObject.h"
#include "geometry.h"
#include "Ray.h"

class Camera : public SceneObject {
  friend std::ostream& operator<<(std::ostream& out, const Camera& c);

  private:
    Point3 target_ = Point3(0,0,0);
    bool use_target_ = false; // true iff the user gave a target point to look at

    float fov_=45, aspect_=1;

  public:
    Camera() = default;


    // TODO enable old constructors and update them

    //Camera(float fov, float aspect_ratio) :
    //  fov_(fov), aspect_(aspect_ratio) { }

    //Camera(const Point3& origin, const Vec3& direction) :
    //  orig_(origin) {
    //    lookAt(direction);
    //    update();
    //  }

    //Camera(const Point3& origin, const Point3& target) :
    //  orig_(origin) {
    //    lookAt(target);
    //    update();
    //  }

    //Camera(float fov, const Point3& origin, const Point3& target) :
    //  orig_(origin), fov_(fov) {
    //    lookAt(target);
    //    update();
    //  }

    //Camera(float fov, const Point3& origin, const Vec3& direction) :
    //  orig_(origin), fov_(fov) {
    //    lookAt(direction);
    //    update();
    //  }

    //Camera(
    //    float fov, float aspect_ratio,
    //    const Point3& origin,
    //    const Point3& target
    //    ) :
    //  orig_(origin), fov_(fov), aspect_(aspect_ratio) {
    //    lookAt(target);
    //    update();
    //  }

    //Camera(
    //    float fov, float aspect_ratio,
    //    const Point3& origin,
    //    const Vec3& direction
    //    ) :
    //  orig_(origin), fov_(fov), aspect_(aspect_ratio) {
    //    lookAt(direction);
    //    update();
    //  }

    Ray generate_ray(float u, float v); // input NDC Coord
    Point3 intoWorldDir(const Point3& p);
    void lookAt(const float x, const float y, const float z);
    void lookAt(const Point3 p);

  public:
    bool update(); // actually updates the camera

    Camera setAspectRatio(const float& aspect_ratio) {
      aspect_ = aspect_ratio;
      //toUpdate();
      return *this;
    }
};



#endif

