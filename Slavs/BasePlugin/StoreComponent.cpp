#include "stdafx.h"

#include "StoreComponent.h"

#include "BaseObjectComposer.h"
#include "TypeNames.h"
#include "TypeEnumerations.h"

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
  StoreComponentSerializer::StoreComponentSerializer(int i_component_global_id, const BaseObjectComposer& i_composer)
    : IComponentSerializer(i_component_global_id, i_composer)
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

    // small hack for resource management
    
    static_cast<Slavs::GameObject*>(store.GetOwner())->GetController()->GetGoverment().GetEconomyManager()->GetStoreSystem()->Remove(&store);
    std::for_each(m_stored_resources.begin(), m_stored_resources.end(), [&store](const std::pair<GameResourceType, size_t>& resource_pair)
      {
      store.m_resource_containers.push_back(std::make_shared<GameResourceContainer>(resource_pair.first, resource_pair.second, 0));
      });
    static_cast<Slavs::GameObject*>(store.GetOwner())->GetController()->GetGoverment().GetEconomyManager()->GetStoreSystem()->Register(&store);
    //static_cast<Slavs::GameObject*>(store.GetOwner())->GetController()->GetGoverment().GetEconomyManager()->GetStoreSystem()->UpdateStoreInformation(&store);
    }

  IComponent* StoreComponentSerializer::CreateComponent(Slavs::GameObject* ip_object) const
    {
    StoreComponent* p_house = new StoreComponent(ip_object, m_component_global_id, m_object_composer);
    ApplyTo(*p_house);
    return p_house;
    }

  //////////////////////////////////////////////////////////////////////////
  // Store Component

  StoreComponent::StoreComponent(Slavs::TGameObject ih_owner, int i_component_id, const BaseObjectComposer& i_composer)
    : IStore(ih_owner, i_component_id)
    , m_object_composer(i_composer)
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
    return static_cast<Slavs::GameObject*>(mp_owner)->HasComponent(m_object_composer.GetComponentGlobalID(ComponentType::CT_STORE)) 
        && static_cast<Slavs::GameObject*>(mp_owner)->HasComponent(m_object_composer.GetComponentGlobalID(ComponentType::CT_STATIC_COMPONENT));
    }

  //////////////////////////////////////////////////////////////////////////
  // IStore

  bool StoreComponent::AddResource (GameResourceBox& i_resource_box)
    {
    for (auto p_resource_container : m_resource_containers)
      {
      if (i_resource_box.GetGRType() != p_resource_container->GetResType())
        continue;
      if (p_resource_container->AddResource(&i_resource_box))
        return true;
      }
    return false;
    }

  bool StoreComponent::AddResource (int i_type, size_t i_number)
    {
    GameResourceType game_resource_type = static_cast<GameResourceType>(i_type);

    // collect information about resources
    GameResourceBox resource_box(static_cast<GameResourceType>(i_type), 0, 0, 0);
    std::vector<std::shared_ptr<GameResourceContainer>> containers_to_push_resources;
    for (auto p_resource_container : m_resource_containers)
      {
      if (game_resource_type != p_resource_container->GetResType())
        continue;
      size_t available_to_push = p_resource_container->GetResMaxNumber() - p_resource_container->GetResNumber();
      if (i_number <= available_to_push)
        {
        resource_box.Add(i_number);
        containers_to_push_resources.push_back(p_resource_container);
        i_number = 0;
        break;
        }
      else if (available_to_push != 0)
        {
        resource_box.Add(available_to_push);
        containers_to_push_resources.push_back(p_resource_container);
        i_number -= available_to_push;
        }
      }

    if (i_number != 0)
      return false;

    for (auto p_resource_container : containers_to_push_resources)
      p_resource_container->AddResource(&resource_box);

    return true;
    }

  size_t StoreComponent::AddResourceForcely(int i_type, size_t i_number)
    {
    GameResourceBox resource_box(static_cast<GameResourceType>(i_type), i_number, 0, 0);
    AddResource(resource_box);
    return i_number - resource_box.GetNumber();
    }

  size_t StoreComponent::PeekResource (int i_type, size_t i_number) const
    {
    GameResourceType game_resource_type = static_cast<GameResourceType>(i_type);    
    size_t peeked_number = 0;
    for (auto p_resource_container : m_resource_containers)
      {
      if (game_resource_type != p_resource_container->GetResType())
        continue;
      size_t resource_number = p_resource_container->GetResNumber();
      if (i_number <= resource_number)
        {
        peeked_number = i_number;
        break;
        }
      else
        i_number -= peeked_number;
      }
    return peeked_number;
    }

  size_t StoreComponent::GetResource (int i_type, size_t i_number)
    {
    GameResourceType game_resource_type = static_cast<GameResourceType>(i_type);

    // collect information about resources
    std::vector<std::shared_ptr<GameResourceContainer>> containers_for_get;
    size_t need_to_get = i_number;
    for (auto p_resource_container : m_resource_containers)
      {
      if (game_resource_type != p_resource_container->GetResType())
        continue;
      size_t available_to_get = p_resource_container->GetResNumber();
      if (need_to_get <= available_to_get)
        {
        containers_for_get.push_back(p_resource_container);
        need_to_get = 0;
        break;
        }
      else if (available_to_get != 0)
        {
        containers_for_get.push_back(p_resource_container);
        need_to_get -= available_to_get;
        }
      }
    
    if (need_to_get != 0)
      return 0;

    need_to_get = i_number;
    for (auto p_resource_container : containers_for_get)
      i_number -= p_resource_container->PeekResource(i_number);

    return need_to_get;
    }

  size_t StoreComponent::GetResourceForcely (int i_type, size_t i_number)
    {
    GameResourceType game_resource_type = static_cast<GameResourceType>(i_type);    
    size_t peeked_number = 0;
    for (auto p_resource_container : m_resource_containers)
      {
      if (game_resource_type != p_resource_container->GetResType())
        continue;
      size_t peeked_from_container = p_resource_container->PeekResource(i_number);
      peeked_number += peeked_from_container;
      i_number -= peeked_from_container;
      if (i_number == 0)
        break;
      }
    return peeked_number;
    }

  bool StoreComponent::AddResourceContainer(std::shared_ptr<GameResourceContainer> ip_container)
    {
    m_resource_containers.push_back(ip_container);
    return true;
    }

  const std::vector<std::shared_ptr<GameResourceContainer>>& StoreComponent::GetResources() const
    {
    return m_resource_containers;
    }

  } // BasePlugin