#include "stdafx.h"

#include "Application.h"

#include "CeguiEventListener.h"
#include "InputManager.h"
#include "InputSubscriber.h"
#include "MenuState.h"
#include "TimeUtilities.h"
#include "ScreenManager.h"
#include "UISettings.h"

#include <Common/Patterns/StateMachine.h>

//////////////////////////////////////////////////////////////////////////

namespace ClientGame
  {
  Application appInstance;
  }

//////////////////////////////////////////////////////////////////////////

Application::Application()
  {  }

Application::~Application()
  {  }

void Application::Start()
  {
  mp_ogre_framework.reset(new OgreFramework());
  mp_ogre_framework->Initialize();
  mp_ogre_framework->GetRoot()->addFrameListener(this);

  mp_cegui_framework.reset(new CEGUIFramework(*mp_ogre_framework));
  mp_cegui_framework->InitializeCEGUI();

  mp_input_manager.reset(new InputManager(*mp_ogre_framework));
  
  mp_input_manager->AddSubscriber(std::make_shared<Cegui::CeguiEventListener>());

  mp_screen_manager.reset(new UI::ScreenManager(*mp_input_manager));

  mp_state_machine.reset(new StateMachine<Application, long>(this));
  mp_state_machine->SetCurrentState(std::make_shared<ClientStates::MenuState>(*this));

  mp_ui_settings.reset(new UI::UISettings());

  m_shutdown = false;
  //start rendering
  mp_ogre_framework->GetRoot()->startRendering();
  }

void Application::Shutdown()
  {
  m_shutdown = true;
  }

void Application::Release()
  {
  if (mp_state_machine)
    mp_state_machine.reset();

  if (mp_screen_manager)
    {
    mp_screen_manager->ClearAllScreens();
    mp_screen_manager.reset();
    }

  if (mp_input_manager)
    mp_input_manager.reset();

  if (mp_cegui_framework)
    mp_cegui_framework.reset();

  if (mp_ogre_framework)
    {
    mp_ogre_framework->Release();
    mp_ogre_framework.reset();
    }
  }

bool Application::frameRenderingQueued(const Ogre::FrameEvent& evt)
  {
  if(m_shutdown)
    return false;

  mp_input_manager->FrameRenderingQueue(evt);

  long elapsed_time = TimeUtilities::ConvertTime(evt.timeSinceLastFrame);

  // 18 milliseconds -> 60 fps
  long sleep_time = 18 - elapsed_time;
  if(0 < sleep_time)
    Sleep(sleep_time);
  
  mp_screen_manager->Update(elapsed_time);
  mp_input_manager->Update(elapsed_time);
  mp_state_machine->Update(elapsed_time);
  return true;
  }