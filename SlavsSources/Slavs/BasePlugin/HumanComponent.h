#pragma once

#include "SlavsBasePluginAPI.h"

#include "IComponentSerializer.h"

#include <SlavsServer/PluginSystem/IHuman.h>

#include <SlavsServer/Game/GameObject.h>
#include <SlavsServer/include/Types.h>

#include <Common/Patterns/StateMachine.h>

class BaseObjectComposer;

namespace BasePlugin
  {
	class Profession;
	typedef std::unique_ptr<Profession> ProfessionPtr;
  class HumanComponentSerializer;

  class HumanComponent : public Slavs::IHuman
    {
    friend HumanComponentSerializer;

    public:
      typedef HumanComponentSerializer TSerializer;

    private:
      const BaseObjectComposer& m_object_composer;

      size_t              m_needed_calories;
      size_t              m_eat_in;
      size_t              m_die_if_no_eat;

      Slavs::HousePtr     mp_home;

      bool                m_has_work;

			StateMachine<HumanComponent, long> m_state_machine;

			std::vector<ProfessionPtr>	m_professions;
			Profession*									mp_current_profession;

    public:
                    SLAVS_BASEPLUGIN_EXPORT           HumanComponent(Slavs::TGameObject ih_owner, int i_component_id, const BaseObjectComposer& i_object_composer);
      virtual       SLAVS_BASEPLUGIN_EXPORT           ~HumanComponent();

    // IComponent
    public:
      virtual void	SLAVS_BASEPLUGIN_EXPORT           TickPerformed() override;
      virtual bool	SLAVS_BASEPLUGIN_EXPORT           HandleMessage(const Telegram& msg) override;

      virtual void	SLAVS_BASEPLUGIN_EXPORT           GetState(GameObjectState& i_state) const override;

      virtual bool	SLAVS_BASEPLUGIN_EXPORT           Probe() override;

    // IHuman
		public:
      virtual void            SLAVS_BASEPLUGIN_EXPORT SetHome(Slavs::HousePtr ip_home) override;
      virtual Slavs::HousePtr SLAVS_BASEPLUGIN_EXPORT GetHome() const override;

      virtual bool            SLAVS_BASEPLUGIN_EXPORT HasWork() const override;

		// HumanComponent
		public:
			StateMachine<HumanComponent, long>& GetStateMachine() { return m_state_machine; }
    };

  class HumanComponentSerializer : public IComponentSerializer
    {
    private:
      size_t m_needed_calories;
      size_t m_eat_in;
      size_t m_die_if_no_eat;

    public:
      HumanComponentSerializer(int i_component_global_id, const BaseObjectComposer& i_composer);
      ~HumanComponentSerializer();

      virtual void        Parse(const TiXmlElement& i_configuration_node) override;
      virtual void        ApplyTo(IComponent& i_component) const override;
      virtual IComponent* CreateComponent(Slavs::GameObject* ip_object) const override;
    };
  }