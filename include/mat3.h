#ifndef MAT3_H
#define MAT3_H

//#include <cmath>
//#include <iostream>
#include <ostream>

#include <glm/mat3x3.hpp>
#include <glm/gtx/string_cast.hpp>

#include "vec3.h" // operator<< on vec3

//Type aliases
using mat3 = glm::mat3x3;

inline std::ostream& operator<<(std::ostream &out, const mat3 &m) {
  return  out << "["
    << m[0] << "\n "
    << m[1] << "\n "
    << m[2] << "]";
}


#endif

