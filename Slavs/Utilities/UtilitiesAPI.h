#pragma once

#ifdef UTILITIES_EXPORTING
#define UTILITIES_EXPORT __declspec(dllexport)
#else 
#define UTILITIES_EXPORT __declspec(dllimport)
#endif