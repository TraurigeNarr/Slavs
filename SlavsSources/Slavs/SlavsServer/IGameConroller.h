#ifndef __IGAMECONTROLLER_H__
#define __IGAMECONTROLLER_H__

namespace SDK
	{
	class GameController;
	}

namespace Slavs
	{
	class GameState;
	}

class IGameController
	{
	public:
		virtual ~IGameController(){}
		virtual void GameBegin(SDK::GameController* ip_main_game_controller) = 0;
		virtual void GameEnd() = 0;
	};

#endif