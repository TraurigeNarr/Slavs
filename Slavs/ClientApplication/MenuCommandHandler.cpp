#include "stdafx.h"

#include "MenuCommandHandler.h"

#include "Application.h"
#include "UIEvents.h"
#include "LoadingState.h"

#include <Patterns/MessageDispatcher/MessageDispatcher.h>

#include <Network/include/Net.h>

#include <Common/Patterns/StateMachine.h>

using namespace ClientStates;

namespace UI
  {

  MenuCommandHandler::MenuCommandHandler (Application& i_application, MessageDispatcher& i_dispatcher)
    : m_dispatcher(i_dispatcher)
    , m_application(i_application)
    {
    m_dispatcher.RegisterHandler<MenuCommandHandler, const ButtonPressed&>(*this, &MenuCommandHandler::ButtonPressedHandler);
    }

  MenuCommandHandler::~MenuCommandHandler()
    {
    m_dispatcher.UnregisterHandler<ButtonPressed>();
    }

  void MenuCommandHandler::ButtonPressedHandler(const ButtonPressed& i_button_pressed)
    {
    switch (i_button_pressed.m_button)
      {
      case ButtonID::BI_EXIT:
        m_application.Shutdown();
        break;
      case ButtonID::BI_CONNECT:
        {
        net::Address address(127, 0, 0, 1, net::ServerPort);
        m_application.GetStateMachine().ChangeState(std::make_shared<LoadingState>(m_application, address));
        }
        break;
      }
    }

  } // UI