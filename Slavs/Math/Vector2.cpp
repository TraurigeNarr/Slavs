#include "stdafx.h"

#include "Vector2.h"

Vector2D::Vector2D ()
  {  }

Vector2D::Vector2D (float i_x, float i_y)
  {
  (*this)[0] = i_x;
  (*this)[1] = i_y;
  }