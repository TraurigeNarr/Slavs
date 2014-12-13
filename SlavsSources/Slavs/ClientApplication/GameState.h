#pragma once

#include "ApplicationStateBase.h"

#include "SendSelectionTask.h"

namespace net
  {
  class Connection;
  }

namespace Network
  {
  class   PacketProvicer;
  struct  Packet;
  }

namespace UI
  {
  class IMessageProvider;
  class GameBaseCommandHandler;
  } // UI

class TimeController;

template <typename Owner, typename Parameter>
class StateMachine;

class ClientGameContext;

namespace ClientGame
  {
  class Camera;
  }

namespace ClientStates
  {

  class GameStateBaseMessageProvider;

  class GameState : public ApplicationStateBase
    {
    private:
      std::unique_ptr<net::Connection>                      mp_connection;

      std::unique_ptr< StateMachine<GameState, long> >      mp_state_machine;

      std::unique_ptr<Network::PacketProvicer>              mp_packet_provider;
      std::unique_ptr<GameStateBaseMessageProvider>         mp_message_provider;

      std::unique_ptr<ClientGameContext>                    mp_context;
      std::unique_ptr<UI::GameBaseCommandHandler>           mp_command_handler;
      std::unique_ptr<TimeController>                       mp_time_controller;

      std::unique_ptr<ClientGame::Camera>                   mp_camera;

			Tasks::SendSelcetionTask															m_send_selection_task;

    private:
      void        _HoldPacket(const Network::Packet& i_packet);

    public:
      GameState(Application& i_application, 
                std::unique_ptr<ClientGameContext> ip_game_context, 
                std::unique_ptr<Network::PacketProvicer> ip_packet_provider, 
                std::unique_ptr<net::Connection> ip_connection);
      virtual ~GameState();

      // State
    public:
      virtual void Enter(Application* ip_owner) override;
      virtual void Execute(Application* ip_owner, long i_elapsed_time) override;
      virtual void Exit(Application* ip_owner) override;

      StateMachine<GameState, long>& GetStateMachine();
      GameStateBaseMessageProvider* GetMessageProvider() const;
      ClientGameContext& GetContext();
      net::Connection&   GetConnection();
    };

  //////////////////////////////////////////////////////////////////////////

  inline StateMachine<GameState, long>& GameState::GetStateMachine()
    {
    return *mp_state_machine;
    }

  inline GameStateBaseMessageProvider* GameState::GetMessageProvider() const
    {
    return mp_message_provider.get();
    }

  inline ClientGameContext& GameState::GetContext()
    {
    return *mp_context;
    }

  inline net::Connection& GameState::GetConnection()
    {
    return *mp_connection;
    }

  } // ClientStates