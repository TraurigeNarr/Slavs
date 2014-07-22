#pragma once

#include "Types.h"

#include <Common/Game/IComponent.h>

#include <vector>

namespace Slavs
  {
  class IHuman;

  class IHouse : public IComponent
  {
  public:
    IHouse(IGameObject* ip_owner, int i_component_id) : IComponent(ip_owner, i_component_id){}
    virtual ~IHouse(){}

    /// gets number of ALL people who have already live in house
    virtual size_t	GetPopulation() const = 0;

    /// gets number of people who has no work
    virtual size_t	GetUnemployedPopulation() const = 0;
    /// push back into vector with unemployed workers;
    virtual void		GetUnemployedPopulation(Humans& o_unemployed) const = 0;

    /// gets free places for people
    virtual size_t	GetFreePlaces() const = 0;

    //add inhabitant in list of house owners
    virtual bool		AddInhabitant(HumanPtr ip_inhabitant) = 0;
    /// remove inhabitant from house
    virtual void    RemoveInhabitant(HumanPtr ip_inhabitant) = 0;

    /// invalidates table of information about humans that
    /// are living in house (e.x., if human get new or loose job)
    virtual void    HumanStateChanged() = 0;
  };

  } // Slavs