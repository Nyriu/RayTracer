#include "Camera.h"

//#include "geometry.h"
#include <iostream>


int printed = 0;
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
  //Point3 p = Point3(su,sv,-1);
  Point3 p = Point3(-su,-sv,1); // perche' ho dovuto cambiare segni????
  //Point3 orig = (translations_).as_Point();
  Point3 orig = intoWorldDir(Point3(0));
  //std::cout << "\norig of ray\n" << orig << std::endl;
  //return Ray(eye, intoWorldDir(direction));
  return Ray(orig, (orig - intoWorldDir(p)).normalize());
}

Point3 Camera::intoWorldDir(const Point3& p) {
  if (use_target_) {
    lookAt(target_);
    //std::cout << "with target\n" <<
    //  target_ <<
    //  std::endl;

  }
  //std::cout <<
  //  "\nmatrix_\n" << matrix_ <<
  //  std::endl;
  return matrix_.inverse() * p;
}

void Camera::lookAt(const float x, const float y, const float z) {
  Point3 p = Point3(x,y,z);
  target_ = p; use_target_ = true;
  lookAt(p);
}

void Camera::lookAt(const Point3 l) {
  //if (l.as_Vec3() == translations_) {
  //  // not lecit
  //  change l or return error
  //  TODO
  //}
  target_ = l; use_target_ = true;

  Vec3 up(0,1,0); // camera up
  Point3 c = translations_.as_Point(); // camera center
  Vec3 v = (c-l).normalize(); // View vector
  Vec3 r = -(v.cross(up)).normalize(); // right
  Vec3 u = v.cross(r); // better up
  Mat4 m;
  m.set_row_0(r);
  m.set_row_1(u);
  m.set_row_2(v);
  m.set_col_3(Vec3(
        -translations_.dot(r),
        -translations_.dot(u),
        -translations_.dot(v)));

        // TODO does not work

  //std::cout <<
  //  "\nv\n" << v <<
  //  "\nr\n" << r <<
  //  "\nu\n" << u <<
  //  "\nm\n" << m <<
  //    std::endl;

  //std::cout <<
  //  "Before update\n" <<
  //  "\nmatrix_\n" << matrix_ <<
  //  "\nm\n" << m << std::endl;

  matrix_ = m;
  // TODO update inverse!!!
  //rotations_ = Vec3(
  //    m.get_rotation_x(),
  //    m.get_rotation_y(),
  //    m.get_rotation_z()
  //    );

  //std::cout <<
  //  "After update\n" <<
  //  "\nmatrix_\n" << matrix_ <<
  //  //"\nm\n" << m <<
  //  //"\nrotations_\n" << rotations_ <<
  //  "\ntranslations_\n" << translations_ <<
  //  //"\n" <<
  //  std::endl;
}

bool Camera::update() {
//  if (isToUpdate()) {
//    // here update dir_, target_, viewMatrix_
//    updateViewMatrix();
//
//    updated_ = true;
//    return true;
//  }
  return true;
}

//void Camera::updateViewMatrix() {
//  Point3 center =
//    //use_target ? (orig_ - target_) : dir_;
//    use_target ? target_ : orig_ + dir_;
//  if (!isToUpdate()) return;
//
//  Vec3 up(0,1,0); // what if we look vertically up/down?
//  viewMatrix_ = lookAt_(orig_, center, up);
//}
//
//
//
////Vec3 Camera::lookAt(const Point3 target) {
//void Camera::lookAt(const Point3 target) {
//  target_ = target;
//  use_target = true;
//
//  toUpdate();
//  //update();
//  //return dir_;
//}
//
////Vec3 Camera::lookAt(const Vec3 direction) {
//void Camera::lookAt(const Vec3 direction) {
//  use_target = false;
//
//  dir_ = direction;
//  dir_.normalize();
//
//  toUpdate();
//  //update();
//  //return dir_;
//}
//
//
//// Vec3 Camera::worldDir(const Vec3& rayDir) {
////   //vec3 up(0,1,0);
////   //vec3 f = normalize(target - orig);
////   //vec3 s = normalize(cross(f, up));
////   //mat4 viewMat(
////   //    vec4(s,0.0),
////   //    vec4(cross(s,f),0.0),
////   //    vec4(-f,0.0),
////   //    vec4(vec3(0), 1)
////   //    );
////   //vec4 worldRayDir = (viewMat * vec4(rayDir, 0));
////   //return normalize(vec3(
////   //      worldRayDir.x, worldRayDir.y, worldRayDir.z)
////   //      ); // extremely ugly
//// 
////   // --------------------
//// 
////   //vec3 up(0,1,0);
////   //mat4 viewMat = glm::lookAt(orig, orig + normalize(dir), up);
////   //vec4 worldRayDir = (viewMat * vec4(rayDir, 0));
////   //return normalize(vec3(
////   //      worldRayDir.x, worldRayDir.y, worldRayDir.z)
////   //      ); // extremely ugly
//// 
////   // --------------------
//// 
////   Vec3 up(0,1,0);
////   return myLookAt(orig_, orig_ + dir_, up, rayDir);
//// }
//
//Vec3 Camera::intoWorldDir(const Vec3& rayDir) {
//  if (isToUpdate()) update();
//
//  return (viewMatrix_ * rayDir)
//  //return (viewMatrix_ * Vec4(rayDir,0))
//    //.drop(3) // drop the exceding dimension
//    .normalize();
//}
//
//
//Point3 Camera::intoWorld(const Point3& p) {
//  if (isToUpdate()) update();
//
//  return (viewMatrix_ * p);
//}
//
//
//
//// Friends Functions
//inline std::ostream& operator<<(std::ostream& out, const Camera& c) {
//  return out << "Camera {" <<
//    "\n\torigin    = " << c.orig_ <<
//    "\n\tdirection = " << c.dir_  <<
//    "\n\tfov       = " << c.fov_  <<
//    "\n\taspect    = " << c.aspect_ <<
//    "\n} End Camera\n";
//}


