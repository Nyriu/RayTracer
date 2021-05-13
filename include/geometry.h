#ifndef GEOMETRY_H
#define GEOMETRY_H

// HERE WE USE FLOAT NOT DOUBLE
// Maybe in future use Templates

#include <ostream>
#include <glm/ext/matrix_float4x4.hpp>
#include <glm/matrix.hpp>
#include <iostream>
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

    friend inline Mat4 geom_lookAt(
        const Point3& eye, const Point3& center, const Vec3& up);

    friend inline Point3 operator+(const Point3& u, const Vec3& v);
    friend inline Point3 operator+(const Vec3& u, const Point3& v);

  private:
    float v_[3] = {0,0,0};

  public:
    Point3() = default;
    Point3(float x, float y, float z ) : v_{x,y,z} {};
    Point3(float x) : v_{x,x,x} {};
    Point3(const Point3& v) : v_{v.v_[0], v.v_[1], v.v_[2]} {};
    //Point3(const Vec3& v) : v_(v.v_) {};

    float x() const { return v_[0]; }
    float y() const { return v_[1]; }
    float z() const { return v_[2]; }

    //Point3 abs() {
    //  v_[0] = std::abs(v_[0]);
    //  v_[1] = std::abs(v_[1]);
    //  v_[2] = std::abs(v_[2]);
    //  return *this;
    //}


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
    friend inline Vec3 operator-(const Vec3& u);
    friend inline Vec3 operator*(const Vec3& u, const Vec3& v);
    friend inline Vec3 operator*(const Vec3& v, const float& f);
    friend inline Vec3 operator*(const float& f, const Vec3& v);

    friend inline Vec3   operator*(const Mat4& m, const Vec3& v);

    friend inline Vec3 operator-(const Vec3& u, const Point3& v);
    friend inline Point3 operator+(const Point3& u, const Vec3& v);
    friend inline Point3 operator+(const Vec3& u, const Point3& v);

    friend inline Mat4 geom_lookAt(
        const Point3& eye, const Point3& center, const Vec3& up);

  private:
    float v_[3] = {0,0,0};
  public:
    Vec3() = default;
    Vec3(float x, float y, float z ) : v_{x,y,z} {};
    Vec3(float x) : v_{x,x,x} {};
    Vec3(const Vec3& v) : v_{v.v_[0], v.v_[1], v.v_[2]} {};


    void set_x(float x) { v_[0] = x; }
    void set_y(float y) { v_[1] = y; }
    void set_z(float z) { v_[2] = z; }

    float x() const { return v_[0]; }
    float y() const { return v_[1]; }
    float z() const { return v_[2]; }

    Vec3 normalize() {
      float len = length();
      if (len > 0) {
        float f = 1 / length();
        v_[0] *= f;
        v_[1] *= f;
        v_[2] *= f;
      }
      return *this;
    }

    //Vec3 abs() {
    //  v_[0] = std::abs(v_[0]);
    //  v_[1] = std::abs(v_[1]);
    //  v_[2] = std::abs(v_[2]);
    //  return *this;
    //}

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
  return Vec3(
      u.v_[0] - v.v_[0],
      u.v_[1] - v.v_[1],
      u.v_[2] - v.v_[2]);
}
inline Vec3 operator-(const Vec3& u) {
  return Vec3(
      - u.v_[0],
      - u.v_[1],
      - u.v_[2]);
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
inline std::ostream& operator<<(std::ostream& out, const Mat4& m);

class Mat4 {
  public:
    friend inline Point3 operator*(const Mat4& m, const Point3& v);
    friend inline Vec3   operator*(const Mat4& m, const Vec3& v);

    friend inline Mat4 geom_lookAt(const Point3& eye, const Point3& center, const Vec3& up);


  private:
    float m_[16] = {
      1,0,0,0,
      0,1,0,0,
      0,0,1,0,
      0,0,0,1
    };

  public:
    Mat4() = default;
    Mat4(float a, float b, float c, float d, float e, float f, float g, float h,
         float i, float j, float k, float l, float m, float n, float o, float p) {
      int index=0;
        m_[index++] = a;
        m_[index++] = b;
        m_[index++] = c;
        m_[index++] = d;
        m_[index++] = e;
        m_[index++] = f;
        m_[index++] = g;
        m_[index++] = h;
        m_[index++] = i;
        m_[index++] = j;
        m_[index++] = k;
        m_[index++] = l;
        m_[index++] = m;
        m_[index++] = n;
        m_[index++] = o;
        m_[index++] = p;
    }

    Mat4(const Mat4& m) {
      for (int i=0; i<16; i++){
        m_[i] = m.m_[i];
      }
    }

    Mat4 set_col_0(const Vec3& v) {
      set(0,0, v.x()); set(1,0, v.y()); set(2,0, v.z());
      return *this;
    }
    Mat4 set_col_1(const Vec3& v) {
      set(0,1, v.x()); set(1,1, v.y()); set(2,1, v.z());
      return *this;
    }
    Mat4 set_col_2(const Vec3& v) {
      set(0,2, v.x()); set(1,2, v.y()); set(2,2, v.z());
      return *this;
    }
    Mat4 set_col_3(const Vec3& v) {
      set(0,3, v.x()); set(1,3, v.y()); set(2,3, v.z());
      return *this;
    }

    Mat4 set_row_0(const Vec3& v) {
      set(0,0, v.x()); set(0,1, v.y()); set(0,2, v.z());
      return *this;
    }
    Mat4 set_row_1(const Vec3& v) {
      set(1,0, v.x()); set(1,1, v.y()); set(1,2, v.z());
      return *this;
    }
    Mat4 set_row_2(const Vec3& v) {
      set(2,0, v.x()); set(2,1, v.y()); set(2,2, v.z());
      return *this;
    }
    //Mat4 set_row_3(const Vec3& v) {
    //  set(3,0, v.x()); set(3,1, v.y()); set(3,2, v.z());
    //  return *this;
    //}


    //bool gluInvertMatrix(const double m[16], double invOut[16]) {
    Mat4 inverse() const {
      float m[16];
      for (int i = 0; i < 16; i++)
        m[i] = m_[i];
      float inv[16], det;

      inv[0] = m[5]  * m[10] * m[15] -
        m[5]  * m[11] * m[14] -
        m[9]  * m[6]  * m[15] +
        m[9]  * m[7]  * m[14] +
        m[13] * m[6]  * m[11] -
        m[13] * m[7]  * m[10];

      inv[4] = -m[4]  * m[10] * m[15] +
        m[4]  * m[11] * m[14] +
        m[8]  * m[6]  * m[15] -
        m[8]  * m[7]  * m[14] -
        m[12] * m[6]  * m[11] +
        m[12] * m[7]  * m[10];

      inv[8] = m[4]  * m[9] * m[15] -
        m[4]  * m[11] * m[13] -
        m[8]  * m[5] * m[15] +
        m[8]  * m[7] * m[13] +
        m[12] * m[5] * m[11] -
        m[12] * m[7] * m[9];

      inv[12] = -m[4]  * m[9] * m[14] +
        m[4]  * m[10] * m[13] +
        m[8]  * m[5] * m[14] -
        m[8]  * m[6] * m[13] -
        m[12] * m[5] * m[10] +
        m[12] * m[6] * m[9];

      inv[1] = -m[1]  * m[10] * m[15] +
        m[1]  * m[11] * m[14] +
        m[9]  * m[2] * m[15] -
        m[9]  * m[3] * m[14] -
        m[13] * m[2] * m[11] +
        m[13] * m[3] * m[10];

      inv[5] = m[0]  * m[10] * m[15] -
        m[0]  * m[11] * m[14] -
        m[8]  * m[2] * m[15] +
        m[8]  * m[3] * m[14] +
        m[12] * m[2] * m[11] -
        m[12] * m[3] * m[10];

      inv[9] = -m[0]  * m[9] * m[15] +
        m[0]  * m[11] * m[13] +
        m[8]  * m[1] * m[15] -
        m[8]  * m[3] * m[13] -
        m[12] * m[1] * m[11] +
        m[12] * m[3] * m[9];

      inv[13] = m[0]  * m[9] * m[14] -
        m[0]  * m[10] * m[13] -
        m[8]  * m[1] * m[14] +
        m[8]  * m[2] * m[13] +
        m[12] * m[1] * m[10] -
        m[12] * m[2] * m[9];

      inv[2] = m[1]  * m[6] * m[15] -
        m[1]  * m[7] * m[14] -
        m[5]  * m[2] * m[15] +
        m[5]  * m[3] * m[14] +
        m[13] * m[2] * m[7] -
        m[13] * m[3] * m[6];

      inv[6] = -m[0]  * m[6] * m[15] +
        m[0]  * m[7] * m[14] +
        m[4]  * m[2] * m[15] -
        m[4]  * m[3] * m[14] -
        m[12] * m[2] * m[7] +
        m[12] * m[3] * m[6];

      inv[10] = m[0]  * m[5] * m[15] -
        m[0]  * m[7] * m[13] -
        m[4]  * m[1] * m[15] +
        m[4]  * m[3] * m[13] +
        m[12] * m[1] * m[7] -
        m[12] * m[3] * m[5];

      inv[14] = -m[0]  * m[5] * m[14] +
        m[0]  * m[6] * m[13] +
        m[4]  * m[1] * m[14] -
        m[4]  * m[2] * m[13] -
        m[12] * m[1] * m[6] +
        m[12] * m[2] * m[5];

      inv[3] = -m[1] * m[6] * m[11] +
        m[1] * m[7] * m[10] +
        m[5] * m[2] * m[11] -
        m[5] * m[3] * m[10] -
        m[9] * m[2] * m[7] +
        m[9] * m[3] * m[6];

      inv[7] = m[0] * m[6] * m[11] -
        m[0] * m[7] * m[10] -
        m[4] * m[2] * m[11] +
        m[4] * m[3] * m[10] +
        m[8] * m[2] * m[7] -
        m[8] * m[3] * m[6];

      inv[11] = -m[0] * m[5] * m[11] +
        m[0] * m[7] * m[9] +
        m[4] * m[1] * m[11] -
        m[4] * m[3] * m[9] -
        m[8] * m[1] * m[7] +
        m[8] * m[3] * m[5];

      inv[15] = m[0] * m[5] * m[10] -
        m[0] * m[6] * m[9] -
        m[4] * m[1] * m[10] +
        m[4] * m[2] * m[9] +
        m[8] * m[1] * m[6] -
        m[8] * m[2] * m[5];

      det = m[0] * inv[0] + m[1] * inv[4] + m[2] * inv[8] + m[3] * inv[12];

      // TODO throw error?
      //if (det == 0)
      //    return false;

      det = 1.0 / det;

      Mat4 outMat;
      for (int i=0; i<16; i++) {
        outMat.m_[i] = inv[i] * det;
      }

      /// // DEBUG PRINTS -----------------------------------
      /// std::cout << "matrix" << std::endl;
      /// std::cout << *this << std::endl;
      /// std::cout << "inverse" << std::endl;
      /// std::cout << outMat << std::endl;

      /// glm::mat4x4 g;
      /// for (int i=0; i<4; i++) {
      ///   for (int j=0; j<4; j++) {
      ///     g[i][j] = get(i,j);
      ///   }
      /// }
      /// std::cout << "g matrix" << std::endl;
      /// std::cout <<
      ///   "[ " << g[0][0] << " " << g[0][1] << " " << g[0][2] << " " << g[0][3] << "\n" <<
      ///   "  " << g[1][0] << " " << g[1][1] << " " << g[1][2] << " " << g[1][3] << "\n" <<
      ///   "  " << g[2][0] << " " << g[2][1] << " " << g[2][2] << " " << g[2][3] << "\n" <<
      ///   "  " << g[3][0] << " " << g[3][1] << " " << g[3][2] << " " << g[3][3] << "\n" <<
      ///   "] " << std::endl;

      /// std::cout << "g inverse" << std::endl;
      /// glm::mat4x4 gi = glm::inverse(g);
      /// std::cout <<
      ///   "[ " << gi[0][0] << " " << gi[0][1] << " " << gi[0][2] << " " << gi[0][3] << "\n" <<
      ///   "  " << gi[1][0] << " " << gi[1][1] << " " << gi[1][2] << " " << gi[1][3] << "\n" <<
      ///   "  " << gi[2][0] << " " << gi[2][1] << " " << gi[2][2] << " " << gi[2][3] << "\n" <<
      ///   "  " << gi[3][0] << " " << gi[3][1] << " " << gi[3][2] << " " << gi[3][3] << "\n" <<
      ///   "] " << std::endl;

      /// std::cout << "g * gi" << std::endl;
      /// glm::mat4x4 pr = g * gi;
      /// std::cout <<
      ///   "[ " << pr[0][0] << " " << pr[0][1] << " " << pr[0][2] << " " << pr[0][3] << "\n" <<
      ///   "  " << pr[1][0] << " " << pr[1][1] << " " << pr[1][2] << " " << pr[1][3] << "\n" <<
      ///   "  " << pr[2][0] << " " << pr[2][1] << " " << pr[2][2] << " " << pr[2][3] << "\n" <<
      ///   "  " << pr[3][0] << " " << pr[3][1] << " " << pr[3][2] << " " << pr[3][3] << "\n" <<
      ///   "] " << std::endl;
      /// // END // DEBUG PRINTS ----------------------------

      return outMat;
    }


    float get(int row, int col) const {
      // TODO throw error
      //if (i > 3 || j > 3)
      //  throw
      return m_[row+col*4]; };

    bool set(int row, int col, float val) {
      if (row > 3 || col > 3)
        return false;
      m_[row+col*4] = val;
      return true;
    };

    float get_rotation_x() const {
      return std::atan2(
          -get(1,0),
          get(0,0)
          );
    }
    float get_rotation_y() const {
      return std::asin(-get(2,0));
    }
    float get_rotation_z() const {
      return std::atan2(
          -get(2,1),
          get(2,2)
          );
    }

};


inline Point3 operator*(const Mat4& m, const Point3& v) {
  float a,b,c,w;
  a = v.v_[0] * m.get(0,0) + v.v_[1] * m.get(0,1) + v.v_[2] * m.get(0,2) + m.get(0,3);
  b = v.v_[0] * m.get(1,0) + v.v_[1] * m.get(1,1) + v.v_[2] * m.get(1,2) + m.get(1,3);
  c = v.v_[0] * m.get(2,0) + v.v_[1] * m.get(2,1) + v.v_[2] * m.get(2,2) + m.get(2,3);
  w = v.v_[0] * m.get(3,0) + v.v_[1] * m.get(3,1) + v.v_[2] * m.get(3,2) + m.get(3,3);
  return Point3(a/w,b/w,c/w);
}

inline Vec3 operator*(const Mat4& m, const Vec3& v) {
  float a,b,c;
  a = v.v_[0] * m.get(0,0) + v.v_[1] * m.get(0,1) + v.v_[2] * m.get(0,2);
  b = v.v_[0] * m.get(1,0) + v.v_[1] * m.get(1,1) + v.v_[2] * m.get(1,2);
  c = v.v_[0] * m.get(2,0) + v.v_[1] * m.get(2,1) + v.v_[2] * m.get(2,2);
  return Vec3(a,b,c);
}

inline std::ostream& operator<<(std::ostream& out, const Mat4& m) {
  return out <<
    "[ " << m.get(0,0) << " " << m.get(0,1) << " " << m.get(0,2) << " " << m.get(0,3) << "\n" <<
    "  " << m.get(1,0) << " " << m.get(1,1) << " " << m.get(1,2) << " " << m.get(1,3) << "\n" <<
    "  " << m.get(2,0) << " " << m.get(2,1) << " " << m.get(2,2) << " " << m.get(2,3) << "\n" <<
    "  " << m.get(3,0) << " " << m.get(3,1) << " " << m.get(3,2) << " " << m.get(3,3) << "\n" <<
    "] ";
}




inline float degree_to_radians(const float deg) {
  return deg * M_PI/180;
}

inline float deg_cos(const float deg) {
  return std::cos(
      degree_to_radians(deg)
      );
}
inline float deg_sin(const float deg) {
  return std::sin(
      degree_to_radians(deg)
      );
}
inline float rad_cos(const float rad) {
  return std::cos(rad);
}
inline float rad_sin(const float rad) {
  return std::sin(rad);
}

//inline Mat4 gen_rotation_matrix_x(const float deg_x) {
//  float c = deg_cos(deg_x);
//  float s = deg_sin(deg_x);
//  Mat4 m;
//  m.set(1,1, c);
//  m.set(1,2, -s);
//  m.set(2,1, s);
//  m.set(2,2, c);
//  return m;
//}
//inline Mat4 gen_rotation_matrix_y(const float deg_y) {
//  float c = deg_cos(deg_y);
//  float s = deg_sin(deg_y);
//  Mat4 m;
//  m.set(0,0, c);
//  m.set(0,2, s);
//  m.set(2,0, -s);
//  m.set(2,2, c);
//  return m;
//}
//inline Mat4 gen_rotation_matrix_z(const float deg_z) {
//  float c = deg_cos(deg_z);
//  float s = deg_sin(deg_z);
//  Mat4 m;
//  m.set(0,0, c);
//  m.set(0,1, -s);
//  m.set(1,0, s);
//  m.set(1,1, c);
//  return m;
//}

//inline Mat4 compose_rotations(const Mat4 rot_x, const Mat4 rot_y, const Mat4 rot_z) {
//}


inline Mat4 gen_rotation_matrix_rad(const float rad_x, const float rad_y, const float rad_z) {
  // return the matrix rotZ * rotY * rotX
  float cx = rad_cos(rad_x), sx = rad_sin(rad_x);
  float cy = rad_cos(rad_y), sy = rad_sin(rad_y);
  float cz = rad_cos(rad_z), sz = rad_sin(rad_z);
  Mat4 m;
  m.set(0,0, cz*cy);
  m.set(0,1, cz*sy*sx - sz*cx);
  m.set(0,2, cz*sy*cx + sz*sx);

  m.set(1,0, sz*cy);
  m.set(1,1, sz*sy*sx + cz*cx);
  m.set(1,2, sz*sy*cx - cz*sx);

  m.set(2,0, -sy);
  m.set(2,1, cy*sx);
  m.set(2,2, cy*cx);

  return m;
}

inline Mat4 gen_rotation_matrix(const float deg_x, const float deg_y, const float deg_z) {
  return gen_rotation_matrix_rad(
      degree_to_radians(deg_x),
      degree_to_radians(deg_y),
      degree_to_radians(deg_z)
      );
}




#endif
