#include "GameController.h"

namespace SDK
	{

	GameController::GameController(Slavs::GameState& i_game_state, SwitchGameStateFunction i_switch_state_function, int i_base_state)
		: SwitchState(i_switch_state_function)
		, m_game_state(i_game_state)
		, m_base_state(i_base_state)
		, mp_current_task(nullptr)
		{
		
		}

	GameController::~GameController()
		{

		}

	void GameController::RequestStateChange(int i_next_state)
		{
		if (SwitchState)
			{
			if (!SwitchState(i_next_state))
				SwitchState(m_base_state);
			}
		}

	void GameController::UpdateTasks()
		{
		if (mp_current_task && mp_current_task->IsComplete())
			{
			mp_current_task.reset();
			if (!m_tasks_pull.empty())
				{
				mp_current_task = std::move(m_tasks_pull.back());
				m_tasks_pull.pop_back();
				}
			}
		}

	void GameController::Update(long i_elapsed_time)
		{
		UpdateTasks();
		if (mp_current_task)
			mp_current_task->Update(i_elapsed_time);
		}


	void GameController::RegisterTask(SDK::TaskPtr ip_task)
		{
		if (mp_current_task == nullptr)
			mp_current_task = std::move(ip_task);
		else
			m_tasks_pull.push_back(std::move(ip_task));
		}

	} // SDK