#include "stdafx.h"

#include "HouseComponent.h"

#include "HumanComponent.h"

#include "BaseObjectComposer.h"
#include "TypeNames.h"
#include "TypeEnumerations.h"

#include <Utilities/XmlUtilities.h>
#include <Game/GameObjectState.h>

#include <SlavsServer/Game/GameContext.h>
#include <SlavsServer/PluginSystem/IHuman.h>
#include <SlavsServer/include/IController.h>
#include <SlavsServer/include/Management/Goverment.h>
#include <SlavsServer/include/Management/ISocietyManager.h>


namespace BasePlugin
  {
  //////////////////////////////////////////////////////////////////////////
  // Human Component Serializer 
  HouseComponentSerializer::HouseComponentSerializer(int i_component_global_id, const BaseObjectComposer& i_composer)
    : IComponentSerializer(i_component_global_id, i_composer)
    , m_max_population(2)
    , m_new_inhabitant_ticks(1000)
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
    assert (elementName == Component_House);
    elementName = "";
#endif

    m_max_population        = XmlUtilities::GetIntAttribute(&i_configuration_node, "max_population", 2);
    m_new_inhabitant_ticks  = XmlUtilities::GetIntAttribute(&i_configuration_node, "new_inhabitant_ticks", 1000);
    }

  void HouseComponentSerializer::ApplyTo(IComponent& i_component) const 
    {
    assert (typeid(HouseComponent) == typeid(i_component));
    HouseComponent& house = static_cast<HouseComponent&>(i_component);
    house.m_max_population        = m_max_population;
    house.m_new_inhabitant_ticks  = m_new_inhabitant_ticks;
    }

  IComponent* HouseComponentSerializer::CreateComponent(Slavs::GameObject* ip_object) const
    {
    HouseComponent* p_house = new HouseComponent(ip_object, m_component_global_id, m_object_composer);
    ApplyTo(*p_house);
    return p_house;
    }

  //////////////////////////////////////////////////////////////////////////
  // Human Component

  HouseComponent::HouseComponent(Slavs::TGameObject ih_owner, int i_component_id, const BaseObjectComposer& i_composer)
    : IHouse(ih_owner, i_component_id)
    , m_max_population(0)
    , m_valid(false)
    , m_object_composer(i_composer)
    , m_current_tick(0)
    , m_new_inhabitant_ticks(0)
    {
    if (nullptr == ih_owner)
      throw std::logic_error("Owner cannot be nullptr");
    ih_owner->GetController()->GetGoverment().GetSocietyManager()->RegisterHouse(this);
    }

  HouseComponent::~HouseComponent()
    {
    m_inhabitants.clear();
    }

  void HouseComponent::Validate() const
    {
    if (m_valid)
      return;

    m_unemployed.clear();
    m_unemployed_number = 0;

    for (Slavs::HumanPtr p_human : m_inhabitants)
      {
      if (!p_human->HasWork())
        {
        m_unemployed.insert(p_human);
        ++m_unemployed_number;
        }
      }

    m_valid = true;
    }

//////////////////////////////////////////////////////////////////////////
// IComponent

  void HouseComponent::TickPerformed()
    {
    if (m_inhabitants.size() >= m_max_population)
      return;

    if (m_current_tick < m_new_inhabitant_ticks)
      {
      ++m_current_tick;
      return;
      }

    Slavs::GameObject* p_owner = static_cast<Slavs::GameObject*>(mp_owner);
    Vector2D& house_position = p_owner->GetPosition();
    Slavs::GameObject* p_new_human = static_cast<Slavs::GameContext&>(p_owner->GetContext()).AddObject(
                                                        m_object_composer.GetObjectGlobalID(ObjectType::OT_HUMAN), 
                                                        Vector2D(house_position.x - rand()%1000, house_position.y + rand()%1000), 
                                                        p_owner->GetController()->GetMask(), 
                                                        p_owner->GetQueryMask());
    HumanComponent* p_component = p_new_human->GetComponent<HumanComponent>();
    p_component->SetHome(this);
    m_inhabitants.insert(p_component);
    m_current_tick = 0;
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
    return static_cast<Slavs::GameObject*>(mp_owner)->HasComponent(m_object_composer.GetComponentGlobalID(ComponentType::CT_HOUSE)) 
        && static_cast<Slavs::GameObject*>(mp_owner)->HasComponent(m_object_composer.GetComponentGlobalID(ComponentType::CT_STATIC_COMPONENT));
    }

//////////////////////////////////////////////////////////////////////////
// IHouse

  size_t HouseComponent::GetPopulation() const
    {
    return m_inhabitants.size();
    }

  void HouseComponent::IncreaseMaximumInhabitatnsBy(size_t i_number)
    {
    m_max_population += i_number;
    }
  
  size_t HouseComponent::GetUnemployedPopulation() const
    {
    Validate();
    return m_unemployed_number;
    }

  void HouseComponent::GetUnemployedPopulation(Slavs::Humans& o_unemployed) const
    {
    Validate();
    std::copy(m_unemployed.begin(), m_unemployed.end(), std::inserter(o_unemployed, o_unemployed.end()));
    }
  
  size_t HouseComponent::GetFreePlaces() const
    {
    return m_max_population - m_inhabitants.size();
    }
  
  bool HouseComponent::AddInhabitant(Slavs::HumanPtr ip_inhabitant)
    {
    if (m_inhabitants.size() >= m_max_population)
      return false;
    return m_inhabitants.insert(ip_inhabitant).second;
    }

  void HouseComponent::RemoveInhabitant(Slavs::HumanPtr ip_inhabitant)
    {
    m_inhabitants.erase(ip_inhabitant);
    }

  void HouseComponent::HumanStateChanged()
    {
    m_valid = false;
    }

  } // BasePlugin