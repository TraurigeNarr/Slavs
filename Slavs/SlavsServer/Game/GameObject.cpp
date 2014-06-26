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
    mp_owner = ip_controller;
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

  //////////////////////////////////////////////////////////////////////////
  // IGameObject
  void GameObject::Init()
    {

    }

  GameObjectState* GameObject::GetState() const
    {
    GameObjectState* p_state = IGameObject::GetState();
    p_state->iFlags |= GOF_Position;
    p_state->pPosition = new Vector2D(m_position);
    return p_state;
    }

  void GameObject::TickPerformed()
    {
    TickForComponents();
    }

  //////////////////////////////////////////////////////////////////////////
  // IMovable

  void GameObject::SetPosition(const Vector2D& i_position)
    {
    m_position = i_position;
    }

  Vector2D GameObject::GetPosition() const
    {
    return m_position;
    }

  Vector2D& GameObject::AccessPosition()
    {
    return m_position;
    }
  }

