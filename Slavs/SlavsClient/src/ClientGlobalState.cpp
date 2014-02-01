//this state
#include "ClientGlobalState.h"
//client
#include "Application.h"
//common
#include <Game/Enumerations.h>
#include <Utilities/TemplateFunctions.h>
//network
#include "Net.h"

ClientGlobalState ::ClientGlobalState()
{
}

ClientGlobalState ::~ClientGlobalState()
{
}

void ClientGlobalState::Enter(Application* ip_owner)
{
	m_exit_state = false;
}

void ClientGlobalState::Execute(Application* ip_owner, long i_elapsed_time)
{
	if(m_exit_state)
		return;

 	char *packetToServer = new char[sizeof(PacketType)];
	ToChar(PT_HasConnection, packetToServer, sizeof(PacketType));
 	net::Connection *connection = ip_owner->GetConnection();
 	if(NULL != connection && connection->IsConnected())
 		ip_owner->GetConnection()->SendPacket( packetToServer, sizeof( PacketType ) );
}

void ClientGlobalState::Exit(Application* ip_owner)
{
	m_exit_state = true;
}

void ClientGlobalState::HoldPacket(Application* ip_owner, unsigned char *packet)
{

}