#pragma once

#include "CeguiScreenBase.h"

namespace UI
  {

  class MenuScreen : public CeguiScreenBase
    {
    private:
      virtual void Initialize() override;

    public:
      MenuScreen(ScreenManager& i_screen_manager);
      ~MenuScreen();
    };

  } // UI