#ifndef IMAGE_H
#define IMAGE_H

#include <string>
#include <fstream>

#include "Color.h"

class Image {
  //private:
  public:
      const float aspect_ratio = 1;
      const int width  = 300;
      const int height = 300;

  private:
      Color* img_;
      const void init_img();
      int at(int i1, int i2) const;

  public:
      Image() = default;
      Image(const int image_width, const int image_height);

      void setPixel(const Color& c, const int x, const int y) {
        img_[at(x,y)] = c;
      }

      Color getPixel(const int x, const int y) const {
        return img_[at(x,y)];
      }


      //bool writePPM(const string& filepath) {
      void writePPM(const char* filepath) { writePPM(std::string(filepath)); }
      void writePPM(const std::string& filepath);
};


#endif
