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

    bool is_to_update_ = true;

  public:
    Camera() = default;

    Ray generate_ray(float u, float v); // input NDC Coord
    Point3 intoWorldDir(const Point3& p);
    void lookAt(const float x, const float y, const float z);
    void lookAt(const Point3 p);

  private:
    void toUpdate(); // flags the need to be updated
  public:
    void update(); // actually updates the camera

    Camera setAspectRatio(const float& aspect_ratio) {
      aspect_ = aspect_ratio;
      toUpdate();
      return *this;
    }
  protected: // from SceneObject
    void update_matrix();
    void update_inverse();
};

#endif
