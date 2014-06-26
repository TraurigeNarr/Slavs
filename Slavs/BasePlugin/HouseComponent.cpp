#include "stdafx.h"

#include "HouseComponent.h"
#include "TypeNames.h"

#include <Utilities/XmlUtilities.h>
#include <Game/GameObjectState.h>

#include <SlavsServer/include/IController.h>
#include <SlavsServer/include/Management/Goverment.h>
#include <SlavsServer/include/Management/ISocietyManager.h>


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
      m_max_population = XmlUtilities::GetIntAttribute(childElement, "max_population", 8);
      }
    }

  void HouseComponentSerializer::ApplyTo(IComponent& i_component) const 
    {
    assert (typeid(HouseComponent) == typeid(i_component));
    HouseComponent& house = static_cast<HouseComponent&>(i_component);
    house.m_max_population = m_max_population;
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
    , m_max_population(0)
    {
    ih_owner->GetController()->GetGoverment().GetSocietyManager()->RegisterHouse(this);
    }

  HouseComponent::~HouseComponent()
    {

    }

//////////////////////////////////////////////////////////////////////////
// IComponent

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

//////////////////////////////////////////////////////////////////////////
// IHouse

  size_t HouseComponent::GetPopulation() const
    {
    return 0;
    }
  
  size_t HouseComponent::GetUnemployedPopulation() const
    {
    return 0;
    }

  void HouseComponent::GetUnemployedPopulation(Slavs::Humans& o_unemployed) const
    {

    }
  
  size_t HouseComponent::GetFreePlaces() const
    {
    return 0;
    }
  
  bool HouseComponent::AddInhabitant(Slavs::HumanPtr ip_inhabitant)
    {
    return false;
    }

  void HouseComponent::RemoveInhabitant(Slavs::HumanPtr ip_inhabitant)
    {

    }

  void HouseComponent::HumanStateChanged()
    {

    }

  } // BasePlugin