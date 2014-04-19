#include "LoadGameState.h"

#include "ServerMain.h"

#include "Game\GameContext.h"
#include "Game\GameObject.h"
#include "LoadingStages.h"

#include <Utilities/TemplateFunctions.h>

#include <Network/include/Net.h>

#include <memory>

//////////////////////////////////////////////////////////////////////////

namespace Slavs
  {

  LoadGameState::LoadGameState(TGameContext ih_game_context)
    : mh_game_context(std::move(ih_game_context))
    , mp_loading_fsm(nullptr)
    { 
    
    }

  LoadGameState::~LoadGameState()
    {    }

  void LoadGameState::Enter(ServerMain* ip_owner)
    {
    printf( "Enters LoadGame state\n" );
    net::Connection &connection = *ip_owner->GetConnection();
    mp_loading_fsm.reset(new LoadingStages::LoadingFSM(&connection, ip_owner, boost::ref(*mh_game_context.get())));
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
      if ( mp_loading_fsm->current_state()[0] == 2)
        {
        // switch to another state
        mp_loading_fsm->stop();
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
    PacketType pType = (PacketType)FromChar<int>((char*)ip_packet);
    char *packetToClient = NULL;
    switch(pType)
      {
      case PT_Achived:
          mp_loading_fsm->process_event(LoadingStages::ConfirmAchievement());
        break;
      }
    }

  }