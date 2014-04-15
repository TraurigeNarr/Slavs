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
    // initialize Loading stages
    mp_loading_fsm.reset(new LoadingStages::LoadingFSM(&connection, ip_owner, boost::ref(*mh_game_context.get())));
    mp_loading_fsm->start();

    /*std::unique_ptr<char[]> h_buffer(new char[PACKET_SIZE]);
    // send definitions
    const MetaFactory::TDefinitionsMap& definitions = ip_owner->GetMetaFactory().GetDefinitions();
    std::for_each(definitions.begin(), definitions.end(), [&h_buffer, &connection](std::pair<std::string, int> i_definition)
      {
      size_t needed_size = sizeof(PacketType) + i_definition.first.size() + sizeof(int) + 1;
      ToChar(PT_Definitions, &h_buffer[0], sizeof(PacketType));
      i_definition.first.copy(&h_buffer[sizeof(PacketType)], i_definition.first.size());
      h_buffer[sizeof(PacketType)+i_definition.first.size()] = ';';
      ToChar(i_definition.second, &h_buffer[sizeof(PacketType)+i_definition.first.size()+1], sizeof(int));
      connection.SendPacket(&h_buffer[0], needed_size);
      });*/

    /*int neededSize = 0;
    
    std::for_each(sendMap.begin(), sendMap.end(), [&neededSize, &connection](std::pair<long, GameObjectState*> p)
      {
      neededSize = p.second->NeededSize() + sizeof(PacketType);
      char *buf = new char[neededSize];
      ToChar(PT_GOState, buf, sizeof(PacketType));
      char *buf_end = buf + sizeof(PacketType);

      p.second->Serialize(buf_end, neededSize);
      connection.SendPacket(buf, neededSize);
      delete []buf;
      });
    //clear map
    ClearSTLMap(sendMap);
    //after sending all states send msg about server ready state
    char *packetToClient = new char[sizeof(PacketType)];
    ToChar(PT_ServerReady, packetToClient, sizeof(PacketType));
    ip_owner->GetConnection()->SendPacket( packetToClient, sizeof( PacketType ) );
    //wait for respond of client that it receives the packet
    delete packetToClient;*/
    }

  void LoadGameState::Execute(ServerMain* ip_owner, long i_elapsed_time)
    {
    // 1. Send packets
    // 2. Send objects
    // 3. Wait for completion of loading on clients side

    std::unique_ptr<unsigned char[]> packet(new unsigned char[PACKET_SIZE]);
    while(true)
      {
      int bytes_read = ip_owner->GetConnection()->ReceivePacket( &packet[0], PACKET_SIZE );
      if(0 == bytes_read)
        break;

      HoldPacket(ip_owner, &packet[0], bytes_read);
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