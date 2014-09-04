#include "stdafx.h"

#include "ClientGameObject.h"

#include <Common/Game/GameObjectState.h>

//////////////////////////////////////////////////////////////////////////

ClientGameObject::ClientGameObject(long i_id, int i_type, GameContext& i_context, IController* ip_owner)
  : GameObject(i_id, i_type, i_context, ip_owner)
  {  }

ClientGameObject::~ClientGameObject()
  {  }

void ClientGameObject::ApplyState(GameObjectState& i_state)
  {
  if(nullptr != i_state.pPosition)
    SetPosition(*i_state.pPosition);
  StateChanged();
  }