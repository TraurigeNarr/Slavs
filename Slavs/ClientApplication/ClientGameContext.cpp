#include "stdafx.h"

#include "ClientGameContext.h"
#include "ClientGameObject.h"

#include <GameCore/IController.h>

#include <Common/Game/GameObjectState.h>

//////////////////////////////////////////////////////////////////////////

namespace
  {

  struct ControllersInformation_SearchByMask
    {
    int m_mask;
    ControllersInformation_SearchByMask(int i_mask_searched)
      : m_mask(i_mask_searched)
      {}
    bool operator () (const GameContext::ControllerInformation& i_information)
      {
      return i_information.mp_controller->GetMask() == m_mask;
      }
    };

  } // namespace

//////////////////////////////////////////////////////////////////////////

ClientGameContext::ClientGameContext(const std::string& i_context_name)
  : GameContext(i_context_name)
  {  }

ClientGameContext::~ClientGameContext()
  {  }

void ClientGameContext::ReleaseContext()
  {

  }

void ClientGameContext::_ApplyState(GameObjectState& i_state, GameObjectUniquePtr& ip_object)
  {
  if(0 != (i_state.iFlags & GOF_Destroyed))
    {
    RemoveObject(ip_object.get());
    return;
    }
  static_cast<ClientGameObject*>(ip_object.get())->ApplyState(i_state);
  }

void ClientGameContext::_AddObject(GameObjectState& i_state)
  {
  long object_id = m_next_object_id++;
  std::unique_ptr<GameObject> p_game_object(new ClientGameObject(object_id, static_cast<int>(i_state.oType), *this, nullptr));

  IController* p_owner = nullptr;
  if (i_state.iOwnerMask != 0)
    {
    std::vector<GameContext::ControllerInformation>::iterator it = std::find_if(m_controllers.begin(), m_controllers.end(), ControllersInformation_SearchByMask(i_state.iOwnerMask));
    if (it != m_controllers.end())
      p_owner = (*it).mp_controller.get();
    }
  p_game_object->SetOwner(p_owner);

  // init object

  auto it = m_objects.insert(std::make_pair(object_id, std::move(p_game_object))).first;
  _ApplyState(i_state, (*it).second);
  }

void ClientGameContext::ApplyState(GameObjectState& i_state)
  {
  auto object_it = m_objects.find(i_state.lID);

  if ( object_it != m_objects.end() )
    {
    _ApplyState(i_state, (*object_it).second);
    }
  else
    {
    _AddObject(i_state);
    }
  }

void ClientGameContext::AddDefinition(const std::pair<std::string, int>&& i_definition)
  {
  m_definitions_map.insert(i_definition);
  }

void ClientGameContext::Initialize()
  {

  }