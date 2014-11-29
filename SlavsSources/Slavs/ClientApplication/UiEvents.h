#pragma once

#include <Patterns/MessageDispatcher/Event.h>

#include "ButtonID.h"

namespace UI
  {

  struct ButtonPressed : public Event
    {
    const ButtonID m_button;

    ButtonPressed(ButtonID i_id)
      : m_button(i_id)
      {      }
    };

  struct CommandButtonPressed : public Event
    {
    const int       m_command_id;

    CommandButtonPressed(int i_command_id)
      : m_command_id(i_command_id)
      {      }
    };

  } // UI