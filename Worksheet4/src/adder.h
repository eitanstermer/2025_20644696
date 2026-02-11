#pragma once

#ifdef maths_STATIC
  #define MATHSLIB_API
#else
  #ifdef maths_EXPORTS
    #define MATHSLIB_API __declspec(dllexport)
  #else
    #define MATHSLIB_API __declspec(dllimport)
  #endif
#endif

extern "C" MATHSLIB_API int add(int a, int b);
