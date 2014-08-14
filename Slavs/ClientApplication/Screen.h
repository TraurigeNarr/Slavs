#pragma once

namespace UI
  {
  class   ScreenManager;
  struct  ControlData;

  class Screen
    {
    private:
      Screen*        mp_owner;

    protected:
      ScreenManager& m_screen_manager;

    private:
      virtual void  SetRootWindow(void* ip_window) = 0;
      virtual void* GetRootWindow() = 0;

      virtual void Initialize() = 0;

    public:
      Screen(ScreenManager& i_screen_manager, Screen* ip_owner = nullptr);
      virtual ~Screen();

      void         Create (Screen* ip_owner = nullptr);

      Screen*      GetOwner();

    public:
      virtual void Destroy() = 0;
      virtual void Update(long i_elapsed_time) {}

      virtual bool ControlChanged(const ControlData& i_control_data) {return false;}
    };

  //////////////////////////////////////////////////////////////////////////

  inline Screen* Screen::GetOwner()
    {
    return mp_owner;
    }

  } // UI