#include "stdafx.h"

#include "EndTaskHanlers.h"

#include "WorldMasteringTask.h"

#include "GameController.h"
#include "BaseObjectComposer.h"
#include "TypeEnumerations.h"
#include "HumanComponent.h"
#include "HouseComponent.h"

#include <SlavsServer/Main/GameController.h>
#include <SlavsServer/GameInfo.h>
#include <SlavsServer/Main/GameState.h>
#include <SlavsServer/Game/GameContext.h>
#include <SlavsServer/Game/GameObject.h>
#include <SlavsServer/Main/ServerMain.h>
#include <SlavsServer/include/IController.h>

namespace BasePlugin
	{

	void ShowWorldHistoryHandler(const boost::any& i_value, BasePlugin::GameController& i_game_controller)
		{
		auto p_context = i_game_controller.GetMainController()->GetGameState().GetContext();

		const std::vector<Slavs::GameContext::ControllerInformation>& controllers = p_context->GetControllers();
		IController* p_player_controller = nullptr;
		for (const auto& info : controllers)
			{
			if (info.mp_controller->GetMask() > 100)
				{
				p_player_controller = info.mp_controller.get();
				break;
				}
			}

		auto spawn = p_context->GetSpawn(p_player_controller->GetMask());

		int human_id = i_game_controller.GetObjectComposer().GetObjectGlobalID(ObjectType::OT_HUMAN);
		int hizhina_id = i_game_controller.GetObjectComposer().GetObjectGlobalID(ObjectType::OT_HIZHINA);
		auto p_hizhina = p_context->AddObject(hizhina_id, Vector2D(spawn.m_x, spawn.m_y), p_player_controller->GetMask(), 0);
		auto p_human = p_context->AddObject(human_id, Vector2D(spawn.m_x - 500, spawn.m_y), p_player_controller->GetMask(), 0);
		auto p_human_component = p_human->GetComponent<HumanComponent>();
		assert(p_human_component);
		p_human_component->SetHome(p_hizhina->GetComponent<HouseComponent>());

		SDK::TaskPtr p_task(new WorldMasteringTask(1, i_game_controller, p_player_controller));
		i_game_controller.GetMainController()->RegisterTask(std::move(p_task));
		}

	void ShowVelesHandler(const boost::any& i_value, BasePlugin::GameController& i_game_controller)
		{

		}

	} // BasePluin