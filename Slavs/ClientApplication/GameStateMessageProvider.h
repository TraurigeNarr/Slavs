#pragma once

#include "IMessageProvider.h"

namespace ClientStates
  {

  class GameState;

  class GameStateMessageProvider : public UI::IMessageProvider
    {
    private:
      GameState&    m_state;

    public:
      GameStateMessageProvider (GameState& i_state);
      virtual ~GameStateMessageProvider ();

      virtual void  Invalidate() override;
    };

  } // ClientStates