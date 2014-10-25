#pragma once

#ifdef MATH_EXPORTING
#define MATH_EXPORT __declspec(dllexport)
#else 
#define MATH_EXPORT __declspec(dllimport)
#endif