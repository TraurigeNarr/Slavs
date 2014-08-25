#include "stdafx.h"

#include "GameScreenMain.h"

#include "ScreenManager.h"
#include "ButtonID.h"

#include "GameStateMessageProvider.h"

//////////////////////////////////////////////////////////////////////////

namespace UI
  {

  GameScreenMain::GameScreenMain(ScreenManager& i_screen_manager)
    : CeguiScreenBase(i_screen_manager)
    {

    }

  GameScreenMain::~GameScreenMain()
    {    }

  void GameScreenMain::Initialize()
    {
    CEGUI::WindowManager& wmgr = CEGUI::WindowManager::getSingleton();
    mp_root_window = wmgr.loadLayoutFromFile("SlavsGame.layout");
    CEGUI::System::getSingleton().getDefaultGUIContext().setRootWindow(mp_root_window);

    _TryAttachButtonHandler(*mp_root_window, "Menu", static_cast<CEGUI::uint>(ButtonID::BI_BACK_TO_MENU));
    _TryAttachButtonHandler(*mp_root_window, "Help", static_cast<CEGUI::uint>(ButtonID::BI_HELP));

    CEGUI::System::getSingleton().getDefaultGUIContext().getMouseCursor().show();
    }

  void GameScreenMain::Update(long i_elapsed_time)
    {
    auto p_message_provider = GetMessageProvider<ClientStates::GameStateMessageProvider>();
    if (p_message_provider == nullptr || p_message_provider->IsValid())
      return;

    p_message_provider->Validate();
    }

  } // UI