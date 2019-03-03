#ifndef CIELAB_HPP_
#define CIELAB_HPP_

class CIELab {
  public:
    void fromXYZ(const double xyz[], double lab[]) const;
    void toXYZ(const double lab[], double xyz[]) const;

  private:
    double fromXYZHelper(double t) const;
    double toXYZHelper(double t) const;

    // D65 white point
    static const double _XYZn[3];
};

#endif  // CIELAB_HPP_

