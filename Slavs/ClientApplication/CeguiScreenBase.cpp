#include "stdafx.h"

#include "CeguiScreenBase.h"
#include "CeguiUtilities.h"
#include "TimeUtilities.h"

#include "ControlData.h"
#include "InputManager.h"
#include "ScreenManager.h"
#include "InputSubscriber.h"

#include <CEGUI/EventArgs.h>
#include <CEGUI/Window.h>

namespace UI
  {

  CeguiScreenBase::CeguiScreenBase(ScreenManager& i_screen_manager)
    : Screen(i_screen_manager)
    , mp_root_window(nullptr)
    {    }

  CeguiScreenBase::~CeguiScreenBase()
    {
    assert (mp_root_window == nullptr && "Call destroy method first.");
    }

  //////////////////////////////////////////////////////////////////////////
  // Screen

  void  CeguiScreenBase::Update(long i_elapsed_time)
    {
    float elapsed_time = TimeUtilities::ConvertTime(i_elapsed_time);
    CEGUI::System::getSingleton().injectTimePulse(elapsed_time);
    CEGUI::System::getSingleton().getDefaultGUIContext().injectTimePulse(elapsed_time);
    }

  void  CeguiScreenBase::Destroy()
    {
    if (mp_root_window)
      {
      CeguiUtilities::ClearWindow(*mp_root_window, true);
      mp_root_window = nullptr;
      }
    }

  void  CeguiScreenBase::SetRootWindow(void* ip_window)
    {
    mp_root_window = reinterpret_cast<CEGUI::Window*>(ip_window);
    if (mp_root_window == nullptr)
      throw std::runtime_error("Root window cannot be nullptr.");
    }

  void* CeguiScreenBase::GetRootWindow()
    {
    return mp_root_window;
    }

  //////////////////////////////////////////////////////////////////////////
  // CeguiScreenBase

  bool CeguiScreenBase::ButtonPressedHandler(const CEGUI::EventArgs& i_arguments)
    {
    const CEGUI::WindowEventArgs& window_arguments = static_cast<const CEGUI::WindowEventArgs&>(i_arguments);
    ControlData control_data;
    control_data.m_control = ControlType::CT_BUTTON;
    control_data.m_button_id = static_cast<ButtonID>(window_arguments.window->getID());
    const std::set<InputSubscriber*>& subscribers = m_screen_manager.GetInputManager().GetSubscribers();
    for (InputSubscriber* subscriber : subscribers)
      subscriber->UiEvent(control_data);

    return true;
    }

  } // UI