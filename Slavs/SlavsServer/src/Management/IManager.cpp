#include "Management\IManager.h"

#include "Management\Goverment.h"

IManager::IManager()
  : mp_goverment(nullptr)
  , m_has_changes(true)
  {  }

IManager::~IManager()
  {

  }

void IManager::SetGoverment(Slavs::TGoverment ip_goverment)
  {
  mp_goverment = ip_goverment;
  }

Slavs::TGoverment IManager::GetGoverment() const
  {
  return mp_goverment;
  }

bool IManager::HasChanges() const
  {
  return m_has_changes;
  }

void IManager::Invalidate()
  {
  m_has_changes = true;
  }

void IManager::ValidateChanges()
  {
  m_has_changes = false;
  }

void IManager::_Invalidate()
  {
  m_has_changes = true;
  }