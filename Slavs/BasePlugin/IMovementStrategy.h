#pragma once

#include <Math\Vector2D.h>

namespace BasePlugin
  {
  class IMovementStrategy
    {
    public:
      virtual ~IMovementStrategy(){}

      virtual Vector2D GetSteering() = 0;
    };
  }