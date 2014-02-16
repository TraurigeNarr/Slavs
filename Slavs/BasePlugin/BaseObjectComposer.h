#pragma once

#include <PluginSystem/IObjectComposer.h>

#include <map>

namespace BasePlugin
  {
  enum class ObjectType : char;
  enum class ComponentType : char;
  }

class BaseObjectComposer : public IObjectComposer
  {
  private:
    /// provides mapping between types constants of this plugin and
    /// id that system gives
    std::map<int, BasePlugin::ObjectType>     m_types_map;
    std::map<int, BasePlugin::ComponentType>  m_components_map;
  public:
    BaseObjectComposer();
    virtual ~BaseObjectComposer();

    virtual void ComposeObject (SGameObject* ip_object) override;
    virtual bool Supports (int i_object_type) override;

    virtual void DeclareSupportedTypes() override;
    virtual bool CheckContracts() const override;
  };