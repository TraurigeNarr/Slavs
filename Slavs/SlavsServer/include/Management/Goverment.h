#pragma once

#include "../SlavsServerAPI.h"

#include "Types.h"

#include <memory>

/*
Class that encapsulates all management systems.
Economy; Military; Technology; Society (in country);
Relationships (out of country)
*/

class SLAVS_SERVER_EXPORT Goverment
{
private:
  IController*    mp_owner;
  Slavs::TEconomy        m_economy_manager;
  Slavs::TMilitary       m_military;
  Slavs::TRelashionships m_relashionships;
  Slavs::TSociety        m_society;
  Slavs::TTechnologies   m_technologies;
public:
  Goverment(IController* ip_owner, Slavs::TEconomy i_economy, 
    Slavs::TMilitary i_military, Slavs::TRelashionships i_relashionships, 
    Slavs::TSociety i_society, Slavs::TTechnologies i_technologies);
  ~Goverment();

  //initializes all managers
  bool            Initialize();

  Slavs::TEconomy        GetEconomyManager();
  Slavs::TMilitary       GetMilitaryManager();
  Slavs::TRelashionships GetRelationshipManager();
  Slavs::TSociety        GetSocietyManager();
  Slavs::TTechnologies   GetTechnologyManager();
};