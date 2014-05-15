#pragma once

#include "IComponentSerializer.h"

#include <Game/IComponent.h>

#include <SlavsServer/Game/GameObject.h>
#include <SlavsServer/include/Types.h>

namespace BasePlugin
  {
  class HouseComponentSerializer;

  class HouseComponent : public IComponent
    {
    friend HouseComponentSerializer;

    public:
      typedef HouseComponentSerializer TSerializer;

    private:
      size_t m_needed_calories;
      size_t m_eat_in;
      size_t m_die_if_no_eat;

    public:
      HouseComponent(Slavs::TGameObject ih_owner, int i_component_id);
      virtual       ~HouseComponent();

      virtual void	TickPerformed() override;
      virtual bool	HandleMessage(const Telegram& msg) override;

      virtual void	GetState(GameObjectState& i_state) const override;

      virtual bool	Probe() override;
    };

  class HouseComponentSerializer : public IComponentSerializer
    {
    private:
      size_t m_needed_calories;
      size_t m_eat_in;
      size_t m_die_if_no_eat;

    public:
      HouseComponentSerializer(int i_component_id);
      ~HouseComponentSerializer();

      virtual void        Parse(const TiXmlElement& i_configuration_node) override;
      virtual void        ApplyTo(IComponent& i_component) const override;
      virtual IComponent* CreateComponent(Slavs::GameObject* ip_object) const override;
    };
  }