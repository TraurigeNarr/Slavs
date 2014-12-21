#include "stdafx.h"

#include "BaseObjectComposer.h"
#include "ObjectSettings.h"
#include "TypeEnumerations.h"
#include "TypeNames.h"

//types
#include "DynamicObjectComponent.h"
#include "HouseComponent.h"
#include "HumanComponent.h"
#include "ManufactureComponent.h"
#include "StaticObjectComponent.h"
#include "StoreComponent.h"
#include "PrimitiveBrainComponent.h"
#include "ResourceComponent.h"
#include "LesovikComponent.h"

#include <Utilities/XmlUtilities.h>

#include <Game/GameObject.h>

//////////////////////////////////////////////////////////////////////////

namespace
  {
  
  IComponentSerializer* _GetSerializerFromString(const std::string& i_type, const BaseObjectComposer& i_composer)
    {
    using namespace BasePlugin;
    if (i_type == Component_Human)
      return new HumanComponent::TSerializer(i_composer.GetComponentGlobalID(ComponentType::CT_HUMAN), i_composer);
    if (i_type == Component_Static)
      return new StaticObjectComponent::TSerializer(i_composer.GetComponentGlobalID(ComponentType::CT_STATIC_COMPONENT), i_composer);
    if (i_type == Component_Dynamic)
      return new DynamicObjectComponent::TSerializer(i_composer.GetComponentGlobalID(ComponentType::CT_DYNAMIC_COMPONENT), i_composer);
    if (i_type == Component_Store)
      return new StoreComponent::TSerializer(i_composer.GetComponentGlobalID(ComponentType::CT_STORE), i_composer);
    if (i_type == Component_House)
      return new HouseComponent::TSerializer(i_composer.GetComponentGlobalID(ComponentType::CT_HOUSE), i_composer);
    if (i_type == Component_Manufacture)
      return new ManufactureComponent::TSerializer(i_composer.GetComponentGlobalID(ComponentType::CT_MANUFACTURE), i_composer);
    if (i_type == Component_PrimitiveBrain)
      return new PrimitiveBrainComponent::TSerializer(i_composer.GetComponentGlobalID(ComponentType::CT_PRIMITIVE_BRAIN), i_composer);
		if (i_type == Component_Resource)
			return new ResourceComponent::TSerializer(i_composer.GetComponentGlobalID(ComponentType::CT_RESOURCE), i_composer);
		if (i_type == Component_Lesovik)
			return new Lesovik::TSerializer(i_composer.GetComponentGlobalID(ComponentType::CT_LESOVIK), i_composer);
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