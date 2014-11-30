#include "GameState.h"

#include "Game\GameContext.h"
#include "Game\GameObject.h"
#include "LoadGameState.h"
#include "ServerMain.h"
#include "misc\ServerEnums.h"

#include "IController.h"

#include "Management/Goverment.h"

#include <Utilities/TemplateFunctions.h>

#include <Network/Packet.h>
#include <Network/SerializableData.h>
#include <Network/include/Net.h>

#include <memory>

//////////////////////////////////////////////////////////////////////////

namespace
  {

  void SendPacket (const Network::Packet& i_packet, net::Connection& i_connection)
    {
    i_connection.SendPacket(i_packet.mp_data, i_packet.m_size+sizeof(Network::PacketType));
    }

  void SendControllerStates (const Slavs::GameContext& i_context, net::Connection& i_connection)
    {
    static std::unique_ptr<char[]> p_buffer(new char[PACKET_SIZE]);
    for (size_t i = 0; i < PACKET_SIZE; ++i)
      p_buffer[i] = 'x';

    ToChar(Network::PacketType::PT_GovermentState, &p_buffer[0], sizeof(Network::PacketType));

    Network::SerializableData data(10, sizeof(float));
    Network::Packet packet(Network::PacketType::PT_GovermentState, PACKET_SIZE, &p_buffer[sizeof(Network::PacketType)]);

    auto& controllers = i_context.GetControllers();
    for (auto& controller : controllers)
      {
      auto& p_goverment = controller.mp_controller->GetGoverment();
      auto* p_economy_manager = p_goverment.GetEconomyManager();

      if (p_economy_manager->HasChanges())
        {        
        p_economy_manager->Serialize(data);
        p_economy_manager->ValidateChanges();
        data.Serialize(packet);
        i_connection.SendPacket(&p_buffer[0], data.GetSize()+sizeof(Network::PacketType));
        }
      }
    }

  void SendStates(const Slavs::GameContext& i_context, net::Connection& i_connection)
    {
    static std::unique_ptr<char[]> p_buffer(new char[PACKET_SIZE]);
    ObjectStateMap states_map;
    i_context.GetAllGameObjectsState(states_map);
    if (!states_map.empty())
      {
      std::for_each(states_map.begin(), states_map.end(), [&i_connection](std::pair<long, GameObjectState*> p)
        {
        size_t neededSize = p.second->NeededSize() + sizeof(Network::PacketType);
        ToChar(Network::PacketType::PT_GOState, &p_buffer[0], sizeof(Network::PacketType));

        p.second->Serialize(&p_buffer[sizeof(Network::PacketType)], neededSize);
        i_connection.SendPacket(&p_buffer[0], neededSize);
        });
      ClearSTLMap(states_map);
      }
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

    // move to tick listener
    //      one state in 100 ticks
    static long time_from_last_send = 0;
    time_from_last_send += i_elapsed_time;
    if (time_from_last_send > MS_FOR_TICK*100)
      {
      SendControllerStates(*mh_game_context, *ip_owner->GetConnection());
      time_from_last_send = 0;
      }
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
    Network::PacketType packet_type = (Network::PacketType)FromChar<int>(reinterpret_cast<char*>(ip_packet));
    switch (packet_type)
      {
      case Network::PacketType::PT_EndGame:
        ip_owner->Stop();
        return;
        break;
      case Network::PacketType::PT_Command:
        // hold
        {
        int controllers_mask = ip_owner->GetConnection()->GetAddress().GetAddress();
        auto& controllers = mh_game_context->GetControllers();
        auto it = std::find_if(controllers.begin(), controllers.end(), [controllers_mask](const GameContext::ControllerInformation& controller_info)
          {
          return controller_info.mp_controller->GetMask() == controllers_mask;
          });
        if (it != controllers.end())
          it->mp_controller->HoldPacket(ip_owner->GetConnection(), ip_packet, i_bytes_read);
        return;
        }
        break;
      }
    }

  } // Slavs