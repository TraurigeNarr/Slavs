#pragma once

#include "Types.h"

#include <Game/IComponent.h>

#include "IGoldKeeper.h"

namespace Slavs
  {

  class IEmployer : public IComponent
                  , public IGoldKeeper
    {
    public:
      IEmployer(IGameObject* ip_owner, int i_component_id) : IComponent(ip_owner, i_component_id){}
      virtual ~IEmployer(){}

      virtual int     GetProducedResourceType() const = 0;
      virtual int     GetWorkerType() const = 0;
      virtual bool    NeedWorkers() const = 0;
      virtual bool    IsWorking() const = 0;
      virtual size_t  GetOperatingCycle() const = 0;

      virtual const EmployerInformation& GetInformation() const = 0;

      //calls from ResourceManager if there is a population. Returns true if hired, false otherwise
      //@param worker - human that can be hired. if he will - moves to position of factory(temporary)
      virtual bool    IsSuitable(Slavs::HumanPtr ip_human) = 0;
      //returns true, if this human`s profession information(e.x. skill) is suitable for this manufacture requirements
      //@param profession - human`s profession information, NULL if human has no profession of this type
      //returns only true for now
      virtual bool    HireWorker(Slavs::HumanPtr ip_human) = 0;

      virtual void    StoreExpanded() = 0;
    };

  } // Slavs