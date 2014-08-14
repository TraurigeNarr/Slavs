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
    CEGUI::System::getSingleton().getDefaultGUIContext().getMouseCursor().setImage("TaharezLook/MouseArrow");
    CEGUI::WindowManager &wmgr = CEGUI::WindowManager::getSingleton();
    mp_root_window = wmgr.createWindow("DefaultWindow", "MenuScreen");

    CEGUI::Window* p_exit_button = wmgr.createWindow("TaharezLook/Button");
    p_exit_button->setSize(CEGUI::USize(CEGUI::UDim(0.15f, 0), CEGUI::UDim(0.05f, 0)));
    p_exit_button->setPosition(CEGUI::UVector2(CEGUI::UDim(0.1f/*relational to screen*/, 0/*pixel mode*/), 
                                CEGUI::UDim(0.1f,0)));
    p_exit_button->setText("Exit");
    p_exit_button->setID(static_cast<CEGUI::uint>(ButtonID::BI_EXIT));
    p_exit_button->subscribeEvent(CEGUI::PushButton::EventClicked,
      CEGUI::Event::Subscriber(&CeguiScreenBase::ButtonPressedHandler, static_cast<CeguiScreenBase*>(this)));

    mp_root_window->addChild(p_exit_button);

    CEGUI::System::getSingleton().getDefaultGUIContext().setRootWindow(mp_root_window);
    }

  } // UI