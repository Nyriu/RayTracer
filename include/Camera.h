#ifndef CAMERA_H
#define CAMERA_H

#include "geometry.h"
#include "Ray.h"
#include <glm/ext/matrix_transform.hpp>

class Camera {
  private:
    Point3 orig_;
    Vec3   dir_;
    float fov_, aspect_;

  public:
    //Camera() = default;

    Camera(float fov, float aspect_ratio) :
      orig_(0), dir_(0,0,-1), fov_(fov), aspect_(aspect_ratio) {
        dir_.normalize();
      }

    //camera(const point3& origin, const point3& target) : orig(origin) {
    Camera(const Point3& origin, const Vec3& direction) :
      orig_(origin), dir_(direction), fov_(45), aspect_(1) {
      dir_.normalize();
      //this->target = target;
    }

    Camera(float fov, const Point3& origin, const Vec3& direction) :
      orig_(origin), dir_(direction), fov_(fov), aspect_(1) {
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
      Vec3 direction = worldDir(Vec3(su,sv,-1));
      return Ray(orig_, direction); // dir can be un-normalized
    }

    //vec3 worldDir(float su, float sv, float sz) {
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


    //void translate(const vec3& v) {
    //  orig += v;
    //}
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

