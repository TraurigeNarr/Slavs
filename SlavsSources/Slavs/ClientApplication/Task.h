#ifndef __TASK_H__
#define __TASK_H__

#include <boost/any.hpp>

namespace SDK
  {

  class Task
    {
    private:
      const int m_task_id;
      bool      m_completed;

    private:
      virtual void CompleteImpl(const boost::any& i_value) = 0;
      virtual void DiscardImpl(const boost::any& i_value) = 0;

    public:
      Task(int i_task_id)
        : m_task_id(i_task_id)
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

      virtual void Complete(const boost::any& i_value)
        {
        m_completed = true;
        CompleteImpl(i_value);
        }
      virtual void Discard(const boost::any& i_value)
        {
        m_completed = true;
        DiscardImpl(i_value);
        }
    };

  typedef std::unique_ptr<Task> TaskPtr;

  }

#endif