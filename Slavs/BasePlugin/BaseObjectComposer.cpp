#include "stdafx.h"

#include "BaseObjectComposer.h"
#include "Types.h"


#include <SGameObject.h>

#include <ServerMain.h>
#include <PluginSystem\MetaFactory.h>

//////////////////////////////////////////////////////////////////////////

namespace BasePlugin
  {
  enum class ObjectType : char
    {
    OT_HUMAN,
    OT_MILL
    };

  enum class ComponentType : char
    {
    CT_HUMAN,
    CT_MANUFACTURE
    };
  }


//////////////////////////////////////////////////////////////////////////

BaseObjectComposer::BaseObjectComposer()
  {

  }

BaseObjectComposer::~BaseObjectComposer()
  {

  }

void BaseObjectComposer::ComposeObject(SGameObject* ip_object)
  {

  }

bool BaseObjectComposer::Supports(int i_object_type)
  {
  return false;
  }

void BaseObjectComposer::DeclareSupportedTypes()
  {
  using namespace BasePlugin;

  MetaFactory& factory = ServerMain::GetInstance().GetMetaFactory();

  // register types
  int global_type_id = -1;
  global_type_id = factory.RegisterType(Object_Human);
  m_types_map[global_type_id] = ObjectType::OT_HUMAN;
  
  // register components
  global_type_id = factory.RegisterComponent(Component_Human);
  m_components_map[global_type_id] = ComponentType::CT_HUMAN;
  }

bool BaseObjectComposer::CheckContracts() const
  {
  return true;
  }