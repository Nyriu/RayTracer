#ifndef GEOMETRY_H
#define GEOMETRY_H

// HERE WE USE FLOAT NOT DOUBLE
// Maybe in future use Templates

//#include <glm/ext/vector_float3.hpp>
#include <glm/fwd.hpp>
#include <glm/common.hpp>

#include <ostream>

#include <glm/vec3.hpp>
#include <glm/gtx/norm.hpp>

#include <glm/mat3x3.hpp>
#include <glm/gtx/string_cast.hpp>

#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>


class Vec3;
class Vec4;
class Mat4;

// ------------------------------
// POINTS
// ------------------------------

class Point3 {
  public:
    friend class Vec3;
    friend class Vec4;

    friend inline Point3 operator+(const Point3& u, const Point3& v);
    friend inline Vec3 operator-(const Point3& u, const Point3& v);
    friend inline Vec3 operator-(const Vec3& u, const Point3& v);
    friend inline Point3 operator*(const Point3& u, const Point3& v);
    friend inline Point3 operator*(const Point3& v, const float& f);
    friend inline Point3 operator*(const float& f, const Point3& v);

    friend inline Vec3 myLookAt(
        const Point3& eye, const Point3& target,
        const Vec3& up, const Vec3& dirToMove); // TODO serve sul serio?

    friend inline Mat4 geom_lookAt(
        const Point3& eye, const Point3& center, const Vec3& up);

    friend inline Point3 operator+(const Point3& u, const Vec3& v);
    friend inline Point3 operator+(const Vec3& u, const Point3& v);

  private:
    glm::vec3 v_;
  public:
    Point3() = default;
    Point3(float x, float y, float z ) : v_(x,y,z) {};
    Point3(float x) : v_(x,x,x) {};
    Point3(const glm::vec3 v) : v_(v) {};
    Point3(const Point3& v) : v_(v.v_) {};
    //Point3(const Vec3& v) : v_(v.v_) {};

    float x() const { return v_.x; }
    float y() const { return v_.y; }
    float z() const { return v_.z; }


    float length() const {
      return glm::length(v_);
    }

    float length2() const {
      return glm::length2(v_);
    }

    float magnitude() const {
      return this->length();
    }

    Point3 cross(const Point3 &v2) const {
      // Needs to change the state of "this"?
      return Point3(glm::cross(v_, v2.v_));
    }

    float dot(const Point3 &v2) const {
      return glm::dot(v_, v2.v_);
    }
};

inline Point3 operator+(const Point3& u, const Point3& v) {
  return Point3(u.v_ + v.v_);
}
inline Point3 operator*(const Point3& u, const Point3& v) {
  return Point3(u.v_ * v.v_);
}

inline Point3 operator*(const Point3& v, const float& f) {
  return Point3(v.v_ * f);
}
inline Point3 operator*(const float& f, const Point3& v) {
  return v*f;
}


inline std::ostream& operator<<(std::ostream& out, const Point3& v) {
  return out << "[" << v.x()  << " " << v.y() << " " << v.z() << "]";
}




// ------------------------------
// VECTORS 3
// ------------------------------
class Vec3 {
  public:
    friend class Point3;
    friend class Vec4;
    friend class Mat3;

    friend inline Vec3 operator+(const Vec3& u, const Vec3& v);
    friend inline Vec3 operator-(const Vec3& u, const Vec3& v);
    friend inline Vec3 operator*(const Vec3& u, const Vec3& v);
    friend inline Vec3 operator*(const Vec3& v, const float& f);
    friend inline Vec3 operator*(const float& f, const Vec3& v);


    friend inline Vec3 myLookAt(
        const Point3& eye, const Point3& target,
        const Vec3& up, const Vec3& dirToMove); // TODO serve sul serio?

    friend inline Vec3 operator-(const Vec3& u, const Point3& v);
    friend inline Point3 operator+(const Point3& u, const Vec3& v);
    friend inline Point3 operator+(const Vec3& u, const Point3& v);

    friend inline Mat4 geom_lookAt(
        const Point3& eye, const Point3& center, const Vec3& up);

  private:
    glm::vec3 v_;
  public:
    Vec3() = default;
    Vec3(float x, float y, float z ) : v_(x,y,z) {};
    Vec3(float x) : v_(x,x,x) {};
    Vec3(const glm::vec3 v) : v_(v) {};
    Vec3(const Vec3& v) : v_(v.v_) {};
    //Vec3(const Point3& v) : v_(v.v_) {};

    float x() const { return v_.x; }
    float y() const { return v_.y; }
    float z() const { return v_.z; }

    Vec3 normalize() {
      v_ = glm::normalize(v_);
      return *this;
    }

    float length() const {
      return glm::length(v_);
    }

    float length2() const {
      return glm::length2(v_);
    }

    float magnitude() const {
      return this->length();
    }

    Vec3 cross(const Vec3 &v2) const {
      // Needs to change the state of "this"?
      return Vec3(glm::cross(v_, v2.v_));
    }

    float dot(const Vec3 &v2) const {
      return glm::dot(v_, v2.v_);
    }

    Point3 as_Point() {
      return Point3(v_);
    }


};

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
// POINT 3 and VECTOR 3 Operators
// ------------------------------
inline Vec3 operator-(const Point3& u, const Point3& v) {
  return Vec3(u.v_ - v.v_);
}
inline Vec3 operator-(const Vec3& u, const Point3& v) {
  return Vec3(u.v_ - v.v_);
}

inline Point3 operator+(const Point3& u, const Vec3& v) {
  return Point3(u.v_ + v.v_);
}
inline Point3 operator+(const Vec3& u, const Point3& v) {
  return v+u;
}




// ------------------------------
// MATRICES 3x3
// ------------------------------
class Mat3 {
  public:
    friend inline Mat3 operator+(const Mat3& m1, const Mat3& m2);
    friend inline Mat3 operator-(const Mat3& m1, const Mat3& m2);
    friend inline Mat3 operator*(const Mat3& m1, const Mat3& m2);

    friend inline std::ostream& operator<<(std::ostream& out, const Mat3& m);

  private:
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

//inline std::ostream& operator<<(std::ostream& out, const Mat3& m) {
//  return  out << "["
//    << m.m_[0] << "\n "
//    << m.m_[1] << "\n "
//    << m.m_[2] << "]";
//}



// ------------------------------
// 4x4 MATRICES and VECTORS 4
// ------------------------------
// Needed for ViewMatrix

class Vec4 {
  public:
    friend class Vec3;
    friend class Mat4;

    friend inline Vec4 operator+(const Vec4& u, const Vec4& v);
    friend inline Vec4 operator-(const Vec4& u, const Vec4& v);
    friend inline Vec4 operator*(const Vec4& u, const Vec4& v);
    friend inline Vec4 operator*(const Vec4& v, const float& f);
    friend inline Vec4 operator*(const float& f, const Vec4& v);

    friend inline Vec4 operator*(const Mat4& m, const Vec4& v);

  private:
    glm::vec4 v_;
  public:
    Vec4() = default;
    Vec4(float x, float y, float z, float w) : v_(x,y,z,w) {};
    Vec4(float x) : v_(x,x,x,x) {};
    Vec4(const glm::vec4 v) : v_(v) {};
    Vec4(const Vec4& v) : v_(v.v_) {};
    Vec4(const Vec3& v, const float w) : v_(glm::vec4(v.v_,0)) {};
    Vec4(const Point3& v, const float w) : v_(glm::vec4(v.v_,0)) {};

    float x() const { return v_.x; }
    float y() const { return v_.y; }
    float z() const { return v_.z; }
    float w() const { return v_.w; }

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

    //Vec4 cross(const Vec4 &v2) {
    //  // Needs to change the state of "this"?
    //  return Vec4(glm::cross(v_, v2.v_));
    //}

    Vec3 drop(const int dim) {
      if (dim < 0 || dim > 3)
        throw "Vec4 has 4 dims! Must drop in [0,3]";

      switch (dim) {
        case 0:
          return Vec3(v_.y, v_.z, v_.w);
        case 1:
          return Vec3(v_.x, v_.z, v_.w);
        case 2:
          return Vec3(v_.x, v_.y, v_.w);
        case 3:
          return Vec3(v_.x, v_.y, v_.z);
      }
      throw "Vec4 unexpected drop dim";
    }

    Point3 asPoint3() { return Point3(v_.x, v_.y, v_.z); }

    float dot(const Vec4 &v2) const {
      return glm::dot(v_, v2.v_);
    }
};

inline Vec4 operator+(const Vec4& u, const Vec4& v) {
  return Vec4(u.v_ + v.v_);
}
inline Vec4 operator-(const Vec4& u, const Vec4& v) {
  return Vec4(u.v_ - v.v_);
}
inline Vec4 operator*(const Vec4& u, const Vec4& v) {
  return Vec4(u.v_ * v.v_);
}

inline Vec4 operator*(const Vec4& v, const float& f) {
  return Vec4(v.v_ * f);
}
inline Vec4 operator*(const float& f, const Vec4& v) {
  return v*f;
}


class Mat4 {
  public:
  //friend inline Mat3 operator+(const Mat3& m1, const Mat3& m2);
  //friend inline Mat3 operator-(const Mat3& m1, const Mat3& m2);
  //friend inline Mat3 operator*(const Mat3& m1, const Mat3& m2);

  //friend inline std::ostream& operator<<(std::ostream& out, const Mat3& m);

  friend inline Vec4 operator*(const Mat4& m, const Vec4& v);

  private:
    glm::mat4x4  m_;
  public:
    Mat4() = default;
    Mat4(const Vec4& v) : m_(v.v_, v.v_, v.v_, v.v_) {};
    Mat4(const Vec4& v1, const Vec4& v2, const Vec4& v3, const Vec4& v4) :
      m_(v1.v_, v2.v_, v3.v_, v4.v_) {};
    Mat4(const glm::mat4x4& m) : m_(m) {};
    Mat4(const Mat4& m) : m_(m.m_) {};

    // Operators
};


inline Vec4 operator*(const Mat4& m, const Vec4& v) {
  return Vec4(m.m_ * v.v_);
}

//inline Mat4 operator+(const Mat4& m1, const Mat4& m2) {
//  return Mat4( m1.m_ + m2.m_);
//}
//inline Mat4 operator-(const Mat4& m1, const Mat4& m2) {
//  return Mat4( m1.m_ - m2.m_);
//}
//inline Mat4 operator*(const Mat4& m1, const Mat4& m2) {
//  return Mat4( m1.m_ * m2.m_);
//}


// ------------------------------
// UTILITIES
// ------------------------------

// Deprecated, remove
//inline Vec3 myLookAt(
//    const Point3& eye, const Point3& target,
//    const Vec3& up, const Vec3& dirToMove) {
//  // extremely ugly
//
//  glm::mat4 viewMat = glm::lookAt(eye.v_, target.v_, up.v_);
//  glm::vec4 worldRayDir = (viewMat * glm::vec4(dirToMove.v_, 0));
//
//  return Vec3(
//      normalize(
//        glm::vec3(
//          worldRayDir.x, worldRayDir.y, worldRayDir.z
//          )));
//}

inline Mat4 geom_lookAt(
    const Point3& eye, const Point3& center, const Vec3& up) {
  return Mat4(glm::lookAt(eye.v_, center.v_, up.v_));
}




#endif
