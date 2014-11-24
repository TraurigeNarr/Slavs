#include "stdafx.h"

#include "EconomySystemMessageProvider.h"
#include "GovermentMessageProvider.h"

#include "GameState.h"

#include <Network/Packet.h>
#include <Network/SerializableData.h>

namespace ClientStates
  {

  EconomySystemMessageProvider::EconomySystemMessageProvider (GameState& i_state)
    : GameStateBaseMessageProvider(i_state)
    , m_economy_state(ManagerState::Good)
    , m_number_of_stores(0)
    {  }

  EconomySystemMessageProvider::~EconomySystemMessageProvider()
    {    }

  void EconomySystemMessageProvider::Invalidate()
    {
    __super::Invalidate();
    }

  bool EconomySystemMessageProvider::HandlePacket(const Network::Packet& i_packet)
    {
    if (i_packet.m_packet != Network::PacketType::PT_GovermentState)
      return false;

    Network::SerializableData data;
    data.Deserialize(i_packet);

    m_economy_state = static_cast<ManagerState>(data.GetDataWithType<char>(0));
    m_number_of_stores = 5;// bug with casting of this data type data.GetDataWithType<size_t>(1);
    Invalidate();
    return true;
    }

  } // ClientStates