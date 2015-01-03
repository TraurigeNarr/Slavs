#include "stdafx.h"

#include "HumanComponent.h"

#include "BaseObjectComposer.h"
#include "TypeNames.h"
#include "TypeEnumerations.h"
#include "Profession.h"

#include <Utilities/XmlUtilities.h>
#include <Game/GameObjectState.h>

#include <iostream>

namespace BasePlugin
  {
  //////////////////////////////////////////////////////////////////////////
  // Human Component Serializer 
  HumanComponentSerializer::HumanComponentSerializer(int i_component_global_id, const BaseObjectComposer& i_composer)
    : IComponentSerializer(i_component_global_id, i_composer)
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
    HumanComponent* p_human = new HumanComponent(ip_object, m_component_global_id, m_object_composer);
    ApplyTo(*p_human);
    return p_human;
    }

  //////////////////////////////////////////////////////////////////////////
  // Human Component

  HumanComponent::HumanComponent(Slavs::TGameObject ih_owner, int i_component_id, const BaseObjectComposer& i_composer)
    : IHuman(ih_owner, i_component_id)
    , m_needed_calories(0)
    , m_object_composer(i_composer)
    , m_has_work(false)
    , mp_home(nullptr)
		, mp_current_profession(nullptr)
		, m_state_machine(this)
    {

    }

  HumanComponent::~HumanComponent()
    {

    }

  void HumanComponent::TickPerformed()
    {
		m_state_machine.Update(10);
    }

  bool HumanComponent::HandleMessage(const Telegram& msg)
    {
    if (msg.Msg == 1) // has work
      {
      std::cout << "[" << this->GetOwner()->GetID() << "] has a work. Temporary solution!!!" << std::endl;
      m_has_work = true;
      return true;
      }
    else if (msg.Msg == 2) // fired
      {
      std::cout << "[" << this->GetOwner()->GetID() << "] fired. Temporary solution!!!" << std::endl;
      m_has_work = false;
      return true;
      }
    return false;
    }

  void HumanComponent::GetState(GameObjectState& i_state) const
    {

    }

  bool HumanComponent::Probe()
    {
    return static_cast<Slavs::GameObject*>(mp_owner)->HasComponent(m_object_composer.GetComponentGlobalID(ComponentType::CT_HUMAN))
        && static_cast<Slavs::GameObject*>(mp_owner)->HasComponent(m_object_composer.GetComponentGlobalID(ComponentType::CT_DYNAMIC_COMPONENT));
    }

  //////////////////////////////////////////////////////////////////////////
  // IHuman

  void HumanComponent::SetHome(Slavs::HousePtr ip_home)
    {
    mp_home = ip_home;
    }

  Slavs::HousePtr HumanComponent::GetHome() const
    {
    return mp_home;
    }

  bool HumanComponent::HasWork() const
    {
    return m_has_work;
    }

  } // BasePlugin