#include "stdafx.h"

#include "GameBaseCommandHandler.h"

#include "Application.h"
#include "UIEvents.h"
#include "GameState.h"
#include "MenuState.h"

#include <Patterns/MessageDispatcher/MessageDispatcher.h>

#include <Common/Patterns/StateMachine.h>

using namespace ClientStates;

namespace UI
  {

  GameBaseCommandHandler::GameBaseCommandHandler (ClientStates::GameState& i_state, Application& i_application, MessageDispatcher& i_dispatcher)
    : m_game_state(i_state)
    , m_dispatcher(i_dispatcher)
    , m_application(i_application)
    {
    m_dispatcher.RegisterHandler<GameBaseCommandHandler, const ButtonPressed&>(*this, &GameBaseCommandHandler::ButtonPressedHandler);
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

  } // UI