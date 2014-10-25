#include "ManufactureRequest.h"

#include "PluginSystem/IEmployer.h"

#include "IGoldKeeper.h"


size_t RequestForWorkers::uiMaxPayment = 0;

EmployerInformation::EmployerInformation(Slavs::EmployerPtr ip_owner)
  : mp_employer(ip_owner)
  , m_active(true)
{

}

const RequestForWorkers& EmployerInformation::GetRequest() const
{
  return m_request;
}

RequestForWorkers& EmployerInformation::AccessRequest()
  {
  return m_request;
  }

Slavs::EmployerPtr EmployerInformation::GetOwner() const
{
  return mp_employer;
}

bool EmployerInformation::IsActive() const
  {
  return mp_employer->NeedWorkers();
  }