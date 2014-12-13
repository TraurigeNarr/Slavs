#include "stdafx.h"

#include "SelectionMouse.h"
#include "SelectionBox.h"

#include "Application.h"

#include "MouseManager.h"
#include "MouseTypes.h"

#include "InputManager.h"

#include "Task.h"

namespace UI
	{
	
	SelectionMouse::SelectionMouse()
		: IMouse(static_cast<int>(MouseType::Selection))
		, mp_selection_box(nullptr)
		{}

	SelectionMouse::~SelectionMouse()
		{}

	void SelectionMouse::ActivateImpl(SDK::Task* ip_task)
		{
		auto p_scene_manager = ClientGame::appInstance.GetOgreFramework().GetSceneManager();
		mp_selection_box.reset(new SelectionBox("SelectionBox", *p_scene_manager));
		
		if (ip_task)
			mp_task = ip_task;

		ClientGame::appInstance.GetInputManager().AddSubscriber(this);
		}

	void SelectionMouse::DeactivateImpl()
		{
		if (m_current_mode == Mode::SelectionBox)
			{
			auto& cursor = CEGUI::System::getSingleton().getDefaultGUIContext().getMouseCursor();
			cursor.show();
			m_current_mode = Mode::Idle;
			}
		mp_selection_box.reset();
		
		ClientGame::appInstance.GetInputManager().RemoveSubscriber(this);
		}

	void SelectionMouse::Update(long i_elapsed_time)
		{

		}

	/////////////////////////////////////////////////////////////////////
	// InputListener

	bool SelectionMouse::KeyPressed(const OIS::KeyEvent &keyEventRef)
		{

		return true;
		}

	bool SelectionMouse::KeyReleased(const OIS::KeyEvent &keyEventRef)
		{
		
		return true;
		}

	bool SelectionMouse::MouseMoved(const OIS::MouseEvent &evt)
		{
		if (m_current_mode == Mode::SelectionBox)
			{
			auto& cursor = CEGUI::System::getSingleton().getDefaultGUIContext().getMouseCursor();
			m_stop.x = cursor.getPosition().d_x / static_cast<float>(evt.state.width);
			m_stop.y = cursor.getPosition().d_y / static_cast<float>(evt.state.height);

			mp_selection_box->setCorners(m_start, m_stop);
			}
		return true;
		}

	bool SelectionMouse::MousePressed(const OIS::MouseEvent &evt, OIS::MouseButtonID id)
		{
		if (id == OIS::MB_Left)
			{
			auto& cursor = CEGUI::System::getSingleton().getDefaultGUIContext().getMouseCursor();
			cursor.hide();
			m_current_mode = Mode::SelectionBox;

			m_start.x = cursor.getPosition().d_x / static_cast<float>(evt.state.width);
			m_start.y = cursor.getPosition().d_y / static_cast<float>(evt.state.height);
			m_stop = m_start;

			mp_selection_box->clear();
			mp_selection_box->setVisible(true);
			}

		return true;
		}

	bool SelectionMouse::MouseReleased(const OIS::MouseEvent &evt, OIS::MouseButtonID id)
		{
		auto& cursor = CEGUI::System::getSingleton().getDefaultGUIContext().getMouseCursor();
		cursor.show();

		if (mp_task)
			{
			auto start_stop = std::make_pair(m_start, m_stop);
			mp_task->Complete(start_stop);
			if (mp_task->IsComplete())
				mp_task = nullptr;
			}

		m_current_mode = Mode::Idle;
		mp_selection_box->clear();
		mp_selection_box->setVisible(false);

		return true;
		}

	} // UI