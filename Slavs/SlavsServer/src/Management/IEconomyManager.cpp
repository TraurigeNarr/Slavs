#include "Management/IEconomyManager.h"

#include "IController.h"
#include "Management/Goverment.h"

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