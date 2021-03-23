#include "Camera.h"

//#include "geometry.h"
#include <iostream>


Ray Camera::generate_ray(float u, float v) { // input NDC Coords
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
  Vec3 direction = Vec3(su,sv,-1).normalize();
  return Ray(orig_, intoWorldDir(direction)) ; //  - orig_);
}

bool Camera::update() {
  if (isToUpdate()) {
    // here update dir_, target_, viewMatrix_
    updateViewMatrix();

    updated_ = true;
    return true;
  }
  return false;
}

void Camera::updateViewMatrix() {
  Point3 center =
    //use_target ? (orig_ - target_) : dir_;
    use_target ? target_ : orig_ + dir_;
  if (!isToUpdate()) return;

  Vec3 up(0,1,0); // what if we look vertically up/down?
  viewMatrix_ = lookAt_(orig_, center, up);
}



//Vec3 Camera::lookAt(const Point3 target) {
void Camera::lookAt(const Point3 target) {
  target_ = target;
  use_target = true;

  toUpdate();
  //update();
  //return dir_;
}

//Vec3 Camera::lookAt(const Vec3 direction) {
void Camera::lookAt(const Vec3 direction) {
  use_target = false;

  dir_ = direction;
  dir_.normalize();

  toUpdate();
  //update();
  //return dir_;
}


// Vec3 Camera::worldDir(const Vec3& rayDir) {
//   //vec3 up(0,1,0);
//   //vec3 f = normalize(target - orig);
//   //vec3 s = normalize(cross(f, up));
//   //mat4 viewMat(
//   //    vec4(s,0.0),
//   //    vec4(cross(s,f),0.0),
//   //    vec4(-f,0.0),
//   //    vec4(vec3(0), 1)
//   //    );
//   //vec4 worldRayDir = (viewMat * vec4(rayDir, 0));
//   //return normalize(vec3(
//   //      worldRayDir.x, worldRayDir.y, worldRayDir.z)
//   //      ); // extremely ugly
// 
//   // --------------------
// 
//   //vec3 up(0,1,0);
//   //mat4 viewMat = glm::lookAt(orig, orig + normalize(dir), up);
//   //vec4 worldRayDir = (viewMat * vec4(rayDir, 0));
//   //return normalize(vec3(
//   //      worldRayDir.x, worldRayDir.y, worldRayDir.z)
//   //      ); // extremely ugly
// 
//   // --------------------
// 
//   Vec3 up(0,1,0);
//   return myLookAt(orig_, orig_ + dir_, up, rayDir);
// }

Vec3 Camera::intoWorldDir(const Vec3& rayDir) {
  if (isToUpdate()) update();

  return (viewMatrix_ * Vec4(rayDir,0))
    .drop(3) // drop the exceding dimension
    .normalize();
}






// Friends Functions
inline std::ostream& operator<<(std::ostream& out, const Camera& c) {
  return out << "Camera {" <<
    "\n\torigin    = " << c.orig_ <<
    "\n\tdirection = " << c.dir_  <<
    "\n\tfov       = " << c.fov_  <<
    "\n\taspect    = " << c.aspect_ <<
    "\n} End Camera\n";
}


