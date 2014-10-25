#pragma once

#include "GameStateBaseMessageProvider.h"

namespace Network
  {
  struct Packet;
  } // Network

namespace ClientStates
  {

  class GameState;
  enum class ManagerState;

  class EconomySystemMessageProvider : public GameStateBaseMessageProvider
    {
    private:
      ManagerState  m_economy_state;
      int           m_number_of_stores;
    public:
      EconomySystemMessageProvider(GameState& i_state);
      virtual ~EconomySystemMessageProvider();

      virtual void Invalidate() override;

      virtual bool HandlePacket (const Network::Packet& i_packet) override;

      ManagerState  GetEconomySystemState() const;
      size_t        GetNumberOfStores() const;
    };

  //////////////////////////////////////////////////////////////////////////

  inline ManagerState EconomySystemMessageProvider::GetEconomySystemState() const
    {
    return m_economy_state;
    }

  inline size_t EconomySystemMessageProvider::GetNumberOfStores() const
    {
    return m_number_of_stores;
    }

  } // ClientStates