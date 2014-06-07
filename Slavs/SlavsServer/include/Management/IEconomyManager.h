#pragma once

#include "SlavsServerAPI.h"

#include "IManager.h"
#include "ManufactureRequest.h"
#include "EventsEnumerations.h"

//////////////////////////////////////////////////////////////////////////

class SLAVS_SERVER_EXPORT IEconomyManager : public IManager
{
protected:
  Slavs::TGoldKeepers    m_employers;
  Slavs::TGoldKeepers m_employees;

public:
  virtual ~IEconomyManager();

  virtual void    RegisterEmployee(Slavs::TGoldKeeper ip_payer);
  virtual void    RemoveEmployee(Slavs::TGoldKeeper ip_payer);
  
  virtual void    RegisterEmployer(Slavs::TGoldKeeper ip_payer);
  virtual void    RemoveEmployer(Slavs::TGoldKeeper ip_payer);
  
  virtual void    GetRegisteredPayers(Slavs::TGoldKeepers& o_payers);
  virtual void    GetEmplyees(Slavs::TGoldKeepers& o_payers);
  virtual void    GetEmployers(Slavs::TGoldKeepers& o_payers);

  virtual void    GetAvailableEmployers(Slavs::TEmployersInformation& o_available) const;

  virtual void    Initialize() = 0;
  virtual void    Update(long i_elapsed_time) = 0;
  virtual void    ProcessEvent(EconomyEvent i_event, void* ip_data = nullptr) = 0;

  virtual Slavs::TStoreSystem GetStoreSystem() = 0;
};