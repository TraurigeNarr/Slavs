#pragma once

#include "GameStateBaseMessageProvider.h"

namespace ClientStates
  {

  enum class ManagerState
    {
    Good, Intermediate, Bad
    };

  class GovermentMessageProvider : public GameStateBaseMessageProvider
    {
    private:
      void _InitializeChildProviders();

    public:
      GovermentMessageProvider (GameState& i_state);
      virtual ~GovermentMessageProvider ();

      virtual void  Invalidate() override;
      virtual bool  HandlePacket(const Network::Packet& i_packet) override;
    };

  } // ClientStates