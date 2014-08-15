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

    _TryAttachButtonHandler(*mp_root_window, "Connect", static_cast<CEGUI::uint>(ButtonID::BI_CONNECT));
    _TryAttachButtonHandler(*mp_root_window, "Options", static_cast<CEGUI::uint>(ButtonID::BI_OPTIONS));
    _TryAttachButtonHandler(*mp_root_window, "Exit", static_cast<CEGUI::uint>(ButtonID::BI_EXIT));
    
    CEGUI::System::getSingleton().getDefaultGUIContext().getMouseCursor().setImage("TaharezLook/MouseArrow");
    }

  } // UI