#ifndef VEC3_H
#define VEC3_H

//#include <cmath>
//#include <iostream>
#include <glm/geometric.hpp>
#include <ostream>

#include <glm/vec3.hpp> // glm::vec3

//Type aliases
using vec3   = glm::vec3;
using point3 = glm::vec3; // 3D point


//inline std::ostream& operator<<(std::ostream &out, const vec3 &v) {
inline std::ostream& operator<<(std::ostream &out, const glm::vec3 &v) {
  return out << "[" << v.x  << " " << v.y << " " << v.z << "]";
}

//inline vec3 unit_vector(const vec3 &v) {
inline vec3 normalize(const vec3 &v) {
  return glm::normalize(v);
}

inline float length(const vec3 &v) {
  return glm::length(v);
}

inline float magnitude(const vec3 &v) {
  return glm::length(v);
}


#endif
