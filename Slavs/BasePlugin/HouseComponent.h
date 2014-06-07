#pragma once

#include "IComponentSerializer.h"

#include <SlavsServer/PluginSystem/IHouse.h>

#include <Game/IComponent.h>

#include <SlavsServer/Game/GameObject.h>
#include <SlavsServer/include/Types.h>

namespace BasePlugin
  {
  class HouseComponentSerializer;

  class HouseComponent : public IComponent
                       , public Slavs::IHouse
    {
    friend HouseComponentSerializer;

    public:
      typedef HouseComponentSerializer TSerializer;

    private:
      size_t        m_max_population;
      Slavs::Humans m_inhabitants;

    public:
      HouseComponent(Slavs::TGameObject ih_owner, int i_component_id);
      virtual       ~HouseComponent();

    // IComponent
    public:
      virtual void	TickPerformed() override;
      virtual bool	HandleMessage(const Telegram& msg) override;

      virtual void	GetState(GameObjectState& i_state) const override;
      virtual bool	Probe() override;

    // IHouse
    public:
      virtual size_t	GetPopulation() const override;

      virtual size_t	GetUnemployedPopulation() const override;
      virtual void		GetUnemployedPopulation(Slavs::Humans& o_unemployed) const override;

      virtual size_t	GetFreePlaces() const;

      virtual bool		AddInhabitant(Slavs::HumanPtr ip_inhabitant) override;
      virtual void    RemoveInhabitant(Slavs::HumanPtr ip_inhabitant) override;

      virtual void    HumanStateChanged() override;
    };

  class HouseComponentSerializer : public IComponentSerializer
    {
    private:
      size_t m_max_population;

    public:
      HouseComponentSerializer(int i_component_id);
      ~HouseComponentSerializer();

      virtual void        Parse(const TiXmlElement& i_configuration_node) override;
      virtual void        ApplyTo(IComponent& i_component) const override;
      virtual IComponent* CreateComponent(Slavs::GameObject* ip_object) const override;
    };
  }