#include "stdafx.h"

#include "GameScreenMain.h"

#include "ScreenManager.h"
#include "ButtonID.h"

#include "GameStateMessageProvider.h"

#include "GovermentPanel.h"

//////////////////////////////////////////////////////////////////////////

namespace UI
  {

  GameScreenMain::GameScreenMain(ScreenManager& i_screen_manager)
    : CeguiScreenBase(i_screen_manager)
    {

    }

  GameScreenMain::~GameScreenMain()
    {
    for (ChildScreenPtr& p_screens : m_child_screens)
      p_screens->Destroy();
    }

  void GameScreenMain::Initialize()
    {
    CEGUI::ImageManager::getSingleton().loadImageset("SlavsGameScreen.imageset");

    CEGUI::WindowManager& wmgr = CEGUI::WindowManager::getSingleton();
    mp_root_window = wmgr.loadLayoutFromFile("SlavsGame.layout");
    CEGUI::System::getSingleton().getDefaultGUIContext().setRootWindow(mp_root_window);

    _TryAttachButtonHandler(*mp_root_window, "Menu", static_cast<CEGUI::uint>(ButtonID::BI_BACK_TO_MENU));
    _TryAttachButtonHandler(*mp_root_window, "Help", static_cast<CEGUI::uint>(ButtonID::BI_HELP));

    CEGUI::System::getSingleton().getDefaultGUIContext().getMouseCursor().show();

    //////////////////////////////////////////////////////////////////////////
    // initialize child screens
    m_child_screens.push_back(ChildScreenPtr(new GovermentPanel(m_screen_manager)));
    
    for (ChildScreenPtr& p_screen : m_child_screens)
      p_screen->Create (this);
    }

  void GameScreenMain::Update(long i_elapsed_time)
    {
    auto p_message_provider = GetMessageProvider<ClientStates::GameStateMessageProvider>();
    if (p_message_provider == nullptr || p_message_provider->IsValid())
      return;

    for (ChildScreenPtr& p_screen : m_child_screens)
      {
      p_screen->SetMessageProvider(p_message_provider);
      p_screen->Update (i_elapsed_time);
      }

    p_message_provider->Validate();
    }

  } // UI