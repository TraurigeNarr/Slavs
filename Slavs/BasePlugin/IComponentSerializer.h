#pragma once

class TiXmlElement;
class IComponent;

namespace Slavs
  {
  class GameObject;
  }

/*
-Parse
  Parse data from i_configuration_node and stores it internally.
-ApplyTo
  Applies stored information to component.

Need specific realizations of IComponentSerializer for each component.

See HumanComponent as example.
*/

class IComponentSerializer
  {
  protected:
    int m_component_global_id;
  public:
    IComponentSerializer(int i_component_id) : m_component_global_id(i_component_id) { }
    virtual ~IComponentSerializer() {}

    virtual void        Parse(const TiXmlElement& i_configuration_node) = 0;
    virtual void        ApplyTo(IComponent& i_component) const = 0;
    /// creates component and sets ip_object as owner
    /// return component pointer if successfull; nullptr otherwise
    virtual IComponent* CreateComponent(Slavs::GameObject* ip_object) const = 0;
  };