#ifndef SCENE_OBJECT_H
#define SCENE_OBJECT_H

#include "geometry.h"
#include <cmath>
#include <iostream>


class SceneObject {
  protected:
    Mat4 matrix_; // The local transform matrix.
    Mat4 matrix_inverse_;
    //Mat4 matrixWorld; // The global transform of the object.
    //                  // If the no parent, then it's identical to the local transform matrix.

    Vec3 translations_;
    Vec3 rotations_; // rotation degrees around x,y,z axes


  public:
    virtual Point3 localToWorld(const Point3& target) { return matrix_ * target; }
    virtual Point3 worldToLocal(const Point3& target) { return matrix_.inverse() * target; }

    virtual Vec3 localToWorld(const Vec3& target) { return matrix_ * target; }
    virtual Vec3 worldToLocal(const Vec3& target) { return matrix_.inverse() * target; }

    //SceneObject translate(const Vec3& t) {
    void translate( const float x, const float y, const float z) {
      translate(Vec3(x,y,z));
      //return *this;
    }
    //SceneObject translate(const Vec3& t) {
    void translate(const Vec3& t) {
      translations_ = translations_ + t;
      update_matrix();
      //return *this;
    }

    //SceneObject rotateX(const float rad) {
    void rotateX(const float deg) {
      rotations_.set_x(deg);
      update_matrix();
      //return *this;
    }
    //SceneObject rotateY(const float deg) {
    void rotateY(const float deg) {
      rotations_.set_y(deg);
      update_matrix();
      //return *this;
    }
    //SceneObject rotateZ(const float deg) {
    void rotateZ(const float deg) {
      rotations_.set_z(deg);
      update_matrix();
      //return *this;
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
    //// first try
    //void update_inverse() {
    //  matrix_inverse_ = gen_rotation_matrix(
    //      -rotations_.x(),
    //      -rotations_.y(),
    //      -rotations_.z());
    //  matrix_inverse_.set(0,3, -translations_.x());
    //  matrix_inverse_.set(1,3, -translations_.y());
    //  matrix_inverse_.set(2,3, -translations_.z());

    //  std::cout <<
    //    "\nmatrix_ = \n" << matrix_ <<
    //    "\nmatrix_inverse_ = \n" << matrix_inverse_ <<
    //    "\ngen_rotation_matrix_z = \n" << gen_rotation_matrix_z(rotations_.z()) <<
    //    std::endl;
    //}
    // second try
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
      //std::cout <<
      //  "\nmatrix_ = \n" << matrix_ <<
      //  "\nmatrix_.inverse() = \n" << matrix_.inverse() <<
      //  "\nmatrix_inverse_ = \n" << matrix_inverse_ <<
      //  "\nR = \n" << R << "\n" <<
      //  r0 << " " << r1 << " " << r2 << "\n" <<
      //  "\nminus_RT = \n" << minus_RT <<
      //  std::endl;
    }
};

#endif
