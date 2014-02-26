#pragma once

#include "IComponentSerializer.h"

#include <Game/IComponent.h>

#include <SlavsServer/GameObject.h>
#include <SlavsServer/include/Types.h>

namespace BasePlugin
  {
  class HumanComponentSerializer;

  class HumanComponent : public IComponent
    {
    friend HumanComponentSerializer;

    public:
      typedef HumanComponentSerializer TSerializer;

    private:
      size_t m_needed_calories;
      size_t m_eat_in;
      size_t m_die_if_no_eat;

    public:
                    HumanComponent(Slavs::TGameObject ih_owner);
      virtual       ~HumanComponent();

      virtual void	TickPerformed() override;
      virtual bool	HandleMessage(const Telegram& msg) override;

      virtual void	GetState(GameObjectState& i_state) const override;

      virtual bool	Probe() override;
    };

  class HumanComponentSerializer : public IComponentSerializer
    {
    private:
      size_t m_needed_calories;
      size_t m_eat_in;
      size_t m_die_if_no_eat;

    public:
      HumanComponentSerializer();
      ~HumanComponentSerializer();

      virtual void        Parse(const TiXmlElement& i_configuration_node) override;
      virtual void        ApplyTo(IComponent& i_component) const override;
      virtual IComponent* CreateComponent(Slavs::GameObject* ip_object) const override;
    };
  }