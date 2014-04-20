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
#include <Net.h>

#include <boost/lexical_cast.hpp>

//////////////////////////////////////////////////////////////////////////

ClientLoadGameState::ClientLoadGameState(int i_own_mask)
  : mh_game_context(new CGameContext("test_01", i_own_mask))
  , mp_server_address(new net::Address(net::Address(127, 0, 0, 1, 30000)))
  , m_content_number(0)
  , m_current_content_number(0)
{
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

	m_state = CLState_Loading;

#pragma region Connect to server
  //try to initialize sockets
  if(!net::InitializeSockets())
    {
    Singleton<OgreFramework>::GetInstancePtr()->m_pLog->logMessage("Initialize socket error.");
    ip_owner->GetFSM()->ChangeState(Singleton<ClientMenuState>::GetInstancePtr());
    return;
    }

  if(!ip_owner->SetConnection(new net::Connection(net::ProtocolId, net::TimeOut), net::ClientPort))
    {
    Singleton<OgreFramework>::GetInstancePtr()->m_pLog->logMessage("Initializing client connection fails.");
    ip_owner->GetFSM()->ChangeState(Singleton<ClientMenuState>::GetInstancePtr());
    return;
    }
  ip_owner->GetConnection()->Connect(*mp_server_address);
  std::unique_ptr<char[]> p_buffer(new char[sizeof(PacketType)]);
  ToChar(PT_Achived, &p_buffer[0], sizeof(PacketType));
  ip_owner->GetConnection()->SendPacket(&p_buffer[0], sizeof(PacketType));
  //check for connection
  if(ip_owner->GetConnection()->IsConnected() || ip_owner->GetConnection()->ConnectFailed())
    {
    Singleton<OgreFramework>::GetInstancePtr()->m_pLog->logMessage("Connection error.");
    ip_owner->GetFSM()->ChangeState(Singleton<ClientMenuState>::GetInstancePtr());
    }
#pragma endregion
  mh_game_context->AddTerrain(mh_game_context->GetMapName());
  m_content_number = m_current_content_number = 0;
}

void ClientLoadGameState::Execute(Application* ip_owner, long i_elapsed_time)
  {
	//hold all packets
  std::unique_ptr<unsigned char[]> packet(new unsigned char[PACKET_SIZE]);
	while ( true )
	  {
		int bytes_read = ip_owner->GetConnection()->ReceivePacket( &packet[0], (PACKET_SIZE) );

		if ( 0 == bytes_read )
			break;
		_HoldPacket(ip_owner, &packet[0], bytes_read);
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

void ClientLoadGameState::SetServerAddress(const net::Address& i_address)
  {
  mp_server_address.reset(new net::Address(i_address));
  }

void ClientLoadGameState::_HoldPacket(Application* ip_owner, unsigned char *packet, size_t i_bytes_read)
{
	PacketType pType = (PacketType)FromChar<int>((char*)packet);
	char *packetToClient = NULL;

	switch(pType)
	{
	case PT_ServerReady:
		new Singleton<ClientGameState>(new ClientGameState(std::move(mh_game_context)));
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
  case PT_ContentNumber:
    m_content_number = FromChar<int>(reinterpret_cast<char*>(packet+sizeof(PacketType)));
    break;
	case PT_GOState:
    {
		GameObjectState state;
		unsigned char *buf = packet + sizeof(int);
		state.Deserialize((char*)buf);
    mh_game_context->ApplyState(state.lID, state);
    ++m_current_content_number;
    if (m_current_content_number == m_content_number)
      {
      _SendReadyPacket(ip_owner);
      m_content_number = m_current_content_number = 0;
      }
    }
		break;
  case  PT_Definitions:
    {
    std::string obj_name  = "";
    int         obj_id    = -1;
    for (size_t i = sizeof(PacketType); i < i_bytes_read; ++i)
      {
      if (packet[i] == ';')
        {
        obj_id = FromChar<int>(reinterpret_cast<char*>(packet+i+1));
        break;
        }
      else
        obj_name += packet[i];
      }
    
    if (obj_id != -1)
      mh_game_context->AddDefinition(std::make_pair(obj_name, obj_id));
    ++m_current_content_number;
    if (m_current_content_number == m_content_number)
      {
      mh_game_context->InitObjectsMap();
      _SendReadyPacket(ip_owner);
      m_content_number = m_current_content_number = 0;
      }
    }
    break;
	}
}

void ClientLoadGameState::_SendReadyPacket(Application* ip_owner)
{
	PacketType pType = PT_Achived;
	char *buf = ToChar(pType);
	ip_owner->GetConnection()->SendPacket(buf, sizeof(int));
	delete[] buf;
}