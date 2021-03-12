#ifndef IMAGE_H
#define IMAGE_H

#include "Color.h"

class Image {
  private:
      const float aspect_ratio_;
      const int width_, height_;

      Color* img_;
      const void init_img() {
        img_ = new Color[width_ * height_];
      }
      int at(int i1, int i2) {
        return i1 * width_ + i2;
      }

  public:
      Image(const int image_width, const float aspect_ratio) :
        width_(image_width), aspect_ratio_(aspect_ratio), height_((int)(width_ / aspect_ratio_)) {
        init_img();
      }

      //Image(const int image_width, const int image_height) : width_(image_width), height_(image_height) {
      //  aspect_ratio_ = (float) width_/height_;
      //  init_img();
      //}

      void setPixel(const Color& c, const int x, const int y) {
        img_[at(x,y)] = c;
      }

      Color getPixel(const int x, const int y) {
        return img_[at(x,y)];
      }

      // TODO write to file
};


#endif
