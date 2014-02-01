//this state
#include "ClientLoadGameState.h"
//client
#include "Application.h"
#include "CGameContext.h"
#include "OgreFramework.h"
#include "ScreenManager.h"
//states
#include "ClientGameState.h"
#include "ClientMenuState.h"
#include "InputManager.h"
//screens
#include "LoadGameScreen.h"
//common
#include <Game/Enumerations.h>
#include <Game/GameObjectState.h>
#include <Patterns/Singleton.h>
#include <Utilities/TemplateFunctions.h>
//network
#include "Net.h"

ClientLoadGameState::ClientLoadGameState(int ownMask)
{
	m_pContext = new CGameContext("test_01", ownMask);
}

ClientLoadGameState::~ClientLoadGameState()
{
}

void ClientLoadGameState::Enter(Application* ip_owner)
{
	Singleton<OgreFramework>::GetInstancePtr()->m_pLog->logMessage("Enter ClientLoadGameState....");
	
	Singleton<ScreenManager>::GetInstancePtr()->SetCurrentScreen(new LoadGameScreen());
	//set main scene manager
  std::shared_ptr<OgreFramework> p_instance = Singleton<OgreFramework>::GetInstancePtr();
  Ogre::SceneManagerEnumerator::MetaDataIterator scene_mgr_it =p_instance->m_pRoot->getSceneManagerMetaDataIterator();
  for ( ; scene_mgr_it.current() != scene_mgr_it.end(); scene_mgr_it.moveNext())
    {
    if (scene_mgr_it.peekNext()->typeName == "OctreeSceneManager")
      break;
    }

  if (scene_mgr_it.current() != scene_mgr_it.end())
    p_instance->CreateSceneManager((*scene_mgr_it.current())->sceneTypeMask, "GameSceneManager");
  else
    p_instance->CreateSceneManager(Ogre::ST_GENERIC, "GameSceneManager");

	m_State = CLState_Loading;
}

void ClientLoadGameState::Execute(Application* ip_owner, long i_elapsed_time)
{
	//hold all packets
	while ( true )
	{
		unsigned char *packet = new unsigned char[PACKET_SIZE];
		int bytes_read = ip_owner->GetConnection()->ReceivePacket( packet, (PACKET_SIZE) );

		if ( 0 == bytes_read )
		{
			delete []packet;
			break;
		}
		_HoldPacket(ip_owner, packet);
		delete []packet;
	}

	if(CLState_ClientReady == m_State)
		_SendReadyPacket(ip_owner);

	ip_owner->GetConnection()->Update(i_elapsed_time/1000.0f);
	if(ip_owner->GetConnection()->ConnectFailed())
	{
		ip_owner->FromGame = true;
		Singleton<OgreFramework>::GetInstancePtr()->m_pLog->logMessage("Connection error.");
		ip_owner->GetFSM()->ChangeState(Singleton<ClientMenuState>::GetInstancePtr());
	}
}

void ClientLoadGameState::Exit(Application* ip_owner)
{
	Singleton<OgreFramework>::GetInstancePtr()->m_pLog->logMessage("Exit ClientLoadGameState....");
}

void ClientLoadGameState::_HoldPacket(Application* ip_owner, unsigned char *packet)
{
	PacketType pType = (PacketType)FromChar<int>((char*)packet);
	char *packetToClient = NULL;

	switch(pType)
	{
	case PT_ServerReady:
		new Singleton<ClientGameState>(new ClientGameState(m_pContext));
		ip_owner->GetFSM()->ChangeState(Singleton<ClientGameState>::GetInstancePtr());
		_SendReadyPacket(ip_owner);
		/*if(CLState_ClientReady == m_State)
		{
			m_State = CLStateGoToGame;
		}
		else
		{
			m_State = CLState_ClientReady;
		}*/
		break;
	case PT_GOState:
		GameObjectState state;
		unsigned char *buf = packet + sizeof(int);
		state.Deserialize((char*)buf);
		if(OT_Terrain == state.oType)
			m_pContext->AddTerrain(m_pContext->GetMapName());
		else
			m_pContext->ApplyState(state.lID, state);
		break;

	}
}

void ClientLoadGameState::_SendReadyPacket(Application* ip_owner)
{
	PacketType pType = PT_ClientReady;
	char *buf = ToChar(pType);
	ip_owner->GetConnection()->SendPacket(buf, sizeof(int));
	delete[] buf;
}