#ifndef CAMERA_H
#define CAMERA_H

#include "geometry.h"
#include "Ray.h"
#include <glm/ext/matrix_transform.hpp>

class Camera {
  friend std::ostream& operator<<(std::ostream& out, const Camera& c);

  private:
    Point3 orig_ = Point3(0);
    Vec3   dir_  = Vec3(0,0,-1);
    float fov_=45, aspect_=1;

    float updated_ = false; // if the camera has been updated

  public:
    Camera() = default;

    Camera(float fov, float aspect_ratio) :
      fov_(fov), aspect_(aspect_ratio) {
        dir_.normalize();
      }

    //camera(const point3& origin, const point3& target) : orig(origin) {
    Camera(const Point3& origin, const Vec3& direction) :
      orig_(origin), dir_(direction) {
      dir_.normalize();
      //this->target = target;
    }

    Camera(float fov, const Point3& origin, const Vec3& direction) :
      orig_(origin), dir_(direction), fov_(fov) {
      dir_.normalize();
      //this->target = target;
    }

    Camera(
        float fov, float aspect_ratio,
        const Point3& camera_origin,
        const Vec3& camera_dir
        ) :
      orig_(camera_origin), dir_(camera_dir), fov_(fov), aspect_(aspect_ratio) {
        dir_.normalize();
      }

    Ray generate_ray(float u, float v); // input NDC Coord

    Vec3 worldDir(const Vec3& rayDir);

    bool isToUpdate() const { return !updated_; }

    bool update() {
      if (isToUpdate()) {
        updated_ = true;
        return true;
      }
      return false;
    }

  private:
    void toUpdate() { updated_ = false; }
  public:
    void translate(const Vec3& v) {
      orig_ = orig_ + v;
      toUpdate();
    }

    Camera setAspectRatio(const float& aspect_ratio) {
      aspect_ = aspect_ratio;
      toUpdate();
      return *this;
    }

    //void translate(float x, float y, float z) {
    //  translate(vec3(x,y,z));
    //}


    //void put_at(const point3& p) {
    //  orig = p;
    //}
    //void put_at(float x, float y, float z) {
    //  put_at(point3(x,y,z));
    //}

    // rework because now we work with directions
    //void look_at(const point3& p) {
    //  target = p;
    //}
    //void look_at(float x, float y, float z) {
    //  look_at(point3(x,y,z));
    //}

};



#endif

