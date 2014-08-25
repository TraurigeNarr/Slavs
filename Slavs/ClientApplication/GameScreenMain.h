#pragma once

#include "CeguiScreenBase.h"

namespace CEGUI
  {
  class DefaultWindow;
  class ProgressBar;
  } // CEGUI


namespace UI
  {

  class GameScreenMain : public CeguiScreenBase
    {
    private:
      virtual void Initialize() override;

    public:
      GameScreenMain(ScreenManager& i_screen_manager);
      ~GameScreenMain();

      virtual void Update (long i_elapsed_time) override;
    };

  } // UI