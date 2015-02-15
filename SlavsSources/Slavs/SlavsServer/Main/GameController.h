#ifndef __GAMECONTROLLER_H__
#define __GAMECONTROLLER_H__

#include "SlavsServerAPI.h"

#include "SlavsServer/Task.h"
#include "GameInfo.h"

#include <functional>
#include <vector>

namespace Slavs
	{
	class GameState;
	}

namespace SDK
	{

	class GameController
		{
		public:
			typedef std::function<bool(int)> SwitchGameStateFunction;
			
		private:
			SwitchGameStateFunction		SwitchState;
			int												m_base_state;

			std::vector<SDK::TaskPtr> m_tasks_pull;
			SDK::TaskPtr							mp_current_task;
			GameInfo									m_game_info;

			Slavs::GameState&					m_game_state;

		private:
			void UpdateTasks();
				
		public:
			// i_base_state - state in which game should be most of time;
			// in undefined situations last task will be completed and game will be switched to i_base_state
			SLAVS_SERVER_EXPORT GameController(Slavs::GameState& i_game_state, SwitchGameStateFunction i_switch_state_function, int i_base_state);
			SLAVS_SERVER_EXPORT ~GameController();

			SLAVS_SERVER_EXPORT void		Update(long i_elapsed_time);

			// should be in task manager
			SLAVS_SERVER_EXPORT void		RegisterTask(SDK::TaskPtr ip_task);

			GameInfo&										GetGameInfo() { return m_game_info; }
			Slavs::GameState&						GetGameState() { return m_game_state; }
			SLAVS_SERVER_EXPORT void		RequestStateChange(int i_next_state);

			bool			IsGameOver() const { return m_tasks_pull.empty() && mp_current_task == nullptr; }
		};

	} // SDK

#endif