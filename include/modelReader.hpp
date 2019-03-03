#ifndef MODELREADER_HPP_
#define MODELREADER_HPP_

struct svm_model;

class ModelReader {
  public:
    svm_model* fromString(const char modelText[]) const;

  private:
    static const char* svm_type_table[6];
    static const char* kernel_type_table[6];
};

#endif  // MODELREADER_HPP_

