#include "stdafx.h"

#include "GameBaseCommandHandler.h"

#include "Application.h"
#include "UIEvents.h"
#include "GameState.h"
#include "MenuState.h"

#include <Network/include/Connection.h>
#include <Network/PacketType.h>

#include <Patterns/MessageDispatcher/MessageDispatcher.h>

#include <Common/Patterns/StateMachine.h>
#include <Common/Utilities/TemplateFunctions.h>

using namespace ClientStates;

namespace UI
  {

  GameBaseCommandHandler::GameBaseCommandHandler (ClientStates::GameState& i_state, Application& i_application, MessageDispatcher& i_dispatcher)
    : m_game_state(i_state)
    , m_dispatcher(i_dispatcher)
    , m_application(i_application)
    {
    m_dispatcher.RegisterHandler<GameBaseCommandHandler, const ButtonPressed&>(*this, &GameBaseCommandHandler::ButtonPressedHandler);
    m_dispatcher.RegisterHandler<GameBaseCommandHandler, const CommandButtonPressed&>(*this, &GameBaseCommandHandler::CommandButtonPressedHandler);
    }

  GameBaseCommandHandler::~GameBaseCommandHandler()
    {
    m_dispatcher.UnregisterHandler<ButtonPressed>();
    }

  void GameBaseCommandHandler::ButtonPressedHandler(const ButtonPressed& i_button_pressed)
    {
    switch (i_button_pressed.m_button)
      {
      case ButtonID::BI_BACK_TO_MENU:
        m_application.GetStateMachine().ChangeState(std::make_shared<MenuState>(m_application));
        break;
      case ButtonID::BI_HELP:
        break;
      case ButtonID::BI_ECONOMY_PLUGIN_STATUS:
        break;
      }
    }

  void GameBaseCommandHandler::CommandButtonPressedHandler(const CommandButtonPressed& i_command_button_pressed)
    {
    net::Connection& connection = m_game_state.GetConnection();

    float x_pos = 1000.f;
    float y_pos = 1000.f;

    const size_t packet_size = sizeof(Network::PacketType) + sizeof(int) + sizeof(float) + sizeof(float);
    std::unique_ptr<char[]> p_buffer(new char[packet_size]);
    size_t offset = 0;
    ToChar(Network::PacketType::PT_Command, &p_buffer[0], sizeof(Network::PacketType));
    offset += sizeof(Network::PacketType);
    ToChar(i_command_button_pressed.m_command_id, &p_buffer[offset], sizeof(int));
    offset += sizeof(int);
    ToChar(x_pos, &p_buffer[offset], sizeof(float));
    offset += sizeof(float);
    ToChar(y_pos, &p_buffer[offset], sizeof(float));
    connection.SendPacket(&p_buffer[0], packet_size);
    }

  } // UI