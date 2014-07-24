#pragma once

#include "SlavsServerAPI.h"

#include "Game/RequestForWorkers.h"

#include "Types.h"

class EmployerInformation
{
private:
  RequestForWorkers     m_request;
  Slavs::EmployerPtr    mp_employer;
  bool                  m_active;

public:
  SLAVS_SERVER_EXPORT       EmployerInformation(Slavs::EmployerPtr ip_owner);
  
  SLAVS_SERVER_EXPORT RequestForWorkers&          AccessRequest();
  SLAVS_SERVER_EXPORT const RequestForWorkers&    GetRequest() const;

  SLAVS_SERVER_EXPORT Slavs::EmployerPtr          GetOwner() const;

  bool IsActive() const;
  void Activate();
  void Deactivate();
};

typedef std::map<Slavs::EmployerPtr, const EmployerInformation&>	EmployersInformation;


inline void EmployerInformation::Activate()
  {
  m_active = true;
  }

inline void EmployerInformation::Deactivate()
  {
  m_active = false;
  }

inline bool EmployerInformation::IsActive() const
  {
  return m_active;
  }