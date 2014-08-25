#include "stdafx.h"

#include "GameStateMessageProvider.h"

#include "GameState.h"

namespace ClientStates
  {

  GameStateMessageProvider::GameStateMessageProvider (GameState& i_state)
    : m_state(i_state)
    {    }

  GameStateMessageProvider::~GameStateMessageProvider()
    {    }

  void GameStateMessageProvider::Invalidate()
    {

    }

  } // ClientStates