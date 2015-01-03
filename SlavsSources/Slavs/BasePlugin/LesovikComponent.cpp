#include "stdafx.h"

#include "LesovikComponent.h"

#include "BaseObjectComposer.h"
#include "TypeNames.h"
#include "TypeEnumerations.h"

#include <Utilities/XmlUtilities.h>
#include <Game/GameObjectState.h>

#include <SlavsServer/Game/GameContext.h>
#include <SlavsServer/Game/GameObject.h>
#include <SlavsServer/Game/GameWorld.h>
#include <SlavsServer/PluginSystem/IResource.h>

//////////////////////////////////////////////////////////////////////////
// temporary solution for humans just to see they can walk somethere
#include "DynamicObjectComponent.h"
#include "MovementStrategyBase.h"
//////////////////////////////////////////////////////////////////////////

namespace BasePlugin
	{

	//////////////////////////////////////////////////////////////////////////
	// Human Component Serializer 
	LesovikSerializer::LesovikSerializer(int i_component_global_id, const BaseObjectComposer& i_composer)
		: IComponentSerializer(i_component_global_id, i_composer)
		{

		}

	LesovikSerializer::~LesovikSerializer()
		{

		}

	void LesovikSerializer::Parse(const TiXmlElement& i_configuration_node)
		{
		std::string elementName = "";

#ifdef _DEBUG
		elementName = XmlUtilities::GetStringAttribute(&i_configuration_node, "type", "");
		assert(elementName == Component_Lesovik);
		elementName = "";
#endif

		const TiXmlElement* childElement = 0;
		const TiXmlElement*	foodElement = 0;

		m_wood_recreating_speed		= XmlUtilities::GetRealAttribute(&i_configuration_node, "wood_recreating_modifier", 0.2f);
		m_slowing_of_wood_mining	= XmlUtilities::GetRealAttribute(&i_configuration_node, "dig_slowing", 0.5f);

		auto p_lakomstva = i_configuration_node.FirstChildElement("lakomstva");
		while (childElement = XmlUtilities::IterateChildElements(p_lakomstva, childElement))
			{
			std::string resource_type = XmlUtilities::GetStringAttribute(childElement, "resource", "");
			assert(!resource_type.empty());
			int res_type = static_cast<int>(GameResourceBox::DefineResType(resource_type));
			size_t res_number = XmlUtilities::GetIntAttribute(childElement, "number", -1);
			size_t use_period = XmlUtilities::GetIntAttribute(childElement, "use_period", -1);

			assert(res_number != -1 && use_period != -1);

			m_resources_for_happy.push_back(Lesovik::Lakomstvo(res_type, res_number, use_period));
			}
		}

	void LesovikSerializer::ApplyTo(IComponent& i_component) const
		{
		assert(typeid(Lesovik) == typeid(i_component));
		Lesovik& lesovik = static_cast<Lesovik&>(i_component);
		
		lesovik.m_slowing_of_wood_mining	= m_slowing_of_wood_mining;
		lesovik.m_wood_recreating_speed		= m_wood_recreating_speed;
		lesovik.m_resources_for_happy			= m_resources_for_happy;
		}

	IComponent* LesovikSerializer::CreateComponent(Slavs::GameObject* ip_object) const
		{
		Lesovik* p_lesovik = new Lesovik(ip_object, m_component_global_id, m_object_composer);
		ApplyTo(*p_lesovik);
		return p_lesovik;
		}

	//////////////////////////////////////////////////////////////////////////
	// Human Component

	Lesovik::Lesovik(Slavs::TGameObject ih_owner, int i_component_id, const BaseObjectComposer& i_composer)
		: IComponent(ih_owner, i_component_id)
		, m_object_composer(i_composer)
		, m_slowing_of_wood_mining(0.5f)
		, m_wood_recreating_speed(0.2f)
		{

		}

	Lesovik::~Lesovik()
		{

		}
	static bool tick = false;
	void Lesovik::TickPerformed()
		{
		tick = true;
		DynamicObjectComponent* p_dynamic_component = this->GetOwner()->GetComponent<DynamicObjectComponent>();
		if (!p_dynamic_component->Moving())
			{
			auto resources = static_cast<Slavs::GameObject*>(mp_owner)->GetContext().GetGameWorld().GetResourcesOfType(GR_Tree);
			assert(!resources.empty());
			auto p_tree = resources.at(0);
			auto position = p_tree->GetOwner()->GetPosition();
			position += Vector2D(rand() % 1000, rand() % 1000);

			std::unique_ptr<IMovementStrategy<DynamicObjectComponent>> p_movement_strategy(new MovementStrategyBase());
			p_movement_strategy->SetTarget(position);
			p_dynamic_component->SetMovementStrategy(std::move(p_movement_strategy));
			}
		}

	bool Lesovik::HandleMessage(const Telegram& msg)
		{
		return false;
		}

	void Lesovik::GetState(GameObjectState& i_state) const
		{
		static bool show = false;
		if (show || !tick)
			return;
		show = true;
		i_state.informationToShow = InformationToShow::Information;
		i_state.informationId = 0;
		}

	bool Lesovik::Probe()
		{
		return static_cast<Slavs::GameObject*>(mp_owner)->HasComponent(m_object_composer.GetComponentGlobalID(ComponentType::CT_LESOVIK))
			&& static_cast<Slavs::GameObject*>(mp_owner)->HasComponent(m_object_composer.GetComponentGlobalID(ComponentType::CT_DYNAMIC_COMPONENT));
		}

	} // BasePlugin