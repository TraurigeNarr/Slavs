#include "ObjectSelectorHandler.h"

#include "SPlayerController.h"

#include <Game/GameObject.h>
#include <Game/GameContext.h>

#include <Utilities/TemplateFunctions.h>

namespace Slavs
	{

	ObjectSelectorHandler::ObjectSelectorHandler(SPlayerController& i_controller, std::vector<GameObject*>& i_selected_objects)
		: m_controller(i_controller)
		, m_selected_objects(i_selected_objects)
		{}

	ObjectSelectorHandler::~ObjectSelectorHandler()
		{}

	void ObjectSelectorHandler::SelectObjects(const unsigned char* ip_objects_id, const size_t i_bytes_number)
		{
		for (auto p_object : m_selected_objects)
			p_object->Deselect();

		m_selected_objects.clear();

		if (i_bytes_number == 0)
			return;

		m_selected_objects.reserve(i_bytes_number / sizeof(long));

		for (size_t i = 0; i < i_bytes_number; i += sizeof(long))
			{
			auto p_object = m_controller.GetGameContext().GetGameObject(ConvertTo<long>(&ip_objects_id[i]));
			if (p_object)
				{
				p_object->Select();
				m_selected_objects.push_back(p_object);
				}
			}
		}

	} // Slavs