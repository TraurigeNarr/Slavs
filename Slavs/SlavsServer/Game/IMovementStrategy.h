#pragma once

class Vector2D;

namespace Slavs
  {

  class IMovementStrategy
    {
    public:
      IMovementStrategy() {}
      virtual           ~IMovementStrategy(){}

      virtual void      GetSteering(Vector2D& o_steering) const = 0;
    };

  } // Slavs