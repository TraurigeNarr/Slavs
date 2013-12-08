#ifndef OGREFramework_h
#define OGREFramework_h

#pragma warning( disable : 4251 )
#pragma warning( disable : 4193 )
#pragma warning( disable : 4275 )

#include <OgreConfigFile.h>
#include <OgreLogManager.h>
#include <Overlay\OgreOverlay.h>
#include <Overlay\OgreOverlayElement.h>
#include <Overlay\OgreOverlayManager.h>
#include <OgreRenderWindow.h>
#include <OgreRoot.h>
#include <OgreSceneManager.h>
#include <OgreViewport.h>

#include <OISEvents.h>
#include <OISInputManager.h>
#include <OISKeyboard.h>
#include <OISMouse.h>
#include <SdkTrays.h>


#include <vector>

class OgreFramework : public Ogre::WindowEventListener
{
public:
	OgreFramework();
	~OgreFramework();

	bool InitOgre(Ogre::String wndTitle, OIS::KeyListener *pKeyListener = NULL, OIS::MouseListener *pMouseListener = NULL);
  //creates new scene; if not null -> removes previous
  Ogre::SceneManager* CreateSceneManager(Ogre::SceneTypeMask i_type, const std::string& i_name = "");
  //creates new camera; if current camera is not null -> removes previous
  Ogre::Camera* CreateCamera(const std::string& i_name = "");
	bool RestoreConfig();
	bool RestoreConfig(const Ogre::String &fileName);

	bool KeyPressed(const OIS::KeyEvent &keyEventRef);
	bool KeyReleased(const OIS::KeyEvent &keyEventRef);

	bool MouseMoved(const OIS::MouseEvent &evt);
	bool MousePressed(const OIS::MouseEvent &evt, OIS::MouseButtonID id);
	bool MouseReleased(const OIS::MouseEvent &evt, OIS::MouseButtonID id);

  virtual void windowResized(Ogre::RenderWindow* rw) override;
  virtual bool windowClosing(Ogre::RenderWindow* rw) override;

	Ogre::RenderTarget& GetAppRenderTarget();

	Ogre::Root*					m_pRoot;
	Ogre::RenderWindow*			m_pRenderWnd;
	Ogre::Viewport*				m_pViewport;
	Ogre::Log*					m_pLog;
	Ogre::Timer*				m_pTimer;

	OIS::InputManager*			m_pInputMgr;
	OIS::Keyboard*				m_pKeyboard;
	OIS::Mouse*					m_pMouse;

	/*OgreBites::SdkTrayManager*	m_pTrayMgr;*/
	Ogre::SceneManager*			m_pSceneManager;
  Ogre::Camera*         mp_camera;
protected:
  void _ClearCurrentScene();
	bool LoadOptions();
	void SetupResources();
	void SetLogFile();
	int m_iWidth, m_iHeigh;
	Ogre::NameValuePairList m_Options;
	bool m_bShowFPS;
};

#endif