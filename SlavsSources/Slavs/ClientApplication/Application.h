#pragma once

#include "Types.h"

class OgreFramework;
class CEGUIFramework;
class InputManager;

namespace GameCore { namespace UI {
  class MouseManager;
    } // UI
  } // GameCore

namespace UI
  {
  class ScreenManager;
  class UISettings;
  }

class Application : public Ogre::FrameListener
  {
  private:
    std::unique_ptr<OgreFramework>            mp_ogre_framework;
    std::unique_ptr<CEGUIFramework>           mp_cegui_framework;
    std::unique_ptr<InputManager>             mp_input_manager;
    bool                                      m_shutdown;

    std::unique_ptr<ApplicationStateMachine>  mp_state_machine;
    std::unique_ptr<UI::ScreenManager>        mp_screen_manager;
    std::unique_ptr<UI::UISettings>           mp_ui_settings;

    std::unique_ptr<GameCore::UI::MouseManager> mp_mouse_manager;

  // Ogre::FrameListener
  protected:
    virtual bool frameRenderingQueued(const Ogre::FrameEvent& evt);

  public:
    Application();
    ~Application();

    void                      Start();
    void                      Shutdown();
    void                      Release();

    OgreFramework&            GetOgreFramework();
    CEGUIFramework&           GetCeguiFramework();
    InputManager&             GetInputManager();
    ApplicationStateMachine&  GetStateMachine();
    UI::ScreenManager&        GetScreenManager();
    UI::UISettings&           GetUISettings();
    GameCore::UI::MouseManager& GetMouseManager();
  };

//////////////////////////////////////////////////////////////////////////

inline OgreFramework& Application::GetOgreFramework()
  {
  return *mp_ogre_framework;
  }

inline CEGUIFramework& Application::GetCeguiFramework()
  {
  return *mp_cegui_framework;
  }

inline InputManager& Application::GetInputManager()
  {
  return *mp_input_manager;
  }

inline ApplicationStateMachine& Application::GetStateMachine()
  {
  return *mp_state_machine;
  }

inline UI::ScreenManager& Application::GetScreenManager()
  {
  return *mp_screen_manager;
  }

inline UI::UISettings& Application::GetUISettings()
  {
  return *mp_ui_settings;
  }

inline GameCore::UI::MouseManager& Application::GetMouseManager()
  {
  return *mp_mouse_manager;
  }

namespace ClientGame
  {
  extern Application appInstance;
  }