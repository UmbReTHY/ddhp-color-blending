#include <cmath>
#include "cieLCh.hpp"

double CIELCh::getHue(double a, double b) {
  static double hueRad;
  if (0 == a && 0 == b) {
    return 0; // arbitrary, but common choice
  } else {
    hueRad = std::atan2(b, a);
    return (hueRad < 0) ? hueRad + TWO_PI : hueRad ;
  }
}

void CIELCh::toLab(const double lch[], double lab[]) const {
  lab[0] = lch[0];
  lab[1] = lch[1] * std::cos(lch[2]);
  lab[2] = lch[1] * std::sin(lch[2]);
}

const double CIELCh::TWO_PI = 2 * 4 * std::atan(1.0);
