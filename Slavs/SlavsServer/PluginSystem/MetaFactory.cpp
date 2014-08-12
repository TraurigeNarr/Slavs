#include "MetaFactory.h"

#include "IObjectComposer.h"
#include "Game/GameObject.h"

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
    {
    m_object_composers.push_back(ih_composer);
    ih_composer->DeclareSupportedTypes();
    }
  }

void MetaFactory::UnregisterComposer(TObjectComposer ih_composer)
  {
  auto new_end_it = std::remove(m_object_composers.begin(), m_object_composers.end(), ih_composer);
  m_object_composers.erase(new_end_it, m_object_composers.end());
  }

bool MetaFactory::ComposeObject(Slavs::GameObject* iop_object)
  {
  int object_type = iop_object->GetType();
  bool composed = false;
  std::for_each (m_object_composers.begin(), m_object_composers.end(), [&composed, iop_object, object_type](TObjectComposer h_object_composer)
    {
    if (h_object_composer->Supports(object_type))
      {
      h_object_composer->ComposeObject(iop_object);
      composed = true;
      }
    });
  iop_object->ProbeComponents();
  return composed;
  }

bool MetaFactory::SupportObject(int i_type) const
  {
  bool support = false;
  std::for_each (m_object_composers.begin(), m_object_composers.end(), [&support, i_type](TObjectComposer h_object_composer)
    {
    support |= h_object_composer->Supports(i_type);
    });
  return support;
  }

bool MetaFactory::SupportObject(const std::string& i_type) const
  {
  auto definition_it = m_type_definitions.find(i_type);
  return definition_it != m_type_definitions.end()
        && SupportObject(definition_it->second);
  }

int MetaFactory::GetObjectID(const std::string& i_type) const
  {
  auto id_it = m_type_definitions.find(i_type);
  return id_it == m_type_definitions.end() ? -1 : (*id_it).second;
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
    assert ("<RegisterType>: type is already existing");
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
    assert ("<RegisterComponent> component is already existing");
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

void MetaFactory::CheckContracts()
  {
  std::for_each(m_object_composers.begin(), m_object_composers.end(), [](TObjectComposer ih_composer)
    {
    if (!ih_composer->CheckContracts())
      throw std::runtime_error("Bad contract");
    });
  }