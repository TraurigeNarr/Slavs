#pragma once

class TiXmlElement;
class IComponent;

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
  public:
    virtual ~IComponentSerializer(){}

    virtual void Parse(const TiXmlElement& i_configuration_node) = 0;
    virtual void ApplyTo(IComponent& i_component) = 0;
  };