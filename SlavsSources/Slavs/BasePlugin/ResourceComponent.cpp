#include "stdafx.h"

#include "ResourceComponent.h"

#include "BaseObjectComposer.h"
#include "TypeNames.h"
#include "TypeEnumerations.h"

#include <SlavsServer/Game/GameContext.h>
#include <SlavsServer/Game/GameWorld.h>

#include <Utilities/XmlUtilities.h>
#include <Game/GameObjectState.h>

namespace BasePlugin
	{

	//////////////////////////////////////////////////////////////////////////
	// Resource Component Serializer 
	ResourceComponentSerializer::ResourceComponentSerializer(int i_component_global_id, const BaseObjectComposer& i_composer)
		: IComponentSerializer(i_component_global_id, i_composer)
		{

		}

	ResourceComponentSerializer::~ResourceComponentSerializer()
		{

		}

	void ResourceComponentSerializer::Parse(const TiXmlElement& i_configuration_node)
		{
		std::string elementName = "";

#ifdef _DEBUG
		elementName = XmlUtilities::GetStringAttribute(&i_configuration_node, "type", "");
		assert(elementName == Component_Resource);
		elementName = "";
#endif
		std::string resource_type = XmlUtilities::GetStringAttribute(&i_configuration_node, "resource_type", "");
		assert(!resource_type.empty());
		m_resource_type = static_cast<int>(GameResourceBox::DefineResType(resource_type));

		m_initial_resources = XmlUtilities::GetIntAttribute(&i_configuration_node, "resource_number", 10);
		}

	void ResourceComponentSerializer::ApplyTo(IComponent& i_component) const
		{
		assert(typeid(ResourceComponent) == typeid(i_component));
		ResourceComponent& resource = static_cast<ResourceComponent&>(i_component);
		
		resource.m_resource_type = m_resource_type;
		resource.m_initial_resources = m_initial_resources;
		resource.m_current_resources = 0;
		}

	IComponent* ResourceComponentSerializer::CreateComponent(Slavs::GameObject* ip_object) const
		{
		ResourceComponent* p_human = new ResourceComponent(ip_object, m_component_global_id, m_object_composer);
		ApplyTo(*p_human);
		return p_human;
		}

	//////////////////////////////////////////////////////////////////////////
	// Resource Component 

	ResourceComponent::ResourceComponent(Slavs::TGameObject ih_owner, int i_component_id, const BaseObjectComposer& i_object_composer)
		: IResource(ih_owner, i_component_id)
		, m_object_composer(i_object_composer)
		{
		static_cast<Slavs::GameObject*>(mp_owner)->GetContext().GetGameWorld().RegisterResource(this);
		}

	ResourceComponent::~ResourceComponent()
		{
		static_cast<Slavs::GameObject*>(mp_owner)->GetContext().GetGameWorld().UnregisterResource(this);
		}

	void ResourceComponent::TickPerformed()
		{}

	bool ResourceComponent::HandleMessage(const Telegram& msg)
		{
		return false;
		}

	void ResourceComponent::GetState(GameObjectState& i_state) const
		{}

	bool ResourceComponent::Probe()
		{
		return static_cast<Slavs::GameObject*>(mp_owner)->HasComponent(m_object_composer.GetComponentGlobalID(ComponentType::CT_RESOURCE))
			&& static_cast<Slavs::GameObject*>(mp_owner)->HasComponent(m_object_composer.GetComponentGlobalID(ComponentType::CT_STATIC_COMPONENT));
		}

	} // BasePlugin