#ifndef CIELCH_HPP_
#define CIELCH_HPP_

class CIELCh {
  public:
    // for efficiency, returns the hue in PI radians
    double getHue(double a, double b);
    void toLab(const double lch[], double lab[]) const;
  private:
    static const double TWO_PI;
};

#endif  // CIELCH_HPP_

