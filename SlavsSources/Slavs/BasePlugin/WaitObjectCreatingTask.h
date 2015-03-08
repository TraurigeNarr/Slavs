#ifndef __WAITOBJECTCREATINGTASK_H__
#define __WAITOBJECTCREATINGTASK_H__

#include <SlavsServer/Task.h>

namespace Slavs
	{
	class GameContext;
	} // Slavs

class IController;

namespace BasePlugin
	{

	class WaitObjectCreatingTask : public SDK::Task
		{
		private:
			Slavs::GameContext* mp_context;
			IController*				mp_player_controller;
			int m_object_type;
			bool m_object_created;

		private:
			virtual void CompleteImpl(const boost::any& i_value) override;
			virtual void DiscardImpl(const boost::any& i_value) override;

		public:
			WaitObjectCreatingTask(int i_task_id, int i_object_id, IController* ip_player_controller);
			virtual ~WaitObjectCreatingTask();

			virtual void Update(long i_elapsed_time) override;
		};

	} // BasePlugin

#endif