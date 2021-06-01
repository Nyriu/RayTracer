#include "Image.h"

const void Image::init_img() {
  img_ = new Color[width * height];
}
int Image::at(int i1, int i2) const {
  return i1 * height + i2;
}

Image::Image(const int image_width, const int image_height) : width(image_width), height(image_height), aspect_ratio((float)image_width/image_height) {
  init_img();
}


void Image::writePPM(const std::string& filepath) {
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
