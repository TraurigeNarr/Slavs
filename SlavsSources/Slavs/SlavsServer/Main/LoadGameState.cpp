#include "LoadGameState.h"

#include "ServerMain.h"
#include "GameState.h"

#include "Game\GameContext.h"
#include "Game\GameObject.h"
#include "IO\GameSerializer.h"
#include "LoadingStages.h"

#include "SPlayerController.h"

#include <Utilities/TemplateFunctions.h>

#include <Network/include/Net.h>

#include <memory>

//////////////////////////////////////////////////////////////////////////

namespace Slavs
  {

  LoadGameState::LoadGameState(const LoadingParameters& i_loading_parameters)
    : m_loading_parameters(i_loading_parameters)
    , mp_loading_fsm(nullptr)
    {
    }

  LoadGameState::~LoadGameState()
    {    }

  void LoadGameState::Enter(ServerMain* ip_owner)
    {
    printf( "Enters LoadGame state\n" );
    mh_game_context.reset(new GameContext());

    net::Connection& connection = *ip_owner->GetConnection();
    // set player controller; now it is only one PC but it is needed to move this to separate class
    //mh_game_context->RegisterController( std::unique_ptr<IController>
    //                          (
    //                          new SPlayerController(connection.GetAddress().GetAddress(), *mh_game_context)
    //                          )
    //                       );
    // also set AI controllers
    // TODO: code here

    // load game
    //GameSerializer serializer(*mh_game_context);
    //serializer.LoadConfigurations(m_loading_parameters.m_configurations_path);
    //serializer.LoadGame(m_loading_parameters.m_map_path);

    mp_loading_fsm.reset(
                    new LoadingStages::LoadingFSM(boost::ref(*mh_game_context.get()),
                                                  m_loading_parameters,
                                                  &connection, 
                                                  ip_owner
                         ));
    mp_loading_fsm->start();
    }

  void LoadGameState::Execute(ServerMain* ip_owner, long i_elapsed_time)
    {
    std::unique_ptr<unsigned char[]> packet(new unsigned char[PACKET_SIZE]);
    while(true)
      {
      int bytes_read = ip_owner->GetConnection()->ReceivePacket( &packet[0], PACKET_SIZE );
      if(0 == bytes_read)
        break;

      HoldPacket(ip_owner, &packet[0], bytes_read);
      // if in Wait state
      if ( mp_loading_fsm->current_state()[0] == LoadingStages::LAST_STATE_INDEX)
        {
        mp_loading_fsm->stop();
        ip_owner->GetStateMachine().ChangeState( std::make_shared<GameState>(std::move(mh_game_context)) );
        return;
        }
      }
    }

  void LoadGameState::Exit(ServerMain* ip_owner)
    {
    printf( "Exits LoadGame state\n" );
    }

  void LoadGameState::HoldPacket(ServerMain* ip_owner, unsigned char* ip_packet, size_t i_bytes_read)
    {
    Network::PacketType pType = (Network::PacketType)FromChar<int>((char*)ip_packet);
    char *packetToClient = NULL;
    switch(pType)
      {
      case Network::PacketType::PT_Achived:
        {
        std::unique_ptr<char[]> p_buffer(new char[sizeof(Network::PacketType)+1]);
        ToChar(Network::PacketType::PT_Connects, &p_buffer[0], sizeof(Network::PacketType));
        ip_owner->GetConnection()->SendPacket( &p_buffer[0], sizeof(Network::PacketType)+1);
        mp_loading_fsm->process_event(LoadingStages::ConfirmAchievement());
        }
        break;
      }
    }

  }