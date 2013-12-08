//client
#include "Application.h"
//frameworks
#include "CEGUIFramework.h"
#include "OgreFramework.h"
//managers
#include "InputManager.h"
#include "ScreenManager.h"
//states
#include "ClientCreateLevelState.h"
#include "ClientGameState.h"
#include "ClientGlobalState.h"
#include "ClientLoadGameState.h"
#include "ClientMenuState.h"
#include "ClientOptionsState.h"
#include "InputManager.h"
#include "VisualInformation.h"

//network
#include <Net.h>

#include <Patterns/Singleton.h>

//frameworks
template<> std::shared_ptr<OgreFramework> Singleton<OgreFramework>::mp_singleton = nullptr;
template<> std::shared_ptr<CEGUIFramework> Singleton<CEGUIFramework>::mp_singleton = nullptr;
//states
template<> std::shared_ptr<ClientGlobalState> Singleton<ClientGlobalState>::mp_singleton = nullptr;
template<> std::shared_ptr<ClientMenuState> Singleton<ClientMenuState>::mp_singleton = nullptr;
template<> std::shared_ptr<ClientCreateLevelState> Singleton<ClientCreateLevelState>::mp_singleton = nullptr;
template<> std::shared_ptr<ClientOptionsState> Singleton<ClientOptionsState>::mp_singleton = nullptr;
template<> std::shared_ptr<ClientLoadGameState> Singleton<ClientLoadGameState>::mp_singleton = nullptr;
template<> std::shared_ptr<ClientGameState> Singleton<ClientGameState>::mp_singleton = nullptr;
//managers
template<> std::shared_ptr<ScreenManager> Singleton<ScreenManager>::mp_singleton = nullptr;
template<> std::shared_ptr<InputManager> Singleton<InputManager>::mp_singleton = nullptr;
//other
template<> std::shared_ptr<VisualInformation> Singleton<VisualInformation>::mp_singleton = nullptr;



void Application::Start()
{
	FromGame = false;
	if(!_InitSystem())
	{
		return;
	}
	m_cConnection = NULL;

	m_bShutdown = false;
	m_pFSM = new StateMachine<Application, long>(std::shared_ptr<Application>(this));
	
	m_pFSM->SetCurrentState(Singleton<ClientMenuState>::GetInstancePtr());
	m_pFSM->SetGlobalState(Singleton<ClientGlobalState>::GetInstancePtr());

	//start rendering
	Singleton<OgreFramework>::GetInstance().m_pRoot->startRendering();
}

void Application::Shutdown()
{
	m_bShutdown = true;
}

bool Application::_InitSystem()
{
  //frameworks
	new Singleton<OgreFramework>(new OgreFramework());
	if(!Singleton<OgreFramework>::GetInstancePtr()->InitOgre("Slavs"))
		return false;
  Singleton<OgreFramework>::GetInstance().m_pRoot->addFrameListener(this);

 	new Singleton<CEGUIFramework>(new CEGUIFramework());
 	if(!Singleton<CEGUIFramework>::GetInstancePtr()->initCEGUI())
 		return false;
  //managers
	new Singleton<InputManager>(new InputManager());
	new Singleton<ScreenManager>(new ScreenManager());
  //states
  new Singleton<ClientGlobalState>(new ClientGlobalState());
  new Singleton<ClientMenuState>(new ClientMenuState());

  new Singleton<ClientOptionsState>(new ClientOptionsState());
  new Singleton<ClientCreateLevelState>(new ClientCreateLevelState());
	return true;
}

bool Application::SetConnection(net::Connection* connection, const int port)
{
	if(NULL != m_cConnection && NULL != connection)
		delete m_cConnection;
	m_cConnection = connection;
	return m_cConnection->Start(port);
}

void Application::StopConnection()
{
	m_cConnection->Stop();
	m_cConnection = NULL;
}

bool Application::frameRenderingQueued(const Ogre::FrameEvent& evt)
{
	if(m_bShutdown)
		return false;
	float sleepTime = 0.018 - evt.timeSinceLastFrame;
	sleepTime *= 1000.0f;
	if(0 < sleepTime)
		Sleep(sleepTime);
	//Singleton<OgreFramework>::GetInstancePtr()->m_pTrayMgr->frameRenderingQueued(evt);
	Ogre::WindowEventUtilities::messagePump();
	//update Application classes
	Singleton<InputManager>::GetInstance().Update(evt.timeSinceLastFrame);
	Singleton<ScreenManager>::GetInstance().Update(evt.timeSinceLastFrame);

	m_pFSM->Update(evt.timeSinceLastFrame*1000.0f);
	return true;
}