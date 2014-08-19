#pragma once

#include "CeguiScreenBase.h"

namespace UI
  {

  class LoadingScreen : public CeguiScreenBase
    {
    private:
      virtual void Initialize() override;

    public:
      LoadingScreen(ScreenManager& i_screen_manager);
      ~LoadingScreen();
    };

  } // UI