#pragma once
#include "Game/RequestForWorkers.h"

#include "Types.h"

class EmployerInformation
{
private:
  RequestForWorkers     m_request;
  Slavs::EmployerPtr    mp_employer;
  bool                  m_active;

public:
  EmployerInformation(Slavs::EmployerPtr ip_owner);
  
  RequestForWorkers&        AccessRequest();
  const RequestForWorkers&  GetRequest() const;

  Slavs::EmployerPtr     GetOwner() const;

  inline bool IsActive() const;
  inline void Activate();
  inline void Deactivate();
};

typedef std::map<Slavs::EmployerPtr, const EmployerInformation&>	EmployersInformation;


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