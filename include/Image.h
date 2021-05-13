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
      const void init_img() {
        img_ = new Color[width * height];
      }
      int at(int i1, int i2) const {
        return i1 * height + i2;
      }

  public:
      Image() = default;

      Image(const int image_width, const int image_height) : width(image_width), height(image_height), aspect_ratio((float)image_width/image_height) {
        init_img();
      }

      void setPixel(const Color& c, const int x, const int y) {
        img_[at(x,y)] = c;
      }

      Color getPixel(const int x, const int y) const {
        return img_[at(x,y)];
      }


      //bool writePPM(const string& filepath) {
      void writePPM(const char* filepath) { writePPM(std::string(filepath)); }
      void writePPM(const std::string& filepath) {
        // TODO check if path exists or error on open
        std::ofstream ofs;
        ofs.open(filepath);
        ofs << "P3\n" << width << " " << height << "\n255\n";
        for (int j=0; j<height; ++j) {
          for (int i=0; i<width; ++i) {
            write_color(ofs, img_[at(i,j)]);
          }
        }
        ofs.close();
      }
};


#endif
