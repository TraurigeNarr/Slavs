#include "stdafx.h"

#include "OgreFramework.h"

#pragma warning (push)
# pragma warning( disable : 4251 )
# pragma warning( disable : 4193 )
# pragma warning( disable : 4275 )

# include <OgreConfigFile.h>
# include <OgreLogManager.h>
# include <Overlay\OgreOverlay.h>
# include <Overlay\OgreOverlayElement.h>
# include <Overlay\OgreOverlayManager.h>
# include <OgreRenderWindow.h>
# include <OgreRoot.h>
# include <OgreSceneManager.h>
# include <OgreViewport.h>

# include <OIS/OISEvents.h>
# include <OIS/OISInputManager.h>
# include <OIS/OISKeyboard.h>
# include <OIS/OISMouse.h>
# include <SdkTrays.h>
#pragma warning (pop)

#include <CEGUI/System.h>
#include <CEGUI/Size.h>
#include <CEGUI/GUIContext.h>

namespace
  {

  const std::string PluginsPath    = ".\\configs\\plugins.cfg";
  const std::string ResourcesPath  = ".\\configs\\resources.cfg";
  const std::string LogPath        = ".\\OgreLog.log";
  const std::string WindowName     = "Slavs";
  
  const size_t  Width = 1024;
  const size_t  Height = 768;

  } // namespace

//////////////////////////////////////////////////////////////////////////

OgreFramework::OgreFramework()
  : mp_scene_manager(nullptr)
  , mp_render_window(nullptr)
  , mp_viewport(nullptr)
  , mp_log(nullptr)
  , mp_input_manager(nullptr)
  , mp_keyboard(nullptr)
  , mp_mouse(nullptr)
  , mp_camera(nullptr)
  , m_show_fps(false)
  {

  }

OgreFramework::~OgreFramework()
  {  }

//////////////////////////////////////////////////////////////////////////
// Initialization block

void OgreFramework::_InitializeLogFile()
  {
  mp_log = Ogre::LogManager::getSingleton().createLog(LogPath, true, false, false);
#ifdef _DEBUG
  mp_log->setDebugOutputEnabled(true);
#endif
  }

void OgreFramework::_InitializeReources()
  {
  // Load resource paths from config file
  Ogre::ConfigFile cf;
  cf.load(ResourcesPath);

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
      archName = i->second;
      Ogre::ResourceGroupManager::getSingleton().addResourceLocation(
        archName, typeName, secName, true);
      }
    }
  Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();
  }

void OgreFramework::_InitializeRenderSystem()
  {
  /*if (mp_root->showConfigDialog())
    return;*/
  Ogre::RenderSystemList::const_iterator r_it = mp_root->getAvailableRenderers().begin();

  // will throw if render system was not selected
  if(r_it == mp_root->getAvailableRenderers().end()) 
    return;

  mp_root->setRenderSystem(*r_it);
  }

void OgreFramework::_InitializeRenderWindow()
  {
  mp_render_window = mp_root->createRenderWindow(
                            WindowName,
                            Width,
                            Height,
                            false,
                            new Options(_DefaultOptions())
                            );
  }

void OgreFramework::_InitializeViewport()
  {
  mp_viewport = mp_render_window->addViewport(nullptr);
  mp_viewport->setBackgroundColour(Ogre::ColourValue(.5f, .5f, .5f, 1.f));
  }

void OgreFramework::_InitializeInput()
  {
  size_t hWnd = 0;
  OIS::ParamList paramList;
  mp_render_window->getCustomAttribute("WINDOW", &hWnd);
  paramList.insert(OIS::ParamList::value_type("WINDOW", Ogre::StringConverter::toString(hWnd)));
  mp_input_manager = OIS::InputManager::createInputSystem(paramList);

  mp_keyboard = static_cast<OIS::Keyboard*>(mp_input_manager->createInputObject(OIS::OISKeyboard, true));

  mp_mouse = static_cast<OIS::Mouse*>(mp_input_manager->createInputObject(OIS::OISMouse, true));
  mp_mouse->getMouseState().height  = mp_render_window->getHeight();
  mp_mouse->getMouseState().width   = mp_render_window->getWidth();
  }

Options OgreFramework::_DefaultOptions()
  {
  Options options;
  options["fullscreen"] = "false";
  options["resolution"] = "1024x768";
  options["vsync"]      = "false";
  return options;
  }

void OgreFramework::_ClearCurrentScene()
  {
  if (nullptr != mp_scene_manager)
    {
    mp_scene_manager->destroyAllCameras();
    mp_scene_manager->destroyAllEntities();
    mp_scene_manager->destroyAllStaticGeometry();
    mp_root->destroySceneManager(mp_scene_manager);
    mp_scene_manager = nullptr;
    }
  }

//////////////////////////////////////////////////////////////////////////

void OgreFramework::Initialize()
  {
  // 1. Create root object
  mp_root.reset(new Ogre::Root(PluginsPath));
  _InitializeRenderSystem();
  //  we do not need auto initialization of window
  mp_root->initialise(false);
  
  _InitializeLogFile();
  _InitializeReources();
  _InitializeRenderWindow();
  _InitializeInput();
  _InitializeViewport();
  
  mp_render_window->setActive(true);
  Ogre::WindowEventUtilities::addWindowEventListener(mp_render_window, this);
  Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();
  }

void OgreFramework::Release()
  {
  Ogre::WindowEventUtilities::removeWindowEventListener(mp_render_window, this);

  assert(nullptr != mp_root);

  if (nullptr != mp_input_manager)
    {
    if (nullptr != mp_keyboard)
      {
      mp_input_manager->destroyInputObject(mp_keyboard);
      mp_keyboard = nullptr;
      }

    if (nullptr != mp_mouse)
      {
      mp_input_manager->destroyInputObject(mp_mouse);
      mp_mouse = nullptr;
      }

    OIS::InputManager::destroyInputSystem(mp_input_manager);
    mp_input_manager = nullptr;
    }

   _ClearCurrentScene();

  if (nullptr != mp_render_window)
    {
    mp_render_window->removeAllViewports();
    mp_root->destroyRenderTarget(mp_render_window);
    mp_render_window = nullptr;
    }
  }

Ogre::SceneManager* OgreFramework::CreateSceneManager(Ogre::SceneTypeMask i_mask, const std::string& i_name )
  {
  _ClearCurrentScene();

  mp_scene_manager = mp_root->createSceneManager(i_mask, i_name);

  return mp_scene_manager;
  }

Ogre::Camera* OgreFramework::CreateCamera(const std::string& i_name /* = "" */)
  {
  if(nullptr == mp_scene_manager)
    return nullptr;
  return mp_camera = mp_scene_manager->createCamera(i_name);
  }

void OgreFramework::windowResized(Ogre::RenderWindow* rw)
  {
  mp_render_window->resize(rw->getWidth(), rw->getHeight());
  mp_mouse->getMouseState().height = mp_render_window->getHeight();
  mp_mouse->getMouseState().width	 = mp_render_window->getWidth();

  if (mp_camera)
    mp_camera->setAspectRatio(Ogre::Real(rw->getWidth()) / Ogre::Real(rw->getHeight()));

  // this should be in CEGUI framework so OGREFramework do not depend on something
  // but it is not critical
  CEGUI::Sizef new_size(static_cast<float>(rw->getWidth()), static_cast<float>(rw->getHeight()));
  CEGUI::GUIContext& context = CEGUI::System::getSingleton().getDefaultGUIContext();

  CEGUI::System::getSingleton().notifyDisplaySizeChanged(new_size);
  context.getMouseCursor().notifyDisplaySizeChanged(new_size);

  for (unsigned short i = 0; i < mp_render_window->getNumViewports(); ++i)
    mp_render_window->getViewport(i)->update();
  }

bool OgreFramework::windowClosing(Ogre::RenderWindow* rw)
  {
  return false;
  }

//////////////////////////////////////////////////////////////////////////

Ogre::RenderTarget* OgreFramework::GetRenderTarget()
  {
  return mp_render_window;
  }