#include "stdafx.h"
#include "MenuState.h"

#include "Application.h"
#include "InputManager.h"

#include "MenuScreen.h"
#include "ScreenManager.h"
#include "ButtoID.h"
#include "ControlData.h"

namespace ClientStates
  {

  MenuState::MenuState(Application& i_application)
    : ApplicationStateBase(i_application)
    {
    }

  MenuState::~MenuState()
    {
    }

  void MenuState::Enter(Application* ip_application)
    {
    // set scene manager
    m_application.GetOgreFramework().CreateSceneManager(Ogre::ST_GENERIC, "MenuState_SceneManager");//->setAmbientLight(Ogre::ColourValue(0.5f, 0.3f, 0.3f));

    // set ogre settings
    Ogre::Camera* p_camera = m_application.GetOgreFramework().CreateCamera("MonkeyCamera");
    m_application.GetOgreFramework().GetViewport()->setCamera(p_camera);

    // set screen
    using namespace UI;
    ScreenManager& screen_manager = ip_application->GetScreenManager();
    screen_manager.SetCurrentScreen(std::unique_ptr<Screen>(new MenuScreen(screen_manager)));
    }

  void MenuState::Execute(Application* ip_application, long i_elapsed_time)
    {
    
    }

  void MenuState::Exit(Application* ip_application)
    {

    }

  bool MenuState::KeyPressed(const OIS::KeyEvent& keyEventRef)
    {
    if (keyEventRef.key == OIS::KC_ESCAPE)
      {
      m_application.Shutdown();
      return true;
      }
    return false;
    }

  } // ClientStates