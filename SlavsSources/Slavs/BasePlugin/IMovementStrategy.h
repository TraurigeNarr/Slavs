#pragma once

#include "SlavsBasePluginAPI.h"

#include "MovingGroup.h"

#include <Math\Vector2D.h>

#include <SlavsServer/include/Types.h>

namespace BasePlugin
  {

  //////////////////////////////////////////////////////////////////////////

  template <typename MovingObject>
  class IMovementStrategy
    {
    protected:
      MovingGroup<MovingObject> m_moving_group;

    public:
      IMovementStrategy() {}
      virtual           ~IMovementStrategy(){}

      virtual Vector2D  GetSteering() const = 0;
      virtual void      GetSteering(Vector2D& o_steering) const = 0;

      virtual void      SetTarget(const Vector2D& i_target) = 0;
      virtual void      SetTarget(const Slavs::GameObject& i_target_object) = 0;

      virtual void      SetClearanceToObject(double i_clearance_to_object) = 0;

      virtual double    GetClearance() const = 0;
      virtual Vector2D  GetTarget() const = 0;

    public:
      const MovingGroup<MovingObject>&      GetMovingGroup() const;
      MovingGroup<MovingObject>&            AccessMovingGroup();
    };

  //////////////////////////////////////////////////////////////////////////

  template <typename MovingObject>
  const MovingGroup<MovingObject>& IMovementStrategy<MovingObject>::GetMovingGroup() const
    {
    return m_moving_group;
    }

  template <typename MovingObject>
  MovingGroup<MovingObject>& IMovementStrategy<MovingObject>::AccessMovingGroup()
    {
    return m_moving_group;
    }

  }