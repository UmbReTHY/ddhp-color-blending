#ifndef MODELS_HPP_
#define MODELS_HPP_

#include <vector>

class ModelContainer {
  public:
    static const std::vector<unsigned char> getModel(const unsigned int idx);
  private:
    static const unsigned int _numBytes[3];
    static const unsigned char _compLightnessModel[18770];
    static const unsigned char _compChromaModel[24084];
    static const unsigned char _compHueModel[6871];
};

#endif  // MODELS_HPP_
