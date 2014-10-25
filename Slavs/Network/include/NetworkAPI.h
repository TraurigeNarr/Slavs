#pragma once

#ifdef NETWORKEXPORTING
#define NETWORKEXPORT __declspec(dllexport)
#else 
#define NETWORKEXPORT __declspec(dllimport)
#endif