#include "Camera.h"

//#include "geometry.h"
#include <iostream>


int printed = 0;
Ray Camera::generate_ray(float u, float v) { // input NDC Coords
  update();

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
  //// Scale
  //su *= scale;
  //sv *= scale;

  // From ScreenCoords to WorldCoords
  Point3 p = Point3(-su,sv,1);
  Point3 orig = intoWorldDir(Point3(0));
  return Ray(orig, (orig - intoWorldDir(p)).normalize());
}

Point3 Camera::intoWorldDir(const Point3& p) {
  if (is_to_update_) update();
  return matrix_inverse_ * p;
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
  //  in other words: not lecit to look in the same point of the eye
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
  matrix_ = m;
  update_inverse();
}

void Camera::toUpdate() {
  is_to_update_ = true;
}

void Camera::update() {
  if (is_to_update_) {
    SceneObject::update();
    // here update dir_, target_ if needed
    if (use_target_) lookAt(target_);
    //update_inverse(); // already done in lookAt
  }
  is_to_update_ = false;
}


//// Friends Functions
//inline std::ostream& operator<<(std::ostream& out, const Camera& c) {
//  return out << "Camera {" <<
//    "\n\torigin    = " << c.orig_ <<
//    "\n\tdirection = " << c.dir_  <<
//    "\n\tfov       = " << c.fov_  <<
//    "\n\taspect    = " << c.aspect_ <<
//    "\n} End Camera\n";
//}

void Camera::update_matrix() {
  if (use_target_) {
    lookAt(target_);
    return;
  }
  SceneObject::update_matrix();
}
void Camera::update_inverse() {
  if (use_target_) {
    matrix_inverse_ = matrix_.inverse();
    return;
  }
  SceneObject::update_inverse();
}


