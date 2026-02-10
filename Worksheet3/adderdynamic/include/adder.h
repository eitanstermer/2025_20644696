#pragma once

#ifdef ADDER_EXPORTS
#define ADDER_API __declspec(dllexport)
#else
#define ADDER_API __declspec(dllimport)
#endif

extern "C" ADDER_API int add(int a, int b);
