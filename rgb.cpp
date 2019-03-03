#include <cmath>
#include "rgb.hpp"

RGB::RGB()
  : linearRGB(3) {
  pLinearRGB = &linearRGB[0];
}

void RGB::toXYZ(const double rgb[], double xyz[]) {
  linearize(rgb);
  for (int i = 0; i < 3; i++) {
    xyz[i] = 0;
    for (int j = 0; j < 3; j++)
      xyz[i] += A[i][j] * pLinearRGB[j];
  }
}

void RGB::fromXYZ(const double xyz[], double rgb[]) const {
  for (int i = 0; i < 3; i++) {
    rgb[i] = 0;
    for (int j = 0; j < 3; j++)
      rgb[i] += B[i][j] * xyz[j];
  }
  compress(rgb);
}

void RGB::clamp(double rgb[]) const {
  for (int i = 0; i < 3; i++)
    if (rgb[i] > 1.0)
      rgb[i] = 1.0;
    else if (rgb[i] < 0.0)
      rgb[i] = 0.0;
}

void RGB::linearize(const double rgb[]) {
  for (int i = 0; i < 3; i++)
      pLinearRGB[i] =  (rgb[i] <= 0.4045) ?
                        rgb[i] / 12.92 :
                        std::pow((rgb[i] + 0.055) / 1.055, 2.4);
}

void RGB::compress(double rgb[]) const {
  for (int i = 0; i < 3; i++)
    rgb[i] = (rgb[i] <= 0.0031308) ? 12.92 * rgb[i] :
             1.055 * std::pow(rgb[i], 1 / 2.4) - 0.055;
}

const double RGB::A[3][3] = {{0.4124, 0.3576, 0.1805},
                             {0.2126, 0.7152, 0.0722},
                             {0.0193, 0.1192, 0.9505}};
const double RGB::B[3][3] = {{3.2406, -1.5372, -0.4986},
                             {-0.9689, 1.8758, 0.0415},
                             {0.0557, -0.2040, 1.0570}};
