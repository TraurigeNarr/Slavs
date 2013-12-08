//this state
#include "ClientOptionsState.h"
//client
#include "Application.h"
#include "InputManager.h"
#include "OgreFramework.h"
#include "ScreenManager.h"
//states
#include "ClientMenuState.h"
//screens
#include "OptionsScreen.h"

ClientOptionsState::ClientOptionsState()
{
}

ClientOptionsState::~ClientOptionsState()
{
}

void ClientOptionsState::Enter(std::shared_ptr<Application> ip_owner)
{
  //TODO: own logger
	Singleton<OgreFramework>::GetInstancePtr()->m_pLog->logMessage("Enter OptionsState....");

	m_State = VarWait;

	Singleton<OgreFramework>::GetInstancePtr()->CreateSceneManager(Ogre::ST_GENERIC, "OptionsStateSceneMgr");

  Singleton<InputManager>::GetInstance().AddSubscriber(this);

	CreateScene();
}

void ClientOptionsState::Execute(std::shared_ptr<Application> ip_owner, long i_elapsed_time)
{
	switch(m_State)
	{
	case VarGoToMenu:
		ip_owner->GetFSM()->ChangeState(Singleton<ClientMenuState>::GetInstancePtr());//to menu
    break;
	}
}

void ClientOptionsState::Exit(std::shared_ptr<Application> ip_owner)
{
	Singleton<OgreFramework>::GetInstancePtr()->m_pLog->logMessage("Leaving OptionsState...");

  Singleton<InputManager>::GetInstance().RemoveSubscriber(this);
}

void ClientOptionsState::CreateScene()
{
	BuildGUI();
  Ogre::Camera* p_camera = Singleton<OgreFramework>::GetInstance().CreateCamera("OptionsCamera");
	p_camera->setPosition(Ogre::Vector3(0, 25, -50));
	p_camera->lookAt(Ogre::Vector3(0, 0, 0));
	p_camera->setNearClipDistance(1);

	p_camera->setAspectRatio(Ogre::Real(Singleton<OgreFramework>::GetInstancePtr()->m_pViewport->getActualWidth()) /
		Ogre::Real(Singleton<OgreFramework>::GetInstancePtr()->m_pViewport->getActualHeight()));

	Singleton<OgreFramework>::GetInstancePtr()->m_pViewport->setCamera(p_camera);
}

void ClientOptionsState::BuildGUI()
{
	Singleton<ScreenManager>::GetInstancePtr()->SetCurrentScreen(new OptionsScreen());
}

/************************************************************************/
/*                           Event handlers                             */
/************************************************************************/
bool ClientOptionsState::KeyPressed(const OIS::KeyEvent &keyEventRef)
{
	if(OIS::KC_ESCAPE == keyEventRef.key)
	{
		m_State = VarGoToMenu;
		return true;
	}

	return false;
}

bool ClientOptionsState::KeyReleased(const OIS::KeyEvent &keyEventRef)
{
	return false;
}

bool ClientOptionsState::MouseMoved(const OIS::MouseEvent &evt)
{
	return false;
}

bool ClientOptionsState::MousePressed(const OIS::MouseEvent &evt, OIS::MouseButtonID id)
{
	return false;
}

bool ClientOptionsState::MouseReleased(const OIS::MouseEvent &evt, OIS::MouseButtonID id)
{
	return false;
}

bool ClientOptionsState::ButtonPressed(ButtonID id, void* extraData)
{
	switch(id)
	{
	case Btn_UI_Menu:
		m_State = VarGoToMenu;
		break;
	}
	return true;
}