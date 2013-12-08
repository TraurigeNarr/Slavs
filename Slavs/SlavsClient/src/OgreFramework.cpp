#include "OgreFramework.h"
#include "Application.h"
#include "ClientUtilities.h"

#include <Utilities/FileUtilities.h>
#include <Utilities/XmlUtilities.h>

#include <Net.h>

#include <CEGUI/CEGUI.h>
#include <CEGUI/Size.h>
#include <CEGUI/System.h>

using namespace Ogre;

//--------------------------------------------------------------------------------------
const Ogre::String appName = "Slavs";
const Ogre::String configFileName = "client\\configs\\Options.xml";
const Ogre::String ResourcesCfg	  =	"client\\configs\\resources.cfg";
const Ogre::String PluginCfg      =	"client\\configs\\plugins.cfg";
const Ogre::String LanguageCfg    =	"client\\configs\\languages.csv";
const Ogre::String LogFileName    = "client\\slavslog.log";

OgreFramework::OgreFramework()
  : m_pSceneManager(nullptr),
    mp_camera(nullptr),
    m_pRenderWnd(nullptr)
{
	m_pRoot				= NULL;
	m_pRenderWnd		= NULL;
	m_pViewport			= NULL;
	m_pLog				= NULL;
	m_pTimer			= NULL;

	m_pInputMgr			= NULL;
	m_pKeyboard			= NULL;
	m_pMouse			= NULL;
/*	m_pTrayMgr          = NULL;*/

	m_bShowFPS = false;
}

OgreFramework::~OgreFramework()
{
	m_pLog->logMessage("Shutdown OGRE...");
	/*if(m_pTrayMgr)      
    delete m_pTrayMgr;*/
	if(m_pInputMgr)		
    OIS::InputManager::destroyInputSystem(m_pInputMgr);
  if(m_pSceneManager)
  {
    _ClearCurrentScene(); 
    m_pRoot->destroySceneManager(m_pSceneManager);
  }
	if(m_pRoot)
    delete m_pRoot;
  
}

Ogre::SceneManager* OgreFramework::CreateSceneManager(SceneTypeMask i_mask, const std::string& i_name /* = "" */)
{
  if(nullptr != m_pSceneManager)
  {
    _ClearCurrentScene(); 
    m_pRoot->destroySceneManager(m_pSceneManager);
  }

  m_pSceneManager = m_pRoot->createSceneManager(i_mask, i_name);

  /*Ogre::OverlaySystem* pOverlaySystem = new Ogre::OverlaySystem();
  m_pSceneManager->addRenderQueueListener(pOverlaySystem);

  OgreBites::InputContext input;
  input.mAccelerometer = NULL;
  input.mKeyboard = m_pKeyboard;
  input.mMouse = m_pMouse;
  input.mMultiTouch = NULL;

  if(m_pTrayMgr)      
    delete m_pTrayMgr;
  m_pTrayMgr = new OgreBites::SdkTrayManager("SlavsTrayMgr", m_pRenderWnd, input);
  m_pTrayMgr->hideCursor()*/;

  return m_pSceneManager;
}

void OgreFramework::_ClearCurrentScene()
{
  assert(nullptr != m_pSceneManager);

  m_pSceneManager->destroyAllCameras();
  m_pSceneManager->destroyAllEntities();
  m_pSceneManager->destroyAllStaticGeometry();
}

Ogre::Camera* OgreFramework::CreateCamera(const std::string& i_name /* = "" */)
{
  if(nullptr == m_pSceneManager)
    return nullptr;
  return mp_camera = m_pSceneManager->createCamera(i_name);
}

bool OgreFramework::InitOgre(Ogre::String wndTitle, OIS::KeyListener *pKeyListener /* = NULL */, OIS::MouseListener *pMouseListener /* = NULL */)
{
	m_pRoot = new Ogre::Root(FileUtilities::JoinPath(FileUtilities::GetApplicationDirectory(), PluginCfg));

	SetLogFile();

	SetupResources();

	if(!LoadOptions())
		RestoreConfig();

	m_pRoot->initialise(false);
	m_pRenderWnd = m_pRoot->createRenderWindow(appName, m_iWidth, m_iHeigh, false, &m_Options);

	m_pViewport = m_pRenderWnd->addViewport(0);
	m_pViewport->setBackgroundColour(ColourValue(0.5f, 0.5f, 0.5f, 1.0f));
	m_pViewport->setCamera(0);

	size_t hWnd = 0;
	OIS::ParamList paramList;
	m_pRenderWnd->getCustomAttribute("WINDOW", &hWnd);

	paramList.insert(OIS::ParamList::value_type("WINDOW", Ogre::StringConverter::toString(hWnd)));

	m_pInputMgr = OIS::InputManager::createInputSystem(paramList);

	m_pKeyboard = static_cast<OIS::Keyboard*>(m_pInputMgr->createInputObject(OIS::OISKeyboard, true));
	m_pMouse = static_cast<OIS::Mouse*>(m_pInputMgr->createInputObject(OIS::OISMouse, true));

	m_pMouse->getMouseState().height = m_pRenderWnd->getHeight();
	m_pMouse->getMouseState().width	 = m_pRenderWnd->getWidth();

	Ogre::TextureManager::getSingleton().setDefaultNumMipmaps(5);
	Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();

	m_pTimer = new Ogre::Timer();
	m_pTimer->reset();

	m_pRenderWnd->setActive(true);

  Ogre::WindowEventUtilities::addWindowEventListener(m_pRenderWnd, this);

	return true;
}

bool OgreFramework::RestoreConfig()
{
  Options options = ::Singleton<Application>::GetInstance().GetOptions();

  options.SetOption("fullscreen", "");
  options.SetOption("resolution", "1024x768");
  options.SetOption("vsync", "false");
  options.SetServerAddress(net::Address(127,0,0,1,net::ServerPort));

	m_Options["fullscreen"] = "false";
	m_Options["resolution"] = "1024x768";
	m_Options["vsync"] = "false";
	return false;
}

/************************************************************************/
/*                       Event handlers                                 */
/************************************************************************/

bool OgreFramework::KeyPressed(const OIS::KeyEvent &keyEventRef)
{
#ifdef _DEBUG
/*
	if(m_pKeyboard->isKeyDown(OIS::KC_O))
	{
		if(!m_bShowFPS)
		{
			m_pTrayMgr->showFrameStats(OgreBites::TL_BOTTOMLEFT);
		}
		else
		{
			m_pTrayMgr->hideFrameStats();
		}
	}*/
#endif
  CEGUI::GUIContext& context = CEGUI::System::getSingleton().getDefaultGUIContext();
  context.injectKeyDown((CEGUI::Key::Scan)keyEventRef.key);
  context.injectChar((CEGUI::Key::Scan)keyEventRef.text);
  return true;
	return true;
}

bool OgreFramework::KeyReleased(const OIS::KeyEvent &keyEventRef)
{
#ifdef _DEBUG
	/*static bool showFrameStats = false;
	if(keyEventRef.key == OIS::KC_O)
	{
		if(showFrameStats)
		{
			m_pTrayMgr->hideFrameStats();
			showFrameStats = false;
		}
		else
		{
			m_pTrayMgr->showFrameStats(OgreBites::TL_BOTTOMLEFT);
			showFrameStats = true;
		}
	}*/
#endif

	CEGUI::System::getSingleton().getDefaultGUIContext().injectKeyUp((CEGUI::Key::Scan)keyEventRef.key);
	CEGUI::System::getSingleton().getDefaultGUIContext().injectChar(keyEventRef.text);
	return true;
}

bool OgreFramework::MouseMoved(const OIS::MouseEvent &evt)
{
  CEGUI::GUIContext& context = CEGUI::System::getSingleton().getDefaultGUIContext();
  context.injectMouseMove(evt.state.X.rel, evt.state.Y.rel);
  // Scroll wheel.
  if (evt.state.Z.rel)
    context.injectMouseWheelChange(evt.state.Z.rel / 120.0f);
	return true;
}

bool OgreFramework::MousePressed(const OIS::MouseEvent &evt, OIS::MouseButtonID id)
{
  CEGUI::GUIContext& context = CEGUI::System::getSingleton().getDefaultGUIContext();
	context.injectMouseButtonDown(ClientUtilities::ConvertButton(id));
	return true;
}

bool OgreFramework::MouseReleased(const OIS::MouseEvent &evt, OIS::MouseButtonID id)
{
  CEGUI::GUIContext& context = CEGUI::System::getSingleton().getDefaultGUIContext();
	context.injectMouseButtonUp(ClientUtilities::ConvertButton(id));
	return true;
}

bool OgreFramework::LoadOptions()
{
	RenderSystemList::const_iterator r_it = m_pRoot->getAvailableRenderers().begin();
	RenderSystem *renderSystem = *r_it;

	if(NULL == renderSystem) 
	{
		return false;
	}

	Ogre::String elementName;

	//open document
	TiXmlDocument document;
	if(!XmlUtilities::LoadXmlDocument(FileUtilities::JoinPath(FileUtilities::GetApplicationDirectory(), configFileName), document))
		return false;

	const TiXmlElement* childElement = 0;
  Options& options = ::Singleton<Application>::GetInstance().GetOptions();
	while ((childElement = XmlUtilities::IterateChildElements(document.RootElement(), childElement)))
	{
		elementName = childElement->Value();
		if("fullscreen" == elementName)
    {
			m_Options["fullscreen"] = XmlUtilities::GetStringAttribute(childElement, "value", "false");
      options.SetOption("fullscreen", XmlUtilities::GetStringAttribute(childElement, "value", "false"));
    }
		if("resolution" == elementName)
    {
			m_Options["resolution"] = XmlUtilities::GetStringAttribute(childElement, "value", "1024x768");
      options.SetOption("resolution", XmlUtilities::GetStringAttribute(childElement, "value", "1024x768"));
    }
		if("vsync" == elementName)
    {
			m_Options["vsync"] = XmlUtilities::GetStringAttribute(childElement, "value", "false");
      options.SetOption("vsync", XmlUtilities::GetStringAttribute(childElement, "value", "false"));
    }
    if("server" == elementName)
    {
      int a = XmlUtilities::GetIntAttribute(childElement, "a", 127);
      int b = XmlUtilities::GetIntAttribute(childElement, "b", 0);
      int c = XmlUtilities::GetIntAttribute(childElement, "c", 0);
      int d = XmlUtilities::GetIntAttribute(childElement, "d", 1);
      int port = XmlUtilities::GetIntAttribute(childElement, "port", net::ServerPort);

      options.SetServerAddress(net::Address(a, b, c, d, port));

      m_Options["server"] = XmlUtilities::GetStringAttribute(childElement, "value", "");
    }
	}
	Root::getSingleton().setRenderSystem(*r_it);		
	m_iWidth = 1024;
	m_iHeigh = 768;
	return true;
}

void OgreFramework::SetupResources()
{
	// Load resource paths from config file
	Ogre::ConfigFile cf;
	cf.load(FileUtilities::JoinPath(FileUtilities::GetApplicationDirectory(), ResourcesCfg));

	// Go through all sections & settings in the file
	Ogre::ConfigFile::SectionIterator seci = cf.getSectionIterator();

	Ogre::String secName, typeName, archName;
	while (seci.hasMoreElements())
	{
		secName = seci.peekNextKey();
		Ogre::ConfigFile::SettingsMultiMap *settings = seci.getNext();
		Ogre::ConfigFile::SettingsMultiMap::iterator i;
		for (i = settings->begin(); i != settings->end(); ++i)
		{
			typeName = i->first;
			archName = FileUtilities::JoinPath(FileUtilities::GetApplicationDirectory(), i->second);
			Ogre::ResourceGroupManager::getSingleton().addResourceLocation(
				archName, typeName, secName);
		}
	}
}

void OgreFramework::SetLogFile()
{
	m_pLog = Ogre::LogManager::getSingleton().createLog(FileUtilities::JoinPath(FileUtilities::GetApplicationDirectory(), LogFileName), true, true, false);
	m_pLog->setDebugOutputEnabled(true);
}

Ogre::RenderTarget& OgreFramework::GetAppRenderTarget()
{
	return *m_pRenderWnd;
}

void OgreFramework::windowResized(Ogre::RenderWindow* rw)
{
  m_pRenderWnd->resize(rw->getWidth(), rw->getHeight());
  mp_camera->setAspectRatio(Ogre::Real(rw->getWidth()) / Ogre::Real(rw->getHeight()));
  m_pMouse->getMouseState().height = m_pRenderWnd->getHeight();
  m_pMouse->getMouseState().width	 = m_pRenderWnd->getWidth();

  CEGUI::Sizef new_size(rw->getWidth(), rw->getHeight());
  CEGUI::GUIContext& context = CEGUI::System::getSingleton().getDefaultGUIContext();

  CEGUI::System::getSingleton().notifyDisplaySizeChanged(new_size);
  context.getMouseCursor().notifyDisplaySizeChanged(new_size);
  

  for (unsigned short i = 0; i < m_pRenderWnd->getNumViewports(); ++i)
    m_pRenderWnd->getViewport(i)->update();
}

bool OgreFramework::windowClosing(Ogre::RenderWindow* rw)
{
  Ogre::WindowEventUtilities::removeWindowEventListener(m_pRenderWnd, this);

  assert(nullptr != m_pRoot);

  if (nullptr != m_pRenderWnd)
  {
    m_pRoot->destroyRenderTarget(m_pRenderWnd);
    m_pRenderWnd = nullptr;
  }
  if (nullptr != m_pSceneManager)
  {
    m_pRoot->destroySceneManager(m_pSceneManager);
    m_pSceneManager = nullptr;
  }

  exit(0);

  return true;
}