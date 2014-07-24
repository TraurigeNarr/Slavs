#pragma once

#include "IComponentSerializer.h"

#include <Game/IComponent.h>
#include <Math/Vector2D.h>

#include <SlavsServer/Game/GameObject.h>
#include <SlavsServer/include/Types.h>

class BaseObjectComposer;

namespace BasePlugin
  {
  class IMovementStrategy;
  class DynamicObjectComponentSerializer;

  class DynamicObjectComponent : public IComponent
    {
    friend DynamicObjectComponentSerializer;

    public:
      typedef DynamicObjectComponentSerializer TSerializer;

    private:
      const BaseObjectComposer& m_object_composer;
      Vector2D  m_velocity;
      //a normalized vector pointing in the direction the entity is heading
      Vector2D	m_heading;
      /// the length of this object's bounding radius
      float     m_bounding_box_radius;
      /// the maximum speed at which this entity may travel
      float     m_maximum_speed;
      /// the maximum force this entity can produce to power itself
      float     m_maximum_force;
      /// the maximum rate (radians per second) at which this vehicle can rotate
      float     m_maximum_turnrate;
      /// strategy will decide where to go
      IMovementStrategy* mp_movement_strategy;

    public:
      DynamicObjectComponent(Slavs::TGameObject ih_owner, int i_component_id, const BaseObjectComposer& i_composer);
      ~DynamicObjectComponent();

      virtual void	TickPerformed() override;
      virtual bool	HandleMessage(const Telegram& msg) override;

      virtual void	GetState(GameObjectState& i_state) const override;
      virtual bool	Probe() override;

      /// when object does not go anywhere this should be called with nullptr
      void          SetMovementStrategy(IMovementStrategy* ip_movement_strategy);
    };
  
  class DynamicObjectComponentSerializer : public IComponentSerializer
    {
    private:
      float m_bounding_box_radius;
      float m_maximum_speed;
      float m_maximum_force;
      float m_maximum_turnrate;

    public:
      DynamicObjectComponentSerializer(int i_component_global_id, const BaseObjectComposer& i_composer);
      ~DynamicObjectComponentSerializer();

      virtual void        Parse(const TiXmlElement& i_configuration_node) override;
      virtual void        ApplyTo(IComponent& i_component) const override;
      virtual IComponent* CreateComponent(Slavs::GameObject* ip_object) const override;
    };
  }