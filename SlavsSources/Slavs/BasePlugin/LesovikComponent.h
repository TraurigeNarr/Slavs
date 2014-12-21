#ifndef __LESOVIKCOMPONENT_H__
#define __LESOVIKCOMPONENT_H__

#include "SlavsBasePluginAPI.h"

#include "IComponentSerializer.h"

#include <SlavsServer/include/Types.h>

#include <Common/Game/IComponent.h>

namespace BasePlugin
	{
	class LesovikSerializer;

	class Lesovik : public IComponent
		{
		public:
			struct Lakomstvo
				{
				int			m_resource_id;
				size_t	m_number;
				size_t	m_use_period;
				Lakomstvo(int i_res_id, size_t i_number, size_t i_use_period)
					: m_resource_id(i_res_id)
					, m_number(i_number)
					, m_use_period(i_use_period)
					{}
				};

			typedef LesovikSerializer TSerializer;

		private:
			friend class LesovikSerializer;

		private:
			const BaseObjectComposer& m_object_composer;

			float m_slowing_of_wood_mining;
			float m_wood_recreating_speed;
			
			std::vector<Lakomstvo> m_resources_for_happy;
			std::vector<Lakomstvo> m_current_resource_for_happy;

			// state machine

		public:
			SLAVS_BASEPLUGIN_EXPORT										Lesovik(Slavs::TGameObject ih_owner, int i_component_id, const BaseObjectComposer& i_object_composer);
			virtual       SLAVS_BASEPLUGIN_EXPORT     ~Lesovik();

			// IComponent
		public:
			virtual void	SLAVS_BASEPLUGIN_EXPORT     TickPerformed() override;
			virtual bool	SLAVS_BASEPLUGIN_EXPORT     HandleMessage(const Telegram& msg) override;

			virtual void	SLAVS_BASEPLUGIN_EXPORT     GetState(GameObjectState& i_state) const override;

			virtual bool	SLAVS_BASEPLUGIN_EXPORT     Probe() override;

		// Lesovik
		public:
			float															GetSlowingOfWood() const { return m_slowing_of_wood_mining; }
			float															GetWoodRecreatingSpeed() const { return m_wood_recreating_speed; }
			const std::vector<Lakomstvo>& GetNeededResourcesForHappy() const { return m_resources_for_happy; }
			const std::vector<Lakomstvo>& GetCurrentResourcesForHappy() const { return m_current_resource_for_happy; }

			SLAVS_BASEPLUGIN_EXPORT void			GiveLakomstvo(const Lakomstvo& i_resource);
		};

	class LesovikSerializer : public IComponentSerializer
		{
		private:
			float m_slowing_of_wood_mining;
			float m_wood_recreating_speed;
			std::vector<Lesovik::Lakomstvo> m_resources_for_happy;

		public:
			LesovikSerializer(int i_component_global_id, const BaseObjectComposer& i_composer);
			~LesovikSerializer();

			virtual void        Parse(const TiXmlElement& i_configuration_node) override;
			virtual void        ApplyTo(IComponent& i_component) const override;
			virtual IComponent* CreateComponent(Slavs::GameObject* ip_object) const override;
		};

	}

#endif