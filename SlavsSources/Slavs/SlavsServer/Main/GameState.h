#pragma once

#include "Types.h"

#include <Patterns/State.h>

#include <memory>

class ServerMain;
class TimeController;

namespace Slavs
  {

  class GameState : public State<ServerMain, long>
    {
    public:
      typedef std::shared_ptr<TimeController> TimeControllerPtr;

    private:
      TGameContext mh_game_context;

      TimeControllerPtr mp_time_controller;

    private:
      void                  HoldPacket(ServerMain* ip_owner, unsigned char* ip_packet, size_t i_bytes_read);

    public:
      GameState(TGameContext ih_game_context);
      ~GameState();

			GameContext* GetContext() const { return mh_game_context.get(); }

    // State
    public:
      virtual void          Enter(ServerMain* ip_owner) override;
      virtual void          Execute(ServerMain* ip_owner, long i_elapsed_time) override;
      virtual void          Exit(ServerMain* ip_owner) override;

    // GameState
    public:

    };

  }