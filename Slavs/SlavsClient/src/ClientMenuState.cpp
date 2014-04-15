//this state
#include "ClientMenuState.h"
//client
#include "Application.h"
#include "InputManager.h"
#include "OgreFramework.h"
#include "ScreenManager.h"
//states
#include "ClientCreateLevelState.h"
#include "ClientGameState.h"
#include "ClientLoadGameState.h"
#include "ClientOptionsState.h"
//screens
#include "MenuScreen.h"
//network
#include <Net.h>

ClientMenuState::ClientMenuState()
{
}

ClientMenuState::~ClientMenuState()
{
}

void ClientMenuState::Enter(Application* ip_owner)
{
	Singleton<OgreFramework>::GetInstancePtr()->m_pLog->logMessage("Enter MenuState....");

	if(ip_owner->FromGame)
	{
    Singleton<ClientGameState>::ReleaseIfValid();
    Singleton<ClientLoadGameState>::ReleaseIfValid();
	}
	m_State = VarWait;

	Singleton<OgreFramework>::GetInstancePtr()->CreateSceneManager(Ogre::ST_GENERIC, "MenuStateSceneMgr")->setAmbientLight(Ogre::ColourValue(0.7f, 0.7f, 0.7f));

  Singleton<InputManager>::GetInstance().AddSubscriber(this);

	CreateScene();
}

void ClientMenuState::Execute(Application* ip_owner, long i_elapsed_time)
{
	switch(m_State)
	{
	case VarQuit:
		ip_owner->Shutdown();
		break;
	case VarGoToOptions:
		ip_owner->GetFSM()->ChangeState(Singleton<ClientOptionsState>::GetInstancePtr());
		break;
	case VarGoToCreateLevel:
    {
    int temp_mask = 1;
    new Singleton<ClientLoadGameState>(new ClientLoadGameState(temp_mask));
    ip_owner->GetFSM()->ChangeState(Singleton<ClientLoadGameState>::GetInstancePtr());
    }
		break;
	}
}

void ClientMenuState::Exit(Application* ip_owner)
{
	Singleton<OgreFramework>::GetInstancePtr()->m_pLog->logMessage("Leaving MenuState...");
  Singleton<InputManager>::GetInstance().RemoveSubscriber(this);
}

void ClientMenuState::CreateScene()
{
	BuildGUI();
  Ogre::Camera* p_camera = Singleton<OgreFramework>::GetInstance().CreateCamera("MenuCam");
	p_camera->setPosition(Ogre::Vector3(0, 25, -50));
	p_camera->lookAt(Ogre::Vector3(0, 0, 0));
	p_camera->setNearClipDistance(1);

	p_camera->setAspectRatio(Ogre::Real(Singleton<OgreFramework>::GetInstancePtr()->m_pViewport->getActualWidth()) /
		Ogre::Real(Singleton<OgreFramework>::GetInstancePtr()->m_pViewport->getActualHeight()));

	Singleton<OgreFramework>::GetInstancePtr()->m_pViewport->setCamera(p_camera);
}

void ClientMenuState::BuildGUI()
{
	Singleton<ScreenManager>::GetInstance().SetCurrentScreen(new MenuScreen());
}

/************************************************************************/
/*                           Event handlers                             */
/************************************************************************/
bool ClientMenuState::KeyPressed(const OIS::KeyEvent &keyEventRef)
{
	if(OIS::KC_ESCAPE == keyEventRef.key)
	{
		m_State = VarQuit;
		return true;
	}
	return false;
}

bool ClientMenuState::KeyReleased(const OIS::KeyEvent &keyEventRef)
{
	return false;
}

bool ClientMenuState::MouseMoved(const OIS::MouseEvent &evt)
{
	return false;
}

bool ClientMenuState::MousePressed(const OIS::MouseEvent &evt, OIS::MouseButtonID id)
{
	return false;
}

bool ClientMenuState::MouseReleased(const OIS::MouseEvent &evt, OIS::MouseButtonID id)
{
	return false;
}

bool ClientMenuState::ButtonPressed(ButtonID id, void* extraData)
{
	switch(id)
	{
	case Btn_UI_CreateLevel:
		m_State = VarGoToCreateLevel;
		break;
	case Btn_UI_Options:
		m_State = VarGoToOptions;
		break;
	case Btn_UI_Quit:
		m_State = VarQuit;
		break;
	}
	return true;
}