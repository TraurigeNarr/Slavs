#include "Management\Goverment.h"

#include "IController.h"

#include "Management\IEconomyManager.h"
#include "Management\IMilitaryManager.h"
#include "Management\IRelashionshipsManager.h"
#include "Management\ISocietyManager.h"
#include "Management\ITechnologyManager.h"

using namespace Slavs;

Goverment::Goverment(IController* ip_owner, TEconomy i_economy, TMilitary i_military, TRelashionships i_relashionships, TSociety i_society, TTechnologies i_technologies)
  : mp_owner(ip_owner)
  , m_economy_manager(i_economy)
  , m_military(i_military)
  , m_relashionships(i_relashionships)
  , m_society(i_society)
  , m_technologies(i_technologies)
{

}

Goverment::~Goverment()
{
}

TEconomy        Goverment::GetEconomyManager()
{
  return m_economy_manager;
}

TMilitary       Goverment::GetMilitaryManager()
{
  return m_military;
}

TRelashionships   Goverment::GetRelationshipManager()
{
  return m_relashionships;
}

TSociety        Goverment::GetSocietyManager()
{
  return m_society;
}

TTechnologies Goverment::GetTechnologyManager()
{
  return m_technologies;
}