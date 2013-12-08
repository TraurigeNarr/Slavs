#include "ManufactureRequest.h"
#include "SManufacureCom.h"

size_t RequestForWorkers::uiMaxPayment = 0;

ManufactureRequest::ManufactureRequest(SManufacureCom* i_request_owner)
  : mp_request_owner(i_request_owner)
{

}

RequestForWorkers& ManufactureRequest::GetRequest()
{
  return m_request;
}

SManufacureCom* ManufactureRequest::GetOwner()
{
  return mp_request_owner;
}