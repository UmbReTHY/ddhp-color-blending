#ifndef MODELDECODER_HPP_
#define MODELDECODER_HPP_

#include <string>
#include <vector>

class ModelDecoder {
  public:
    static std::string decodeModel(const std::vector<unsigned char>& codedModel);
};

#endif  // MODELDECODER_HPP_

