#include "stdafx.h"

#include "LoadingScreen.h"

#include "ScreenManager.h"

#include "LoadStateMessageProvider.h"

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
    
    // cache windows
    try
      {
      mp_current_stage_progress = static_cast<CEGUI::ProgressBar*>(mp_root_window->getChild("CurrentStageProgress"));
      mp_general_progress       = static_cast<CEGUI::ProgressBar*>(mp_root_window->getChild("GeneralProgress"));
      mp_information_label      = static_cast<CEGUI::DefaultWindow*>(mp_root_window->getChild("InformationLabel"));

      mp_general_progress->hide();
      mp_current_stage_progress->hide();
      }
    catch (CEGUI::UnknownObjectException&)
      {
      mp_general_progress       = nullptr;
      mp_current_stage_progress = nullptr;
      mp_information_label      = nullptr;
      }
    }

  void LoadingScreen::Update(long i_elapsed_time)
    {
    if (mp_general_progress == nullptr || mp_current_stage_progress == nullptr || mp_information_label == nullptr)
      return;

    auto p_message_provider = GetMessageProvider<ClientStates::LoadStateMessageProvider>();
    if (p_message_provider == nullptr || p_message_provider->IsValid())
      return;

    mp_information_label->setText(p_message_provider->GetCurrentStage());

    if (!mp_general_progress->isVisible())
      {
      if (p_message_provider->GetCurrentProgress() != 0 || p_message_provider->GetGeneralProgress() != 0)
        {
        mp_general_progress->show();
        mp_current_stage_progress->show();
        }
      }
      
    mp_general_progress->setProgress(p_message_provider->GetGeneralProgress());
    mp_current_stage_progress->setProgress(p_message_provider->GetCurrentProgress());
    
    p_message_provider->Validate();
    }

  } // UI