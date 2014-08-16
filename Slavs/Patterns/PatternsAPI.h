#pragma once

#pragma once

#ifdef PATTERNS_EXPORTING
#define PATTERN_EXPORT __declspec(dllexport)
#else 
#define PATTERN_EXPORT __declspec(dllimport)
#endif