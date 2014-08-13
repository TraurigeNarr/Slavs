#pragma once

class OgreFramework;
class CEGUIFramework;
class InputManager;
class MonkeyScene;

class Application : public Ogre::FrameListener
  {
  private:
    std::unique_ptr<OgreFramework>  mp_ogre_framework;
    std::unique_ptr<CEGUIFramework> mp_cegui_framework;
    std::unique_ptr<InputManager>   mp_input_manager;
    bool                            m_shutdown;
  
  // Ogre::FrameListener
  protected:
    virtual bool frameRenderingQueued(const Ogre::FrameEvent& evt);

  public:
    Application();
    ~Application();

    void Start();
    void Shutdown();
    void Release();

    OgreFramework&  GetOgreFramework();
    CEGUIFramework& GetCeguiFramework();
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