#ifndef CAMERA_H
#define CAMERA_H

#include "vec3.h"
#include "mat3.h"
#include "ray.h"
//#include <glm/ext/matrix_transform.hpp>
//#include <glm/geometric.hpp>
//KK##include <glm/gtc/look_at

class camera {
  private:
    point3 orig; // = point3(0);
    vec3   dir; // = normalize(vec3(0,0,-1));
    float fov, aspect;

    //mat3 rotation(1,0,0,
    vec3 translation;


  public:
    camera() {}

    camera(float fov, float aspect_ratio) : orig(0), dir(0,0,-1) { this->fov = fov;
      this->aspect = aspect_ratio;
    }

    camera(const point3& origin, const vec3& direction) : orig(origin) {
      this->dir = normalize(direction);
      this->fov = 90;
      this->aspect = 1;
    }

    // TODO default values
    //camera(float fov=90, float aspect_ratio=1,
    //    const point3& origin=point3(0), const vec3& direction=vec3(0,0,-1)) : orig(origin) {
    camera(float fov, float aspect_ratio,
        const point3& origin, const vec3& direction) : orig(origin) {
      this->dir = normalize(direction);
      this->fov = fov;
      this->aspect = aspect_ratio;
    }

    ray generate_ray(float u, float v) { // input NDC Coords
      // Put coords in [-1,1]
      float su = 2 * u - 1; // Screen Coord
      float sv = 1 - 2 * v; // Screen Coord (flip y axis)

      // Aspect Ratio
      su *= aspect; // x in [-asp ratio, asp ratio]
      // y in [-1,1] (as before)

      // Field Of View
      su *= std::tan(fov/2);
      sv *= std::tan(fov/2);

      //// Scale // TODO needed? meaning?
      //// like a zoom?
      //su *= scale;
      //sv *= scale;

      // From ScreenCoords to WorldCoords
      float wu = su;
      float wv = sv;
      vec3 direction(wu,wv,-1);
      //vec3 direction(wu,wv,0);
      //return ray(orig,direction * dir); // dir can be un-normalized
      return ray(orig, direction); // dir can be un-normalized
      //return ray(orig, direction * lookAt(vec3(0,0,0))); // dir can be un-normalized
    }


    // TODO translate
    //void translate(const vec3& v) {
    //  orig += v;
    //}
    //void translate(float x, float y, float z) {
    //  translate(vec3(x,y,z));
    //}


    // TODO
    //void put_at(const point3& v) {
    //  orig = v;
    //}
    //void put_at(float x, float y, float z) {
    //  put_at(point3(x,y,z));
    //}

    // TODO
    //void look_at(const point3& v) {
    //  //dir = normalize(v);
    //  //cam direction = glm::normalize(cameraPos - cameraTarget);

    //  //cam pos, target, up
    //  glm::mat4x4 view =
    //    glm::lookAt(
    //        orig,
    //        v,
    //        vec3(0.0f, 1.0f, 0.0f));
    //}

    //void look_at(const point3& v) {
    //  dir = normalize(v);
    //}
    //void look_at(float x, float y, float z) {
    //  look_at(point3(x,y,z));
    //}

    //mat3 lookAt(const point3& to, const point3& tmp = point3(0, 1, 0)) {
    //  vec3 from = orig;
    //  vec3 forward = normalize(from - to);
    //  //vec3 right = crossProduct(normalize(tmp), forward);
    //  vec3 right = normalize(tmp) * forward;
    //  //vec3 up = crossProduct(forward, right);
    //  vec3 up = forward * right;

    //  mat3 camToWorld;

    //  camToWorld[0][0] = right.x;
    //  camToWorld[0][1] = right.y;
    //  camToWorld[0][2] = right.z;
    //  camToWorld[1][0] = up.x;
    //  camToWorld[1][1] = up.y;
    //  camToWorld[1][2] = up.z;
    //  camToWorld[2][0] = forward.x;
    //  camToWorld[2][1] = forward.y;
    //  camToWorld[2][2] = forward.z;

    //  //camToWorld[3][0] = from.x;
    //  //camToWorld[3][1] = from.y;
    //  //camToWorld[3][2] = from.z;

    //  return camToWorld;
    //}

};

#endif

