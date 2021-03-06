#include "stdafx.h"

#include "CeguiScreenBase.h"
#include "CeguiUtilities.h"
#include "TimeUtilities.h"

#include "ControlData.h"
#include "InputManager.h"
#include "ScreenManager.h"
#include "InputSubscriber.h"

#include "UiEvents.h"

#include <Patterns/MessageDispatcher/MessageDispatcher.h>

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

  void CeguiScreenBase::_TryAttachButtonHandler(const CEGUI::Window& i_window, const CEGUI::String& i_name_path, CEGUI::uint i_id)
    {
    _TryAttachButtonHandler(i_window, i_name_path, i_id, false);
    }

  void CeguiScreenBase::_TryAttachButtonHandler(const CEGUI::Window& i_window, const CEGUI::String& i_name_path, CEGUI::uint i_id, bool i_command_button)
    {
    try
      {
      // Connect
      CEGUI::Window* p_connect_button = i_window.getChild(i_name_path);
      if (p_connect_button != nullptr)
        {
        p_connect_button->setID(i_id);
        if (i_command_button)
          {
          p_connect_button->subscribeEvent(CEGUI::PushButton::EventClicked,
            CEGUI::Event::Subscriber(&CeguiScreenBase::CommandButtonPressedHandler, static_cast<CeguiScreenBase*>(this)));
          }
        else
          {
          p_connect_button->subscribeEvent(CEGUI::PushButton::EventClicked,
            CEGUI::Event::Subscriber(&CeguiScreenBase::ButtonPressedHandler, static_cast<CeguiScreenBase*>(this)));
          }
        }
      }
    catch (CEGUI::UnknownObjectException&)
      {
      assert(false);
      }
    }

  bool CeguiScreenBase::ButtonPressedHandler(const CEGUI::EventArgs& i_arguments)
    {
    const CEGUI::WindowEventArgs& window_arguments = static_cast<const CEGUI::WindowEventArgs&>(i_arguments);
    ButtonID button_id = static_cast<ButtonID>(window_arguments.window->getID());
    m_screen_manager.GetMessageDispatcher().HandleMessage(ButtonPressed(button_id));
    return true;
    }

  bool CeguiScreenBase::CommandButtonPressedHandler(const CEGUI::EventArgs& i_arguments)
    {
    const CEGUI::WindowEventArgs& window_arguments = static_cast<const CEGUI::WindowEventArgs&>(i_arguments);
    int command_id = window_arguments.window->getID();
    m_screen_manager.GetMessageDispatcher().HandleMessage(CommandButtonPressed(command_id));
    return true;
    }

  } // UI