#include "stdafx.h"

#include "MenuCommandHandler.h"

#include "Application.h"
#include "UIEvents.h"

#include "MessageDispatcher.h"

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
      }
    }

  } // UI