#pragma once

#include "CeguiScreenBase.h"

namespace CEGUI
  {
  class DefaultWindow;
  class ProgressBar;
  } // CEGUI


namespace UI
  {

  class LoadingScreen : public CeguiScreenBase
    {
    private:
      CEGUI::ProgressBar*   mp_general_progress;
      CEGUI::ProgressBar*   mp_current_stage_progress;
      CEGUI::DefaultWindow* mp_information_label;

    private:
      virtual void Initialize() override;

    public:
      LoadingScreen(ScreenManager& i_screen_manager);
      ~LoadingScreen();

      virtual void Update (long i_elapsed_time) override;
    };

  } // UI