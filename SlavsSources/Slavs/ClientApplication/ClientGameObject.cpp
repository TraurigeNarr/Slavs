#include "stdafx.h"

#include "ClientGameObject.h"

#include "Application.h"
#include "GameState.h"

#include "InformationMessageProvider.h"

#include <Common/Game/GameObjectState.h>
#include <Common/Patterns/StateMachine.h>

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
	
	m_selected = (i_state.iFlags & GOF_Selected) == GOF_Selected;

	if (i_state.informationToShow != InformationToShow::None)
		{
		auto p_game_state = static_cast<ClientStates::GameState*>(ClientGame::appInstance.GetStateMachine().GetCurrentState().get());
		auto p_message_provider = p_game_state->GetMessageProvider()->GetProvider<ClientStates::InformationMessageProvider>();
		UI::WindowType window_type = UI::WindowType::None;
		switch (i_state.informationToShow)
			{
			case InformationToShow::Dialog:
				window_type = UI::WindowType::Dialog;
				break;
			case InformationToShow::Information:
				window_type = UI::WindowType::Information;
				break;
			}

		p_message_provider->AddInformation(window_type, i_state.informationId);
		}

  StateChanged();
  }