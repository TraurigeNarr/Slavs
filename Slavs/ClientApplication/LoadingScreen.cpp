#include "stdafx.h"

#include "LoadingScreen.h"

#include "ScreenManager.h"

//////////////////////////////////////////////////////////////////////////

namespace UI
  {

  LoadingScreen::LoadingScreen(ScreenManager& i_screen_manager)
    : CeguiScreenBase(i_screen_manager)
    {

    }

  LoadingScreen::~LoadingScreen()
    {    }

  void LoadingScreen::Initialize()
    {
    CEGUI::WindowManager& wmgr = CEGUI::WindowManager::getSingleton();
    CEGUI::ImageManager::getSingleton().loadImageset("SlavsLoadScreen.imageset");
    mp_root_window = wmgr.loadLayoutFromFile("SlavsLoad.layout");
    CEGUI::System::getSingleton().getDefaultGUIContext().setRootWindow(mp_root_window);

    CEGUI::System::getSingleton().getDefaultGUIContext().getMouseCursor().hide();
    CEGUI::System::getSingleton().getDefaultGUIContext().getMouseCursor().setImage("TaharezLook/MouseArrow");
    }

  } // UI