#include "stdafx.h"

#include "GameController.h"

#include "ShowHistoryTask.h"
#include "EndTaskHanlers.h"

#include <SlavsServer/Main/GameController.h>

namespace BasePlugin
	{

	GameController::GameController(BaseObjectComposer& i_composer)
		: mp_game_controller(nullptr)
		, m_object_composer(i_composer)
		{}

	GameController::~GameController()
		{}

	void GameController::GameBegin(SDK::GameController* ip_main_game_controller)
		{
		mp_game_controller = ip_main_game_controller;
		mp_game_controller->RegisterTask(SDK::TaskPtr(new ShowHistoryTask(0, *this, 1, &ShowWorldHistoryHandler)));
		}

	void GameController::GameEnd()
		{
		mp_game_controller = nullptr;
		}

	} // BasePlugin