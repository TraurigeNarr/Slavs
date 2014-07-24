#include "stdafx.h"

#include "DynamicObjectComponent.h"

#include "BaseObjectComposer.h"
#include "TypeNames.h"
#include "TypeEnumerations.h"
#include "IMovementStrategy.h"

#include <Utilities/XmlUtilities.h>
#include <Game/GameObjectState.h>

//////////////////////////////////////////////////////////////////////////

namespace BasePlugin
  {
  //////////////////////////////////////////////////////////////////////////
  // Dynamic Object Component Serializer

  DynamicObjectComponentSerializer::DynamicObjectComponentSerializer(int i_component_global_id, const BaseObjectComposer& i_composer)
    : IComponentSerializer(i_component_global_id, i_composer)
    {    }

  DynamicObjectComponentSerializer::~DynamicObjectComponentSerializer()
    {    }

  void DynamicObjectComponentSerializer::Parse(const TiXmlElement& i_configuration_node)
    {
#ifdef _DEBUG
    std::string elementName = "";
    elementName = XmlUtilities::GetStringAttribute(&i_configuration_node, "type", "");
    assert (elementName == Component_Dynamic);
#endif
    m_bounding_box_radius = XmlUtilities::GetRealAttribute(&i_configuration_node, "bbox_radius", 10.f);
    m_maximum_force       = XmlUtilities::GetRealAttribute(&i_configuration_node, "max_force", 1.f);
    m_maximum_speed       = XmlUtilities::GetRealAttribute(&i_configuration_node, "max_speed", 1.f);
    m_maximum_turnrate    = XmlUtilities::GetRealAttribute(&i_configuration_node, "max_turnrate", 1.f);
    }

  void DynamicObjectComponentSerializer::ApplyTo(IComponent& i_component) const
    {
    assert (typeid(DynamicObjectComponent) == typeid(i_component));
    DynamicObjectComponent& dynamic_object = static_cast<DynamicObjectComponent&>(i_component);
    
    dynamic_object.m_bounding_box_radius    = m_bounding_box_radius;
    dynamic_object.m_maximum_force          = m_maximum_force;
    dynamic_object.m_maximum_speed          = m_maximum_speed;
    dynamic_object.m_maximum_turnrate       = m_maximum_turnrate;
    }

  IComponent* DynamicObjectComponentSerializer::CreateComponent(Slavs::GameObject* ip_object) const
    {
    DynamicObjectComponent* p_dynamic = new DynamicObjectComponent(ip_object, m_component_global_id, m_object_composer);
    ApplyTo(*p_dynamic);
    return p_dynamic;
    }


  //////////////////////////////////////////////////////////////////////////
  // Dynamic Object Component

  DynamicObjectComponent::DynamicObjectComponent(Slavs::TGameObject ih_owner, int i_component_id, const BaseObjectComposer& i_composer)
    : IComponent(ih_owner, i_component_id)
    , m_bounding_box_radius(.0f)
    , mp_movement_strategy(nullptr)
    , m_object_composer(i_composer)
    {

    }

  DynamicObjectComponent::~DynamicObjectComponent()
    {

    }

  void DynamicObjectComponent::TickPerformed()
    {
    if (mp_movement_strategy == nullptr)
      return;
    
    m_velocity = mp_movement_strategy->GetSteering();
    m_velocity.Truncate(m_maximum_speed);

    mp_owner->AccessPosition() += m_velocity;

    mp_owner->StateChanged();
    }

  bool DynamicObjectComponent::HandleMessage(const Telegram& msg)
    {
    return false;
    }

  void DynamicObjectComponent::GetState(GameObjectState& i_state) const
    {
    
    }

  bool DynamicObjectComponent::Probe()
    {
    return static_cast<Slavs::GameObject*>(mp_owner)->HasComponent(m_object_composer.GetComponentGlobalID(ComponentType::CT_DYNAMIC_COMPONENT));
    }

  void DynamicObjectComponent::SetMovementStrategy(IMovementStrategy* ip_movement_strategy)
    {
    mp_movement_strategy = ip_movement_strategy;
    }

  }