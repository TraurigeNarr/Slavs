#pragma once

#include "../Math/Vector2D.h"

class IMovable
  {
  public:
    ~IMovable(){}

    virtual void      SetPosition(const Vector2D& i_position) = 0;
    virtual Vector2D  GetPosition() const = 0;
    virtual Vector2D& AccessPosition() = 0;
  };