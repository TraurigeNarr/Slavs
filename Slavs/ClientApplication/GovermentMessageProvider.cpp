#include "stdafx.h"

#include "GovermentMessageProvider.h"
#include "EconomySystemMessageProvider.h"

#include "GameState.h"

#include <Network/Packet.h>

namespace ClientStates
  {

  GovermentMessageProvider::GovermentMessageProvider (GameState& i_state)
    : GameStateBaseMessageProvider(i_state)
    {
    _InitializeChildProviders();
    }

  GovermentMessageProvider::~GovermentMessageProvider()
    {    }

  void GovermentMessageProvider::_InitializeChildProviders()
    {
    m_providers.push_back(ProviderPtr(new EconomySystemMessageProvider(m_state)));
    }

  void GovermentMessageProvider::Invalidate()
    {
    __super::Invalidate();
    }

  bool GovermentMessageProvider::HandlePacket(const Network::Packet& i_packet)
    {
    if (i_packet.m_packet != Network::PacketType::PT_GovermentState)
      return false;

    for (auto& p_provider : m_providers)
      if (p_provider->HandlePacket(i_packet))
        {
        Invalidate();
        return true;
        }

    return false;
    }

  } // ClientStates