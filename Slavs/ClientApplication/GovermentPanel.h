#pragma once

#include "CeguiScreenBase.h"

namespace CEGUI
  {
  class DefaultWindow;
  class ProgressBar;
  } // CEGUI


namespace UI
  {

  enum class ButtonID;

  class GovermentPanel : public CeguiScreenBase
    {
    private:
      std::map<ButtonID, CEGUI::Window*> m_plugin_buttons;

    private:
      virtual void Initialize() override;

    public:
      GovermentPanel(ScreenManager& i_screen_manager);
      ~GovermentPanel();

      virtual void Update (long i_elapsed_time) override;
    };

  } // UI