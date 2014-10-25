#include "stdafx.h"

#include "GameStateMessageProvider.h"
#include "GameState.h"
#include "GovermentMessageProvider.h"

#include <Network/Packet.h>

//////////////////////////////////////////////////////////////////////////

namespace ClientStates
  {

  GameStateMessageProvider::GameStateMessageProvider (GameState& i_state)
    : GameStateBaseMessageProvider(i_state)
    {
    _CreateProviders();
    }

  GameStateMessageProvider::~GameStateMessageProvider()
    {    }

  void GameStateMessageProvider::_CreateProviders()
    {
    m_providers.push_back(ProviderPtr(new GovermentMessageProvider(m_state)));
    }

  void GameStateMessageProvider::Invalidate()
    {
    __super::Invalidate();
    }

  bool GameStateMessageProvider::HandlePacket(const Network::Packet& i_packet)
    {
    for (auto& p_provider : m_providers)
      {
      if (p_provider->HandlePacket(i_packet))
        {
        Invalidate();
        return true;
        }
      }

    return false;
    }

  } // ClientStates