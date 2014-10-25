#pragma once

#include "SlavsBasePluginAPI.h"

#include "IMovementStrategy.h"

#include <SlavsServer/include/Types.h>

namespace BasePlugin
  {

  class DynamicObjectComponent;

  class MovementStrategyBase : public IMovementStrategy<DynamicObjectComponent>
    {
    private:
      Vector2D  m_target;
      double    m_clearance_to_object;

    public:
      SLAVS_BASEPLUGIN_EXPORT                     MovementStrategyBase();
      SLAVS_BASEPLUGIN_EXPORT virtual             ~MovementStrategyBase();

    // IMovementStrategy
    public:
      virtual Vector2D  SLAVS_BASEPLUGIN_EXPORT   GetSteering() const override;
      virtual void      SLAVS_BASEPLUGIN_EXPORT   GetSteering(Vector2D& o_steering) const override;

      virtual void      SLAVS_BASEPLUGIN_EXPORT   SetTarget(const Vector2D& i_target);
      virtual void      SLAVS_BASEPLUGIN_EXPORT   SetTarget(const Slavs::GameObject& i_target_object);

      virtual void      SLAVS_BASEPLUGIN_EXPORT   SetClearanceToObject(double i_clearance_to_object);

      virtual double                                      GetClearance() const;
      virtual Vector2D                                    GetTarget() const;
    };


  //////////////////////////////////////////////////////////////////////////
  inline double MovementStrategyBase::GetClearance() const
    {
    return m_clearance_to_object;
    }

  inline Vector2D MovementStrategyBase::GetTarget() const
    {
    return m_target;
    }

  } // BasePlugin