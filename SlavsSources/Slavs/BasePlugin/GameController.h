#ifndef	__BASEPLUGINGAMECONTROLLER_H__
#define __BASEPLUGINGAMECONTROLLER_H__

#include <SlavsServer/IGameConroller.h>

class BaseObjectComposer;

namespace BasePlugin
	{

	class GameController : public IGameController
		{
		private:
			SDK::GameController* mp_game_controller;
			BaseObjectComposer& m_object_composer;

		public:
			GameController(BaseObjectComposer& i_object_composer);
			virtual ~GameController();
			virtual void GameBegin(SDK::GameController* ip_main_game_controller) override;
			virtual void GameEnd() override;

			SDK::GameController*	GetMainController() { return mp_game_controller; }
			BaseObjectComposer&		GetObjectComposer() { return m_object_composer; }
		};

	} // BasePlugin

#endif