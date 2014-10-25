#pragma once

#ifdef SLAVSEXPORTING
  #define SLAVS_SERVER_EXPORT __declspec(dllexport)
#else 
  #define SLAVS_SERVER_EXPORT __declspec(dllimport)
#endif