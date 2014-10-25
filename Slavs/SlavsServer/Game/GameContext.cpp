#include "GameContext.h"

#include "IController.h"
#include "GameObject.h"

#include "Management/GlobalEconomics.h"

#include "Main/ServerMain.h"
#include "PluginSystem\MetaFactory.h"
#include "PluginSystem\IObjectComposer.h"

#include <Common/Math/Vector2D.h>

#include <algorithm>

//////////////////////////////////////////////////////////////////////////

namespace
  {

  struct ControllersInformation_SearchByMask
    {
    int m_mask;
    ControllersInformation_SearchByMask(int i_mask_searched)
      : m_mask(i_mask_searched)
      {}
    bool operator () (const Slavs::GameContext::ControllerInformation& i_information)
      {
      return i_information.mp_controller->GetMask() == m_mask;
      }
    };

  } // namespace

//////////////////////////////////////////////////////////////////////////

namespace Slavs
  {
  //////////////////////////////////////////////////////////////////////////
  // GameContext::ControllerInformation

  GameContext::ControllerInformation::ControllerInformation()
    : mp_controller(nullptr)
    {

    }

  GameContext::ControllerInformation::ControllerInformation(ControllerInformation&& i_information)
    : mp_controller(std::move(i_information.mp_controller))
    , m_needed_resources(std::move(i_information.m_needed_resources))
    {

    }

  bool GameContext::ControllerInformation::operator == (const ControllerInformation& i_other)
    {
    return mp_controller->GetMask() == i_other.mp_controller->GetMask();
    }

  //////////////////////////////////////////////////////////////////////////
  // GameContext
  GameContext::GameContext()
    : IGameContext("null_map")
    , m_next_object_id(0)
    , mp_global_economics(new GlobalEconomics())
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

  void GameContext::ReleaseContext()
    {
    ReleaseGameObjects();
    m_controllers.clear();
    mp_global_economics.reset();
    }

  TGameObject GameContext::AddObject(int i_type, const Vector2D& i_position, int i_controllers_mask /* = 0 */, int i_selection_mask /* = 0 */)
    {
    MetaFactory& meta_factory = ServerMain::GetInstance().GetMetaFactory();
    if (meta_factory.SupportObject(i_type))
      {
      GameObject* p_new_object = new GameObject(*this, m_next_object_id, i_type, i_selection_mask);
      
      IController* p_owner = nullptr;
      if (i_controllers_mask != 0)
        {
        std::vector<GameContext::ControllerInformation>::iterator it = std::find_if(m_controllers.begin(), m_controllers.end(), ControllersInformation_SearchByMask(i_controllers_mask));
        if (it != m_controllers.end())
          p_owner = (*it).mp_controller.get();
        }
      p_new_object->SetOwner(p_owner);

      meta_factory.ComposeObject(p_new_object);

      if (p_new_object->ProbeComponents())
        {
        p_new_object->SetPosition(i_position);
        m_mGameObjects[m_next_object_id] = p_new_object;
        ++m_next_object_id;
        return p_new_object;
        }
      else
        {
        delete p_new_object;
        return nullptr;
        }
      }

    assert ("Normally we should not get here.");
    return nullptr;
    }

  void GameContext::RemoveObject(TGameObject ih_object)
    {
    m_dead_pool.insert(ih_object);
    }

  void GameContext::RegisterController(std::unique_ptr<IController> ip_controller)
    {
    ControllerInformation controller_info;
    controller_info.mp_controller = std::move(ip_controller);
    m_controllers.push_back(std::move(controller_info));
    }

  void GameContext::RegisterResources (int i_type, const ResourcesCountSet& i_resources)
    {
    std::for_each(m_controllers.begin(), m_controllers.end(), [this, i_type, &i_resources](ControllerInformation& i_information)
      {
      std::copy(i_resources.begin(), i_resources.end(), i_information.m_needed_resources[i_type].begin());
      });
    }

  const ResourcesCountSet& GameContext::GetResourcesFor (int i_type, int i_controller_mask)
    {
    std::vector<GameContext::ControllerInformation>::iterator it = std::find_if(m_controllers.begin(), m_controllers.end(), ControllersInformation_SearchByMask(i_controller_mask));
#ifdef _DEBUG
    if (it == m_controllers.end())
      {
      assert (false && "Who asked for this");
      return ResourcesCountSet();
      }
      
    if ((*it).m_needed_resources.find(i_type) == (*it).m_needed_resources.end())
      {
      assert (false && "Type is not registered");
      return ResourcesCountSet();
      }

#endif

    return (*it).m_needed_resources[i_type];
    }

  GlobalEconomics& GameContext::GetGlobalEconomics() const
    {
    return *mp_global_economics;
    }

  }