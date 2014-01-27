#include "ManufactureRequest.h"

#include "SManufacureCom.h"

#include "IGoldKeeper.h"


size_t RequestForWorkers::uiMaxPayment = 0;

EmployerInformation::EmployerInformation(SManufacureCom* ip_owner)
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

SManufacureCom* EmployerInformation::GetOwner() const
{
  return mp_employer;
}