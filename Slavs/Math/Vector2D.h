#pragma once

#include "MathAPI.h"

#include "Vector.h"

class Vector2D : public Vector<float, 2>
  {
  public:
    MATH_EXPORT Vector2D();
    MATH_EXPORT Vector2D (float i_x, float i_y);
  };