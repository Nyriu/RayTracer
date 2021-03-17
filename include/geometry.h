#ifndef GEOMETRY_H
#define GEOMETRY_H

// HERE WE USE FLOAT NOT DOUBLE
// Maybe in future use Templates

//#include <glm/ext/vector_float3.hpp>
#include <glm/fwd.hpp>
#include <glm/common.hpp>

#include <ostream>


// ------------------------------
// VECTORS
// ------------------------------
#include <glm/vec3.hpp>
#include <glm/gtx/norm.hpp>


class Vec3 {
  //private:
  public:
    glm::vec3 v_;
  public:
    Vec3() = default;
    Vec3(float x, float y, float z ) : v_(x,y,z) {};
    Vec3(float x) : v_(x,x,x) {};
    Vec3(const glm::vec3 v) : v_(v) {};
    Vec3(const Vec3& v) : v_(v.v_) {};

    float x() const { return v_.x; }
    float y() const { return v_.y; }
    float z() const { return v_.z; }

    void normalize() {
      v_ = glm::normalize(v_);
    }

    float length() {
      return glm::length(v_);
    }

    float length2() {
      return glm::length2(v_);
    }

    float magnitude() {
      return this->length();
    }

    Vec3 cross(const Vec3 &v2) {
      // Needs to change the state of "this"?
      return Vec3(glm::cross(v_, v2.v_));
    }

    float dot(const Vec3 &v2) {
      return glm::dot(v_, v2.v_);
    }

    // // Operators that change the state of "this"
    // Vec3 operator-() {
    //   v_ = -v_;
    //   return *this;
    // }

    // Vec3& operator+(const Vec3& v) {
    //   v_ += v.v_;
    //   return *this;
    // }

    // Vec3 operator-(const Vec3& v) {
    //   v_ -= v.v_;
    //   return *this;
    // }

    // Vec3& operator*(const Vec3& v) {
    //   v_ *= v.v_;
    //   return *this;
    // }

};


// When use this, instead of the one in the class?
// How to define it if the member is private?
// How to define it if I don't want to change the state of u?
//inline Vec3 operator+(const Vec3& u, const Vec3& v) {
//  Vec3 tmp(u);
//  return Vec3(tmp+v);
//}
inline Vec3 operator+(const Vec3& u, const Vec3& v) {
  return Vec3(u.v_ + v.v_);
}
inline Vec3 operator-(const Vec3& u, const Vec3& v) {
  return Vec3(u.v_ - v.v_);
}
inline Vec3 operator*(const Vec3& u, const Vec3& v) {
  return Vec3(u.v_ * v.v_);
}

inline Vec3 operator*(const Vec3& v, const float& f) {
  return Vec3(v.v_ * f);
}
inline Vec3 operator*(const float& f, const Vec3& v) {
  return v*f;
}


inline std::ostream& operator<<(std::ostream& out, const Vec3& v) {
  return out << "[" << v.x()  << " " << v.y() << " " << v.z() << "]";
}



// ------------------------------
// POINTS
// ------------------------------

// Point3 should be a class
using Point3 = Vec3; // 3D point



// ------------------------------
// MATRICES
// ------------------------------

#include <glm/mat3x3.hpp>
#include <glm/gtx/string_cast.hpp>

class Mat3 {
  //private:
  public:
    glm::mat3x3  m_;
  public:
    Mat3() = default;
    Mat3(const Vec3& v) : m_(v.v_, v.v_, v.v_) {};
    Mat3(const Vec3& v1, const Vec3& v2, const Vec3& v3) : m_(v1.v_, v2.v_, v3.v_) {};
    Mat3(const glm::mat3x3& m) : m_(m) {};
    Mat3(const Mat3& m) : m_(m.m_) {};

    // Operators
};

inline Mat3 operator+(const Mat3& m1, const Mat3& m2) {
  return Mat3( m1.m_ + m2.m_);
}
inline Mat3 operator-(const Mat3& m1, const Mat3& m2) {
  return Mat3( m1.m_ - m2.m_);
}
inline Mat3 operator*(const Mat3& m1, const Mat3& m2) {
  return Mat3( m1.m_ * m2.m_);
}


inline std::ostream& operator<<(std::ostream& out, const Mat3& m) {
  return  out << "["
    << m.m_[0] << "\n "
    << m.m_[1] << "\n "
    << m.m_[2] << "]";
}


//#include <glm/mat4x4.hpp>
//
//class Mat4 {
//  //private:
//  public:
//    glm::mat4x4  m_;
//  public:
//    //Mat4() = default;
//    //Mat4(const Vec4& v) : m_(v.v_, v.v_, v.v_, v.v_) {};
//    //Mat4(const Vec4& v1, const Vec4& v2, const Vec4& v3, const Vec4& v4) :
//      //m_(v1.v_, v2.v_, v3.v_, v4.v_) {};
//    Mat4(const glm::mat4x4& m) : m_(m) {};
//    Mat4(const Mat4& m) : m_(m.m_) {};
//
//    // Operators
//};

// TODO mat4 and (above) vec4
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
// #include <glm/gtx/string_cast.hpp>
// inline std::ostream& operator<<(std::ostream &out, const glm::vec4 &v) {
//   return out << "[" << v.x  << " " << v.y << " " << v.z << " " << v.w << "]";
// }
// 
// //Type aliases
// using mat4 = glm::mat4x4;
// 
// using vec4 = glm::vec4; // TODO That's not the right place
// 
// inline std::ostream& operator<<(std::ostream &out, const mat4 &m) {
//   return  out << "["
//     << m[0] << "\n "
//     << m[1] << "\n "
//     << m[2] << "\n "
//     << m[3] << "]";
// }
// 
// 
// 




inline Vec3 myLookAt(
    const Point3& eye, const Point3& target,
    const Vec3& up, const Vec3& dirToMove) {
  // extremely ugly
  glm::mat4 viewMat = glm::lookAt(eye.v_, target.v_, up.v_);
  glm::vec4 worldRayDir = (viewMat * glm::vec4(dirToMove.v_, 0));

  return Vec3(
      normalize(
        glm::vec3(
          worldRayDir.x, worldRayDir.y, worldRayDir.z
          )));
}




#endif
