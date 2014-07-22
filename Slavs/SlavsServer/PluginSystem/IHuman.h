#pragma once

#include "Types.h"

#include <Common/Game/IComponent.h>

namespace Slavs
  {

  class IHouse;

  class IHuman : public IComponent
    {
    public:
      IHuman(IGameObject* ip_owner, int i_component_id) : IComponent(ip_owner, i_component_id){}
      virtual ~IHuman(){}

      virtual void            SetHome(Slavs::HousePtr ip_home) = 0;
      virtual Slavs::HousePtr GetHome() const = 0;

      virtual bool            HasWork() const = 0;
    };

  }
