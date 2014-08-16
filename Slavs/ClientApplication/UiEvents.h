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

  } // UI