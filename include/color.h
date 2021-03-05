#ifndef COLOR_H
#define COLOR_H

//#include <iostream>
#include <ostream>
#include <fstream>

#include <glm/vec3.hpp> // glm::vec3

//Type aliases
using color  = glm::vec3; // RGB color


inline void write_color(std::ofstream &out, color pixel_color) {
  // Write the translated [0,255] value of each color component
  out << static_cast<int>(255.999 * pixel_color.x) << " "
      << static_cast<int>(255.999 * pixel_color.y) << " "
      << static_cast<int>(255.999 * pixel_color.z) << "\n";
}


//inline std::ostream& operator<<(std::ostream &out, const color &c) {
//  return out
//    << static_cast<int>(255.999 * c.x) << " "
//    << static_cast<int>(255.999 * c.y) << " "
//    << static_cast<int>(255.999 * c.z) << " ";
//}

#endif
