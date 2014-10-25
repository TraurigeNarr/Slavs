#include "Management\Goverment.h"

#include "IController.h"

#include "Management\IEconomyManager.h"
#include "Management\IMilitaryManager.h"
#include "Management\IRelashionshipsManager.h"
#include "Management\ISocietyManager.h"
#include "Management\ITechnologyManager.h"

using namespace Slavs;

Goverment::Goverment(IController* ip_owner, 
                     std::unique_ptr<IEconomyManager>&& ip_economy, 
                     std::unique_ptr<IMilitaryManager>&& ip_military, 
                     std::unique_ptr<IRelashionshipsManager>&& ip_relashionships, 
                     std::unique_ptr<ISocietyManager>&& ip_society, 
                     std::unique_ptr<ITechnologyManager>&& ip_technologies)
  : mp_owner(ip_owner)
  , m_economy_manager(std::move(ip_economy))
  , m_military(std::move(ip_military))
  , m_relashionships(std::move(ip_relashionships))
  , m_society(std::move(ip_society))
  , m_technologies(std::move(ip_technologies))
  {
  m_economy_manager->SetGoverment(this);
  m_economy_manager->Initialize();

  m_society->SetGoverment(this);
  }

Goverment::~Goverment()
{
}

void Goverment::Initialize()
  {

  }

TEconomy Goverment::GetEconomyManager() const
{
  return m_economy_manager.get();
}

TMilitary Goverment::GetMilitaryManager() const
{
  return m_military.get();
}

TRelashionships Goverment::GetRelationshipManager() const
{
  return m_relashionships.get();
}

TSociety Goverment::GetSocietyManager() const
{
  return m_society.get();
}

TTechnologies Goverment::GetTechnologyManager() const
{
  return m_technologies.get();
}

IController* Goverment::GetOwner() const
  {
  return mp_owner;
  }