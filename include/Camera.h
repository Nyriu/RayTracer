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

    Ray generate_ray(float u, float v) { // input NDC Coords
      // Put coords in [-1,1]
      float su = 2 * u - 1; // Screen Coord
      float sv = 1 - 2 * v; // Screen Coord (flip y axis)

      // Aspect Ratio
      su *= aspect_; // x in [-asp ratio, asp ratio]
      // y in [-1,1] (as before)

      // Field Of View
      su *= std::tan(fov_/2);
      sv *= std::tan(fov_/2);

      //float scale = 1;
      //// Scale // TODO needed? meaning?
      //// like a zoom?
      //su *= scale;
      //sv *= scale;

      // From ScreenCoords to WorldCoords
      Vec3 direction = Vec3(su,sv,-1);
      direction.normalize();
      return Ray(orig_, worldDir(direction)); // dir can be un-normalized
      // compute viewMat iff !updated_
    }

    //vec3 worldDir(float su, float sv, float sz)
    Vec3 worldDir(const Vec3& rayDir) {
      //vec3 up(0,1,0);
      //vec3 f = normalize(target - orig);
      //vec3 s = normalize(cross(f, up));
      //mat4 viewMat(
      //    vec4(s,0.0),
      //    vec4(cross(s,f),0.0),
      //    vec4(-f,0.0),
      //    vec4(vec3(0), 1)
      //    );
      //vec4 worldRayDir = (viewMat * vec4(rayDir, 0));
      //return normalize(vec3(
      //      worldRayDir.x, worldRayDir.y, worldRayDir.z)
      //      ); // extremely ugly

      // --------------------

      //vec3 up(0,1,0);
      //mat4 viewMat = glm::lookAt(orig, orig + normalize(dir), up);
      //vec4 worldRayDir = (viewMat * vec4(rayDir, 0));
      //return normalize(vec3(
      //      worldRayDir.x, worldRayDir.y, worldRayDir.z)
      //      ); // extremely ugly

      // --------------------

      Vec3 up(0,1,0);
      return myLookAt(orig_, orig_ + dir_, up, rayDir);
    }

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


inline std::ostream& operator<<(std::ostream& out, const Camera& c) {
  return out << "Camera {" <<
    "\n\torigin    = " << c.orig_ <<
    "\n\tdirection = " << c.dir_  <<
    "\n\tfov       = " << c.fov_  <<
    "\n\taspect    = " << c.aspect_ <<
    "\n} End Camera\n";
}




#endif

