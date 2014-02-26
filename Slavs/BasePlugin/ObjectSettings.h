#pragma once

#include "IComponentSerializer.h"

class TiXmlElement;

namespace Slavs
  {
  class GameObject;
  }

namespace BasePlugin
  {
  enum class ObjectType : char;
  enum class ComponentType : char;
  }

/*
Settings that are read from .xml configuration
node or .xml document.
Contains information:
  - what components object consists of
  - settings for components
*/

class ObjectSettings
  {
  public:
    typedef std::unique_ptr<IComponentSerializer> TSerializer;
    typedef std::vector<TSerializer> TComponents;

  private:
    BasePlugin::ObjectType m_object_type;
    TComponents mh_component_serializers;

  public:
    ObjectSettings ();
    // construct object settings for i_type
    // parse i_object_node for getting list of components
    ObjectSettings (BasePlugin::ObjectType i_type, const TiXmlElement& i_object_node);

    void Initialize(BasePlugin::ObjectType i_type, const TiXmlElement& i_object_node);

    BasePlugin::ObjectType GetType() const;    
    void                   SetupObject(Slavs::GameObject* ip_object) const;
  };