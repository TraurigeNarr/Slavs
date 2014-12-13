#ifndef __OBJECTSELECTORHANDLER_H__
#define __OBJECTSELECTORHANDLER_H__

#include <vector>

class SPlayerController;

namespace Slavs
	{

	class GameObject;

	class ObjectSelectorHandler
		{
		private:
			SPlayerController&				m_controller;
			std::vector<GameObject*>&	m_selected_objects;

		public:
			ObjectSelectorHandler(SPlayerController& i_cotroller, std::vector<Slavs::GameObject*>& i_selected_objects);
			~ObjectSelectorHandler();

			void SelectObjects(const unsigned char* ip_objects_id, const size_t i_bytes_number);
		};

	} // Slavs

#endif