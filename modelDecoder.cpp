#include <cstdlib>
#include <string>
#include <vector>

#include "zlib/zlib.h"

#include "modelDecoder.hpp"

using std::string;
using std::vector;

string ModelDecoder::decodeModel(const vector<unsigned char>& codedModel) {
  uLongf decodedSize = 40000000; // 40 MB
  Bytef* decodedBuffer = reinterpret_cast<Bytef*>(malloc(decodedSize));
  uncompress OF((decodedBuffer, &decodedSize,
                &codedModel[0], codedModel.size()));
  char* pString = reinterpret_cast<char*>(decodedBuffer);
  // append the \0 char
  pString[decodedSize] = '\0';
  string result(pString);
  free(decodedBuffer);
  return result;
}
