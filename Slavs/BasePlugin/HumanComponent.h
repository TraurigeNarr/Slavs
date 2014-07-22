#pragma once

#include "IComponentSerializer.h"

#include <SlavsServer/PluginSystem/IHuman.h>

#include <SlavsServer/Game/GameObject.h>
#include <SlavsServer/include/Types.h>

namespace BasePlugin
  {
  class HumanComponentSerializer;

  class HumanComponent : public Slavs::IHuman
    {
    friend HumanComponentSerializer;

    public:
      typedef HumanComponentSerializer TSerializer;

    private:
      size_t m_needed_calories;
      size_t m_eat_in;
      size_t m_die_if_no_eat;

      Slavs::HousePtr mp_home;

    public:
                    HumanComponent(Slavs::TGameObject ih_owner, int i_component_id);
      virtual       ~HumanComponent();

    // IComponent
    public:
      virtual void	TickPerformed() override;
      virtual bool	HandleMessage(const Telegram& msg) override;

      virtual void	GetState(GameObjectState& i_state) const override;

      virtual bool	Probe() override;

    // IHouse 
    public:
      virtual void            SetHome(Slavs::HousePtr ip_home) override;
      virtual Slavs::HousePtr GetHome() const override;

      virtual bool            HasWork() const override;
    };

  class HumanComponentSerializer : public IComponentSerializer
    {
    private:
      size_t m_needed_calories;
      size_t m_eat_in;
      size_t m_die_if_no_eat;

    public:
      HumanComponentSerializer(int i_component_id);
      ~HumanComponentSerializer();

      virtual void        Parse(const TiXmlElement& i_configuration_node) override;
      virtual void        ApplyTo(IComponent& i_component) const override;
      virtual IComponent* CreateComponent(Slavs::GameObject* ip_object) const override;
    };
  }