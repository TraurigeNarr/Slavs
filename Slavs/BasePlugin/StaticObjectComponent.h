#pragma once

#include "IComponentSerializer.h"

#include <Game/IComponent.h>
#include <Math/Vector2D.h>

#include <SlavsServer/Game/GameObject.h>
#include <SlavsServer/include/Types.h>

namespace BasePlugin
  {
  class StaticObjectComponentSerializer;

  class StaticObjectComponent : public IComponent
    {
    friend StaticObjectComponentSerializer;

    public:
      typedef StaticObjectComponentSerializer TSerializer;

    private:
      float     m_bounding_box_radius;

    private:
      const BaseObjectComposer& m_object_composer;

    public:
      StaticObjectComponent(Slavs::TGameObject ih_owner, int i_component_id, const BaseObjectComposer& i_composer);
      ~StaticObjectComponent();

      virtual void	TickPerformed() override;
      virtual bool	HandleMessage(const Telegram& msg) override;

      virtual void	GetState(GameObjectState& i_state) const override;
      virtual bool	Probe() override;
    };

  class StaticObjectComponentSerializer : public IComponentSerializer
    {
    private:
      float m_bounding_box_radius;

    public:
      StaticObjectComponentSerializer(int i_component_global_id, const BaseObjectComposer& i_composer);
      ~StaticObjectComponentSerializer();

      virtual void        Parse(const TiXmlElement& i_configuration_node) override;
      virtual void        ApplyTo(IComponent& i_component) const override;
      virtual IComponent* CreateComponent(Slavs::GameObject* ip_object) const override;
    };
  }