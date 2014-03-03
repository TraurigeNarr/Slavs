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

  bool GameObject::HasComponent(int i_id) const
    {
    return std::find(m_components_ids.begin(), m_components_ids.end(), i_id) != m_components_ids.end();
    }

 /* bool GameObject::ProcessCommand(const CommandData& cData)
    {
    return false;
    }

  bool HandleMessage (const Telegram& msg)
    {

    }*/

  void GameObject::AddComponent (std::unique_ptr<IComponent> ih_component)
    {
    m_components_ids.push_back(ih_component->GetComponentID());
    m_components.push_back(ih_component.release());
    }
  
  bool GameObject::ProbeComponents ()
    {
    for (IComponent* ip_component : m_components)
      ip_component->Probe();
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

