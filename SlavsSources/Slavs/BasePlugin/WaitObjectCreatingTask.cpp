#include "stdafx.h"

#include "WaitObjectCreatingTask.h"

#include <SlavsServer/Game/GameContext.h>
#include <SlavsServer/Game/GameObject.h>

#include <SlavsServer/Main/ServerMain.h>
#include <SlavsServer/Main/GameState.h>

namespace BasePlugin
	{

	WaitObjectCreatingTask::WaitObjectCreatingTask(int i_task_id, int i_object_type, IController* ip_player_controller)
		: Task(i_task_id, false)
		, mp_context(nullptr)
		, m_object_type(i_object_type)
		, m_object_created(false)
		, mp_player_controller(ip_player_controller)
		{		}

	WaitObjectCreatingTask::~WaitObjectCreatingTask()
		{}

	void WaitObjectCreatingTask::CompleteImpl(const boost::any& i_value)
		{

		}

	void WaitObjectCreatingTask::DiscardImpl(const boost::any& i_value)
		{

		}
	
	void WaitObjectCreatingTask::Update(long i_elapsed_time)
		{
		if (mp_context == nullptr)			
			mp_context = static_cast<Slavs::GameState*>(ServerMain::GetInstance().GetStateMachine().GetCurrentState().get())->GetContext();

		if (m_object_created)
			{
			Complete(true);
			return;
			}

		auto& objects = mp_context->GetAllObjects();
		for (auto& object_pair : objects)
			{
			Slavs::GameObject* p_object = object_pair.second;
			if (p_object->GetType() == m_object_type && p_object->GetController() == mp_player_controller)
				{
				m_object_created = true;
				return;
				}
			}
		}

	} // BasePlugin