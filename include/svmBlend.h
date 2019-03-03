#ifndef SVMBLEND_H_
#define SVMBLEND_H_

#ifdef WIN32
#  ifdef svmBlend_EXPORTS
#    define EXPORT __declspec(dllexport)
#  else
#    define EXPORT __declspec(dllimport)
#  endif
#else
#  define EXPORT
#endif

#ifdef __cplusplus
extern "C" {
#endif

  /**
    @brief Blends two 24-bit sRGB colors
           NOT THREAD SAFE!
    @param the sRGB channel values are stored in 8 bit, respectively,
           where RED is stored in the lower 8 bits
           (also known as BGR format in big endian byte order)
    @return the blended opaque(alpha = 1) color in BGR format
  */
  EXPORT int __cdecl blend_rgb_colors(int front_rgb,
                                      int back_rgb,
                                      float alpha);
	
#ifdef __cplusplus
}
#endif

#endif  // SVMBLEND_H_

