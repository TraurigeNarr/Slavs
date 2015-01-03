#ifndef __PRIMITIVEWINTASK_H__
#define __PRIMITIVEWINTASK_H__

#include <SlavsServer/Task.h>

namespace Slavs
	{
	class GameContext;
	} // Slavs

namespace BasePlugin
	{

	class PrimitiveWinTask : public SDK::Task
		{
		private:
			Slavs::GameContext* mp_context;
			int m_object_type;

			size_t m_clocks;
			bool m_object_created;

		private:
			virtual void CompleteImpl(const boost::any& i_value) override;
			virtual void DiscardImpl(const boost::any& i_value) override;

		public:
			PrimitiveWinTask(int i_task_id, int i_object_id);
			virtual ~PrimitiveWinTask();

			virtual void Update(long i_elapsed_time) override;
		};

	} // BasePlugin

#endif