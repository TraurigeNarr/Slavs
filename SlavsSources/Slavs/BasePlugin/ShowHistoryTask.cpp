#include "stdafx.h"
#include "ShowHistoryTask.h"

#include "WorldMasteringTask.h"

#include "GameController.h"
#include <SlavsServer/Main/GameController.h>
#include <SlavsServer/GameInfo.h>
#include <SlavsServer/Main/GameState.h>

namespace BasePlugin
	{

	ShowHistoryTask::ShowHistoryTask(int i_task_id, GameController& i_game_controller)
		: Task(i_task_id, false)
		, m_game_controller(i_game_controller)
		, m_send_request(true)
		{		}

	ShowHistoryTask::~ShowHistoryTask()
		{}

	void ShowHistoryTask::CompleteImpl(const boost::any& i_value)
		{
		SDK::TaskPtr p_task(new WorldMasteringTask(1, m_game_controller));
		m_game_controller.GetMainController()->RegisterTask(std::move(p_task));
		}

	void ShowHistoryTask::DiscardImpl(const boost::any& i_value)
		{

		}

	void ShowHistoryTask::Update(long i_elapsed_time)
		{
		if (m_send_request)
			{
			m_game_controller.GetMainController()->GetGameInfo().m_requested_action = static_cast<int>(SDK::RequestedAction::ShowInformation);
			m_game_controller.GetMainController()->GetGameInfo().m_dialog_id = 1;
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