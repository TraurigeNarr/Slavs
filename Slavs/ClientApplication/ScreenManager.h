#pragma once

class InputManager;

namespace UI
  {

  class Screen;

  class ScreenManager
    {
    private:
      typedef std::unique_ptr<Screen> ScreenPtr;
    private:
      InputManager& m_input_manager;

      std::vector<ScreenPtr> m_screen_stack;
      Screen*                mp_current_screen;

    private:
      void _PushToStack(std::unique_ptr<Screen> ip_screen, Screen* ip_owner);

    public:
      ScreenManager (InputManager& i_input_manager);
      virtual ~ScreenManager ();

      void          Update (long i_elapsed_time);

      InputManager& GetInputManager ();

      /// Removes previous screen from stack and set ip_screen as current
      void SetCurrentScreen (std::unique_ptr<Screen> ip_screen);
      /// Do not remove previous screen from stack. Set owner of ip_screen last
      ///  screen on stack
      void AddScreenToStack (std::unique_ptr<Screen> ip_screen);

      void ClearAllScreens ();
      void RemoveScreenFromStack ();

      Screen* GetCurrentScreen();
    };

  //////////////////////////////////////////////////////////////////////////

  inline InputManager& ScreenManager::GetInputManager()
    {
    return m_input_manager;
    }

  inline Screen* ScreenManager::GetCurrentScreen()
    {
    return mp_current_screen;
    }

  }