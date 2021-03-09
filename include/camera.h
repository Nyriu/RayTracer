#ifndef CAMERA_H
#define CAMERA_H

#include "vec3.h"
#include "mat3.h"
#include "mat4.h"
#include "ray.h"
#include <glm/ext/matrix_transform.hpp>
//#include <glm/ext/matrix_transform.hpp>
//#include <glm/geometric.hpp>
//KK##include <glm/gtc/look_at

class camera {
  private:
    point3 orig; // = point3(0);
    vec3   dir;  // cam dir
    float fov, aspect;

  public:
    camera() {}

    camera(float fov, float aspect_ratio) : orig(0), dir(0,0,-1) { this->fov = fov;
      this->aspect = aspect_ratio;
    }

    //camera(const point3& origin, const point3& target) : orig(origin) {
    camera(const point3& origin, const vec3& direction) : orig(origin) {
      this->dir = normalize(direction);
      //this->target = target;
      this->fov = 90;
      this->aspect = 1;
    }

    // TODO default values
    //camera(float fov=90, float aspect_ratio=1,
    //    const point3& origin=point3(0), const vec3& direction=vec3(0,0,-1)) : orig(origin) {
    camera(float fov, float aspect_ratio,
        //const point3& origin, const point3& target) : orig(origin) {
        const point3& origin, const vec3& direction) : orig(origin) {
      this->dir = normalize(direction);
      //this->target = target;
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

      //float scale = 1;
      //// Scale // TODO needed? meaning?
      //// like a zoom?
      //su *= scale;
      //sv *= scale;

      // From ScreenCoords to WorldCoords
      vec3 direction = worldDir(vec3(su,sv,-1));
      return ray(orig, direction); // dir can be un-normalized
    }

    //vec3 worldDir(float su, float sv, float sz) {
    vec3 worldDir(const vec3& rayDir) {
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

      vec3 up(0,1,0);
      mat4 viewMat = glm::lookAt(orig, orig + normalize(dir), up);
      vec4 worldRayDir = (viewMat * vec4(rayDir, 0));
      return normalize(vec3(
            worldRayDir.x, worldRayDir.y, worldRayDir.z)
            ); // extremely ugly

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

