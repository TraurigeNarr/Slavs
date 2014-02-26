#include "stdafx.h"

#include "HumanComponent.h"
#include "TypeNames.h"

#include <Utilities/XmlUtilities.h>

#include <typeinfo>


namespace BasePlugin
  {
  //////////////////////////////////////////////////////////////////////////
  // Human Component Serializer 
  HumanComponentSerializer::HumanComponentSerializer()
    {

    }

  HumanComponentSerializer::~HumanComponentSerializer()
    {

    }

  void HumanComponentSerializer::Parse(const TiXmlElement& i_configuration_node)
    {
    std::string elementName = "";

#ifdef _DEBUG
    elementName = XmlUtilities::GetStringAttribute(&i_configuration_node, "type", "");
    assert (elementName == Component_Human);
    elementName = "";
#endif

    const TiXmlElement* childElement = 0;
    const TiXmlElement*	foodElement = 0;

    while (childElement = XmlUtilities::IterateChildElements(&i_configuration_node, childElement))
      {
      elementName = childElement->Value();
      // food preferences
      if("Food" == elementName)
        {
        m_needed_calories = XmlUtilities::GetIntAttribute(childElement, "Calories",   10);
        m_eat_in          = XmlUtilities::GetIntAttribute(childElement, "EatIn",      500);
        m_die_if_no_eat   = XmlUtilities::GetIntAttribute(childElement, "DieIfNoEat", 5000);
        }
      }
    }

  void HumanComponentSerializer::ApplyTo(IComponent& i_component) const 
    {
    assert (typeid(HumanComponent) == typeid(i_component));
    HumanComponent& human = static_cast<HumanComponent&>(i_component);
    human.m_needed_calories = m_needed_calories;
    human.m_eat_in          = m_eat_in;
    human.m_die_if_no_eat   = m_die_if_no_eat;
    }

  IComponent* HumanComponentSerializer::CreateComponent(Slavs::GameObject* ip_object) const
    {
    HumanComponent* p_human = new HumanComponent(ip_object);
    ApplyTo(*p_human);
    return p_human;
    }

  //////////////////////////////////////////////////////////////////////////
  // Human Component

  HumanComponent::HumanComponent(Slavs::TGameObject ih_owner)
    : IComponent(ih_owner)
    , m_needed_calories(0)
    {

    }

  HumanComponent::~HumanComponent()
    {

    }

  void HumanComponent::TickPerformed()
    {

    }

  bool HumanComponent::HandleMessage(const Telegram& msg)
    {
    return false;
    }

  void HumanComponent::GetState(GameObjectState& i_state) const
    {

    }

  bool HumanComponent::Probe()
    {
    return true;
    }

  } // BasePlugin