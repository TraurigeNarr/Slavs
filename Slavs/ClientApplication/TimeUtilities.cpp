#include "stdafx.h"

#include "TimeUtilities.h"

namespace TimeUtilities
  {

  float ConvertTime(long i_milliseconds)
    {
    return i_milliseconds / 1000.f;
    }

  long  ConvertTime(float i_seconds)
    {
    return static_cast<long>(i_seconds * 1000.f);
    }

  } // TimeUtilities