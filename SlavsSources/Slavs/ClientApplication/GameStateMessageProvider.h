#pragma once

#include "GameStateBaseMessageProvider.h"

namespace ClientStates
  {

  class GameState;

  class GameStateMessageProvider : public GameStateBaseMessageProvider
    {
    private:
      void _CreateProviders ();

    public:
      GameStateMessageProvider (GameState& i_state);
      virtual ~GameStateMessageProvider ();

      virtual void  Invalidate() override;
      virtual bool  HandlePacket(const Network::Packet& i_packet) override;
    };

  } // ClientStates