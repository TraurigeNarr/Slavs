#include "GameContext.h"

#include "IController.h"
#include "GameObject.h"

#include "ServerMain.h"
#include "PluginSystem\MetaFactory.h"
#include "PluginSystem\IObjectComposer.h"

#include <Common//Math/Vector2D.h>

//////////////////////////////////////////////////////////////////////////

namespace Slavs
  {
  GameContext::GameContext()
    : IGameContext("null_map")
    , m_next_object_id(0)
    {

    }

  GameContext::~GameContext()
    {

    }

  void GameContext::TickPerformed()
    {
    // clear dead pool
    std::for_each(m_dead_pool.begin(), m_dead_pool.end(), [this](GameObject* p_object)
      {
      if (1 != m_mGameObjects.erase(p_object->GetID()))
        assert ("One object should be erased.");
      delete p_object;
      });
    m_dead_pool.clear();

    // update objects
    std::for_each(m_mGameObjects.begin(), m_mGameObjects.end(), [this](std::pair<long, GameObject*> p)
      {
      p.second->TickPerformed();
      if(p.second->Destroyed())
        m_dead_pool.insert(p.second);
      });
    }

  TGameObject GameContext::AddObject(int i_type, const Vector2D& i_position, IController* ip_owner /* = nullptr */, int i_selection_mask /* = 0 */)
    {
    MetaFactory& meta_factory = ServerMain::GetInstance().GetMetaFactory();
    if (meta_factory.SupportObject(i_type))
      {
      GameObject* p_new_object = new GameObject(*this, m_next_object_id, i_type, i_selection_mask);
      meta_factory.ComposeObject(p_new_object);
      p_new_object->SetOwner(ip_owner);
      m_mGameObjects[m_next_object_id] = p_new_object;
      ++m_next_object_id;
      return p_new_object;
      }

    assert ("Normally we should not get here.");
    return nullptr;
    }

  void GameContext::RemoveObject(TGameObject ih_object)
    {
    m_dead_pool.insert(ih_object);
    }
  }