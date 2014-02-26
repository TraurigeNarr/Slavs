#include "stdafx.h"

#include "BaseObjectComposer.h"
#include "TypeNames.h"
#include "TypeEnumerations.h"
#include "ObjectSettings.h"

#include <GameObject.h>

#include <Utilities/XmlUtilities.h>

#include <ServerMain.h>
#include <PluginSystem\MetaFactory.h>

//////////////////////////////////////////////////////////////////////////

namespace
  {
  BasePlugin::ObjectType _GetTypeFromString(const std::string& i_type)
    {
    if (i_type == BasePlugin::Object_Human)
      {
      return BasePlugin::ObjectType::OT_HUMAN;
      }
    return BasePlugin::ObjectType::OT_NONE;
    }
  } // namespace

//////////////////////////////////////////////////////////////////////////

BaseObjectComposer::BaseObjectComposer()
  {

  }

BaseObjectComposer::~BaseObjectComposer()
  {

  }

void BaseObjectComposer::ComposeObject(Slavs::GameObject* ip_object)
  {
  std::map<int, BasePlugin::ObjectType>::iterator it = m_types_map.find(ip_object->GetType());
  assert (it != m_types_map.end());
  std::map<BasePlugin::ObjectType, TObjectSettings>::iterator settings_it = m_object_settings.find(it->second);
  assert (settings_it != m_object_settings.end());
  
  settings_it->second->SetupObject(ip_object);
  }

bool BaseObjectComposer::Supports(int i_object_type)
  {
  return m_types_map.find(i_object_type) != m_types_map.end();
  }

void BaseObjectComposer::DeclareSupportedTypes()
  {
  MetaFactory& factory = ServerMain::GetInstance().GetMetaFactory();

  // register types
  int global_type_id = -1;
  global_type_id = factory.RegisterType(BasePlugin::ConstructBasePluginName(BasePlugin::Object_Human));
  m_types_map[global_type_id] = BasePlugin::ObjectType::OT_HUMAN;
  
  // register components
  global_type_id = factory.RegisterComponent(BasePlugin::ConstructBasePluginName(BasePlugin::Component_Human));
  m_components_map[global_type_id] = BasePlugin::ComponentType::CT_HUMAN;
  global_type_id = factory.RegisterComponent(BasePlugin::ConstructBasePluginName(BasePlugin::Component_Static));
  m_components_map[global_type_id] = BasePlugin::ComponentType::CT_STATIC_OBJECT;
  }

bool BaseObjectComposer::CheckContracts() const
  {
  return true;
  }

void BaseObjectComposer::Initialize(const TiXmlElement& i_composer_element)
  {
#ifdef _DEBUG
  std::string element_name = i_composer_element.Value();
  assert (element_name == "Composer");
#endif

  std::string object_name = "";

  const TiXmlElement* p_child = nullptr;

  while ((p_child = XmlUtilities::IterateChildElements(&i_composer_element, p_child)))
    {
    object_name = p_child->Value();

    BasePlugin::ObjectType obj_type = _GetTypeFromString(object_name);

    if (obj_type != BasePlugin::ObjectType::OT_NONE)
      m_object_settings[obj_type] = TObjectSettings(new ObjectSettings(obj_type, *p_child));
    }

  }