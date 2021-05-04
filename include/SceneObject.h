#ifndef SCENE_OBJECT_H
#define SCENE_OBJECT_H

#include "geometry.h"
#include <cmath>
#include <iostream>


class SceneObject {
  protected:
    Mat4 matrix_; // The local transform matrix.
    Mat4 matrix_inverse_;

    Vec3 translations_;
    Vec3 rotations_; // rotation degrees around x,y,z axes // Be aware of Gimbal lock

    Vec3 speed_; // x,y,z movement done in one time tick
    Vec3 spin_;  // x,y,z deg of rotation done in one time tick

    const SceneObject *parent_ = nullptr;
    bool has_parent_ = false;


  public:
    bool hasParent() const { return parent_ != nullptr && has_parent_; }
    void setParent(const SceneObject *parent) {
      // if already present override parent
      // TODO handle cycle parent(A,B) AND parent(B,A) // check on children list?
      has_parent_ = true;
      parent_ = parent;
    }


    virtual Point3 localToWorld(const Point3& target) const {
      return hasParent() ?
        matrix_ * parent_->localToWorld(target) :
        matrix_ * target;
    }
    virtual Point3 worldToLocal(const Point3& target) const {
      return hasParent() ?
        matrix_inverse_ * parent_->worldToLocal(target) :
        matrix_inverse_ * target;
    }

    virtual Vec3 localToWorld(const Vec3& target) const {
      return hasParent() ?
        matrix_ * parent_->localToWorld(target) :
        matrix_ * target;
    }
    virtual Vec3 worldToLocal(const Vec3& target) const {
      return hasParent() ?
        matrix_inverse_ * parent_->worldToLocal(target) :
        matrix_inverse_ * target;
    }



    SceneObject translate( const float x, const float y, const float z) {
      translate(Vec3(x,y,z));
      return *this;
    }

    SceneObject translate(const Vec3& t) {
      translations_ = translations_ + t;
      update_matrix();
      return *this;
    }

    SceneObject rotateX(const float deg) {
      rotations_.set_x(rotations_.x() + deg);
      if (rotations_.x() >= 360)
        rotations_.set_x(rotations_.x() - 360);
      update_matrix();
      return *this;
    }
    SceneObject rotateY(const float deg) {
      rotations_.set_y(rotations_.y() + deg);
      if (rotations_.y() >= 360)
        rotations_.set_y(rotations_.y() - 360);
      update_matrix();
      return *this;
    }
    SceneObject rotateZ(const float deg) {
      rotations_.set_z(rotations_.z() + deg);
      if (rotations_.z() >= 360)
        rotations_.set_z(rotations_.z() - 360);
      update_matrix();
      return *this;
    }
    SceneObject rotate(const float deg_x, const float deg_y, const float deg_z) {
      rotateX(deg_x);
      rotateY(deg_y);
      rotateZ(deg_z);
      return *this;
    }
    SceneObject rotate(const Vec3& rotations) {
      rotate( rotations.x(), rotations.y(), rotations.z());
      return *this;
    }

    SceneObject set_speed(const Vec3& speed) {
      speed_ = speed;
      return *this;
    }
    SceneObject set_spin(const Vec3& spin) {
      spin_ = spin;
      return *this;
    }
    SceneObject set_speed(const float speed_x, const float speed_y, const float speed_z) {
      set_speed(Vec3(speed_x, speed_y, speed_z));
      return *this;
    }
    SceneObject set_spin(const float spin_x, const float spin_y, const float spin_z) {
      set_spin(Vec3(spin_x, spin_y, spin_z));
      return *this;
    }


    void update() {
      translate(speed_);
      rotate(spin_);
    }


    virtual ~SceneObject() {}

  private:
    void update_matrix() {
      matrix_ = gen_rotation_matrix(
          rotations_.x(),
          rotations_.y(),
          rotations_.z());
      matrix_.set(0,3, translations_.x());
      matrix_.set(1,3, translations_.y());
      matrix_.set(2,3, translations_.z());
      update_inverse();
    }
    void update_inverse() {
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
};

#endif
