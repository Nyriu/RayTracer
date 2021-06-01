#include "SceneObject.h"

Point3 SceneObject::localToWorld(const Point3& target) const {
  return hasParent() ?
    matrix_ * parent_->localToWorld(target) :
    matrix_ * target;
}
Point3 SceneObject::worldToLocal(const Point3& target) const {
  return hasParent() ?
    matrix_inverse_ * parent_->worldToLocal(target) :
    matrix_inverse_ * target;
}

Vec3 SceneObject::localToWorld(const Vec3& target) const {
  return hasParent() ?
    matrix_ * parent_->localToWorld(target) :
    matrix_ * target;
}
Vec3 SceneObject::worldToLocal(const Vec3& target) const {
  return hasParent() ?
    matrix_inverse_ * parent_->worldToLocal(target) :
    matrix_inverse_ * target;
}


SceneObject SceneObject::translate(const float x, const float y, const float z) {
  translate(Vec3(x,y,z));
  return *this;
}

SceneObject SceneObject::translate(const Vec3& t) {
  translations_ = translations_ + t;
  update_matrix();
  return *this;
}

SceneObject SceneObject::rotateX(const float deg) {
  rotations_.set_x(rotations_.x() + deg);
  if (rotations_.x() >= 360)
    rotations_.set_x(rotations_.x() - 360);
  update_matrix();
  return *this;
}
SceneObject SceneObject::rotateY(const float deg) {
  rotations_.set_y(rotations_.y() + deg);
  if (rotations_.y() >= 360)
    rotations_.set_y(rotations_.y() - 360);
  update_matrix();
  return *this;
}
SceneObject SceneObject::rotateZ(const float deg) {
  rotations_.set_z(rotations_.z() + deg);
  if (rotations_.z() >= 360)
    rotations_.set_z(rotations_.z() - 360);
  update_matrix();
  return *this;
}
SceneObject SceneObject::rotate(const float deg_x, const float deg_y, const float deg_z) {
  rotateX(deg_x);
  rotateY(deg_y);
  rotateZ(deg_z);
  return *this;
}
SceneObject SceneObject::rotate(const Vec3& rotations) {
  rotate( rotations.x(), rotations.y(), rotations.z());
  return *this;
}

SceneObject SceneObject::set_speed(const Vec3& speed) {
  speed_ = speed;
  return *this;
}
SceneObject SceneObject::set_spin(const Vec3& spin) {
  spin_ = spin;
  return *this;
}
SceneObject SceneObject::set_speed(const float speed_x, const float speed_y, const float speed_z) {
  set_speed(Vec3(speed_x, speed_y, speed_z));
  return *this;
}
SceneObject SceneObject::set_spin(const float spin_x, const float spin_y, const float spin_z) {
  set_spin(Vec3(spin_x, spin_y, spin_z));
  return *this;
}


void SceneObject::update() {
  translate(speed_);
  rotate(spin_);
}

void SceneObject::update_matrix() {
  matrix_ = gen_rotation_matrix(
      rotations_.x(),
      rotations_.y(),
      rotations_.z());
  matrix_.set(0,3, translations_.x());
  matrix_.set(1,3, translations_.y());
  matrix_.set(2,3, translations_.z());
  update_inverse();
}
void SceneObject::update_inverse() {
  // from Real-Time Rendering 4th pag 66
  Mat4 R = gen_rotation_matrix(
      rotations_.x(),
      rotations_.y(),
      rotations_.z()
      );
  Vec3 r0(R.get(0,0), R.get(0,1), R.get(0,2));
  Vec3 r1(R.get(1,0), R.get(1,1), R.get(1,2));
  Vec3 r2(R.get(2,0), R.get(2,1), R.get(2,2));

  matrix_inverse_.set_col_0(r0);
  matrix_inverse_.set_col_1(r1);
  matrix_inverse_.set_col_2(r2);

  Mat4 minus_RT;
  minus_RT.set_col_0(-r0);
  minus_RT.set_col_1(-r1);
  minus_RT.set_col_2(-r2);
  matrix_inverse_.set_col_3(minus_RT * translations_);
}

