#include "IController.h"

#include "SGameContext.h"

#include "Game/GameContext.h"
#include "Game/GameObject.h"

#include "Management/Goverment.h"
#include "Management/EconomyManagerBase.h"
#include "Management/SocietyManagerBase.h"
#include "Management/IStoreSystem.h"

#include <Net.h>

#include <cassert>

IController::IController(int iMask, Slavs::GameContext& i_context)
	: m_game_context(i_context)
  , m_iMask(iMask)
{
	m_pResourceManager = new SResourceManager();
  mp_goverment.reset(new Goverment(this, std::make_shared<EconomyManagerBase>(), nullptr, nullptr, std::make_shared<SocietyManagerBase>(), nullptr));
}

IController::~IController()
{}

void IController::ControllerStaff()
{
}

void IController::SetContext(SGameContext *context)
{
	if(NULL != context)
		m_pContext = context;
}

/*
------------------------------------------------------------------------------
							Serialize/Deserialize
------------------------------------------------------------------------------
*/

int IController::Serialize(char* buf_end, int size) const
{
	if(m_iNeededSize > size)
	{
		assert("<SResourceManager::Serialize>:Not enough size.");
		return 0;
	}

	return m_pResourceManager->Serialize(buf_end, size);
}

int IController::NeededSize() const
{
	m_iNeededSize = m_pResourceManager->NeededSize();
	return m_iNeededSize;
}

//////////////////////////////////////////////////////////////////////////

bool IController::CheckResources(int i_object_type)
  {
  const Slavs::ResourcesCountSet& needed_resources = m_game_context.GetResourcesFor(i_object_type, m_iMask);

  Slavs::TStoreSystem p_store_system = mp_goverment->GetEconomyManager()->GetStoreSystem();

  Slavs::ResourcesCountSet::const_iterator it_cur = needed_resources.begin();
  Slavs::ResourcesCountSet::const_iterator it_end = needed_resources.end();

  for ( ; it_cur != it_end; ++it_cur)
    {
    if ( !p_store_system->PeekResources(static_cast<GameResourceType>(it_cur->first), it_cur->second) )
      return false;
    }

  return true;
  }

bool IController::CreateObject(int i_object_type)
  {
  const Slavs::ResourcesCountSet& needed_resources = m_game_context.GetResourcesFor(i_object_type, m_iMask);

  Slavs::TStoreSystem p_store_system = mp_goverment->GetEconomyManager()->GetStoreSystem();

  Slavs::ResourcesCountSet peeked_resources;

  Slavs::ResourcesCountSet::const_iterator it_cur = needed_resources.begin();
  Slavs::ResourcesCountSet::const_iterator it_end = needed_resources.end();

  bool enough_resources = true;

  for ( ; it_cur != it_end; ++it_cur)
    {
    size_t peeked_resource = p_store_system->GetResources(static_cast<GameResourceType>(it_cur->first), it_cur->second);
    if (peeked_resource == 0)
      {
      enough_resources = false;
      break;
      }
    peeked_resources.push_back( Slavs::ResourcePair(it_cur->first, it_cur->second) );
    }

  p_store_system->Add(peeked_resources);
  return false;
  }