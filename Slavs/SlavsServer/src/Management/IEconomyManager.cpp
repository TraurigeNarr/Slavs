#include "Management/IEconomyManager.h"
#include "IController.h"

#include <Utilities/TemplateFunctions.h>

#include <Game/GameResources.h>

#include <algorithm>

IEconomyManager::~IEconomyManager()
{

}

void IEconomyManager::SetOwner(IController* ip_owner)
{
  mp_owner = ip_owner;
}

IController* IEconomyManager::GetOwner() const
{
  return mp_owner;
}

void IEconomyManager::RegisterEmployer(Slavs::TGoldKeeper ip_payer)
{
  Slavs::TGoldKeepers::iterator it = std::find(m_employers.begin(), m_employers.end(), ip_payer);

  if (it == m_employers.end())
    m_employers.push_back(ip_payer);
}

void IEconomyManager::RemoveEmployer(Slavs::TGoldKeeper ip_payer)
{
  Slavs::TGoldKeepers::iterator it = std::find(m_employers.begin(), m_employers.end(), ip_payer);

  std::remove_if (m_employers.begin(), m_employers.end(), [ip_payer](Slavs::TGoldKeeper ip_keeper) -> bool
  {
    return ip_payer == ip_payer;
  });
}

void IEconomyManager::RegisterEmployee(Slavs::TGoldKeeper ip_payer)
{
  Slavs::TGoldKeepers::iterator it = std::find(m_employees.begin(), m_employees.end(), ip_payer);

  if (it == m_employees.end())
    m_employees.push_back(ip_payer);
}

void IEconomyManager::RemoveEmployee(Slavs::TGoldKeeper ip_payer)
{
  Slavs::TGoldKeepers::iterator it = std::find(m_employees.begin(), m_employees.end(), ip_payer);

  std::remove_if (m_employees.begin(), m_employees.end(), [ip_payer](Slavs::TGoldKeeper ip_keeper) -> bool
  {
    return ip_payer == ip_payer;
  });
}

void IEconomyManager::GetRegisteredPayers(Slavs::TGoldKeepers& o_payers)
{
  o_payers.insert(o_payers.begin(), m_employers.begin(), m_employers.end());
  o_payers.insert(o_payers.begin(), m_employees.begin(), m_employees.end());
}

void IEconomyManager::GetEmplyees(Slavs::TGoldKeepers& o_payers)
{
  o_payers.insert(o_payers.begin(), m_employees.begin(), m_employees.end());
}

void IEconomyManager::GetEmployers(Slavs::TGoldKeepers& o_payers)
{
  o_payers.insert(o_payers.begin(), m_employers.begin(), m_employers.end());
}