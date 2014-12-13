#ifndef __SELECTIONMOUSE_H__
#define __SELECTIONMOUSE_H__


#include "IMouse.h"

#include "InputSubscriber.h"

namespace UI
	{
	class SelectionBox;

	class SelectionMouse : public GameCore::UI::IMouse
														, public InputSubscriber
		{
		private:
			enum class Mode
				{
				Idle,
				SelectionBox,
				};

			Mode					m_current_mode;
			std::unique_ptr<SelectionBox> mp_selection_box;

			Ogre::Vector2 m_start; 
			Ogre::Vector2 m_stop;

			SDK::Task*		mp_task;

		private:
			virtual void          ActivateImpl(SDK::Task* ip_task) override;
			virtual void          DeactivateImpl() override;

			void									SendSelected();

		public:
			SelectionMouse();
			virtual ~SelectionMouse();

			virtual std::string   GetName() const override { return "Game.SelectionMouse"; }

			virtual void          Update(long i_elapsed_time) override;

		public:
			virtual bool KeyPressed(const OIS::KeyEvent &keyEventRef) override;
			virtual bool KeyReleased(const OIS::KeyEvent &keyEventRef) override;

			virtual bool MouseMoved(const OIS::MouseEvent &evt) override;
			virtual bool MousePressed(const OIS::MouseEvent &evt, OIS::MouseButtonID id) override;
			virtual bool MouseReleased(const OIS::MouseEvent &evt, OIS::MouseButtonID id) override;
		};

	} // UI

#endif