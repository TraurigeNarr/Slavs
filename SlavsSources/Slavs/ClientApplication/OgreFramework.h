#pragma once

#include <OgreCommon.h>

#include <OgreWindowEventUtilities.h>

namespace Ogre
  {
  class Root;
  class RenderWindow;
  class Viewport;
  class Log;
  class InputManager;
  class Camera;
  class SceneManager;

  typedef uint16 SceneTypeMask;
  } // Ogre

namespace OIS
  {

  class InputManager;
  class Keyboard;
  class Mouse;

  } // OIS

typedef Ogre::NameValuePairList Options;

class OgreFramework : public Ogre::WindowEventListener
  {
  private:
    std::unique_ptr<Ogre::Root> mp_root;
    Ogre::RenderWindow*			    mp_render_window;
    Ogre::Viewport*				      mp_viewport;
    Ogre::Log*					        mp_log;

    OIS::InputManager*			    mp_input_manager;
    OIS::Keyboard*				      mp_keyboard;
    OIS::Mouse*					        mp_mouse;

    Ogre::SceneManager*			    mp_scene_manager;
    Ogre::Camera*               mp_camera;
    bool                        m_show_fps;

  private:
    void    _InitializeLogFile();
    void    _InitializeReources();
    void    _InitializeRenderSystem();
    void    _InitializeRenderWindow();
    void    _InitializeViewport();
    void    _InitializeInput();
    Options _DefaultOptions();
    void    _ClearCurrentScene();

  public:
    OgreFramework();
    ~OgreFramework();

    void Initialize();
    void Release();

    Ogre::SceneManager* CreateSceneManager(Ogre::SceneTypeMask i_mask, const std::string& i_name /* = "" */);
    Ogre::Camera*       CreateCamera(const std::string& i_name = "");

    Ogre::Root*         GetRoot();
    OIS::Keyboard*      GetKeyboard();
    OIS::Mouse*         GetMouse();
    Ogre::Viewport*     GetViewport();
    Ogre::RenderTarget* GetRenderTarget();
    Ogre::SceneManager* GetSceneManager();
    Ogre::Camera*       GetCamera();

  // Ogre::WindowEventListener
  public:
    virtual void windowResized(Ogre::RenderWindow* rw) override;
    virtual bool windowClosing(Ogre::RenderWindow* rw) override;
  };

//////////////////////////////////////////////////////////////////////////

inline Ogre::Root* OgreFramework::GetRoot()
  {
  return mp_root.get();
  }

inline OIS::Keyboard* OgreFramework::GetKeyboard()
  {
  return mp_keyboard;
  }

inline OIS::Mouse* OgreFramework::GetMouse()
  {
  return mp_mouse;
  }

inline Ogre::Viewport* OgreFramework::GetViewport()
  {
  return mp_viewport;
  }

inline Ogre::SceneManager* OgreFramework::GetSceneManager()
  {
  return mp_scene_manager;
  }

inline Ogre::Camera* OgreFramework::GetCamera()
  {
  return mp_camera;
  }