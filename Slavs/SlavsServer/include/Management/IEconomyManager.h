#pragma once

#include "SlavsServerAPI.h"

#include "IManager.h"
#include "ManufactureRequest.h"
#include "EventsEnumerations.h"

//////////////////////////////////////////////////////////////////////////

class IEconomyManager : public IManager
{
protected:
  Slavs::TGoldKeepers   m_employers;
  Slavs::TGoldKeepers   m_employees;

public:
  virtual SLAVS_SERVER_EXPORT         ~IEconomyManager();

  virtual void    SLAVS_SERVER_EXPORT RegisterEmployee(Slavs::TGoldKeeper ip_payer);
  virtual void    SLAVS_SERVER_EXPORT RemoveEmployee(Slavs::TGoldKeeper ip_payer);
  
  virtual void    SLAVS_SERVER_EXPORT RegisterEmployer(Slavs::TGoldKeeper ip_payer);
  virtual void    SLAVS_SERVER_EXPORT RemoveEmployer(Slavs::TGoldKeeper ip_payer);
  
  virtual void    SLAVS_SERVER_EXPORT GetRegisteredPayers(Slavs::TGoldKeepers& o_payers);
  virtual void    SLAVS_SERVER_EXPORT GetEmplyees(Slavs::TGoldKeepers& o_payers);
  virtual void    SLAVS_SERVER_EXPORT GetEmployers(Slavs::TGoldKeepers& o_payers);

  virtual void    SLAVS_SERVER_EXPORT GetAvailableEmployers(Slavs::TEmployersInformation& o_available) const;

  virtual void    Initialize() = 0;
  virtual void    Update(long i_elapsed_time) = 0;
  virtual void    ProcessEvent(EconomyEvent i_event, void* ip_data = nullptr) = 0;

  virtual Slavs::TStoreSystem GetStoreSystem() = 0;
};