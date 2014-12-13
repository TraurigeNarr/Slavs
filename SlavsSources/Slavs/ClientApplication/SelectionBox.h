#ifndef __SELECTIONBOX_H__
#define __SELECTIONBOX_H__

#include "OgreManualObject.h"

namespace UI
	{

	class SelectionBox : public Ogre::ManualObject
		{
		private:
			Ogre::SceneManager& m_scene_manager;

		public:
			SelectionBox(const std::string& i_name, Ogre::SceneManager& i_manager);
			virtual ~SelectionBox();

			void setCorners(float left, float top, float right, float bottom);
			void setCorners(const Ogre::Vector2& topLeft, const Ogre::Vector2& bottomRight);
		};

	}

#endif