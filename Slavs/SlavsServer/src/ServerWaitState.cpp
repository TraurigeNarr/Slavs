//this state
#include "ServerWaitState.h"
//server
#include "SPlayerController.h"
#include "Main\ServerMain.h"
//states
#include "ServerGameState.h"
#include "ServerLoadGameState.h"
//common
#include <Game\Enumerations.h>
#include <Patterns/Singleton.h>
#include <Utilities/TemplateFunctions.h>

#include <iostream>

WaitState::WaitState()
{
	m_pControllers = std::make_shared<std::map<int,IController*>>(*new std::map<int, IController*>());
}

WaitState::~WaitState()
{}

void WaitState::Enter(ServerMain* ip_owner)
{
	printf( "Enters wait state\n" );
  if(m_pControllers->size() != 0)
  {
    ClearSTLMap(*m_pControllers);
    m_pControllers->clear();
  }

  //check prevous state and if it is Game state then release
	if(ip_owner->FromGame)
    {
    ip_owner->GetFSM()->SetPreviousState(nullptr);
    Singleton<ServerGameState>::ReleaseIfValid();
    }

	m_bExitState = false;
}

void WaitState::Execute(ServerMain* ip_owner, long i_elapsed_time)
{
	while(true)
	{
		unsigned char *packet = new unsigned char[PACKET_SIZE];
		int bytes_read = ip_owner->GetConnection()->ReceivePacket( packet, (PACKET_SIZE) );
		if(0 == bytes_read)
		{
			delete []packet;
			break;
		}
		HoldPacket(ip_owner, packet, bytes_read);
		delete []packet;
		if(m_bExitState)
			return;
	}
}

void WaitState::Exit(ServerMain* ip_owner)
{
	printf( "Exits wait state\n" );
}

void WaitState::HoldPacket(ServerMain* ip_owner, unsigned char *packet, size_t size)
{
	PacketType pType = (PacketType)FromChar<int>((char*)packet);
	char *packetBuf = NULL;
	char *m = new char[8];
	m[0] = 't';
	m[1] = 'e';
	m[2] = 's';
	m[3] = 't';
	m[4] = '_';
	m[5] = '0';
	m[6] = '1';
	m[7] = '\0';
	switch(pType)
	{
	case PT_Connects:		
		ClientConnects(ip_owner);
		break;
	case PT_Maps:
		packetBuf = new char[sizeof(int) + 8*sizeof(char)];
		ToChar(PT_Maps, packetBuf, sizeof(int));
		ToChar(*m, packetBuf + sizeof(int), 8*sizeof(char));
		ip_owner->GetConnection()->SendPacket( packetBuf, sizeof(int) + 8*sizeof(char) );
		printf("Need maps\n");
		break;
	case PT_StartGame:
		new Singleton<ServerLoadGameState>(new ServerLoadGameState(m_pControllers));
		packet[size] = '\0';
		printf("Map is %s\n", (packet + sizeof(int)));
		ip_owner->GetFSM()->ChangeState(Singleton<ServerLoadGameState>::GetInstancePtr());
		m_bExitState = true;
		break;
	}
	if(NULL != packetBuf)
		delete []packetBuf;
	delete []m;
}

void WaitState::ClientConnects(ServerMain* ip_owner)
{
	//find address in list of controllers
	int address = ip_owner->GetConnection()->GetAddress().GetAddress();
	std::map<int, IController*>::const_iterator address_iter = m_pControllers->find(address);
	//if controller with such mask already exists return
	if(m_pControllers->end() == address_iter)
  {
    //create new controller
    SPlayerController* controller = new SPlayerController(address);
    m_pControllers->insert(std::pair<int, IController*>(controller->GetMask(), controller));
  }
	
	//send to client his mask
	char *packetBuf = new char[sizeof(PacketType)];
	ToChar(PT_Connects, packetBuf, sizeof(PacketType));
	ip_owner->GetConnection()->SendPacket( packetBuf, sizeof(PacketType) );
	delete []packetBuf;
}