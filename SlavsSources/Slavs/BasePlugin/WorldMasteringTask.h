#ifndef __WORLDMASTERINGTASK_H__
#define __WORLDMASTERINGTASK_H__

#include <SlavsServer/Task.h>

class BaseObjectComposer;

namespace BasePlugin
	{
	class GameController;

	class WorldMasteringTask : public SDK::Task
		{
		private:
			GameController& m_game_controller;

			std::vector<SDK::TaskPtr> m_tasks;

			long m_time;

		private:
			virtual void CompleteImpl(const boost::any& i_value) override;
			virtual void DiscardImpl(const boost::any& i_value) override;

		public:
			WorldMasteringTask(int i_task_id, GameController& i_game_controller);
			virtual ~WorldMasteringTask();

			virtual void Update(long i_elapsed_time) override;
		};

	} // BasePlugin

#endif