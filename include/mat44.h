#ifndef VEC3_H
#define VEC3_H


#include <glm/vec3.hpp> // glm::vec3
#include <glm/vec4.hpp> // glm::vec4
#include <glm/mat4x4.hpp> // glm::mat4
#include <glm/ext/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale
#include <glm/ext/matrix_clip_space.hpp> // glm::perspective
#include <glm/ext/scalar_constants.hpp> // glm::pi

inline glm::mat4 camera(float Translate, glm::vec2 const& Rotate)
{
  TODO
	glm::mat4 Projection = glm::perspective(glm::pi<float>() * 0.25f, 4.0f / 3.0f, 0.1f, 100.f);
	glm::mat4 View = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -Translate));
	View = glm::rotate(View, Rotate.y, glm::vec3(-1.0f, 0.0f, 0.0f));
	View = glm::rotate(View, Rotate.x, glm::vec3(0.0f, 1.0f, 0.0f));
	glm::mat4 Model = glm::scale(glm::mat4(1.0f), glm::vec3(0.5f));
	return Projection * View * Model;
}

#endif

//#ifndef VEC3_H
//#define VEC3_H
//
//#include <cmath>
//#include <iostream>
//#include <ostream>
//
//using std::sqrt;
//
//class vec3 {
//  public:
//    double e[3];
//    vec3() :  e{0,0,0} {}
//    vec3(double e0) : e{e0, e0, e0} {}
//    vec3(double e0, double e1, double e2) : e{e0, e1, e2} {}
//
//    double x() const { return e[0]; }
//    double y() const { return e[1]; }
//    double z() const { return e[2]; }
//
//    vec3 operator-() const { return vec3(-e[0], -e[1], -e[2]); }
//    double operator[](int i) const { return e[i]; }
//    double& operator[](int i) { return e[i]; }
//
//    vec3& operator +=(const vec3 &v){
//      e[0] += v.e[0];
//      e[1] += v.e[1];
//      e[2] += v.e[2];
//      return *this;
//    }
//
//    vec3& operator *=(const double t){
//      e[0] *= t;
//      e[1] *= t;
//      e[2] *= t;
//      return *this;
//    }
//
//    vec3& operator /=(const double t){
//      return *this *= 1/t;
//    }
//
//    double length() const {
//      return sqrt(length_squared());
//    }
//
//    double length_squared() const {
//      return e[0]*e[0] + e[1]*e[1] + e[2]*e[2];
//    }
//
//
//};
//
////Type aliases for vec3
//using point3 = vec3;  // 3D point
//using color  = vec3;  // RGB color
//
//
//// vec3 Utility Functions
//
//inline std::ostream& operator<<(std::ostream &out, const vec3 &v) {
//  return out << v.e[0] << " " << v.e[1] << " " << v.e[2];
//}
//
//inline vec3 operator+(const vec3 &u, const vec3 &v) {
//  return vec3(u.e[0] + v.e[0], u.e[1] + v.e[1], u.e[2] + v.e[2]);
//}
//
//inline vec3 operator-(const vec3 &u, const vec3 &v) {
//  return vec3(u.e[0] - v.e[0], u.e[1] - v.e[1], u.e[2] - v.e[2]);
//}
//
//inline vec3 operator*(const vec3 &u, const vec3 &v) {
//  return vec3(u.e[0] * v.e[0], u.e[1] * v.e[1], u.e[2] * v.e[2]);
//}
//
//inline vec3 operator*(double t, const vec3 &v) {
//  return vec3(t * v.e[0], t * v.e[1], t * v.e[2]);
//}
//
//inline vec3 operator*(const vec3 &v, double t) {
//  return t * v;
//}
//
//inline vec3 operator/(const vec3 &v, double t) {
//  return (1/t) * v;
//}
//
//inline double dot(const vec3 &u, const vec3 &v) {
//  return u.e[0] * v.e[0] +
//         u.e[1] * v.e[1] +
//         u.e[2] * v.e[2];
//}
//
//inline vec3 cross(const vec3 &u, const vec3 &v) {
//  return vec3(u.e[1] * v.e[2] - u.e[2] * v.e[1],
//              u.e[2] * v.e[0] - u.e[0] * v.e[2],
//              u.e[0] * v.e[1] - u.e[1] * v.e[0]);
//}
//
//inline vec3 unit_vector(vec3 v) {
//  return v / v.length();
//}
//
//#endif
