#ifndef __RESOURCECOMPONENT_H__
#define __RESOURCECOMPONENT_H__

#include "SlavsBasePluginAPI.h"

#include "IComponentSerializer.h"

#include <SlavsServer/PluginSystem/IResource.h>

#include <SlavsServer/Game/GameObject.h>
#include <SlavsServer/include/Types.h>

class BaseObjectComposer;

namespace BasePlugin
	{

	class ResourceComponentSerializer;

	class ResourceComponent : public Slavs::IResource
		{
		friend ResourceComponentSerializer;

		public:
			typedef ResourceComponentSerializer TSerializer;

		private:
			const BaseObjectComposer& m_object_composer;

			int m_resource_type;
			size_t m_initial_resources;
			size_t m_current_resources;

		public:
			SLAVS_BASEPLUGIN_EXPORT													ResourceComponent(Slavs::TGameObject ih_owner, int i_component_id, const BaseObjectComposer& i_object_composer);
			virtual       SLAVS_BASEPLUGIN_EXPORT           ~ResourceComponent();

			// IComponent
		public:
			virtual void	SLAVS_BASEPLUGIN_EXPORT           TickPerformed() override;
			virtual bool	SLAVS_BASEPLUGIN_EXPORT           HandleMessage(const Telegram& msg) override;

			virtual void	SLAVS_BASEPLUGIN_EXPORT           GetState(GameObjectState& i_state) const override;

			virtual bool	SLAVS_BASEPLUGIN_EXPORT           Probe() override;
		};

	class ResourceComponentSerializer : public IComponentSerializer
		{
		private:
			int m_resource_type;
			size_t m_initial_resources;

		public:
			ResourceComponentSerializer(int i_component_global_id, const BaseObjectComposer& i_composer);
			~ResourceComponentSerializer();

			virtual void        Parse(const TiXmlElement& i_configuration_node) override;
			virtual void        ApplyTo(IComponent& i_component) const override;
			virtual IComponent* CreateComponent(Slavs::GameObject* ip_object) const override;
		};

	} // BasePlugin

#endif