#pragma once

#include "ApplicationStateBase.h"

#include <Network/include/Address.h>

namespace net
  {
  class Connection;
  }

namespace Network
  {
  class PacketProvicer;
  }

namespace UI
  {
  class IMessageProvider;
  } // UI

template <typename Owner, typename Parameter>
class StateMachine;

class ClientGameContext;

namespace ClientStates
  {

  class LoadingState : public ApplicationStateBase
    {
    private:
      std::unique_ptr<net::Connection>                      mp_connection;

      std::unique_ptr< StateMachine<LoadingState, long> >   mp_state_machine;

      net::Address                                          m_connection_address;

      std::unique_ptr<Network::PacketProvicer>              mp_packet_provider;
      std::unique_ptr<UI::IMessageProvider>                 mp_message_provider;

      std::unique_ptr<ClientGameContext>                    mp_context;

    private:
      net::Address  _GetClientAddress() const;

    public:
      LoadingState(Application& i_application, const net::Address& i_connection_address);
      virtual ~LoadingState();

      // State
    public:
      virtual void Enter(Application* ip_owner) override;
      virtual void Execute(Application* ip_owner, long i_elapsed_time) override;
      virtual void Exit(Application* ip_owner) override;

      net::Address GetConnectionAddress() const;
      StateMachine<LoadingState, long>& GetStateMachine();
      UI::IMessageProvider* GetMessageProvider() const;
      ClientGameContext& GetContext();
    };

  //////////////////////////////////////////////////////////////////////////

  inline net::Address LoadingState::GetConnectionAddress() const
    {
    return m_connection_address;
    }

  inline StateMachine<LoadingState, long>& LoadingState::GetStateMachine()
    {
    return *mp_state_machine;
    }

  inline UI::IMessageProvider* LoadingState::GetMessageProvider() const
    {
    return mp_message_provider.get();
    }

  inline ClientGameContext& LoadingState::GetContext()
    {
    return *mp_context;
    }

  } // ClientStates