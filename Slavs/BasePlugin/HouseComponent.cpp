#include "stdafx.h"

#include "HouseComponent.h"
#include "TypeNames.h"

#include <Utilities/XmlUtilities.h>
#include <Game/GameObjectState.h>

#include <SlavsServer/include/IController.h>
#include <SlavsServer/include/Management/Goverment.h>
#include <SlavsServer/include/Management/ISocietyManager.h>

#include <typeinfo>


namespace BasePlugin
  {
  //////////////////////////////////////////////////////////////////////////
  // Human Component Serializer 
  HouseComponentSerializer::HouseComponentSerializer(int i_component_id)
    : IComponentSerializer(i_component_id)
    {

    }

  HouseComponentSerializer::~HouseComponentSerializer()
    {

    }

  void HouseComponentSerializer::Parse(const TiXmlElement& i_configuration_node)
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

  void HouseComponentSerializer::ApplyTo(IComponent& i_component) const 
    {
   
    }

  IComponent* HouseComponentSerializer::CreateComponent(Slavs::GameObject* ip_object) const
    {
    HouseComponent* p_house = new HouseComponent(ip_object, m_component_global_id);
    ApplyTo(*p_house);
    return p_house;
    }

  //////////////////////////////////////////////////////////////////////////
  // Human Component

  HouseComponent::HouseComponent(Slavs::TGameObject ih_owner, int i_component_id)
    : IComponent(ih_owner, i_component_id)
    , m_needed_calories(0)
    {
//    ih_owner->GetController()->GetGoverment().GetSocietyManager()->RegisterHouse(this);
    }

  HouseComponent::~HouseComponent()
    {

    }

  void HouseComponent::TickPerformed()
    {

    }

  bool HouseComponent::HandleMessage(const Telegram& msg)
    {
    return false;
    }

  void HouseComponent::GetState(GameObjectState& i_state) const
    {

    }

  bool HouseComponent::Probe()
    {
    return static_cast<Slavs::GameObject*>(mp_owner)->HasComponent(m_component_id);
    }

  } // BasePlugin