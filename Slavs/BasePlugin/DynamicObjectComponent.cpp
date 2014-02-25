#include "stdafx.h"

#include "DynamicObjectComponent.h"
#include "TypeNames.h"
#include "IMovementStrategy.h"

#include <Utilities/XmlUtilities.h>

#include <typeinfo>

namespace BasePlugin
  {
  //////////////////////////////////////////////////////////////////////////
  // Dynamic Object Component Serializer

  DynamicObjectComponentSerializer::DynamicObjectComponentSerializer()
    {    }

  DynamicObjectComponentSerializer::~DynamicObjectComponentSerializer()
    {    }

  void DynamicObjectComponentSerializer::Parse(const TiXmlElement& i_configuration_node)
    {
#ifdef _DEBUG
    std::string elementName = "";
    elementName = i_configuration_node.Value();
    assert (elementName == Component_Dynamic);
#endif
    m_bounding_box_radius = XmlUtilities::GetRealAttribute(&i_configuration_node, "bbox_radius", 10.f);
    m_maximum_force       = XmlUtilities::GetRealAttribute(&i_configuration_node, "max_force", 1.f);
    m_maximum_speed       = XmlUtilities::GetRealAttribute(&i_configuration_node, "max_speed", 1.f);
    m_maximum_turnrate    = XmlUtilities::GetRealAttribute(&i_configuration_node, "max_turnrate", 1.f);
    }

  void DynamicObjectComponentSerializer::ApplyTo(IComponent& i_component)
    {
    assert (typeid(DynamicObjectComponent) == typeid(i_component));
    DynamicObjectComponent& dynamic_object = static_cast<DynamicObjectComponent&>(i_component);
    
    dynamic_object.m_bounding_box_radius    = m_bounding_box_radius;
    dynamic_object.m_maximum_force          = m_maximum_force;
    dynamic_object.m_maximum_speed          = m_maximum_speed;
    dynamic_object.m_maximum_turnrate       = m_maximum_turnrate;
    }

  //////////////////////////////////////////////////////////////////////////
  // Dynamic Object Component

  DynamicObjectComponent::DynamicObjectComponent(Slavs::TGameObject ih_owner)
    : IComponent(ih_owner)
    , m_bounding_box_radius(.0f)
    , m_position(0.f, 0.f)
    , mp_movement_strategy(nullptr)
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

    m_position += m_velocity;

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
    return true;
    }

  void DynamicObjectComponent::SetMovementStrategy(IMovementStrategy* ip_movement_strategy)
    {
    mp_movement_strategy = ip_movement_strategy;
    }

  }