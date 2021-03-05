#ifndef MAT4_H
#define MAT4_H

//#include <cmath>
//#include <iostream>
#include <ostream>

#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtx/string_cast.hpp>

inline std::ostream& operator<<(std::ostream &out, const glm::vec4 &v) {
  return out << "[" << v.x  << " " << v.y << " " << v.z << " " << v.w << "]";
}

//Type aliases
using mat4 = glm::mat4x4;

inline std::ostream& operator<<(std::ostream &out, const mat4 &m) {
  return  out << "["
    << m[0] << "\n "
    << m[1] << "\n "
    << m[2] << "\n "
    << m[3] << "]";
}




#endif
