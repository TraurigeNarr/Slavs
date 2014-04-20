#pragma once

#include <Patterns/State.h>

#include <boost/msm/back/state_machine.hpp>

#include "Types.h"

class ServerMain;
class IController;

namespace Slavs
  {
  struct LoadingParameters
    {
    std::string m_map_name;
    // may be some other parameters:
    //  1. Connections
    //  2. Players
    //  3. Settings
    };

  namespace LoadingStages
    {
    namespace msm = boost::msm;
    namespace mpl = boost::mpl;
    struct loading_;
    typedef msm::back::state_machine<loading_> LoadingFSM;
    }

  class LoadGameState : public State<ServerMain, long>
    {
    TGameContext                                mh_game_context;
    LoadingParameters                           m_loading_parameters;
    std::unique_ptr<LoadingStages::LoadingFSM>  mp_loading_fsm;
    std::vector<std::unique_ptr<IController>>   m_controllers;

    public:
      LoadGameState(const LoadingParameters& i_loading_parameters);
      ~LoadGameState();

      virtual void          Enter(ServerMain* ip_owner) override;
      virtual void          Execute(ServerMain* ip_owner, long i_elapsed_time) override;
      virtual void          Exit(ServerMain* ip_owner) override;

      void                  HoldPacket(ServerMain* ip_owner, unsigned char* ip_packet, size_t i_bytes_read);
    };

  }