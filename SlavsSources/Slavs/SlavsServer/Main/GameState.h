#pragma once

#include "SlavsServerAPI.h"

#include "Types.h"
#include "GameController.h"

#include <Patterns/StateMachine.h>
#include <Patterns/State.h>

#include <memory>

class ServerMain;
class TimeController;

namespace Slavs
  {

  class GameState : public State<ServerMain, long>
    {
    public:
      typedef std::shared_ptr<TimeController> TimeControllerPtr;
			
			enum State
				{
				Play,
				PlayerDecision,
				};
			typedef StateMachine<GameState, long> GameStateFSM;

    private:
      TGameContext mh_game_context;

      TimeControllerPtr mp_time_controller;
			
			SDK::GameController m_game_controller;
			GameStateFSM				m_game_state_FSM;

    private:
      void                  HoldPacket(ServerMain* ip_owner, unsigned char* ip_packet, size_t i_bytes_read);
			bool									SwitchState(int i_state);

    public:
      GameState(TGameContext ih_game_context);
      ~GameState();

			GameContext* GetContext() const { return mh_game_context.get(); }

    // State
    public:
      virtual void          Enter(ServerMain* ip_owner) override;
      virtual void          Execute(ServerMain* ip_owner, long i_elapsed_time) override;
      virtual void          Exit(ServerMain* ip_owner) override;

    // GameState
    public:
			SDK::GameController& GetGameController() { return m_game_controller; }
			TimeController& GetTimeController() { return *mp_time_controller; }
			SLAVS_SERVER_EXPORT State						GetGameFSMState() const;
    };

  }