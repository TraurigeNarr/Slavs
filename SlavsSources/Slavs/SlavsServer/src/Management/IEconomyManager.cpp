#include "Management/IEconomyManager.h"

#include "IController.h"

#include "PluginSystem/IEmployer.h"

#include "Management/GlobalEconomics.h"
#include "Management/Goverment.h"

#include "Game/GameContext.h"
#include "Game/GameObject.h"

#include <algorithm>
#include <vector>
//////////////////////////////////////////////////////////////////////////

IEconomyManager::~IEconomyManager()
  {
  if (GetGoverment())
    GetGoverment()->GetOwner()->GetGameContext().GetGlobalEconomics().RemoveEconomy(this);
  }

void IEconomyManager::RegisterEmployer(Slavs::EmployerPtr ip_employer)
  {
  m_employers.insert(ip_employer);
  }

void IEconomyManager::RemoveEmployer(Slavs::EmployerPtr ip_employer)
  {
  m_employers.erase(ip_employer);
  }

void IEconomyManager::RegisterEmployee(Slavs::TGoldKeeper ip_employee)
{
  m_employees.insert(ip_employee);
}

void IEconomyManager::RemoveEmployee(Slavs::TGoldKeeper ip_employee)
{
  m_employees.erase(ip_employee);
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

void IEconomyManager::GetEmployers(Slavs::Employers& o_employers)
  {
  o_employers.insert(m_employers.begin(), m_employers.end());
  }

void IEconomyManager::GetAvailableEmployers(Slavs::TEmployersInformation& o_available) const
  {
  std::for_each(m_employers.begin(), m_employers.end(), [&o_available](Slavs::EmployerPtr i_employer)
    {
    const EmployerInformation& information = i_employer->GetInformation();
    if (information.IsActive())
      o_available.push_back(&information);
    });
  }