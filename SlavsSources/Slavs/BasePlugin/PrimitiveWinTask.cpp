#include "stdafx.h"

#include "PrimitiveWinTask.h"

#include <SlavsServer/Game/GameContext.h>
#include <SlavsServer/Game/GameObject.h>

#include <SlavsServer/Main/ServerMain.h>
#include <SlavsServer/Main/GameState.h>

namespace BasePlugin
	{

	PrimitiveWinTask::PrimitiveWinTask(int i_task_id, int i_object_type)
		: Task(i_task_id, false)
		, mp_context(nullptr)
		, m_object_type(i_object_type)
		, m_clocks(0)
		, m_object_created(false)
		{		}

	PrimitiveWinTask::~PrimitiveWinTask()
		{}

	void PrimitiveWinTask::CompleteImpl(const boost::any& i_value)
		{

		}

	void PrimitiveWinTask::DiscardImpl(const boost::any& i_value)
		{

		}
	
	void PrimitiveWinTask::Update(long i_elapsed_time)
		{
		if (mp_context == nullptr)			
			mp_context = static_cast<Slavs::GameState*>(ServerMain::GetInstance().GetStateMachine().GetCurrentState().get())->GetContext();

		if (m_object_created)
			{
			++m_clocks;
			if (m_clocks == 5)
				Complete(true);
			return;
			}

		auto& objects = mp_context->GetAllObjects();
		for (auto& object_pair : objects)
			{
			Slavs::GameObject* p_object = object_pair.second;
			if (p_object->GetType() == m_object_type)
				{
				m_object_created = true;
				return;
				}
			}
		}

	} // BasePlugin