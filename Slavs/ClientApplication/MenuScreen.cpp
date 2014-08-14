#include "stdafx.h"

#include "MenuScreen.h"

#include "ScreenManager.h"
#include "ButtoID.h"

//////////////////////////////////////////////////////////////////////////

namespace UI
  {

  MenuScreen::MenuScreen(ScreenManager& i_screen_manager)
    : CeguiScreenBase(i_screen_manager)
    {

    }

  MenuScreen::~MenuScreen()
    {    }

  void MenuScreen::Initialize()
    {
    CEGUI::WindowManager& wmgr = CEGUI::WindowManager::getSingleton();
    mp_root_window = wmgr.loadLayoutFromFile("SlavsMenu.layout");
    CEGUI::System::getSingleton().getDefaultGUIContext().setRootWindow(mp_root_window);

    CEGUI::Window* p_exit_button = mp_root_window->getChild("Exit");
    if (p_exit_button != nullptr)
      {
      p_exit_button->setID(static_cast<CEGUI::uint>(ButtonID::BI_EXIT));
      p_exit_button->subscribeEvent(CEGUI::PushButton::EventClicked,
                        CEGUI::Event::Subscriber(&CeguiScreenBase::ButtonPressedHandler, static_cast<CeguiScreenBase*>(this)));
      }

    CEGUI::System::getSingleton().getDefaultGUIContext().getMouseCursor().setImage("TaharezLook/MouseArrow");
    }

  } // UI