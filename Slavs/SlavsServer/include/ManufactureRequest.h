#pragma once
#include "Game/RequestForWorkers.h"

#include "Types.h"

class IGoldKeeper;
class SManufacureCom;

class EmployerInformation
{
private:
  RequestForWorkers m_request;
  SManufacureCom* mp_employer;

  bool m_active;

public:
  EmployerInformation(SManufacureCom* ip_owner);
  
  RequestForWorkers& AccessRequest();
  const RequestForWorkers& GetRequest() const;

  SManufacureCom* GetOwner() const;

  inline bool IsActive() const;
  inline void Activate();
  inline void Deactivate();
};

typedef std::map<SManufacureCom*, const EmployerInformation&>	EmployersInformation;


void EmployerInformation::Activate()
  {
  m_active = true;
  }

void EmployerInformation::Deactivate()
  {
  m_active = false;
  }

bool EmployerInformation::IsActive() const
  {
  return m_active;
  }