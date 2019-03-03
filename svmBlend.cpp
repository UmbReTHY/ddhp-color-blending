#include <cmath>
#include <vector>

#include "rgb.hpp"
#include "cieLab.hpp"
#include "cieLCh.hpp"
#include "modelReader.hpp"
#include "modelContainer.hpp"
#include "modelDecoder.hpp"

#include "svmBlend.h"
#include "svm.h"

using std::vector;

class SVMBlender {
  public:
    SVMBlender()
      : _cieLabSpace(), _cieLChSpace(), _rgbSpace(), _normRGB(3), _xyz(3),
        _frontLab(3), _backLab(3), _lch(3), _nodes(8) {
      // initialize the indeces of the nodes just once
      for (int i = 0; i < 7; i++)
        _nodes[i].index = i + 1;
      // set the index of the last node to -1 to indicate the end of the vector
      _nodes[7].index = -1;
      // load the models
      ModelReader modelReader;
      _pLightModel = modelReader.fromString(ModelDecoder::decodeModel(ModelContainer::getModel(0)).c_str());
      _pChromaModel = modelReader.fromString(ModelDecoder::decodeModel(ModelContainer::getModel(1)).c_str());
      _pHueModel = modelReader.fromString(ModelDecoder::decodeModel(ModelContainer::getModel(2)).c_str());
    }

    ~SVMBlender() {
      svm_free_and_destroy_model(&_pHueModel);
      svm_free_and_destroy_model(&_pLightModel);
      svm_free_and_destroy_model(&_pChromaModel);
    }

  int blendRGBColors(int frontRGB, int backRGB, float alpha) {
    // initialize the feature vector
    fillFeatureVector(frontRGB, &_frontLab[0], 0);
    fillFeatureVector(backRGB, &_backLab[0], 3);
    _nodes[6].value = alpha;
    // normalize the feature vector
    normalizeNodes();
    // execute the prediction
    predictLCh();
    // convert back to rgb
    _cieLChSpace.toLab(&_lch[0], &_frontLab[0]);
    _cieLabSpace.toXYZ(&_frontLab[0], &_xyz[0]);
    _rgbSpace.fromXYZ(&_xyz[0], &_normRGB[0]);
    // take care of rounding errors
    _rgbSpace.clamp(&_normRGB[0]);
    // pack the rgb components into an int
    formatResult();
    return _result;
  }

  private:
    // converts to normalized (= [0,1]) double and stores into given array
    void normalizeRGB(int rgb, double rgbArray[]) {
      for (int i = 0; i < 3; i++)
        rgbArray[i] = (((rgb >> (8 * i)) & 0xFF )) / 255.0;
    }

    void rgbToLab(const double rgb[], double lab[]) {
      _rgbSpace.toXYZ(rgb, &_xyz[0]);
      _cieLabSpace.fromXYZ(&_xyz[0], lab);
    }

    void fillFeatureVector(int rgb, double lab[], int nodeIdx) {
      normalizeRGB(rgb, &_normRGB[0]);
      rgbToLab(&_normRGB[0], lab);
      for (int i = 0; i < 3; ++i)
        _nodes[nodeIdx++].value = lab[i];
    }

    // normalize to [-1,1]
    void normalizeNodes() {
      for (int i = 0; i < 7; i++)
        _nodes[i].value = 2 * (_nodes[i].value - MIN[i]) / (MAX[i] - MIN[i])
                          - 1;
    }

    void predictLCh() {
      _lch[0] = svm_predict(_pLightModel, &_nodes[0]);
      _lch[1] = svm_predict(_pChromaModel, &_nodes[0]);
      _lch[2] = (0 == svm_predict(_pHueModel, &_nodes[0])) ?
                  _cieLChSpace.getHue(_frontLab[1], _frontLab[2]) :
                  _cieLChSpace.getHue(_backLab[1], _backLab[2]);
    }

    int symmetricRound(const double value) {
      return static_cast<int>(std::floor(value + 0.5));
    }

    void formatResult() {
      _result = 0;
      for (int i = 0; i < 3; i++)
        _result = _result | (symmetricRound(_normRGB[i] * 255) << (8 * i));
    }

    // required color spaces
    CIELab _cieLabSpace;
    CIELCh _cieLChSpace;
    RGB _rgbSpace;

    // members for caching purposes
    vector<double> _normRGB;
    vector<double> _xyz;
    vector<double> _frontLab;
    vector<double> _backLab;
    vector<double> _lch;
    vector<svm_node> _nodes;
    int _result;

    // the models for prediction
    svm_model* _pHueModel;
    svm_model* _pLightModel;
    svm_model* _pChromaModel;

    // the MIN and MAX values for normalizing the feature vector
    static const double MIN[7];
    static const double MAX[7];
};

const double SVMBlender::MIN[7] = {0, -73.9079, -97.7963,
                                   0, -73.9079, -97.7963,
                                   0};
const double SVMBlender::MAX[7] = {100, 90.86239999999999, 88.0429,
                                   100, 90.86239999999999, 88.0429,
                                   1};

extern int /*__cdecl*/ blend_rgb_colors(int front_rgb, int back_rgb, float alpha) {
  static SVMBlender svmBlender;
  // handle the trivial cases without the need to ask the blender
  if (alpha == 1.0f)
    return front_rgb;
  else if (alpha == 0.0f)
    return back_rgb;
  else
    return svmBlender.blendRGBColors(front_rgb, back_rgb, alpha); 
}

