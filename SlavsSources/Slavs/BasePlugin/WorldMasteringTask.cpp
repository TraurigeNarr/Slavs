#include "stdafx.h"

#include "WorldMasteringTask.h"

#include "WaitObjectCreatingTask.h"

#include "GameController.h"
#include "BaseObjectComposer.h"
#include "TypeEnumerations.h"

namespace BasePlugin
	{

	WorldMasteringTask::WorldMasteringTask(int i_task_id, GameController& i_game_controller)
		: Task(i_task_id, false)
		, m_game_controller(i_game_controller)
		, m_time(0)
		{
		m_tasks.reserve(5);
		m_tasks.push_back(SDK::TaskPtr(new WaitObjectCreatingTask(0, i_game_controller.GetObjectComposer().GetObjectGlobalID(ObjectType::OT_MANUFACTURE))));
		}

	WorldMasteringTask::~WorldMasteringTask()
		{

		}

	void WorldMasteringTask::CompleteImpl(const boost::any& i_value)
		{

		}

	void WorldMasteringTask::DiscardImpl(const boost::any& i_value)
		{

		}

	void WorldMasteringTask::Update(long i_elapsed_time)
		{
		m_time += i_elapsed_time;
		if (m_time < 100)
			return;
		m_time = 0;

		size_t completeCount = 0;
		for (auto& p_task : m_tasks)
			{
			if (p_task->IsComplete())
				{
				++completeCount;
				continue;
				}
			p_task->Update(i_elapsed_time);
			}
		if (completeCount == m_tasks.size())
			Complete(true);
		}

	} // BasePlugin