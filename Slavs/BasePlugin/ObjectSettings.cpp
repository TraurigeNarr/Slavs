#include "stdafx.h"

#include "BaseObjectComposer.h"
#include "ObjectSettings.h"
#include "TypeEnumerations.h"
#include "TypeNames.h"

//types
#include "HouseComponent.h"
#include "HumanComponent.h"
#include "StaticObjectComponent.h"
#include "DynamicObjectComponent.h"
#include "StoreComponent.h"

#include <Utilities/XmlUtilities.h>

#include <Game/GameObject.h>

//////////////////////////////////////////////////////////////////////////

namespace
  {
  
  IComponentSerializer* _GetSerializerFromString(const std::string& i_type, const BaseObjectComposer& i_composer)
    {
    using namespace BasePlugin;
    if (i_type == Component_Human)
      return new HumanComponent::TSerializer(i_composer.GetComponentGlobalID(ComponentType::CT_HUMAN));
    if (i_type == Component_Static)
      return new StaticObjectComponent::TSerializer(i_composer.GetComponentGlobalID(ComponentType::CT_STATIC_OBJECT));
    if (i_type == Component_Dynamic)
      return new DynamicObjectComponent::TSerializer(i_composer.GetComponentGlobalID(ComponentType::CT_DYNAMIC_OBJECT));
    if (i_type == Component_Store)
      return new StoreComponent::TSerializer(i_composer.GetComponentGlobalID(ComponentType::CT_STORE));
    if (i_type == Component_House)
      return new HouseComponent::TSerializer(i_composer.GetComponentGlobalID(ComponentType::CT_HOUSE));
    return nullptr;
    }
  }

//////////////////////////////////////////////////////////////////////////

ObjectSettings::ObjectSettings (const BaseObjectComposer& i_composer)
  : m_composer(i_composer)
  , m_object_type(BasePlugin::ObjectType::OT_NONE)
  {

  }

ObjectSettings::ObjectSettings(BasePlugin::ObjectType i_type, const TiXmlElement& i_object_node, const BaseObjectComposer& i_composer)
  : m_object_type(i_type)
  , m_composer(i_composer)
  {
  Initialize(i_type, i_object_node);
  }

void ObjectSettings::Initialize(BasePlugin::ObjectType i_type, const TiXmlElement& i_object_node)
  {
  m_object_type = i_type;
  std::string component_name = "";

  const TiXmlElement* p_child = 0;

  while (p_child = XmlUtilities::IterateChildElements(&i_object_node, p_child))
    {
    component_name = XmlUtilities::GetStringAttribute(p_child, "type", "");
    IComponentSerializer* p_serializer = _GetSerializerFromString(component_name, m_composer);
    if (nullptr != p_serializer)
      {
      mh_component_serializers.push_back(TSerializer(p_serializer));
      p_serializer->Parse(*p_child);
      }
    }
  }

BasePlugin::ObjectType ObjectSettings::GetType() const
  {
  return m_object_type;
  }

void ObjectSettings::SetupObject(Slavs::GameObject* ip_object) const
  {
  for (const TSerializer& serializer : mh_component_serializers)
    {
    std::unique_ptr<IComponent> h_component(serializer->CreateComponent(ip_object));
    if (h_component)
      {
      ip_object->AddComponent(std::move(h_component));
      }
    }
  }