#pragma once

#include "SlavsServerAPI.h"

#include "Types.h"

// unique_ptr needs complete destructor of object
// so we need to include headers for interfaces
#include "Management/IEconomyManager.h"
#include "Management/IMilitaryManager.h"
#include "Management/IRelashionshipsManager.h"
#include "Management/IStoreSystem.h"
#include "Management/ITechnologyManager.h"

#include <boost/noncopyable.hpp>

#include <memory>

/*
Class that encapsulates all management systems.
Economy; Military; Technology; Society (in country);
Relationships (out of country)
*/

class Goverment : boost::noncopyable
  {
private:
  IController*                            mp_owner;
  std::unique_ptr<IEconomyManager>        m_economy_manager;
  std::unique_ptr<IMilitaryManager>       m_military;
  std::unique_ptr<IRelashionshipsManager> m_relashionships;
  std::unique_ptr<ISocietyManager>        m_society;
  std::unique_ptr<ITechnologyManager>     m_technologies;

public:
  SLAVS_SERVER_EXPORT           Goverment(IController* ip_owner, 
                                          std::unique_ptr<IEconomyManager>&& ip_economy, 
                                          std::unique_ptr<IMilitaryManager>&& ip_military, 
                                          std::unique_ptr<IRelashionshipsManager>&& ip_relashionships, 
                                          std::unique_ptr<ISocietyManager>&& ip_society, 
                                          std::unique_ptr<ITechnologyManager>&& ip_technologies);
  SLAVS_SERVER_EXPORT virtual    ~Goverment();

  //initializes all managers
  SLAVS_SERVER_EXPORT virtual void           Initialize();

  SLAVS_SERVER_EXPORT Slavs::TEconomy        GetEconomyManager() const;
  SLAVS_SERVER_EXPORT Slavs::TMilitary       GetMilitaryManager() const;
  SLAVS_SERVER_EXPORT Slavs::TRelashionships GetRelationshipManager() const;
  SLAVS_SERVER_EXPORT Slavs::TSociety        GetSocietyManager() const;
  SLAVS_SERVER_EXPORT Slavs::TTechnologies   GetTechnologyManager() const;
  SLAVS_SERVER_EXPORT IController*           GetOwner() const;
  };