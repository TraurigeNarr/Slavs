#include "stdafx.h"

#include "GameState.h"

#include "ClientGameContext.h"
#include "PlayerController.h"
#include "GameBaseCommandHandler.h"
#include "GameStateMessageProvider.h"

#include "Application.h"
#include "InputManager.h"

#include "ScreenManager.h"
#include "GameScreenMain.h"

#include "PacketProvider.h"
#include "PacketType.h"

#include <Common/Patterns/StateMachine.h>
#include <Common/Utilities/TemplateFunctions.h>

#include <Network/include/Net.h>

#include <exception>

//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////

namespace ClientStates
  {

  GameState::GameState(Application& i_application, 
                       std::unique_ptr<ClientGameContext> ip_game_context, 
                       std::unique_ptr<Network::PacketProvicer> ip_packet_provider, 
                       std::unique_ptr<net::Connection> ip_connection)
    : ApplicationStateBase(i_application)
    , mp_context(std::move(ip_game_context))
    , mp_packet_provider(std::move(ip_packet_provider))
    , mp_connection(std::move(ip_connection))
    , mp_state_machine(nullptr)
    , mp_message_provider(nullptr)
    , mp_command_handler(nullptr)
    {    }

  GameState::~GameState()
    {    }

  void GameState::Enter(Application* ip_application)
    {
    // set screen
    using namespace UI;
    ScreenManager& screen_manager = ip_application->GetScreenManager();
    screen_manager.SetCurrentScreen(std::unique_ptr<Screen>(new GameScreenMain(screen_manager)));

    // set message provider
    mp_message_provider = std::make_shared<GameStateMessageProvider>(*this);
    screen_manager.GetCurrentScreen()->SetMessageProvider(mp_message_provider);

    // set message handler
    mp_command_handler.reset(new GameBaseCommandHandler(*this, m_application, screen_manager.GetMessageDispatcher()));

    // set state machine
    mp_state_machine.reset(new StateMachine<GameState, long>(this));

    m_application.GetInputManager().AddSubscriber(this);
    }

  void GameState::Execute(Application* ip_application, long i_elapsed_time)
    {
    
    }

  void GameState::Exit(Application* ip_application)
    {
    std::unique_ptr<char[]> p_buffer(new char[sizeof(Network::PacketType)+1]);
    ToChar(Network::PacketType::PT_EndGame, &p_buffer[0], sizeof(Network::PacketType));
    mp_connection->SendPacket( &p_buffer[0], sizeof(Network::PacketType)+1);

    mp_connection.reset();
    mp_state_machine.reset();
    mp_packet_provider.reset();
    mp_command_handler.reset();

    m_application.GetInputManager().RemoveSubscriber(this);
    }

  } // ClientStates