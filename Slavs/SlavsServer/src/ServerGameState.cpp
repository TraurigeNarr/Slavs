#include "ServerGameState.h"
//server
#include "SGameContext.h"
#include "Main/ServerMain.h"
#include "misc\ServerEnums.h"
//common
#include <Game/Enumerations.h>
#include <Game/GameObjectState.h>
#include <Game/TimeController.h>
#include <Patterns/Singleton.h>
#include <Utilities/TemplateFunctions.h>
//states
#include "ServerWaitState.h"
#include "ServerLoadGameState.h"
//standard
#include <algorithm>
#include <iostream>

//////////////////////////////////////////////////////////////////////////

//initialize singleton pointer template
/*template<> MessageDispatcher<SGameContext>* MessageDispatcher<SGameContext>::ms_singleton = NULL;*/

ServerGameState::ServerGameState(SGameContext* context, std::shared_ptr<std::map<int, IController*>> controllers)
	: m_pSContext(context), m_last_sent(0)
{
	m_pControllers = controllers;
	//get from config!
	mp_time_controller = std::make_shared<TimeController>(MS_FOR_TICK);
}

ServerGameState::~ServerGameState()
{
}

ServerGameState::TTimeController ServerGameState::GetTimeController() const
{
  return mp_time_controller;
}

void ServerGameState::Enter(ServerMain* ip_owner)
{
	printf( "Enters GAME state\n" );
  ip_owner->GetFSM()->SetPreviousState(nullptr);
  Singleton<ServerLoadGameState>::ReleaseIfValid();

	m_bExitState = false;
	ip_owner->FromGame = true;

	//initialize tick listeners
  mp_time_controller->AddSubscriber(m_pSContext);
	std::for_each(m_pControllers->begin(), m_pControllers->end(), [this](std::pair<int, IController*> contr)
	{
		mp_time_controller->AddSubscriber(contr.second);
	});
}

void ServerGameState::Execute(ServerMain* ip_owner, long i_elapsed_time)
{
	//catch all packets
	unsigned char *packet = new unsigned char[PACKET_SIZE];
	while(true)
	{
		int bytes_read = ip_owner->GetConnection()->ReceivePacket( packet, (PACKET_SIZE) );
		if(0 == bytes_read)
			break;
		HoldPacket(ip_owner, packet, bytes_read);
	}
	delete[] packet;
	//check if we have to go to Menu state --> game is over =(
	if(m_bExitState)
		return;
	//update time controller
	mp_time_controller->Update(i_elapsed_time);
  //sends all object states
  SendStates(ip_owner);
}

void ServerGameState::Exit(ServerMain* ip_owner)
{
	printf( "Exits GAME state\n" );
  //clear objects
  mp_time_controller->RemoveSubscriber(m_pSContext);
  m_pSContext->ReleaseContext();
  delete m_pSContext;
  //disconnect from client
	ip_owner->GetConnection()->Disconnect();
  std::for_each(m_pControllers->begin(), m_pControllers->end(), [this](std::pair<int, IController*> contr)
    {
    mp_time_controller->RemoveSubscriber(contr.second);
    });

  ClearSTLMap(*m_pControllers);
  m_pControllers->clear();
}

void ServerGameState::HoldPacket(ServerMain* ip_owner, unsigned char *packet, size_t bytes_read)
{
	PacketType pType = (PacketType)FromChar<int>((char*)packet);
	char *packetToClient = NULL;
	switch(pType)
	{
	case PT_EndGame:
		new Singleton<WaitState>(new WaitState());
		m_bExitState = true;
		ip_owner->GetFSM()->ChangeState(Singleton<WaitState>::GetInstancePtr());
		return;
	case PT_Command:
	case PT_Selection:
		PassToController(ip_owner, packet, bytes_read);
		break;
	}
}

void ServerGameState::SendStates(ServerMain* ip_owner)
{
  net::Connection &connection = *ip_owner->GetConnection();

	ObjectStateMap sendMap;
  m_pSContext->GetAllGameObjectsState(sendMap);
	size_t neededSize = 0;
	if(!sendMap.empty())
  {
    std::for_each(sendMap.begin(), sendMap.end(), [&neededSize](std::pair<long, GameObjectState*> p)
    {
      neededSize = neededSize < p.second->NeededSize() ? p.second->NeededSize() : neededSize;
    });
    neededSize += sizeof(PacketType);
    char *buf = new char[neededSize];
    std::for_each(sendMap.begin(), sendMap.end(), [&connection, buf](std::pair<long, GameObjectState*> p)
    {
      size_t neededSize = p.second->NeededSize() + sizeof(PacketType);
      ToChar(PT_GOState, buf, sizeof(PacketType));
      char *buf_end = buf + sizeof(PacketType);

      p.second->Serialize(buf_end, neededSize);
      connection.SendPacket(buf, neededSize);
    });
    delete []buf;

    ClearSTLMap(sendMap);
  }

  ++m_last_sent;
  neededSize = m_pControllers->begin()->second->NeededSize() + sizeof(PacketType);
  if(m_last_sent < 200 || neededSize == 0)
    return;
  m_last_sent = 0;

  char* buf = new char[neededSize];
	//send controller`s states --> currently only resources
	std::for_each(m_pControllers->begin(), m_pControllers->end(), [&buf, &neededSize, &connection](std::pair<int, IController*> controller)
	{
		neededSize = controller.second->NeededSize() + sizeof(PacketType);
		ToChar(PT_ControllerState, buf, sizeof(PacketType));
		char *buf_end = buf + sizeof(PacketType);

		if(0 != controller.second->Serialize(buf_end, neededSize))
		{
			connection.SendPacket(buf, neededSize);
		}
	});
  delete []buf;
}

void ServerGameState::PassToController(ServerMain* ip_owner, unsigned char *packet, size_t bytes_read)
{
	std::map<int, IController*>::const_iterator iter = m_pControllers->find(ip_owner->GetConnection()->GetAddress().GetAddress());

	if(m_pControllers->end() != iter)
		iter->second->HoldPacket(ip_owner->GetConnection(), packet, bytes_read);
}