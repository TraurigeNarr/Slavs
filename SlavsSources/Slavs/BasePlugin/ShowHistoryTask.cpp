#include "stdafx.h"
#include "ShowHistoryTask.h"

#include "WorldMasteringTask.h"

#include "GameController.h"
#include "BaseObjectComposer.h"
#include "TypeEnumerations.h"
#include "HumanComponent.h"
#include "HouseComponent.h"

#include <SlavsServer/Main/GameController.h>
#include <SlavsServer/GameInfo.h>
#include <SlavsServer/Main/GameState.h>
#include <SlavsServer/Game/GameContext.h>
#include <SlavsServer/Game/GameObject.h>
#include <SlavsServer/Main/ServerMain.h>
#include <SlavsServer/include/IController.h>


namespace BasePlugin
	{

	ShowHistoryTask::ShowHistoryTask(int i_task_id, GameController& i_game_controller, int i_info_id, CompleteFunction i_complete_function)
		: Task(i_task_id, false)
		, m_game_controller(i_game_controller)
		, m_send_request(true)
		, m_info_id(i_info_id)
		, m_complete_function(i_complete_function)
		{		}

	ShowHistoryTask::~ShowHistoryTask()
		{}

	void ShowHistoryTask::CompleteImpl(const boost::any& i_value)
		{
		if (m_complete_function)
			m_complete_function(i_value, m_game_controller);
		}

	void ShowHistoryTask::DiscardImpl(const boost::any& i_value)
		{

		}

	void ShowHistoryTask::Update(long i_elapsed_time)
		{
		if (m_send_request)
			{
			m_game_controller.GetMainController()->GetGameInfo().m_requested_action = static_cast<int>(SDK::RequestedAction::ShowInformation);
			m_game_controller.GetMainController()->GetGameInfo().m_dialog_id = m_info_id;
			m_game_controller.GetMainController()->RequestStateChange(Slavs::GameState::State::PlayerDecision);
			m_send_request = false;
			return;
			}
		else if (m_game_controller.GetMainController()->GetGameState().GetGameFSMState() == Slavs::GameState::State::Play)
			{
			Complete(true);
			}
		}

	} // BasePlugin