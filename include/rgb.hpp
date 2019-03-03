#ifndef RGB_HPP_
#define RGB_HPP_

#include <vector>

class RGB {
  public:
    RGB();

    void toXYZ(const double rgb[], double xyz[]);
    void fromXYZ(const double xyz[], double rgb[]) const;
    void clamp(double rgb[]) const;

  private:
    void linearize(const double rgb[]);
    void compress(double rgb[]) const;

    // caches the linearRGB intermediate result
    double* pLinearRGB;
    std::vector<double> linearRGB;
    // stores the rgb to xyz transformation matrix
    static const double A[3][3];
    // stores the xyz to rgb transformation matrix
    static const double B[3][3];
};

#endif  // RGB_HPP_

