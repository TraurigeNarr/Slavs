#ifndef __SENDSELECTIONTASK_H__
#define __SENDSELECTIONTASK_H__

#include "Task.h"

namespace net
	{
	class Connection;
	}

class ClientGameContext;

namespace Tasks
	{

	class SendSelcetionTask : public SDK::Task
		{
		private:
			net::Connection&		m_connection;
			ClientGameContext&	m_context;
			size_t							m_last_selected_number;

		public:
			SendSelcetionTask(int i_task_id, net::Connection& i_connection, ClientGameContext& i_context);
			virtual ~SendSelcetionTask();

			virtual void CompleteImpl(const boost::any& i_value) override;
			virtual void DiscardImpl(const boost::any& i_value) override;
		};

	} // Tasks

#endif