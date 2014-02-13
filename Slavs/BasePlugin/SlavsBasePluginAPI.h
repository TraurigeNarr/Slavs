#pragma once

#pragma once

#ifdef SLAVS_BASEPLUGIN_EXPORTING
#define SLAVS_BASEPLUGIN_EXPORT __declspec(dllexport)
#else 
#define SLAVS_BASEPLUGIN_EXPORT __declspec(dllimport)
#endif