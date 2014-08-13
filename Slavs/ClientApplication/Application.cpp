#include "stdafx.h"

#include "Application.h"
#include "InputManager.h"
#include "InputSubscriber.h"

#include "CeguiEventListener.h"

namespace Test
  {
  class DefaultInputListener : public InputSubscriber
    {
    private:
      Application& m_application;
    public:
      DefaultInputListener(Application& i_application)
        : m_application(i_application)
        {        }

      virtual bool KeyReleased(const OIS::KeyEvent &keyEventRef) override
        {
        if (keyEventRef.key == OIS::KC_ESCAPE)
          {
          m_application.Shutdown();
          return true;
          }
        return false;
        }
    };

  } // namespace

//////////////////////////////////////////////////////////////////////////

Application::Application()
  {  }

Application::~Application()
  {  }

void Application::Start()
  {
  mp_ogre_framework.reset(new OgreFramework());
  mp_ogre_framework->Initialize();

  mp_cegui_framework.reset(new CEGUIFramework(*mp_ogre_framework));
  mp_cegui_framework->InitializeCEGUI();

  mp_input_manager.reset(new InputManager(*mp_ogre_framework));
  mp_ogre_framework->GetRoot()->addFrameListener(this);
  
  mp_input_manager->AddSubscriber(std::make_shared<Test::DefaultInputListener>(*this));
  mp_input_manager->AddSubscriber(std::make_shared<Cegui::CeguiEventListener>());

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
  if (mp_input_manager)
    mp_input_manager.reset();

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

  float sleepTime = 0.018f - evt.timeSinceLastFrame;
  sleepTime *= 1000.0f;
  if(0 < sleepTime)
    Sleep(static_cast<DWORD>(sleepTime));

  mp_input_manager->Update(evt.timeSinceLastFrame);
  return true;
  }