#include "stdafx.h"

#include "HumanComponent.h"

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

  void HumanComponentSerializer::ApplyTo(IComponent& i_component)
    {
    assert (typeid(HumanComponent) == typeid(i_component));
    HumanComponent& human = static_cast<HumanComponent&>(i_component);
    human.m_needed_calories = m_needed_calories;
    human.m_eat_in          = m_eat_in;
    human.m_die_if_no_eat   = m_die_if_no_eat;
    }

  //////////////////////////////////////////////////////////////////////////
  // Human Component

  std::string HumanComponent::ComponentName = "Human";

  HumanComponent::HumanComponent()
    : m_needed_calories(0)
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
    return true;
    }

  void HumanComponent::GetState(GameObjectState& i_state) const
    {

    }

  bool HumanComponent::Probe()
    {
    return true;
    }

  } // BasePlugin