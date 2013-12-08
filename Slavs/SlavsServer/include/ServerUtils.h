#ifndef ServerUtils_h
#define ServerUtils_h
//standard
#include <cassert>
#include <stdlib.h>

template<typename T>
inline void Clamp(T& arg, const T& minVal, const T& maxVal)
{
	assert(minVal < maxVal && "<ServerUtils::Clamp>: minVal > maxVal");

	if(arg < minVal)
	{
		arg = minVal;
	}
	if(arg > maxVal)
	{
		arg = maxVal;
	}
}

//-----------------------------------------------------------------------------
//some random number generator functions
//-----------------------------------------------------------------------------

//return number between 0 and 1
inline float RandFloat()
{
	return rand()/(RAND_MAX+1.0f);
}

//return number between 0 and 1
inline float RandInRange(float x, float y)
{
	return x + RandFloat()*(y-x);
}

#endif