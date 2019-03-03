#include <cstdio>
#include "svmBlend.h"

int arrayToInt(const int rgb[]);
void intToArray(const int packed, int rgb[]);

int main(int argc, char* argv[]) {
  int frontArray[] = {255, 0, 0};
  int backArray[] = {0, 128, 0};
  float alpha = 0.5f;

  int result = blend_rgb_colors(arrayToInt(frontArray),
                                arrayToInt(backArray),
                                alpha);
  int resultArray[3];
  intToArray(result, resultArray);

  std::printf("Front: %i,%i,%i\n"
              "Back: %i,%i,%i\n"
              "Result: %i,%i,%i\n",
              frontArray[0], frontArray[1], frontArray[2],
              backArray[0], backArray[1], backArray[2],
              resultArray[0], resultArray[1], resultArray[2]);

  return 0;
}

int arrayToInt(const int rgb[]) {
  return (rgb[0] | (rgb[1] << 8) | (rgb[2] << 16));
}

void intToArray(const int packed, int rgb[]) {
  for (int i = 0; i < 3; i++)
    rgb[i] = ((packed >> (i * 8)) & 0xFF);
}
