#pragma once

#include "Screen.h"

namespace CEGUI
  {
  class Window;
  class EventArgs;
  }

namespace UI
  {

  class CeguiScreenBase : public Screen
    {
    protected:
      CEGUI::Window*	mp_root_window;
     
    private:
      virtual void  SetRootWindow(void* ip_window) override;
      virtual void* GetRootWindow() override;

    protected:
      void _TryAttachButtonHandler(const CEGUI::Window& i_window, const CEGUI::String& i_string, CEGUI::uint i_id);

    public:
      CeguiScreenBase (ScreenManager& i_screen_manager);
      virtual ~CeguiScreenBase ();

      virtual void Update (long i_elapsed_time) override;
      virtual void Destroy () override;

    public:
      virtual bool ButtonPressedHandler(const CEGUI::EventArgs& i_arguments);
    };

  } // UI