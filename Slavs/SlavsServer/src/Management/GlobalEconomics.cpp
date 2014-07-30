#include "Management/GlobalEconomics.h"

#include "Management/IEconomyManager.h"
#include "Management/Goverment.h"

#include "Utilities/TemplateFunctions.h"

#include <algorithm>
#include <cassert>

namespace
{

  Slavs::TEconomyManagers::const_iterator _FindEconomy (const Slavs::TEconomyManagers& i_managers, const Goverment* ip_economy_owner)
  {
    return std::find_if (i_managers.cbegin(), i_managers.cend(), [ip_economy_owner] (Slavs::TEconomy ip_economy) -> bool
    {
      return ip_economy->GetGoverment() == ip_economy_owner;
    });
  }

  bool _EconomyExists (const Slavs::TEconomyManagers& i_managers, const Slavs::TEconomy ip_economy)
  {
    assert (nullptr != ip_economy->GetGoverment() && "Economy owner is nullptr");
    
    return i_managers.end() != _FindEconomy(i_managers, ip_economy->GetGoverment());
  }

  void _RemoveEconomy (Slavs::TEconomyManagers& i_managers, const Slavs::TEconomy ip_economy)
  {
    Slavs::TEconomyManagers::const_iterator it = _FindEconomy(i_managers, ip_economy->GetGoverment());
    if (it != i_managers.end())
      i_managers.erase(it);
  }

} // namespace

//////////////////////////////////////////////////////////////////////////

GlobalEconomics::GlobalEconomics()
{

}

GlobalEconomics::~GlobalEconomics()
{

}

void GlobalEconomics::RegisterEconomy(Slavs::TEconomy i_economy)
{
  if (!_EconomyExists(m_economy_managers, i_economy))
    m_economy_managers.push_back(i_economy);
}

void GlobalEconomics::RemoveEconomy(Slavs::TEconomy i_economy)
{
  _RemoveEconomy(m_economy_managers, i_economy);
}

Slavs::TEconomy GlobalEconomics::GetEconomy(Goverment* ip_economy_owner) const
{
  return *_FindEconomy(m_economy_managers, ip_economy_owner);
}