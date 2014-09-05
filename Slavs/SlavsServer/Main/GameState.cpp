#include "GameState.h"

#include "Game\GameContext.h"
#include "Game\GameObject.h"
#include "LoadGameState.h"
#include "ServerMain.h"
#include "misc\ServerEnums.h"

#include <Utilities/TemplateFunctions.h>

#include <Network/PacketProvider.h>
#include <Network/PacketType.h>
#include <Network/include/Net.h>

#include <memory>

//////////////////////////////////////////////////////////////////////////

namespace
  {

  void SendStates(const Slavs::GameContext& i_context, net::Connection& i_connection)
    {
    ObjectStateMap states_map;
    i_context.GetAllGameObjectsState(states_map);
    if (!states_map.empty())
      {
      std::unique_ptr<char[]> p_buffer(new char[PACKET_SIZE]);

      std::for_each(states_map.begin(), states_map.end(), [&i_connection, &p_buffer](std::pair<long, GameObjectState*> p)
        {
        size_t neededSize = p.second->NeededSize() + sizeof(Network::PacketType);
        ToChar(Network::PacketType::PT_GOState, &p_buffer[0], sizeof(Network::PacketType));

        p.second->Serialize(&p_buffer[sizeof(Network::PacketType)], neededSize);
        i_connection.SendPacket(&p_buffer[0], neededSize);
        });

      }
    ClearSTLMap(states_map);
    }

  } // namespace

//////////////////////////////////////////////////////////////////////////

namespace Slavs
  {
  GameState::GameState(TGameContext ih_game_context)
    : mh_game_context(std::move(ih_game_context))
    , mp_time_controller(std::make_shared<TimeController>(MS_FOR_TICK))
    {     }

  GameState::~GameState()
    {    }

  void GameState::Enter(ServerMain* ip_owner)
    {
    printf( "Enters GameState state\n" );
    
    ip_owner->GetStateMachine().SetPreviousState(nullptr);
    ip_owner->FromGame = true;

    mp_time_controller->AddSubscriber(mh_game_context.get());
    }

  void GameState::Execute(ServerMain* ip_owner, long i_elapsed_time)
    {
    std::unique_ptr<unsigned char[]> packet(new unsigned char[PACKET_SIZE]);
    while(true)
      {
      int bytes_read = ip_owner->GetConnection()->ReceivePacket( &packet[0], PACKET_SIZE );
      if(0 == bytes_read)
        break;

      HoldPacket(ip_owner, &packet[0], bytes_read);
      }
    mp_time_controller->Update(i_elapsed_time);
    SendStates(*mh_game_context, *ip_owner->GetConnection());
    }

  void GameState::Exit(ServerMain* ip_owner)
    {
    printf( "Exits LoadGame state\n" );
    mp_time_controller->RemoveSubscriber(mh_game_context.get());
    mh_game_context->ReleaseContext();
    ip_owner->GetConnection()->Disconnect();
    }

  void GameState::HoldPacket(ServerMain* ip_owner, unsigned char* ip_packet, size_t i_bytes_read)
    {
    Network::PacketType pType = (Network::PacketType)FromChar<int>((char*)ip_packet);
    char *packetToClient = NULL;
    switch(pType)
      {
      case Network::PacketType::PT_EndGame:
        ip_owner->Stop();
        return;
      }
    }

  } // Slavs