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


    virtual Point3 localToWorld(const Point3& target) const;
    virtual Point3 worldToLocal(const Point3& target) const;

    virtual Vec3 localToWorld(const Vec3& target) const;
    virtual Vec3 worldToLocal(const Vec3& target) const;

    SceneObject translate(const float x, const float y, const float z);
    SceneObject translate(const Vec3& t);

    SceneObject rotateX(const float deg);
    SceneObject rotateY(const float deg);
    SceneObject rotateZ(const float deg);
    SceneObject rotate(const float deg_x, const float deg_y, const float deg_z);
    SceneObject rotate(const Vec3& rotations);

    SceneObject set_speed(const Vec3& speed);
    SceneObject set_spin(const Vec3& spin);
    SceneObject set_speed(const float speed_x, const float speed_y, const float speed_z);
    SceneObject set_spin(const float spin_x, const float spin_y, const float spin_z);


    virtual void update();


    virtual ~SceneObject() {}

  protected:
    virtual void update_matrix();
    virtual void update_inverse();
};

#endif
