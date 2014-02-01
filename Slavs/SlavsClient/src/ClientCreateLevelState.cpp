//this state
#include "ClientCreateLevelState.h"
//client
#include "Application.h"
#include "CGameContext.h"
#include "ClientEnums.h"
#include "InputManager.h"
#include "OgreFramework.h"
#include "ScreenManager.h"
//states
#include "ClientLoadGameState.h"
#include "ClientMenuState.h"
//screens
#include "CreateLevelScreen.h"
//common
#include <Game/Enumerations.h>
#include <Utilities/TemplateFunctions.h>
//network
#include "Net.h"

#pragma warning(disable : 4996)


ClientCreateLevelState::ClientCreateLevelState()
{
}

ClientCreateLevelState::~ClientCreateLevelState()
{
}

void ClientCreateLevelState::Enter(Application* ip_owner)
{
	Singleton<OgreFramework>::GetInstancePtr()->m_pLog->logMessage("Enter CreateLevelState....");
	Singleton<InputManager>::GetInstance().AddSubscriber(this);

	m_State = VarWait;;
	//try to initialize sockets
	if(!net::InitializeSockets())
	{
		Singleton<OgreFramework>::GetInstancePtr()->m_pLog->logMessage("Initialize socket error.");
		ip_owner->GetFSM()->ChangeState(Singleton<ClientMenuState>::GetInstancePtr());
		return;
	}
	/*===========================================*
	*           Start Server                     *
	*============================================*/
	//LPCTSTR lpApplicationName = L"D:\\gamedev\\game-development\\Slav_component\\Slavs\\Debug\\SlavsServer.exe"; /* The program to be executed */

	//STARTUPINFO si;
	//PROCESS_INFORMATION pi;

	//ZeroMemory( &si, sizeof(si) );
	//si.cb = sizeof(si);
	//ZeroMemory( &pi, sizeof(pi) );
	//
	///* Create the process */
	//if (!CreateProcess( lpApplicationName,   // No module name (use command line)
	//	NULL,			// Command line
	//	NULL,           // Process handle not inheritable
	//	NULL,           // Thread handle not inheritable
	//	FALSE,          // Set handle inheritance to FALSE
	//	NORMAL_PRIORITY_CLASS,              // No creation flags
	//	NULL,           // Use parent's environment block
	//	NULL,           // Use parent's starting directory 
	//	&si,            // Pointer to STARTUPINFO structure
	//	&pi )           // Pointer to PROCESS_INFORMATION structure
	//	)
	//{
	//	DWORD error = GetLastError();
	//	Singleton<OgreFramework>::GetInstancePtr()->m_pLog->logMessage("Start server error." + error);
	//	owner->GetFSM()->ChangeState(MenuState::Instance());
	//	return;
	//}
	//connects to server
	if(!ip_owner->SetConnection(new net::Connection(net::ProtocolId, net::TimeOut), net::ClientPort))
	{
		Singleton<OgreFramework>::GetInstancePtr()->m_pLog->logMessage("Initializing client connection fails.");
		ip_owner->GetFSM()->ChangeState(Singleton<ClientMenuState>::GetInstancePtr());
		return;
	}
	if(!Connect(ip_owner, ip_owner->GetOptions().GetServerAddress()))
	{
		Singleton<OgreFramework>::GetInstancePtr()->m_pLog->logMessage("Can`t connects to server.");
		ip_owner->GetFSM()->SetGlobalState(NULL);
		ip_owner->GetFSM()->ChangeState(Singleton<ClientMenuState>::GetInstancePtr());
		return;
	}

	Singleton<OgreFramework>::GetInstancePtr()->CreateSceneManager(Ogre::ST_GENERIC, "CreateLevelStateSceneMgr");
	Singleton<OgreFramework>::GetInstancePtr()->m_pSceneManager->setAmbientLight(Ogre::ColourValue(0.7f, 0.7f, 0.7f));

	CreateScene();
	m_State = VarGetMaps;
}

bool ClientCreateLevelState::Connect(Application* ip_owner, const net::Address& address)
{
	net::Connection* connection = ip_owner->GetConnection();
	connection->Connect(address);
	//send packet to server
	int packetType = PT_Connects;
	char *packetToServer = ToChar(packetType);
	while( true )
	{	
		bool f = connection->SendPacket( packetToServer, sizeof( packetToServer ) );
		//wait for delta time
		Sleep(net::DeltaTime * 3000);

		unsigned char *packet = new unsigned char[PACKET_SIZE];
		int bytes_read = connection->ReceivePacket( packet, (PACKET_SIZE) );
		if(0 != bytes_read)
		{
			PacketType pType = (PacketType)FromChar<int>((char*)packet);
			return true;
		}
		//check for connection
		if(connection->IsConnected())
		{
			delete []packet;
			return true;
		}
		if(connection->ConnectFailed())
		{
			delete []packet;
			return false;
		}
		connection->Update( net::DeltaTime );
		//send packet to server again --> suppose that another packets server didn`t recieve
		connection->SendPacket( packetToServer, sizeof( packetToServer ) );
		delete []packet;
		//wait for delta time
		Sleep(net::DeltaTime * 1000);
	}
	delete packetToServer;
	return false;
}

void ClientCreateLevelState::AvailableMaps(net::Connection* connection)
{
	while(true)
	{
		unsigned char *packet = new unsigned char[PACKET_SIZE];
		int bytes_read = connection->ReceivePacket( packet, (PACKET_SIZE) );
		if(0 == bytes_read)
		{
			delete[] packet;
			break;
		}
		int packetType = FromChar<int>((char*)packet);
		if(PT_Maps == packetType)
		{
			char* c = new char[bytes_read - sizeof(int)];
			std::copy(packet + sizeof(int), packet + bytes_read, c);
			Singleton<ScreenManager>::GetInstancePtr()->GetCurrentScreen()->HandleData(Cmd_UI_AddMap, c);
			delete []c;
			m_State = VarWait;
		}
		delete []packet;
	}
	//send packet to server
	char *packetToServer = new char[sizeof(PacketType)];
	ToChar(PT_Maps, packetToServer, sizeof(PacketType));
	connection->SendPacket( packetToServer, sizeof(PacketType));
	delete []packetToServer;
	Sleep(net::DeltaTime * 5000);
}

void ClientCreateLevelState::Execute(Application* ip_owner, long i_elapsed_time)
{
	switch(m_State)
	{
	case VarGetMaps:
		AvailableMaps(ip_owner->GetConnection());
		break;
	case VarGoToMenu:
		ip_owner->GetFSM()->ChangeState(Singleton<ClientMenuState>::GetInstancePtr());
		break;
	case VarGoToGame:
		SendSelectedMap(ip_owner->GetConnection(), m_sCurrentMap.c_str(), m_sCurrentMap.length());
		new Singleton<ClientLoadGameState>(new ClientLoadGameState(ip_owner->GetConnection()->GetAddress().GetAddress()));
		ip_owner->GetFSM()->ChangeState(Singleton<ClientLoadGameState>::GetInstancePtr());
		break;
	}
}

void ClientCreateLevelState::Exit(Application* ip_owner)
{
	Singleton<OgreFramework>::GetInstancePtr()->m_pLog->logMessage("Leaving CreateLevelState...");
  Singleton<InputManager>::GetInstance().RemoveSubscriber(this);
}

void ClientCreateLevelState::CreateScene()
{
	BuildGUI();

  Ogre::Camera* p_camera = Singleton<OgreFramework>::GetInstancePtr()->CreateCamera("CreateLvlCamera");

	p_camera->setPosition(Ogre::Vector3(0, 25, -50));
	p_camera->lookAt(Ogre::Vector3(0, 0, 0));
	p_camera->setNearClipDistance(1);

	p_camera->setAspectRatio(Ogre::Real(Singleton<OgreFramework>::GetInstancePtr()->m_pViewport->getActualWidth()) /
		Ogre::Real(Singleton<OgreFramework>::GetInstancePtr()->m_pViewport->getActualHeight()));

	Singleton<OgreFramework>::GetInstancePtr()->m_pViewport->setCamera(p_camera);
}

void ClientCreateLevelState::BuildGUI()
{
	Singleton<ScreenManager>::GetInstancePtr()->SetCurrentScreen(new CreateLevelScreen());
}

void ClientCreateLevelState::SendSelectedMap(net::Connection* connection, const char* selectedMap, int size)
{
	char *packetToServer = new char[sizeof(int) + size];
	ToChar(PT_StartGame, packetToServer, sizeof(int));
		
	ToChar(*selectedMap, packetToServer + sizeof(int), size);

	connection->SendPacket( packetToServer, sizeof(int) + size );
}

/************************************************************************/
/*                           Event handlers                             */
/************************************************************************/
bool ClientCreateLevelState::KeyPressed(const OIS::KeyEvent &keyEventRef)
{
	if(OIS::KC_ESCAPE == keyEventRef.key)
	{
		m_State = VarGoToMenu;
		return true;
	}

	return false;
}

bool ClientCreateLevelState::KeyReleased(const OIS::KeyEvent &keyEventRef)
{
	return false;
}

bool ClientCreateLevelState::MouseMoved(const OIS::MouseEvent &evt)
{
	return false;
}

bool ClientCreateLevelState::MousePressed(const OIS::MouseEvent &evt, OIS::MouseButtonID id)
{
	return false;
}

bool ClientCreateLevelState::MouseReleased(const OIS::MouseEvent &evt, OIS::MouseButtonID id)
{
	return false;
}

bool ClientCreateLevelState::ButtonPressed(ButtonID id, void* extraData)
{
	switch(id)
	{
	case  Btn_UI_MapChanged:
		if(NULL != extraData)
			m_sCurrentMap = static_cast<char*>(extraData);
		break;
	case Btn_UI_Menu:
		m_State = VarGoToMenu;
		break;
	case Btn_UI_StartGame:
		m_State = VarGoToGame;
		break;
	}
	return true;
}