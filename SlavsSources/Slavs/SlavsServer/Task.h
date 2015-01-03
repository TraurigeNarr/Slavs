#ifndef __TASK_H__
#define __TASK_H__

#include <boost/any.hpp>

#include <memory>

namespace SDK
	{

	class Task
		{
		private:
			const int m_task_id;
			bool      m_completed;
			bool			m_recurrent;

		private:
			virtual void CompleteImpl(const boost::any& i_value) = 0;
			virtual void DiscardImpl(const boost::any& i_value) = 0;

		public:
			Task(int i_task_id, bool i_recurrent = false)
				: m_task_id(i_task_id)
				, m_recurrent(i_recurrent)
				, m_completed(false)
				{}
			virtual ~Task(){}

			int GetTaskId() const
				{
				return m_task_id;
				}

			bool IsComplete() const
				{
				return m_completed;
				}

			void SetRecurrent(bool i_recurrent) { m_recurrent = i_recurrent; }

			virtual void Update(long i_elapsed_time) {}

			virtual void Complete(const boost::any& i_value)
				{
				if (m_recurrent == false)
					m_completed = true;
				CompleteImpl(i_value);
				}
			virtual void Discard(const boost::any& i_value)
				{
				if (m_recurrent == false)
					m_completed = true;
				DiscardImpl(i_value);
				}
		};

	typedef std::unique_ptr<Task> TaskPtr;

	}

#endif