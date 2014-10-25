#pragma once

#include "IMessageProvider.h"

namespace Network
  {
  struct Packet;
  } // Network

namespace ClientStates
  {

  class GameState;

  class GameStateBaseMessageProvider : public UI::IMessageProvider
    {
    public:
      typedef std::unique_ptr<GameStateBaseMessageProvider> ProviderPtr;

    protected:
      GameState&                m_state;
      std::vector<ProviderPtr>  m_providers;

    public:
      GameStateBaseMessageProvider(GameState& i_state)
        : m_state (i_state)
        {        }
      virtual ~GameStateBaseMessageProvider(){}

      virtual bool HandlePacket (const Network::Packet& i_packet) = 0;

      template <typename ProviderType>
      ProviderType* GetProvider() const;
    };

  //////////////////////////////////////////////////////////////////////////

  template <typename ProviderType>
  inline ProviderType* GameStateBaseMessageProvider::GetProvider() const
    {
    for (const ProviderPtr& provider : m_providers)
      if (auto p_provider = dynamic_cast<ProviderType*>(provider.get()))
        return p_provider;
    return nullptr;
    }

  } // ClientStates