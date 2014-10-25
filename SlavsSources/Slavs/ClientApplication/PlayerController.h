#pragma once

#include <GameCore/IController.h>

class PlayerController : public IController
  {
  public:
    PlayerController (int i_mask, GameContext& i_context);
    virtual ~PlayerController();
  };