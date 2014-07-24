#pragma once

#include "../SlavsServerAPI.h"

#include "Types.h"

#include <memory>

/*
Class that encapsulates all management systems.
Economy; Military; Technology; Society (in country);
Relationships (out of country)
*/

class Goverment
{
private:
  IController*           mp_owner;
  Slavs::TEconomy        m_economy_manager;
  Slavs::TMilitary       m_military;
  Slavs::TRelashionships m_relashionships;
  Slavs::TSociety        m_society;
  Slavs::TTechnologies   m_technologies;

public:
  SLAVS_SERVER_EXPORT Goverment(IController* ip_owner, 
                                Slavs::TEconomy i_economy, 
                                Slavs::TMilitary i_military, 
                                Slavs::TRelashionships i_relashionships, 
                                Slavs::TSociety i_society, 
                                Slavs::TTechnologies i_technologies);
  SLAVS_SERVER_EXPORT ~Goverment();

  //initializes all managers
  bool                  SLAVS_SERVER_EXPORT  Initialize();

  Slavs::TEconomy        SLAVS_SERVER_EXPORT GetEconomyManager();
  Slavs::TMilitary       SLAVS_SERVER_EXPORT GetMilitaryManager();
  Slavs::TRelashionships SLAVS_SERVER_EXPORT GetRelationshipManager();
  Slavs::TSociety        SLAVS_SERVER_EXPORT GetSocietyManager();
  Slavs::TTechnologies   SLAVS_SERVER_EXPORT GetTechnologyManager();
};