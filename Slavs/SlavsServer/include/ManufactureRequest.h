#pragma once
#include "Game/RequestForWorkers.h"

class SManufacureCom;

class ManufactureRequest
{
public:
  ManufactureRequest(SManufacureCom* i_request_owner);
  RequestForWorkers& GetRequest();

  SManufacureCom* GetOwner();
private:
  RequestForWorkers m_request;
  SManufacureCom* mp_request_owner;
};

typedef std::map<SManufacureCom*, ManufactureRequest*>	ManufactureRequests;