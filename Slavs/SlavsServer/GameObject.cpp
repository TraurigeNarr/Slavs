#include "GameObject.h"

#include "GameContext.h"
#include "IController.h"

#include <Utilities/TemplateFunctions.h>

//////////////////////////////////////////////////////////////////////////

namespace Slavs
  {

  GameObject::GameObject(GameContext& i_context, long i_id, int i_type, int i_selection_mask)
    : IGameObject(i_id, i_type, i_selection_mask)
    , m_context(i_context)
    , mp_owner(nullptr)
    {

    }
  
  GameObject::~GameObject()
    {
    ClearVector(m_components);
    }

  void GameObject::SetOwner(IController* ip_controller)
    {

    }

  IController* GameObject::GetController()
    {
    return mp_owner;
    }

  bool GameObject::HasOwner() const
    {
    return mp_owner == nullptr;
    }

 /* bool GameObject::ProcessCommand(const CommandData& cData)
    {
    return false;
    }

  bool HandleMessage (const Telegram& msg)
    {

    }*/

  void GameObject::AddComponent (IComponent* ip_component)
    {

    }
  
  bool GameObject::ProbeComponents ()
    {
    return true;
    }

  void GameObject::Init()
    {

    }

  void GameObject::TickPerformed()
    {
    TickForComponents();
    }

  }

