#pragma once

#include "Types.h"
#include "SlavsServerAPI.h"

#include <memory>
#include <vector>

//////////////////////////////////////////////////////////////////////////
class IEconomyManager;
class Goverment;


/*
This class can be accessed through ServerMain singleton.
It provides access to Economy managers for controllers.
Each controller has its own economy manager which can be defined
in separate modules.
Design of this class assumes that it WOULD not be inherited.
*/

class SLAVS_SERVER_EXPORT GlobalEconomics
{
private:
  Slavs::TEconomyManagers m_economy_managers;
public:
  GlobalEconomics();
  ~GlobalEconomics();

  void              RegisterEconomy(Slavs::TEconomy i_economy);
  void              RemoveEconomy(Slavs::TEconomy i_economy);

  Slavs::TEconomy   GetEconomy(Goverment* ip_economy_owner) const;
};