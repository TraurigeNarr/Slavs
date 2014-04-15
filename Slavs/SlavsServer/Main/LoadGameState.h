#pragma once

#include <Patterns/State.h>

#include <boost/msm/back/state_machine.hpp>

#include "Types.h"

class ServerMain;

namespace Slavs
  {

  namespace LoadingStages
    {
    namespace msm = boost::msm;
    namespace mpl = boost::mpl;
    struct loading_;
    typedef msm::back::state_machine<loading_> LoadingFSM;
    }

  class LoadGameState : public State<ServerMain, long>
    {
    TGameContext mh_game_context;

    std::unique_ptr<LoadingStages::LoadingFSM> mp_loading_fsm;

    public:
      LoadGameState(TGameContext ih_game_context);
      ~LoadGameState();

      virtual void          Enter(ServerMain* ip_owner) override;
      virtual void          Execute(ServerMain* ip_owner, long i_elapsed_time) override;
      virtual void          Exit(ServerMain* ip_owner) override;

      void                  HoldPacket(ServerMain* ip_owner, unsigned char* ip_packet, size_t i_bytes_read);
    };

  }