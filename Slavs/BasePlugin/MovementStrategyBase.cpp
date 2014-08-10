#include "stdafx.h"

#include "MovementStrategyBase.h"

#include <SlavsServer/Game/GameObject.h>


namespace BasePlugin
  {

  MovementStrategyBase::MovementStrategyBase()
    : m_target()
    , m_clearance_to_object(10.)
    {    }

  MovementStrategyBase::~MovementStrategyBase()
    {    }

  //////////////////////////////////////////////////////////////////////////
  // IMovementStrategy

  Vector2D MovementStrategyBase::GetSteering() const
    {
    Vector2D steering;
    GetSteering(steering);
    return steering;
    }

  void MovementStrategyBase::GetSteering(Vector2D& o_steering) const
    {
    o_steering = m_target;
    }

  //////////////////////////////////////////////////////////////////////////
  // MovementStrategyBase internal staff

  void MovementStrategyBase::SetTarget(const Vector2D& i_target)
    {
    m_target = i_target;
    }

  void MovementStrategyBase::SetTarget(const Slavs::GameObject& i_target_object)
    {
    m_target = i_target_object.GetPosition();
    }

  void MovementStrategyBase::SetClearanceToObject(double i_clearance_to_object)
    {
    m_clearance_to_object = i_clearance_to_object;
    }

  } // BasePlugin