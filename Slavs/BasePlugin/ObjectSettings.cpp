#include "stdafx.h"

#include "ObjectSettings.h"
#include "TypeNames.h"
#include "TypeEnumerations.h"

//types
#include "HumanComponent.h"
#include "StaticObjectComponent.h"
#include "DynamicObjectComponent.h"

#include <Utilities/XmlUtilities.h>

#include <GameObject.h>

#include <typeinfo>

//////////////////////////////////////////////////////////////////////////

namespace
  {
  
  IComponentSerializer* _GetSerializerFromString(const std::string& i_type)
    {
    if (i_type == BasePlugin::Component_Human)
      return new BasePlugin::HumanComponent::TSerializer();
    if (i_type == BasePlugin::Component_Static)
      return new BasePlugin::StaticObjectComponent::TSerializer();
    if (i_type == BasePlugin::Component_Dynamic)
      return new BasePlugin::DynamicObjectComponent::TSerializer();
    return nullptr;
    }
  }

//////////////////////////////////////////////////////////////////////////

ObjectSettings::ObjectSettings()
  : m_object_type(BasePlugin::ObjectType::OT_NONE)
  {

  }

ObjectSettings::ObjectSettings(BasePlugin::ObjectType i_type, const TiXmlElement& i_object_node)
  : m_object_type(i_type)
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
    IComponentSerializer* p_serializer = _GetSerializerFromString(component_name);
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
    serializer->CreateComponent(ip_object);
    }
  }