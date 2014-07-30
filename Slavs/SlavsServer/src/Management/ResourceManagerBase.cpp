#include "Management/ResourceManagerBase.h"

#include "Management/IEconomyManager.h"

//////////////////////////////////////////////////////////////////////////

ResourceManagerBase::ResourceManagerBase(Slavs::TEconomy ip_economy)
  : mp_economy(ip_economy)
  {  }

ResourceManagerBase::~ResourceManagerBase()
  {  }

bool ResourceManagerBase::AddResource(GameResourceBox& io_resource_box)
  {
  return false;
  }

bool ResourceManagerBase::AddResource(int i_type, size_t i_number)
  {
  return false;
  }

size_t ResourceManagerBase::PeekResource(int i_type, size_t i_number) const
  {
  return 0;
  }

size_t ResourceManagerBase::GetResource(int i_type, size_t i_number)
  {
  return 0;
  }

size_t ResourceManagerBase::GetResourceForcely(int i_type, size_t i_number)
  {
  return 0;
  }