#include "Management/IEconomyManager.h"

#include "IController.h"
#include "SManufacureCom.h"

#include "Management/Goverment.h"

#include <algorithm>
#include <vector>
//////////////////////////////////////////////////////////////////////////

IEconomyManager::~IEconomyManager()
{

}

void IEconomyManager::RegisterEmployer(Slavs::TGoldKeeper ip_payer)
{
  m_employers.insert(ip_payer);
}

void IEconomyManager::RemoveEmployer(Slavs::TGoldKeeper ip_payer)
{
  m_employers.erase(ip_payer);
}

void IEconomyManager::RegisterEmployee(Slavs::TGoldKeeper ip_payer)
{
  m_employees.insert(ip_payer);
}

void IEconomyManager::RemoveEmployee(Slavs::TGoldKeeper ip_payer)
{
  m_employees.erase(ip_payer);
}

void IEconomyManager::GetRegisteredPayers(Slavs::TGoldKeepers& o_payers)
{
  o_payers.insert(m_employers.begin(), m_employers.end());
  o_payers.insert(m_employees.begin(), m_employees.end());
}

void IEconomyManager::GetEmplyees(Slavs::TGoldKeepers& o_payers)
{
  o_payers.insert(m_employees.begin(), m_employees.end());
}

void IEconomyManager::GetEmployers(Slavs::TGoldKeepers& o_payers)
{
  o_payers.insert(m_employers.begin(), m_employers.end());
}

void IEconomyManager::GetAvailableEmployers(Slavs::TEmployersInformation& o_available) const
  {
  std::for_each(m_employers.begin(), m_employers.end(), [&o_available](Slavs::TGoldKeeper i_employer)
    {
    if (typeid(*i_employer) == typeid(SManufacureCom))
      {
      const EmployerInformation& information = static_cast<SManufacureCom*>(i_employer)->GetInformation();
      if (information.IsActive())
        o_available.push_back(&information);
      }
    });
  }