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

	ShowHistoryTask::ShowHistoryTask(int i_task_id, GameController& i_game_controller)
		: Task(i_task_id, false)
		, m_game_controller(i_game_controller)
		, m_send_request(true)
		{		}

	ShowHistoryTask::~ShowHistoryTask()
		{}

	void ShowHistoryTask::CompleteImpl(const boost::any& i_value)
		{
		auto p_context = m_game_controller.GetMainController()->GetGameState().GetContext();

		const std::vector<Slavs::GameContext::ControllerInformation>& controllers = p_context->GetControllers();
		IController* p_player_controller = nullptr;
		for (const auto& info : controllers)
			{
			if (info.mp_controller->GetMask() > 100)
				{
				p_player_controller = info.mp_controller.get();
				break;
				}
			}

		auto spawn = p_context->GetSpawn(p_player_controller->GetMask());

		int human_id = m_game_controller.GetObjectComposer().GetObjectGlobalID(ObjectType::OT_HUMAN);
		int hizhina_id = m_game_controller.GetObjectComposer().GetObjectGlobalID(ObjectType::OT_HIZHINA);
		auto p_hizhina = p_context->AddObject(hizhina_id, Vector2D(spawn.m_x, spawn.m_y), p_player_controller->GetMask(), 0);
		auto p_human = p_context->AddObject(human_id, Vector2D(spawn.m_x - 500, spawn.m_y), p_player_controller->GetMask(), 0);
		auto p_human_component = p_human->GetComponent<HumanComponent>();
		assert(p_human_component);
		p_human_component->SetHome(p_hizhina->GetComponent<HouseComponent>());

		SDK::TaskPtr p_task(new WorldMasteringTask(1, m_game_controller, p_player_controller));
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