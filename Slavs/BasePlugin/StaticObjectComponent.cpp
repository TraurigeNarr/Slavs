#include "stdafx.h"

#include "StaticObjectComponent.h"
#include "TypeNames.h"

#include <Utilities/XmlUtilities.h>

#include <typeinfo>


namespace BasePlugin
  {
  //////////////////////////////////////////////////////////////////////////
  // Static Component Serializer 
  StaticObjectComponentSerializer::StaticObjectComponentSerializer(int i_component_id)
    : IComponentSerializer(i_component_id)
    {

    }

  StaticObjectComponentSerializer::~StaticObjectComponentSerializer()
    {

    }

  void StaticObjectComponentSerializer::Parse(const TiXmlElement& i_configuration_node)
    {
#ifdef _DEBUG
    std::string elementName = "";
    elementName = XmlUtilities::GetStringAttribute(&i_configuration_node, "type", "");
    assert (elementName == Component_Static);
#endif
    m_bounding_box_radius = XmlUtilities::GetRealAttribute(&i_configuration_node, "bbox_radius", 10.f);
    }

  void StaticObjectComponentSerializer::ApplyTo(IComponent& i_component) const
    {
    assert (typeid(StaticObjectComponent) == typeid(i_component));
    StaticObjectComponent& static_object = static_cast<StaticObjectComponent&>(i_component);
    static_object.m_bounding_box_radius = m_bounding_box_radius;
    }

  IComponent* StaticObjectComponentSerializer::CreateComponent(Slavs::GameObject* ip_object) const
    {
    StaticObjectComponent* p_static = new StaticObjectComponent(ip_object, m_component_global_id);
    ApplyTo(*p_static);
    return p_static;
    }


  //////////////////////////////////////////////////////////////////////////
  // Static Component

  StaticObjectComponent::StaticObjectComponent(Slavs::TGameObject ih_owner, int i_component_id)
    : IComponent(ih_owner, i_component_id)
    , m_bounding_box_radius(.0f)
    , m_position(0.f, 0.f)
    {

    }

  StaticObjectComponent::~StaticObjectComponent()
    {

    }

  void StaticObjectComponent::TickPerformed()
    {

    }

  bool StaticObjectComponent::HandleMessage(const Telegram& msg)
    {
    return false;
    }

  void StaticObjectComponent::GetState(GameObjectState& i_state) const
    {

    }

  bool StaticObjectComponent::Probe()
    {
    return true;
    }
  }