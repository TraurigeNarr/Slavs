#include "stdafx.h"

#include "GameCamera.h"

#include "OgreFramework.h"
#include "InputManager.h"
#include "InputSubscriber.h"

#include <SdkCameraMan.h>

#include <OIS/OISKeyboard.h>
#include <OIS/OISMouse.h>

//////////////////////////////////////////////////////////////////////////

namespace ClientGame
  {

  //////////////////////////////////////////////////////////////////////////
  // CameraImpl

  class Camera::CameraImpl : public InputSubscriber
    {
    private:
      OgreFramework&                            m_framework;
      InputManager&                             m_input_manager;
      std::unique_ptr<OgreBites::SdkCameraMan>  mp_camera_man;

    public:
      CameraImpl(OgreFramework& i_framework, InputManager& i_input_manager)
        : m_framework(i_framework)
        , m_input_manager(i_input_manager)
        {
        Ogre::Camera* p_camera = m_framework.GetCamera();
        p_camera->setPosition(Ogre::Vector3(-660, 1750, 2650));
        p_camera->setOrientation(Ogre::Quaternion(0.85f, -0.2f, -0.45f, -0.1f));
        p_camera->setNearClipDistance(5);
        
        mp_camera_man.reset(new OgreBites::SdkCameraMan(p_camera));
        mp_camera_man->setTopSpeed(1500.f);

        m_input_manager.AddSubscriber(this);
        }

      virtual ~CameraImpl()
        {
        m_input_manager.RemoveSubscriber(this);
        }

    // InputSubscriber
    public:
      virtual bool KeyPressed(const OIS::KeyEvent &keyEventRef) override 
        { 
        mp_camera_man->injectKeyDown(keyEventRef);
        if (keyEventRef.key == OIS::KeyCode::KC_T)
          mp_camera_man->setTopSpeed(mp_camera_man->getTopSpeed()+10);
        return true;
        }

      virtual bool KeyReleased(const OIS::KeyEvent &keyEventRef) override 
        {
        mp_camera_man->injectKeyUp(keyEventRef);
        return true;
        }

      virtual bool MouseMoved(const OIS::MouseEvent &evt) override 
        { 
        mp_camera_man->injectMouseMove(evt);
        return true;
        }
      virtual bool MousePressed(const OIS::MouseEvent &evt, OIS::MouseButtonID id) override 
        {
        mp_camera_man->injectMouseDown(evt, id);
        return true;
        }
      virtual bool MouseReleased(const OIS::MouseEvent &evt, OIS::MouseButtonID id) override 
        {
        mp_camera_man->injectMouseUp(evt, id);
        return true;
        }

      virtual void FrameRenderingQueue(const Ogre::FrameEvent& evt) override
        {
        mp_camera_man->frameRenderingQueued(evt);
        }
    };

  //////////////////////////////////////////////////////////////////////////
  // Camera

  Camera::Camera (OgreFramework& i_framework, InputManager& i_input_manager)
    : m_framework(i_framework)
    , m_input_manager(i_input_manager)
    {
    mp_impl.reset(new CameraImpl(i_framework, i_input_manager));
    }

  Camera::~Camera()
    {    }

  } // ClientGame