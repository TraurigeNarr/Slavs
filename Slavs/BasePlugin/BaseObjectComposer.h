#pragma once

#include <PluginSystem/IObjectComposer.h>

namespace BasePlugin
  {
  enum class ObjectType : char;
  enum class ComponentType : char;
  }

class ObjectSettings;
class TiXmlElement;

class BaseObjectComposer : public IObjectComposer
  {
  typedef std::unique_ptr<ObjectSettings> TObjectSettings;
  private:
    /// provides mapping between types constants of this plugin and
    /// id that system gives
    std::map<int, BasePlugin::ObjectType>     m_types_map;
    std::map<int, BasePlugin::ComponentType>  m_components_map;

    std::map<BasePlugin::ObjectType, TObjectSettings> m_object_settings;

  public:
    BaseObjectComposer();
    virtual ~BaseObjectComposer();

    virtual void ComposeObject (Slavs::GameObject* ip_object) override;
    virtual bool Supports (int i_object_type) override;

    virtual void DeclareSupportedTypes() override;
    virtual bool CheckContracts() const override;

    void Initialize(const TiXmlElement& i_composer_element);
  };