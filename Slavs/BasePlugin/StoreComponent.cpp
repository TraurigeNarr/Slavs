#include "stdafx.h"

#include "StoreComponent.h"

#include "TypeNames.h"

#include <Utilities/XmlUtilities.h>
#include <Game/GameObjectState.h>

#include <SlavsServer/include/IController.h>
#include <SlavsServer/include/Management/Goverment.h>
#include <SlavsServer/include/Management/IStoreSystem.h>
#include <SlavsServer/include/Management/IEconomyManager.h>

using namespace Slavs;

namespace BasePlugin
  {
  //////////////////////////////////////////////////////////////////////////
  // Store Component Serializer 
  StoreComponentSerializer::StoreComponentSerializer(int i_component_id)
    : IComponentSerializer(i_component_id)
    {

    }

  StoreComponentSerializer::~StoreComponentSerializer()
    {

    }

  void StoreComponentSerializer::_FillResources(const TiXmlElement& i_resource_element)
    {
    std::string element_name = "";
#ifdef _DEBUG
    element_name = i_resource_element.Value();
    assert (element_name == "resources");
    element_name = "";
#endif

    const TiXmlElement* childElement = 0;
    const TiXmlElement*	foodElement = 0;

    std::string res_type_name;
    size_t max_number = 0;
    while (childElement = XmlUtilities::IterateChildElements(&i_resource_element, childElement))
      {
      element_name = childElement->Value();

      // type
      res_type_name = XmlUtilities::GetStringAttribute(childElement, "type", "");
      GameResourceType res_type = GameResourceBox::DefineResType(res_type_name);
      if (res_type == GR_None)
        continue;
      // max number
      max_number = XmlUtilities::GetIntAttribute(childElement, "max_number", 0);

      m_stored_resources.push_back(std::make_pair(res_type, max_number));
      }
    }

  void StoreComponentSerializer::Parse(const TiXmlElement& i_configuration_node)
    {
    std::string elementName = "";

#ifdef _DEBUG
    elementName = XmlUtilities::GetStringAttribute(&i_configuration_node, "type", "");
    assert (elementName == Component_Store);
    assert (i_configuration_node.FirstChildElement("resources"));
    elementName = "";
#endif

    m_workers_number = XmlUtilities::GetIntAttribute(&i_configuration_node, "workers", 0);
    assert (m_workers_number != 0);

    _FillResources(*i_configuration_node.FirstChildElement("resources"));
    }

  void StoreComponentSerializer::ApplyTo(IComponent& i_component) const 
    {
    assert (typeid(StoreComponent) == typeid(i_component));
    StoreComponent& store = static_cast<StoreComponent&>(i_component);

    store.m_workers_number = m_workers_number;
    std::for_each(m_stored_resources.begin(), m_stored_resources.end(), [&store](const std::pair<GameResourceType, size_t>& resource_pair)
      {
      store.m_resource_containers.push_back(std::make_shared<GameResourceContainer>(resource_pair.first, resource_pair.second, 0));
      });

    }

  IComponent* StoreComponentSerializer::CreateComponent(Slavs::GameObject* ip_object) const
    {
    StoreComponent* p_house = new StoreComponent(ip_object, m_component_global_id);
    ApplyTo(*p_house);
    return p_house;
    }

  //////////////////////////////////////////////////////////////////////////
  // Store Component

  StoreComponent::StoreComponent(Slavs::TGameObject ih_owner, int i_component_id)
    : IComponent(ih_owner, i_component_id)
    {
    ih_owner->GetController()->GetGoverment().GetEconomyManager()->GetStoreSystem()->Register(this);
    }

  StoreComponent::~StoreComponent()
    {

    }

  //////////////////////////////////////////////////////////////////////////
  // IComponent

  void StoreComponent::TickPerformed()
    {

    }

  bool StoreComponent::HandleMessage(const Telegram& msg)
    {
    return false;
    }

  void StoreComponent::GetState(GameObjectState& i_state) const
    {

    }

  bool StoreComponent::Probe()
    {
    return static_cast<Slavs::GameObject*>(mp_owner)->HasComponent(m_component_id);
    }

  //////////////////////////////////////////////////////////////////////////
  // IStore

  bool StoreComponent::AddResource (GameResourceBox& i_resource_box)
    {
    return false;
    }

  void StoreComponent::AddResource (int i_type, size_t i_number)
    {

    }

  bool StoreComponent::PeekResource (int i_type, size_t i_number)
    {
    return false;
    }

  size_t StoreComponent::GetResource (int i_type, size_t i_number)
    {
    return 0;
    }

  size_t StoreComponent::GetResourceForcely (int i_type, size_t i_number)
    {
    return 0;
    }

  const std::vector<std::shared_ptr<GameResourceContainer>>& StoreComponent::GetResources() const
    {
    return m_resource_containers;
    }

  } // BasePlugin