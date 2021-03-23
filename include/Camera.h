#ifndef CAMERA_H
#define CAMERA_H

#include "geometry.h"
#include "Ray.h"
//#include <glm/ext/matrix_transform.hpp>

class Camera {
  friend std::ostream& operator<<(std::ostream& out, const Camera& c);

  private:
    Point3 orig_ = Point3(0);

    Point3 target_ = Point3(0,0,0);
    Vec3   dir_    = Vec3(0,0,-1).normalize();
    bool use_target = false; // true iff the user gave a target point to look at

    float fov_=45, aspect_=1;

    Mat4 viewMatrix_;

    float updated_ = false; // if the camera has been updated
                            // if false need to update (e.g. viewMatrix)

  public:
    Camera() = default;

    Camera(float fov, float aspect_ratio) :
      fov_(fov), aspect_(aspect_ratio) { }

    Camera(const Point3& origin, const Vec3& direction) :
      orig_(origin) {
        lookAt(direction);
        update();
      }

    Camera(const Point3& origin, const Point3& target) :
      orig_(origin) {
        lookAt(target);
        update();
      }

    Camera(float fov, const Point3& origin, const Point3& target) :
      orig_(origin), fov_(fov) {
        lookAt(target);
        update();
      }

    Camera(float fov, const Point3& origin, const Vec3& direction) :
      orig_(origin), fov_(fov) {
        lookAt(direction);
        update();
      }

    Camera(
        float fov, float aspect_ratio,
        const Point3& origin,
        const Point3& target
        ) :
      orig_(origin), fov_(fov), aspect_(aspect_ratio) {
        lookAt(target);
        update();
      }

    Camera(
        float fov, float aspect_ratio,
        const Point3& origin,
        const Vec3& direction
        ) :
      orig_(origin), fov_(fov), aspect_(aspect_ratio) {
        lookAt(direction);
        update();
      }

    Ray generate_ray(float u, float v); // input NDC Coord

    //Vec3 lookAt(const Point3 p);
    //Vec3 lookAt(const Vec3 direction);
    void lookAt(const Point3 p);
    void lookAt(const Vec3 direction);
  private:
    Mat4 lookAt_(const Point3& eye, const Point3& center, const Vec3& up) {
      return geom_lookAt(eye, center, up);
    }
  public:
    //Vec3 worldDir(const Vec3& rayDir);
    Vec3 intoWorldDir(const Vec3& rayDir);

    void toUpdate() { updated_ = false; }
    void updateViewMatrix();

  public:
    bool isToUpdate() const { return !updated_; }
    bool update(); // actually updates the camera

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

