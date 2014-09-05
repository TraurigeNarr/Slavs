#include "ServerLoadGameState.h"
//server
#include "SGameContext.h"
#include "Main/ServerMain.h"
#include "misc\SceneLoader.h"
//states
#include "ServerGameState.h"
#include "ServerWaitState.h"

#include "PluginSystem/MetaFactory.h"

#include <Net.h>
#include <Network/Packet.h>
#include <Network/PacketType.h>
//common
#include <Game/Enumerations.h>
#include <Game/GameObjectState.h>
#include <Patterns/Singleton.h>
#include <Utilities/TemplateFunctions.h>
#include <Utilities/FileUtilities.h>
//standard
#include <algorithm>
#include <iostream>
#include <string>

const std::string MapName = "server\\maps\\test_01.scene";

ServerLoadGameState::ServerLoadGameState(std::shared_ptr<std::map<int, IController*>> controllers)
{
	m_pControllers = controllers;
}

ServerLoadGameState::~ServerLoadGameState()
{}

void ServerLoadGameState::Enter(ServerMain* ip_owner)
{
	printf( "Enters LoadGame state\n" );
	Singleton<WaitState>::ReleaseIfValid();


  // load libraries
  ServerMain::GetInstance().Start("F:\\Projects\\Slavs\\Resources\\server\\StartServer.xml");

	m_pGameContext = new SGameContext("test_01");
	SceneLoader sLoader(m_pGameContext, &std::cout);
	std::string s = FileUtilities::JoinPath(FileUtilities::GetApplicationDirectory(), MapName);
	sLoader.LoadScene(s);
	//initialize controllers
	std::for_each(m_pControllers->begin(), m_pControllers->end(), [this](std::pair<int, IController*> controller)
		{
			controller.second->SetContext(m_pGameContext);
	});
	//initialize all controllers with specific objects and resources
	std::for_each(m_pControllers->begin(), m_pControllers->end(), [this](std::pair<int, IController*> controller)
	{
		m_pGameContext->GiveInitialObjects(controller.second);
	});
	//send all objects state to client and wait for respond
	ObjectStateMap sendMap;
	m_pGameContext->GetAllGameObjectsState(sendMap);
	int neededSize = 0;
	net::Connection &connection = *ip_owner->GetConnection();
	std::for_each(sendMap.begin(), sendMap.end(), [&neededSize, &connection](std::pair<long, GameObjectState*> p)
	{
		neededSize = p.second->NeededSize() + sizeof(Network::PacketType);
		char *buf = new char[neededSize];
    ToChar(Network::PacketType::PT_GOState, buf, sizeof(Network::PacketType));
		char *buf_end = buf + sizeof(Network::PacketType);

		p.second->Serialize(buf_end, neededSize);
		connection.SendPacket(buf, neededSize);
		delete []buf;
	});
	//clear map
	ClearSTLMap(sendMap);
	//after sending all states send msg about server ready state
	char *packetToClient = new char[sizeof(Network::PacketType)];
	ToChar(Network::PacketType::PT_ServerReady, packetToClient, sizeof(Network::PacketType));
	ip_owner->GetConnection()->SendPacket( packetToClient, sizeof( Network::PacketType ) );
	//wait for respond of client that it receives the packet
	delete packetToClient;
	printf("Begin update load\n");
}

void ServerLoadGameState::Execute(ServerMain* ip_owner, long i_elapsed_time)
{
  unsigned char *packet = new unsigned char[PACKET_SIZE];
	while(true)
	{
		int bytes_read = ip_owner->GetConnection()->ReceivePacket( packet, PACKET_SIZE );
		if(0 == bytes_read)
			break;

		HoldPacket(ip_owner, packet, bytes_read);
	}
  delete []packet;
}

void ServerLoadGameState::Exit(ServerMain* ip_owner)
{
	printf( "Exits LoadGame state\n" );
}

void ServerLoadGameState::HoldPacket(ServerMain* ip_owner, unsigned char* ip_packet, size_t i_bytes_read)
{
	Network::PacketType pType = (Network::PacketType)FromChar<int>((char*)ip_packet);
	char *packetToClient = NULL;
	switch(pType)
	{
	case Network::PacketType::PT_ClientReady:
		if(!Singleton<ServerGameState>::IsValid())
		{
			new Singleton<ServerGameState>(new ServerGameState(m_pGameContext, m_pControllers));
			ip_owner->GetFSM()->ChangeState(Singleton<ServerGameState>::GetInstancePtr());
		}
		return;
		break;
	}
}