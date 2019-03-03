#include <cmath>
#include "cieLab.hpp"

void CIELab::fromXYZ(const double xyz[], double lab[]) const {
  static double tmp;
  tmp = fromXYZHelper(xyz[1] / _XYZn[1]);
  lab[0] = 116 * tmp - 16;
  lab[1] = 500 * (fromXYZHelper(xyz[0] / _XYZn[0]) - tmp);
  lab[2] = 200 * (tmp - fromXYZHelper(xyz[2] / _XYZn[2]));
}

void CIELab::toXYZ(const double lab[], double xyz[]) const {
  static double tmp;
  tmp = (1 / 116.0) * (lab[0] + 16);
  xyz[1] = _XYZn[1] * toXYZHelper(tmp);
  xyz[0] = _XYZn[0] * toXYZHelper(tmp + (1 / 500.0) * lab[1]);
  xyz[2] = _XYZn[2] * toXYZHelper(tmp - (1 / 200.0) * lab[2]);
}

double CIELab::fromXYZHelper(double t) const {
  if (t > ((6.0 / 29.0) * (6.0 / 29.0) * (6.0 / 29.0)))
    return std::pow(t, 1.0 / 3.0);
  else
    return (1.0 / 3.0) * (29.0 / 6.0) * (29.0 / 6.0) * t + (4.0 / 29.0);
}

double CIELab::toXYZHelper(double t) const {
  return (t > 6.0 / 29.0) ? t * t * t :
         3 * (6.0 / 29.0) * (6.0 / 29.0) * (t - 4.0 / 29.0);
}

const double CIELab::_XYZn[3] = {0.31271 / 0.32902,
                                 1, (1 - 0.31271 - 0.32902) / 0.32902};
