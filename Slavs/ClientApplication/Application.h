#pragma once

#include "Types.h"

class OgreFramework;
class CEGUIFramework;
class InputManager;
class MonkeyScene;

namespace UI
  {
  class ScreenManager;
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

