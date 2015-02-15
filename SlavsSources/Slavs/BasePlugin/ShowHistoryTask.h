#ifndef __SHOWHISTORY_H__
#define __SHOWHISTORY_H__

#include <SlavsServer/Task.h>

namespace BasePlugin
	{
	class GameController;

	class ShowHistoryTask : public SDK::Task
		{
		private:
			GameController& m_game_controller;
			bool m_send_request;

		private:
			virtual void CompleteImpl(const boost::any& i_value) override;
			virtual void DiscardImpl(const boost::any& i_value) override;

		public:
			ShowHistoryTask(int i_task_id, GameController& i_game_controller);
			virtual ~ShowHistoryTask();

			virtual void Update(long i_elapsed_time) override;
		};

	} // BasePlugin

#endif