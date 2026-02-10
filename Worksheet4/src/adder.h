#pragma once

#ifdef _WIN32
  #ifdef ADDER_EXPORTS
    #define ADDER_API __declspec(dllexport)
  #else
    #define ADDER_API __declspec(dllimport)
  #endif
#else
  #define ADDER_API
#endif

extern "C" ADDER_API int add(int a, int b);
