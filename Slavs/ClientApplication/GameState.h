#pragma once

#include "ApplicationStateBase.h"

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

  class GameState : public ApplicationStateBase
    {
    private:
      std::unique_ptr<net::Connection>                      mp_connection;

      std::unique_ptr< StateMachine<GameState, long> >      mp_state_machine;

      std::unique_ptr<Network::PacketProvicer>              mp_packet_provider;
      std::shared_ptr<UI::IMessageProvider>                 mp_message_provider;

      std::unique_ptr<ClientGameContext>                    mp_context;
      std::unique_ptr<UI::GameBaseCommandHandler>           mp_command_handler;
      std::unique_ptr<TimeController>                       mp_time_controller;

      std::unique_ptr<ClientGame::Camera>                   mp_camera;

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
      std::shared_ptr<UI::IMessageProvider> GetMessageProvider() const;
      ClientGameContext& GetContext();
    };

  //////////////////////////////////////////////////////////////////////////

  inline StateMachine<GameState, long>& GameState::GetStateMachine()
    {
    return *mp_state_machine;
    }

  inline std::shared_ptr<UI::IMessageProvider> GameState::GetMessageProvider() const
    {
    return mp_message_provider;
    }

  inline ClientGameContext& GameState::GetContext()
    {
    return *mp_context;
    }

  } // ClientStates