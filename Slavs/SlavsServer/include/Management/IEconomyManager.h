#pragma once

#include "SlavsServerAPI.h"

#include "EventsEnumerations.h"
#include "Types.h"

#include <vector>
#include <memory>

//////////////////////////////////////////////////////////////////////////

class IController;

class SLAVS_SERVER_EXPORT IEconomyManager
{
protected:
  Slavs::TGoldKeepers m_employers;
  Slavs::TGoldKeepers m_employees;
  IController* mp_owner;
public:
  virtual ~IEconomyManager();

  void            SetOwner(IController* ip_owner);
  IController*    GetOwner() const;

  virtual void    RegisterEmployee(Slavs::TGoldKeeper ip_payer);
  virtual void    RemoveEmployee(Slavs::TGoldKeeper ip_payer);
  
  virtual void    RegisterEmployer(Slavs::TGoldKeeper ip_payer);
  virtual void    RemoveEmployer(Slavs::TGoldKeeper ip_payer);
  
  virtual void    GetRegisteredPayers(Slavs::TGoldKeepers& o_payers);
  virtual void    GetEmplyees(Slavs::TGoldKeepers& o_payers);
  virtual void    GetEmployers(Slavs::TGoldKeepers& o_payers);

  virtual void    Initialize() = 0;
  virtual void    Update(long i_elapsed_time) = 0;
  virtual void    ProcessEvent(EconomyEvent i_event, void* ip_data = nullptr) = 0;

  virtual Slavs::TStoreSystem GetStoreSystem() = 0;
};