#ifndef GEOMETRY_H
#define GEOMETRY_H

// HERE WE USE FLOAT NOT DOUBLE
// Maybe in future use Templates

#include <ostream>
#include <cmath>

class Point3;
class Vec3;
class Mat4;

// ------------------------------
// 3D POINT
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

    friend inline Point3 operator*(const Mat4& m, const Point3& v);

    //friend inline Vec3 myLookAt(
    //    const Point3& eye, const Point3& target,
    //    const Vec3& up, const Vec3& dirToMove);

    friend inline Mat4 geom_lookAt(
        const Point3& eye, const Point3& center, const Vec3& up);

    friend inline Point3 operator+(const Point3& u, const Vec3& v);
    friend inline Point3 operator+(const Vec3& u, const Point3& v);

  private:
    float v_[3];

  public:
    Point3() = default;
    Point3(float x, float y, float z ) : v_{x,y,z} {};
    Point3(float x) : v_{x,x,x} {};
    Point3(const Point3& v) : v_{v.v_[0], v.v_[1], v.v_[2]} {};
    //Point3(const Vec3& v) : v_(v.v_) {};

    float x() const { return v_[0]; }
    float y() const { return v_[1]; }
    float z() const { return v_[2]; }


    float length() const {
      return std::sqrt(length2());
    }

    float length2() const {
      return v_[0]*v_[0] +
             v_[1]*v_[1] +
             v_[2]*v_[2] ;
    }

    float magnitude() const {
      return this->length();
    }

    Point3 cross(const Point3 &v2) const {
      // Needs to change the state of "this"?
      return Point3(
          v_[1] * v2.v_[2] - v_[2] * v2.v_[1],
          v_[2] * v2.v_[0] - v_[0] * v2.v_[2],
          v_[0] * v2.v_[1] - v_[1] * v2.v_[0]);
    }

    float dot(const Point3 &v2) const {
      return v_[0] * v2.v_[0] +
             v_[1] * v2.v_[1] +
             v_[2] * v2.v_[2];
    }

    inline Vec3 as_Vec3() const;

};

inline Point3 operator+(const Point3& u, const Point3& v) {
  return Point3(
      u.v_[0] + v.v_[0],
      u.v_[1] + v.v_[1],
      u.v_[2] + v.v_[2]);

}
inline Point3 operator*(const Point3& u, const Point3& v) {
  return Point3(
      u.v_[0] * v.v_[0],
      u.v_[1] * v.v_[1],
      u.v_[2] * v.v_[2]);
}

inline Point3 operator*(const Point3& v, const float& f) {
  return Point3(
      f * v.v_[0],
      f * v.v_[1],
      f * v.v_[2]);
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

    friend inline Vec3   operator*(const Mat4& m, const Vec3& v);

    //friend inline Vec3 myLookAt(
    //    const Point3& eye, const Point3& target,
    //    const Vec3& up, const Vec3& dirToMove);

    friend inline Vec3 operator-(const Vec3& u, const Point3& v);
    friend inline Point3 operator+(const Point3& u, const Vec3& v);
    friend inline Point3 operator+(const Vec3& u, const Point3& v);

    friend inline Mat4 geom_lookAt(
        const Point3& eye, const Point3& center, const Vec3& up);

  private:
    float v_[3];
  public:
    Vec3() = default;
    Vec3(float x, float y, float z ) : v_{x,y,z} {};
    Vec3(float x) : v_{x,x,x} {};
    Vec3(const Vec3& v) : v_{v.v_[0], v.v_[1], v.v_[2]} {};

    float x() const { return v_[0]; }
    float y() const { return v_[1]; }
    float z() const { return v_[2]; }

    Vec3 normalize() {
      float len = length();
      v_[0] = v_[0] / len;
      v_[1] = v_[1] / len;
      v_[2] = v_[2] / len;
      return *this;
    }

    float length() const {
      return std::sqrt(length2());
    }

    float length2() const {
      return v_[0]*v_[0] +
             v_[1]*v_[1] +
             v_[2]*v_[2] ;
    }

    float magnitude() const {
      return this->length();
    }

    Vec3 cross(const Vec3 &v2) const {
      // Needs to change the state of "this"?
      return Vec3(
          v_[1] * v2.v_[2] - v_[2] * v2.v_[1],
          v_[2] * v2.v_[0] - v_[0] * v2.v_[2],
          v_[0] * v2.v_[1] - v_[1] * v2.v_[0]);
    }

    float dot(const Vec3 &v2) const {
      return v_[0] * v2.v_[0] +
             v_[1] * v2.v_[1] +
             v_[2] * v2.v_[2];
    }

    Point3 as_Point() const {
      return Point3(v_[0], v_[1], v_[2]);
    }
};


inline Vec3 Point3::as_Vec3() const {
  return Vec3(v_[0], v_[1], v_[2]);
}



inline Vec3 operator+(const Vec3& u, const Vec3& v) {
  return Vec3(
      u.v_[0] + v.v_[0],
      u.v_[1] + v.v_[1],
      u.v_[2] + v.v_[2]);
}
inline Vec3 operator-(const Vec3& u, const Vec3& v) {
  return Vec3(u.v_ - v.v_);
}

inline Vec3 operator*(const Vec3& u, const Vec3& v) {
  return Vec3(
      u.v_[0] * v.v_[0],
      u.v_[1] * v.v_[1],
      u.v_[2] * v.v_[2]);
}
inline Vec3 operator*(const Vec3& v, const float& f) {
  return Vec3(
      f * v.v_[0],
      f * v.v_[1],
      f * v.v_[2]);
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
  return Vec3(
      u.v_[0] - v.v_[0],
      u.v_[1] - v.v_[1],
      u.v_[2] - v.v_[2]
      );
}
inline Vec3 operator-(const Vec3& u, const Point3& v) {
  return u.as_Point() - v;
}

inline Point3 operator+(const Point3& u, const Vec3& v) {
  return u + v.as_Point();
}
inline Point3 operator+(const Vec3& u, const Point3& v) {
  return v+u;
}




// ------------------------------
// 4x4 MATRICES
// ------------------------------
class Mat4 {
  public:
    friend inline Point3 operator*(const Mat4& m, const Point3& v);
    friend inline Vec3   operator*(const Mat4& m, const Vec3& v);

    friend inline Mat4 geom_lookAt(const Point3& eye, const Point3& center, const Vec3& up);


  private:
    float m_[4][4] = {{1,0,0,0},{0,1,0,0},{0,0,1,0},{0,0,0,1}};

  public:
    Mat4() = default;
    Mat4(float a, float b, float c, float d, float e, float f, float g, float h,
         float i, float j, float k, float l, float m, float n, float o, float p) {
        m_[0][0] = a;
        m_[0][1] = b;
        m_[0][2] = c;
        m_[0][3] = d;
        m_[1][0] = e;
        m_[1][1] = f;
        m_[1][2] = g;
        m_[1][3] = h;
        m_[2][0] = i;
        m_[2][1] = j;
        m_[2][2] = k;
        m_[2][3] = l;
        m_[3][0] = m;
        m_[3][1] = n;
        m_[3][2] = o;
        m_[3][3] = p;
    }

    Mat4(const Mat4& m) {
      for (int j=0; j<4; j++){
        for (int i=0; i<4; i++){
          m_[i][j] = m.m_[i][j];
        }
      }
    }

    // Operators ?
};


inline Point3 operator*(const Mat4& m, const Point3& v) {
  float a,b,c,w;
  a = v.v_[0] * m.m_[0][0] + v.v_[1] * m.m_[1][0] + v.v_[2] * m.m_[2][0] + m.m_[3][0];
  b = v.v_[0] * m.m_[0][1] + v.v_[1] * m.m_[1][1] + v.v_[2] * m.m_[2][1] + m.m_[3][1];
  c = v.v_[0] * m.m_[0][2] + v.v_[1] * m.m_[1][2] + v.v_[2] * m.m_[2][2] + m.m_[3][2];
  w = v.v_[0] * m.m_[0][3] + v.v_[1] * m.m_[1][3] + v.v_[2] * m.m_[2][3] + m.m_[3][3];
  return Point3(a/w,b/w,c/w);
}

inline Vec3 operator*(const Mat4& m, const Vec3& v) {
  float a,b,c;
  a = v.v_[0] * m.m_[0][0] + v.v_[1] * m.m_[1][0] + v.v_[2] * m.m_[2][0];
  b = v.v_[0] * m.m_[0][1] + v.v_[1] * m.m_[1][1] + v.v_[2] * m.m_[2][1];
  c = v.v_[0] * m.m_[0][2] + v.v_[1] * m.m_[1][2] + v.v_[2] * m.m_[2][2];
  return Vec3(a,b,c);
}


inline Mat4 geom_lookAt( const Point3& eye, const Point3& center, const Vec3& up) {
  Mat4 mat;
  Vec3 x,y,z;

  z = eye - center;
  z.normalize();
  y = up;
  x = y.cross(z);
  y = z.cross(x);

  x.normalize();
  y.normalize();

  mat.m_[0][0] = x.x();
  mat.m_[1][0] = x.y();
  mat.m_[2][0] = x.z();
  mat.m_[3][0] = -x.dot( eye.as_Vec3() );
  mat.m_[0][1] = y.x();
  mat.m_[1][1] = y.y();
  mat.m_[2][1] = y.z();
  mat.m_[3][1] = -y.dot( eye.as_Vec3() );
  mat.m_[0][2] = z.x();
  mat.m_[1][2] = z.y();
  mat.m_[2][2] = z.z();
  mat.m_[3][2] = -z.dot( eye.as_Vec3() );
  mat.m_[0][3] = 0;
  mat.m_[1][3] = 0;
  mat.m_[2][3] = 0;
  mat.m_[3][3] = 1.0f;

  return mat;
}
// // from
// https://stackoverflow.com/a/21830596
// LMatrix4 LookAt( const LVector3& Eye, const LVector3& Center, const LVector3& Up )
// {
//   LMatrix4 Matrix;
//   LVector3 X, Y, Z;
// 
// 
//   Z = Eye - Center;
//   Z.Normalize();
//   Y = Up;
//   X = Y.Cross( Z );
// 
//   Y = Z.Cross( X );
// 
//   X.Normalize();
//   Y.Normalize();
// 
//   Matrix[0][0] = X.x;
//   Matrix[1][0] = X.y;
//   Matrix[2][0] = X.z;
//   Matrix[3][0] = -X.Dot( Eye );
//   Matrix[0][1] = Y.x;
//   Matrix[1][1] = Y.y;
//   Matrix[2][1] = Y.z;
//   Matrix[3][1] = -Y.Dot( Eye );
//   Matrix[0][2] = Z.x;
//   Matrix[1][2] = Z.y;
//   Matrix[2][2] = Z.z;
//   Matrix[3][2] = -Z.Dot( Eye );
//   Matrix[0][3] = 0;
//   Matrix[1][3] = 0;
//   Matrix[2][3] = 0;
//   Matrix[3][3] = 1.0f;
// 
//   return Matrix;
// }














#endif
