#pragma once

#include <GameCore/GameObject.h>

class GameObjectState;

class ClientGameObject : public GameObject
  {
  public:
    ClientGameObject(long i_id, int i_type, GameContext& i_context, IController* ip_owner);
    ~ClientGameObject();

    void ApplyState(GameObjectState& i_state);
  };