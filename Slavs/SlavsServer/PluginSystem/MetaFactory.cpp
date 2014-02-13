#include "MetaFactory.h"

#include "IObjectComposer.h"
#include "SGameObject.h"

#include <algorithm>
#include <cassert>

//////////////////////////////////////////////////////////////////////////

namespace
  {

  bool _IsDefinitionRegistered(const std::vector<std::string>& i_definitions, const std::string& i_definition)
    {
    return i_definitions.end() != std::find(i_definitions.begin(), i_definitions.end(), i_definition);
    }

  } // namespace

//////////////////////////////////////////////////////////////////////////

MetaFactory::MetaFactory()
  : m_next_definitions_id(0)
  {  }

MetaFactory::~MetaFactory()
  {  }

void MetaFactory::RegisterObjectComposer(TObjectComposer ih_composer)
  {
  TObjectComposers::iterator it = std::find(m_object_composers.begin(), m_object_composers.end(), ih_composer);
  if (it == m_object_composers.end())
    m_object_composers.push_back(ih_composer);
  }

void MetaFactory::UnregisterComposer(TObjectComposer ih_composer)
  {
  std::remove(m_object_composers.begin(), m_object_composers.end(), ih_composer);
  }

void MetaFactory::ComposeObject(SGameObject* iop_object)
  {
  int object_type = iop_object->GetType();

  std::for_each (m_object_composers.begin(), m_object_composers.end(), [iop_object, object_type](TObjectComposer h_object_composer)
    {
    if (h_object_composer->Supports(object_type))
      h_object_composer->ComposeObject(iop_object);
    });
  }


int MetaFactory::RegisterType(const std::string& i_type_name)
  {
  int type_id = -1;
  if (m_type_definitions.end() == m_type_definitions.find(i_type_name))
    {
    type_id = m_type_definitions[i_type_name] = m_next_definitions_id++;
    m_registered_types.push_back(i_type_name);
    }
  else if (!_IsDefinitionRegistered(m_registered_types, i_type_name))
    {
    type_id = m_type_definitions[i_type_name];
    m_registered_types.push_back(i_type_name);
    }
  else
    {
    assert ("<RegisterType>: " + i_type_name + " is already existing");
    //print something to log
    }

  return type_id;
  }

int MetaFactory::RegisterComponent(const std::string& i_component_name)
  {
  int component_id = -1;
  if (m_type_definitions.end() == m_type_definitions.find(i_component_name))
    {
    component_id = m_type_definitions[i_component_name] = m_next_definitions_id++;
    m_registered_types.push_back(i_component_name);
    }
  else if (!_IsDefinitionRegistered(m_registered_types, i_component_name))
    {
    component_id = m_type_definitions[i_component_name];
    m_registered_types.push_back(i_component_name);
    }
  else
    {
    assert ("<RegisterComponent>: " + i_component_name + " is already existing");
    //print something to log
    }

  return component_id;
  }

int MetaFactory::AttachToType(const std::string& i_type_name)
  {
  if (m_type_definitions.end() != m_type_definitions.find(i_type_name))
    {
    return m_type_definitions[i_type_name];
    }
  else
    {
    return m_type_definitions[i_type_name] = m_next_definitions_id++;
    }
  }

int MetaFactory::AttachToComponent(const std::string& i_component_name)
  {
  if (m_type_definitions.end() != m_type_definitions.find(i_component_name))
    {
    return m_type_definitions[i_component_name];
    }
  else
    {
    return m_type_definitions[i_component_name] = m_next_definitions_id++;
    }
  }