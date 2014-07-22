#pragma once

#include "SlavsBasePluginAPI.h"

#include "IComponentSerializer.h"

#include <SlavsServer/PluginSystem/IHouse.h>

#include <SlavsServer/Game/GameObject.h>
#include <SlavsServer/include/Types.h>

namespace BasePlugin
  {
  class HouseComponentSerializer;

  class HouseComponent : public Slavs::IHouse
    {
    friend HouseComponentSerializer;

    public:
      typedef HouseComponentSerializer TSerializer;

    private:
      size_t        m_max_population;
      Slavs::Humans m_inhabitants;

      mutable bool          m_valid;
      mutable size_t        m_unemployed_number;
      mutable Slavs::Humans m_unemployed;

    private:
      void          Validate() const;

    public:
      SLAVS_BASEPLUGIN_EXPORT                 HouseComponent(Slavs::TGameObject ih_owner, int i_component_id);
      virtual       SLAVS_BASEPLUGIN_EXPORT   ~HouseComponent();

    // IComponent
    public:
      virtual void	SLAVS_BASEPLUGIN_EXPORT   TickPerformed() override;
      virtual bool	SLAVS_BASEPLUGIN_EXPORT   HandleMessage(const Telegram& msg) override;

      virtual void	SLAVS_BASEPLUGIN_EXPORT   GetState(GameObjectState& i_state) const override;
      virtual bool	SLAVS_BASEPLUGIN_EXPORT   Probe() override;

    // IHouse
    public:
      virtual size_t	SLAVS_BASEPLUGIN_EXPORT GetPopulation() const override;
      virtual void    SLAVS_BASEPLUGIN_EXPORT IncreaseMaximumInhabitatnsBy(size_t i_number) override;

      virtual size_t	SLAVS_BASEPLUGIN_EXPORT GetUnemployedPopulation() const override;
      virtual void		SLAVS_BASEPLUGIN_EXPORT GetUnemployedPopulation(Slavs::Humans& o_unemployed) const override;

      virtual size_t	SLAVS_BASEPLUGIN_EXPORT GetFreePlaces() const;

      virtual bool		SLAVS_BASEPLUGIN_EXPORT AddInhabitant(Slavs::HumanPtr ip_inhabitant) override;
      virtual void    SLAVS_BASEPLUGIN_EXPORT RemoveInhabitant(Slavs::HumanPtr ip_inhabitant) override;

      virtual void    SLAVS_BASEPLUGIN_EXPORT HumanStateChanged() override;
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